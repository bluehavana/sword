/******************************************************************************
 *
 * unicodertf -	SWFilter decendant to convert a double byte unicode file
 *				 to RTF tags
 */


#include <stdlib.h>
#include <stdio.h>
#include <unicodertf.h>

SWORD_NAMESPACE_START

UnicodeRTF::UnicodeRTF() {
}


char UnicodeRTF::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	const unsigned char *from;
	char digit[10];
	short ch;	// must be signed per unicode spec (negative is ok for big numbers > 32768)
	unsigned char from2[7];

	SWBuf orig = text;

	from = (const unsigned char *)orig.c_str();

	// -------------------------------
	for (text = ""; *from; from++) {
		ch = 0;
                //case: ANSI
		if ((*from & 128) != 128) {
			text += *from;
			continue;
		}
                //case: Invalid UTF-8 (illegal continuing byte in initial position)
		if ((*from & 128) && ((*from & 64) != 64)) {
			continue;
		}
                //case: 2+ byte codepoint
		from2[0] = *from;
		from2[0] <<= 1;
		int subsequent;
		for (subsequent = 1; (from[0] & 128) && (subsequent < 7); subsequent++) {
			from2[0] <<= 1;
			from2[subsequent] = from[subsequent];
			from2[subsequent] &= 63;
			ch <<= 6;
			ch |= from2[subsequent];
		}
		subsequent--;
		from2[0] <<= 1;
		char significantFirstBits = 8 - (2+subsequent);
		
		ch |= (((short)from2[0]) << (((6*subsequent)+significantFirstBits)-8));
		from += subsequent;
		text += '\\';
		text += 'u';
		sprintf(digit, "%d", ch);
                text += digit;
		text += '?';
	}
	   
	return 0;
}

SWORD_NAMESPACE_END
