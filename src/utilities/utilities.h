/* ********************************************************
   *  utilities.h
   *  for TgTake2 project
   *
   *  Created by Geoff R. Mclane, Paris
   *  (C) 2011 GPL2 (or later)
   *
   ******************************************************** */
#ifndef UTILITIES_H
#define UTILITIES_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QDateTime>
#include <QProcess>
#include <QWidget>
#include <QMessageBox>

#include "app_config.h"

// #include "zlib/fgx_zlib.h"
/* ZIP FILE */
// extern QString gzreadLine(gzFile file);

/* LOG FILE FUNCTIONS */

// out a QString to the current log
// flag - bit 1 = add line term (default)
//        bit 2 = prepend time
#define olflg_AddLE 0x00000001
#define olflg_AddTm 0x00000002
#define olflg_8000  0x00008000  // output, and clear 'keep' list
#define olflg_4000  0x00004000  // add to a 'keep' list

extern void outLog(QString s, int flag = olflg_AddLE);
extern void util_setLogFile(QString file);
extern QString util_getLogFile();
extern bool util_createLogFile(QString file);
extern bool util_setStdLogFile();

/* GET FILE LIST per filter */
extern QStringList findFiles(const QString &startDir, QStringList filters, bool recurse = false);

// given millisecond, return appropriate (nice) string, with units
extern QString getElapTimeStg(int ms);
extern QString util_getDateTimestg(void); // "yyyy/MM/dd HH:mm:ss"
extern QString util_getTimestg(void);     // "HH:mm:ss"

extern QString hmm_to_decimal(QString hmm);

// a distance estimate between two lat, lon positions, in km... using
#define EARTH_RAD 6370  // actually from 6,353 to 6,384 km
#define DEG2RAD 0.017453229251994329509  // est PI / 180
extern double dist_est_km(double lat1, double lon1, double lat2, double lon2);

extern char * abc_to_morse(char ch);
extern char * abc_to_telephony(char ch);
extern char * abc_to_phonetic(char ch);
extern char telephony_to_abc(char * pch);
extern char *telephony_to_morse(char * pch);
extern char *telephony_to_phonetic(char * pch);
extern QString callsign_to_sentence(QString input);
extern QString callsign_to_phonetics(QString input);

QString util_browseDirectory(QString prompt, QString current,
                             QWidget * parent = 0);
QString util_browseFile(QString prompt, QString current,
                        QWidget * parent = 0);
QString util_browseNewFile(QString prompt, QString current,
                           QWidget * parent = 0);

// added for TgTake2
extern bool util_latInRange(double lat);
extern bool util_lonInRange(double lon);
extern bool util_inWorldRange(double lat, double lon);
extern QString util_trimDoubleStg(double d);
extern int util_getYesNo( QString title, QString msg, QWidget * parent = 0 );

extern int util_lookslike_Fgfs_Construct( QString info );
extern int util_lookslike_hgtchop( QString info );
extern int util_lookslike_terrafit( QString info );
extern int util_lookslike_shape_decode( QString info );
extern int util_lookslike_genapts( QString info );

extern bool util_verifyProjDir(QString dir);

#endif // UTILITIES_H
// eof - utilities.h
