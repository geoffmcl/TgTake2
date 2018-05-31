/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#include "app_config.h"
#include "mainwindow.h"
#include "tabelevations.h"
#include "utilities/app_macros.h"
#include "utilities/tg_utils.h"
#include <QProcessEnvironment>

tabElevations::tabElevations(QWidget *parent) :
    QWidget(parent)
{
    QString tmp;
    int row, col;
    int spac  = 1;
    int left  = 5;
    int top   = 5;
    int right = 5;
    int bott  = 5;
    main = (MainWindow *)parent;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(left, top, right, bott);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    // ===================================
    // LIMITS
    // ===================================
    QGroupBox *currGroup = new QGroupBox(tr("Current Limits - change on Setup page"),this);
    mainLayout->addWidget(currGroup);

    QGridLayout *limitLayout = new QGridLayout;
    limitLayout->setContentsMargins(left, top, right, bott);
    limitLayout->setSpacing(spac);
    currGroup->setLayout(limitLayout);

    // Min. Lat. [   ] Min.Lon. [   ] Max. Lat. [   ] Max. Lon. [   ]
    minlat1Label = new QLabel("Min. Lat.",this);
    minlat2Label = new QLabel("         ",this);
    minlon1Label = new QLabel("Min. Lon.",this);
    minlon2Label = new QLabel("         ",this);
    maxlat1Label = new QLabel("Max. Lat.",this);
    maxlat2Label = new QLabel("         ",this);
    maxlon1Label = new QLabel("Max. Lon.",this);
    maxlon2Label = new QLabel("         ",this);

    // add box and sizing
    QSizePolicy szpol;
    szpol.setVerticalStretch(0);
    minlat2Label->setFrameShape(QFrame::Box);
    minlat2Label->setSizePolicy(szpol);
    minlon2Label->setFrameShape(QFrame::Box);
    minlon2Label->setSizePolicy(szpol);
    maxlat2Label->setFrameShape(QFrame::Box);
    maxlat2Label->setSizePolicy(szpol);
    maxlon2Label->setFrameShape(QFrame::Box);
    maxlon2Label->setSizePolicy(szpol);

    row = 0;
    col = 0;
    limitLayout->addWidget(minlat1Label,row,col);
    col++;
    limitLayout->addWidget(minlat2Label,row,col);
    col++;
    limitLayout->addWidget(minlon1Label,row,col);
    col++;
    limitLayout->addWidget(minlon2Label,row,col);
    col++;
    limitLayout->addWidget(maxlat1Label,row,col);
    col++;
    limitLayout->addWidget(maxlat2Label,row,col);
    col++;
    limitLayout->addWidget(maxlon1Label,row,col);
    col++;
    limitLayout->addWidget(maxlon2Label,row,col);

    srtmLabel    = new QLabel("SRTM Range:",this);
    minsrtmLabel = new QLabel("           ",this);
    maxsrtmLabel = new QLabel("           ",this);

    // add box and sizing
    QSizePolicy szpol2;
    szpol2.setVerticalStretch(0);
    minsrtmLabel->setFrameShape(QFrame::Box);
    minsrtmLabel->setSizePolicy(szpol2);
    maxsrtmLabel->setFrameShape(QFrame::Box);
    maxsrtmLabel->setSizePolicy(szpol2);

    row++;
    col = 0;
    limitLayout->addWidget(srtmLabel,row,col,1,2);
    col += 2;
    limitLayout->addWidget(minsrtmLabel,row,col,1,2);
    col += 2;
    limitLayout->addWidget(maxsrtmLabel,row,col,1,2);


    QString statmsg("HGT directory - output/input");
    QGroupBox *dirGroup = new QGroupBox(tr("HGT File Directory"),this);
    mainLayout->addWidget(dirGroup);

    QHBoxLayout *layoutHgt = new QHBoxLayout();
    layoutHgt->setContentsMargins(10, 0, 10, 0);
    layoutHgt->setSpacing(0);
    dirGroup->setLayout(layoutHgt);

    // HGT output/input directory
    //hgtLabel = new QLabel(tr("HGT Directory:"));
    hgtLabel = new QLabel(tr("HGT Dir.:"));
    hgtLabel->setStatusTip(statmsg);
    tmp = main->m_Settings->value(dirshgtPath,"").toString();
    hgtEdit = new QLineEdit(tmp,this);
    hgtEdit->setStatusTip(statmsg);
    connect(hgtEdit,SIGNAL(editingFinished()),this,SLOT(on_hgt_edit()));
    hgtBrowse = new QPushButton("...",this);
    hgtBrowse->setStatusTip(statmsg);
    connect(hgtBrowse,SIGNAL(pressed()),this,SLOT(on_hgt_browse()));

    layoutHgt->addWidget(hgtLabel);
    layoutHgt->addWidget(hgtEdit);
    layoutHgt->addWidget(hgtBrowse);

    // ===================================
    // OPTIONS
    // ===================================
    QGroupBox *optionGroup = new QGroupBox(tr("Options"),this);
    mainLayout->addWidget(optionGroup);

    QGridLayout *optionLayout = new QGridLayout;
    optionLayout->setContentsMargins(left, top, right, bott);
    optionLayout->setSpacing(spac);
    optionGroup->setLayout(optionLayout);

    resLabel = new QLabel("Resolution:",this);
    resCombo = new QComboBox(this);
    arcLabel = new QLabel("arc sec. Usually 3, 1 for USA or Special",this);
    resCombo->addItem("1",QVariant("1"));
    resCombo->addItem("3",QVariant("3"));

    maxnodes = new QLabel("Max. nodes:",this);
    maxEdit = new QLineEdit(this);
    tmp = main->m_Settings->value(maxnodesPath,QVariant("1000")).toString();
    maxEdit->setText(tmp);
    minnodes = new QLabel("Min. nodes:",this);
    minEdit = new QLineEdit(this);
    tmp = main->m_Settings->value(minnodesPath,QVariant("50")).toString();
    minEdit->setText(tmp);
    maxerrs = new QLabel("Max. errors:",this);
    errsEdit = new QLineEdit(this);
    tmp = main->m_Settings->value(maxerrorPath,QVariant("40")).toString();
    errsEdit->setText(tmp);

    resetButton = new QPushButton("Reset Defaults",this);

    skipzipCheck = new QCheckBox("Skip existing zips",this);
    skipzipCheck->setChecked(main->m_Settings->value(skipzipPath, "1").toBool());
    skiphgtCheck = new QCheckBox("Skip existing hgt",this);
    skiphgtCheck->setChecked(main->m_Settings->value(skiphgtPath, "1").toBool());
    skipcfmCheck = new QCheckBox("Skip confirmations",this);
    skipcfmCheck->setChecked(main->m_Settings->value(skipcfmPath, "1").toBool());
    conterrCheck = new QCheckBox("Continue on errors",this);
    conterrCheck->setChecked(main->m_Settings->value(conterrPath, "0").toBool());
    // **TBD** connect the edits and check boxes

    row = 0;
    col = 0;
    int wid = 30;
    int hit = 14;
    int fac = 3;
    QSpacerItem * sp;
    //sp = new QSpacerItem(wid*fac,hit);
    optionLayout->addWidget(resLabel,row,col,1,1);
    col++;
    optionLayout->addWidget(resCombo,row,col,1,1);
    sp = new QSpacerItem(wid,hit);
    col++;
    optionLayout->addItem(sp,row,col,1,2);
    col++;
    optionLayout->addWidget(arcLabel,row,col,1,2);

    row++;
    col = 0;
    optionLayout->addWidget(maxnodes,row,col);
    col++;
    optionLayout->addWidget(maxEdit,row,col);
    sp = new QSpacerItem(wid*fac,hit);
    col++;
    optionLayout->addItem(sp,row,col,1,2);
    col++;
    optionLayout->addWidget(skipzipCheck,row,col,1,2);

    row++;
    optionLayout->addWidget(minnodes,row,0);
    optionLayout->addWidget(minEdit,row,1);
    sp = new QSpacerItem(wid*fac,hit);
    optionLayout->addItem(sp,row,2,1,2);
    optionLayout->addWidget(skiphgtCheck,row,3,1,2);

    row++;
    optionLayout->addWidget(maxerrs,row,0);
    optionLayout->addWidget(errsEdit,row,1);

    sp = new QSpacerItem(wid*fac,hit);
    optionLayout->addItem(sp,row,2,1,2);
    optionLayout->addWidget(skipcfmCheck,row,3,1,2);
    row++;

    optionLayout->addWidget(resetButton,row,0);
    //... blank col 2
    sp = new QSpacerItem(wid*fac,hit);
    optionLayout->addItem(sp,row,2,1,2);
    optionLayout->addWidget(conterrCheck,row,3,1,2);

    // ===================================
    // ACTIONS
    // ===================================
    QGroupBox *actionGroup = new QGroupBox(tr("Actions"),this);
    mainLayout->addWidget(actionGroup);

    QHBoxLayout *actionLayout = new QHBoxLayout(this);
    actionLayout->setContentsMargins(left, top, right, bott);
    actionLayout->setSpacing(spac);
    actionGroup->setLayout(actionLayout);

    actDownload = new QPushButton("Download HGT",this);
    actUnzipHGT = new QPushButton("Unzip HGT",this);
    actDecode   = new QPushButton("Decode HGT",this);
    actTerrafit = new QPushButton("Run terrafit",this);

    actionLayout->addWidget(actDownload);
    actionLayout->addWidget(actUnzipHGT);
    actionLayout->addWidget(actDecode);
    actionLayout->addWidget(actTerrafit);

    // **TBD** add slots for actions


    // ===================================
    // INFORMATION
    // ===================================
    QGroupBox *infoGroup = new QGroupBox(tr("Information"),this);
    mainLayout->addWidget(infoGroup);

    QVBoxLayout *infoLayout = new QVBoxLayout(this);
    infoLayout->setContentsMargins(left, top, right, bott);
    infoLayout->setSpacing(spac);
    infoGroup->setLayout(infoLayout);

    infoEdit = new QPlainTextEdit(this);
    infoEdit->setReadOnly(true);

    infoLayout->addWidget(infoEdit);

    m_urlThread = new workThread(this);
    connect(m_urlThread, SIGNAL(work_done(int,int)), this, SLOT(on_url_done(int,int)));
    m_urlWork = 0;

}

