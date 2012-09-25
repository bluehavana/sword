#ifndef QSWUTILS_H
#define QSWUTILS_H

#include "qstring.h"
#include "swbuf.h"

class QSWUtils
{
public:
    static const char* ToCString(const QString &string);

    static QString ToQString(const char* string);
    static QString ToQString(const sword::SWBuf& buffer);

private:
    QSWUtils();
};

#endif // QSWUTILS_H
