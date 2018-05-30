/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */

// tg_util.cpp

#include "tg_utils.h"
#include "utilities.h"
#include "newbucket.h"
#include "srtm_utils.h" // for LL struct

bool m_User_Break = false;

// list of correpsonding materials
QStringList tg_getMaterialList()
{
    QStringList csMater;
    // *TODO* - should compare this to default_priorities.txt
    csMater << "AgroForest" << "Airport" << "Asphalt" << "BarrenCover" << "Bog" <<
            "Burnt" << "Canal" << "Cemetery" << "ComplexCrop" << "Construction" <<
            "CropGrass" << "DeciduousForest" << "Default" << "Dirt" << "DryCrop" <<
            "Dump" << "Estuary" << "EvergreenForest" << "FloodLand" << "Freeway" <<
            "Glacier" << "GolfCourse" << "GrassLand" << "GreenSpace" << "Heath" <<
            "HerbTundra" << "Industrial" << "IntermittentLake" << "IntermittentStream" <<
            "IrrCrop" << "Lagoon" << "Lake" << "Lava" << "Littoral" << "Marsh" <<
            "MixedCrop" << "MixedForest" << "NaturalCrop" << "Olives" << "OpenMining" <<
            "Orchard" << "PackIce" << "PolarIce" << "Port" << "Railroad" << "Railroad" <<
            "RainForest" << "Rice" << "Road" << "Rock" << "Saline" << "SaltMarsh" <<
            "Sand" << "Sclerophyllous" << "ScrubCover" << "Stream" << "SubUrban" << "Town" <<
            "Transport" << "Urban" << "Vineyard" << "Watercourse" << "Landmass";
    return csMater;
}

// list of custom scenery shapefiles
QStringList tg_getCustomShapes()
{
    QStringList csShape;
    csShape << "cs_agroforest" << "cs_airport" << "cs_asphalt" << "cs_barrencover" << "cs_bog" <<
            "cs_burnt" << "cs_canal" << "cs_cemetery" << "cs_complexcrop" << "cs_construction" <<
            "cs_cropgrass" << "cs_deciduousforest" << "cs_default" << "cs_dirt" << "cs_drycrop" <<
            "cs_dump" << "cs_estuary" << "cs_evergreenforest" << "cs_floodland" << "cs_freeway" <<
            "cs_glacier" << "cs_golfcourse" << "cs_grassland" << "cs_greenspace" << "cs_heath" <<
            "cs_hebtundra" << "cs_industrial" << "cs_intermittentlake" << "cs_intermittentstream" <<
            "cs_irrcrop" << "cs_lagoon" << "cs_lake" << "cs_lava" << "cs_littoral" << "cs_marsh" <<
            "cs_mixedcrop" << "cs_mixedforest" << "cs_naturalcrop" << "cs_olives" << "cs_openmining" <<
            "cs_orchard" << "cs_packice" << "cs_polarice" << "cs_port" << "cs_railroad1" << "cs_railroad2" <<
            "cs_rainforest" << "cs_rice" << "cs_road" << "cs_rock" << "cs_saline" << "cs_saltmarsh" <<
            "cs_sand" << "cs_sclerophyllous" << "cs_scrub" << "cs_stream" << "cs_suburban" << "cs_town" <<
            "cs_transport" << "cs_urban" << "cs_vineyard" << "cs_watercourse" << "v0_landmass";
    return csShape;
}


bool tg_verifyProjDir(QString path)
{
    QDir d;
    if ((path.size() == 0)|| // MAIN project directory
        ( !d.exists(path) )) {
        QMessageBox::critical(0,"NO PROJECT DIECTORY",
"The main PROJECT DIRECTORY, has NOT been set, OR is INVALID!\n"
"Can NOT proceed to process anything until this is fixed!\n\n"
"Return to the SetUp page, to correct this problem.");
        return false;
    }
    return true;
}

bool tg_createDirectory(QString path)
{
    QDir d;
    if ( ! d.exists(path) ) {
        //download.mkdir(path); // hmmmm this FAILS
        d.mkpath(path); // but this seems ok???
        if ( ! d.exists(path) ) {
            QString info;
            info = "Failed to create ["+path+"]!\n";
            info += "Can only abort the action.\n";
            info += "Perhaps change the 'Project Directory'\n";
            info += "and/or check permissions on this path.\n";
            QMessageBox::critical(0,"MAKE PATH FAILED", info);
            return false;
        }
    }
    return true;
}

