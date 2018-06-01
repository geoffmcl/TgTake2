/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#include "tabairports.h"
#include "mainwindow.h"
#include "tg_utils.h"

#define ED_CONNECT(a,b)    connect(a,SIGNAL(editingFinished()),this,SLOT(b))

#define ED_SAVE(a,b)    { QString _tmp = a->text().trimmed(); \
    if (_tmp.length()) main->saveSetting(b,_tmp); }

#define ED_SET(a,b) a->setText(main->m_Settings->value(b,"").toString())

/* -------
   line = new QFrame(w);
   line->setObjectName(QString::fromUtf8("line"));
   line->setGeometry(QRect(320, 150, 118, 3));
   line->setFrameShape(QFrame::HLine);
   line->setFrameShadow(QFrame::Sunken);
   ------- */
tabAirports::tabAirports(QWidget *parent) :
    QWidget(parent)
{
    int row, col;
    int spac  = 4;
    int left  = 5;
    int top   = 5;
    int right = 5;
    int bott  = 5;
    int max_col;
    QString tmp;

    main = (MainWindow *)parent;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(left, top, right, bott);
    mainLayout->setSpacing(spac);
    setLayout(mainLayout);

    // ===================================
    // DATABASE
    // ===================================
    QGroupBox *dataGroup = new QGroupBox(tr("Airport Database - green=ok, red=not found"),this);
    mainLayout->addWidget(dataGroup);

    QHBoxLayout *layoutData = new QHBoxLayout();
    layoutData->setContentsMargins(10, 0, 10, 0);
    layoutData->setSpacing(0);
    dataGroup->setLayout(layoutData);

    fileLabel = new QLabel("Airport File:",this);
    fileEdit = new QLineEdit(this);
    fileBrowse = new QPushButton("...",this);
    layoutData->addWidget(fileLabel);
    layoutData->addWidget(fileEdit);
    layoutData->addWidget(fileBrowse);

    tmp = main->m_Settings->value(fileaptPath,"").toString();
    if (tmp.isEmpty()) {
        tmp = main->m_tabSetup->m_fgrootEdit->text();
        if (!tmp.isEmpty()) {
            tmp.append("/Airports/apt.dat.gz");
            QFile f;
            if (!f.exists(tmp))
                tmp = "";
        }
    }
    fileEdit->setText(tmp);
    connect(fileEdit,SIGNAL(editingFinished()),this,SLOT(on_file_edit()));
    connect(fileBrowse,SIGNAL(clicked()),this,SLOT(on_file_browse()));

    // ===================================
    // CHOICES
    // ===================================
    QGroupBox *limitGroup = new QGroupBox(tr("Choose Limit Type"),this);
    mainLayout->addWidget(limitGroup);

    QHBoxLayout *layoutButtons = new QHBoxLayout;
    layoutButtons->setContentsMargins(10, 0, 10, 0);
    layoutButtons->setSpacing(0);
    limitGroup->setLayout(layoutButtons);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);
    QRadioButton *sceneryButton = new QRadioButton(tr("Use Scenery Limits"),this);
    QRadioButton *airportButton = new QRadioButton(tr("One Airport Only"),this);
    QRadioButton *chunkButton = new QRadioButton(tr("Tile or Chunk Limit"),this);

    layoutButtons->addWidget(sceneryButton);
    buttonGroup->addButton(sceneryButton, 0);
    layoutButtons->addWidget(airportButton);
    buttonGroup->addButton(airportButton, 1);
    layoutButtons->addWidget(chunkButton);
    buttonGroup->addButton(chunkButton, 2);

    buttonGroup->button(main->m_Settings->value(choicebutPath, "0").toInt())->setChecked(true);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(on_button_click(int)));

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

    allaptsCheck = new QCheckBox("Do _ALL_ airports",this);
    allaptsLabel = new QLabel("Need ALL elevations, and takes a LONG time!");
    allaptsCheck->setChecked(main->m_Settings->value(allaptsPath, "0").toBool());
    connect(allaptsCheck,SIGNAL(toggled(bool)),this,SLOT(on_allapts_toggle(bool)));

    startLabel = new QLabel("Start Airport ICAO",this);
    startEdit = new QLineEdit(this);
    connect(startEdit,SIGNAL(editingFinished()),this,SLOT(on_startEdit_finished()));
    startInfo = new QLabel("Set to start at this airport",this);
    ED_SET(startEdit,ap_startEdit);

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
    col++;
    max_col = col;
    row++;
    col = 0;
    limitLayout->addWidget(allaptsCheck,row,col,1,3);
    col += 3;
    limitLayout->addWidget(allaptsLabel,row,col,1,max_col - 4);
    row++;
    col = 0;
    limitLayout->addWidget(startLabel,row,col,1,2);
    col += 2;
    limitLayout->addWidget(startEdit,row,col,1,2);
    col += 2;
    limitLayout->addWidget(startInfo,row,col,1,max_col - 4);

    oneLabel = new QLabel("Airport ICAO",this);
    oneEdit = new QLineEdit(this);
    ED_CONNECT(oneEdit,on_oneEdit_finished());
    oneInfo = new QLabel("Set to build one airport only",this);
    ED_SET(oneEdit,ap_oneEdit);
    row++;
    col = 0;
    limitLayout->addWidget(oneLabel,row,col,1,2);
    col += 2;
    limitLayout->addWidget(oneEdit,row,col,1,2);
    col += 2;
    limitLayout->addWidget(oneInfo,row,col,1,max_col - 4);

    tileLabel = new QLabel("Tile ID",this);
    tileEdit = new QLineEdit(this);
    ED_SET(tileEdit,ap_tileId);
    ED_CONNECT(tileEdit,on_tileEdit_finished());

    orLabel = new QLabel("   or  ",this);

    chunkLabel = new QLabel("Chunk ID",this);
    chunkEdit = new QLineEdit(this);
    ED_SET(chunkEdit,ap_chunkId);
    ED_CONNECT(chunkEdit,on_chunkEdit_finished());

    row++;
    col = 0;
    limitLayout->addWidget(tileLabel,row,col);
    col++;
    limitLayout->addWidget(tileEdit,row,col);
    col++;
    limitLayout->addWidget(orLabel,row,col);
    col++;
    limitLayout->addWidget(chunkLabel,row,col);
    col++;
    limitLayout->addWidget(chunkEdit,row,col);

    // ===================================
    // OPTIONS
    // ===================================
    QGroupBox *optionGroup = new QGroupBox(tr("Options"),this);
    mainLayout->addWidget(optionGroup);

    QGridLayout *optionLayout = new QGridLayout;
    optionLayout->setContentsMargins(left, top, right, bott);
    optionLayout->setSpacing(spac);
    optionGroup->setLayout(optionLayout);

    maxslopeLabel = new QLabel("Max. runway slope",this);
    maxslopeEdit = new QLineEdit(this);
    nudgeLabel = new QLabel("Nudge factor:",this);
    nudgeEdit = new QLineEdit(this);
    limdemCheck = new QCheckBox("Limit DEM paths.",this);

    row = 0;
    col = 0;
    optionLayout->addWidget(maxslopeLabel,row,col);
    col++;
    optionLayout->addWidget(maxslopeEdit,row,col);
    col++;
    // =======
    col++;
    optionLayout->addWidget(nudgeLabel,row,col);
    col++;
    optionLayout->addWidget(nudgeEdit,row,col);
    row++;
    col = 0;
    optionLayout->addWidget(limdemCheck,row,col,1,2);
    col += 2;

    // ===================================
    // ACTIONS
    // ===================================
    QGroupBox *actionGroup = new QGroupBox(tr("Actions"),this);
    mainLayout->addWidget(actionGroup);

    QHBoxLayout *layoutActions = new QHBoxLayout;
    layoutActions->setContentsMargins(10, 0, 10, 0);
    layoutActions->setSpacing(0);
    actionGroup->setLayout(layoutActions);

    genaptsAction = new QPushButton("Generate Airport(s)",this);
    layoutActions->addWidget(genaptsAction);
    connect(genaptsAction,SIGNAL(clicked()),this,SLOT(on_genapts_triggered()));

    // ===================================
    // INFORMATION
    // ===================================
    QGroupBox *infoGroup = new QGroupBox(tr("Information"),this);
    mainLayout->addWidget(infoGroup);

    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->setContentsMargins(left, top, right, bott);
    infoLayout->setSpacing(spac);
    infoGroup->setLayout(infoLayout);

    infoEdit = new QPlainTextEdit(this);
    infoEdit->setReadOnly(true);

    infoLayout->addWidget(infoEdit);
}

