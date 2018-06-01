/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#include "tabconstruction.h"
#include "mainwindow.h"
#include "utilities/app_macros.h"

tabConstruction::tabConstruction(QWidget *parent) :
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
    QRadioButton *sceneryButton = new QRadioButton("Scenery Limits",this);
    QRadioButton *airportButton = new QRadioButton("Tile ID",this);

    button_count = 0;
    layoutButtons->addWidget(sceneryButton);
    buttonGroup->addButton(sceneryButton, button_count);
    button_count++;
    layoutButtons->addWidget(airportButton);
    buttonGroup->addButton(airportButton, button_count);
    button_count++;

    row = 0;
    col = 0;
    max_col = 0;
    tmp = "";

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
    minlat1Label = new QLabel("Center Lat:",this);
    minlat2Label = new QLabel("         ",this);
    minlon1Label = new QLabel("Center Lon:",this);
    minlon2Label = new QLabel("         ",this);
    maxlat1Label = new QLabel("x-distance:",this);
    maxlat2Label = new QLabel("         ",this);
    maxlon1Label = new QLabel("y-distance:",this);
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

    oneLabel = new QLabel("Tile ID",this);
    oneEdit = new QLineEdit(this);
    oneInfo = new QLabel("Set to build single bucket index only",this);

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
    limitLayout->addWidget(oneLabel,row,col);
    col++;
    limitLayout->addWidget(oneEdit,row,col);
    col++;
    limitLayout->addWidget(oneInfo,row,col,1,max_col-2);

    QHBoxLayout *layoutBox = new QHBoxLayout;
    layoutBox->setContentsMargins(0, 0, 0, 0);
    layoutBox->setSpacing(0);

    //mainLayout->addWidget(layoutBox);
    mainLayout->addItem(layoutBox);

    // ===================================
    // TERRAIN TYPES - LEFT
    // ===================================
    QGroupBox *terrainGroup = new QGroupBox(tr("Terrain Types"),this);
    //mainLayout->addWidget(terrainGroup);

    QVBoxLayout *layoutData = new QVBoxLayout();
    layoutData->setContentsMargins(10, 0, 10, 0);
    layoutData->setSpacing(0);
    terrainGroup->setLayout(layoutData);

    listWidget = new QListWidget(this);
    //listWidget->setBaseSize(200,16*14);
    //listWidget->setFrameRect(rect);
    //listWidget->setFixedHeight(200);
    listWidget->setFixedWidth(200);
    listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    layoutData->addWidget(listWidget);

    // ===================================
    // OPTIONS
    // ===================================
    QGroupBox *optionGroup = new QGroupBox(tr("Options"),this);
    //mainLayout->addWidget(optionGroup);

    QVBoxLayout *layoutOptions = new QVBoxLayout();
    layoutOptions->setContentsMargins(10, 0, 10, 0);
    layoutOptions->setSpacing(0);
    optionGroup->setLayout(layoutOptions);

    useukCheck = new QCheckBox("Use UK grid.",this);
    ignlmCheck = new QCheckBox("Ignore Landmass.",this);
    noovrCheck = new QCheckBox("No overwrite existing.",this);
    nosrcCheck = new QCheckBox("Skip if no source data.",this);
    ignerrsCheck = new QCheckBox("Ignore Errors.",this);
    skipdnCheck = new QCheckBox("Skip done confirmations.",this);
    nopriorCheck = new QCheckBox("No --priorities command.",this);

    layoutOptions->addWidget(useukCheck);
    layoutOptions->addWidget(ignlmCheck);
    layoutOptions->addWidget(noovrCheck);
    layoutOptions->addWidget(nosrcCheck);
    layoutOptions->addWidget(ignerrsCheck);
    layoutOptions->addWidget(skipdnCheck);
    layoutOptions->addWidget(nopriorCheck);

    // left = options
    layoutBox->addWidget(optionGroup);
    // right = terrain list
    layoutBox->addWidget(terrainGroup);

    // ===================================
    // ACTIONS
    // ===================================
    QGroupBox *actionGroup = new QGroupBox(tr("Actions"),this);
    mainLayout->addWidget(actionGroup);
    QHBoxLayout *layoutActions = new QHBoxLayout();
    layoutActions->setContentsMargins(10, 0, 10, 0);
    layoutActions->setSpacing(0);
    actionGroup->setLayout(layoutActions);

    updateButton = new QPushButton("Update Terrain List",this);
    generateButton = new QPushButton("Generate Scenery",this);

    layoutActions->addWidget(updateButton);
    layoutActions->addWidget(generateButton);

    connect(updateButton,SIGNAL(clicked()), this, SLOT(on_update_clicked()));
    connect(generateButton, SIGNAL(clicked()), this, SLOT(on_generate_clicked()));

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
    /* end tabConstruction */

}

