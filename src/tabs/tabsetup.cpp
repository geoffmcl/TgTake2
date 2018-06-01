/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#include "app_config.h"
#include "tabsetup.h"
#include "mainwindow.h"
#include <QSpacerItem>
#include <QSizePolicy>
#include "utilities/srtm_utils.h"
#include "utilities/workThread.h"
#include <math.h>

tabSetup::tabSetup(QWidget *parent) :
    QWidget(parent)
{
    int left  = 5;
    int top   = 5;
    int right = 5;
    int bott  = 5;
    QString tmp;

    main = (MainWindow *)parent;
    setup_state = 0;
    m_pwsu = 0;

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(left, top, right, bott);
    mainLayout->setSpacing(0);
    //centralWidget()->setLayout(mainLayout);
    QLayout *playout = layout();
    if (playout) {
        playout->addItem(mainLayout);
        outLog("Added item to existing layout");
    } else {
        setLayout(mainLayout);
        outLog("SetLayout to mainLayout");
    }

    QGridLayout *fileLayout = new QGridLayout;
    fileLayout->setContentsMargins(10, 0, 10, 0);
    fileLayout->setSpacing(0);
    //mainLayout->addLayout(fileLayout);

    QGroupBox *fileGroup = new QGroupBox(tr("Primary Directory Setting - green=ok, red=not valid"));
    // Project directory
    projdirLabel = new QLabel(tr("Project Directory:"));
    projdirLabel->setStatusTip("Primary scenery output directory");
    tmp = main->m_Settings->value(dirsprojectPath,"").toString();
    m_projdirEdit = new QLineEdit(tmp,this);
    m_projdirEdit->setStatusTip("Primary scenery output directory");
    connect(m_projdirEdit,SIGNAL(editingFinished()),this,SLOT(on_projdir_edit()));
    projdirBrowse = new QPushButton("...",this);
    projdirBrowse->setStatusTip("Primary scenery output directory");
    connect(projdirBrowse,SIGNAL(pressed()),this,SLOT(on_projdir_browse()));

    // TG Tool directory
    tgtoolLabel = new QLabel(tr("TG Tool Directory:"));
    tgtoolLabel->setStatusTip("TerraGear binary directory");
    tmp = main->m_Settings->value(dirstgtoolsPath,"").toString();
    m_tgtoolEdit = new QLineEdit(tmp,this);
    m_tgtoolEdit->setStatusTip("TerraGear binary directory");
    connect(m_tgtoolEdit,SIGNAL(editingFinished()),this,SLOT(on_tgtool_edit()));
    tgtoolBrowse = new QPushButton("...",this);
    tgtoolBrowse->setStatusTip("TerraGear binary directory");
    connect(tgtoolBrowse,SIGNAL(pressed()),this,SLOT(on_tgtool_browse()));

    // FG root directory
    fgrootLabel = new QLabel(tr("FG Root Directory:"));
    fgrootLabel->setStatusTip("FlightGear root data directory");
    tmp = main->m_Settings->value(dirsfgrootPath,"").toString();
    m_fgrootEdit = new QLineEdit(tmp,this);
    m_fgrootEdit->setStatusTip("FlightGear root data directory");
    connect(m_fgrootEdit,SIGNAL(editingFinished()),this,SLOT(on_fgroot_edit()));
    fgrootBrowse = new QPushButton("...",this);
    fgrootBrowse->setStatusTip("FlightGear root data directory");
    connect(fgrootBrowse,SIGNAL(pressed()),this,SLOT(on_fgroot_browse()));

    // HGT root directory - elevations directory - always searched, after local downloads/unzips
    hgtrootLabel = new QLabel(tr("HGT Root:"));
    hgtrootLabel->setStatusTip("HGT Elevations Root Directory");
    tmp = main->m_Settings->value(dirshgtrootPath, "").toString();
    m_hgtrootEdit = new QLineEdit(tmp, this);
    m_hgtrootEdit->setStatusTip("HGT Elevations Root Directory");
    connect(m_hgtrootEdit, SIGNAL(editingFinished()), this, SLOT(on_hgtroot_edit()));
    hgtrootBrowse = new QPushButton("...", this);
    hgtrootBrowse->setStatusTip("HGT Elevations Root Directory");
    connect(hgtrootBrowse, SIGNAL(pressed()), this, SLOT(on_hgtroot_browse()));

    int row = 0;
    fileLayout->addWidget(projdirLabel,row,0);
    fileLayout->addWidget(m_projdirEdit,row,1);
    fileLayout->addWidget(projdirBrowse,row,2);

    row++;
    fileLayout->addWidget(tgtoolLabel,row,0);
    fileLayout->addWidget(m_tgtoolEdit,row,1);
    fileLayout->addWidget(tgtoolBrowse,row,2);

    row++;
    fileLayout->addWidget(fgrootLabel,row,0);
    fileLayout->addWidget(m_fgrootEdit,row,1);
    fileLayout->addWidget(fgrootBrowse,row,2);

    row++;
    fileLayout->addWidget(hgtrootLabel, row, 0);
    fileLayout->addWidget(m_hgtrootEdit, row, 1);
    fileLayout->addWidget(hgtrootBrowse, row, 2);

    fileGroup->setLayout(fileLayout);
    mainLayout->addWidget(fileGroup);

    QGroupBox *limitsGroup = new QGroupBox(tr("Scenery Limits"));

    mmLabel = new QLabel(tr(""));
    minLabel = new QLabel(tr("Minimum"));
    maxLabel = new QLabel(tr("Maximum"));

    latLabel = new QLabel(tr("Latitude:"));
    tmp = main->m_Settings->value(minlatPath,"").toString();
    minlatEdit = new QLineEdit(tmp,this);
    connect(minlatEdit, SIGNAL(editingFinished()), this, SLOT(on_range_change()));
    connect(minlatEdit, SIGNAL(textChanged(QString)), this, SLOT(on_range_change()));

    tmp = main->m_Settings->value(maxlatPath,"").toString();
    maxlatEdit = new QLineEdit(tmp,this);
    connect(maxlatEdit, SIGNAL(editingFinished()), this, SLOT(on_range_change()));
    connect(maxlatEdit, SIGNAL(textChanged(QString)), this, SLOT(on_range_change()));

    lonLabel = new QLabel(tr("Longitude:"));

    tmp = main->m_Settings->value(minlonPath,"").toString();
    minlonEdit = new QLineEdit(tmp,this);
    connect(minlonEdit, SIGNAL(editingFinished()), this, SLOT(on_range_change()));
    connect(minlonEdit, SIGNAL(textChanged(QString)), this, SLOT(on_range_change()));

    tmp = main->m_Settings->value(maxlonPath,"").toString();
    maxlonEdit = new QLineEdit(tmp,this);
    connect(maxlonEdit, SIGNAL(editingFinished()), this, SLOT(on_range_change()));
    connect(maxlonEdit, SIGNAL(textChanged(QString)), this, SLOT(on_range_change()));

    QGridLayout *limitsLayout = new QGridLayout;
    limitsLayout->setContentsMargins(10, 0, 10, 0);
    limitsLayout->setSpacing(0);

    row = 0;
    int wid = 30;
    int fac = 3;
    QSpacerItem * sp;
    sp = new QSpacerItem(wid*fac,14);
    limitsLayout->addItem(sp,row,0);
    limitsLayout->addWidget(mmLabel,row,1);
    sp = new QSpacerItem(wid,14);
    limitsLayout->addItem(sp,row,2);
    limitsLayout->addWidget(minLabel,row,3);
    sp = new QSpacerItem(wid,14);
    limitsLayout->addItem(sp,row,4);
    limitsLayout->addWidget(maxLabel,row,5);
    sp = new QSpacerItem(wid*fac,14);
    limitsLayout->addItem(sp,row,6);

    row++;
    sp = new QSpacerItem(wid*fac,14);
    limitsLayout->addItem(sp,row,0);
    limitsLayout->addWidget(latLabel,row,1);
    sp = new QSpacerItem(wid,14);
    limitsLayout->addItem(sp,row,2);
    limitsLayout->addWidget(minlatEdit,row,3);
    sp = new QSpacerItem(wid,14);
    limitsLayout->addItem(sp,row,4);
    limitsLayout->addWidget(maxlatEdit,row,5);
    sp = new QSpacerItem(wid*fac,14);
    limitsLayout->addItem(sp,row,6);

    row++;
    sp = new QSpacerItem(wid*fac,14);
    limitsLayout->addItem(sp,row,0);
    limitsLayout->addWidget(lonLabel,row,1);
    sp = new QSpacerItem(wid,14);
    limitsLayout->addItem(sp,row,2);
    limitsLayout->addWidget(minlonEdit,row,3);
    sp = new QSpacerItem(wid,14);
    limitsLayout->addItem(sp,row,4);
    limitsLayout->addWidget(maxlonEdit,row,5);
    sp = new QSpacerItem(wid*fac,14);
    limitsLayout->addItem(sp,row,6);

    limitsGroup->setLayout(limitsLayout);
    mainLayout->addWidget(limitsGroup);

    QGroupBox *currentGroup = new QGroupBox(tr("Current Limits"));
    QGridLayout *currentLayout = new QGridLayout;
    currentLayout->setContentsMargins(10, 0, 10, 0);
    //currentLayout->setSpacing(0);
    currentLayout->setHorizontalSpacing(4);
    // current user values
    QSizePolicy szpol;
    szpol.setVerticalStretch(0);
    centlat1Label = new QLabel(tr("Center Lat:"));
    centlat2Label = new QLabel(tr("     "));
    centlat2Label->setFrameShape(QFrame::Box);
    centlat2Label->setSizePolicy(szpol);
    centlon1Label = new QLabel(tr("Center Lon:"));
    centlon2Label = new QLabel(tr("     "));
    centlon2Label->setFrameShape(QFrame::Box);
    centlon2Label->setSizePolicy(szpol);
    xdist1Label = new QLabel(tr("xdist:"));
    xdist2Label = new QLabel(tr("      "));
    xdist2Label->setFrameShape(QFrame::Box);
    xdist2Label->setSizePolicy(szpol);
    ydist1Label = new QLabel(tr("ydist:"));
    ydist2Label = new QLabel(tr("      "));
    ydist2Label->setFrameShape(QFrame::Box);
    ydist2Label->setSizePolicy(szpol);

    row = 0;
    currentLayout->addWidget(centlat1Label,row,0);
    currentLayout->addWidget(centlat2Label,row,1);

    currentLayout->addWidget(centlon1Label,row,2);
    currentLayout->addWidget(centlon2Label,row,3);

    currentLayout->addWidget(xdist1Label,row,4);
    currentLayout->addWidget(xdist2Label,row,5);

    currentLayout->addWidget(ydist1Label,row,6);
    currentLayout->addWidget(ydist2Label,row,7);

    row++;
    srtm1Label = new QLabel(tr("SRTM Range:"));
    currentLayout->addWidget(srtm1Label,row,0);
    srtm2Label = new QLabel(tr("           "));
    srtm2Label->setFrameShape(QFrame::Box);
    srtm2Label->setSizePolicy(szpol);
    currentLayout->addWidget(srtm2Label,row,1,1,1);
    srtm3Label = new QLabel(tr("           "));
    srtm3Label->setFrameShape(QFrame::Box);
    srtm3Label->setSizePolicy(szpol);
    currentLayout->addWidget(srtm3Label,row,2,1,1);
    srtm4Label = new QLabel(tr("           "));
    currentLayout->addWidget(srtm4Label,row,3,1,5);

    currentGroup->setLayout(currentLayout);
    mainLayout->addWidget(currentGroup);

//#ifdef ADD_SETUP_INFO
    // ===================================
    // INFORMATION
    // ===================================
    int spac = 4;
    QGroupBox *infoGroup = new QGroupBox(tr("Information"),this);
    mainLayout->addWidget(infoGroup);

    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->setContentsMargins(left, top, right, bott);
    infoLayout->setSpacing(spac);
    infoGroup->setLayout(infoLayout);

    infoEdit = new QPlainTextEdit(this);
    infoEdit->setReadOnly(true);

    infoLayout->addWidget(infoEdit);
//#endif // #ifdef ADD_SETUP_INFO


}

