/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */

#ifndef APP_MACROS_H
#define APP_MACROS_H
#include <QMessageBox>

// MessageBox::information ( title and message )
#define MBI(t,m) QMessageBox::information(this,t,m);

// some MACROS to connect and set a value
#define MCON_T(a,b) connect(a,SIGNAL(toggled(bool)),this,SLOT(b(bool)))
#define MCON_E(a,b) connect(a, SIGNAL(editingFinished()), this, SLOT(b()))

#define MSET_T(a,c,d) a->setChecked(main->m_Settings->value(c,d).toBool())
#define MSET_E(a,c,d) a->setText(main->m_Settings->value(c,d).toString())

// combined macros
#define MCONSET_T(a,b,c,d) MCON_T(a,b); MSET_T(a,c,d)
#define MCONSET_E(a,b,c,d) MCON_E(a,b); MSET_E(a,c,d)

// generate code blocks
#define M_CONSET_T_LU(a,b,c,d) void tabLanduse::b(bool v) { main->saveSetting(c,v); }
#define M_CONSET_E_LU(a,b,c,d) void tabLanduse::b() { QString tmp(a->text()); if (tmp.length()) main->saveSetting(c,tmp); }

#define M_CONSET_T_CO(a,b,c,d) void tabConstruction::b(bool v) { main->saveSetting(c,v); }
#define M_CONSET_E_CO(a,b,c,d) void tabConstruction::b() { QString tmp(a->text()); if (tmp.length()) main->saveSetting(c,tmp); }

// ELEVATION
#define M_CONSET_T_EL(a,b,c,d) void tabElevations::b(bool v) { main->saveSetting(c,v); }
#define M_CONSET_E_EL(a,b,c,d) void tabElevations::b() { QString tmp(a->text()); if (tmp.length()) main->saveSetting(c,tmp); }

// ADVANCED
#define M_CONSET_T_AD(a,b,c,d) void tabAdvanced::b(bool v) { main->saveSetting(c,v); }
#define M_CONSET_E_AD(a,b,c,d) void tabAdvanced::b() { QString tmp(a->text()); if (tmp.length()) main->saveSetting(c,tmp); }

#endif // APP_MACROS_H