void tabElevations::openUrl(void * vp)
{
    PWORK pw = (PWORK)vp;
    QUrl qu(pw->work_desc);
    QDesktopServices::openUrl(qu);
}

void tabElevations::openBrowser(QString url)
{
    if (!m_urlWork)
        m_urlWork = new WORK;
    m_urlWork->work_type = WT_URL;
    m_urlWork->work_desc = url;
    m_urlWork->work_tt.start();
    m_urlWork->work_done = false;
    m_urlWork->abort_work = false;
    m_urlWork->work_num = m_urlThread->work(openUrl,m_urlWork);
}

void tabElevations::on_url_done(int id,int ms)
{
    Q_UNUSED(id);
    if (m_urlWork) {
        m_urlWork->work_done = true;
        outLog("Openned "+m_urlWork->work_desc+" in "+getElapTimeStg(ms));
    }
}

void tabElevations::on_closeEvent()
{
    if (m_urlThread)
        delete m_urlThread;
    m_urlThread = 0;
    if (m_urlWork)
        delete m_urlWork;
    m_urlWork = 0;
}

void tabElevations::finalize()
{
    MCONSET_E(maxEdit,on_max_edit,maxnodesPath,"1000");
    MCONSET_E(minEdit,on_min_edit,minnodesPath,"50");
    MCONSET_E(errsEdit,on_err_edit,maxerrorPath,"40");

    resCombo->setCurrentIndex(main->m_Settings->value(arcsecPath,"1").toInt());
    connect(resCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(on_res_combo(int)) );

    MCONSET_T(skipzipCheck,on_skipzip_toggle,skipzipPath,"1");  // = new QCheckBox("Skip existing zips",this);
    MCONSET_T(skiphgtCheck,on_skiphgt_toggle,skiphgtPath, "1"); // = new QCheckBox("Skip existing hgt",this);
    MCONSET_T(skipcfmCheck,on_skipcfm_toggle,skipcfmPath, "1"); // = new QCheckBox("Skip confirmations",this);
    MCONSET_T(conterrCheck,on_conterr_toggle,conterrPath, "0"); // = new QCheckBox("Continue on errors",this);

    connect(actDownload,SIGNAL(clicked()),this,SLOT(on_download_clicked())); // = new QPushButton("Download HGT",this);
    connect(actUnzipHGT,SIGNAL(clicked()),this,SLOT(on_unzip_clicked()));   // = new QPushButton("Unzip HGT",this);
    connect(actDecode,SIGNAL(clicked()),this,SLOT(on_decode_clicked()));  //   = new QPushButton("Decode HGT",this);
    connect(actTerrafit,SIGNAL(clicked()),this,SLOT(on_terrafit_clicked())); // = new QPushButton("Run terrafit",this);
    connect(resetButton,SIGNAL(clicked()),this,SLOT(on_reset_clicked()));   // "Reset Defaults"

    on_hgt_edit();
    // ***TBD*** update information block
}

