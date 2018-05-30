/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#include "app_config.h"
#include "tabadvanced.h"
#include "mainwindow.h"
#include "utilities/testExe.h"
#include "utilities/app_macros.h"

tabAdvanced::tabAdvanced(QWidget *parent) :
    QWidget(parent)
{
    QString tmp;
    main = (MainWindow *)parent;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 0, 10, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    QGroupBox *fileGroup = new QGroupBox(tr("Other Tools - green=ok, red=not valid"),this);
    mainLayout->addWidget(fileGroup);

    QGridLayout *fileLayout = new QGridLayout;
    fileLayout->setContentsMargins(10, 0, 10, 0);
    fileLayout->setSpacing(0);

    wgetLabel = new QLabel(tr("wget tool:"),this);
    tmp = main->m_Settings->value(toolwgetPath,"wget").toString();
    wgetEdit = new QLineEdit(tmp,this);
    wgetBrowse = new QPushButton("...",this);
    unzipLabel = new QLabel(tr("unzip tool:"),this);
    tmp = main->m_Settings->value(toolunzipPath,"unzip").toString();
    unzipEdit = new QLineEdit(tmp,this);
    unzipBrowse = new QPushButton("...",this);
    logLabel = new QLabel(tr("log file:"),this);
    // tmp = main->m_Settings->value(toollogPath,"").toString();
    tmp = util_getLogFile();
    logEdit = new QLineEdit(tmp,this);
    logBrowse = new QPushButton("...",this);

    int row = 0;
    fileLayout->addWidget(wgetLabel,row,1);
    fileLayout->addWidget(wgetEdit,row,2);
    fileLayout->addWidget(wgetBrowse,row,3);

    row++;
    fileLayout->addWidget(unzipLabel,row,1);
    fileLayout->addWidget(unzipEdit,row,2);
    fileLayout->addWidget(unzipBrowse,row,3);

    row++;
    fileLayout->addWidget(logLabel,row,1);
    fileLayout->addWidget(logEdit,row,2);
    fileLayout->addWidget(logBrowse,row,3);

    fileGroup->setLayout(fileLayout);

    // make connections
    connect(wgetEdit, SIGNAL(editingFinished()), this, SLOT(on_wget_edit()));
    connect(wgetBrowse,SIGNAL(pressed()), this, SLOT(on_wget_browse()));
    connect(unzipEdit, SIGNAL(editingFinished()), this, SLOT(on_unzip_edit()));
    connect(unzipBrowse,SIGNAL(pressed()), this, SLOT(on_unzip_browse()));
    connect(logEdit, SIGNAL(editingFinished()), this, SLOT(on_log_edit()));
    connect(logBrowse,SIGNAL(pressed()), this, SLOT(on_log_browse()));

    QGroupBox *optionsGroup = new QGroupBox(tr("Global Options"),this);
    mainLayout->addWidget(optionsGroup);

    QGridLayout *optLayout = new QGridLayout;
    optLayout->setContentsMargins(10, 0, 10, 0);
    optLayout->setSpacing(20);
    optionsGroup->setLayout(optLayout);

    delayLabel = new QLabel("Delay secs. (int)",this);
    delayEdit = new QLineEdit(this);
    delayInfo  = new QLabel("Minimum is one (1) second.",this);

    maxLabel = new QLabel("Max. Dialog text.",this);
    maxEdit = new QLineEdit(this);
    maxInfo  = new QLabel("Set zero (0) to disable.",this);

    confirmCheck = new QCheckBox("Confirm to proceed.",this);
    verifyCheck = new QCheckBox("Verify Tools on use.",this);

    defButton = new QPushButton("Reset Defaults",this);

    row = 0;
    optLayout->addWidget(delayLabel,row,0);
    optLayout->addWidget(delayEdit,row,1);
    optLayout->addWidget(delayInfo,row,2,1,2);
    row++;
    optLayout->addWidget(maxLabel,row,0);
    optLayout->addWidget(maxEdit,row,1);
    optLayout->addWidget(maxInfo,row,2,1,2);
    row++;
    optLayout->addWidget(confirmCheck,row,0,1,2);
    optLayout->addWidget(verifyCheck,row,2,1,2);
    row++;
    optLayout->addWidget(defButton,row,0);

    // ***TODO*** more/other GLOBAL options
    // anything else for this advanced page???
}

void tabAdvanced::finalize()
{
    MCONSET_E(delayEdit,on_delay_edit,ad_delayPath,"3");
    MCONSET_E(maxEdit,on_max_edit,ad_maxPath,"512");

    MCONSET_T(confirmCheck,on_confirm_toggle,ad_confirmPath,"1");   // = new QCheckBox("Confirm to proceed.",this);
    MCONSET_T(verifyCheck,on_verify_toggle,ad_verifyPath,"0");      // = new QCheckBox("Verify Tools on use.",this);

    connect(defButton,SIGNAL(clicked()),this,SLOT(on_def_clicked()));

    on_wget_edit();
    on_unzip_edit();
    on_log_edit();
    on_delay_edit();
    on_max_edit();
}

