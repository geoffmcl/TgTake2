#ifndef TABADVANCED_H
#define TABADVANCED_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QDir>
#include <QFile>
#include <QCheckBox>

class MainWindow;

class tabAdvanced : public QWidget
{
    Q_OBJECT
public:
    explicit tabAdvanced(QWidget *parent = 0);
    MainWindow * main;

    QLabel *wgetLabel;
    QLineEdit *wgetEdit;
    QPushButton *wgetBrowse;

    QLabel *unzipLabel;
    QLineEdit *unzipEdit;
    QPushButton *unzipBrowse;

    QLabel *logLabel;
    QLineEdit *logEdit;
    QPushButton *logBrowse;

    QLabel *delayLabel;     // = new QLabel("Delay secs. (int)",this);
    QLineEdit *delayEdit;   // = new QLineEdit(this);
    QLabel *delayInfo;      //  = new QLabel("Minimum is one (1) second.",this);

    QLabel *maxLabel;       // = new QLabel("Delay secs. (int)",this);
    QLineEdit *maxEdit;     // = new QLineEdit(this);
    QLabel *maxInfo;        // = new QLabel("Minimum is one (1) second.",this);

    QCheckBox *confirmCheck;    // = new QCheckBox("Confirm to proceed.",this);
    QCheckBox *verifyCheck;     // = new QCheckBox("Verify Tools on use.",this);

    QPushButton *defButton;     // = new QPushButton("Reset Defaults",this);

    void finalize();

signals:

public slots:
    void on_wget_edit();
    void on_wget_browse();
    void on_unzip_edit();
    void on_unzip_browse();
    void on_log_edit();
    void on_log_browse();
    void on_delay_edit();   // MCONSET_E(delayEdit,on_delay_edit,ad_delayPath,"3");
    void on_max_edit();     // MCONSET_E(maxEdit,on_max_edit,ad_maxPath,"512");

    void on_confirm_toggle(bool); // MCONSET_T(confirmCheck,on_confirm_toggle,ad_confirmPath,"1");   // = new QCheckBox("Confirm to proceed.",this);
    void on_verify_toggle(bool);  // MCONSET_T(verifyCheck,on_verify_toggle,ad_verifyPath,"0");      // = new QCheckBox("Verify Tools on use.",this);

    void on_def_clicked();  // QPushButton *defButton = new QPushButton("Reset Defaults",this);

};

#endif // TABADVANCED_H
