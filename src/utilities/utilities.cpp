/* ********************************************************
   *  utilities.cpp
   *  for TgTake2 project
   *
   *  Created by Geoff R. Mclane, Paris
   *  (C) 2011 GPL2 (or later)
   *
   ******************************************************** */

#include <QMutex>
#include <QFileDialog>
#include "utilities.h"
#include <math.h>
#include <stdlib.h>
#include "app_config.h"

/*
    LOG FILE FUNCTIONS
 */

static QString m_logFile;
static QMutex m_logMutex;

// set log file name
void util_setLogFile(QString file)
{
    m_logFile = file; // set name
}

QString util_getLogFile()
{
    return m_logFile; // set name
}

// create a log file and set name
bool util_createLogFile(QString file)
{
    bool bret = false;
    QFile data(file);
    if (data.open(QIODevice::ReadWrite | QIODevice::Truncate | QFile::Text)) {
        QTextStream out(&data);
        QString dt = util_getDateTimestg();
        out << dt << " - ";
        out.flush();
        data.close();
        bret = true;    // log is ok
        util_setLogFile(file); // set name
    }
    if (bret) {
        QString msg;
        msg = "Commenced log, version ";
        msg.append(PGM_VERSION);
        msg.append(" of date ");
        msg.append(PGM_DATE);
        outLog(msg);
    }
    return bret;
}

bool util_setStdLogFile()
{
    QString dir;
    QString file;
    QString log;

    file = PGM_LOG;     // default LOG file
    // default PATH for LOG file
#if defined(Q_OS_WIN)
    dir  = "C:/Windows/Temp";
    char * cp = getenv("TEMP");
    if (cp)
        dir = cp;
#elif defined(Q_OS_MAC)
   dir.append(QDir::homePath());
   dir.append("/Library/Logs");
#elif defined(Q_OS_UNIX)
   dir  = "/tmp";
#else
#error Unknown OS - Add default log dir and file for this OS!
#endif
   log = dir + "/" + file;
   // return util_createLogFile(MainObject::log_file_path());
   return util_createLogFile(log);
}

static QStringList logs_kept;

// append to LOG file, after acquiring the mutext
void outLog(QString s, int flag)
{
    if (m_logFile.size()) {
        QFile data(m_logFile);
        m_logMutex.lock();  // grap the mutex
        if (data.open(QFile::WriteOnly | QFile::Append | QFile::Text)) {
            QTextStream out(&data);
            if (flag & 0x8000) {
                int max = logs_kept.count();
                for (int i = 0; i < max; i++)
                    out << "RPT: "+logs_kept.at(i)+"\n";
                logs_kept.clear();
            }
            if (flag & olflg_AddTm)
                out << util_getTimestg()+" - ";
            out << s;
            if (flag & olflg_AddLE)
                out << "\n";
            data.close(); // ensure it get to disk
            if (flag & 0x4000)
                logs_kept.append(s);
        }
        m_logMutex.unlock(); // release the mutex
    }
}

