// JX3HDUnpackerL.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <cstring>
#include <cstdio>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <direct.h>
#elif defined(__linux__)
#include <sys/stat.h>
#include <sys/types.h>
#endif
#include <zstd.h>
#include <curl/curl.h>
#include "INIReader.h"
#include "zip.h"
#include "config.h"
// TODO: 在此处引用程序需要的其他标头。

void dounpack(FILE*& fin, const char* idxpath, std::string& datpath, std::string& path);

void download_update(const char** argv);
