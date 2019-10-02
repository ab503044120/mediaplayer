//
// Created by huihui on 2019/8/19.
//
#include "androidUtils.h"

#ifndef UTILS_H
#define UTILS_H

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

#endif //QLAUNCHER_UTILS_H