/*
    Utility function that recursively searches, if desired, for files per filters.
*/
QStringList findFiles(const QString &startDir, QStringList filters, bool recurse)
{
    QStringList names;
    QDir dir(startDir);

    // 1: Get the files matching the filter
    foreach (QString file, dir.entryList(filters, QDir::Files))
        names += startDir + "/" + file;

    if (recurse) {
        // 2: If recursive, get ALL directories, and try for more files
        foreach (QString subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
            names += findFiles(startDir + "/" + subdir, filters, recurse);
    }
    return names;
}

// given millisecond, return appropriate (nice) string, with units
QString getElapTimeStg(int ms)
{
    QString fmt = "";
    if (ms < 1000) {
        fmt.sprintf("%d ms", ms);
    } else {
        int secs = ms / 1000;
        ms = ms % 1000;
        if (secs < 60) {
            if (ms)
                fmt.sprintf("%d.%03d secs", secs, ms);
            else
                fmt.sprintf("%d secs", secs);
        } else {
            int mins = secs / 60;
            secs = secs % 60;
            if (mins < 60) {
                if (ms)
                    fmt.sprintf("%d:%02d.%03d mins", mins, secs, ms);
                else if (secs)
                    fmt.sprintf("%d:%02d mins", mins, secs);
                else
                    fmt.sprintf("%d mins", mins);
            } else {
                int hrs = mins / 60;
                mins = mins % 60;
                if (ms)
                    fmt.sprintf("%d:%02d:%02d.%03d hrs", hrs, mins, secs, ms);
                else if (secs)
                    fmt.sprintf("%d:%02d:%02d hrs", hrs, mins, secs);
                else if (mins)
                    fmt.sprintf("%d:%02d hrs", hrs, mins);
                else
                    fmt.sprintf("%d hrs", hrs);
            }
        }
    }
    return fmt;
}

QString util_getDateTimestg(void)
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString dt = datetime.toString("yyyy/MM/dd HH:mm:ss");
    return dt;
}
QString util_getTimestg(void)
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString dt = datetime.toString("HH:mm:ss");
    return dt;
}

// FIX20110823 - dist_est_km() returned 'nan' if virtually the same point
double dist_est_km(double lat1, double lon1, double lat2, double lon2)
{
    double d, dd;
    double small = 0.0000001;
    double dt = fabs(lat1 - lat2);
    double dn = fabs(lon1 - lon2);
    if ((dt < small)&&(dn < small))
        return 0.0; // this is to try to avoid a 'nan'
    dt = lat1 * DEG2RAD;
    dn = lat2 * DEG2RAD;
    dd = (lon2 - lon1) * DEG2RAD;
    d = sin(dt) * sin(dn);
    d += cos(dt) * cos(dn) * cos(dd);
    return (acos(d) * EARTH_RAD);
}

/*
             lat="N52 18.101"
             lon="E04 47.433"
Degrees Minutes.m to Decimal Degrees
.d = M.m / 60
Decimal Degrees = Degrees + .d

*/
// QString Helpers::hmm_to_decimal(QString hmm)
QString hmm_to_decimal(QString hmm)
{
    QStringList parts = hmm.split(" ");
    QString h = parts.at(0);
    double mult = 1;
    double deg;

    if( h.contains("N") ){
        h.replace("N", "");
    }else if(h.contains("S")){
        h.replace("S", "");
        mult = -1;
    }else if(h.contains("E")){
        h.replace("E", "");
    }else if(h.contains("W")){
        h.replace("W", "");
        mult = -1.0;
    }
    deg = h.toDouble();
    deg *= mult;

    double ms;
    if(deg > 0){
        ms = deg + parts.at(1).toDouble() / 60;
    }else{
        ms = deg - parts.at(1).toDouble() / 60;
    }
    //return QString("%1").arg(ms);
    h.sprintf("%.12f",ms);
    return h;
}


typedef struct tagABC {
  char abc;
  char * morse;
  char * telephony;
  char * phonetic;
}ABC, * PABC;