void tabAirports::finalize()
{
    int id = buttonGroup->checkedId();
    on_button_click(id);
    on_file_edit();
    maxslopeEdit->setText(main->m_Settings->value(maxslopePath,"").toString());
    nudgeEdit->setText(main->m_Settings->value(nudgePath,"").toString());

    // ***TBD*** any other setup

}

void tabAirports::on_button_click(int id)
{
    bool b0, b1, b2;
    b0 = true;
    b1 = false;
    b2 = false;
    switch (id)
    {
    case 0:
        // choice 0
        b0 = true;
        b1 = false;
        b2 = false;
        break;
    case 1:
        b0 = false;
        b1 = true;
        b2 = false;
        break;
    case 2:
        b0 = false;
        b1 = false;
        b2 = true;
        break;
    }

    // choice 0
    minlat1Label->setEnabled(b0);
    minlat2Label->setEnabled(b0);
    minlon1Label->setEnabled(b0);
    minlon2Label->setEnabled(b0);
    maxlat1Label->setEnabled(b0);
    maxlat2Label->setEnabled(b0);
    maxlon1Label->setEnabled(b0);
    maxlon2Label->setEnabled(b0);
    allaptsCheck->setEnabled(b0);
    allaptsLabel->setEnabled(b0);
    startLabel->setEnabled(b0);     // = new QLabel("Start Airport ICAO",this);
    startEdit->setEnabled(b0);   // = new QLineEdit(this);
    startInfo->setEnabled(b0);      // = new QLabel("Set to start at this airport",this);

    // choice 1
    oneLabel->setEnabled(b1);   // = new QLabel("Airport ICAO",this);
    oneEdit->setEnabled(b1); // = new QLineEdit(this);
    oneInfo->setEnabled(b1);    // = new QLabel("Set to build one airport only",this);

    // choice 2
    tileLabel->setEnabled(b2);;  // = new QLabel("Tile ID",this);
    tileEdit->setEnabled(b2);    // = new QLineEdit(this);
    orLabel->setEnabled(b2);     // = new QLabel("   or  ",this);
    chunkLabel->setEnabled(b2);  // = new QLabel("Chunk ID",this);
    chunkEdit->setEnabled(b2);   // = new QLineEdit(this);

    QString val;
    val.sprintf("%d",id);
    if (id != -1)
        main->m_Settings->setValue(choicebutPath, QVariant(val));

    set_airport_info();
}

