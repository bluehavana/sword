/******************************************************************************
*  swopen.cpp  - code for swopen to wrap file opening
*
* $Id$
*
* Copyright 2013 CrossWire Bible Society (http://www.crosswire.org)
*	CrossWire Bible Society
*	P. O. Box 2528
*	Tempe, AZ  85280-2528
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

#include "swopen.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#include <errno.h>
#include <wchar.h>
#include <direct.h>
#include <io.h>
#include <sys/utime.h>
#else
#include <unistd.h>
#include <errno.h>
#endif

#include <dirent.h>

#include <swbuf.h>
#include <utf8utf16.h>
#include <utf16utf8.h>

struct _SWDir
{
#ifdef WIN32
  _WDIR *wdirp;
#else
  DIR *dirp;
#endif
#ifdef WIN32
  char utf8_buf[FILENAME_MAX*4];
#endif
};

wchar_t* _str_to_utf16(const char *str) {
    sword::UTF8UTF16 *filter = new sword::UTF8UTF16();
    sword::SWBuf swStr = str;
    filter->processText(swStr, NULL, NULL);
    delete filter;

    return (wchar_t*) swStr.c_str();
}

char* _utf16_to_utf8(const wchar_t *str) {
    sword::UTF16UTF8 *filter = new sword::UTF16UTF8();
    sword::SWBuf swStr = (char*)str;
    filter->processText(swStr, NULL, NULL);
    delete filter;

    return (char*) swStr.c_str();
}

/**
 * @brief cross-platform method for opening a file
 *
 * This wraps either a simple call to the clib to open a file or
 * to the wide-character API on Windows to open a file
 *
 * Arguments and return are the same as the standard clib open
 * arguments.
 *
 * @param filename
 * @param flags
 * @param mode
 * @return
 */
extern "C" int
sw_open(const char   *filename,
       int          flags,
       int          mode) {
#ifdef WIN32
    wchar_t *wfilename = _str_to_utf16 (filename);
    int retval;
    int save_errno;

    if (wfilename == NULL)
      {
        errno = EINVAL;
        return -1;
      }

    retval = _wopen (wfilename, flags, mode);
    save_errno = errno;

    errno = save_errno;
    return retval;
#else
    int fd;
    do
      fd = open (filename, flags, mode);
    while (fd == -1 && errno == EINTR);
    return fd;
#endif
}

/**
 * @brief cross-platform method for determining read, write, execute, etc permission
 *
 * The arguments and return values are the same as they would be to the access method
 * in your standard clib.
 *
 * This wraps the wide-character API calls on Windows.
 *
 * @param filename
 * @param mode
 * @return
 */
extern "C" int sw_access(const char *filename, int mode) {
#ifdef WIN32
    wchar_t *wfilename = _str_to_utf16 (filename);
    int retval;
    int save_errno;

    if (wfilename == NULL)
      {
        errno = EINVAL;
        return -1;
      }

  #ifndef X_OK
  #define X_OK 1
  #endif

    retval = _waccess (wfilename, mode & ~X_OK);
    save_errno = errno;

    errno = save_errno;
    return retval;
#else
    return access (filename, mode);
#endif
}

/**
 * @brief cross-platform method for removing a file or directory
 *
 * Arguments are the same as the standard clib arguments to the
 * access method. Will either call that method in a UTF-8 friendly
 * environment or call the wide character APIs on Windows.
 *
 * @param filename
 * @return
 */
int sw_remove (const char *filename) {
#ifdef WIN32
    wchar_t *wfilename = _str_to_utf16 (filename);
    int retval;
    int save_errno;

    if (wfilename == NULL)
      {
        errno = EINVAL;
        return -1;
      }

    retval = _wremove (wfilename);
    if (retval == -1)
      retval = _wrmdir (wfilename);
    save_errno = errno;

    errno = save_errno;
    return retval;
#else
    return remove (filename);
#endif
}

/**
 * @brief cross-platform method for creating a directory
 *
 * Accepts the same arguments as the clib mkdir method. On
 * Windows this will invoke the wide character API.
 *
 * @param filename
 * @param mode
 * @return
 */
extern "C" int sw_mkdir(const char *filename, int mode) {
#ifdef WIN32
    wchar_t *wfilename = _str_to_utf16 (filename);
    int retval;
    int save_errno;

    if (wfilename == NULL)
      {
        errno = EINVAL;
        return -1;
      }

    retval = _wmkdir (wfilename);
    save_errno = errno;

    errno = save_errno;
    return retval;
#else
    return mkdir (filename, mode);
#endif
}

/**
 * @brief cross-platform method for opening a file
 *
 * @param filename
 * @param mode
 * @return
 */
