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

#include <swbuf.h>
#include <utf8utf16.h>

wchar_t* _str_to_utf16(const char *str) {
    sword::UTF8UTF16 *filter = new sword::UTF8UTF16();
    sword::SWBuf swStr = str;
    filter->processText(swStr, NULL, NULL);
    delete filter;

    return (wchar_t*) swStr.c_str();
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
swopen(const char   *filename,
       int          flags,
       int          mode) {
#ifdef WIN32
    HANDLE hFile;
    DWORD  dwDesiredAccess      = 0;
    DWORD  dwFlagsAndAttributes = 0;
    DWORD  dwDisposition        = OPEN_EXISTING;
    DWORD  dwSharedAccess       = FILE_SHARE_READ | FILE_SHARE_DELETE;
    // Convert name to UTF-16 for file paths
    wchar_t* wfilename = _str_to_utf16(filename);
    int retval;
    int save_errno;

    if (wfilename == NULL) {
        errno = EINVAL;
        return -1;
    }

    // Set up the access modes and other attributes
    if ((flags & _O_CREAT) && (mode & _S_IREAD)) {
        if (! (mode & _S_IWRITE))
            dwFlagsAndAttributes    = FILE_ATTRIBUTE_READONLY; // Sets file to 'read only' after the file gets closed
    }
    if ( !(flags & _O_ACCMODE)) {
        // Equates to _O_RDONLY
        if (flags & _O_TRUNC) {
            errno = EINVAL;
        }

        dwDesiredAccess |= GENERIC_READ;
        dwSharedAccess  |= FILE_SHARE_WRITE;
    }
    if (flags & _O_WRONLY) {
        if (flags & _O_RDWR) {
            errno = EINVAL;
        }

        dwDesiredAccess |= GENERIC_WRITE;
    }
    if (flags & _O_RDWR) {
        dwDesiredAccess |= GENERIC_READ;
        dwDesiredAccess |= GENERIC_WRITE;
    }
    if (flags & _O_TRUNC) {
        if (flags & _O_CREAT)
            dwDisposition = CREATE_ALWAYS;
        else
            dwDisposition = TRUNCATE_EXISTING;
    }
    if ((flags & _O_CREAT) && !(flags & _O_TRUNC)) {
        if (flags & _O_EXCL)
            dwDisposition = CREATE_NEW;
        else
            dwDisposition = OPEN_ALWAYS;
    }
    if (flags & _O_CREAT) {
        // Handle the other flags that can be attached to _O_CREAT
        if ((flags & _O_TEMPORARY) || (flags & _O_SHORT_LIVED))
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_TEMPORARY;

        if (flags & _O_TEMPORARY)
            dwFlagsAndAttributes |= FILE_FLAG_DELETE_ON_CLOSE;
    }
    if ((flags & _O_SEQUENTIAL) || (flags & _O_APPEND)) {
        dwFlagsAndAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
    }
    else if (flags & _O_RANDOM) {
        dwFlagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;
    }

    if (0 == dwFlagsAndAttributes) {
        dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    }
    hFile = CreateFileW(wfilename, dwDesiredAccess, dwSharedAccess, NULL, dwDisposition, dwFlagsAndAttributes, NULL);

    if (INVALID_HANDLE_VALUE == hFile) {
        retval = (-1);

        switch (GetLastError()) {
#define CASE(a,b) case ERROR_##a: errno = b; break
            CASE (FILE_NOT_FOUND, ENOENT);
            CASE (PATH_NOT_FOUND, ENOENT);
            CASE (ACCESS_DENIED, EACCES);
            CASE (NOT_SAME_DEVICE, EXDEV);
            CASE (LOCK_VIOLATION, EACCES);
            CASE (SHARING_VIOLATION, EACCES);
            CASE (FILE_EXISTS, EEXIST);
            CASE (ALREADY_EXISTS, EEXIST);
#undef CASE
            default: errno = EIO;
        }
    } else {
        retval = _open_osfhandle((long)hFile, flags);
    }

    if ((-1) != retval) {
        // We have a valid file handle. Set binary/text mode as appropriate
        if ((!(flags & _O_TEXT)) && (_fmode == _O_BINARY))
            _setmode(retval, _O_BINARY);
        else if ((flags & _O_TEXT) || (_fmode == _O_TEXT))
            _setmode(retval, _O_TEXT);
        else
            _setmode(retval, _O_BINARY);
    }

    save_errno = errno;
    errno = save_errno;

    return retval;
#else
    int fd;
    do
        fd = open (filename, flags, mode);
    while ( fd == -1 && errno == EINTR);
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
extern "C" int swaccess(const char *filename, int mode) {
#ifdef WIN32
    wchar_t *wfilename = _str_to_utf16(filename);
    int retval;

    if (wfilename == NULL) {
        errno = EINVAL;
        return -1;
    }

#ifndef X_OK
#define X_OK 1
#endif

    retval = _waccess (wfilename, mode & ~X_OK);

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
int swremove (const char *filename) {
#ifdef WIN32
    wchar_t *wfilename = _str_to_utf16(filename);
    int retval;

    if (wfilename == NULL) {
        errno = EINVAL;
        return -1;
    }

    retval = _wremove (wfilename);
    if (retval == -1)
        retval = _wrmdir (wfilename);
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
extern "C" int swmkdir(const char *filename, int mode) {
#ifdef WIN32
    wchar_t* wfilename = _str_to_utf16(filename);
    int retval;

    if (wfilename == NULL) {
        errno = EINVAL;
        return -1;
    }

    retval = _wmkdir (wfilename);

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
extern "C" FILE* swfopen(const char *filename, const char* mode) {
#ifdef WIN32
  int   hFile;
  int   flags  = 0;
  char priv_mode[4];
  FILE *retval = NULL;

  if ((NULL == filename) || (NULL == mode))
  {
    errno = EINVAL;
    goto out;
  }
  if ((strlen(mode) < 1) || (strlen(mode) > 3))
  {
    errno = EINVAL;
    goto out;
  }

  strncpy(priv_mode, mode, 3);
  priv_mode[3] = '\0';

  /* Set up any flags to pass to 'swopen()' */
  if (3 == strlen(priv_mode))
  {
    if (('c' == priv_mode[2]) || ('n' == priv_mode[2]))
      priv_mode[2] = '\0';
    else
    {
      if (0 == strcmp(priv_mode, "a+b"))
        flags = _O_RDWR | _O_CREAT | _O_APPEND | _O_BINARY;
      else if (0 == strcmp(priv_mode, "a+t"))
        flags = _O_RDWR | _O_CREAT | _O_APPEND | _O_TEXT;
      else if (0 == strcmp(priv_mode, "r+b"))
        flags = _O_RDWR | _O_BINARY;
      else if (0 == strcmp(priv_mode, "r+t"))
        flags = _O_RDWR | _O_TEXT;
      else if (0 == strcmp(priv_mode, "w+b"))
        flags = _O_RDWR | _O_CREAT |_O_TRUNC | _O_BINARY;
      else if (0 == strcmp(priv_mode, "w+t"))
        flags = _O_RDWR | _O_CREAT |_O_TRUNC | _O_TEXT;
      else
      {
        errno = EINVAL;
        goto out;
      }
    }
  }
  if (2 == strlen(priv_mode))
  {
    if (('c' == priv_mode[1]) || ('n' == priv_mode[1]))
      priv_mode[1] = '\0';
    else
    {
      if (0 == strcmp(priv_mode, "a+"))
        flags = _O_RDWR | _O_CREAT | _O_APPEND;
      else if (0 == strcmp(priv_mode, "ab"))
        flags = _O_WRONLY | _O_CREAT | _O_APPEND | _O_BINARY;
      else if (0 == strcmp(priv_mode, "at"))
        flags = _O_WRONLY | _O_CREAT | _O_APPEND | _O_TEXT;
      else if (0 == strcmp(priv_mode, "rb"))
        flags = _O_RDONLY | _O_BINARY;
      else if (0 == strcmp(priv_mode, "rt"))
        flags = _O_RDONLY | _O_TEXT;
      else if (0 == strcmp(priv_mode, "wb"))
        flags = _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY;
      else if (0 == strcmp(priv_mode, "wt"))
        flags = _O_WRONLY | _O_CREAT | _O_TRUNC | _O_TEXT;
      else
      {
        errno = EINVAL;
        goto out;
      }
    }
  }
  if (1 == strlen(priv_mode))
  {
    if (0 == strcmp(priv_mode, "a"))
      flags = _O_WRONLY | _O_CREAT | _O_APPEND;
    else if (0 == strcmp(priv_mode, "r"))
      flags = _O_RDONLY;
    else if (0 == strcmp(priv_mode, "w"))
      flags = _O_WRONLY | _O_CREAT | _O_TRUNC;
    else if ( !((0 == strcmp(priv_mode, "c")) || (0 == strcmp(priv_mode, "n"))))
    {
      errno = EINVAL;
      goto out;
    }
  }

  hFile = swopen (filename, flags, (_S_IREAD | _S_IWRITE));

  if (INVALID_HANDLE_VALUE == (HANDLE)hFile)
    /* 'errno' will have already been set by 'swopen()' */
    retval = NULL;
  else
    retval = _fdopen(hFile, mode);

  out:
  return retval;
#else
  return fopen (filename, mode);
#endif
}
