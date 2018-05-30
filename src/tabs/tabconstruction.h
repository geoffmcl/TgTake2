/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#ifndef TABCONSTRUCTION_H
#define TABCONSTRUCTION_H

#include <QWidget>
#include <QButtonGroup>
#include <QString>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSizePolicy>
#include <QFrame>
#include <QAbstractItemView>

class MainWindow;

// bit flags
#define flg_AirObj  1
#define flg_AirArea 2
#define flg_Default 4
#define flg_SRTM    8
#define flg_Land   16
#define flg_AirObj_cnt  0x80000000
#define flg_AirArea_cnt 0x40000000
#define flg_Default_cnt 0x20000000
#define flg_SRTM_cnt    0x10000000
#define flg_Land_cnt    0x08000000

#define flg_ALL (flg_AirObj | flg_AirArea | flg_Default | flg_SRTM | flg_Land)
#define flg_ALL_cnt (flg_AirObj_cnt | flg_AirArea_cnt | flg_Default_cnt | flg_SRTM_cnt | flg_Land_cnt)


class tabConstruction : public QWidget
{
    Q_OBJECT
public:
    explicit tabConstruction(QWidget *parent = 0);
    MainWindow * main;

    QButtonGroup *buttonGroup;  // = new QButtonGroup(this);
    int button_count;           // count of buttons added

    // choice 0 - min max
    QLabel *minlat1Label;
    QLabel *minlat2Label;
    QLabel *minlon1Label;
    QLabel *minlon2Label;
    QLabel *maxlat1Label;
    QLabel *maxlat2Label;
    QLabel *maxlon1Label;
    QLabel *maxlon2Label;
    // choice 1
    QLabel *oneLabel;   // = new QLabel("Tile ID",this);
    QLineEdit *oneEdit; // = new QLineEdit(this);
    QLabel *oneInfo;    // = new QLabel("Set to build a single bucket index only",this);

    // terrain list
    QListWidget *listWidget;
    // options
    QCheckBox *useukCheck;      // = new QCheckBox("Use UK grid.",this);
    QCheckBox *ignlmCheck;      // = new QCheckBox("Ignore Landmass.",this);
    QCheckBox *noovrCheck;      // = new QCheckBox("No overwrite existing.",this);
    QCheckBox *nosrcCheck;      // = new QCheckBox("Skip if no source data.",this);
    QCheckBox *ignerrsCheck;    // = new QCheckBox("Ignroe Errors.",this);
    QCheckBox *skipdnCheck;     // = new QCheckBox("Skip done confirmations.",this);
    QCheckBox *nopriorCheck;    // = new QCheckBox("No --priorities command.",this);

    QPushButton *updateButton;      // = new QPushButton("Udate Terrain List",this);
    QPushButton *generateButton;    // = new QPushButton("Generate Scenery",this);


    QPlainTextEdit *infoEdit;   // = new QPlainTextEdit(this);

    int process_Landuse(QString & selectedMaterials,
                                     QString & results,
                                     bool in_detail = false );

    void finalize();

signals:

public slots:

    // connect to handler, and restore previous checks
    void on_useuk_clicked(bool);    // = new QCheckBox("Use UK grid.",this);
    void on_ignlm_clicked(bool);    // = new QCheckBox("Ignore Landmass.",this);
    void on_noovr_clicked(bool);    // = new QCheckBox("No overwrite existing.",this);
    void on_nosrc_clicked(bool);    // = new QCheckBox("Skip if no source data.",this);
    void on_ignerrs_clicked(bool);  // = new QCheckBox("Ignore Errors.",this);
    void on_skipdn_clicked(bool);   // = new QCheckBox("Skip done confirmations.",this);
    void on_noprior_clicked(bool);  // = new QCheckBox("No --priorities command.",this);

    void on_button_clicked(int);    // select limit type buttons

    void on_update_clicked();
    void on_generate_clicked();

};

#endif // TABCONSTRUCTION_H
