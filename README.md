TgTake2 - 20180530 - Started 2011-08-28
-----------------

#### License

GNU GPL v2.0 (or later) - See LICENSE.txt

#### Outline

To function correctly, TgTake2 must be given certain information.

The primary is the project directory - must exist

```
--project=/any/path/
```

Several other directories will be constructed from this project 
root.

```
void MainWindow::updateDirectories() {
    QString projectDir(m_tabSetup->m_projdirEdit->text());
    // set composite directories
    if (projectDir.size()) {
        m_dataDir = projectDir+"/data"; // source (raw) data
        m_outpDir = projectDir+"/scenery"; // final construction
        m_workDir = projectDir+"/work"; // source (chopped) data
        m_tempDir = projectDir+"/temp"; // temporary work directory - for downloads
        m_elevDir = m_workDir+"/SRTM-30"; // HGT decode destination

        if (m_srtmDir.size() == 0) {
            m_srtmDir = m_tempDir+"/hgt"; // temp for HGT work files
        }
        if (m_shapDir.size() == 0) {
            m_shapDir = m_tempDir+"/shape"; // temp for SHAPE work files
        }
    }
}
```


And more as the scenery construction continues-

    m_workDir + "/AirportObj";
    m_workDir + "/AirportArea";
    m_workDir + "/Default";
    m_workDir + "/Landmass";


Wiki Information:
-----------------------

    - http://wiki.flightgear.org/Using_TerraGear - Elevations, Land Uses, Airports
    - http://wiki.flightgear.org/Howto:Make_an_airport - Using X-Plane  WED
    - http://wiki.flightgear.org/WorldEditor - WED source, build, ...
    - http://wiki.flightgear.org/TerraGear_GUI - The inspiration for this... tks Gijs...

Preprocessing Terrain - from terragear
=====================

TerraGear supports several terrain data sources:

1. 30-arcsec SRTM based terrain data covering the world (recommended
   over other 30-arcsec data sources):

   ftp://edcsgs9.cr.usgs.gov/pub/data/srtm/SRTM30/

   I don't recall the details at the moment for processing this data.
   Probably similar to the processing of the GLOBE data.

2. 30-arcsec world wide data: GLOBE project:

   http://www.ngdc.noaa.gov/seg/topo/globe.shtml

   a) First convert the "bin" DEM format to "ascii" DEM format using
      "Prep/DemRaw2ascii/raw2ascii"

   b) Then process the resulting files with "Prep/DemChop/demchop"


3. 30-arcsec world wide data: GTOPO30 data:

   http://edcwww.cr.usgs.gov/landdaac/gtopo30/gtopo30.html

   a) First convert the "bin" DEM format to "ascii" DEM format using
      "Prep/DemRaw2ascii/raw2ascii"

   b) Then process the resulting files with "Prep/DemChop/demchop"


4. SRTM (version 1 & 2) (1 and 3-arcsec nearly world wide coverage):

   ftp://edcsgs9.cr.usgs.gov/pub/data/srtm/

   a) Chop up each .zip files using "Prep/DemChop/hgtchop"


5. 3-arcsec ASCII DEM files:

   Generally, I recommend using the SRTM data over this older data
   set, however in places like Alaska, there is no SRTM coverage so
   this data is better than the 30 arcsec data.

       http://edcwww.cr.usgs.gov/doc/edchome/ndcdb/ndcdb.html

   a) Create the .arr.gz files using the "Prep/DemChop/demchop" utility.


The result for any of these terrain sources should be a "work" tree
with a .arr.gz file for each FG tile.

6. SRTM data comes with 'voids'.  These are areas where their
   automated data processing system could not reliably determine the
   elevation.  Often this happens over water, or over mountain peaks.
   There is a big chunk of the Grand Canyon missing, a big chunk of
   Rhode Island, etc.

   So as a partial fix I came up with a little program that will fill
   in the voids from another data source.  In this case the only thing
   I half trust is the USGS 3 arcsec DEM data for the USA.  So we
   can't fix voids outside the USA right now.

   In the same directory as DemChop and HgtChop there is a "fillvoids"
   program.  You might wish to run it with something like the
   following command line:

     find /export/fgfs05/curt/Work/SRTM2-North_America3 -name '*.arr.gz' -exec ./fillvoids {} /stage/fgfs05/curt/Work/USGS-DEM-USA-3 \;