void tabConstruction::finalize()
{
    // connect to handler, and restore previous checks
    MCONSET_T(useukCheck,on_useuk_clicked,co_useukPath,"0");    // = new QCheckBox("Use UK grid.",this);
    MCONSET_T(ignlmCheck,on_ignlm_clicked,co_ignlmPath,"0");    // = new QCheckBox("Ignore Landmass.",this);
    MCONSET_T(noovrCheck,on_noovr_clicked,co_noovrPath,"1");        // = new QCheckBox("No overwrite existing.",this);
    MCONSET_T(nosrcCheck,on_nosrc_clicked,co_nosrcPath,"1");    // = new QCheckBox("Skip if no source data.",this);
    MCONSET_T(ignerrsCheck,on_ignerrs_clicked,co_ignerrsPath,"0");  // = new QCheckBox("Ignore Errors.",this);
    MCONSET_T(skipdnCheck,on_skipdn_clicked,co_skipdnPath,"0"); // = new QCheckBox("Skip done confirmations.",this);
    MCONSET_T(nopriorCheck,on_noprior_clicked,co_nopriorPath,"0");  // = new QCheckBox("No --priorities command.",this);

    // ***TBD*** connect and perform actions
    int id = main->m_Settings->value(constbutPath, "0").toInt();
    buttonGroup->button(id)->setChecked(true);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(on_button_clicked(int)));
    on_button_clicked(id);

    on_update_clicked();
    // anything to be done
}

