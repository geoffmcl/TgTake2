/* ==================================================
   Threading project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */

/* ********************************************************
   *               job number, and ms of running, so use
   *           connect(worker, SIGNAL(work_done(int,int)),
   *                   this, SLOT(done_it(int,int)));
   *          to connect this to a SLOT job_done(int job_number, int ms)
   *
   *  Presently, if processing a LONG job at application exit, the full exit of
   *     the application will only happen when the job is done.
   *  A future enhancement would be to provide a 'cancel' mechanism, to enable
   *     killing the thread even is 'working'...
   *
   ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ** "AS IS" WITH NO EXPRESS OR IMPLIED WARRANTIES OF ANY TYPE.
   *
   ******************************************************** */

#include "workThread.h"

workThread::workThread(QObject *parent)
    : QThread(parent)
{
    in_function = false;
    was_terminated = false;
    restart = 0;
    abort = false;
    cyc_count = 0;
    jobs_in = 0;
    jobs_out = 0;
    cum_running = 0;
    jobs.clear();
    outLog("workThread: instantiated");
}

workThread::~workThread()
{
    mutex.lock();
    abort = true; // set abort
    condition.wakeOne(); // wake it up
    mutex.unlock();
    QString msg;
    msg.sprintf("workThread: destroyed after %d cycles, ", cyc_count);
    msg.append("cumulative "+getElapTimeStg(cum_running));
    outLog(msg);
    if (!was_terminated)
        wait(); // wait for thread exit
}

#ifdef ADD_VOID_PTR
int workThread::work(ACTION func, void * vp)
{
    QString msg;
    int cnt;
    WORKITEM wi;
    // queue the jobs
    mutex.lock();
    jobs_in++;
    wi.act = func;
    wi.vp = vp;
    jobs.append(wi);
    cnt = jobs.size();
    mutex.unlock();

    msg.sprintf("workThread: Queued job %d, as %d - ", jobs_in, cnt);
    if ( isRunning() ) {
        msg.append("Restarting work thread");
        restart++;
        condition.wakeOne();
    } else {
        msg.append("Starting work thread");
        start(LowPriority);
    }
    outLog(msg);
    return jobs_in; // return this JOB number
}
#else
int workThread::work(ACTION func)
{
    QString msg;
    int cnt;

    // queue the jobs
    mutex.lock();
    jobs_in++;
    jobs.append(func);
    cnt = jobs.size();
    mutex.unlock();

    msg.sprintf("workThread: Queued job %d, as %d - ", jobs_in, cnt);
    if ( isRunning() ) {
        msg.append("Restarting work thread");
        restart++;
        condition.wakeOne();
    } else {
        msg.append("Starting work thread");
        start(LowPriority);
    }
    outLog(msg);
    return jobs_in; // return this JOB number
}
#endif

void workThread::run()
{
    int ms;
    ACTION func;
    QString msg;
    WORKITEM wi;
    wi.act = 0;
    wi.vp = 0;
    void * vp = 0;
    forever {
        // time to exit thread
        if ( abort ) {
            outLog("workThread: Thread got abort 1");
            return; // end of show
        }
        cyc_count++; // cycle count

        mutex.lock();
        // get params
        if ( jobs.isEmpty() ) {
            func = 0;
        } else {
#ifdef ADD_VOID_PTR
            wi = jobs.takeFirst();
            func = wi.act;
            vp = wi.vp;
#else
            func = jobs.takeFirst();
#endif
        }
        mutex.unlock();

        // do the work
        if (func) {
            jobs_out++;
            msg.sprintf("workThread: Doing job %d",jobs_out);
            outLog(msg);
            wt.start();
            in_function = true;
#ifdef ADD_VOID_PTR
            func(vp); // perform the action
#else
            func(); // perform the action
#endif
            in_function = false;
            ms = wt.elapsed();
            cum_running += ms;
            msg.sprintf("workThread: Done job %d, in ",jobs_out);
            msg.append(getElapTimeStg(ms));
            outLog(msg);
            if (!abort)
                emit work_done(jobs_out,ms); // inform
        }

        // time to exit thread
        if ( abort ) {
            outLog("workThread: Thread got abort 2");
            return; // end of show
        }

        // sleep on mutex, or
        // continue, if another job arrived
        mutex.lock();
        if (!restart)
            condition.wait(&mutex);
        if (restart)
            restart--;
        mutex.unlock();

        // time to exit thread
        if ( abort ) {
            outLog("workThread: Thread got abort 3");
            return; // end of show
        }
    }
}

// eof - workThread.cpp
