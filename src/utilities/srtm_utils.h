/********************************************************
    file: tggui_srtm.h

    Written by Geoff R. Mclane, started May 2011.

    Copyright (C) 2011  Geoff R. McLane - reports@geoffair.info

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    $Id: 0.0.1 2001-05-11 - version 0.0.1 $

    Some utility functions for SRTM

 ********************************************************************/

#ifndef TGGUI_SRTM_H
#define TGGUI_SRTM_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QDir>

#include "utilities/newbucket.h"
#include "utilities/workThread.h"

typedef QMap<QString, QString> MAPSTR2STR;

typedef struct tagWORKsu {
    WORK work;  // ALWAYS TOP ENTRY
    double north, south, east, west;
    QString result;
    int i_Hgt_Cnt;
    QStringList hgturlList;
    QStringList hgtroot; // user supplied HGT directory list (m_hgtrootEdit)
    MAPSTR2STR wSrtmList;
}WORKsu, *PWORKsu;

//#include "tggui_utils.h" // for LL, PLL structure
typedef struct tagLL {
    double lat;
    double lon;
}LL, * PLL;


extern QString util_getSRTMContinent( double lat, double lon );
extern QString util_getSRTM1Region( double lat, double lon );

//extern bool util_checkSRTMFiles( QString elevs, QString path );
extern int util_checkSRTMFiles(QMap<QString, QString> & elev, QString path);
// version using built-in lists
extern int util_checkSRTMFiles2( QMap<QString, QString> & elevs );
// is an elevation directory
extern bool srtm_isElevationDir(QString dirName);

// check if ANY elevation data is available, in any 'KNOWN' directory
extern int srtm_verifySRTMfiles(QString minLat, QString maxLat, QString minLon, QString maxLon,
                         QString workDirectory, int * ptot);
extern QStringList srtm_getSRTMIndexList( QString workDirectory );
extern int srtm_verifySRTMfiles(double dminLat, double dmaxLat, double dminLon, double dmaxLon,
                         QString workDirectory, int * ptot);

// get the HGT online lists
extern QStringList getSRTM3_Africa( QString & url );
extern QStringList getSRTM3_Australia( QString & url );
extern QStringList getSRTM3_Eurasia( QString & url );
extern QStringList getSRTM3_Islands( QString & url );
extern QStringList getSRTM3_North_America( QString & url );
extern QStringList getSRTM3_South_America( QString & url );
extern QStringList getSRTM1Region1( QString & url );
extern QStringList getSRTM1Region2( QString & url );
extern QStringList getSRTM1Region3( QString & url );
extern QStringList getSRTM1Region4( QString & url );
extern QStringList getSRTM1Region5( QString & url );
extern QStringList getSRTM1Region6( QString & url );
extern QStringList getSRTM1Region7( QString & url );

typedef QStringList (*GETHGTLIST) (QString &);
extern QList<GETHGTLIST> srtm_getHGTListFuncs();
extern bool srtm_getUrlForHgtBase( QString hgtBase, QString & url );

// convert lat,lon to HGT, and HGT to Bucket
extern QString srtm_PLLToHGTBase( PLL pll );
extern QString srtm_latlonToHGTBase( double dlat, double dlon );
extern QString srtm_latlonToHGTBase( int lat, int lon );
extern bool srtm_baseHGTtoBucket( QString hgtBase, QString & buck_path);
extern bool srtm_baseHGTtoPLL( QString hgtBase, PLL pll);
extern QString srtm_bucketToHGTBase( SGBucket & b );

// given a world range, find available HGT files for that range
extern bool srtm_getHGTListforRange(double minLat, double minLon,
                                    double maxLat, double maxLon,
                                    QStringList & hgtList);

// added TgTake2
//extern QString srtm_getSRTMListstg(double dsouth, double dwest,
//                                   double dnorth, double deast);
extern QString srtm_getSRTMListstg(PWORKsu pwsu);

extern bool abortSRTMCount;

#endif // TGGUI_SRTM_H