void tabConstruction::on_button_clicked(int id)
{
        bool b0, b1;
        b0 = true;
        b1 = false;
        switch (id)
        {
        case 0:
            // choice 0
            b0 = true;
            b1 = false;
            break;
        case 1:
            b0 = false;
            b1 = true;
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

        // choice 1
        oneLabel->setEnabled(b1);   // = new QLabel("Airport ICAO",this);
        oneEdit->setEnabled(b1); // = new QLineEdit(this);
        oneInfo->setEnabled(b1);    // = new QLabel("Set to build one airport only",this);

        QString val;
        val.sprintf("%d",id);
        if ((id != -1)&&(id >= 0)&&(id < button_count))
            main->m_Settings->setValue(constbutPath, QVariant(val));
}


int tabConstruction::process_Landuse(QString & selectedMaterials,
                                 QString & results,
                                 bool in_detail )
{
    int maxItems = listWidget->count();
    int folderCnt = 0; // START COUNTER
    QString test;
    QStringList srtm;
    int flag = 0;
    int i;

    // create string with selected terraintypes
    for (i = 0; i < maxItems; ++i) {
        test = listWidget->item(i)->text(); // get landuse folder
        if (listWidget->item(i)->isSelected() == 1) {
            selectedMaterials += test+" ";
            folderCnt++;
            if (test == "AirportObj") {
                // could also check for ICAO.btg.gz and <index>.ind files
                flag |= flg_AirObj;
            } else if (test == "AirportArea") {
                // could also check for <chunk>/<tile>/<index>.NNN array files
                // and maybe even poly_counter to check genapts WAS run
                flag |= flg_AirArea;
            } else if (test == "Default") {
                // could also check for <chunk>/<tile>/<index>.NNN array files
                flag |= flg_Default;
            } else if ( srtm_isElevationDir(test) ) {
                // hmmm, of course it IS possible to use other than this
                // hard coded set of directories, but...
                flag |= flg_SRTM;
                srtm += test;
            } else if (test == "Landmass") {
                // again could check more...
                flag |= flg_Land;
            }
        }
    }
    QString info;
    QString tmp;
    QString dir;
    QStringList list;
    //info.sprintf("List of %d landuse, with %d selected items.",
    //             maxItems, folderCnt);
    info.sprintf("List of %d landuse items.", maxItems);

    // Elevations
    // ==========================================================
    if (flag & flg_SRTM) {
        tmp = "\nFound Elevation folder.";
        if (in_detail) {
            i = 0;
            foreach (dir, srtm) {
                dir = m_workDir + "/" + dir;
                //NEWWAITMESSAGE("Finding elevation array files...");
                list = findFiles(dir, QStringList() << "*.arr.gz", true);
                i += list.size();
            }
            tmp.sprintf("\nFound %d Elevation folder with %d arrays.", srtm.size(), i);
            if (i)
                flag |= flg_SRTM_cnt;
        }
    } else {
        tmp = "\nWARNING: NO elevation folders found = A FLAT WORLD AT -9999 FEET!";
    }
    info += tmp;


    // Airports
    // ==========================================================
    // AirportObj
    dir = m_workDir + "/AirportObj";
    if (flag & flg_AirObj) {
        tmp = "\nFound AirportObj folder.";
        if (in_detail) {
            // NEWWAITMESSAGE("Finding airport BTG files...");
            list = findFiles(dir, QStringList() << "*.btg.gz", true);
            tmp.sprintf("\nFound AirportObj with %d a/p files.", list.size());
            if (list.size())
                flag |= flg_AirObj_cnt;
        }
    } else {
        tmp = "\nWARNING: No AirportObj folder found = NO AIRPORTS!";
    }
    info += tmp;

    // AirportArea
    dir = m_workDir + "/AirportArea";
    if (flag & flg_AirArea) {
        if (in_detail) {
            // NEWWAITMESSAGE("Finding airport cutout arrays...");
            list = findFiles(dir, QStringList() << "*", true);
            tmp.sprintf("\nFound AirportArea with %d files.", list.size());
            if (list.size())
                flag |= flg_AirArea_cnt;
        } else {
            tmp = "\nFound AirportArea folder.";
        }
    } else {
        tmp = "\nWARNING: No AirportArea folder found = NO AIRPORT CUTOUTS!";
    }
    info += tmp;

    // Default area
    // ==========================================================
    // Default
    dir = m_workDir + "/Default";
    if (flag & flg_Default) {
        tmp = "\nFound Default folder.";
        if (in_detail) {
            // NEWWAITMESSAGE("Finding default area arrays...");
            list = findFiles(dir, QStringList() << "*", true);
            tmp.sprintf("\nFound Default with %d files.", list.size());
            if (list.size())
                flag |= flg_Default_cnt;
        }
    } else {
        tmp = "\nWARNING: No Default folder found = NO DEFAULT AREA!";
    }
    info += tmp;


    // Landmass Area
    // ==========================================================
    // Landmass
    dir = m_workDir + "/Landmass";
    if (flag & flg_Land) {
        tmp = "\nFound Landmass folder.";
        if (in_detail) {
            // NEWWAITMESSAGE("Finding Landmass area arrays...");
            list = findFiles(dir, QStringList() << "*", true);
            tmp.sprintf("\nFound Landmass with %d files.", list.size());
            if (list.size())
                flag |= flg_Land_cnt;
        }
    } else {
        tmp = "\nWARNING: No Landmass folder found = NO LANDMASS AREA!";
    }
    info += tmp;

    // Summary
    // ==========================================================
    if ((flag & flg_ALL) == flg_ALL) {
        info += "\nConstruction looks good to go ;=))";
    }
    if ((flag & flg_ALL_cnt) == flg_ALL_cnt) {
        info += "\nwith files in each category- congrats!";
    }
    results = info;
    return folderCnt;
}


// generate code block handlers for checks
#define M_CONSET_T  M_CONSET_T_CO
//#define M_CONSET_E  M_CONSET_E_CO

M_CONSET_T(useukCheck,on_useuk_clicked,co_useukPath,"0");    // = new QCheckBox("Use UK grid.",this);
M_CONSET_T(ignlmCheck,on_ignlm_clicked,co_ignlmPath,"0");    // = new QCheckBox("Ignore Landmass.",this);
M_CONSET_T(noovrCheck,on_noovr_clicked,co_noovrPath,"1");        // = new QCheckBox("No overwrite existing.",this);
M_CONSET_T(nosrcCheck,on_nosrc_clicked,co_nosrcPath,"1");    // = new QCheckBox("Skip if no source data.",this);
M_CONSET_T(ignerrsCheck,on_ignerrs_clicked,co_ignerrsPath,"0");  // = new QCheckBox("Ignore Errors.",this);
M_CONSET_T(skipdnCheck,on_skipdn_clicked,co_skipdnPath,"0"); // = new QCheckBox("Skip done confirmations.",this);
M_CONSET_T(nopriorCheck,on_noprior_clicked,co_nopriorPath,"0");  // = new QCheckBox("No --priorities command.",this);

// ACTIONS
void tabConstruction::on_update_clicked()
{
    int i, j = 0;
    QString tmp;
    QString info;
    QDir dir(m_workDir);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    listWidget->clear(); // clear the list
    QFileInfoList list = dir.entryInfoList();
    j = 0;
    for (i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        // do not add the Shared folder
        if (fileInfo.fileName() != "Shared"){
            //QString test = qPrintable(QString("%1").arg(fileInfo.fileName()));
            //QString test = fileInfo.fileName();
            new QListWidgetItem(tr(qPrintable(QString("%1").arg(fileInfo.fileName()))), listWidget);
            // select all materials per default
            listWidget->item(j)->setSelected(1);
            j++;
        }
    }
    //j = process_Landuse(tmp,info);
    j = process_Landuse(tmp,info,true);
    outLog("Landuse info\n"+info+"\n");
    tmp = info;
    //util_CleanLine(tmp);
    infoEdit->setPlainText(tmp);

}

void tabConstruction::on_generate_clicked()
{

}

// eof - tabconstruction.cpp
