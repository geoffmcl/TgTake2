/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#ifndef TABLANDUSE_H
#define TABLANDUSE_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QAbstractItemView>
#include <QComboBox>
#include <QPushButton>
#include <QIODevice>
#include <QString>
#include <QXmlStreamReader>
#include <QCheckBox>
//#include <QTreeWidget>
//#include <QTreeView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPlainTextEdit>

class MainWindow;

enum LU_COLS {
    C_FILE = 0,
    C_MAT = 1,
    C_WIDTH = 2
};

class tabLanduse : public QWidget
{
    Q_OBJECT
public:
    explicit tabLanduse(QWidget *parent = 0);
    MainWindow * main;
    QLabel *minlat1Label;
    QLabel *minlat2Label;
    QLabel *minlon1Label;
    QLabel *minlon2Label;
    QLabel *maxlat1Label;
    QLabel *maxlat2Label;
    QLabel *maxlon1Label;
    QLabel *maxlon2Label;

    //QTreeWidget *landuseTree;
    QTableWidget *landuseTree;  // = new QTableWidget(this);
    //QTreeView *landuseTree;  // = new QTreeView(this);

    QLabel *materialLabel;  //= new QLabel("Materials",this);
    QComboBox *materialCombo; //= new QComboBox(this);

    QLabel *shapeLabel;       // = new QLabel(tr("Shape Directory:"));
    QLineEdit *shapeEdit;     // = new QLineEdit(tmp,this);
    QPushButton *shapeBrowse; // = new QPushButton("...",this);

    QCheckBox *inclmCheck;      // = new QCheckBox("Include Landmass",this);
    QCheckBox *noovrdCheck;     // = new QCheckBox("No download overwrite",this);
    QCheckBox *useshpCheck;     // = new QCheckBox("Use shape-decode",this);
    QLabel *ptwidLabel;         // = new QLabel("Point width:",this);
    QLineEdit *ptwidEdit;       // = new QLineEdit(this);
    QLabel *maxsegLabel;        // = new QLabel("Max. segment:",this);
    QLineEdit *maxsegEdit;      // = new QLineEdit(this);
    QCheckBox *ignerrsCheck;    // = new QCheckBox("Ignore Errors.",this);
    QCheckBox *noovrCheck;      // = new QCheckBox("No overwrites.",this);
    QCheckBox *skiperrCheck;    // = new QCheckBox("Skip Errors.",this);
    QCheckBox *skipcfmCheck;    // = new QCheckBox("Skip Confirm.",this);

    QPushButton *actDownload; // = new QPushButton("Download HGT",this);
    QPushButton *actUnzip;    // = new QPushButton("Unzip",this);
    QPushButton *actRefresh;  // = new QPushButton("Refresh",this);
    QPushButton *actProcess;  // = new QPushButton("Process",this);

    QPushButton * materialButton;   // = new QPushButton("Add",this);

    void finalize();
    void updateMaterials();
    bool m_Auto_Roll;
    QPlainTextEdit *infoEdit; // = new QPlainTextEdit(this);
    bool verify_decode_tool();

signals:

public slots:
    void on_shape_edit();
    void on_shape_browse();
    void on_inclm_toggled(bool); // = new QCheckBox("Include Landmass",this);
    void on_noovrd_toggled(bool); // = new QCheckBox("No download overwrite",this);
    void on_useshp_toggled(bool); // = new QCheckBox("Use shape-decode",this);
    void on_ptwid_edit();        // = new QLabel("Point width:",this);   ptwidEdit = new QLineEdit(this);
    void on_maxseg_edit();      // QLabel("Max. segment:",this);  maxsegEdit = new QLineEdit(this);
    void on_ignerrs_toggled(bool); // new QCheckBox("Ignore Errors.",this);
    void on_noovr_toggled(bool);    // new QCheckBox("No overwrites.",this);
    void on_skiperr_toggled(bool);  // new QCheckBox("Skip Errors.",this);
    void on_skipcfm_toggled(bool); // new QCheckBox("Skip Confirm.",this);
    void on_add_clicked();  // new QPushButton("add",this);

    void on_download_clicked(); // = new QPushButton("Download",this);
    void on_unzip_clicked(); // = new QPushButton("Unzip",this);
    void updateShapeFiles(); //  = new QPushButton("Refresh",this);
    void on_process_clicked(); //  = new QPushButton("Process",this);

};

#endif // TABLANDUSE_H