// QString to const char *
size_t QString_to_buffer(const QString & str1, char *buf, size_t buflen )
{
    // QString str1 = "Test";
    QByteArray ba = str1.toLatin1();
    const char *c_str2 = ba.data();
    size_t len = strlen(c_str2);
    //printf("str2: %s", c_str2);
    if (len > buflen) {
        strncpy(buf, c_str2, buflen);
        buf[buflen - 1] = 0;
    } 
    else
        strcpy(buf,c_str2);
    return len;
}
#define MY_MAX_BUF 1024
void show_QString(const QString & msg)
{
    char buf[MY_MAX_BUF+16];
    QString_to_buffer(msg, buf, MY_MAX_BUF);
}

// ***TODO*** Change to use the NEW testExe class
int tg_testApplication(QString cmd, QString & msg)
{
    int errCode = -2;
    QTime rt;
    QString tm;
    QString std;
    QString err;

    rt.start();
    tm = "";
    msg = "Running ["+cmd+"]\n";
    //show_QString(msg);
    QProcess proc;
    // run command
    proc.start(cmd, QIODevice::ReadWrite);
    //proc.waitForReadyRead();
    if (proc.waitForStarted()) {
        if (proc.QProcess::waitForFinished()) {
            errCode = proc.exitCode();
            std = proc.readAllStandardOutput();
            err = proc.readAllStandardError();
            std.trimmed();
            std = std.simplified();
            err = err.simplified();
            if ((std.size() == 0)&&(err.size() == 0)) {
                msg += "No 'stdout' or 'errout' text!\n";
            } else {
                if (std.size())
                    msg += "S["+std+"]\n";
                if (err.size())
                    msg += "E["+err+"]\n";
            }
            tm.sprintf(" for %d ms.", rt.elapsed());
            msg += "*PROC_ENDED*"+tm+"\n";
        } else {
            msg += "proc.QProcess::waitForFinished() returned FALSE";
        }
    } else {
        msg += "proc.waitForStarted() returned FALSE";
    }
    return errCode;
}

bool tg_trimLongMessage( int max, QString & info )
{
    bool iret = false;
    if ((max > 10) && (info.size() > max)) {
        // we need to CHOP some info
        // 1: write the FULL string to the LOG
        outLog("Chop =["+info+"]=FULL\n"); // to  =["+tmp+"]=\n");
        // 2: try just 'simplify'
        info = info.simplified();
        if (info.size() > max) {
            // 3: ok, really TRUCATE
            QString tmp = info.left(max / 2);
            tmp += " ... ";
            tmp += info.right(max / 2);
            info = tmp;
            iret = true;
        }
    }
    return iret;
}

// rename 'file' to file.OLD, file.BAK, file.BAK1, file.BAK2, etc
// never deletes anything
// returns -1 if error
// returns 0 if nothing done
// returns 1 if renamed to OLD
// returns 2 if renamed to BAK
// returns 3++ if renamed to BAK+number
int tg_renameToOLDBAK(QString file)
{
    QFile f(file);
    if (f.exists()) {
        QString filNew = file + ".old";
        if (f.exists(filNew)) {
            filNew = file + ".bak";
            if (f.exists(filNew)) {
                int i = 1;
                QString ext;
                ext.sprintf(".%d",i);
                filNew = ".bak"+ext;
                while (f.exists(filNew)) {
                    i++;
                    ext.sprintf(".%d",i);
                    filNew = ".bak"+ext;
                }
                if (f.rename(file,filNew))
                    return 2+i;
                else
                    return -1;
            } else {
                if (f.rename(file,filNew))
                    return 2;
                else
                    return -1;
            }
        } else {
            if (f.rename(file,filNew))
                return 1;
            else
                return -1;
        }
    }
    return 0;
}

