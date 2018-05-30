/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#include "app_config.h"
#include "mainwindow.h"

// bunch of 'composite' directories
QString m_dataDir;
QString m_outpDir;
QString m_workDir;
QString m_tempDir;
QString m_srtmDir;
QString m_shapDir;
QString m_elevDir;

#define MAX_TAB_PAGES   6

typedef struct TAB_PAGES {
    int index;
    QString name;
}TAB_PAGES, *PTAB_PAGES;

TAB_PAGES tab_pages[MAX_TAB_PAGES] = {
    { 0, "Setup" },
    { 1, "Elevations" },
    { 2, "Landuse" },
    { 3, "Airports" },
    { 4, "Construction" },
    { 5, "Advanced" }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QString msg;
#ifdef TRY_GLOBAL_SETTINGS
    // http://doc.qt.io/qt-5/qsettings.html
    m_Settings = new QSettings(QSettings::IniFormat, QSettings::SystemScope, DEF_ORG_NAME, PGM_NAME, this);
    restoreGeometry(m_Settings->value("mainWindowGeometry").toByteArray());
#else
    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
#endif

    do_quit = false;
    in_closeEvent = false;
    msg = m_Settings->value(toollogPath,"").toString();
    if (msg.length())
        util_createLogFile(msg);
    else
        util_setStdLogFile();

    msg = "INI: ";
    msg += m_Settings->fileName();
    outLog(msg);
    //====================================================
    //** Main Central Widget and Layout
    //====================================================
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    //mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    widget->setLayout(mainLayout);

    //== File Menu
    menuFile = new QMenu(tr("&File"));
    //exitAct = menuFile->addAction(QIcon(":/icon/quit"), tr("&Quit"), this, SLOT(on_quit()));
    exitAct = menuFile->addAction(tr("&Quit"), this, SLOT(on_quit()));
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Quit the application"));
    menuFile->addAction(exitAct);
    //exitAct->setIconVisibleInMenu(true);

    //menuAction = new QMenu(tr("&Actions"));
    //actionAct = menuAction->addAction(tr("&SRTM Count"), this, SLOT(on_srtm_count()));
    //actionAct->setStatusTip(tr("Begin SRTM count"));

    menuHelp = new QMenu(tr("&Help"));
    //exitAct = menuFile->addAction(QIcon(":/icon/quit"), tr("&Quit"), this, SLOT(on_quit()));
    aboutAct = menuHelp->addAction(tr("&About"), this, SLOT(on_about()));
    aboutAct->setStatusTip(tr("Show About dialog"));
    //exitAct->setIconVisibleInMenu(true);
    menuHelp->addAction(tr("About Qt"), this, SLOT(on_about_qt()));

    m_menuBar = new QMenuBar(this);
    m_menuBar->addMenu(menuFile);
    //m_menuBar->addMenu(menuAction);
    m_menuBar->addMenu(menuHelp);

    mainLayout->addWidget(m_menuBar);

    // CENTER - Tab Widget
    tabWidget = new QTabWidget(this);

    // add the pages
    m_tabSetup = new tabSetup(this);
    tabWidget->addTab(m_tabSetup,"Setup");

    m_tabElevations = new tabElevations(this);
    tabWidget->addTab(m_tabElevations,"Elevations");

    m_tabLanduse = new tabLanduse(this);
    tabWidget->addTab(m_tabLanduse,"Landuse");

    m_tabAirports = new tabAirports(this);
    tabWidget->addTab(m_tabAirports,"Airports");

    m_tabConstruction = new tabConstruction(this);
    tabWidget->addTab(m_tabConstruction,"Constructions");

    m_tabAdvanced = new tabAdvanced(this);
    tabWidget->addTab(m_tabAdvanced,"Advanced");

    // add the tab widget to main
    mainLayout->addWidget(tabWidget);
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tabChanged(int)));

    // BOTTOM - Status Bar
    m_statusBar = new QStatusBar(this);
    connect(m_statusBar, SIGNAL(messageChanged(QString)),
            this, SLOT(on_message_changed(QString)));
    m_statusBar->showMessage("Ready");
    msg = util_getTimestg();
    labelTimer = new QLabel(msg);
    //label->setFixedWidth(50);
    labelTimer->setFrameStyle(QFrame::Panel | QFrame::Raised);
    //label->setMargin(1);
    m_statusBar->addPermanentWidget(labelTimer);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer()));
    timer->start(1000);

    setStatusBar(m_statusBar);
    //mainLayout->addWidget(m_statusBar);

    // now see if any command line over-rulling anything
    processCommands();

    if (do_quit)
        return;

    work_count = 0;
    total_work = 0;
    m_workThread = new workThread;
    connect(m_workThread, SIGNAL(work_done(int,int)), this, SLOT(on_work_done(int,int)));

    updateDirectories();

    // finalise the pages
    m_tabSetup->finalize(); // now it the time to verify
    m_tabAdvanced->finalize();
    m_tabElevations->finalize();
    m_tabLanduse->finalize();
    m_tabAirports->finalize();
    m_tabConstruction->finalize();

    // restore the tab index
    int index = m_Settings->value(tabindexPath).toInt();
    tabWidget->setCurrentIndex(index);
    //msg.sprintf("Set tab index to %d",index);
    //outLog(msg);
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    in_closeEvent = true;
    set_abort_work(WT_ALL);   // ask all and any threads to EXIT
    QString msg;
    QTime tm;
    int msecs = 0;
    outLog("closeEvent: entered",3);

    // propogate close for others
    m_tabElevations->on_closeEvent();

    if (m_workThread) {
        // If it has been run, once or more, and
        // is in the USER function - no idea about for HOW LONG, so
        if (m_workThread->in_function) {
            tm.start();
            while (msecs < 5000) {
                msecs += 50;
                abortSRTMCount = true;
                SleeperThread::msleep(50);
                //QWaitCondition sleep;
                //sleep.wait(2000);   // two seconds
                if (!m_workThread->in_function)
                    break;
            }
            if (m_workThread->in_function) {
                msg = "closeEvent: still in function after "+getElapTimeStg(tm.elapsed());
                msg.append(" - using 'terminate()'");
                outLog(msg,3);
                m_workThread->was_terminated = true;
                m_workThread->terminate();
            } else {
                msg.sprintf("closeEvent: cleared function in %d msecs. ",msecs);
                msg.append("("+getElapTimeStg(tm.elapsed())+")");
                outLog(msg,3);
            }
        }
        delete m_workThread;
        outLog("closeEvent: deleted workthread",3);
    }
    m_workThread = 0;
    clearWorkList();
    saveSettings();
    outLog(util_getDateTimestg()+" - Application closeEvent()",0x8001);
    event->accept();
}