void tabElevations::on_reset_clicked()
{  // connect(resetButton,SIGNAL(clicked()),this,SLOT(on_reset_clicked()));
   // "Reset Defaults"
    resCombo->setCurrentIndex(1);
    maxEdit->setText("1000");
    minEdit->setText("50");
    errsEdit->setText("40");
    // write to settings
    on_res_combo(1);
    on_max_edit();
    on_min_edit();
    on_err_edit();

}

void tabElevations::on_hgt_edit()
{
    QString tmp;
    QDir d;
    QString style;
    tmp = hgtEdit->text();
    //setup_state &= suf_scene;
    if (d.exists(tmp)) {
        style = "color:green";
        main->saveSetting(dirshgtPath,tmp);
        //setup_state |= suf_scene;
    } else {
        style = "color:red";
    }
    hgtLabel->setStyleSheet(style);
}

void tabElevations::on_hgt_browse()
{
    //main->m_statusBar->showMessage("Set Project Directory",5000);
    main->setStatusMessage("Set HGT in/out Directory",5000);
    QString prompt(tr("Select HGT Directory"));
    QString current = hgtEdit->text();
    QString filePath = util_browseDirectory(prompt,current);
    QDir d;
    if (!filePath.isEmpty() && d.exists(filePath)) {
        hgtEdit->setText(filePath);
        on_hgt_edit();
    }

}

#define got_limits      (main->m_tabSetup->setup_state & suf_ranges)
#define got_projdir     (main->m_tabSetup->setup_state & suf_scene)
#define got_tooldir     (main->m_tabSetup->setup_state & suf_tools)

void tabElevations::update_information()
{
    QString info, tmp;
    QDir d;
    bool enabled = false;
    int hgturl_cnt = -1;
    int hgt_cnt = -1;
    infoEdit->setPlainText("Collecting information... moment...");
    if (main->m_tabSetup->m_pwsu) {
        hgturl_cnt = main->m_tabSetup->m_pwsu->hgturlList.count();
        hgt_cnt = main->m_tabSetup->m_pwsu->i_Hgt_Cnt;
    }
    // ACTIONS: Download-Unzip-Decode-Terrafit
    //outLog("Changed to Elevations\n");
    // To down load
    if (got_limits && got_projdir && got_tooldir)
        enabled = true;
    else
        enabled = false;
    actDownload->setEnabled(enabled); // download HGT
    if (!enabled) {
        actUnzipHGT->setEnabled(enabled); // unzip HGT
        actDecode->setEnabled(enabled); // Decode HGT
        actTerrafit->setEnabled(enabled); // Run Terrafit
    } else {
        // need to check unzip tool, and files to UNZIP
        actUnzipHGT->setEnabled(enabled); // unzip HGT
        // need to check decode tool, and unzipped files
        actDecode->setEnabled(enabled); // Decode HGT
        // need to check Terrafit tool, and decoded hgt files done
        actTerrafit->setEnabled(enabled); // Run Terrafit
    }

    //info.sprintf("Of %d HGT, %d avail. for download.",m_Hgt_Cnt,hgturlList.size());
    info.sprintf("Of %d HGT, %d avail. for download.",hgt_cnt,hgturl_cnt);
    if (enabled) {
        // basically enabled to go do elevations
        QStringList list1 = findFiles(m_srtmDir, QStringList() << "*.zip", false);
        QStringList list2 = findFiles(m_srtmDir, QStringList() << "*.hgt", false);
        int zip_cnt = list1.size();
        int hgt_cnt = list2.size();
        int arr_cnt = 0;
        int fit_cnt = 0;
        if (zip_cnt) {
            tmp.sprintf("\nFound %d zips.", zip_cnt);
            info += tmp;
        } else {
            info += "\nNo zips found. Maybe need to do 'Download HGT'";
        }
        if (hgt_cnt) {
            tmp.sprintf("\nFound %d hgt.", hgt_cnt);
            info += tmp;
        }
        if (zip_cnt  && (zip_cnt != hgt_cnt)) {
                if (hgturl_cnt < hgt_cnt) {
                    info += "\nLooks like 'download HGT' needs to be completed";
                    info += "\nand 'Unzip HGT' can be run.";
                } else {
                    info += "\nLooks like 'Unzip HGT' needs to be run.";
                }
        } else if (zip_cnt && (zip_cnt == hgt_cnt)) {
            info += "\nLooks like decode HGT needs to be done.";
        }

        if ( m_elevDir.size() && d.exists(m_elevDir) ) {
            //list1 = findFiles(m_elevDir,QStringList() << "*",true);
            list1 = findFiles(m_elevDir,QStringList() << "*.arr.gz",true);
            list2 = findFiles(m_elevDir,QStringList() << "*.fit.gz",true);
            arr_cnt = list1.size();
            fit_cnt = list2.size();
            tmp.sprintf("\nFound work directory with %d array files, and %d fit files.",
                 arr_cnt, fit_cnt);
            info += tmp;
            if (arr_cnt) {
                if (arr_cnt == fit_cnt) {
                    info += "\nLook like elevations have been completed, ready for airport building, and final construction.";
                } else if (fit_cnt == 0) {
                    info += "\nLook like 'Terrafit' needs to be run.";
                }
            } else if (hgt_cnt) {
                info += "\nLook like 'Decode HGT' needs to be run.";
            }
        } else {
            // do not yet have an work/SRTM-30 elevation directory
            if (hgt_cnt)
                info += "\nLook like 'Decode HGT' needs to be run.";
        }
    }
    infoEdit->setPlainText(info);

}

