#pragma once
#include "tools.h"

#include <QHostInfo>
#include <QNetworkInterface>
#include <QDir>
#include <QFile>

QString get_localmachine_name()
{
    QString machineName  = QHostInfo::localHostName();
    return machineName;
}

QString get_localmachine_ip()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for( int i = 0; i < ipAddressesList.size(); ++i )
    {
        if( ipAddressesList.at( i ) != QHostAddress::LocalHost &&
            ipAddressesList.at( i ).toIPv4Address() )
        {
            ipAddress = ipAddressesList.at( i ).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if( ipAddress.isEmpty() )
        ipAddress = QHostAddress( QHostAddress::LocalHost ).toString();
    return ipAddress;
}

QString get_localmachine_mac()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();
//    int i = 0;
//    foreach( QNetworkInterface ni, nets )
//    {
//        i++;
//        qDebug() << i << ni.name() << ni.hardwareAddress() << ni.humanReadableName();
//    }
    return nets[0].hardwareAddress();
}


void privateConvert( char *pStr )
{
    int len = strlen( pStr );
    int i = 0;
    for( i = 0; i < len; i++ )
    {
        pStr[i] = ( pStr[i] + 16 ) * 2 / 3;
        if( '\0' == pStr[i] )
        {
            pStr[i] = 1;
        }
    }
//    qDebug() << pStr;
}

bool getAuthorized( QString licenseFile )
{
    QFile file( licenseFile );
    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        return false;
    }
    QByteArray line = file.readLine();

    QString mac_address = get_localmachine_mac();
    QByteArray mac_address_byte = mac_address.toLatin1();
    char *mac_address_char = mac_address_byte.data();
    privateConvert(mac_address_char);
    if (line == QByteArray(mac_address_char)) {
        return true;
    }
    else {
        return false;
    }

}