7. After you create the .arr.gz files you have to create a
   corresponding .fit.gz file for each of these.  This is a data
   reduction step which fits a set of polygons to the raw terrain with
   a set of constraints on the maximum error allowed relative to the
   original data set, and a max/min number of allowed nodes in the
   fitted set.  The provided tools use a scheme that produces an
   adaptive fit which means fewer polygons in smooth flat areas, and
   more polygons in complex rough areas.  The end result is a *much*
   better fit with fewer polygons than you could achieve by just keeping
   every "nth" point from the original array.

   To walk through an entire tree of .arr.gz files and produce the
   corresponding .fit.gz files, use the "Prep/TerraFit/terrafit.py"
   utility.  Please ignore the old "ArrayFit" tools which use a stupid
   algorithm and are basically useless in comparison to TerraFit.
   

You should now have a large tree of .arr.gz files with a corresponding
.fit.gz file for each .arr.gz file.  It's worth double checking the
contents of your directory and counting all files of each type to make
sure you do have a one to one match and didn't miss anything.


Generating Airports
===================

Robin Peel maintains a world wide database of airports and navaids for
the X-Plane and FlightGear projects:

    http://www.x-plane.org/users/robinp/

Robin's apt.dat needs to be run through two scripts

    cat apt.dat | ./xp2simpleapt.pl > basic.dat
    cat apt.dat | ./xp2runway.pl > runways.dat

Compress these and copy them to $FG_ROOT/data/Airports

Now run the runways.dat through the getapt utility:

    genapts --input=runways.dat --work=$FG_WORK_DIR

Note: this creates a last_apt file which shows you the airport genapts
is currently working on.

    - if genapts crashes (which is possible if you try to run through the
      entire runways.dat file) you can look at last_apt to see where to start
      up again.

    - You can start in midstream using the --start-id=KABC option to genapts.

    - If you get a consistant crash on a particular airport, you probably
      found a bug in genapts, or there is some degenerate information at that
      airport (40 mile long runways, 2 runways spaced miles apart, etc.)
      Often you can fix the data and proceed.  Sometimes you can "nudge"
      things around to get past a genapts bug.  For instance, if you crahs
      consistantly on a valid looking runway, try nudging the heading or
      position by a least significant digit.  Sometimes we can get numerical
      problems with the polygon cliper and this often works around it.

Other considerations:

    - Airport generation pre-depends on terrain data being preped and ready
      so airport surfaces can be built properly.

    - If you prep new terrain data, you should probably rerun the airport
      generation step.


Processing VMAP0 data
=====================

Most of the FlightGear terrain features come from the VMAP0 data set
available from:

    http://geoengine.nima.mil/

There is a script in src/Prep/TGVPF/ called process.sh which will
generate all the VPF data for all 4 CD's.  Look at the script and
understand what it's doing.  I don't run it exactly as is because it
takes too long to run everything at once and if something bombs in the
middle, you generally have to start from the beginning just to be on
the safe side and avoid duplications and other potential weirdness.

I usually run each CD individual and copy the data to a separate area
when done.  Then I reassemble all the VPF data in the end before
running the final tile builder.


Processing Radio Tower data
===========================

I found some very detailed, very complete, very current radio tower
data from the FCC.  This even includes cell phone towers and
individual hobbiest towers.  However it only has USA coverage:

    http://wireless.fcc.gov/cgi-bin/wtb-datadump.pl

When you unzip this file, the good stuff is in DE.dat, I believe there
is file format information at the above url.

Run:

    .../Prep/Tower/tower.pl --input=$RAW_DIR/DE.dat --outdir=$WORK_DIR/TowerObj

This creates a tree of .ind files in $WORK_DIR/TowerObj/

If you add this directory to the list of directories searched by the
build tile program, these entries should get added to the final
scenery.  This of course requires the corresponding tower models to be
available in the base package.

==========================

; eof
