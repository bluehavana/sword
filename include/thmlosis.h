/******************************************************************************
 *
 * $Id: thmlosis.h,v 1.6 2003/02/20 07:25:20 scribe Exp $
 *
 * Copyright 2003 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef THMLOSIS_H
#define THMLOSIS_H

#include <swfilter.h>

SWORD_NAMESPACE_START

/** this filter converts ThML text to OSIS text
 */
class SWDLLEXPORT ThMLOSIS : public SWFilter {
protected:
	virtual const char *convertToOSIS(const char *, const SWKey *key);
public:
	ThMLOSIS();
	virtual ~ThMLOSIS();
	virtual char processText(SWBuf &text, const SWKey *key = 0, const SWModule *module = 0);
};

SWORD_NAMESPACE_END
#endif /* THMLOSIS_H */
