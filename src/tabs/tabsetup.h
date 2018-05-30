/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#ifndef TABSETUP_H
#define TABSETUP_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QStatusBar>
#include <QPlainTextEdit>

#include "utilities/workThread.h"
#include "utilities/srtm_utils.h"

class MainWindow;

class tabSetup : public QWidget
{
    Q_OBJECT
public:
    explicit tabSetup(QWidget *parent = 0);

    MainWindow *main;
    QLabel *projdirLabel;
    QLineEdit * m_projdirEdit;
    QPushButton *projdirBrowse;

    QLabel *tgtoolLabel;
    QLineEdit *m_tgtoolEdit;
    QPushButton *tgtoolBrowse;

    QLabel *fgrootLabel;
    QLineEdit *m_fgrootEdit;
    QPushButton *fgrootBrowse;

    QLabel *hgtrootLabel;
    QLineEdit *m_hgtrootEdit;
    QPushButton *hgtrootBrowse;

    void saveSetting(QString path, QString value);

    QLabel *mmLabel;
    QLabel *minLabel; // "Minimum"
    QLabel *maxLabel; // "Maximum"

    QLabel *latLabel; // "Latitude:"
    QLineEdit *minlatEdit;
    QLineEdit *maxlatEdit;

    QLabel *lonLabel; // "Longitude:"
    QLineEdit *minlonEdit;
    QLineEdit *maxlonEdit;

    QLabel *centlat1Label;
    QLabel *centlat2Label;
    QLabel *centlon1Label;
    QLabel *centlon2Label;
    QLabel *xdist1Label;
    QLabel *xdist2Label;
    QLabel *ydist1Label;
    QLabel *ydist2Label;

    QLabel *srtm1Label;
    QLabel *srtm2Label;
    QLabel *srtm3Label;
    QLabel *srtm4Label;

    QPlainTextEdit *infoEdit;   // = new QPlainTextEdit(this);

    int setup_state;

    PWORKsu m_pwsu;

    bool isInitThreadDone();

    void setSRTMStg(PWORK pw);
    void getSRTMList();
    static void getSRTMStg(void * vp); // called by the THREAD

    void finalize();
    double m_minLat, m_maxLat, m_minLon, m_maxLon;

signals:

public slots:
    void on_projdir_edit();
    void on_projdir_browse();
    void on_tgtool_edit();
    void on_tgtool_browse();
    void on_fgroot_edit();
    void on_fgroot_browse();
    void on_hgtroot_edit();
    void on_hgtroot_browse();
    void on_range_change();
    void validate_dirs();


};

#endif // TABSETUP_H
