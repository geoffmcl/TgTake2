/* ********************************************************
   *  testExe.cpp
   *  for TgTake2 project
   *
   *  Created by Geoff R. Mclane, Paris
   *  (C) Aug 2011 GPL2 (or later)
   *
   ******************************************************** */
#include "testExe.h"

// =================================
// testing an exe
void testExe::init()
{
    timeOut = 3; // default to 3 seconds
    result = -3;
    // really, these have already been done
    exe.clear();
    args.clear();
    finds.clear();
    extraEnv.clear();
    rtDir.clear();
}

testExe::testExe()
{
    init();
}
testExe::testExe(QString ex)
{
    init();
    exe = ex; // set the EXE
}
testExe::~testExe()
{
    // nothing allocated, so nothing to do...
}
bool testExe::runTest()
{
    result = false;
    QTime rt;
    int i, max, cnt, ind;
    errExit = -2;
    runTime = 0;
    if (exe.length() == 0) {
        stgResult = "FAILED: No exe set!";
        return result;
    }
    QProcess proc;
    QString all, tmp;
    rt.start();
    if (rtDir.length()) {
        //= set the specific runtime directory
        proc.setWorkingDirectory(rtDir);
    }
    if (extraEnv.size()) {
        //= append new env vars
        QStringList env = QProcess::systemEnvironment();
        env << extraEnv;
        proc.setEnvironment(env);
    }
    QString arguments = exe;
    int ms = timeOut * 1000;
    if (ms <= 0)
        ms = 10; // minimum of 10 ms wait
    if (args.size())
        arguments += " "+args.join(" ");
    // try the running of the process
    proc.start(arguments, QIODevice::ReadWrite);
    if (proc.waitForStarted()) {
        proc.waitForFinished(ms);
        errExit = proc.exitCode();
        outStd = proc.readAllStandardOutput();
        outErr = proc.readAllStandardError();
        all = outStd + outErr;
        max = finds.size();
        if (max) {
            // must find these string in the result
            cnt = 0;
            for (i = 0; i < max; i++) {
                tmp = finds.at(i);
                ind = all.indexOf(tmp);
                if (ind >= 0)
                    cnt++;
            }
            if (cnt == max) { // found all required finds
                stgResult = "SUCCESS: All finds found";
                result = true;
            } else {
                stgResult.sprintf("FAILED: Found %d of %d in output",
                                  cnt, max);
            }
        } else {
            // no finds to find - base result on exit code
            if (errExit == 0) {
                stgResult = "SUCCESS: Exit code is ZERO";
                result = true;
            } else {
                stgResult.sprintf("FAILED: Exit code is %d",errExit);
            }
        }
    } else {
        stgResult = "FAILED: NO start ["+arguments+"]";
    }
    runTime = rt.elapsed();
    return result;
}

// eof - testExe.cpp

