/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#include "app_config.h"
#include "tablanduse.h"
#include "mainwindow.h"
#include "utilities/tg_utils.h"
#include "utilities/utilities.h"
#include "utilities/app_macros.h"

tabLanduse::tabLanduse(QWidget *parent) :
    QWidget(parent)
{
    QString tmp;
    int row, col, colsp;
    int spac  = 1;
    int left  = 5;
    int top   = 5;
    int right = 5;
    int bott  = 5;
    int tree_rows = 20;
    int tree_cols = 3;

    main = (MainWindow *)parent;

    m_Auto_Roll = false;

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
    //limitLayout->setContentsMargins(left, top, right, bott);
    //limitLayout->setSpacing(spac);
    limitLayout->setContentsMargins(left, 0, right, 0);
    limitLayout->setSpacing(0);
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

    // add boxes, and adjust sizing poilicy
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

    row++;

    // ===================================
    // SHAPE FILE IO
    // ===================================
    QString statmsg("Shape directory - output/input");
    QGroupBox *dirGroup = new QGroupBox(tr("Shape File Directory"),this);
    mainLayout->addWidget(dirGroup);

    QHBoxLayout *layoutShape = new QHBoxLayout();
    layoutShape->setContentsMargins(10, 0, 10, 0);
    layoutShape->setSpacing(0);
    dirGroup->setLayout(layoutShape);

    // Shape File output/input directory
    //shapeLabel = new QLabel(tr("Shape Directory:"));
    shapeLabel = new QLabel(tr("Shape Dir.:"));
    shapeLabel->setStatusTip(statmsg);
    tmp = main->m_Settings->value(dirsshapePath,"").toString();
    shapeEdit = new QLineEdit(tmp,this);
    shapeEdit->setStatusTip(statmsg);
    connect(shapeEdit,SIGNAL(editingFinished()),this,SLOT(on_shape_edit()));
    shapeBrowse = new QPushButton("...",this);
    shapeBrowse->setStatusTip(statmsg);
    connect(shapeBrowse,SIGNAL(pressed()),this,SLOT(on_shape_browse()));

    layoutShape->addWidget(shapeLabel);
    layoutShape->addWidget(shapeEdit);
    layoutShape->addWidget(shapeBrowse);

    // ===================================
    // LAND USE
    // ===================================
    row = 0;
    col = 0;
    QGroupBox *landuseGroup = new QGroupBox(tr("Land use files"),this);
    mainLayout->addWidget(landuseGroup);

    QGridLayout *landuseLayout = new QGridLayout;
    landuseLayout->setContentsMargins(left, top, right, bott);
    landuseLayout->setSpacing(spac);
    landuseLayout->setRowMinimumHeight(0,150);
    landuseLayout->setColumnMinimumWidth(0,150);
    landuseGroup->setLayout(landuseLayout);

    // QTreeWidget - Shape File, Materials, Line Width
    //                  Materials - QComboBox
#ifdef USE_TREE_WIDGET
    landuseTree = new QTreeWidget(this);
    //landuseTree = new QTreeView(this);
    landuseTree->setAlternatingRowColors(true);
    landuseTree->setRootIsDecorated(true);
    landuseTree->setUniformRowHeights(true);
    //landuseTree->setSortingEnabled(true);
    landuseTree->setSelectionMode(QAbstractItemView::SingleSelection);
    landuseTree->setSelectionBehavior(QAbstractItemView::SelectRows);

    landuseTree->headerItem()->setText(C_FILE, "Shape File");
    landuseTree->headerItem()->setText(C_MAT, "Material");
    landuseTree->headerItem()->setText(C_WIDTH, "Line Width");
#else
    landuseTree = new QTableWidget(this);
    landuseTree->setColumnCount(3);
    QStringList header;
    header << "Shape File" << "Material" << "Line Width";
    landuseTree->setHorizontalHeaderLabels(header);
    landuseTree->verticalHeader()->hide();
    landuseTree->setAlternatingRowColors(true); // seems to DO NOTHING???
    landuseTree->setShowGrid(false);
#if QT_VERSION < 0x050000
    landuseTree->horizontalHeader()->setClickable(false);
#endif // QT_VERSION
    landuseTree->setSelectionMode(QAbstractItemView::SingleSelection);
    landuseTree->setSelectionBehavior(QAbstractItemView::SelectRows);
#endif

    // add the view
    landuseLayout->addWidget(landuseTree,row,col,tree_rows,tree_cols);
    while (col < tree_cols)
        col++;

    materialLabel = new QLabel("Materials",this);
    materialLabel->sizePolicy().setVerticalStretch(0);
    materialButton = new QPushButton("Add",this);
    materialButton->sizePolicy().setVerticalStretch(0);
    materialButton->setStatusTip("To 'assign' material to 'selected' shape file");

    landuseLayout->addWidget(materialLabel,row,col);
    landuseLayout->addWidget(materialButton,row,col+1);
    connect(materialButton,SIGNAL(clicked()),this,SLOT(on_add_clicked()));

    materialCombo = new QComboBox(this);
    materialCombo->sizePolicy().setVerticalStretch(0);
    row++;
    landuseLayout->addWidget(materialCombo,row,col,1,2);

    // fill the space below (if poss?)
    //QSpacerItem * sp = new QSpacerItem(60,60,QSizePolicy::Maximum);
    //row++;
    //landuseLayout->addItem(sp,row,col,6,1);

    // ===================================
    // OPTIONS
    // ===================================
    QGroupBox *optionGroup = new QGroupBox(tr("Options"),this);
    mainLayout->addWidget(optionGroup);

    QGridLayout *optionLayout = new QGridLayout;
    optionLayout->setContentsMargins(left, top, right, bott);
    optionLayout->setSpacing(spac);
    optionGroup->setLayout(optionLayout);

    inclmCheck = new QCheckBox("Include Landmass",this);
    noovrdCheck = new QCheckBox("No download overwrite",this);
    useshpCheck = new QCheckBox("Use shape-decode",this);
    ptwidLabel = new QLabel("Point width:",this);
    ptwidEdit = new QLineEdit(this);
    maxsegLabel = new QLabel("Max. segment:",this);
    maxsegEdit = new QLineEdit(this);
    ignerrsCheck = new QCheckBox("Ignore Errors.",this);
    noovrCheck = new QCheckBox("No overwrites.",this);
    skiperrCheck = new QCheckBox("Skip Errors.",this);
    skipcfmCheck = new QCheckBox("Skip Confirm.",this);

    connect(inclmCheck,SIGNAL(toggled(bool)),this,SLOT(on_inclm_toggled(bool)));
    inclmCheck->setChecked(main->m_Settings->value(lu_inclmPath,"1").toBool());

    row = 0;
    col = 0;
    colsp = 2;
    optionLayout->addWidget(inclmCheck,row,col,1,colsp);
    col += colsp;
    optionLayout->addWidget(noovrdCheck,row,col,1,colsp);
    col += colsp;
    optionLayout->addWidget(useshpCheck,row,col,1,colsp);
    col += colsp;

    row++;
    col = 0;
    optionLayout->addWidget(ptwidLabel,row,col);
    col++;
    optionLayout->addWidget(ptwidEdit,row,col);
    col++;
    optionLayout->addWidget(maxsegLabel,row,col);
    col++;
    optionLayout->addWidget(maxsegEdit,row,col);
    col++;
    colsp = 2;
    optionLayout->addWidget(ignerrsCheck,row,col,1,colsp);
    col += colsp;

    row++;
    col = 0;
    colsp = 2;
    optionLayout->addWidget(noovrCheck,row,col,1,colsp);
    col += colsp;
    optionLayout->addWidget(skiperrCheck,row,col,1,colsp);
    col += colsp;
    optionLayout->addWidget(skipcfmCheck,row,col,1,colsp);
    col += colsp;


    // ===================================
    // ACTIONS
    // ===================================
    QGroupBox *actionGroup = new QGroupBox(tr("Actions"),this);
    mainLayout->addWidget(actionGroup);

    QHBoxLayout *actionLayout = new QHBoxLayout;
    actionLayout->setContentsMargins(left, top, right, bott);
    actionLayout->setSpacing(spac);
    actionGroup->setLayout(actionLayout);

    actDownload = new QPushButton("Download",this);
    actUnzip    = new QPushButton("Unzip",this);
    actRefresh  = new QPushButton("Refresh",this);
    actProcess  = new QPushButton("Process",this);

    actionLayout->addWidget(actDownload);
    actionLayout->addWidget(actUnzip);
    actionLayout->addWidget(actRefresh);
    actionLayout->addWidget(actProcess);

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

void tabLanduse::finalize()
{

    // options - slot and init
    MCONSET_T(inclmCheck,on_inclm_toggled,lu_inclmPath,"1");   // = new QCheckBox("Include Landmass",this);
    MCONSET_T(noovrdCheck,on_noovrd_toggled,lu_noovrdPath,"1"); // = new QCheckBox("No download overwrite",this);
    MCONSET_T(useshpCheck,on_useshp_toggled,lu_useshpPath,"0"); // = new QCheckBox("Use shape-decode",this);
    MCONSET_E(ptwidEdit,on_ptwid_edit,lu_ptwidPath,"");   // = new QLabel("Point width:",this);   ptwidEdit = new QLineEdit(this);
    MCONSET_E(maxsegEdit,on_maxseg_edit,lu_maxsegPath,"");   // QLabel("Max. segment:",this);  maxsegEdit = new QLineEdit(this);
    MCONSET_T(ignerrsCheck,on_ignerrs_toggled,lu_ignerrsPath,"0"); // new QCheckBox("Ignore Errors.",this);
    MCONSET_T(noovrCheck,on_noovr_toggled,lu_noovrPath,"1"); // new QCheckBox("No overwrites.",this);
    MCONSET_T(skiperrCheck,on_skiperr_toggled,lu_skiperrPath,"0"); // new QCheckBox("Skip Errors.",this);
    MCONSET_T(skipcfmCheck,on_skipcfm_toggled,lu_skipcfmPath,"0"); // new QCheckBox("Skip Confirm.",this);

    // add slots for actions
    connect(actDownload,SIGNAL(clicked()),this,SLOT(on_download_clicked())); // = new QPushButton("Download",this);
    connect(actUnzip,SIGNAL(clicked()),this,SLOT(on_unzip_clicked())); // = new QPushButton("Unzip",this);
    connect(actRefresh,SIGNAL(clicked()),this,SLOT(updateShapeFiles())); //  = new QPushButton("Refresh",this);
    connect(actProcess,SIGNAL(clicked()),this,SLOT(on_process_clicked())); //  = new QPushButton("Process",this);

    // fill combo
    updateMaterials();
    on_shape_edit();

    // fill tree
    bool save = m_Auto_Roll;
    m_Auto_Roll = true;
    updateShapeFiles();
    m_Auto_Roll = save;

}

// populate material list with materials from FG's materials.xml
// or a default set if no 'materials.xml' file
void tabLanduse::updateMaterials()
{

    QStringList materialList;
    QString file(main->m_tabSetup->m_fgrootEdit->text());
    file.append("/materials.xml");
    QFile materialfile(file);
    if (materialfile.exists() == true) {
        if (materialfile.open(QIODevice::ReadOnly)) {
            QXmlStreamReader materialreader(&materialfile);
            QXmlStreamReader::TokenType tokenType;
            QString material;
            while ((tokenType = materialreader.readNext()) != QXmlStreamReader::EndDocument) {
                if (materialreader.name() == "material") {
                    while ((tokenType = materialreader.readNext()) != QXmlStreamReader::EndDocument) {
                        if (materialreader.name() == "name") {
                            material = materialreader.readElementText();
                            // ignore materials already present
                            if (materialList.indexOf(material, 0) == -1)
                                materialList.append(material);
                        }
                        // ignore sign materials
                        if (materialreader.name() == "glyph") {
                            materialreader.skipCurrentElement();
                        }
                    }
                }
            }
            materialfile.close();
        }
    }
    if (materialList.size() == 0)
        materialList = tg_getMaterialList();
    materialList.sort();
    materialCombo->clear();
    materialCombo->addItems(materialList);
}

void tabLanduse::on_shape_edit()
{
    QString tmp;
    QDir d;
    QString style("color:red");
    tmp = shapeEdit->text();
    //setup_state &= suf_scene;
    if (d.exists(tmp)) {
        style = "color:green";
        main->saveSetting(dirsshapePath,tmp);
        //setup_state |= suf_scene;
    }
    shapeLabel->setStyleSheet(style);
}

void tabLanduse::on_shape_browse()
{
    main->setStatusMessage("Set Shape in/out Directory",5000);
    QString prompt(tr("Select Shape Directory"));
    QString current = shapeEdit->text();
    QString filePath = util_browseDirectory(prompt,current);
    QDir d;
    if (!filePath.isEmpty() && d.exists(filePath)) {
        shapeEdit->setText(filePath);
        on_shape_edit();
    }

}

#define M_CONSET_T  M_CONSET_T_LU
#define M_CONSET_E  M_CONSET_E_LU

M_CONSET_T(inclmCheck,on_inclm_toggled,lu_inclmPath,"1");   // = new QCheckBox("Include Landmass",this);
M_CONSET_T(noovrdCheck,on_noovrd_toggled,lu_noovrdPath,"1"); // = new QCheckBox("No download overwrite",this);
M_CONSET_T(useshpCheck,on_useshp_toggled,lu_useshpPath,"0"); // = new QCheckBox("Use shape-decode",this);
M_CONSET_E(ptwidEdit,on_ptwid_edit,lu_ptwidPath,"");   // = new QLabel("Point width:",this);   ptwidEdit = new QLineEdit(this);
M_CONSET_E(maxsegEdit,on_maxseg_edit,lu_maxsegPath,"");   // QLabel("Max. segment:",this);  maxsegEdit = new QLineEdit(this);
M_CONSET_T(ignerrsCheck,on_ignerrs_toggled,lu_ignerrsPath,"0"); // new QCheckBox("Ignore Errors.",this);
M_CONSET_T(noovrCheck,on_noovr_toggled,lu_noovrPath,"1"); // new QCheckBox("No overwrites.",this);
M_CONSET_T(skiperrCheck,on_skiperr_toggled,lu_skiperrPath,"0"); // new QCheckBox("Skip Errors.",this);
M_CONSET_T(skipcfmCheck,on_skipcfm_toggled,lu_skipcfmPath,"0"); // new QCheckBox("Skip Confirm.",this);


bool tabLanduse::verify_decode_tool()
{
    bool res = false;
    QString toolDir(main->m_tabSetup->m_tgtoolEdit->text());
    // ***TODO*** check shape decode tool - should run the exe as a real test, not just exists...
    // check for tool existance
    QString tgTool = toolDir+"/ogr-decode";
#ifdef Q_OS_WIN
    tgTool += ".exe";
#endif
    QFile f;
    if ( ! f.exists(tgTool) ) {
        tgTool = toolDir+"/shape-decode";
#ifdef Q_OS_WIN
        tgTool += ".exe";
#endif
        if (f.exists(tgTool)) {
            useshpCheck->setCheckState(Qt::Checked);
        }
    }
    return res;
}

// update shapefiles list for ogr-decode/shape-decode
// void tabLanduse::updateShapeFiles(); //  = new QPushButton("Refresh",this);
void tabLanduse::updateShapeFiles()
{
    QString info;
    QString tmp;
    QFileInfo fInfo;
    QString file;
    int i, cnt;
    // move shapefiles to "private" directories
    QDir dir(m_dataDir); // search 'data' folder, for 'files'
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList fList = dir.entryInfoList();
    // search 'data' folder, for 'directories'
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList dList = dir.entryInfoList();
    QStringList dirs;
    info = "Searching in ["+m_dataDir+"] for shape files, directories.";

    cnt = 0;
    for (i = 0; i < fList.size(); ++i) {
        fInfo = fList.at(i);
        if (fInfo.suffix() == "shp") {
            file = fInfo.fileName();
            file.chop(4);
            if (dirs.indexOf(file) == -1) {
                dirs += file;
            }
            cnt++;
        }
    }
    tmp.sprintf("Found total %d files, %d dirs, %d shapefiles, %d types.",
                fList.size(),dList.size(),cnt,dirs.size());
    info += "\n"+tmp;
    if ((fList.size() == 0) && dList.size())
        info += "\nAppeara first part of listing is done.";

    outLog(info);
    info += "\n\nContinue to process these files?";
    if (!m_Auto_Roll) {
        if (!util_getYesNo("CONFIRM SHAPEFILE LISTING",info,this))
            return;
    }

    for (i = 0; i < fList.size(); ++i) {
        fInfo = fList.at(i);
        QString fPath = fInfo.absolutePath();
        QString fFilePath = fInfo.absoluteFilePath();
        QString fFileName1 = fInfo.fileName();
        QString fFileName2 = fInfo.fileName();

        // move only shapefiles
        if ((fInfo.suffix() == "dbf")||
            (fInfo.suffix() == "prj")||
            (fInfo.suffix() == "shp")||
            (fInfo.suffix() == "shx")) {
            fFileName1.chop(4);     // remove fileformat from name
            //QFile file (fFilePath);
            QString fPath_ren = fPath+"/"+fFileName1+"/"+fFileName2;
            dir.mkpath(fPath+"/"+fFileName1);
            dir.rename(fFilePath, fPath_ren);
        }
    }

    // clear the current list
    while (landuseTree->rowCount() != 0) {
        landuseTree->removeRow(0);
    }

    // update list
    // list of custom scenery shapefiles
    QStringList csShape;
    csShape = tg_getCustomShapes();
    // list of correpsonding materials
    QStringList csMater; // *TBD* - should compare this to default_priorities.txt
    csMater = tg_getMaterialList();
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList dirList = dir.entryInfoList(); // get updated directory list

    for (int i = 0; i < dirList.size(); ++i) {
        QFileInfo dirInfo = dirList.at(i);
        file = dirInfo.fileName();
        if (srtm_isElevationDir(file)) // should NOT be in here, but
            continue;
        // add this one
        landuseTree->insertRow(landuseTree->rowCount());
        QTableWidgetItem *twiCellShape = new QTableWidgetItem(0);
        twiCellShape->setText(tr(qPrintable(QString("%1").arg(dirInfo.fileName()))));
        twiCellShape->setFlags(Qt::ItemFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        landuseTree->setItem(landuseTree->rowCount()-1, 0, twiCellShape);
        QTableWidgetItem *twiCellMater = new QTableWidgetItem(0);

        // suggest a material
        QString suggestedMaterial;
        for (int j = 0; j < csShape.length(); ++j) {
            if (dirInfo.fileName() == csShape[j]){
                suggestedMaterial = csMater[j];
            }
        }
        twiCellMater->setText(suggestedMaterial);
        landuseTree->setItem(i, 1, twiCellMater);
    }
    landuseTree->resizeRowsToContents();
#if 0 // ***TBD*** check shape decode tool
    // check for tool existance
    QString tgTool = toolDir+"/ogr-decode";
#ifdef Q_OS_WIN
    tgTool += ".exe";
#endif
    QFile f(tgTool);
    if ( ! f.exists() ) {
        tgTool = toolDir+"/shape-decode";
#ifdef Q_OS_WIN
        tgTool += ".exe";
#endif
        if (f.exists(tgTool)) {
            ui->checkBox_useshp->setCheckState(Qt::Checked);
        }
    }
#endif
}

// add material ui->tblShapesAlign = landuseTree
//              ui->comboBox_2 = materialCombo
void tabLanduse::on_add_clicked()
{
    int cur_row = landuseTree->currentRow();
    int cur_ind = materialCombo->currentIndex();
    QString material = materialCombo->itemText(cur_ind);
    if (cur_row < 0) {
        QString msg("Must select a 'shape' file in the table\nwhere you want this material '");
        msg.append(material+"' assigned.");
        msg.append("\nIf there is already material assigned, it will\nbe replaced with this material");
        MBI("NO SELECTION",msg);
    } else {
        if (landuseTree->item(cur_row, 1) == 0) {

            QTableWidgetItem *twiMaterialCell = new QTableWidgetItem(0);
            twiMaterialCell->setText(material);
            landuseTree->setItem(cur_row, 1, twiMaterialCell);
        } else {
            landuseTree->item(cur_row, 1)->setText(material);
        }
    }
}

void tabLanduse::on_download_clicked() {
    // ***TBD*** = new QPushButton("Download",this);
}

void tabLanduse::on_unzip_clicked() {
    // ***TBD*** // = new QPushButton("Unzip",this);
}

void tabLanduse::on_process_clicked() {
    // **TBD** //  = new QPushButton("Process",this);
}

// eof - tablanduse.cpp
