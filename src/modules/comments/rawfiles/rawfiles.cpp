/******************************************************************************
 *  rawfiles.cpp - code for class 'RawFiles'- a module that produces HTML HREFs
 *			pointing to actual text desired.  Uses standard
 *			files:	ot and nt using indexs ??.bks ??.cps ??.vss
 */


#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include <utilfuns.h>
#include <rawverse.h>
#include <rawfiles.h>
#include <filemgr.h>

#ifndef O_BINARY                // O_BINARY is needed in Borland C++ 4.53
#define O_BINARY 0              // If it hasn't been defined than we probably
#endif                          // don't need it.


 /******************************************************************************
 * RawFiles Constructor - Initializes data for instance of RawFiles
 *
 * ENT:	iname - Internal name for module
 *	idesc - Name to display to user for module
 *	idisp	 - Display object to use for displaying
 */

RawFiles::RawFiles(const char *ipath, const char *iname, const char *idesc, SWDisplay *idisp, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : RawVerse(ipath, O_RDWR), SWCom(iname, idesc, idisp, enc, dir, mark, ilang)
{
}


/******************************************************************************
 * RawFiles Destructor - Cleans up instance of RawFiles
 */

RawFiles::~RawFiles()
{
}


/******************************************************************************
 * RawFiles::operator char *	- Returns the correct verse when char * cast
 *					is requested
 *
 * RET: string buffer with verse
 */

char *RawFiles::getRawEntry() {
	FileDesc *datafile;
	long  start = 0;
	unsigned short size = 0;
	char *tmpbuf;
	VerseKey *key = 0;

#ifndef _WIN32_WCE
	try {
#endif
		key = SWDYNAMIC_CAST(VerseKey, this->key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	if (!key)
		key = new VerseKey(this->key);

	findoffset(key->Testament(), key->Index(), &start, &size);

	if (entrybuf)
		delete [] entrybuf;

	if (size) {
		tmpbuf   = new char [ (size + 2) + strlen(path) + 5 ];
		sprintf(tmpbuf,"%s/",path);
		readtext(key->Testament(), start, (size + 2), tmpbuf+strlen(tmpbuf));
		datafile = FileMgr::systemFileMgr.open(tmpbuf, O_RDONLY|O_BINARY);
		delete [] tmpbuf;
		if (datafile->getFd() > 0) {
			size = lseek(datafile->getFd(), 0, SEEK_END);
			entrybuf = new char [ size * FILTERPAD ];
			memset(entrybuf, 0, size * FILTERPAD);
			lseek(datafile->getFd(), 0, SEEK_SET);
			read(datafile->getFd(), entrybuf, size);
//			preptext(entrybuf);
		}
		else {
			entrybuf = new char [2];
			entrybuf[0] = 0;
			entrybuf[1] = 0;
		}
		FileMgr::systemFileMgr.close(datafile);
	}
	else {
		entrybuf = new char [2];
		entrybuf[0] = 0;
		entrybuf[1] = 0;
	}

	if (key != this->key)
		delete key;

	return entrybuf;
}


/******************************************************************************
 * RawFiles::operator << (char *)- Update the modules current key entry with
 *				provided text
 *
 * RET: *this
 */

SWModule &RawFiles::operator <<(const char *inbuf) {
	FileDesc *datafile;
	long  start;
	unsigned short size;
	char *tmpbuf;
	VerseKey *key = 0;

#ifndef _WIN32_WCE
	try {
#endif
		key = SWDYNAMIC_CAST(VerseKey, this->key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	if (!key)
		key = new VerseKey(this->key);

	findoffset(key->Testament(), key->Index(), &start, &size);

	if (size) {
		tmpbuf   = new char [ (size + 2) + strlen(path) + 1 ];
		sprintf(tmpbuf, "%s/", path);
		readtext(key->Testament(), start, (size + 2), tmpbuf+strlen(tmpbuf));
	}
	else {
		tmpbuf   = new char [ 16 + strlen(path) + 1 ];
		sprintf(tmpbuf, "%s/%s", path, getnextfilename());
		settext(key->Testament(), key->Index(), tmpbuf+strlen(path)+1);
	}
	datafile = FileMgr::systemFileMgr.open(tmpbuf, O_CREAT|O_WRONLY|O_BINARY|O_TRUNC);
	delete [] tmpbuf;
	if (datafile->getFd() > 0) {
		write(datafile->getFd(), inbuf, strlen(inbuf));
	}
	FileMgr::systemFileMgr.close(datafile);
	
	if (key != this->key)
		delete key;

	return *this;
}


/******************************************************************************
 * RawFiles::operator << (SWKey *)- Link the modules current key entry with
 *				another module entry
 *
 * RET: *this
 */

SWModule &RawFiles::operator <<(const SWKey *inkey) {

	long  start;
	unsigned short size;
	char *tmpbuf;
	const VerseKey *key = 0;

#ifndef _WIN32_WCE
	try {
#endif
		key = SWDYNAMIC_CAST(VerseKey, inkey);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	if (!key)
		key = new VerseKey(this->key);

	findoffset(key->Testament(), key->Index(), &start, &size);

	if (size) {
		tmpbuf   = new char [ size + 2];
		readtext(key->Testament(), start, size + 2, tmpbuf);

		if (key != inkey)
			delete key;
		key = 0;

#ifndef _WIN32_WCE
		try {
#endif
			key = SWDYNAMIC_CAST(VerseKey, inkey);
#ifndef _WIN32_WCE
		}
		catch ( ... ) {}
#endif
		if (!key)
			key = new VerseKey(this->key);
		settext(key->Testament(), key->Index(), tmpbuf);
	}
	
	if (key != inkey)
		delete key;

	return *this;
}


/******************************************************************************
 * RawFiles::deleteEntry	- deletes this entry
 *
 * RET: *this
 */

void RawFiles::deleteEntry() {

	VerseKey *key = 0;

#ifndef _WIN32_WCE
	try {
#endif
		key = SWDYNAMIC_CAST(VerseKey, this->key);
#ifndef _WIN32_WCE
	}
	catch ( ... ) {}
#endif
	if (!key)
		key = new VerseKey(this->key);

	settext(key->Testament(), key->Index(), "");

	if (key != this->key)
		delete key;
}


/******************************************************************************
 * RawFiles::getnextfilename - generates a valid filename in which to store
 *				an entry
 *
 * RET: filename
 */

char *RawFiles::getnextfilename() {
	static char incfile[255];
	long number;
	FileDesc *datafile;

	sprintf(incfile, "%s/incfile", path);
	datafile = FileMgr::systemFileMgr.open(incfile, O_RDONLY|O_BINARY);
	if (read(datafile->getFd(), &number, 4) != 4)
		number = 0;
	number++;
	FileMgr::systemFileMgr.close(datafile);
	
	datafile = FileMgr::systemFileMgr.open(incfile, O_CREAT|O_WRONLY|O_BINARY|O_TRUNC);
	write(datafile->getFd(), &number, 4);
	FileMgr::systemFileMgr.close(datafile);
	sprintf(incfile, "%.7ld", number-1);
	return incfile;
}


char RawFiles::createModule (const char *path) {
	char *incfile = new char [ strlen (path) + 16 ];
    static long zero = 0;
	FileDesc *datafile;

	sprintf(incfile, "%s/incfile", path);
	datafile = FileMgr::systemFileMgr.open(incfile, O_CREAT|O_WRONLY|O_BINARY|O_TRUNC);
    delete [] incfile;
	write(datafile->getFd(), &zero, 4);
	FileMgr::systemFileMgr.close(datafile);

    return RawVerse::createModule (path);
}



