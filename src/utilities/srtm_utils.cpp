/********************************************************
    file: srtm_utils.cpp

    Written by Geoff R. Mclane, started May 2011.

    Copyright (C) 2011-2016  Geoff R. McLane - reports@geoffair.info

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

#include "app_config.h"
#include "srtm_utils.h"
#include "newbucket.h"
#include "srtm_data.h"
#include "utilities/utilities.h"

#ifndef NDEBUG
#define TGS_DEBUG_VERS
#endif

bool abortSRTMCount = false;

#define latlonInWorld util_inWorldRange
// Url = [/srtm/version2_1/SRTM1/Region_01]
// Url = [/srtm/version2_1/SRTM1/Region_02]
// Url = [/srtm/version2_1/SRTM1/Region_03]
// Url = [/srtm/version2_1/SRTM1/Region_04]
// Url = [/srtm/version2_1/SRTM1/Region_05]
// Url = [/srtm/version2_1/SRTM1/Region_06]
// Url = [/srtm/version2_1/SRTM1/Region_07]
// Url = [/srtm/version2_1/SRTM3/Africa]
// Url = [/srtm/version2_1/SRTM3/Australia]
// Url = [/srtm/version2_1/SRTM3/Eurasia]
// Url = [/srtm/version2_1/SRTM3/Islands]
// Url = [/srtm/version2_1/SRTM3/North_America]
// Url = [/srtm/version2_1/SRTM3/South_America]
QStringList getSRTM3_Africa( QString & url )
{
    QStringList sl;
    sl = Africa.split(" ",QString::SkipEmptyParts);
    url = srtm_Africa_Url; // "/srtm/version2_1/SRTM3/Africa";
    return sl;
}
QStringList getSRTM3_Australia( QString & url )
{
    QStringList sl;
    sl = Australia.split(" ",QString::SkipEmptyParts);
    url = srtm_Australia_Url; // "/srtm/version2_1/SRTM3/Australia";
    return sl;
}
QStringList getSRTM3_Eurasia( QString & url )
{
    QStringList sl;
    sl = Eurasia.split(" ",QString::SkipEmptyParts);
    url = srtm_Eurasia_Url; // "/srtm/version2_1/SRTM3/Eurasia";
    return sl;
}
QStringList getSRTM3_Islands( QString & url )
{
    QStringList sl;
    sl = Islands.split(" ",QString::SkipEmptyParts);
    url = srtm_Islands_Url; // "/srtm/version2_1/SRTM3/Islands";
    return sl;
}
QStringList getSRTM3_North_America( QString & url )
{
    QStringList sl;
    sl = North_America.split(" ",QString::SkipEmptyParts);
    url = srtm_North_America_Url; // "/srtm/version2_1/SRTM3/North_America";
    return sl;
}
QStringList getSRTM3_South_America( QString & url )
{
    QStringList sl;
    sl = South_America.split(" ",QString::SkipEmptyParts);
    url = srtm_South_America_Url; // "/srtm/version2_1/SRTM3/South_America";
    return sl;
}

QStringList getSRTM1Region1( QString & url )
{
    QStringList sl;
    sl = Region_01.split(" ",QString::SkipEmptyParts);
    url = srtm_Region_01_Url; // "/srtm/version2_1/SRTM1/Region_01";
    return sl;
}
QStringList getSRTM1Region2( QString & url )
{
    QStringList sl;
    sl = Region_02.split(" ",QString::SkipEmptyParts);
    url = srtm_Region_02_Url; // "/srtm/version2_1/SRTM1/Region_02";
    return sl;
}
QStringList getSRTM1Region3( QString & url )
{
    QStringList sl;
    sl = Region_03.split(" ",QString::SkipEmptyParts);
    url = srtm_Region_03_Url; // "/srtm/version2_1/SRTM1/Region_03";
    return sl;
}
QStringList getSRTM1Region4( QString & url )
{
    QStringList sl;
    sl = Region_04.split(" ",QString::SkipEmptyParts);
    url = srtm_Region_04_Url; // "/srtm/version2_1/SRTM1/Region_04";
    return sl;
}
QStringList getSRTM1Region5( QString & url )
{
    QStringList sl;
    sl = Region_05.split(" ",QString::SkipEmptyParts);
    url = srtm_Region_05_Url; // "/srtm/version2_1/SRTM1/Region_05";
    return sl;
}
QStringList getSRTM1Region6( QString & url )
{
    QStringList sl;
    sl = Region_06.split(" ",QString::SkipEmptyParts);
    url = srtm_Region_06_Url; // "/srtm/version2_1/SRTM1/Region_06";
    return sl;
}
QStringList getSRTM1Region7( QString & url )
{
    QStringList sl;
    sl = Region_07.split(" ",QString::SkipEmptyParts);
    url = srtm_Region_07_Url; // "/srtm/version2_1/SRTM1/Region_07";
    return sl;
}


QList<GETHGTLIST> srtm_getHGTListFuncs()
{
    QList<GETHGTLIST> flist;
    flist += getSRTM1Region1;
    flist += getSRTM1Region2;
    flist += getSRTM1Region3;
    flist += getSRTM1Region4;
    flist += getSRTM1Region5;
    flist += getSRTM1Region6;
    flist += getSRTM1Region7;
    flist += getSRTM3_Africa;
    flist += getSRTM3_Australia;
    flist += getSRTM3_Eurasia;
    flist += getSRTM3_Islands;
    flist += getSRTM3_North_America;
    flist += getSRTM3_South_America;
    return flist;
}

bool srtm_getUrlForHgtBase( QString hgtBase, QString & url )
{
    QList<GETHGTLIST> flist;
    flist = srtm_getHGTListFuncs();
    foreach (GETHGTLIST func, flist) {
        QString u;
        QStringList sl;
        sl = func(u);
        foreach (QString hgt, sl) {
            if (hgt == hgtBase) {
                url = u;
                return true;
            }
        }
    }
    return false; // FAILED to find this HGT
}

//typedef QStringList (*getfunc) (QString &);
int srtm_getHGTZipCount(int flag)
{
    int iret = 0;
    QList<GETHGTLIST> flist;
    if ((flag == 0) || (flag & 1)) {
        flist += getSRTM1Region1;
        flist += getSRTM1Region2;
        flist += getSRTM1Region3;
        flist += getSRTM1Region4;
        flist += getSRTM1Region5;
        flist += getSRTM1Region6;
        flist += getSRTM1Region7;
    }
    if ((flag == 0) || (flag & 2)) {
        flist += getSRTM3_Africa;
        flist += getSRTM3_Australia;
        flist += getSRTM3_Eurasia;
        flist += getSRTM3_Islands;
        flist += getSRTM3_North_America;
        flist += getSRTM3_South_America;
    }
    foreach (GETHGTLIST func, flist) {
        QString url;
        QStringList sl;
        sl = func(url);
        iret += sl.size();
    }
    return iret;
}

int util_checkSRTMFiles2( QMap<QString, QString> & elevs )
{
    int iret = 0;
    QList<GETHGTLIST> flist;
    flist = srtm_getHGTListFuncs();
    foreach (GETHGTLIST func, flist) {
        QString url;
        QStringList sl;
        sl = func(url);
        foreach (QString line, sl) {
            QMap<QString, QString>::iterator i = elevs.find(line);
            if (i != elevs.end()) {
                elevs.insert(line,url); // insert the URL
                iret++;
            }
        }
    }
    return iret;
}

// THIS HAS THE PROBLEM AS TO WHERE TO PUT THESE FILES
// The above function uses BUILT-IN data
// =================================
// THIS FILE FUNCTION IS DEPRECIATED
// =================================
int util_checkSRTMFiles( QMap<QString, QString> & elevs, QString path )
{
    int iret = 0;
    QStringList srtmList;
    srtmList << "SRTM1_Region_01.txt" <<
            "SRTM1_Region_02.txt" <<
            "SRTM1_Region_03.txt" <<
            "SRTM1_Region_04.txt" <<
            "SRTM1_Region_05.txt" <<
            "SRTM1_Region_06.txt" <<
            "SRTM1_Region_07.txt" <<
            "SRTM3_Africa.txt" <<
            "SRTM3_Australia.txt" <<
            "SRTM3_Eurasia.txt" <<
            "SRTM3_Islands.txt" <<
            "SRTM3_North_America.txt" <<
            "SRTM3_South_America.txt";
    QString url;
    QString index = "Index ";
    QString line;
    //QStringList elevList;
    //elevList = elevs.split("\n", QString::SkipEmptyParts);
    foreach (QString fil, srtmList) {
        QString file = path+"/"+fil;
        QFile f(file);
        if (f.open(QIODevice::ReadOnly)) {
            QTextStream textStream(&f);
            while (textStream.atEnd() == false) {
                line = textStream.readLine();
                if (line.at(0) == '#') {
                    int off = line.indexOf(index);
                    if (off > 0) {
                        url = line.mid(off+index.size());
                    } else if (line.contains(" eof"))
                        break;
                    continue;
                }
                line.trimmed();
                QMap<QString, QString>::iterator i = elevs.find(line);
                if (i != elevs.end()) {
                    elevs.insert(line,url); // found
                    iret++;
                }
            }
        }
    }

    return iret;
}

// estimated from : http://dds.cr.usgs.gov/srtm/version2_1/Documentation/Continent_def.gif
// Other Web sites of interest:
// NASA/JPL SRTM: http://www.jpl.nasa.gov/srtm/
// NGA: http://www.nga.mil/
// STS-99 Press Kit: http://www.shuttlepresskit.com/STS-99/index.htm
// Johnson Space Center STS-99: http://spaceflight.nasa.gov/shuttle/archives/sts-99/index.html
// German Space Agency: http://www.dlr.de/srtm
// Italian Space Agency: http://srtm.det.unifi.it/index.htm
// U.S. Geological Survey, EROS Data Center: http://edc.usgs.gov/
// Note: DTED is a trademark of the National Imagery and Mapping Agency

//URL: SRTM1/Region_01     -  164 HGT zips,   4 'chunks' Min  38,-126, Max  49,-112 ( 11 x  14 degs)
//URL: SRTM1/Region_02     -  161 HGT zips,   6 'chunks' Min  38,-111, Max  49, -98 ( 11 x  13 degs)
//URL: SRTM1/Region_03     -  150 HGT zips,   4 'chunks' Min  38, -97, Max  49, -84 ( 11 x  13 degs)
//URL: SRTM1/Region_04     -  156 HGT zips,   4 'chunks' Min  28,-123, Max  37,-101 (  9 x  22 degs)
//URL: SRTM1/Region_05     -  165 HGT zips,   6 'chunks' Min  25,-100, Max  37, -84 ( 12 x  16 degs)
//URL: SRTM1/Region_06     -  169 HGT zips,   8 'chunks' Min  17, -83, Max  47, -65 ( 30 x  18 degs)
//URL: SRTM1/Region_07     -  152 HGT zips,  16 'chunks' Min -15,-180, Max  59, 179 ( 74 x 359 degs)
//URL: SRTM3/Africa        - 3250 HGT zips,  55 'chunks' Min -35, -32, Max  39,  63 ( 74 x  95 degs)
//URL: SRTM3/Australia     - 1060 HGT zips,  35 'chunks' Min -44,-180, Max -11, 179 ( 33 x 359 degs)
//URL: SRTM3/Eurasia       - 5876 HGT zips, 134 'chunks' Min -13,-177, Max  60, 179 ( 73 x 356 degs)
//URL: SRTM3/Islands       -  141 HGT zips,  29 'chunks' Min -56,-179, Max  28, 179 ( 84 x 358 degs)
//URL: SRTM3/North_America - 2412 HGT zips,  54 'chunks' Min  10,-180, Max  60, 179 ( 50 x 359 degs)
//URL: SRTM3/South_America - 1807 HGT zips,  35 'chunks' Min -56, -93, Max  14, -33 ( 70 x  60 degs)

QString util_getSRTMContinent( double lat, double lon )
{
    if ((lon >= -170) && (lon <= -58) &&
        (lat <= 61) && (lat >= 15)) {
        // but there are some island exceptions
        if ((lat <= 29) && (lat >= 16) &&
            (lon >= -179) && (lon <= -154)) {
            return "Islands";
        } else {
            return "North_America";
        }
    }
    if ((lon >= -32) && (lon <= 60) &&
        (lat <= 35) && (lat >= -35) ) {
        // N00E006 -
        return "Africa";
    }
    if ((lon >= -95) && (lon <= -32)&&
        (lat <= 15) && (lat >= -56)) {
        return "South_America";
    }
    if ((lon >= -14) && (lon <= 60) &&
        (lat <= 60) && (lat >= 35) ) {
        return "Eurasia";
    }
    if ((lon >= 60) && (lon <= 180) &&
        (lat <= 60) && (lat >= -10)) {
        return "Eurasia";
    }
    if ((lon >= 110) && (lon <= 180)&&
        (lat <= -10) && (lat >= -55)) {
        // but NZ is in Islands
        if ((lon >= 165) && (lon <= 180)&&
            (lat <= -28) && (lat >= -55)) {
            return "Islands";
        } else {
            return "Australia";
        }
    }
    if ((lon >= -180)&&(lon <= -115)&&
        (lat <= -10)&&(lat >= -30)) {
        return "Australia";
    }
    return "Islands";
}

QString util_getSRTM1Region( double lat, double lon )
{
    if ((lon >= -126)&&(lon >= -111)&&
        (lat <= 50) && (lat >= 38)){
        return "Region_01";
    }
    if ((lon >= -111)&&(lon <= -97)&&
        (lat <= 50)&&(lat >= 38)){
        return "Region_02";
    }
    if ((lon >= -97)&&(lon <= -83)&&
        (lat <= 50)&&(lat >= 38)) {
        return "Region_03";
    }
    if ((lon >= -123)&&(lon <= -100)&&
        (lat <= 38)&&(lat >= 28)) {
        return "Region_04";
    }
    if ((lon >= -100)&&(lon <= -83)&&
        (lat <= 38)&&(lat >= 25)){
        return "Region_05";
    }
    if ((lon >= -83)&&(lon <= -65)&&
        (lat <= 48)&&(lat > 17)){
        return "Region_06";
    }
    if ((lon >= -180)&&(lon <= -131)&&
        (lat <= 60)&&(lat >= -15)) {
        return "Region_07";
    }
    return "NO REGION";
}

// =======================================================

//#define INDEX_DEBUG
#undef INDEX_DEBUG

// Get the LIST of SRTM folders
QStringList srtm_getSRTMDirList(void)
{
    QStringList elev_src;
    elev_src <<  "SRTM2-Africa-3" << "SRTM2-Australia-3" <<
            "SRTM2-Eurasia-3" << "SRTM2-Islands-3" <<
            "SRTM2-North_America-3" << "SRTM2-South_America-3" <<
            "DEM-USGS-3" << "SRTM-1" << "SRTM-3" <<
            "SRTM-30";
    // Should this be extended by these ??? << "SRTM" << "SRTM3" << "SRTM30";
    return elev_src;
}

// Is a directory a known SRTM folder
bool srtm_isElevationDir(QString dirName)
{
    QStringList elevList;
    elevList = srtm_getSRTMDirList();
    for (int i = 0; i < elevList.size(); i++) {
        if (elevList[i] == dirName)
            return true;
    }
    return false;
}

int srtm_verifySRTMfiles(QString minLat, QString maxLat, QString minLon, QString maxLon,
                         QString workDirectory, int * ptot)
{
    double dminLon = minLon.toDouble();
    double dminLat = minLat.toDouble();
    double dmaxLon = maxLon.toDouble();
    double dmaxLat = maxLat.toDouble();
    return srtm_verifySRTMfiles(dminLat, dmaxLat, dminLon, dmaxLon, workDirectory, ptot);
}


QStringList srtm_getSRTMIndexList( QString workDirectory )
{
    QStringList indexList;
    QStringList elev_src;
    QStringList files;
    QString index;
    int m, i, j, k;
#ifdef INDEX_DEBUG
    int wrap = 0;
    int max_wrap = 20;
#endif // #ifdef INDEX_DEBUG
    elev_src = srtm_getSRTMDirList();
    for (m = 0; m < elev_src.size(); m++) {
        QDir dir(workDirectory+"/"+elev_src[m]);
        if (dir.exists()) {
            // files = findFiles(dir.absolutePath(), QStringList() << "*.arr.gz" << "*.fit.gz", true);
            // maybe should ONLY find the ARRAY files, since construct
            // has some built-in 'fit' capability
            files = findFiles(dir.absolutePath(), QStringList() << "*.arr.gz", true);
            if (files.count() > 0) {
                foreach (QString file, files) {
                    //QFile f(file);
                    //if (file.contains(rx)) {
                    for (i = 0; i < file.size(); i++) {
                        if ( file.at(i) == QChar('/') ) {
                            // got a path separator - get index
                            index = ""; // clear the string
                            for (j = i + 1; j < file.size(); j++) {
                                if ((file.at(j) >= QChar('0')) && (file.at(j) <= QChar('9'))) {
                                    index += file.at(j);
                                } else break;
                            }
                            if ((j < file.size()) && (file.at(j) == QChar('.')) && (index.size() == 7)) {
                                for (k = 0; k < indexList.size(); k++) {
                                    if (indexList[k] == index)
                                        break;
                                }
                                if (k == indexList.size()) {
                                    indexList += index;
#ifdef INDEX_DEBUG
                                    outLog(index+" ",0);
                                    wrap++;
                                    if (wrap >= max_wrap) {
                                        outLog("\n",0);
                                        wrap = 0;
                                    }
#endif // #ifdef INDEX_DEBUG
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return indexList;
}

int srtm_verifySRTMfiles(double dminLat, double dmaxLat, double dminLon, double dmaxLon,
                         QString workDirectory, int * ptot)
{
    int result = 0;
    QStringList indexList;
    QStringList indexList_needed;
    int i, j;
    QString index = "";
    double clat, clon;

#ifdef INDEX_DEBUG
    QString tmp;
    int wrap;
    int max_wrap = 20;
    int ichk = 0;
    outLog("\nGet existing index list...\n",0);
#endif // #ifdef INDEX_DEBUG

    indexList = srtm_getSRTMIndexList(workDirectory);
    if (indexList.size() == 0) {
        // not much use checking what elevations are needed,
        // if we have NONE to start with...
        if (ptot)
            *ptot = -1; // advise indeterminate
        return result;
    }

    // break the desired area into buckets
    // this does not appear to work on very large areas ;=((
    SGBucket b_min( dminLon, dminLat );
    SGBucket b_max( dmaxLon, dmaxLat );
    if (b_min == b_max) {
        // just one bucket
        index.sprintf("%ld", b_min.gen_index());
        indexList_needed += index;
    } else {
        // we have a range of buckets
        int dx, dy, odx;
        SGBucket b_cur;
        sgBucketDiff(b_min, b_max, &dx, &dy);
        b_cur.set_bucket( (dminLon + dmaxLon) / 2, (dmaxLat + dminLat) / 2);
        // all buckets have height of 1/8 deg = SG_BUCKET_SPAN
        double width = dmaxLon - dminLon;
#ifdef INDEX_DEBUG
        double height =  dmaxLat - dminLat;
        int b4hgt = (int)((height / SG_BUCKET_SPAN) + 0.5);
#endif // #ifdef INDEX_DEBUG

        // width depends on latitude - get SMALLEST
        // the LARGEST is always at high/low lat - full 360 degs
        double bwid = b_min.get_width() < b_max.get_width() ?
                      b_min.get_width() : b_max.get_width();
        if ( b_cur.get_width() < bwid )
            bwid = b_cur.get_width();
        int b4wid = (int)((width / bwid) + 0.5);
        odx = dx;
        if (b4wid > dx) {
            // bucket services returns abnormally low value
            // when a very BIG world range is used - so compensate here
            dx = b4wid; // not sure how to fix the bucket services???
        }
#ifdef INDEX_DEBUG
        index.sprintf("\nGot range lon/lat Min %.0f,%.0f, Max. %.0f,%.0f (bwid %f, bhgt %f degs)\n",
                      dminLon, dminLat, dmaxLon, dmaxLat,
                      bwid, SG_BUCKET_SPAN );
        outLog(index,0);
        index.sprintf("Width %.0f, Height %.0f degrees - w.steps %d, h.steps %d (%d)\n",
                      width, height, b4wid, b4hgt, ((b4wid+1) * (b4hgt+1)));
        outLog(index,0);
        index.sprintf("Got range dx=%d, dy=%d, potential total %d (was %d)\n",
                      dx, dy, ((dx+1)*(dy+1)), ((odx+1)*(dy+1)) );
        outLog(index,0);
#if 0
        wrap = 0;
        //for ( j = 436; j <= 443; j++ ) {  // stick with -34
        for ( j = 436; j <= 436; j++ ) {  // stick with -34
            for ( i = 0; i <= dy; i++ ) {
                b_cur = sgBucketOffset(dminLon, dminLat, i, j);
                //index.sprintf("%07ld", b_cur.gen_index());
                index.sprintf("%ld", b_cur.gen_index());
                outLog(index,0);
                clat = b_cur.get_center_lat();
                clon = b_cur.get_center_lon();
                index.sprintf(" %3.0f,%3.0f dx=%d, dy=%d\n", clat, clon, i, j );
                outLog(index,0);
            }
        }
#endif // 0
        wrap = 0;
        ichk = 0;
#endif // #ifdef INDEX_DEBUG
        for ( j = 0; j <= dy; j++ ) {
            for ( i = 0; i <= dx; i++ ) {
                b_cur = sgBucketOffset(dminLon, dminLat, i, j);
                clat = b_cur.get_center_lat();
                clon = b_cur.get_center_lon();
                index.sprintf("%ld", b_cur.gen_index());
                if (indexList_needed.indexOf(index) >= 0)
                    continue; // do NOT repeat any indexes
                indexList_needed += index; // add new index
#ifdef INDEX_DEBUG
                wrap++;
                if (i == ichk) {
                    tmp.sprintf("\nOffset y=%d, x=%d, lat=%3.0f, lon=%3.0f\n",
                                j, i, clat, clon);
                    outLog(tmp,0);
                    wrap = 0;
                }
                outLog(" "+index,0);
                if (wrap >= max_wrap) {
                    wrap = 0;
                    outLog("\n",0);
                }
#endif // #ifdef INDEX_DEBUG
            }
#ifdef INDEX_DEBUG
            ichk++;
            if (ichk > dx)
                ichk = 0;
#endif // #ifdef INDEX_DEBUG
        }
    }
    // ok, got a list of indexes needed
#ifdef INDEX_DEBUG
    wrap = 0;
    index.sprintf("\nNow compare list - %d with %d\n",
                  indexList.size(),indexList_needed.size());
    outLog(index,0);
#endif // #ifdef INDEX_DEBUG

    // compare the INDEXES found, with those needed for area
    for (i = 0; i < indexList.size(); i++) {
        for (j = 0; j < indexList_needed.size(); j++) {
            if (indexList[i] == indexList_needed[j])
                result++; // even if only one result in many, it is better than none
        }
    }
    if (ptot)
        *ptot = indexList_needed.size();
    return result; // If 0, oops, NO matching indexes
}

// =====================================================
// More on elevations (HGT zip) files
// ==================================
bool srtm_baseHGTtoPLL( QString hgtBase, PLL pll)
{
    QChar ch;
    bool ok;
    double lat, lon;

    if (hgtBase.size() != 7)
        return false;

    ch = hgtBase.at(0);
    if ( ! ((ch == QChar('N'))||(ch == QChar('S'))) )
        return false;
    lat = hgtBase.mid(1,2).toDouble(&ok);
    if (!ok)
        return false;
    if (ch == QChar('S'))
        lat = -lat;

    ch = hgtBase.at(3);
    if ( ! ((ch == QChar('E'))||(ch == QChar('W'))) )
        return false;
    lon = hgtBase.mid(4).toDouble(&ok);
    if (!ok)
        return false;
    if (ch == QChar('W'))
        lon = -lon;

    pll->lat = lat;
    pll->lon = lon;
    return true;
}

// 0  3
// N38W123
bool srtm_baseHGTtoBucket( QString hgtBase, QString & buck_path)
{
    LL ll;
    PLL pll = &ll;
    if ( !srtm_baseHGTtoPLL( hgtBase, pll) )
        return false;
    SGBucket b(pll->lon,pll->lat);
    buck_path = b.gen_base_path().c_str();
    buck_path += "/";
    buck_path += b.gen_index_str().c_str();
    return true;
}

QString srtm_latlonToHGTBase( int lat, int lon )
{
    QString hgt;
    QString clat;
    QString clon;

    if (lat < 0.0) {
        hgt = "S";
        lat = -lat;
        lat += 1;   // 20151101 - reversed 20161021 - Important change
    } else
        hgt = "N";

    clat.sprintf("%02d", lat);
    hgt += clat;

    if (lon < 0.0) {
        hgt += "W";
        lon = -lon;
        lon += 1;   // 20151101 - reversed 20161021 - Important change
    } else
        hgt += "E";

    clon.sprintf("%03d", lon);
    hgt += clon;

    return hgt;
}

QString srtm_latlonToHGTBase( double dlat, double dlon )
{
    int lat = (int)dlat;
    int lon = (int)dlon;
    return srtm_latlonToHGTBase( lat, lon );
}

QString srtm_PLLToHGTBase( PLL pll )
{
    return srtm_latlonToHGTBase(pll->lat,pll->lon);
}

QString srtm_bucketToHGTBase( SGBucket & b )
{
    return srtm_latlonToHGTBase(b.get_center_lat(),b.get_center_lon());
}


bool srtm_getHGTListforRange(double minLat, double minLon,
                             double maxLat, double maxLon,
                             QStringList & hgtList) {
    int iminlat, iminlon, imaxlat, imaxlon;
    iminlat = (int)minLat;
    imaxlat = (int)maxLat;
    iminlon = (int)minLon;
    imaxlon = (int)maxLon;
    if ((minLat <= maxLat) && (minLon <= maxLon) &&
        latlonInWorld( minLat, minLon ) &&
        latlonInWorld( maxLat, maxLon ) &&
        ((double)iminlat == minLat) &&
        ((double)imaxlat == maxLat) &&
        ((double)iminlon == minLon) &&
        ((double)imaxlon == maxLon) ) {
        int i, j;
        QString hgt;
        QString url;
        for (i = iminlat; i <= imaxlat; i++) {
            for (j = iminlon; j <= imaxlon; j++) {
                hgt = srtm_latlonToHGTBase(i,j);
                if (srtm_getUrlForHgtBase( hgt, url )) {
                    hgtList += url+"/"+hgt; // store the HGT (base) zip name
                }
            }
        }
        return true;
    }
    return false;
}

// ==================================
//QString srtm_getSRTMListstg(double dsouth, double dwest, double dnorth, double deast)
// NOTE: RUN ON A WORKER THREAD
// QString url = "http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/";
// Africa/ Australia / Eurasia / Islands / North_America / South_America /
// OR
// From a supplied USER m_hgtrootEdit directory...

QString srtm_getSRTMListstg(PWORKsu pwsu)
{
    // MAPSTR2STR qmSRTM2URL2;
    //QStringList hgturlList;
    int buck_cnt = 0;
    int buck_srtm = 0;
    QStringList list, elevList;
    QString url, hgt, tmp, index, path, msg, elevStg;
    int i, j, dx, dy, max;
    //int m_Hgt_Cnt;
    double dnorth = pwsu->north;
    double dsouth = pwsu->south;
    double dwest  = pwsu->west;
    double deast  = pwsu->east;

    double minlon = dwest;
    double minlat = dsouth;
    double maxlon = deast;
    double maxlat = dnorth;

    int inorth = (int)dnorth;
    int iwest = (int)dwest;
    int ieast = (int)deast;
    int isouth = (int)dsouth;
    list.clear();
    SGBucket b_min(dwest,dsouth);
    SGBucket b_max(deast,dnorth);
    SGBucket b_cur;
    msg = "";
    if ((minlon < maxlon) &&
        (minlat < maxlat)) {
        msg.sprintf("BBox: %lf,%lf,%lf,%lf\n", minlon, minlat, maxlon, maxlat );
    }
    else {
        msg.sprintf("BAD RANGE: BBox: %lf,%lf,%lf,%lf\n", minlon, minlat, maxlon, maxlat);
    }
    outLog("srtm_getSRTMListstg: " + msg);

    if (b_min == b_max) {
        buck_cnt++;
        hgt = srtm_bucketToHGTBase(b_cur); // get the HGTbase string
        if ( srtm_getUrlForHgtBase( hgt, url ) ) {
            if (list.indexOf(url) == -1)
                list += url;    // accumulate SRTM url's
            buck_srtm++;
        }
    } else {
        sgBucketDiff(b_min,b_max,&dx,&dy);
        for (j = 0; j <= dy; j++) {
            for (i = 0; i <= dx; i++) {
                if (pwsu->work.abort_work) {
                    abortSRTMCount = false;
                    outLog("SRTM count aborted 1");
                    return "";
                }
                if (abortSRTMCount) {
                    abortSRTMCount = false;
                    outLog("SRTM count aborted 4");
                    return "";
                }
                b_cur = sgBucketOffset(dwest,dsouth,i,j);
                buck_cnt++;
                tmp.sprintf("%d of %d: ", (j*dx)+i+1, (dx+1)*(dy+1));
                index.sprintf("%ld", b_cur.gen_index());
                path.sprintf("%s", b_cur.gen_base_path().c_str());
                path += "/"+index;
                // only if the 'working' dialog is up...
                // NEWWAITMESSAGE("Moment... Doing startup of "+appname+"\nupdateCenter() - "+tmp+path+"\n");
                hgt = srtm_bucketToHGTBase(b_cur); // get the HGTbase string
                if ( srtm_getUrlForHgtBase( hgt, url ) ) {
                    if (list.indexOf(url) == -1)
                        list += url;    // accumulate SRTM url's
                    buck_srtm++;
                }
            }
        }
    }
    tmp.sprintf(" - Est. %d of %d buckets.",buck_srtm, buck_cnt);
    msg += tmp;

    // build a HELPFUL SRTM list to add to the LOG
    elevStg = "";
#ifdef USE_FILE_INTERFACE // use the FILE ineterface
    qmSRTM2URL.clear();
#else
    pwsu->wSrtmList.clear();
#endif
    pwsu->hgturlList.clear();
    pwsu->i_Hgt_Cnt = 0;
    for (i = iwest; i <= ieast; i++) {
        for (j = isouth; j <= inorth; j++) {
            if (pwsu->work.abort_work) {
                abortSRTMCount = false;
                outLog("SRTM count aborted 2");
                return "";
            }
            if (abortSRTMCount) {
                abortSRTMCount = false;
                outLog("SRTM count aborted 3");
                return "";
            }
            hgt = srtm_latlonToHGTBase(j,i);
            if (elevStg.size()) elevStg += ";"; // add separator
            elevStg += hgt;
            elevList.push_back(hgt);
#ifdef USE_FILE_INTERFACE // use the FILE ineterface
            qmSRTM2URL.insert(hgt,"");
#else
            pwsu->wSrtmList.insert(hgt, "");
#endif
            if (srtm_getUrlForHgtBase( hgt, url )) {
                pwsu->hgturlList += url+"/"+hgt; // store the HGT (base) zip name
            }
            pwsu->i_Hgt_Cnt++;
        }
    }
    tmp.sprintf("Of %d HGT %d avail.",pwsu->i_Hgt_Cnt,pwsu->hgturlList.size());
    //tmp.sprintf(", of %d HGT %d avail.",hgt_cnt,hgturlList.size());
    //msg += tmp;
    max = pwsu->hgturlList.size();
    if (max) {
        outLog(tmp+"\n",0);
        for (i = 0; i < max; i++) {
            url = pwsu->hgturlList[i];
            outLog(url+"\n",0);
        }
    }
    // are the HGT elevations available in a user supplied hgtroot dir?
    size_t ii1, max1 = pwsu->hgtroot.size();
    size_t max2 = pwsu->wSrtmList.size();
    if (max1 && max2) {
        hgt.sprintf("Have %d hgt root directories", max1);
        outLog(hgt);
        for (ii1 = 0; ii1 < max1; ii1++) {
            url = pwsu->hgtroot.at((int)ii1);
            MAPSTR2STR::iterator ib = pwsu->wSrtmList.begin();
            MAPSTR2STR::iterator ie = pwsu->wSrtmList.end();
            for (; ib != ie; ib++) {
                hgt = ib.key();
                path = url;
                path += "/";
                path += hgt;
                path += ".hgt";
                QFile f(path);
                if (f.exists()) {
                    pwsu->wSrtmList.insert(hgt, path);
                    path += " ok";
                }
                else {
                    path += " NF";
                }
                outLog(path);
            }
        }
    }
// #ifdef TGS_DEBUG_VERS // but outputs extra info to the log file
    outLog("List of elevations\n",0);
    outLog(elevStg+"\n",0);
// #endif // #ifdef TGS_DEBUG_VERS // but outputs extra info to the log file
    return tmp;
}

// eof - srtm_utils.cpp