void tabSetup::finalize()
{
    // try short label names
    projdirLabel->setText(tr("Project:"));
    tgtoolLabel->setText(tr("TG Tools:"));
    fgrootLabel->setText(tr("FG Root:"));
    on_projdir_edit();
    on_tgtool_edit();
    on_fgroot_edit();
    on_hgtroot_edit();
    on_range_change();
}

void tabSetup::on_projdir_edit()
{
    QString tmp;
    QDir f;
    QString style;
    tmp = m_projdirEdit->text();
    setup_state &= suf_scene;
    if (f.exists(tmp)) {
        style = "color:green";
        saveSetting(dirsprojectPath,tmp);
        setup_state |= suf_scene;
    } else {
        style = "color:red";
    }
    projdirLabel->setStyleSheet(style);
}

void tabSetup::on_projdir_browse()
{
    main->setStatusMessage("Set Project Directory",5000);
    QString prompt(tr("Select Project Directory"));
    QString current = m_projdirEdit->text();
    QString filePath = util_browseDirectory(prompt,current);
    QDir d;
    if (!filePath.isEmpty() && d.exists(filePath)) {
        m_projdirEdit->setText(filePath);
        on_projdir_edit();
    }

}

void tabSetup::on_tgtool_edit()
{
    QString tmp;
    QDir d;
    QString style;
    tmp = m_tgtoolEdit->text();
    setup_state &= ~suf_tools;
    if (d.exists(tmp)) {
        style = "color:green";
        saveSetting(dirstgtoolsPath,tmp);
        setup_state |= suf_tools;
    } else {
        style = "color:red";
    }
    tgtoolLabel->setStyleSheet(style);
}