void tabElevations::on_res_combo(int id)
{
    if ((id >= 0)&&(id < resCombo->count())) {
        QString tmp;
        tmp.sprintf("%d",id);
        main->saveSetting(arcsecPath,tmp);
    }

}

// generate standard handlers of edit and toggle
#define M_CONSET_E  M_CONSET_E_EL
#define M_CONSET_T  M_CONSET_T_EL

M_CONSET_E(maxEdit,on_max_edit,maxnodesPath,"1000")
M_CONSET_E(minEdit,on_min_edit,minnodesPath,"50")
M_CONSET_E(errsEdit,on_err_edit,maxerrorPath,"40")
M_CONSET_T(skipzipCheck,on_skipzip_toggle,skipzipPath,"1")  // = new QCheckBox("Skip existing zips",this);
M_CONSET_T(skiphgtCheck,on_skiphgt_toggle,skiphgtPath, "1") // = new QCheckBox("Skip existing hgt",this);
M_CONSET_T(skipcfmCheck,on_skipcfm_toggle,skipcfmPath, "1") // = new QCheckBox("Skip confirmations",this);
M_CONSET_T(conterrCheck,on_conterr_toggle,conterrPath, "0") // = new QCheckBox("Continue on errors",this);


bool tabElevations::checkInitThreadDone()
{
    //if (!main->m_tabSetup->isInitThreadDone()) {
    //    QString msg;
    //    msg = "WARNING: The initial setup, and counting thread is still working...";
    //    msg.append("\nThis can be seem by the '*' appended to the time string");
   //     msg.append("\n\nUse the download button again after this thread completes!");
   //     MBI("WAIT - INTITIAL THREAD RUNNING",msg);
   //     if (!main->m_tabSetup->isInitThreadDone())
   //         return false;
    //}
    return true;
}


/***********************************
  Elevations Page
 ***********************************/
// download the STRM HGT elevation files
//void MainWindow::on_pushButton_srtm_clicked() {