void tabAirports::on_file_edit()
{
    QString tmp;
    QFile f;
    QString style;
    tmp = fileEdit->text();
    if (f.exists(tmp)) {
        style = "color:green";
        main->saveSetting(fileaptPath,tmp);
    } else {
        style = "color:red";
    }
    fileLabel->setStyleSheet(style);
}

void tabAirports::on_file_browse()
{
    main->setStatusMessage("Set apt.dat.gz file",5000);
    QString prompt(tr("Select valid apt.dat[.gz] airport file"));
    QString current = fileEdit->text();
    QString filePath = util_browseFile(prompt,current);
    QFile f;
    if (!filePath.isEmpty() && f.exists(filePath)) {
        fileEdit->setText(filePath);
        on_file_edit();
    }
}

void tabAirports::on_allapts_toggle(bool b)
{
    main->saveSetting(allaptsPath,b);
}

void tabAirports::on_maxslope_edit() {    // Path "max/slope"     //= new QLineEdit(this);
    main->saveSetting(maxslopePath,maxslopeEdit->text());
}

void tabAirports::on_nudge_edit() {   // Path "nudge/factor"  //= new QLineEdit(this);
    main->saveSetting(nudgePath,nudgeEdit->text());
}

void tabAirports::on_limdem_toggle(bool b) { // Path "check/limdem"  //= new QCheckBox("Limit DEM paths.",this);
    main->saveSetting(limdemPath,b);
}