void tabAdvanced::on_def_clicked()
{
    delayEdit->setText("3");
    maxEdit->setText("512");
    confirmCheck->setChecked(true); //  (bool)Qt::Checked);
    verifyCheck->setChecked(false); //  (bool)Qt::Unchecked);
    on_delay_edit();
    on_max_edit();
    on_confirm_toggle(true);
    on_verify_toggle(false);

}

void tabAdvanced::on_wget_edit()
{
    QString tmp(wgetEdit->text());
    QString style("color:red");
    if (tmp.length()) {
        testExe x(tmp);
        x.args << "--version";
        if (x.runTest()) {
            style = "color:green";
            main->saveSetting(toolwgetPath,tmp);
        }
    }
    wgetLabel->setStyleSheet(style);
}

void tabAdvanced::on_wget_browse()
{
    main->setStatusMessage("Set wget Tool",5000);
    QString prompt(tr("Select wget Tool"));
    QString current = wgetEdit->text();
    QString filePath = util_browseFile(prompt,current);
    QFile f;
    if (!filePath.isEmpty() && f.exists(filePath)) {
        wgetEdit->setText(filePath);
        on_wget_edit();
    }
}

void tabAdvanced::on_unzip_edit()
{
    QString tmp(unzipEdit->text());
    QString style("color:red");
    if (tmp.length()) {
        testExe x(tmp);
        x.args << "-v";
        if (x.runTest()) {
            style = "color:green";
            main->saveSetting(toolunzipPath,tmp);
        }
    }
    unzipLabel->setStyleSheet(style);

}

void tabAdvanced::on_unzip_browse()
{
    main->setStatusMessage("Set Unzip Tool",5000);
    QString prompt(tr("Select Unzip Tool"));
    QString current = unzipEdit->text();
    QString filePath = util_browseFile(prompt,current);
    QFile f;
    if (!filePath.isEmpty() && f.exists(filePath)) {
        unzipEdit->setText(filePath);
        on_unzip_edit();
    }
}

void tabAdvanced::on_log_edit()
{
    QString tmp(logEdit->text());
    QString style("color:red");
    if (tmp.length()) {
        QString curr;
        curr = util_getLogFile();
        if (curr == tmp) {
            style = "color:green";
        } else {
            QFile f;
            QString msg;
            msg = "Confirm to CHANGE the LOG file from\n";
            msg.append(curr);
            msg.append("\nto\n");
            msg.append(tmp);
            if (f.exists(tmp)) {
                msg.append("\nNOTE: This NEW file exists, and will be over written");
            } else {
                msg.append("\nNOTE: This NEW file does not exist, and will be created");
            }
            if (util_getYesNo("CONFIRM CHANGE LOG FILE",msg,this)) {
                if (util_createLogFile(tmp) &&
                    f.exists(tmp)) {
                    style = "color:green";
                    main->saveSetting(toollogPath,tmp);
                }
            } else {
                // stay unchanged
                style = "color:green";
                logEdit->setText(curr);
            }
        }
    }
    logLabel->setStyleSheet(style);
}

void tabAdvanced::on_log_browse()
{
    main->setStatusMessage("Set LOG file Tool",5000);
    QString prompt(tr("Select new log file"));
    QString current = logEdit->text();
    QString filePath = util_browseNewFile(prompt,current);
    QFile f;
    if (!filePath.isEmpty()) {
        logEdit->setText(filePath);
        on_log_edit();
    }
}


//MCONSET_E(delayEdit,on_delay_edit,ad_delayPath,"3");
void tabAdvanced::on_delay_edit()
{
    QString tmp = delayEdit->text();
    int val = tmp.toInt();
    tmp.sprintf("%d",val);
    if (val < 1) {
        main->setStatusMessage("WARNING: Min. delay is '1' second.",10000);
        val = 1;
        tmp.sprintf("%d",val);
        delayEdit->setText(tmp);
    }
    main->saveSetting(ad_delayPath,tmp);
}

// MCONSET_E(maxEdit,on_max_edit,ad_maxpath,"512");
void tabAdvanced::on_max_edit()
{
    main->saveSetting(ad_maxPath,maxEdit->text());
}

#define M_CONSET_T M_CONSET_T_AD
M_CONSET_T(confirmCheck,on_confirm_toggle,ad_confirmPath,"1")   // = new QCheckBox("Confirm to proceed.",this);
M_CONSET_T(verifyCheck,on_verify_toggle,ad_verifyPath,"0")      // = new QCheckBox("Verify Tools on use.",this);

