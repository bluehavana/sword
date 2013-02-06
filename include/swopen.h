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

#ifdef __cplusplus
extern "C" {
#endif

int swopen(const char* filename, int flags, int mode);
int swaccess(const char* filename, int mode);
int swremove(const char* filename);
int swmkdir(const char* filename, int mode);
FILE* swfopen(const char* filename, const char *mode);

#ifdef __cplusplus
}
#endif
// swopendir, swrewinddir, swdirclose, swgetenv, swfopen
#endif // SWOPEN_H
