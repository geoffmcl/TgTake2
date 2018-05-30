/* ********************************************************
   *  testExe.h
   *  for TgTake2 project
   *
   *  Created by Geoff R. Mclane, Paris
   *  (C) Aug 2011 GPL2 (or later)
   *
   ******************************************************** */
#ifndef TESTEXE_H
#define TESTEXE_H

#include <QProcess>
#include <QTime>
#include <QString>
#include <QStringList>

// simple class to 'test' a runtime
class testExe
{
public:
    testExe();
    testExe(QString ex);
    ~testExe();
    void setExe(QString ex) { exe = ex; }
    void setArgs(QStringList ag) { args = ag; }
    void setFinds(QStringList fnd) { finds = fnd; }
    void setTimeout(int secs) { timeOut = secs; }
    QString exe;
    QStringList args;
    QStringList finds;
    QStringList extraEnv;
    QString rtDir;
    QString outStd;
    QString outErr;
    QString stgResult;
    int errExit;
    int timeOut;
    int runTime;
    int flagOpts;
    bool result;
    bool runTest();
private:
    void init();
};


#endif // TESTEXE_H
