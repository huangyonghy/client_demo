#ifndef TOOLS_H
#define TOOLS_H

#pragma once
#include <QString>
#include <QList>
#include <QMap>
#include <QColor>
#include <QFont>
#include <QVector>

/******************************************************************************************************************/
/*                                              Get Computer Infomation                                           */

QString get_localmachine_name();
QString get_localmachine_ip();
QString get_localmachine_mac();

/******************************************************************************************************************/

/******************************************************************************************************************/
/*                                                 Small Functions                                                */

void privateConvert(char *pStr);
bool getAuthorized(QString licenseFile);

/******************************************************************************************************************/


#endif // TOOLS_H