static ABC abc[] = {
  { 'A', (char *)".-", (char *)"Alfa", (char *)"AL-FAH" },
  { 'B', (char *)"-...", (char *)"Bravo", (char *)"BRAH-VOH" },
  { 'C', (char *)"-.-.", (char *)"Charlie", (char *)"CHAR-LEE" },
  { 'D', (char *)"-..", (char *)"Delta", (char *)"DELL-TAH" },
  { 'E', (char *)".", (char *)"Echo", (char *)"ECK-OH" },
  { 'F', (char *)"..-.", (char *)"Foxtrot", (char *)"FOKS-TROT" },
  { 'G', (char *)"--.", (char *)"Golf", (char *)"GOLF" },
  { 'H', (char *)"....", (char *)"Hotel", (char *)"HOH-TEL" },
  { 'I', (char *)"..", (char *)"India", (char *)"IN-DEE-AH" },
  { 'J', (char *)".---", (char *)"Juliet", (char *)"JEW-LEE-ETT" },
  { 'K', (char *)"-.-", (char *)"Kilo", (char *)"KEY-LOH" },
  { 'L', (char *)".-..", (char *)"Lima", (char *)"LEE-MAH" },
  { 'M', (char *)"--", (char *)"Mike", (char *)"MIKE" },
  { 'N', (char *)"-.", (char *)"November", (char *)"NO-VEM-BER" },
  { 'O', (char *)"---", (char *)"Oscar", (char *)"OSS-CAH" },
  { 'P', (char *)".--.", (char *)"Papa", (char *)"PAH-PAH" },
  { 'Q', (char *)"--.-", (char *)"Quebec", (char *)"KEH-BECK" },
  { 'R', (char *)".-.", (char *)"Romeo", (char *)"ROW-ME-OH" },
  { 'S', (char *)"...", (char *)"Sierra", (char *)"SEE-AIR-RAH" },
  { 'T', (char *)"-", (char *)"Tango", (char *)"TANG-GO" },
  { 'U', (char *)"..-", (char *)"Uniform", (char *)"YOU-NEE-FORM" },
  { 'V', (char *)"...-", (char *)"Victor", (char *)"VIK-TAH" },
  { 'W', (char *)".--", (char *)"Whiskey", (char *)"WISS-KEY" },
  { 'X', (char *)"-..-", (char *)"Xray", (char *)"ECKS-RAY" },
  { 'Y', (char *)"-..-", (char *)"Yankee", (char *)"YANG-KEY" },
  { 'Z', (char *)"--..", (char *)"Zulu", (char *)"ZOO-LOO" },
  { '1', (char *)".----", (char *)"One", (char *)"WUN" },
  { '2', (char *)"..---", (char *)"Two", (char *)"TOO" },
  { '3', (char *)"...--", (char *)"Three", (char *)"TREE" },
  { '4', (char *)"....-", (char *)"Four", (char *)"FOW-ER" },
  { '5', (char *)".....", (char *)"Five", (char *)"FIFE" },
  { '6', (char *)"-....", (char *)"Six", (char *)"SIX" },
  { '7', (char *)"--...", (char *)"Seven", (char *)"SEV-EN" },
  { '8', (char *)"---..", (char *)"Eight", (char *)"AIT" },
  { '9', (char *)"----.", (char *)"Nine", (char *)"NIN-ER" },
  { '0', (char *)"-----", (char *)"Zero", (char *)"ZEE-RO" },
  { 0, 0, 0, 0 }
};

char * abc_to_morse(char ch)
{
    PABC pabc = &abc[0];
    while (pabc->morse) {
        if (ch == pabc->abc)
            return pabc->morse;
        pabc++;
    }
    return 0;
}
char * abc_to_telephony(char ch)
{
    PABC pabc = &abc[0];
    while (pabc->morse) {
        if (ch == pabc->abc)
            return pabc->telephony;
        pabc++;
    }
    return 0;
}
char * abc_to_phonetic(char ch)
{
    PABC pabc = &abc[0];
    while (pabc->morse) {
        if (ch == pabc->abc)
            return pabc->phonetic;
        pabc++;
    }
    return 0;
}

char telephony_to_abc(char * pch)
{
    PABC pabc = &abc[0];
    while (pabc->morse) {
        if (strcmp(pch,pabc->telephony) == 0)
            return pabc->abc;
        pabc++;
    }
    return 0;
}
char *telephony_to_morse(char * pch)
{
    PABC pabc = &abc[0];
    while (pabc->morse) {
        if (strcmp(pch,pabc->telephony) == 0)
            return pabc->morse;
        pabc++;
    }
    return 0;
}
char *telephony_to_phonetic(char * pch)
{
    PABC pabc = &abc[0];
    while (pabc->morse) {
        if (strcmp(pch,pabc->telephony) == 0)
            return pabc->phonetic;
        pabc++;
    }
    return 0;
}

