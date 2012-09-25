#include "qswutils.h"

#include "qbytearray.h"

QSWUtils::QSWUtils()
{
    // NOP
}

const char * QSWUtils::ToCString(const QString &string) {
    QByteArray byteArray = string.toUtf8();
    const char * cString = byteArray.constData();

    return cString;
}

QString QSWUtils::ToQString(const char *string) {
    QString qString = QString::fromUtf8(string);

    return qString;
}

QString QSWUtils::ToQString(const sword::SWBuf &buffer) {
    return QSWUtils::ToQString(buffer.c_str());
}
