/* ==================================================
   Threading project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
/* ********************************************************
   *  workThread.cpp[.h]
   *
   *  Created by Geoff R. Mclane, Paris
   *  (c) Aug 2011 GPL2 (or later)
   *
   *  Create a simple 'worker' thread, using QThread
   *  Instantiation: Either through declaration
   *  workThread thread, or allocated
   *  workThread * worker = new workThread, and remember
   *  to do 'delete worker' at end of the application.
   *
   *  API: Call job_number = worker->work( function ), with the function
   *       to be run. Function must be void foo() type.
   *       2011-08-29: Or the function can be void foo(void *) type
   *  This can be called multiple times, and the 'jobs' will be stacked,
   *  and processed one after the other.
   *
   *  SIGNAL: workThread emits a work_done(int,int) when completed, passing the
   *               job number, and ms of running, so use
   *           connect(worker, SIGNAL(work_done(int,int)),
   *                   this, SLOT(done_it(int,int)));
   *          to connect this to a SLOT job_done(int job_number, int ms)
   *
   *  Presently, if processing a LONG job at application exit, the full exit of
   *     the application will only happen when the job is done.
   *  A future enhancement would be to provide a 'cancel' mechanism, to enable
   *     killing the thread even if 'working'...
   *
   ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ** "AS IS" WITH NO EXPRESS OR IMPLIED WARRANTIES OF ANY TYPE.
   *
   ******************************************************** */
#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTime>
#include <QList>
#include "utilities.h"

#define ADD_VOID_PTR

#ifdef ADD_VOID_PTR
typedef void (*ACTION)(void *);
typedef struct tagWORKITEM {
    ACTION act;
    void * vp;
}WORKITEM, * PWORKITEM;
typedef QList<WORKITEM> THREAD_LIST;

#else
typedef void (*ACTION)();
#endif

class workThread : public QThread
{
    Q_OBJECT
public:
    workThread(QObject *parent = 0);
    ~workThread();
    bool in_function;
    bool was_terminated;
#ifdef ADD_VOID_PTR
    int work(ACTION func, void *vp);
#else
    int work(ACTION func);
#endif
signals:
    void work_done(int jn, int ms);

protected:
    void run();

private:
    QMutex mutex;
    QWaitCondition condition;
    int restart;
    bool abort;
#ifdef ADD_VOID_PTR
    THREAD_LIST jobs;
#else
    QList<ACTION> jobs;
#endif
    int jobs_in;
    int jobs_out;
    int cyc_count;
    int cum_running;
    QTime wt;
};

class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

enum WorkType {
    WT_SETUPINFO,
    WT_SRTMCNT,
    WT_AIRINFO,
    WT_LANDINFO,
    WT_CONSTINFO,
    WT_ADVANINFO,
    WT_URL,
    WT_MAXVALUE,
    WT_ALL = -1
};

// structure for the application
// to keep track of 'work' passed to thread
typedef struct tagWORK {
    WorkType work_type;  // will determine actions when DONE
    int work_num;
    QTime work_tt;
    QString work_desc;
    bool work_done;
    bool abort_work;
}WORK, *PWORK;

typedef QList<PWORK> WORK_LIST;

#endif // WORKTHREAD_H
// eof - workthread.h