void tabAirports::on_startEdit_finished() {
    ED_SAVE(startEdit,ap_startEdit)
    set_airport_info();
}
void tabAirports::on_oneEdit_finished() {
    ED_SAVE(oneEdit,ap_oneEdit)
    set_airport_info();
}
void tabAirports::on_tileEdit_finished() {
    ED_SAVE(tileEdit,ap_tileId)
    set_airport_info();
}
void tabAirports::on_chunkEdit_finished() {
    ED_SAVE(chunkEdit,ap_chunkId)
    set_airport_info();
}


void tabAirports::set_airport_info()
{
    QString tmp;
    QFile f;
    QString info;
    // 1 check the file exist
    tmp = fileEdit->text();
    if (f.exists(tmp)) {
        info += "Found file ["+tmp+"]\n";
    } else {
        info += "File ["+tmp+"] NOT FOUND\n";
    }
    // 2 check limits in place
    int id = buttonGroup->checkedId();
    switch (id) {
    case 0:
        // a - Range, possibly starting at
        info += "Range:";
        info += " Min. Lat. ";
        info += minlat2Label->text().trimmed();
        info += " Min. Lon. ";
        info += minlon2Label->text().trimmed();
        info += " Max. Lat. ";
        info += maxlat2Label->text().trimmed();
        info += " Max. Lon. ";
        info += maxlon2Label->text().trimmed();
        info += "\n";
        if (allaptsCheck->isChecked()) {
            info += "all airports - takes lot of time\n";
        } else {
            tmp = startEdit->text().trimmed();
            if (tmp.length())
                info += "Start at ICAO "+tmp+"\n";
        }
        break;
    case 1:
        // b - One airport ICAO
        info += "One airport ICAO ";
        tmp = oneEdit->text().trimmed();
        if (tmp.length())
            info += tmp;
        else
            info += "***WARNING: NOT SET***";
        break;
    case 2:
        // c - Per tile ID or chunk value
        tmp = tileEdit->text().trimmed();
        if (tmp.length())
            info += "Per Tile ID "+tmp;
        else {
            tmp = chunkLabel->text().trimmed();
            if (tmp.length())
                info += "Per CHUNK "+tmp;
            else
                info += "***WARNING: Neither Tile ID nor CHUNK values set!***";
        }
        info += "\n";
        break;
    }

    infoEdit->setPlainText(info);
}