void tabElevations::on_download_clicked()
{
    QString msg;
    msg = "Download HGT:on_download_clicked:...\n";
    outLog(msg,3);
    if (!checkInitThreadDone()) {
        return;
    }
    QString projectDir(main->m_tabSetup->m_projdirEdit->text());
    QString toolWget(main->m_tabAdvanced->wgetEdit->text());
    int m_MaxDiag = main->m_tabAdvanced->maxEdit->text().toInt();
    bool m_Verify_Tools = main->m_tabAdvanced->verifyCheck->isChecked(); // ui->checkBox_verify->isChecked();
    bool m_User_Break = false;
    bool b_confirm = main->m_tabAdvanced->confirmCheck->isChecked();
    // Unable to proceed, if no valid project directory
    if (!tg_verifyProjDir(projectDir))
        return; // down hgt - projectDir invlaid
    if (!tg_createDirectory(m_srtmDir))
        return; // down hgt - m_srtmDir for output not valid
    QStringList allZips = findFiles(m_srtmDir, QStringList() << "*.zip", false);
    QStringList allHGT = findFiles(m_srtmDir, QStringList() << "*.hgt", false);
    skiphgtCheck = new QCheckBox("Skip existing hgt", this);
    bool skip_exist_zip = skipzipCheck->isChecked(); // "Skip existing zips"
    bool skip_cfm = skipcfmCheck->isChecked();       //  "Skip confirmations"
    bool ign_errors = conterrCheck->isChecked();     //  "Continue on errors"

    // ================================================
    QString tmp;
    //QString url = "http://dds.cr.usgs.gov";
    QString url = "http://dds.cr.usgs.gov";    //  srtm / version2_1 / SRTM3 / ";
    // Africa/ Australia / Eurasia / Islands / North_America / South_America /
    bool opnd = false;
    outLog("Base URL: " + url);
    // openBrowser(url + "/srtm/"); // open the link on a QThread - no time lost on main thread...
    QString info;
    QString runtime = toolWget; // get 'wget' tool
    QString argument;    // build each argument
    // check the wget tool functions
    if (runtime.contains(QChar(' ')))
        runtime = "\"" + runtime + "\"";
    argument = runtime;
    argument += " --help";
    int res = tg_testApplication(argument, info);
    if (res || m_Verify_Tools) {
        tg_trimLongMessage(m_MaxDiag, info);
        tmp.sprintf("\nTool returned value %d", res);
        info += tmp;
        info += "\nThe name (and location if not in your PATH)";
        info += "\nis set on the 'Advanced' pages.";
        outLog(info);
        info += "\n\nClick 'Yes' to continue to use this tool.";
        res = util_getYesNo("DOWNLOAD TOOL", info, this);
        if (!res)
            return; // elevation download tool NOT acceptable
    }
    // int srtmcnt2 = qmSRTM2URL2.count();
    int srtmcnt2 = 0;
    if (main && main->m_tabSetup->m_pwsu) {
        srtmcnt2 = main->m_tabSetup->m_pwsu->wSrtmList.count();
    }
    if (!srtmcnt2) {
        outLog("No SRTM count to download!");
        return;
    }
    info = "\ndownload the SRTM";
    info += "\nA list of the ranges should be in the templog.txt file";
    info += "\nWhen the zips are downloaded they should be expanded into a\ndirectory of your choice before running 'decode hgt'.";
    int count2 = util_checkSRTMFiles2(main->m_tabSetup->m_pwsu->wSrtmList);
    if (!count2) {
        outLog("Could not find any SRTM (HGT) count to download!");
        return;
    }

    // BUILD THE ARGUMENT LIST
    QStringList argList; // list of arguments to run
    QStringList filList; // and 'information' file list
    QStringList m_zipList;
    QStringList allList; // all arguments
    QString file;        // file name only
    QString outfile;     // fully qualified file name
    int skipped = 0;
    int argcount = 0;
    m_zipList.clear();
    int srtmcount = 0;
    QMap<QString, QString>::iterator i2 = main->m_tabSetup->m_pwsu->wSrtmList.begin();
    for (; i2 != main->m_tabSetup->m_pwsu->wSrtmList.end(); i2++) {
        srtmcount++;
        // QString val = i2.value(); // get the URL
        QString val = i2.key(); // get the file name
        // if we have a URL for this SRTM item...
        if (val.size()) {
            QString url1;
            QString key = i2.key(); // get the SRTM we want
            file = key + ".hgt.zip";  // build the 'file'
            // outLog(key+": "+val);
            // need a url for downloading - 
            // of the form http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/<region>/S18W150/S18W150.hgt.zip ...
            if (!srtm_getUrlForHgtBase(key, url1)) {
                skipped++;
                continue;
            }
            argument = runtime + " "; // the 'tested' agreed wget tool
            //argument += url + url1 + "/" + val + "/" + file; // add what we want...
            argument += url + url1 + "/" + file; // add what we want...
            outfile = m_srtmDir + "/" + file;
            allList += argument;
            // if (skip_exist_zip && util_isinFileList(allZips,file))
            if (skip_exist_zip && (allZips.indexOf(outfile) != -1)) {
                skipped++;
                continue;
            }
            argList += argument; // wget request argument
            filList += outfile;
            argcount++;
        }
    }

    if (!argcount) {
        msg.sprintf("Download click, with srtm %d, but no 'wget' args to run... skipped %d... exit", srtmcount, skipped);
        outLog(msg);
        return;
    }

    outLog("Full wget argument list, which may include those already downloaded");
    outLog(allList.join("\n"));
    outLog(filList.join("\n"));

    // msg = "Download HGT:on_download_clicked: TODO: ***TBD*** handle download of HGT zip file...\n";
    // outLog(msg);
    int i;
    bool do_wait;
    int ext;
    QFile f;
    int wait_secs = 10;
    for (i = 0; i < argList.size(); i++) {
        if (m_User_Break)
            break;
        do_wait = (skip_cfm ? false : true);
        msg.sprintf("%d of %d: ", (i + 1), argList.size());
        argument = argList[i];

        file = filList[i];
        outfile = file;
        outLog(msg + "Downloading [" + file + "] with arg [" + argument + "]");

        // YUK, 'wget' needs params to overwrite existing,
        // but that gets complicated, so...
        ext = tg_renameToOLDBAK(outfile); // rename any exsiting
        //outArg(argument);
        info = tg_runProcess(argument, m_srtmDir, &res);
        tg_trimLongMessage(m_MaxDiag, info);
        info += "\nFile " + file + " ";
        if (f.exists(outfile)) {
            // was this an already existing
            if (allZips.indexOf(outfile) == -1)
                info += "DOWNLOADED\n"; // no, is NEW download
            else
                info += "RE-DOWNLOAD\n";
            m_zipList += outfile;
        }
        else {
            if (!ign_errors)
                do_wait = true;
            info += "failed (does not exist)\n";
        }
        info += "\n";
        outLog(info);
        if (do_wait) {
            if ((i + 1) < argList.size()) {
                info += msg + "Continue with next download?";
                res = util_getYesNo("DONE DOWNLOAD", info, this);
                if (!res)
                    return; // down hgt - done one, but no cfm to proceeed
            }
        }
        else {
            tg_openNonModalDialog("DOWNLOAD INFORMATION", info, wait_secs, this);
        }
    }


#if 0 // ===========================
    QStringList m_zipList;
    // ***TBD*** actDownload, on_download_clicked() "Download HGT";
    int i;
    int srtmcnt2 = qmSRTM2URL2.count();
    // minElev and maxElev set
    outLog(elevList); /* provide a 'helpful' list of SRTM files */
    info = "\ndownload the SRTM in the range [" + minElev + "] to [" + maxElev + "]";
    info += "\nA list of the ranges should be in the templog.txt file";
    info += "\nWhen the zips are downloaded they should be expanded into a\ndirectory of your choice before running 'decode hgt'.";
    int count2 = util_checkSRTMFiles2(qmSRTM2URL2);

    // ===============================================
#if (defined(TGS_DEBUG_VERS) && defined(USE_FILE_INTERFACE)) // but outputs extra info to the log file
    QString m_appDir = "C:/Qt/2010.05/qt/projects/TgScenery";
    int count = util_checkSRTMFiles(qmSRTM2URL,m_appDir);
    // DEBUG - a file read versus the new 'internal' data functions
    // these two functions should ABSOLUTELY yield the SAME result ;=))
    // and DEBUG is only if they are NOT '=='
    if (count2 != count) {
        {
            msg.sprintf("Cnt %d of %d", count2, qmSRTM2URL2.size());
            outLog("List of elevations found in 2 "+msg);
            QMap<QString, QString>::iterator ii = qmSRTM2URL2.begin();
            for ( ; ii != qmSRTM2URL2.end(); ii++) {
                QString val = ii.value();
                if (val.size()) {
                    QString key = ii.key();
                    outLog(key+": "+val);
                }
            }
        }
        {
            msg.sprintf("Cnt %d of %d", count, qmSRTM2URL.size());
            outLog("List of elevations found in 1 "+msg);
            QMap<QString, QString>::iterator ii = qmSRTM2URL.begin();
            for ( ; ii != qmSRTM2URL.end(); ii++) {
                QString val = ii.value();
                if (val.size()) {
                    QString key = ii.key();
                    outLog(key+": "+val);
                }
            }
        }
    }
#endif // #ifdef TGS_DEBUG_VERS // but outputs extra info to the log file
    // ok, we proceed with the DOWNLOAD of the desired HGT files
    // to the temp/hgt directory...
    //QString tm;
    QStringList argList; // list of arguments to run
    QStringList filList; // and 'information' file list
    QStringList allList; // all arguments
    QString file;        // file name only
    QString outfile;     // fully qualified file name
    QString argument;    // build each argument
    QString runtime = toolWget; // get 'wget' tool
    // QStringList zipList;
    QString tmp;
    QFile f;
    int ext, res;
    int skipped = 0;
    int wait_secs = main->m_tabAdvanced->delayEdit->text().toInt();
    if (wait_secs < 1) wait_secs = 1;
    bool do_wait;

    // check the wget tool functions
    if (runtime.contains(QChar(' ')))
        runtime = "\""+runtime+"\"";
    argument = runtime;
    argument += " --help";
    res = tg_testApplication(argument, info);
    if (res || m_Verify_Tools) {
        tg_trimLongMessage(m_MaxDiag, info);
        tmp.sprintf("\nTool returned value %d",res);
        info += tmp;
        info += "\nThe name (and location if not in your PATH)";
        info += "\nis set on the 'Advanced' pages.";
        outLog(info);
        info += "\n\nClick 'Yes' to continue to use this tool.";
        res = util_getYesNo("DOWNLOAD TOOL",info,this);
        if (!res)
            return; // elevation download tool NOT acceptable
    }

    // BUILD THE ARGUMENT LIST
    m_zipList.clear();
    QMap<QString, QString>::iterator i2 = qmSRTM2URL2.begin();
    for ( ; i2 != qmSRTM2URL2.end(); i2++) {
        QString val = i2.value(); // get the URL
        // if we have a URL for this SRTM item...
        if (val.size()) {
            QString key = i2.key(); // get the SRTM we want
            file = key+".hgt.zip";  // build the 'file'
            // outLog(key+": "+val);
            argument = runtime+" "; // the 'tested' agreed wget tool
            argument += url+val+"/"+file; // add what we want...
            outfile = m_srtmDir+"/"+file;
            allList += argument;
            // if (skip_exist_zip && util_isinFileList(allZips,file))
            if (skip_exist_zip && (allZips.indexOf(outfile) != -1)) {
                skipped++;
                continue;
            }
            argList += argument; // wget request argument
            filList += file;
        }
    }

    outLog("Full wget argument list, which may include those already downloaded");
    outLog(allList.join("\n"));

    if (count2 == 0) {
        msg.sprintf("Of the desired %d HGT elevation files, found NONE!", srtmcnt2);
    } else if (count2 < srtmcnt2) {
        msg.sprintf("Of the possible %d HGT elevation files, found URL for %d ", srtmcnt2, count2);
        msg += "\nIt must be remembered there will be NO elevation files for ocean areas, so this could be the correct count of what is needed.";
    } else {
        msg.sprintf("Found the URL for ALL the desired %d HGT elevation files.", srtmcnt2);
    }
    if (opnd) {
        msg += "\nIn any case the URL should be open in your browser, so you could MANUALLY do downloads.\n";
        //msg += info;
    }
    msg += "\nBut alternatively all 'wget' arguments have been output\nto the LOG file so you could run these in the \n["+m_srtmDir+"] directory";

    if (skip_exist_zip && skipped) {
        argument.sprintf("\n\nDue to skip existing checked, skipping %d, leaving %d for download", skipped, argList.size());
        msg += argument;
    }
    if (skip_cfm) {
        msg += "\nOption 'Skip confirmation' checked, so this will be the only confirmation.";
        if (ign_errors)
            msg += "\nand will also not confirm on a download error.";
        else
            msg += "\nbut will confirm if a download error detected.";
    } else {
        msg += "\nOption 'Skip confirmation' is not checked, so will confirm after each download.";
    }
    if (argList.count() == 0) {
        msg += "\n\nAdvice only, since there is no work to do. Click either...";
    } else {
        msg += "\n\nDo you want to CONTINUE with this download?";
    }

    res = 0;
    if (b_confirm) {
        outLog("CONFIRMATION TO PROCEED");
        outLog(msg);
        res = util_getYesNo("CONFIRMATION TO PROCEED", msg, this);
    } else if ( argList.count() == 0 ) {
        int secs = (wait_secs > 5) ? wait_secs : 5;
        tg_openNonModalDialog("NO WORK TO DO",msg,secs,this);
    }
    if (!res || (argList.count()==0)) {
       return; // down hgt - no confirmation to proceed, or NO arguments
    }
    // got the set of download arguments to run...
    // and the name of the desired file
    m_User_Break = false;
    for (i = 0; i < argList.size(); i++) {
        if (m_User_Break)
            break;
        do_wait = (skip_cfm ? false : true);
        msg.sprintf("%d of %d: ", (i+1), argList.size());
        argument = argList[i];
        file = filList[i];
        outfile = m_srtmDir+"/"+file;
        outLog(msg+"Downloading ["+file+"] with arg ["+argument+"]");
        // YUK, 'wget' needs params to overwrite existing,
        // but that gets complicated, so...
        ext = tg_renameToOLDBAK(outfile); // rename any exsiting
        //outArg(argument);
        info = tg_runProcess(argument,m_srtmDir,&res);
        tg_trimLongMessage(m_MaxDiag, info);
        info += "\nFile "+file+" ";
        if (f.exists(outfile)) {
            // was this an already existing
            if (allZips.indexOf(outfile) == -1)
                info += "DOWNLOADED\n"; // no, is NEW download
            else
                info += "RE-DOWNLOAD\n";
            m_zipList += outfile;
        } else {
            if (!ign_errors)
                do_wait = true;
            info += "failed (does not exist)\n";
        }
        info += "\n";
        outLog(info);
        if (do_wait) {
            if ((i + 1) < argList.size()) {
                info += msg+"Continue with next download?";
                res = util_getYesNo("DONE DOWNLOAD",info,this);
                if (!res)
                    return; // down hgt - done one, but no cfm to proceeed
            }
        } else {
            tg_openNonModalDialog("DOWNLOAD INFORMATION",info,wait_secs,this);
        }
    }

    // done the LAST, or aborted by user
    msg.sprintf("%d of %d: ", i, argList.size());
    if (m_User_Break)
        info += "\nUser abort"+msg;
    else
        info += "\nDone "+msg;
    if (i == argList.size()) {
        info += "\nThe next step would be to do the UNZIPPING?";
    } else {
        info += "\nIt seems the download is unfinished!";
        info += "\nThat needs to be completed before the UNZIPPING?";
    }
    m_User_Break = false;
    wait_secs = (wait_secs > 5) ? wait_secs : 5;
    tg_openNonModalDialog("DONE HGT ZIP DOWNLOAD",info,wait_secs,this);
#endif // 0
}