QString callsign_to_sentence(QString input)
{
    QString text = input.toUpper(); // just to MAKE sure
    QStringList words;
    QString tmp;
    char * cp;
    char ch;
    QRegExp rxCS("[A-Z0-9]");
    int i, max;
    max = text.size(); // get max size once
    for (i = 0; i < max; i++) {
        tmp = text.at(i); // get string
        ch = tmp.at(0).toLatin1(); // get char
        if (ch == '-')
            continue;   // just skip a 'dash'
        if( !rxCS.exactMatch(tmp) )
            continue;   // not in our league
        cp = abc_to_telephony(ch); // char to ptr
        if ( !cp )
            continue;   // oops, no can do!
        tmp = cp;       // char * to string
        if (tmp.size()) // should always have size!
            words << tmp;   // add to list
    }
    text = words.join(" ");
    return text;
}

QString callsign_to_phonetics(QString input)
{
    QString text = input.toUpper(); // just to MAKE sure
    QStringList words;
    QString tmp;
    char * cp;
    char ch;
    QRegExp rxCS("[A-Z0-9]");
    int i, max;
    max = text.size(); // get max size once
    for (i = 0; i < max; i++) {
        tmp = text.at(i); // get string
        ch = tmp.at(0).toLatin1(); // get char
        if (ch == '-')
            continue;   // just skip a 'dash'
        if( !rxCS.exactMatch(tmp) )
            continue;   // not in our league
        cp = abc_to_phonetic(ch); // char to ptr
        if ( !cp )
            continue;   // oops, no can do!
        tmp = cp;       // char * to string
        if (tmp.size()) // should always have size!
            words << tmp;   // add to list
    }
    text = words.join(" ");
    return text;
}

// =================================

QString util_browseDirectory(QString prompt, QString current, QWidget * parent)
{
    QString filePath =
        QFileDialog::getExistingDirectory(
                parent, prompt, current,
                QFileDialog::ShowDirsOnly);
    return filePath;
}

QString util_browseFile(QString prompt, QString current, QWidget * parent)
{
    QString filePath =
        QFileDialog::getOpenFileName(
                parent, prompt, current );
    return filePath;
}

QString util_browseNewFile(QString prompt, QString current, QWidget * parent)
{
    QString filePath =
        QFileDialog::getSaveFileName(
                parent, prompt, current );
    return filePath;
}

bool util_latInRange(double lat)
{
    if ((lat >= -90.0)&&(lat <= 90.0))
        return true;
    return false;
}
bool util_lonInRange(double lon)
{
    if ((lon >= -180.0)&&(lon <= 180.0))
        return true;
    return false;
}

bool util_inWorldRange(double lat, double lon)
{
    if ((util_latInRange(lat))&&
        (util_lonInRange(lon)))
        return true;
    return false;
}

// eliminate trailing zeros, leaving 1 after decimal
QString util_trimDoubleStg(double d)
{
    QString res;
    res.sprintf("%f",d);
    int ind1 = res.indexOf(".");
    if (ind1 >= 0) {
        ind1++;
        int ind2 = res.indexOf("0",ind1+1);
        if (ind2 > 0) {
            ind1++;
            while ((res.size() > ind1)&&(res.at(res.size()-1) == QChar('0')))
                res.chop(1);    // eliminate trailing zero
        }
    }
    return res;
}

int util_getYesNo( QString title, QString msg, QWidget * parent )
{
    QMessageBox msgBox(parent);
    //msgBox.setText(title); // set TITLE text
    msgBox.setWindowTitle(title);
    msgBox.setIcon(QMessageBox::Information); // set predefined icon, icon is show on left side of text.
    msgBox.setInformativeText(msg); // set the Question
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No); // Add Yes and No buttons.
    msgBox.setDefaultButton(QMessageBox::Yes);  //Set focus on Yes button
    //execute message box. method exec() return the button value of clicked button
    int ret = msgBox.exec();
    //User get input from returned value (ret). you can handle it here.
    switch (ret)
    {
    case QMessageBox::Yes:
        ret = 1;    // 1 for OK to continue
        break;
    case QMessageBox::No:
        ret = 0; // Cancel was clicked
        break;
    default:
        // should never be reached, or maybe with ESCAPE?
        ret = 0; // but assume NO
        break;
    }
    return ret;
}

