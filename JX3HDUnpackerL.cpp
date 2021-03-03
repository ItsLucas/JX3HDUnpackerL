
#include "JX3HDUnpackerL.h"

using namespace std;

long long offset[500010];
long zsize[500010];
short dataflag[500010];

string versionString = "0.0.3";

string cmdString = "luac -p ";

void writefile(char* content, string filename, string base,
    unsigned long long sz) {
    FILE* fout;
    if ((fout = fopen(filename.c_str(), "wb")) == NULL) {
        printf("\n");
        printf("Cannot open output file %s\n", filename.c_str());
        exit(1);
    }
    fwrite(content, sz, 1, fout);
    fclose(fout);
#if defined(__WIN32__)
    return;
#elif defined(__linux__)
    string cmd = cmdString + filename + "> /dev/null 2>&1";
    if (WEXITSTATUS(system(cmd.c_str())) == 0) {
        stringstream ss;
        ss << filename;
        ss << ".lua";
        rename(filename.c_str(), ss.str().c_str());
    }
#endif
}

void local_unpack(string _datpath) {
    FILE* fin;
    string path = "./";
    string datpath = _datpath;
    char* idxpath = (char*)malloc(sizeof(char) * 10);
    strncpy(idxpath, datpath.c_str(), datpath.length() - 4);
    path.append(idxpath);
    path.append("/");
    strcat(idxpath, ".idx");
    if ((fin = fopen(idxpath, "rb")) == NULL) {
        printf("Cannot open idx file\n");
        exit(1);
    }
    fseek(fin, 0, SEEK_END);
    int chunknum = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    chunknum -= 0x10;
    chunknum /= 0x1F;
    fseek(fin, 0x10, SEEK_CUR);
    cout << "[Unpacker] Chunknum = " << chunknum << endl;
    for (int i = 1; i <= chunknum; i++) {
        printf("[Unpacker] IDX read Progress %d/%d....\r", i, chunknum);
        fread(&offset[i], 8, 1, fin);
        fread(&zsize[i], 4, 1, fin);
        fread(&dataflag[i], 2, 1, fin);
        fseek(fin, 17, SEEK_CUR);
        // cout << "[FIN] tellg=" << fin.tellg() << endl;
        // cout << "[Unpacker] chunk " << i << ": " << tmp1 << " " << tmp2 << "
        // "
        //<< tmp3 << endl;
    }
    cout << "\n[Unpacker] IDX read finish" << endl;
    fclose(fin);
    if ((fin = fopen(datpath.c_str(), "rb")) == NULL) {
        printf("Cannot open dat file\n");
        exit(1);
    }
    fseek(fin, 0x10, SEEK_CUR);
#if defined(__WIN32__)
    if (_mkdir(path.c_str())) {
        printf("cannot create path %s\n", path.c_str());
        exit(1);
    }
#elif defined(__linux__)
    struct stat st = { 0 };
    if (stat(path.c_str(), &st) == -1) {
        mkdir(path.c_str(), 0700);
    }
    else {
        printf("cannot create path %s\n", path.c_str());
    }
#endif
    for (int i = 1; i <= chunknum; i++) {
        printf("[Unpacker] Extracting chunk %d....\r", i);
        stringstream ss;
        ss << path << "Chunk_" << i << "_";
        string base = ss.str();
        fseek(fin, offset[i], SEEK_SET);
        fseek(fin, 0x17, SEEK_CUR);
        int off = ftell(fin);
        int checklong;
        fread(&checklong, 4, 1, fin);
        char* s = (char*)malloc(zsize[i]);
        if (checklong == 0xFD2FB528) {
            ss << "c.dat";
            fseek(fin, off, SEEK_SET);
            fread(s, zsize[i], 1, fin);
            unsigned long long decompsize =
                ZSTD_getFrameContentSize(s, zsize[i]);
            unsigned long long elementsize = decompsize;
            if (decompsize > 0xFFFFFFFFFFFFFFFDULL) {
                cout << "[ERROR] invalid file size" << endl;
                free(s);
                exit(1);
            }
            char* decomped = (char*)malloc(decompsize);
            ZSTD_decompress(decomped, elementsize, s, zsize[i]);
            free(s);
            writefile(decomped, ss.str(), base, elementsize);
            free(decomped);
        }
        else {
            ss << "u.dat";
            fseek(fin, off, SEEK_SET);
            fread(s, zsize[i], 1, fin);
            writefile(s, ss.str(), base, zsize[i]);
            free(s);
        }
    }
    cout << endl;
    cout << "[Unpacker] JB finished" << endl;
}