void tabElevations::on_unzip_clicked() {
    // ***TBD*** // connect(actUnzipHGT,SIGNAL(clicked()),this,SLOT(on_unzip_clicked()));   // = new QPushButton("Unzip HGT",this);
    QStringList m_zipList;
    int srtmcount = 0;
    QString file,outfile;
    QMap<QString, QString>::iterator i2 = main->m_tabSetup->m_pwsu->wSrtmList.begin();
    for (; i2 != main->m_tabSetup->m_pwsu->wSrtmList.end(); i2++) {
        // QString val = i2.value(); // get the URL
        QString val = i2.key(); // get the file name
                                // if we have a URL for this SRTM item...
        if (val.size()) {
            QString url1;
            QString key = i2.key(); // get the SRTM we want
            file = key + ".hgt.zip";  // build the 'file'
            outfile = m_srtmDir + "/" + file;
            srtmcount++;
            m_zipList += outfile;
        }
    }
    size_t zips = m_zipList.size();
}

/* 
Usage hgtchop version 2.1.1
hgtchop <resolution> <hgt_file> <work_dir>
resolution must be either 1 or 3 for 1arcsec or 3arcsec
*/
void tabElevations::on_decode_clicked() {
    // ***TBD*** // connect(actDecode,SIGNAL(clicked()),this,SLOT(on_decode_clicked()));  //   = new QPushButton("Decode HGT",this);
    // use hgtchop / gdalchop
    // QString chopTool = "hgtchop";
    QString chopTool = "gdalchop";
    QFile f;
    MAPSTR2STR::iterator ib, ie;
    // 1: Test if ANY hgt files are available for decode
    PWORKsu pwsu = main->m_tabSetup->m_pwsu;
    QString projectDir = main->m_tabSetup->m_projdirEdit->text().trimmed();
    size_t ii, max2 = pwsu->wSrtmList.size();
    QStringList hgtFiles;
    QString hgt, msg;
    QString elevationRes = "3";
    bool usegdal = (chopTool == "gdalchop") ? true : false;
    ie = pwsu->wSrtmList.end();
    if (max2) {
        ib = pwsu->wSrtmList.begin();
        max2 = 0;
        for (; ib != ie; ib++) {
            hgt = ib.value();
            if (f.exists(hgt)) {
                hgtFiles.push_back(hgt);
                max2++;
            }
        }
        if (!max2) {
            // TODO: Message
            return;
        }
    }
    else {
        // TODO messag
        return;
    }
    QString toolDir(main->m_tabSetup->m_tgtoolEdit->text());
    QString tgTool = toolDir + "/" + chopTool;
#ifdef Q_OS_WIN
    tgTool += ".exe";
#endif
    if (!f.exists(tgTool)) {
        // TODO: Message
        return;
    }

    //QString srcDir(main->m_tabSetup->m_hgtrootEdit->text());
    QString targDir(hgtEdit->text());   // HGT Dir
    QString outDir(targDir);
    outDir += "/SRTM-" + elevationRes;

    // TODO: Get ENV from Advanced
    QString envPath = "X:\\3rdParty.x64\\bin";
    // for gdal DLL
    if (usegdal)
        envPath += ";Z:\\software.x64\\bin;X:\\install\\msvc140-64\\PROJ4\\bin";

    QString arguments;
    QStringList argList;
    max2 = hgtFiles.size();
    // build up an argument list, one for each HGT file to process - chop into arrays
    for (ii = 0; ii < max2; ii++) {
        hgt = hgtFiles.at((int)ii);
        if (usegdal) {
            // arguments += "\"" + tgTool + "\" \"" + projectDir + "/work/SRTM-" + elevationRes + "\" \"" + hgt + "\"";
            arguments = "\"" + tgTool + "\" \"" + outDir + "\" \"" + hgt + "\"";
        }
        else {
            // arguments += "\"" + tgTool + "\" " + elevationRes + " \"" + hgt + "\" \"" + projectDir + "/work/SRTM-" + elevationRes + "\"";
            arguments = "\"" + tgTool + "\" " + elevationRes + " \"" + hgt + "\" \"" + outDir + "\"";
        }
        argList += arguments;
        outLog(arguments);
    }
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", envPath + ";" + env.value("Path"));
    // to run batch files, like
    // QProcess* proc = new QProcess();
    // proc->start("cmd.exe /c \"call env.bat && script.bat\"");

    max2 = argList.size();
    for (ii = 0; ii < max2; ii++) {
        arguments = argList[(int)ii];
        QProcess proc;
        proc.setProcessEnvironment(env);
        proc.setWorkingDirectory(projectDir);
        proc.setProcessChannelMode(QProcess::MergedChannels);
        proc.start(arguments, QIODevice::ReadWrite);

        // run hgtchop or gdalchop command
        while (proc.waitForReadyRead()) {
            QCoreApplication::processEvents();

            //QString info(proc.readAll());
            QString info(proc.readAll().trimmed());
            //QString info(proc.readAll().trimmed().simplified());
            if (info.size())
                outLog(info);
        }

        // ui->convertElevationProgressBar->setValue((50 / argList.size())*i);  // hgtchop make progress btw 0% -> 50%
        proc.QProcess::waitForFinished(-1);
        // GUILog("ENDED in " + getElapTimeStg(pt.elapsed()) + " secondes\n", "default");

    }


}