void tabSetup::on_tgtool_browse()
{
    main->setStatusMessage("Set TG Tool Directory",5000);
    QString prompt(tr("Select TG Tool Directory"));
    QString current = m_tgtoolEdit->text();
    QString filePath = util_browseDirectory(prompt,current);
    QDir d;
    if (!filePath.isEmpty() && d.exists(filePath)) {
        m_tgtoolEdit->setText(filePath);
        on_tgtool_edit();
    }
}

void tabSetup::on_fgroot_edit()
{
    QString tmp;
    QDir d;
    QString style;
    tmp = m_fgrootEdit->text();
    setup_state &= ~suf_root;
    if (d.exists(tmp)) {
        style = "color:green";
        saveSetting(dirsfgrootPath,tmp);
        setup_state |= suf_root;
    } else {
        style = "color:red";
    }
    fgrootLabel->setStyleSheet(style);
}

void tabSetup::on_fgroot_browse()
{
    //main->m_statusBar->showMessage("Set FG Root Directory",5000);
    main->setStatusMessage("Set FG Root Directory",5000);
    QString prompt(tr("Select FG Rool Directory"));
    QString current = m_fgrootEdit->text();
    QString filePath = util_browseDirectory(prompt,current,this);
    QDir d;
    if (!filePath.isEmpty() && d.exists(filePath)) {
        m_fgrootEdit->setText(filePath);
        on_fgroot_edit();
    }
}