void remote_unpack(string apath) {
    FILE* fin;
    stringstream pss;
    pss << "./" << apath << "/";
    string path = pss.str();
    string datpath = "000.dat";
    const char* idxpath = "000.idx";
    if ((fin = fopen(idxpath, "rb")) == NULL) {
        printf("Cannot open idx file\n");
        exit(1);
    }
    fseek(fin, 0, SEEK_END);
    int chunknum = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    chunknum -= 0x10;
    chunknum /= 0x1F;
    fseek(fin, 0x10, SEEK_CUR);
    cout << "[Unpacker] Chunknum = " << chunknum << endl;
    for (int i = 1; i <= chunknum; i++) {
        printf("[Unpacker] IDX read Progress %d/%d....\r", i, chunknum);
        fread(&offset[i], 8, 1, fin);
        fread(&zsize[i], 4, 1, fin);
        fread(&dataflag[i], 2, 1, fin);
        fseek(fin, 17, SEEK_CUR);
        // cout << "[FIN] tellg=" << fin.tellg() << endl;
        // cout << "[Unpacker] chunk " << i << ": " << tmp1 << " " << tmp2 << "
        // "
        //<< tmp3 << endl;
    }
    cout << "\n[Unpacker] IDX read finish" << endl;
    fclose(fin);
    if ((fin = fopen(datpath.c_str(), "rb")) == NULL) {
        printf("Cannot open dat file\n");
        exit(1);
    }
    fseek(fin, 0x10, SEEK_CUR);
#if defined(__WIN32__)
    if (_mkdir(path.c_str())) {
        printf("cannot create path %s\n", path.c_str());
        exit(1);
    }
#elif defined(__linux__)
    struct stat st = { 0 };
    if (stat(path.c_str(), &st) == -1) {
        mkdir(path.c_str(), 0700);
    }
    else {
        printf("cannot create path %s\n", path.c_str());
    }
#endif
    for (int i = 1; i <= chunknum; i++) {
        printf("[Unpacker] Extracting chunk %d....\r", i);
        stringstream ss;
        ss << path << "Chunk_" << i << "_";
        string base = ss.str();
        fseek(fin, offset[i], SEEK_SET);
        fseek(fin, 0x17, SEEK_CUR);
        int off = ftell(fin);
        int checklong;
        fread(&checklong, 4, 1, fin);
        char* s = (char*)malloc(zsize[i]);
        if (checklong == 0xFD2FB528) {
            ss << "c.dat";
            fseek(fin, off, SEEK_SET);
            fread(s, zsize[i], 1, fin);
            unsigned long long decompsize =
                ZSTD_getFrameContentSize(s, zsize[i]);
            unsigned long long elementsize = decompsize;
            if (decompsize > 0xFFFFFFFFFFFFFFFDULL) {
                cout << "[ERROR] invalid file size" << endl;
                free(s);
                exit(1);
            }
            char* decomped = (char*)malloc(decompsize);
            ZSTD_decompress(decomped, elementsize, s, zsize[i]);
            free(s);
            writefile(decomped, ss.str(), base, elementsize);
            free(decomped);
        }
        else {
            ss << "u.dat";
            fseek(fin, off, SEEK_SET);
            fread(s, zsize[i], 1, fin);
            writefile(s, ss.str(), base, zsize[i]);
            free(s);
        }
    }
    cout << endl;
    cout << "[Unpacker] JB finished" << endl;
}
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main(int argc, const char** argv) {
    if (argc < 3) {
        // cout << WEXITSTATUS(system("luac -p ./000/Chunk_1811_c.dat")) <<
        // endl;
        cout << "JX3HD Cross-Platform Unpacker v" << versionString << endl;
        cout << "Usage: ./JX3HDUnpacker -l [dat-file] or -r [patchNum]" << endl;

        return 0;
    }
    if (strcmp(argv[1], "-l") == 0) {
        local_unpack(argv[2]);
    }
    else if (strcmp(argv[1], "-r") == 0) {
        CURL* curl = curl_easy_init();
        if (curl) {
            CURLcode res;
            FILE* fp;
            fp = fopen("autoupdateentry.txt", "w");
            curl_easy_setopt(curl, CURLOPT_URL, "https://jx3hdv4.autoupdate.kingsoft.com/jx3hd_v4/zhcn_hd/autoupdateentry.txt");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);
            INIReader reader("autoupdateentry.txt");
            if (reader.ParseError() < 0) {
                cout << "[Downloader] Update entry parse failed" << endl;
                exit(1);
            }
            stringstream ss;
            ss << "Patch_";
            ss << atoi(argv[2]);
            string url = reader.GetString("PatchList", ss.str(), "error");

            if (url == "error") {
                cout << "[Downloader] No such patch!" << endl;
                exit(1);
            }
            else {
                cout << "[Downloader] Downloading patch " << url << endl;
                stringstream ss2;
                ss2 << "https://jx3hdv4.autoupdate.kingsoft.com/jx3hd_v4/zhcn_hd/";
                ss2 << url;
                //cout << ss2.str() << endl;
                CURLcode res;
                FILE* fp;
                CURL* curl = curl_easy_init();
                fp = fopen(url.c_str(), "wb");
                curl_easy_setopt(curl, CURLOPT_URL, ss2.str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_FILE, fp);
                res = curl_easy_perform(curl);
                //cout << res << endl;
                curl_easy_cleanup(curl);
                fclose(fp);
                bool havedat = false;
                bool haveidx = false;
                struct zip_t* zip = zip_open(url.c_str(), 0, 'r');
                int i, n = zip_total_entries(zip);
                for (i = 0; i < n; ++i) {
                    zip_entry_openbyindex(zip, i);
                    {
                        const char* name = zip_entry_name(zip);
                        if (strstr(name, "000.dat")) {
                            havedat = true;
                            cout << "[Downloader] Found 000.dat, unzipping" << endl;
                            zip_entry_fread(zip, "000.dat");
                        }
                        if (strstr(name, "000.idx")) {
                            haveidx = true;
                            cout << "[Downloader] Found 000.idx, unzipping" << endl;
                            zip_entry_fread(zip, "000.idx");
                        }
                    }
                    zip_entry_close(zip);
                    if (havedat && haveidx) break;
                }
                zip_close(zip);
                if (havedat) {
                    remote_unpack(ss.str());
                }
            }
        }
    }

    return 0;
}