QString get_last_arg(QString args)
{
    // 1 split sting
    QStringList words = args.split(QRegExp("\\s"));
    // 2 get length
    int max = words.size();
    if (max)    // if there is lenght
        max--;  // back up to last
    return words[max];  // return last
}


void tabElevations::on_terrafit_clicked() {
    // ***TBD*** // connect(actTerrafit,SIGNAL(clicked()),this,SLOT(on_terrafit_clicked())); // = new QPushButton("Run terrafit",this);
    QString fitTool = "terrafit";
    QString toolDir(main->m_tabSetup->m_tgtoolEdit->text());
    QString tgTool = toolDir + "/" + fitTool;
    QString elevationRes = "3"; // TODO: get from edit box

    QFile f;
#ifdef Q_OS_WIN
    tgTool += ".exe";
#endif
    if (!f.exists(tgTool)) {
        // TODO: Message
        return;
    }

    //QString srcDir(main->m_tabSetup->m_hgtrootEdit->text());
    QString projectDir = main->m_tabSetup->m_projdirEdit->text().trimmed();
    // TODO: Decide should this be Project directory + '/SRTM-$res', or...
    QString targDir(hgtEdit->text());   // HGT Dir
    QString outDir(targDir);
    outDir += "/SRTM-" + elevationRes;

    QStringList list1;
    QStringList list2;
    QString info, tmp;
    QDir d;
    size_t arr_cnt, fit_cnt;
    // QStringList elev_src;
    // elev_src << "SRTM-1" << "SRTM-3" << "SRTM-30";
    // if (m_elevDir.size() && d.exists(m_elevDir)) {
    //list1 = findFiles(m_elevDir,QStringList() << "*",true);
    QStringList dirList;
    QString dir;

    if ( d.exists(outDir) ) {
        dir = outDir;
        list1 = findFiles(dir, QStringList() << "*.arr.gz", true);
        list2 = findFiles(dir, QStringList() << "*.fit.gz", true);
        arr_cnt = list1.size();
        fit_cnt = list2.size();
        tmp.sprintf("\nterrafit: Found work dir with %d arr.gz files, and %d fit files.",
            arr_cnt, fit_cnt);
        info += tmp;
        if (arr_cnt) {
            dirList += dir;
            if (arr_cnt == fit_cnt) {
                info += "\nLook like elevations have been completed, ready for airport building, and final construction.";
            }
            else if (fit_cnt == 0) {
                info += "\nLook like 'Terrafit' needs to be run.";
            }
        }
    }
    else {
        info += "\noutDir " + outDir + " NOT FOUND. Need to run the 'chop' tools...";
    }
    outLog(info);
    append_information(info);

    QString arguments;
    QStringList argList;
    size_t ii, max2 = dirList.size();
    for (ii = 0; ii < max2; ii++) {
        dir = dirList.at((int)ii);
        arguments = tgTool + " " + dir;
        argList += arguments;
        outLog(arguments);
    }
    max2 = argList.size();
    if (max2) {
        // TODO: Get ENV from Advanced
        QString envPath = "X:\\3rdParty.x64\\bin";
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//#ifdef Q_OS_WIN
        env.insert("PATH", envPath + ";" + env.value("Path"));
//#endif
        for (ii = 0; ii < max2; ii++) {
            arguments = argList[(int)ii];
            dir = get_last_arg(arguments);
            list2 = findFiles(dir, QStringList() << "*.fit.gz", true);
            fit_cnt = list2.size();
            QProcess proc;
            QString info;
            proc.setProcessEnvironment(env);
            proc.setWorkingDirectory(projectDir);
            proc.setProcessChannelMode(QProcess::MergedChannels);
            proc.start(arguments, QIODevice::ReadWrite);

            // run command
            while (proc.waitForReadyRead()) {
                QCoreApplication::processEvents();

                //QString info(proc.readAll());
                info = proc.readAll().trimmed();
                //QString info(proc.readAll().trimmed().simplified());
                if (info.size())
                    outLog(info);
            }

            // ui->convertElevationProgressBar->setValue((50 / argList.size())*i);  // hgtchop make progress btw 0% -> 50%
            proc.QProcess::waitForFinished(-1);
            // GUILog("ENDED in " + getElapTimeStg(pt.elapsed()) + " secondes\n", "default");
            list2 = findFiles(dir, QStringList() << "*.fit.gz", true);
            if (fit_cnt == list2.size()) {
                info.sprintf("\nterrafit: Operation did not change the fit count %d", (int)fit_cnt);
            }
            else {
                info.sprintf("\nterrafit: Fit count went from %d to %d", (int)fit_cnt, (int)list2.size());
            }
            outLog(info);
            append_information(info);
        }
    }
}

void tabElevations::append_information(QString str)
{
    infoEdit->moveCursor(QTextCursor::End);
    infoEdit->insertPlainText(str);
    infoEdit->moveCursor(QTextCursor::End);
}


// eof - tabelevations.cpp