void tabSetup::on_hgtroot_edit()
{
    QString tmp;
    QDir d;
    QString style;
    tmp = m_hgtrootEdit->text();
    setup_state &= ~suf_hgtroot;
    if (d.exists(tmp)) {
        style = "color:green";
        saveSetting(dirshgtrootPath, tmp);
        setup_state |= suf_hgtroot;
    }
    else {
        style = "color:red";
    }
    hgtrootLabel->setStyleSheet(style);
}

void tabSetup::on_hgtroot_browse()
{
    //main->m_statusBar->showMessage("Set FG Root Directory",5000);
    main->setStatusMessage("Set HGT Root Directory", 5000);
    QString prompt(tr("Select HGT Rool Directory"));
    QString current = m_hgtrootEdit->text();
    QString filePath = util_browseDirectory(prompt, current, this);
    QDir d;
    if (!filePath.isEmpty() && d.exists(filePath)) {
        m_hgtrootEdit->setText(filePath);
        on_hgtroot_edit();
    }
}

void tabSetup::validate_dirs()
{
    on_projdir_edit();
    on_tgtool_edit();
    on_fgroot_edit();
}

void tabSetup::saveSetting(QString path, QString value)
{
    main->m_Settings->setValue(path,value);
}

// msg.append(srtm_getSRTMListstg(dsouth, dwest, dnorth, deast));
void tabSetup::getSRTMList()
{
    PWORKsu pwsu = m_pwsu;
    bool is_same = false;
    if (pwsu) {
        if ((fabs(pwsu->south - m_minLat) < APP_EPSILON)&&
            (fabs(pwsu->west - m_minLon) < APP_EPSILON)&&
            (fabs(pwsu->north - m_maxLat) < APP_EPSILON)&&
            (fabs(pwsu->east - m_maxLon) < APP_EPSILON)) {
            is_same = true;
        }
        if (!pwsu->work.work_done) {
            if (!is_same) {
                // ***TBD*** need to stop current, and start this, or re-schedule for later
            }
            return; // still running the previous
        }
        if (is_same) {
            // we have a NO CHANGE condition - so do NOT waste time re-doing it
            pwsu->work.work_tt.start();
            pwsu->work.work_done = false;
            pwsu->work.abort_work = false;
            main->on_work_done(pwsu->work.work_num,0);
            return;
        }
    }
    // ok, we are re-doing this
    pwsu = new WORKsu;  // create worker thread block
    m_pwsu = pwsu;
    // clear the list
    pwsu->south = m_minLat; // south;
    pwsu->west  = m_minLon; // west;
    pwsu->north = m_maxLat; // north;
    pwsu->east  = m_maxLon; // east;
    pwsu->i_Hgt_Cnt = 0;
    pwsu->hgturlList.clear();
    pwsu->hgtroot.clear();
    QString tmp = main->m_tabSetup->m_hgtrootEdit->text();
    if (tmp.size())
        pwsu->hgtroot.push_back(tmp);

    pwsu->result = "";

    pwsu->work.work_type = WT_SRTMCNT;
    pwsu->work.work_desc = "Est. SRTM count";
    pwsu->work.work_done = false;
    pwsu->work.abort_work = false;

    // ***TBD*** should maybe STOP any previous calculation in progress - These calculations can take 2-4 minutes
    pwsu->work.work_tt.start();
    main->appendWorkList((PWORK)pwsu);

    pwsu->work.work_num = main->runWorker(getSRTMStg,pwsu);
    main->setStatusMessage("Began SRTM count...",5000);
}