// need QApplication::processEvents(); somewhere
// run a process with 'arguments', in 'dir', if any
// ***TODO*** need to run this process on a QThread
QString tg_runProcess(QString arguments, QString dir, int * pErr)
{
    QTime rt;
    QString info;
    QString tm;
    rt.start();
    QProcess proc;
    int errCode = -2;
    QString std;
    QString err;
    if (dir.size())
        proc.setWorkingDirectory(dir);
    proc.start(arguments, QIODevice::ReadWrite);
    // proc.waitForReadyRead();
    if (proc.waitForStarted()) {
        // proc.QProcess::waitForFinished(-1);
        proc.waitForFinished(-1);
        errCode = proc.exitCode();
        std = proc.readAllStandardOutput();
        err = proc.readAllStandardError();
    } else {
        info = "waitForStarted() FAILED or timed out!";
        errCode = -2;
    }
    tm = " in "+getElapTimeStg(rt.elapsed());
    if (errCode) {
        info.sprintf("*PROC_ENDED* with error %d",errCode);
    } else {
        info = "*PROC_ENDED*";
    }
    info += tm+"\n";
    info += "arg="+arguments;
    if (dir.size())
        info += ", in "+dir;
    info += "\n";
    // std.trimmed();
    std.simplified();
    err.simplified();
    if (std.size())
        info += "stdout=["+std+"]";
    if (err.size())
        info += "errout=["+err+"]";
    if (pErr)
        *pErr = errCode;
    return info;
}

void tg_openNonModalDialog(QString title, QString msg, int secs, QWidget *parent)
{

    QString tm;
    tm.sprintf("\n\nThis dialog will close after %d seconds.\n",secs);
    QDialog *d = new QDialog(parent);

    d->setModal(false);
    d->setWindowTitle(title);
    QVBoxLayout *l = new QVBoxLayout(d);
    QLabel * pLab = new QLabel(d);
    // pLab->setSizePolicy(QSizePolicy::ExpandFlag);
    pLab->setWordWrap(true);
    pLab->setText(msg+tm);

#ifdef ADD_PUSH_BUTTONS
  QPushButton *button = new QPushButton;
  button->setText("Yes");
  connect(button, SIGNAL(clicked()), SLOT(close()));
  l->addWidget(button);
  button = new QPushButton;
  button->setText("No");
  connect(button, SIGNAL(clicked()), SLOT(close()));
  l->addWidget(button);
#endif

    QPushButton *button = new QPushButton;
    button->setText("Abort");
    // connect(button, SIGNAL(clicked()), parent, SLOT(dialog_abort_clicked()));

    l->addWidget(pLab);
    l->addWidget(button);
    //d->setFocusPolicy(Qt::FocusPolicy);
    d->show();
    d->repaint();
    // qApp->processEvents();

    QTime to;
    to.start();
    int i = 0;
    int sec_cnt = 0;
    int last_sec = 0;
    int elap;
    int ms = secs * 1000;
    elap = 0; // to.elapsed();
    while (!m_User_Break && (elap < ms)) {
        i++;
        elap = to.elapsed();
        sec_cnt = elap / 1000;
        d->update();
        if (sec_cnt != last_sec) {
            last_sec = sec_cnt;
            tm.sprintf("\n\nThis dialog will close after %d seconds.",secs - last_sec);
            pLab->setText(msg+tm);
            d->repaint();
            //qApp->processEvents();
        }
        if (m_User_Break)
            break;
    }

    d->close();
    delete d;
    //qApp->processEvents();
}

bool tg_ValidTileId(QString tileId, PLL pll) 
{
    // expect [e|w]nnn[n|s]nn tile ID
    // substring, then left(), mid() and right()
    QString ew = tileId.left(1);
    int ilon = tileId.mid(2, 3).toInt();
    QString ns = tileId.mid(5, 1);
    int ilat = tileId.right(1).toInt();
    if (((ew == "e") || (ew == "E") || ((ew == "w") || (ew == "W"))) &&
        ((ns == "s") || (ns == "S") || ((ns == "n") || (ns == "N"))) ) {
        // there is a chance...
        double lat = (double)ilat;
        double lon = (double)ilon;
        if ((ew == "w") || (ew == "W"))
            lon *= -1;
        if ((ns == "s") || (ns == "S"))
            lat *= -1;
    }
    return false;

}

bool tg_ValidChunkId(QString chunkId, PLL pll)
{
    // TODO: valid chunk
    if (tg_ValidTileId(chunkId, pll)) {

        // maybe return true
    }
    return false;
}
// eof - tg_utils.cpp
