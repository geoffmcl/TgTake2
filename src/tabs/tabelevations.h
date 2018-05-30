#ifndef TABELEVATIONS_H
#define TABELEVATIONS_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpacerItem>
#include <QVariant>
#include <QUrl>
#include <QDesktopServices>
#include <utilities/workThread.h>

class MainWindow;

class tabElevations : public QWidget
{
    Q_OBJECT
public:
    explicit tabElevations(QWidget *parent = 0);

    MainWindow *main;
    // worker thread
    workThread * m_urlThread;
    PWORK m_urlWork;
    static void openUrl(void *);
    void openBrowser(QString);
    void on_closeEvent();

    QLabel *minlat1Label;
    QLabel *minlat2Label;
    QLabel *minlon1Label;
    QLabel *minlon2Label;
    QLabel *maxlat1Label;
    QLabel *maxlat2Label;
    QLabel *maxlon1Label;
    QLabel *maxlon2Label;
    QLabel *srtmLabel;
    QLabel *minsrtmLabel;
    QLabel *maxsrtmLabel;
    QLabel * resLabel; // = new QLabel("Resolution:",this);
    QComboBox *resCombo; // = new QComboBox(this);
    QLabel *arcLabel; // = new QLabel("arc sec. Usually 3, 1 for USA or Special",this);

    QLabel *hgtLabel;       // = new QLabel(tr("HGT Directory:"));
    QLineEdit *hgtEdit;     // = new QLineEdit(tmp,this); HGT Dir
    QPushButton *hgtBrowse; // = new QPushButton("...",this); HGT Dir

    QPushButton *actDownload; // = new QPushButton("Download HGT",this);
    QPushButton *actUnzipHGT; // = new QPushButton("Unzip HGT",this);
    QPushButton *actDecode;   // = new QPushButton("Decode HGT",this);
    QPushButton *actTerrafit; // = new QPushButton("Run terrafit",this);

    // Left
    QLabel *maxnodes;       // = new QLabel("Max. nodes:",this);
    QLineEdit *maxEdit;     // = new QLineEdit(this);
    QLabel *minnodes;       // = new QLabel("Min. nodes:",this);
    QLineEdit *minEdit;     // = new QLineEdit(this);
    QLabel *maxerrs;        // = new QLabel("Max. errors:",this);
    QLineEdit *errsEdit;    // = new QLineEdit(this);
    QPushButton * resetButton; // = new QPushButton("Reset Defaults",this);

    // Right
    QCheckBox *skipzipCheck;// = new QCheckBox("Skip existing zips",this);
    QCheckBox *skiphgtCheck;// = new QCheckBox("Skip existing hgt",this);
    QCheckBox *skipcfmCheck;// = new QCheckBox("Skip confirmations",this);
    QCheckBox *conterrCheck;// = new QCheckBox("Continue on errors",this);

    QPlainTextEdit *infoEdit; // = new QPlainTextEdit();

    void finalize();
    void update_information();
    bool checkInitThreadDone();
    void append_information(QString str);

signals:

public slots:
    void on_hgt_edit();
    void on_hgt_browse();
    void on_res_combo(int);
    void on_download_clicked(); //connect(actDownload,SIGNAL(clicked()),this,SLOT(on_download_clicked())); // = new QPushButton("Download HGT",this);
    void on_unzip_clicked();    // connect(actUnzipHGT,SIGNAL(clicked()),this,SLOT(on_unzip_clicked()));   // = new QPushButton("Unzip HGT",this);
    void on_decode_clicked();   // connect(actDecode,SIGNAL(clicked()),this,SLOT(on_decode_clicked()));  //   = new QPushButton("Decode HGT",this);
    void on_terrafit_clicked(); // connect(actTerrafit,SIGNAL(clicked()),this,SLOT(on_terrafit_clicked())); // = new QPushButton("Run terrafit",this);
    void on_reset_clicked();    // connect(resetButton,SIGNAL(clicked()),this,SLOT(on_reset_clicked()));   // "Reset Defaults"

    void on_max_edit(); // M_CONSET_E(maxEdit,on_max_edit,maxnodesPath,"1000");
    void on_min_edit(); // M_CONSET_E(minEdit,on_min_edit,minnodesPath,"50");
    void on_err_edit(); // M_CONSET_E(errsEdit,on_err_edit,maxerrorPath,"40");
    void on_skipzip_toggle(bool); // MCONSET_T(skipzipCheck,on_skipzip_toggle,skipzipPath,"1");  // = new QCheckBox("Skip existing zips",this);
    void on_skiphgt_toggle(bool); // MCONSET_T(skiphgtCheck,on_skiphgt_toggle,skiphgtPath, "1"); // = new QCheckBox("Skip existing hgt",this);
    void on_skipcfm_toggle(bool); // MCONSET_T(skipcfmCheck,on_skipcfm_toggle,skipcfmPath, "1"); // = new QCheckBox("Skip confirmations",this);
    void on_conterr_toggle(bool); // MCONSET_T(conterrCheck,on_conterr_toggle,conterrPath, "0"); // = new QCheckBox("Continue on errors",this);
    void on_url_done(int,int);

};

#endif // TABELEVATIONS_H
