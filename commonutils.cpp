#include "commonutils.h"
#include <QString>
#include <QByteArray>
#include <QDebug>

CommonUtils::CommonUtils()
{

}

CommonUtils::~CommonUtils()
{

}

char* CommonUtils::QStringToChars(QString str)
{
    QByteArray qba = str.toUtf8();
    char* cStr = qba.data();
    return cStr;
}

QStringList CommonUtils::ParseChannelNames(QString channelBlob)
{
    channelBlob.remove('\n').remove('\r').remove(' ');
    QStringList channelNames = channelBlob.split(',');

    return channelNames;
}
