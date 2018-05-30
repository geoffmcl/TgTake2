/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#ifndef TABAIRPORTS_H
#define TABAIRPORTS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPlainTextEdit>

class MainWindow;

class tabAirports : public QWidget
{
    Q_OBJECT
public:
    explicit tabAirports(QWidget *parent = 0);
    MainWindow * main;
    QButtonGroup *buttonGroup; //= new QButtonGroup(this);

    // choice 0
    QLabel *minlat1Label;
    QLabel *minlat2Label;
    QLabel *minlon1Label;
    QLabel *minlon2Label;
    QLabel *maxlat1Label;
    QLabel *maxlat2Label;
    QLabel *maxlon1Label;
    QLabel *maxlon2Label;

    QCheckBox *allaptsCheck;    // = new QCheckBox("Do _ALL_ airports",this);
    QLabel *allaptsLabel;        // = new QLabel("Need ALL elevations, and takes a LONG time!");

    QLabel *startLabel;     // = new QLabel("Start Airport ICAO",this);
    QLineEdit *startEdit;   // = new QLineEdit(this);
    QLabel *startInfo;      // = new QLabel("Set to start at this airport",this);

    // choice 1
    QLabel *oneLabel;   // = new QLabel("Airport ICAO",this);
    QLineEdit *oneEdit; // = new QLineEdit(this);
    QLabel *oneInfo;    // = new QLabel("Set to build one airport only",this);

    // choice 2
    QLabel *tileLabel;  // = new QLabel("Tile ID",this);
    QLineEdit *tileEdit;// = new QLineEdit(this);
    QLabel *orLabel;    // = new QLabel("   or  ",this);
    QLabel *chunkLabel; // = new QLabel("Chunk ID",this);
    QLineEdit *chunkEdit;// = new QLineEdit(this);

    QLabel *fileLabel;          // = new QLabel("Airport File:",this);
    QLineEdit *fileEdit;        // = new QLineEdit(this);
    QPushButton *fileBrowse;    // = new QPushButton("...",this);

    // options
    QLabel *maxslopeLabel;      // = new QLabel("Max. runway slope",this);
    QLineEdit *maxslopeEdit;    // = new QLineEdit(this);
    QLabel *nudgeLabel;         // = new QLabel("Nudge factor:",this);
    QLineEdit *nudgeEdit;       // = new QLineEdit(this);
    QCheckBox *limdemCheck;     // = new QCheckBox("Limit DEM paths.",this);

    // actions
    QPushButton *genaptsAction; // = new QPushButton("Generate Airport(s)",this);
    QPlainTextEdit *infoEdit;   // = new QPlainTextEdit(this);

    void finalize();
    void set_airport_info();


signals:

public slots:
    void on_button_click(int);
    void on_file_edit();
    void on_file_browse();
    void on_allapts_toggle(bool);
    void on_maxslope_edit();    // Path "max/slope"     //= new QLineEdit(this);
    void on_nudge_edit();       // Path "nudge/factor"  //= new QLineEdit(this);
    void on_limdem_toggle(bool);// Path "check/limdem"  //= new QCheckBox("Limit DEM paths.",this);
    void on_startEdit_finished();
    void on_oneEdit_finished();
    void on_tileEdit_finished();
    void on_chunkEdit_finished();
    void on_genapts_triggered();
};

#endif // TABAIRPORTS_H
