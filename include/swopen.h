/******************************************************************************
*  swopen.h  - declarations for sword file handling methods
*
* $Id$
*
* Copyright 2013 CrossWire Bible Society (http://www.crosswire.org)
*       CrossWire Bible Society
*       P. O. Box 2528
*       Tempe, AZ  85280-2528
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation version 2.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
*/
#ifndef SWOPEN_H
#define SWOPEN_H

#include <stdio.h>

typedef struct _SWDir SWDir;

#ifdef __cplusplus
extern "C" {
#endif

int sw_open(const char* filename, int flags, int mode);
int sw_access(const char* filename, int mode);
int sw_remove(const char* filename);
int sw_mkdir(const char* filename, int mode);
FILE* sw_fopen(const char* filename, const char *mode);

SWDir* sw_dir_open (const char* path);
void sw_dir_rewind(SWDir *dir);
void sw_dir_close(SWDir *dir);
const char* sw_dir_read_name(SWDir *dir);

const char* sw_getenv(const char* variable);
#ifdef __cplusplus
}
#endif
// swgetenv
#endif // SWOPEN_H
