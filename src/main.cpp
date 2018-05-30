/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   20161020 - Update to Qt5 msvc140-64
   See: http://doc.qt.io/qt-5/sourcebreaks.html
   ================================================== */
#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif
#include "mainwindow.h"
#include <QFont>
#include <QFontDatabase>
#include "utilities/utilities.h"

//* message handler
#if QT_VERSION >= 0x050000
// see: http://doc.qt.io/qt-5/qtglobal.html#qInstallMessageHandler
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}
#else // !#if QT_VERSION >= 0x050000

void my_ErrorMessages(QtMsgType type, const char *msg)
{
    QString m = msg;
    switch (type)
    {
    case QtDebugMsg:
        outLog("***debug: "+m+" ***\n",0);
        break;
    case QtWarningMsg:
        outLog("***warning: "+m+" ***\n",0);
        break;
    case QtCriticalMsg:
        outLog("***critical: "+m+" ***\n",0);
        break;
    case QtFatalMsg:
        outLog("***fatal: "+m+" ***\n",0);
        abort();
        break;
    }
}
#endif // #if QT_VERSION >= 0x050000 y/n

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString pgm_version(PGM_VERSION);
    QString pgm_date(PGM_DATE);
    QString pgm_name(PGM_NAME);
    QString set_version(SET_VERSION);

    QApplication::setOrganizationName(DEF_ORG_NAME);
    QApplication::setOrganizationDomain(DEF_ORG_DOMAIN);
    QApplication::setApplicationName(pgm_name);
    QApplication::setApplicationVersion(set_version);

#ifdef SET_DEFAULT_FONTS
    // Default font for the whole application
    QFontDatabase::addApplicationFont(":/font/freeuniversalregular");
    QFontDatabase::addApplicationFont(":/font/freeuniversalbold");

    // Setting a default size too, and spacing, because we need it on screen
    QFont universal("FreeUniversal", 10);
    universal.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    a.setFont(universal);
#endif // SET_DEFAULT_FONTS

    QString title = pgm_name+" - v"+pgm_version+" "+pgm_date;

    MainWindow w;
    w.setWindowTitle(title);
    w.show();

#if QT_VERSION >= 0x050000
    qInstallMessageHandler(myMessageOutput);
#else // !#if QT_VERSION >= 0x050000
    qInstallMsgHandler(my_ErrorMessages);
#endif // #if QT_VERSION >= 0x050000 y/n

    return a.exec();
}