// static - run on a THREAD
void tabSetup::getSRTMStg(void * vp)
{
    PWORKsu pwsu = (PWORKsu)vp;
    // ***TBD*** Should probably get and keep some more
    // of the information from this very heavy service
    pwsu->result = srtm_getSRTMListstg(pwsu);
}

bool tabSetup::isInitThreadDone()
{
    PWORKsu pwsu = m_pwsu;
    if (pwsu) {
        if (pwsu->work.work_done)
            return true;
    }
    return false;
}

void tabSetup::setSRTMStg(PWORK pw)
{
    PWORKsu pwsu = (PWORKsu)pw;
    if (pwsu->result.length()) {
        srtm4Label->setText(pwsu->result);
        main->m_tabElevations->update_information();
    }
}

#define dnorth m_maxLat
#define dsouth m_minLat
#define deast  m_maxLon
#define dwest  m_minLon

void tabSetup::on_range_change()
{
    QString msg;
    QString style;
    QString sminlat(minlatEdit->text());
    QString smaxlat(maxlatEdit->text());
    QString sminlon(minlonEdit->text());
    QString smaxlon(maxlonEdit->text());
    m_minLat = sminlat.toDouble();
    m_maxLat = smaxlat.toDouble();
    m_minLon = sminlon.toDouble();
    m_maxLon = smaxlon.toDouble();
    setup_state &= ~suf_ranges;
    if ((util_inWorldRange(m_minLat, m_minLon))&&
        (util_inWorldRange(m_maxLat, m_maxLon))&&
        (m_minLat <= m_maxLat)&&
        (m_minLon <= m_minLon)&&
        !sminlat.isEmpty() && !smaxlat.isEmpty() &&
        !sminlon.isEmpty() && !smaxlon.isEmpty() &&
        ((fabs(m_maxLat - m_minLat) > APP_EPSILON)||(fabs(m_maxLon - m_minLon) > APP_EPSILON)) )
    {
        double centlat = (m_minLat + m_maxLat) / 2.0;
        double centlon = (m_minLon + m_maxLon) / 2.0;
        double latdiff = m_maxLat - m_minLat;
        double londiff = m_maxLon - m_minLon;
        /* --- construction
            minlat1Label = new QLabel("Center Lat:",this);
            minlat2Label = new QLabel("         ",this);
            minlon1Label = new QLabel("Center Lon:",this);
            minlon2Label = new QLabel("         ",this);
            maxlat1Label = new QLabel("x-distance:",this);
            maxlat2Label = new QLabel("         ",this);
            maxlon1Label = new QLabel("y-distance:",this);
            maxlon2Label = new QLabel("         ",this);
           --- */
        msg = util_trimDoubleStg(centlat);
        centlat2Label->setText(msg);
        main->m_tabConstruction->minlat2Label->setText(msg);

        msg = util_trimDoubleStg(centlon);
        centlon2Label->setText(msg);
        main->m_tabConstruction->minlon2Label->setText(msg);

        msg = util_trimDoubleStg(londiff);
        xdist2Label->setText(msg);
        main->m_tabConstruction->maxlat2Label->setText(msg);

        msg = util_trimDoubleStg(latdiff);
        ydist2Label->setText(msg);
        main->m_tabConstruction->maxlon2Label->setText(msg);

        // ***TBD*** SRTM Range
        // max. elevation SRTM - north and east
        QString maxElev = srtm_latlonToHGTBase( dnorth, deast );
        // min. elevation SRTM - south and west
        QString minElev = srtm_latlonToHGTBase( dsouth, dwest );

        style = "color:green";
#ifdef GOT_THREAD_EXIT_OK
        msg = "ok, estimating SRTM count for range...";
        // THIS HAS TO BE DONE ON A THREAD
        // msg.append(srtm_getSRTMListstg(dsouth, dwest, dnorth, deast));
        // getSRTMList(dsouth, dwest, dnorth, deast);
        getSRTMList();
#else
        msg = "ok";
#endif
        srtm1Label->setStyleSheet(style);
        srtm1Label->setText("SRTM Range:");
        srtm2Label->show();
        srtm3Label->show();
        srtm4Label->show();

        srtm2Label->setText(minElev);
        srtm3Label->setText(maxElev);

        srtm4Label->setStyleSheet(style);
        srtm4Label->setText(msg);
#ifdef TRY_GLOBAL_SETTINGS
        main->m_Settings->setValue(minlatPath,sminlat);
        main->m_Settings->setValue(maxlatPath,smaxlat);
        main->m_Settings->setValue(minlonPath,sminlon);
        main->m_Settings->setValue(maxlonPath,smaxlon);
#endif
        setup_state |= suf_ranges;
        // populate other pages
        // RANGES
        msg = util_trimDoubleStg(m_minLat);
        main->m_tabElevations->minlat2Label->setText(msg);
        main->m_tabLanduse->minlat2Label->setText(msg);
        main->m_tabAirports->minlat2Label->setText(msg);

        msg = util_trimDoubleStg(m_maxLat);
        main->m_tabElevations->maxlat2Label->setText(msg);
        main->m_tabLanduse->maxlat2Label->setText(msg);
        main->m_tabAirports->maxlat2Label->setText(msg);

        msg = util_trimDoubleStg(m_minLon);
        main->m_tabElevations->minlon2Label->setText(msg);
        main->m_tabLanduse->minlon2Label->setText(msg);
        main->m_tabAirports->minlon2Label->setText(msg);

        msg = util_trimDoubleStg(m_maxLon);
        main->m_tabElevations->maxlon2Label->setText(msg);
        main->m_tabLanduse->maxlon2Label->setText(msg);
        main->m_tabAirports->maxlon2Label->setText(msg);

        // SRTM RANGE
        main->m_tabElevations->minsrtmLabel->setText(minElev);
        main->m_tabElevations->maxsrtmLabel->setText(maxElev);
    } else {
        style = "color:red";
        msg = "Range error: ";
        if (sminlat.isEmpty()||smaxlat.isEmpty()||sminlon.isEmpty()||smaxlon.isEmpty()) {
            if (sminlat.isEmpty())
                msg.append("min.lon is empty ");
            if (smaxlat.isEmpty())
                msg.append("max.lat is empty ");
            if (sminlon.isEmpty())
                msg.append("min.lon is empty ");
            if (smaxlon.isEmpty())
                msg.append("max.lon is empty ");
        } else if (m_minLat > m_maxLat)
            msg = "min.lat GT max.lat";
        else if (m_minLon > m_maxLon)
            msg = "min.lon GT max.lon";
        else
            msg = "min. == max.";
        srtm2Label->hide();
        srtm3Label->hide();
        srtm4Label->hide();
        srtm1Label->setStyleSheet(style);
        srtm1Label->setText(msg);

    }
}

// eof - tabsetup.cpp