/* ==========================================
S[Usage: C:\FG\TG2\terragear-cs\msvc\bin\fgfs_construct.exe
[ --output-dir=<directory>
--work-dir=<directory>
--cover=<path to land-cover raster>
--tile-id=<id>
--lon=<degrees>
--lat=<degrees>
--xdist=<degrees>
--ydist=<degrees>
--nudge=<float>
--priorities=<filename>
--usgs-map=<filename>
--useUKgrid
--no-write-shared-edges
--use-own-shared-edges
--ignore-landmass
] <load directory...>
BUT older versions had -
S[Usage: C:\FG\TG2\terragear-cs\msvc\bin\fgfs-construct.exe
[ --output-dir=<directory>
  --work-dir=<directory>
  --cover=<path to land-cover raster>
  --tile-id=<id>
  --lon=<degrees>
  --lat=<degrees>
  --xdist=<degrees>
  --ydist=<degrees>
  --nudge=<float>
  --useUKgrid
 ] <load directory...>
 ======================================== */
int util_lookslike_Fgfs_Construct( QString info ) {
    int flag = 0;
    if (info.contains("Usage") &&
        info.contains("--output-dir=") &&
        info.contains("--work-dir") &&
        info.contains("--tile-id") &&
        info.contains("--useUKgrid") &&
        info.contains("--nudge") )
        flag |= 1;  // got the basics
    if (info.contains("--priorities=") &&
        info.contains("--no-write-shared-edges") &&
        info.contains("--ignore-landmass") )
        flag |= 2; // newer version
    return flag;
}

/* --------------------------------------
Usage hgtchop <resolution> <hgt_file> <work_dir>

        resolution must be either 1 or 3 for 1arcsec or 3arcsec
   -------------------------------------- */
int util_lookslike_hgtchop( QString info )
{
    if ( info.contains("<resolution>") &&
         info.contains("<hgt_file>") &&
         info.contains("<work_dir>"))
        return 1; // has enough of hgtchop's personality ;=))
    return 0; // nope, it ain't a known hgtchop ;=((
}

/* -----------------------------------------

$ terrafit --help
Usage: terrafit [options] <file | path to walk>
         -h | --help
         -m | --minnodes 50
         -x | --maxnodes 1000
         -e | --maxerror 40
         -v | --version

Algorithm will produce at least <minnodes> fitted nodes, but no
more than <maxnodes>.  Within that range, the algorithm will stop
if the maximum elevation error for any remaining point
drops below <maxerror> meters.

Increasing the maxnodes value and/or decreasing maxerror
will produce a better surface approximation.

The input file must be a .arr.gz file such as that produced
by demchop or hgtchop utils.

**** NOTE ****:
If a directory is input all .arr.gz files in directory will be
processed recursively.

The output file(s) is/are called .fit.gz and is simply a list of
from the resulting fitted surface nodes.  The user of the
.fit.gz file will need to retriangulate the surface.
Min points = 50
Max points = 1000
Max error  = 40
Insufficient arguments

  ------------------------------------------- */
int util_lookslike_terrafit( QString info )
{
    if ( info.contains(" --minnodes ") &&
         info.contains(" --maxnodes ") &&
         info.contains(" --maxerror "))
        return 1; // has enough of terrafit's personality ;=))
    return 0; // nope, it ain't a known terrafit ;=((
}