void MainWindow::saveSetting(QString path, QVariant value)
{
#ifdef TRY_GLOBAL_SETTINGS
    m_Settings->setValue(path,value);
#else
    QSettings settings;
    settings.setValue(path,value);
#endif
}


void MainWindow::saveSettings()
{
#ifdef TRY_GLOBAL_SETTINGS
    m_Settings->setValue("mainWindowGeometry", saveGeometry());
    m_Settings->setValue("mainWindowState", saveState());
#else
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
#endif
}

QString MainWindow::getCommandHelp()
{
    QString msg;
    msg = "Command Help\n";
    msg.append(" --fg-scenery <path>\n");
    msg.append(" --tg-tools <path>\n");
    msg.append(" --fg-root <path>\n");
    msg.append(" --minlat <degs>\n");
    msg.append(" --maxlat <degs>\n");
    msg.append(" --minlon <degs>\n");
    msg.append(" --maxlon <degs>\n");
    return msg;
}


void MainWindow::processCommands()
{
    int i, i2, max;
    QString msg, arg, num;
    QStringList cmd_args = QCoreApplication::arguments();
    max = cmd_args.count();
    bool need_update = false;
    bool need_validate = false;
    for (i = 0; i < max; i++) {
        i2 = i + 1;
        num.sprintf("CMD %2d: ", i2);
        arg = cmd_args.at(i);
        num.append(arg);
        if (i) {
            if ((arg == "--help")||(arg == "-?")||(arg == "-h")) {
                msg = getCommandHelp();
                msg.append("\nAborting on OK");
                if (!do_quit)
                    QMessageBox::information(this,"HELP COMMAND",msg);
                do_quit = true;
            } else if (i2 < max) {
                if (arg == "--fg-scenery") {
                    i++;
                    arg = cmd_args.at(i);
                    num.append(" "+arg);
                    m_tabSetup->m_projdirEdit->setText(arg);
                    need_validate = true;
                } else if (arg == "--tg-tools") {
                    i++;
                    arg = cmd_args.at(i);
                    num.append(" "+arg);
                    m_tabSetup->m_tgtoolEdit->setText(arg);
                    need_validate = true;
                } else if (arg == "--fg-root") {
                    i++;
                    arg = cmd_args.at(i);
                    num.append(" "+arg);
                    m_tabSetup->m_fgrootEdit->setText(arg);
                    need_validate = true;
                } else if (arg == "--minlat") {
                    i++;
                    arg = cmd_args.at(i);
                    num.append(" "+arg);
                    m_tabSetup->minlatEdit->setText(arg);
                    need_update = true;
                } else if (arg == "--maxlat") {
                    i++;
                    arg = cmd_args.at(i);
                    num.append(" "+arg);
                    m_tabSetup->maxlatEdit->setText(arg);
                    need_update = true;
                } else if (arg == "--minlon") {
                    i++;
                    arg = cmd_args.at(i);
                    num.append(" "+arg);
                    m_tabSetup->minlonEdit->setText(arg);
                    need_update = true;
                } else if (arg == "--maxlon") {
                    i++;
                    arg = cmd_args.at(i);
                    num.append(" "+arg);
                    m_tabSetup->maxlonEdit->setText(arg);
                    need_update = true;
                } else {
                    msg = "Unknown Command ["+arg+"]!\n";
                    msg.append(getCommandHelp());
                    msg.append("\nAborting on OK");
                    if (!do_quit)
                        QMessageBox::critical(this,"BAD COMMAND ARGUMENT",msg);
                    do_quit = true;
                }
            } else {
                msg = "Command ["+arg+"] requires a following argument!\n";
                msg.append(getCommandHelp());
                msg.append("\nAborting on OK");
                if (!do_quit)
                    QMessageBox::critical(this,"BAD COMMAND ARGUMENT",msg);
                do_quit = true;
            }
        }
        outLog(num);
        if (do_quit)
            break;
    }
#if 0   // this is now done in finalize() the tab page
    if (!do_quit) {
        if (need_validate)
            m_tabSetup->validate_dirs();
        if (need_update)
            m_tabSetup->on_range_change(); // fix the ranges
    }
#endif // 0 chopped code

}

