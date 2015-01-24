#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QString>
#include <QStringList>

class CommonUtils
{
public:
    CommonUtils();
    ~CommonUtils();

    static char* QStringToChars(QString str);
    static QStringList ParseChannelNames(QString channelBlob);
};

#endif // COMMONUTILS_H
