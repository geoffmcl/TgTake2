/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// move to github
#define PGM_VERSION TGT2_VERSION
#define PGM_DATE    TGT2_DATE

// move to try Ubuntu development
// #define PGM_VERSION "0.0.3"
// #define PGM_DATE    "2011-09-02"

// doing the Setup page, and dealing with the exit
// when in the SRTM count service
// PGM_VERSION "0.0.2"
// PGM_DATE    "2011-09-01"

// initial version
// PGM_VERSION "0.0.1"
// PGM_DATE    "2011-08-28"

// These should NOT be changed, unless the persistent
// 'settings' have to be changed
#define PGM_NAME    "TgTake2"
#define SET_VERSION "0.0.3"
#define PGM_LOG     "TgTake2.txt"
#ifndef DEF_ORG_NAME
#define DEF_ORG_NAME "geoffair"
#endif
#ifndef DEF_ORG_DOMAIN
#define DEF_ORG_DOMAIN "geoffair.org"
#endif

// features
#define TRY_GLOBAL_SETTINGS
#define GOT_THREAD_EXIT_OK   // in tabsetup.cpp

#ifndef APP_EPSILON
#define APP_EPSILON 0.0000001
#endif

#include "settings.h"

#endif // APP_CONFIG_H
