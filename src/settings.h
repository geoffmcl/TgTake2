/* ==================================================
   TgTake2 project
   Created: Geoff R. McLane - Aug 2011
   License: GPL2 (or later)
   ================================================== */
#ifndef SETTINGS_H
#define SETTINGS_H

// last tab index
#define tabindexPath "tab/index"

// scenery limits
#define minlatPath "limits/minlat"
#define maxlatPath "limits/maxlat"
#define minlonPath "limits/minlon"
#define maxlonPath "limits/maxlon"

// directories
#define dirsprojectPath   "dirs/project"
#define dirstgtoolsPath   "dirs/tgtools"
#define dirsfgrootPath    "dirs/fgroot"
#define dirshgtrootPath    "dirs/hgtroot"

// elevations
#define maxnodesPath    "nodes/max"
#define minnodesPath    "nodes/min"
#define maxerrorPath    "max/errors"
#define dirshgtPath     "dirs/hgt"
#define arcsecPath      "value/arcsecs"

#define skipzipPath     "check/skipzip" //= new QCheckBox("Skip existing zips",this);
#define skiphgtPath     "check/skiphgt" //= new QCheckBox("Skip existing hgt",this);
#define skipcfmPath     "check/skipcmf" //= new QCheckBox("Skip confirmations",this);
#define conterrPath     "check/conterr" //= new QCheckBox("Continue on errors",this);


// landuse
#define dirsshapePath     "dirs/shape"
#define lu_inclmPath    "check/lu_inclmn"   // Check = new QCheckBox("Include Landmass",this);
#define lu_noovrdPath   "check/lu_noovrd"   // Check = new QCheckBox("No download overwrite",this);
#define lu_useshpPath   "check/lu_useshp"   // Check = new QCheckBox("Use shape-decode",this);
#define lu_ptwidPath    "point/lu_width"    // new QLabel("Point width:",this); ptwidEdit = new QLineEdit(this);
#define lu_maxsegPath   "max/lu_segment"    // new QLabel("Max. segment:",this); maxsegEdit = new QLineEdit(this);
#define lu_ignerrsPath  "check/lu_ignerrs"  // Check = new QCheckBox("Ignore Errors.",this);
#define lu_noovrPath    "check/lu_noovr"    // Check = new QCheckBox("No overwrites.",this);
#define lu_skiperrPath  "check/lu_skiperr"  // Check = new QCheckBox("Skip Errors.",this);
#define lu_skipcfmPath  "check/lu_skipcfm"  // Check = new QCheckBox("Skip Confirm.",this);


// airports
#define choicebutPath   "button/choice"
#define fileaptPath     "file/airport"
#define allaptsPath     "check/allapts"
#define maxslopePath    "max/slope"     //= new QLineEdit(this);
#define nudgePath       "nudge/factor"  //= new QLineEdit(this);
#define limdemPath      "check/limdem"  //= new QCheckBox("Limit DEM paths.",this);
#define ap_startEdit    "apedit/startEdit"
#define ap_oneEdit      "apedit/oneEdit"
#define ap_tileId       "apedit/tileId"
#define ap_chunkId      "apedit/chunkId"

// construction
#define constbutPath    "button/const"
#define co_useukPath    "check/co_useuk"    // QCheckBox("Use UK grid.",this);
#define co_ignlmPath    "check/co_ignlm"    // QCheckBox("Ignore Landmass.",this);
#define co_noovrPath    "check/co_noovr"    // QCheckBox("No overwrite existing.",this);
#define co_nosrcPath    "check/co_nosrc"    // QCheckBox("Skip if no source data.",this);
#define co_ignerrsPath  "check/co_ignerrs"  // QCheckBox("Ignore Errors.",this);
#define co_skipdnPath   "check/co_skipdn"   // QCheckBox("Skip done confirmations.",this);
#define co_nopriorPath  "check/co_noprior"  // QCheckBox("No --priorities command.",this);

// advanced - tools
#define toolwgetPath    "tool/wget"
#define toolunzipPath   "tool/unzip"
#define toollogPath     "tool/log"
#define ad_delayPath    "max/ad_delay"  // MCONSET_E(delayEdit,on_delay_edit,ad_delayPath,"3");
#define ad_maxPath      "max/ad_max"    // MCONSET_E(maxEdit,on_max_edit,ad_maxpath,"512");
#define ad_confirmPath  "check/ad_confirm"  // MCONSET_T(confirmCheck,on_confirm_toggle,ad_confirmPath,"1");   // = new QCheckBox("Confirm to proceed.",this);
#define ad_verifyPath   "check/ad_verify"   // MCONSET_T(verifyCheck,on_verify_toggle,ad_verifyPath,"0");      // = new QCheckBox("Verify Tools on use.",this);

// setup_state flags
#define suf_scene   0x00000001
#define suf_tools   0x00000002
#define suf_root    0x00000004
#define suf_ranges  0x00000008
#define suf_hgtroot 0x00000010

#define suf_ok (suf_scene | suf_tools | suf_root | suf_ranges)

#endif // SETTINGS_H
// eof - settings.h