void MainWindow::on_quit()
{
    close();
}

void MainWindow::on_about()
{
    QString msg;
    msg.append("<html><body><p>");
    msg.append(PGM_NAME);
    msg.append(" Version: ");
    msg.append(PGM_VERSION);
    msg.append(", dated ");
    msg.append(PGM_DATE);
    msg.append("<br>");
    msg.append("Built: ");
    msg.append(__DATE__);
    msg.append(" at ");
    msg.append(__TIME__);
    msg.append("</p>");
    msg.append("<p>Purpose: A TerraGear Scenery Building Tool, using <a href=\"http://qt.nokia.com/\">Qt</a><br>");
    msg.append("After setting essential paths, and establishing scenery limits<br>");
    msg.append("will download, and chop elevation, landuse and airport raw data<br>");
    msg.append("into arrays to be used by the final scenery construction tool.</p>");
    msg.append("<p><b>Source: <a href=\"http://geoffair.org/fg/tg-05.htm\">TgTake2</a></b></p>");
    msg.append("<p>License: GNU GPL v2 or later - see LICENSE.txt<br>");
    msg.append("Copyright: &copy; Geoff R. McLane</p>");
    msg.append("<p>With a big thanks to Gijs de Rooy,<br>for his very inspirational <a href=\"http://gitorious.org/terragear-addons\">TerraGear GUI</a><br>");
    msg.append("that got me started on this GUI ;=))");
    msg.append("</body></html>");
    QMessageBox::about(this, tr("About TgTake2"), msg);
}

void MainWindow::on_about_qt()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_timer()
{
    QString msg = util_getTimestg();
    if (do_quit)
        on_quit();
    if (work_count)
        msg.append("*");
    labelTimer->setText(msg);
}

void MainWindow::setStatusMessage(QString msg, int timeout)
{
    m_statusBar->showMessage(msg,timeout);
    outLog("STATUS: "+msg);
}

