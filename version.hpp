
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/




#ifndef VERSION

#define     VERSION     "PFM Software - fileEdit3D V4.16 - 06/04/18"

#endif

/*

    Version 1.00
    Jan C. Depner
    01/01/10

    First working version.


    Version 1.01
    Jan C. Depner
    04/29/10

    Fixed posfix and fixpos calls to use numeric constants instead of strings for type.


    Version 1.02
    Jan C. Depner
    06/25/10

    Added support for HAWKEYE CSS Generic Binary Output Format data. Also, added support for displaying
    PFM_HAWKEYE_DATA in the chartsMonitor ancillary program.


    Version 1.03
    Jan C. Depner
    06/28/10

    Changed chartsMonitor to lidarMonitor due to multiple lidar support.


    Version 1.04
    Jan C. Depner
    07/06/10

    Fixed the ever elusive "deep flyer not showing up" problem.  Unfortunately I waited 3 months to fix it after I fixed it in
    pfmEdit3D.  I forgot about geoSwath3d having the same problem.  DOH!


    Version 1.05
    Jan C. Depner
    08/17/10

    Replaced our kludgy old UTM transform with calls to the PROJ 4 library functions.  All hail the PROJ 4 developers!


    Version 2.00
    Jan C. Depner
    11/05/10

    Finally fixed auto Z scaling in nvMapGL (nvutility library).  Also added minimum Z extents to preferences.


    Version 3.00
    Jan C. Depner
    12/08/10

    Added 2D tracker and auto filter options like in pfmEdit3D.  Cleaned up the button and ancillary program hotkey handling.


    Version 3.01
    Jan C. Depner
    12/09/10

    Fixed the slice and size bar buttons (finally).


    Version 3.02
    Jan C. Depner
    12/14/10

    We can finally mark data points in a somewhat proper way.  They don't disappear when you rotate or zoom.


    Version 3.03
    Jan C. Depner
    12/16/10

    Can now highlight and un-highlight in polygons.  Also, pressing the Enter key when data is highlighted will
    invalidate the highlighted points.


    Version 3.04
    Jan C. Depner
    01/05/11

    Added hotkey check in the preferences dialog to prevent conflicts.  I really should have done this a long time
    ago but it was way down on the priority list ;-)


    Version 3.05
    Jan C. Depner
    01/06/11

    Correct problem with the way I was instantiating the main widget.  This caused an intermittent error on Windows7/XP.


    Version 3.06
    Jan C. Depner
    01/15/11

    Added an exaggeration scrollbar to the left side of the window.


    Version 3.07
    Jan C. Depner
    01/16/11

    Positioned non-modal popup dialogs in the center of the screen.


    Version 3.08
    Jan C. Depner
    02/24/11

    Fixed a badly stupid error in io_data.cpp for GSF that was only showing up in 32 bit Windows XP for some unknown reason.
    It should have hosed things on all platforms but seemed to sneak through on all of the others (including 64 bit Windows 7).


    Version 3.09
    Jan C. Depner
    02/25/11

    Switched to using PFM_HAWKEYE_HYDRO_DATA and PFM_HAWKEYE_TOPO_DATA to replace the deprecated PFM_HAWKEYE_DATA.


    Version 3.10
    Jan C. Depner
    03/28/11

    Allow user to define the key for invalidating filter highlighted or manually highlighted data.  Try to save the user's
    hot key definitions even across setting version changes.


    Version 3.11
    Jan C. Depner
    04/14/11

    Removed tofWaterKill ancillary program.


    Version 3.12
    Jan C. Depner
    06/16/11

    Removed HMPS_SPARE_1 flag check since we don't do swath filtering anymore and it's been removed from hmpsflag.h.


    Version 3.13
    Jan C. Depner
    06/27/11

    Save all directories used by the QFileDialogs.  Add current working directory to the sidebar for all QFileDialogs.
    Removed the . on the command line option since the current working directory is in the sidebar.


    Version 3.14
    Jan C. Depner
    07/22/11

    Using setSidebarUrls function from nvutility to make sure that current working directory (.) and
    last used directory are in the sidebar URL list of QFileDialogs.


    Version 3.15
    Jan C. Depner
    11/30/11

    Converted .xpm icons to .png icons.


    Version 3.16
    Jan C. Depner
    02/21/12

    Now translates between PFM_DESIGNATED_SOUNDING flag and GSF HMPS_SELECTED_DESIGNATED flag.


    Version 3.17
    Jan C. Depner
    03/19/12

    Added a local Hockey Puck filter to the editor.  This was added after playing with the Point Cloud
    Library RadiusOutlierRemoval filter which worked well but was spherical in nature.  The Hockey Puck
    filter allows us to use different X/Y distances versus Z distances.  It also allows us to use the 
    horizontal and vertical uncertainty to make our filter smarter.  Oh, and did I mention, the Hockey
    Puck filter is about twice as fast ;-)


    Version 3.18
    Jan C. Depner
    07/18/12

    Added support for preliminary CZMIL CPF format.


    Version 3.19
    Jan C. Depner (PFM Software)
    12/09/13

    Switched to using .ini file in $HOME (Linux) or $USERPROFILE (Windows) in the ABE.config directory.  Now
    the applications qsettings will not end up in unknown places like ~/.config/navo.navy.mil/blah_blah_blah on
    Linux or, in the registry (shudder) on Windows.


    Version 3.20
    Stacy Johnson
    02/05/2014

    Removed optimization from mk script to try to improve upon segfault occurence


    Version 3.21
    Jan C. Depner (PFM Software)
    02/26/14

    Cleaned up "Set but not used" variables that show up using the 4.8.2 version of gcc.


    Version 3.22
    Jan C. Depner (PFM Software)
    03/03/14

    Replaced HMPS flags with NV_GSF flags.


    Version 3.23
    Jan C. Depner (PFM Software)
    03/17/14

    Removed WLF support.  Top o' the mornin' to ye!


    Version 3.24
    Jan C. Depner (PFM Software)
    03/19/14

    - Straightened up the Open Source acknowledgments.


    Version 3.25
    Jan C. Depner (PFM Software)
    03/21/14

    Removed rmsMonitor button (not used anymore).


    Version 3.26
    Jan C. Depner (PFM Software)
    05/27/14

    - Added the new LGPL licensed GSF library to the acknowledgments.


    Version 3.27
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 3.28
    Jan C. Depner (PFM Software)
    07/29/14

    - Fixed errors discovered by cppcheck.


    Version 3.29
    Jan C. Depner (PFM Software)
    12/09/14

    - Added minimum bounding rectangle checks to set_area in an attempt to speed up point inside polygon checking.


    Version 3.30
    Jan C. Depner (PFM Software)
    12/24/14

    - Replaced realloc with std::vector to address speed issues in Windows.


    Version 3.31
    Jan C. Depner (PFM Software)
    12/28/14

    - Taking advantage of a change to get2DCoords in nvMapGL.cpp in the nvutility library.  Basically, it no longer makes calls to
      get the viewport, mvmatrix, and projmatrix except the first time it's called for the loop of points.  This really shows up
      in Windows but it actually makes the Linux version faster as well.


    Version 3.32
    Jan C. Depner (PFM Software)
    02/15/15

    - To give better feedback to shelling programs in the case of errors I've added the program name to all output to stderr.
    - All non-fatal error messages and fatal error messages from child processes (like lidarMonitor) will now be displayed in a
      message dialog that will automatically pop up when a message is sent to stderr.


    Version 3.33
    Jan C. Depner (PFM Software)
    02/22/15

    - Added "Save" button for the error messages dialog.


    Version 3.34
    Jan C. Depner (PFM Software)
    03/14/15

    - Since AHAB Hawkeye has switched to LAS format I have removed support for the old Hawkeye I binary format.


    Version 3.35
    Jan C. Depner (PFM Software)
    03/24/15

    - Replaced the last two realloc calls with std::vector.
    - Added LAS data type.
    - Added LASwaveMonitor as ancillary program.
    - Fixed screwup that wasn't allowing data to be written except on exit.
    - Now computes Z bounds in get_bounds not just in compute_bounds.


    Version 3.36
    Jan C. Depner (PFM Software)
    03/27/15

    - Added slas_update_point_data to slas.cpp and replaced the direct update of bit flags in the LAS record.
      It's a bit slower but much easier to use.


    Version 4.00
    Jan C. Depner (PFM Software)
    03/30/15

    - Changed the name from geoSwath3D to fileEdit3D to avoid any problems with a certain sonar company.  Even
      though we were using the name geoSwath long before they were.  Sorry Tom ;-)
    - Now handles uppercase LAS and LAZ file extensions for Windows afflicted files that have been transferred
      to operating systems that UnDeRsTaNd ThE DiFfErEnCe between upper and lowercase characters.


    Version 4.01
    Jan C. Depner (PFM Software)
    03/31/15

    - Fixed name filter for GSF files so that it wouldn't find files that don't end in .dNN where NN is numeric.
      This prevents us from seeing, for example, ESRI shape .dbf files in the GSF name list.


    Version 4.02
    Jan C. Depner (PFM Software)
    04/09/15

    - Modified io_data.cpp and slas.cpp to use extended_number_of_point_records when dealing with LAS v1.4 files.


    Version 4.03
    Jan C. Depner (PFM Software)
    05/19/15

    - Removed waveWaterfall ancillary programs.  They didn't work and they didn't make much sense with just a single file.
    - Fixed screwup that caused CZMILwavemonitor to flash.  Only checking for 10 actions keys in buildCommand.
    - Fixed display of measurement data.


    Version 4.04
    Jan C. Depner (PFM Software)
    06/08/15

    - Changed the WAG of returns per shot for CZMIL to 16.
    - Increased the maximum number of points to 1,000,000.


    Version 4.05
    Jan C. Depner (PFM Software)
    07/09/15

    - Now handles Riegl LAS files that don't have GeographicTypeGeoKey and ProjectedCSTypeGeoKey defined in
      the GeoKeyDirectoryTag (34735) required VLR.


    Version 4.06
    Jan C. Depner (PFM Software)
    08/09/15

    - Added threaded Hockey Puck filter option.
    - Added option to set data outside of the slice to gray, semi-transparent (default) instead of colored, semi-transparent.
    - Fixed CTRL + Q exit hot key.


    Version 4.07
    Jan C. Depner (PFM Software)
    04/24/16

    - Replaced CZMIL_RETURN_SELECTED_SOUNDING with CZMIL_RETURN_CLASSIFICATION_MODIFIED.


    Version 4.08
    Jan C. Depner (PFM Software)
    05/04/16

    - Added ability to read Well-known Text (WKT) from LAS 1.4 files.
    - Now asks for Well-known Text for any LAS file that it can't handle (missing 34735, no WKT, etc.).


    Version 4.09
    Jan C. Depner (PFM Software)
    05/09/16

    - Now reads the LAS 1.4 WKT properly (I hope).


    Version 4.10
    Jan C. Depner (PFM Software)
    05/12/16

    - Yet another LAS hack - some people don't put in the GeographicTypeGeoKey and just depend on the
      ProjectedCSTypeGeoKey to tell them what datum they're using.


    Version 4.11
    Jan C. Depner (PFM Software)
    08/09/16

    - Now gets font from globalABE.ini as set in Preferences by pfmView.
    - To avoid possible confusion, removed translation setup from QSettings in env_in_out.cpp.


    Version 4.12
    Jan C. Depner (PFM Software)
    10/15/16

    - Just a little clean-up of a couple of proj.4 strings.


    Version 4.13
    Jan C. Depner (PFM Software)
    05/02/17

    - Switched to using the TEMP directory (from QDir::tempPath) for temporary shared_file.


    Version 4.14
    Jan C. Depner (PFM Software)
    06/23/17

    -  Removed redundant functions from slas.cpp that are available in the nvutility library.


    Version 4.15
    Jan C. Depner (PFM Software)
    09/25/17

    - A bunch of changes to support doing translations in the future.  There is a generic
      fileEdit3D_xx.ts file that can be run through Qt's "linguist" to translate to another language.


    Version 4.16
    Jan C. Depner (PFM Software)
    06/04/18

    - Replaced CZMIL_RETURN_USER_FLAG constant with CZMIL_RETURN_REPROCESSED due to change in CZMIL API.

*/
