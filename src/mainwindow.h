/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif
#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QMenuBar>
#include <QFrame>
#include <QLabel>
#include <QTimer>
#include <QStatusBar>
#include <QTabWidget>
#include <QString>
#include <QWaitCondition>

#include "app_config.h"
#include "tabs/tabsetup.h"
#include "tabs/tabelevations.h"
#include "tabs/tablanduse.h"
#include "tabs/tabairports.h"
#include "tabs/tabconstruction.h"
#include "tabs/tabadvanced.h"

#include "utilities/utilities.h"
#include "utilities/workThread.h"
#include "utilities/srtm_utils.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    void saveSetting(QString path, QVariant value);
    void saveSettings();
    void processCommands();
#ifdef TRY_GLOBAL_SETTINGS
    QSettings * m_Settings;
#endif

    // TOP OF PAGE
    //= Menu Bar
    QMenuBar *m_menuBar;
    //= Menu Items
    QMenu *menuFile;
    //QMenu *menuTest;
    QMenu *menuHelp;
    //= Actions
    QAction *exitAct;
    QAction *aboutAct;

    QMenu *menuAction;
    QAction * actionAct;

    // CENTER OF PAGE - Contents of each 'tab'
    QTabWidget *tabWidget;
    //= Tab Pages
    tabSetup *m_tabSetup;
    tabElevations *m_tabElevations;
    tabLanduse *m_tabLanduse;
    tabAirports *m_tabAirports;
    tabConstruction *m_tabConstruction;
    tabAdvanced *m_tabAdvanced;

    // BOTTOM OF PAGE
    QStatusBar *m_statusBar;
    QLabel *labelTimer;
    QTimer *timer;
    void setStatusMessage(QString msg, int timeout = 0);

    bool do_quit;
    QString getCommandHelp();

    // worker thread
    workThread * m_workThread;
    WORK_LIST m_workList;   // list of actions passed to thread
    void appendWorkList(PWORK pw);
    void clearWorkList();
    int runWorker(ACTION func, void * vp);
    int work_count;
    int total_work;
    void set_abort_work(WorkType wt = WT_ALL);
    bool in_closeEvent;

    // some utility functions
    void updateDirectories();

public slots:
    void on_quit();
    void on_about();
    void on_about_qt();
    void on_timer();
    void on_message_changed(QString);
    void on_work_done(int,int);
    void on_tabChanged(int);
    void on_srtm_count();
};

// bunch of 'composite' directories
extern QString m_dataDir, m_outpDir, m_workDir, m_tempDir, m_srtmDir, m_shapDir, m_elevDir;

#endif // MAINWINDOW_H