void MainWindow::on_message_changed(QString msg)
{
    if (msg.isEmpty())
        m_statusBar->showMessage("Ready");

}

void MainWindow::on_tabChanged(int index)
{
    m_Settings->setValue(tabindexPath,index);
    switch (index) {
    case 0:
        // m_tabSetup
        break;
    case 1:
        // m_tabElevations
        break;
    case 2:
        // m_tabLanduse
        break;
    case 3:
        // m_tabAirports
        if (m_tabAirports)
            m_tabAirports->set_airport_info();
        break;
    case 4:
        // m_tabConstruction
        break;
    case 5:
        // m_tabAdvanced
        break;
    }

    QString msg;
    msg.sprintf("Current tab index %d ",index);
    if ((index >= 0) && (index < MAX_TAB_PAGES)) {
        msg += tab_pages[index].name;
    }
    outLog(msg);
}

void MainWindow::appendWorkList(PWORK pw)
{
    m_workList.append(pw);
    work_count++;
    total_work++;
}

void MainWindow::clearWorkList()
{
    while (!m_workList.isEmpty())
        delete m_workList.takeFirst();
}

//pwsu->work.test_num = main->runWorker(getSRTMStg,pwsu);
int MainWindow::runWorker(ACTION func, void * vp)
{
    return m_workThread->work(func,vp);
}

void MainWindow::set_abort_work(WorkType wt)
{
    PWORK pw;
    int i;
    int max = m_workList.count();
    for (i = 0; i < max; i++) {
        pw = m_workList.at(i);
        if ((wt == WT_ALL)||(pw->work_type == wt))
            pw->abort_work = true;
    }
}


void MainWindow::on_work_done(int id, int ms)
{
    if (in_closeEvent)
        return; // nothing to do here
    QString msg, tmp;
    PWORK pw;
    int i;
    int done_cnt = 0;
    int ind = -1;
    int max = m_workList.count();
    for (i = 0; i < max; i++) {
        pw = m_workList.at(i);
        if (pw->work_done)
            done_cnt++;
        if (pw->work_num == id)
            ind = i;
    }
    done_cnt++; // count one more as DONE
    if (work_count)
        work_count--;
    if (ind != -1) {
        pw = m_workList.at(ind);
        pw->work_done = true;
        msg = "Work: '"+pw->work_desc+"'' in "+getElapTimeStg(ms);
        tmp.sprintf(", done %d of %d. ",done_cnt,max);
        msg.append(tmp);
        if (done_cnt == max)
            msg.append("LAST");
        // different actions depending on WHAT was run
        if (pw->work_type == WT_SRTMCNT) {
            m_tabSetup->setSRTMStg(pw);
        }
    } else {
        if (in_closeEvent)
            msg = "In close event - exiting...";
        else
            msg.sprintf("INTERNAL PROBLEM: No 'work' of num %d in QUEUE of %d!", id, max);
    }
    setStatusMessage(msg,10000);
}

void MainWindow::on_srtm_count()
{
    m_tabSetup->srtm4Label->setText("ok, est. SRTM count for range...");
    m_tabSetup->getSRTMList();
}

// some utility functions
void MainWindow::updateDirectories()
{
    QString projectDir(m_tabSetup->m_projdirEdit->text());
    // set composite directories
    if (projectDir.size()) {
        m_dataDir = projectDir+"/data"; // source (raw) data
        m_outpDir = projectDir+"/scenery"; // final construction
        m_workDir = projectDir+"/work"; // source (chopped) data
        m_tempDir = projectDir+"/temp"; // temporary work directory - for downloads
        m_elevDir = m_workDir+"/SRTM-30"; // HGT decode destination

        if (m_srtmDir.size() == 0) {
            m_srtmDir = m_tempDir+"/hgt"; // temp for HGT work files
            m_Settings->setValue(dirshgtPath,m_srtmDir);
            m_tabElevations->hgtEdit->setText(m_srtmDir);
        }
        if (m_shapDir.size() == 0) {
            m_shapDir = m_tempDir+"/shape"; // temp for SHAPE work files
            m_Settings->setValue(dirsshapePath,m_shapDir);
            m_tabLanduse->shapeEdit->setText(m_shapDir);
        }
    }
}

// eof - mainwindow.cpp

