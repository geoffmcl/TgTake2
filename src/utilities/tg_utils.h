/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */

#ifndef TG_UTILS_H
#define TG_UTILS_H

#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QIODevice>
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "srtm_utils.h" // for LL struct

extern QStringList tg_getMaterialList();
extern QStringList tg_getCustomShapes();
extern bool tg_verifyProjDir(QString path);
extern bool tg_createDirectory(QString path);

// run command
extern int tg_testApplication(QString cmd, QString & msg);
extern bool tg_trimLongMessage( int max, QString & info );
extern int tg_renameToOLDBAK(QString file);
extern QString tg_runProcess(QString arguments, QString dir, int * pErr);
extern void tg_openNonModalDialog(QString title, QString msg, int secs, QWidget *parent = 0);

extern bool tg_ValidTileId(QString tileId, PLL pll);
extern bool tg_ValidChunkId(QString tileId, PLL pll);

#endif // TG_UTILS_H