/* ********************************************************
Usage: shape-decode [--line-width width] [--point-width width] [--area-column col] [--code-col col] [--line-width-column col ]  [--continue-on-errors] [--max-segment max_segment_length] [--start-record num] <shape_file> <work_dir> [ area_string ]
Options:
--line-width width
        Width in meters for the lines
--point-width width
        Size in meters of the squares generated from points
--max-segment max_segment_length
        Maximum segment length in meters
--area-column col
        Get areatype for objects from column number col
        in associated dbf-file (only if <area_string> is not given)
--code-column col
        Get codetype for objects from column number col
        in associated dbf-file
        (currently code is only used in debug printouts)
--continue-on-errors
        Continue even if the file seems fishy

--start-record record-number
        Start processing at the specified record number (first record num=0)
--end-record record-number
        End processing at (immediately after) the specified record number (first record num=0)

<shape_file>
        Name of the shape-file to process, without .shp extension
<work_dir>
        Directory to put the polygon files in
<area_string>
        (Optional) Area type for all objects in file
        Overrides --area-column option if present

   ******************************************************** */
int util_lookslike_shape_decode( QString info )
{
    if ( info.contains("--line-width ") &&
         info.contains("--point-width ") &&
         info.contains("--area-column ") &&
         info.contains("--continue-on-errors") )
        return 1; // has enough shape-code, and hopefull ogr-decode personality
    return 0;
}

/* *************************************************************
~/projects$ genapts --help
genapts generates airports for use in generating scenery for the FlightGear flight simulator.  Airport, runway, and taxiway vector data and attributes are input, and generated 3D airports are output for further processing by the TerraGear scenery creation tools.

The standard input file is runways.dat.gz which is found in $FG_ROOT/Airports.  This file is periodically generated for the FlightGear project by Robin Peel, who maintains an airport database for both the X-Plane and FlightGear simulators.  The format of this file is documented on the FlightGear web site.  Any other input file corresponding to this format may be used as input to genapts.  Input files may be gzipped or left as plain text as required.

Processing all the world's airports takes a *long* time.  To cut down processing time when only some airports are required, you may refine the input selection either by airport or by area.  By airport, either one airport can be specified using --airport=abcd, where abcd is a valid airport code eg. --airport-id=KORD, or a starting airport can be specified using --start-id=abcd where once again abcd is a valid airport code.  In this case, all airports in the file subsequent to the start-id are done.  This is convienient when re-starting after a previous error.
An input area may be specified by lat and lon extent using min and max lat and lon.  Alternatively, you may specify a chunk (10 x 10 degrees) or tile (1 x 1 degree) using a string such as eg. w080n40, e000s27.
An input file containing only a subset of the world's airports may of course be used.

It is necessary to generate the elevation data for the area of interest PRIOR TO GENERATING THE AIRPORTS.  Failure to do this will result in airports being generated with an elevation of zero.  The following subdirectories of the work-dir will be searched for elevation files:

SRTM2-Africa-3
SRTM2-Australia-3
SRTM2-Eurasia-3
SRTM2-Islands-3
SRTM2-North_America-3
SRTM2-South_America-3
DEM-USGS-3
SRTM-1
SRTM-3
SRTM-30

Usage genapts --input=<apt_file> --work=<work_dir> [ --start-id=abcd ] [ --nudge=n ]
[--min-lon=<deg>] [--max-lon=<deg>] [--min-lat=<deg>] [--max-lat=<deg>] [--clear-dem-path]
[--dem-path=<path>] [--max-slope=<decimal>] [ --airport=abcd ]  [--tile=<tile>]
[--chunk=<chunk>] [--verbose] [--help]

   ************************************************************* */
int util_lookslike_genapts( QString info )
{
    int flag = 0;
    if ( info.contains("--input=") &&
         info.contains("--work=") &&
         info.contains("--start-id=") &&
         info.contains("--min-lon=") &&
         info.contains("--max-lon=") &&
         info.contains("--min-lat=") &&
         info.contains("--airport=") &&
         info.contains("--tile=") &&
         info.contains("--chunk=") &&
         info.contains("--max-lat=") &&
         info.contains("--nudge=") )
        flag |= 1; // has enough genapts personality
    if ( info.contains("--clear-dem-path") &&
         info.contains("--dem-path="))
        flag |= 2; // this is a later version
    return flag;
}

bool util_verifyProjDir(QString dir)
{
    QDir d;
    if (d.exists(dir))
        return true;
    return false;
}

// eof - utilities.cpp