extern "C" FILE* sw_fopen(const char *filename, const char* mode) {
#ifdef WIN32
    wchar_t *wfilename = _str_to_utf16 (filename);
    wchar_t *wmode;
    FILE *retval;
    int save_errno;

    if (wfilename == NULL)
      {
        errno = EINVAL;
        return NULL;
      }

    wmode = _str_to_utf16 (mode);

    if (wmode == NULL)
      {
        delete wfilename;
        errno = EINVAL;
        return NULL;
      }

    retval = _wfopen (wfilename, wmode);
    save_errno = errno;

    errno = save_errno;
    return retval;
  #else
    return fopen (filename, mode);
  #endif
}

/**
 * @brief cross-plaform method for opening a directory
 */
extern "C" SWDir *
sw_dir_open (const char  *path)
{
  SWDir *dir;
#ifdef WIN32
  wchar_t *wpath;
#endif

  if(path == NULL) return NULL;

#ifdef WIN32
  wpath = _str_to_utf16 (path);

  if (wpath == NULL)
    return NULL;

  dir = new SWDir;

  dir->wdirp = _wopendir (wpath);

  if (dir->wdirp)
    return dir;

  return NULL;
#else
  dir = new SWDir;

  dir->dirp = opendir (path);

  if (dir->dirp)
    return dir;

  return NULL;
#endif
}

void
sw_dir_rewind(SWDir *dir) {
    if(dir == NULL) return;

#ifdef WIN32
  _wrewinddir (dir->wdirp);
#else
  rewinddir (dir->dirp);
#endif
}

void
sw_dir_close (SWDir *dir)
{
    if(dir == NULL) return;

#ifdef WIN32
  _wclosedir (dir->wdirp);
#else
  closedir (dir->dirp);
#endif
  delete dir;
}

const char *
sw_dir_read_name (SWDir *dir)
{
#ifdef WIN32
  char *utf8_name;
  struct _wdirent *wentry;
#else
  struct dirent *entry;
#endif

  if(dir == NULL) return NULL;

#ifdef WIN32
  while (1)
    {
      wentry = _wreaddir (dir->wdirp);
      while (wentry
         && (0 == wcscmp (wentry->d_name, L".") ||
         0 == wcscmp (wentry->d_name, L"..")))
    wentry = _wreaddir (dir->wdirp);

      if (wentry == NULL)
    return NULL;

      utf8_name = _utf16_to_utf8 (wentry->d_name);

      if (utf8_name == NULL)
    continue;		/* Huh, impossible? Skip it anyway */

      strcpy (dir->utf8_buf, utf8_name);

      return dir->utf8_buf;
    }
#else
  entry = readdir (dir->dirp);
  while (entry
         && (0 == strcmp (entry->d_name, ".") ||
             0 == strcmp (entry->d_name, "..")))
    entry = readdir (dir->dirp);

  if (entry)
    return entry->d_name;
  else
    return NULL;
#endif
}

const char* sw_getenv(const char *variable) {
    if(variable == NULL) return NULL;
#ifdef WIN32
    char *value;
    wchar_t dummy[2], *wname, *wvalue;
    int len;

    //g_return_val_if_fail (g_utf8_validate (variable, -1, NULL), NULL);

    /* On Windows NT, it is relatively typical that environment
     * variables contain references to other environment variables. If
     * so, use ExpandEnvironmentStrings(). (In an ideal world, such
     * environment variables would be stored in the Registry as
     * REG_EXPAND_SZ type values, and would then get automatically
     * expanded before a program sees them. But there is broken software
     * that stores environment variables as REG_SZ values even if they
     * contain references to other environment variables.)
     */

    wname = _str_to_utf16 (variable);

    len = GetEnvironmentVariableW (wname, dummy, 2);

    if (len == 0)
      {
        delete wname;
        if (GetLastError () == ERROR_ENVVAR_NOT_FOUND)
          return NULL;

        return "";
      }
    else if (len == 1)
      len = 2;

    wvalue = new wchar_t[len];

    if (GetEnvironmentVariableW (wname, wvalue, len) != len - 1)
      {
        delete wname;
        delete wvalue;
        return NULL;
      }

    if (wcschr (wvalue, L'%') != NULL)
      {
        wchar_t *tem = wvalue;

        len = ExpandEnvironmentStringsW (wvalue, dummy, 2);

        if (len > 0)
          {
            wvalue = new wchar_t[len];

            if (ExpandEnvironmentStringsW (tem, wvalue, len) != len)
              {
                delete wvalue;
                wvalue = tem;
              }
            else
              delete tem;
          }
      }

    value = _utf16_to_utf8 (wvalue);

    delete wname;
    delete wvalue;

    return value;
#else
    return getenv(variable);
#endif
}