// genapts command
// ======================================
// generate airports
// ======================================
//void MainWindow::on_pushButton_genair_clicked()
void tabAirports::on_genapts_triggered()
{
// #if 0 // ======================
    LL ll;
    QString info;
    QString tmp;
    QDir d;
    // options
    QString maxSlope  = maxslopeEdit->text().trimmed();
    QString nudge = nudgeEdit->text().trimmed();
    bool limit_dem = limdemCheck->isChecked();
    int m_MaxDiag = main->m_tabAdvanced->maxEdit->text().toInt();
    bool ignore_ranges = false;
    int id = buttonGroup->checkedId();
    if (id != 0)
        ignore_ranges = true;

    QString airportId   = oneEdit->text().trimmed(); // given a SPECIFIC a/p
    QString startAptId  = startEdit->text().trimmed(); // set a START

    QString tileId  = tileEdit->text().trimmed(); // set min/max from this
    QString chunkId = chunkEdit->text().trimmed(); // or from this

    QString aptFile = fileEdit->text().trimmed();

    // GLOBAL Scenery Limits
    QString m_south = minlat2Label->text().trimmed();
    QString minLat  = m_south; // = min. lat
    QString m_north = maxlat2Label->text().trimmed();
    QString maxLat  = m_north; // = max. lat
    QString m_west  = minlon2Label->text().trimmed();
    QString minLon  = m_west;  // = min. lon
    QString m_east  = maxlon2Label->text().trimmed();
    QString maxLon  = m_east;  // = max. lon

    QString runtime = main->m_tabSetup->m_tgtoolEdit->text().trimmed(); //toolDir+"/genapts";
    runtime += "/genapts";
    QString projectDir = main->m_tabSetup->m_projdirEdit->text().trimmed();
    QString airOut = projectDir; //m_workDir+"/AirportObj";
    QString outDir = airOut;    // m_workDir;
    QString demDir = airOut;    // m_workDir+"/SRTM-30";
    airOut += "/AirportObj";
    demDir += "/SRTM-30";

    QString arguments;
    int res, ver, air_cnt, ind_cnt;
    bool check_limits = true;
    bool range_ok = true; // assumed ok - only checked if used

    if (runtime.contains(QChar(' ')))
        runtime = "\""+runtime+"\"";
    if (aptFile.contains(QChar(' ')))
        aptFile = "\""+aptFile+"\"";
    if (outDir.contains(QChar(' ')))
        outDir = "\""+outDir+"\"";
    if (demDir.contains(QChar(' ')))
        demDir = "\""+demDir+"\"";

    bool m_Verify_Tools = false; // ui->checkBox_verify->isChecked();

    double south = minLat.toDouble();
    double north = maxLat.toDouble();
    double east = maxLon.toDouble();
    double west = minLon.toDouble();

    // Unable to proceed, if no valid project directory
    if (!util_verifyProjDir(projectDir))
        return; // genapts - projectDir invalid

    QStringList airList = findFiles(airOut, QStringList() << "*.gz", true);
    QStringList indList = findFiles(airOut, QStringList() << "*.ind", true);
    air_cnt = airList.size();
    ind_cnt = indList.size();

    arguments = runtime;
    arguments += " --help";
    res = tg_testApplication(arguments, info);
    ver = util_lookslike_genapts(info);
    if (res || m_Verify_Tools) {
        if (!m_Verify_Tools && (res == 255) && ver) {
            // looks good to go ;=))
        } else {
            // ask the user to continue
            // util_trimLongMessage(m_MaxDiag, info);
            tg_trimLongMessage(m_MaxDiag, info);
            tmp.sprintf("\nTool returned value %d",res);
            info += tmp;
            info += "\nThe Terragear Tool directory is set on the Setup page.";
            outLog(info+"\n");
            info += "\n\nClick 'Yes' to continue to use this tool.";
            res = util_getYesNo("AIRPORT TOOL",info);
            if (!res)
                return; // airport tool NOT acceptable
        }
    }

    // begin command
    arguments = runtime + " --input="+aptFile+" --work="+outDir+" ";
    info = "About to run airport generation,\n";
    // ensure only the 'known'/'generated' elevation directory is used
    if (limit_dem) {
        arguments += "--clear-dem-path --dem-path="+demDir+" ";
        info += "limiting the elevation input to ["+demDir+"]\n";
    } else {
        info += "Using built-in default DEM paths.\n";
        info += "This can generate a lot of worthless output where elevation files do not exist.\n";
    }
    if (nudge.size() > 0) {
        arguments += "--nudge="+nudge+" ";
        info += "applying a nudge factor of ["+nudge+"]\n";
    }
    if (maxSlope.size() > 0) {
        arguments += "--max-slope="+maxSlope+" ";
        info += "setting max. runway slope to ["+maxSlope+"]\n";
    }
    if (airportId.size() > 0) {
        arguments += "--airport="+airportId+" ";
        info += "ONLY processing airport ["+airportId+"]\n";
    }
    if (startAptId.size() > 0) {
        arguments += "--start-id="+startAptId+" ";
        info += "STARTING at airport ["+startAptId+"]\n";
    }

    // set limits for airport generation
    if (tileId.size() > 0) {
        if ( !tg_ValidTileId(tileId, &ll)) {
            info = "Parameter error. The tile ID must be of the form\n";
            info += "[e|w]nnn[n|s]nn, length 7. Got ["+tileId+"]?\n";
            info += "Either remove this option, or correct to continue.";
            QMessageBox::information(this,"OPTION ERROR",info);
            return; // genapts aborted - not a valid tile ID
        }
        // use min 1x1 tile ID sizing
        arguments += "--tile="+tileId+" ";
        info += "Area limited to 1x1 degree tile ID ["+tileId+"]\n";
        south = ll.lat;
        north = south + 1.0;
        west = ll.lon;
        east = west + 1.0;
        check_limits = true;
    } else if (chunkId.size() > 0) {
        // use 10x10 degree chunk size
        if ( !tg_ValidChunkId(chunkId, &ll)) {
            info = "Parameter error. The tile ID must be of the form\n";
            info += "[e|w]nnn[n|s]nn, length 7. Got ["+chunkId+"]?\n";
            info += "Either remove this option, or correct to continue.";
            QMessageBox::information(this,"OPTION ERROR",info);
            return; // genapts aborted - not a valid chunk ID
        }
        arguments += "--chunk="+chunkId+" ";
        info += "Area limited to 10x10 degree chunk ID ["+chunkId+"]\n";
        south = ll.lat;
        north = south + 9.0;
        west = ll.lon;
        east = west + 9.0;
        check_limits = true;
    } else if (ignore_ranges) {
        info += "No area limit = Do all in airport file given.\n";
        info += "Make sure you have all the elevations needed, ";
        info += "downloaded and chopped!\n";
        info += "This can take a LONG time if using the 27K fgdata file.\n";
        south = -90.0;
        north = 90.0;
        west = -180.0;
        east = 180.0;
        check_limits = false;
    } else {
        if ((west < east) && (north > south) &&
            util_inWorldRange(north,east) &&
            util_inWorldRange(south,west)) {
            range_ok = true; // all in world range
        } else
            range_ok = false;
        // or use existing min/max
        arguments += "--min-lon="+minLon+" ";
        arguments += "--max-lon="+maxLon+" ";
        arguments += "--min-lat="+minLat+" ";
        arguments += "--max-lat="+maxLat+" ";
        info += "Area limited to lat/lon min "+minLat+","+minLon+", max "+maxLat+","+maxLon+"\n";
        // south, north, west and east already set
        check_limits = true;
    }

    info += "Full tools arguments are -\n";
    info += arguments;
    if (check_limits) {
        int total = 0;
        int count = srtm_verifySRTMfiles(south, north, west, east, m_workDir, &total);
        tmp.sprintf("\nFound %d of %d elevations.\n", count, total);
        info += tmp;
    } else {
        // hmmm doing all - how can I help - check what
        QStringList elevList = srtm_getSRTMIndexList(m_workDir);
        tmp.sprintf("\nFound %d elevation files.", elevList.size());
        info += tmp;
    }

    if (air_cnt) {
        tmp.sprintf("\n\nPresently have %d generated airports, %d *.ind files\n",
                    air_cnt, ind_cnt);
        info += tmp;
        info += "in the output target ["+airOut+"]\n";
    } else {
        info += "\nPresently no generated airports in target ["+airOut+"]\n";
    }
    if (range_ok) {
        info += "\nProceed with airport generations?";
        res = util_getYesNo("CONFIRM AIRPORT GENERATION",info);
        if (!res)
            return; // genapts - user aborted run
    } else {
        info += "\nPrimary min/max range error - aborting airport generation!";
        QMessageBox::critical(this,"BAD MIN/MAX RANGES",info);
        return; // genapts - ranges NOT correct
    }

#if 0 // 0000000000000000000000000000000000000000000000000000000
    // ok, now to do it ;=))
    //SHOWWAITDIALOG("AIRPORT GENERATION",info);
    qApp->processEvents();
    outArg(arguments);
    info = util_runProcess(arguments,"");
    util_trimLongMessage(m_MaxDiag, info);
    info += "\nDone airport generation.";
    airList = findFiles(airOut, QStringList() << "*.gz", true);
    indList = findFiles(airOut, QStringList() << "*.ind", true);
    if (air_cnt == 0) {
        // had none previous - was anything generated
        if (airList.count())
            tmp.sprintf("\nGenerated %d airports, %d ind files.",airList.count(), indList.count());
        else
            tmp = "\nLooks like an ERROR! No airports generated.";
    } else {
        // had some previous - were new items generated?
        if (airList.count() > air_cnt)
            tmp.sprintf("\nStarted with %d, now have %d airports. Created %d", air_cnt, airList.count(), airList.count()-air_cnt);
        else
            tmp.sprintf("\nStarted with %d, and have no new airports.", air_cnt);
    }
    info += tmp;
    CLOSEWAITDIALOG;
    outLog(info+"\n");
    QMessageBox::information(this,"DONE AIRPORT GENERATION",info);

#endif // 0

}

// eof - tabairports.cpp
