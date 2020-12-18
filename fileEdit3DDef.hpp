
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



#ifndef _FILEEDIT3D_DEF_H_
#define _FILEEDIT3D_DEF_H_

#include "nvutility.h"
#include "nvutility.hpp"
#include "nvMapGL.hpp"

#include "pfm.h"
#include "pfm_extras.h"

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <proj_api.h>
#include "gdal_priv.h"
#include <ogr_spatialref.h>

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QSharedMemory>

#include "charts.h"
#include "gsf.h"
#include "gsf_indx.h"
#include "gsf_flags.hpp"
#include "FileHydroOutput.h"
#include "FileTopoOutput.h"
#include "czmil.h"
#include <lasreader.hpp>
#include "slas.hpp"


#define GCS_NAD83 4269
#define GCS_WGS_84 4326


//   WGS84 / UTM northern hemisphere: 326zz where zz is UTM zone number
//   WGS84 / UTM southern hemisphere: 327zz where zz is UTM zone number
//   US State Plane (NAD83): 269xx

#define PCS_NAD83_UTM_zone_3N 26903
#define PCS_NAD83_UTM_zone_23N 26923
#define PCS_WGS84_UTM_zone_1N 32601
#define PCS_WGS84_UTM_zone_60N 32660
#define PCS_WGS84_UTM_zone_1S 32701
#define PCS_WGS84_UTM_zone_60S 32760


#define         MARKER_W        15
#define         MARKER_H        10


#define         AREA_BASED_FILTER           0
#define         HP_FILTER                   1

#define         HP_MIN_RADIUS               0.1
#define         HP_MAX_RADIUS               100.0
#define         HP_MIN_HEIGHT               0.1
#define         HP_MAX_HEIGHT               100.0
#define         HP_MIN_NEIGHBORS            1
#define         HP_MAX_NEIGHBORS            100


//    Pointer interaction functions.

#define         NOOP                        0
#define         DELETE_RECTANGLE            1
#define         DELETE_POLYGON              2
#define         RESTORE_RECTANGLE           3
#define         RESTORE_POLYGON             4
#define         DELETE_POINT                5
#define         KEEP_POLYGON                6
#define         HOTKEY_POLYGON              7
#define         ROTATE                      8
#define         ZOOM                        9
#define         MASK_INSIDE_RECTANGLE       10
#define         MASK_OUTSIDE_RECTANGLE      11
#define         MASK_INSIDE_POLYGON         12
#define         MASK_OUTSIDE_POLYGON        13
#define         MEASURE                     14
#define         POLY_FILTER_MASK            15
#define         RECT_FILTER_MASK            16
#define         HIGHLIGHT_POLYGON           17
#define         CLEAR_POLYGON               18


#define         NUMSHADES                   100
#define         FREEZE_POPUP                3
#define         NUM_SCALE_LEVELS            16
#define         DEFAULT_SEGMENT_LENGTH      0.25
#define         CHRTRNULL                   10000000000000000.0
#define         EDIT_OPTIONS                7
#define         LINE_WIDTH                  2
#define         POLYGON_POINTS              NVMAPGL_POLYGON_POINTS
#define         NUM_TOOLBARS                7
#define         MAX_SLICE_SIZE              50
#define         MAX_TRANS_VALUE             64


//  NUMPOPUPS should always be one more than you think you're using in rightMouse because the last one 
//  (i.e. [NUMPOPUPS - 1]) is used for turning off slicing.  You also have to modify slotPopupMenu
//  because it checks this value for debug purposes.

#define         NUMPOPUPS                   8


//  Button hotkeys that are editable by the user.  These are used as indexes into the options.buttonAccel, misc.buttonText,
//  misc.button, and misc.buttonIcon arrays.  To add or remove from this list you need to change it here, in set_defaults.cpp
//  and in fileEdit3D.cpp (look for the tool buttons that have one of these defined names attached).

#define         OPEN_FILE_KEY               0
#define         QUIT_KEY                    1
#define         RESET_KEY                   2
#define         DELETE_POINT_MODE_KEY       3
#define         DELETE_RECTANGLE_MODE_KEY   4
#define         DELETE_POLYGON_MODE_KEY     5
#define         FILTER_KEY                  6
#define         RECTANGLE_FILTER_MASK_KEY   7
#define         POLYGON_FILTER_MASK_KEY     8
#define         RUN_HOTKEY_POLYGON_MODE_KEY 9
#define         UNDO_KEY                    10
#define         CLEAR_HIGHLIGHT_KEY         11
#define         HIGHLIGHT_POLYGON_MODE_KEY  12
#define         CLEAR_POLYGON_MODE_KEY      13

#define         HOTKEYS                     14


//  PROGRAM indices into the ancillary program arrays.  If you want to change this (i.e. add or subtract programs
//  it needs to be done here, in set_defaults.cpp, and add or subtract buttons in fileEdit3D.cpp.

#define         EXAMGSF                     0
#define         GSFMONITOR                  1
#define         WAVEFORMMONITOR             2
#define         LIDARMONITOR                3
#define         CHARTSPIC                   4
#define         CZMILWAVEMONITOR            5
#define         LASWAVEMONITOR              6
#define         HOFRETURNKILL               7
#define         HOFRETURNKILL_SWA           8
#define         DELETE_SINGLE_POINT         9
#define         ACCEPT_FILTER_HIGHLIGHTED   10
#define         REJECT_FILTER_HIGHLIGHTED   11
#define         FREEZE                      12
#define         FREEZE_ALL                  13

#define         NUMPROGS                    14


//  IHO order levels for minimum Z window size computation.

#define         IHO_SPECIAL_ORDER           1
#define         IHO_ORDER_1A                2
#define         IHO_ORDER_1B                3
#define         IHO_ORDER_2                 4


//  MAX_STACK_POINTS points nearest to the cursor.  MAX_STACK_POINTS is defined in ABE.h (utility library directory).

typedef struct
{
  int32_t       point[MAX_STACK_POINTS];
  double        dist[MAX_STACK_POINTS];
  int32_t       num_stack_points;
} NEAREST_STACK;


//  The OPTIONS structure contains all those variables that can be saved to the
//  users fileEdit3D QSettings.


typedef struct
{
  int32_t     position_form;              //  Position format number (MISC)
  QColor      edit_color;                 //  Color to be used for marker and edit rectangles and polygons
  QColor      tracker_color;              //  Color to be used for "other ABE window" tracker
  QColor      background_color;           //  Color to be used for background
  QColor      scale_color;                //  Color to be used for the "scale"
  QColor      waveColor[MAX_STACK_POINTS];//  Colors for point markers and waveforms used in waveMonitor and CZMILwaveMonitor 
  QFont       font;                       //  Font used for all ABE map GUI applications
  int32_t     flag_index;                 //  0 - no flag, 1 - flag suspect, 2 - flag invalid
  float       min_window_size;            //  Minimum window size in Z
  int32_t     iho_min_window;             //  If not 0 compute minimum window size using IHO order level (special=1, 1=2, 2=3)
  uint8_t     display_invalid;            //  Display invalid data flag
  int32_t     function;                   //  Active edit or zoom function
  int32_t     slice_percent;              //  Percent of "Z" depth to use for a rotated "slice" view
  int32_t     slice_alpha;                //  Alpha (transparency) value for data that falls outside of the slice (0-255).
  uint8_t     slice_gray;                 //  Set if the user wants data outside the slice to be gray scale instead of color.
  float       z_factor;                   //  Conversion factor for Z values.  May be used to convert 
                                          //  m to ft...
  float       z_offset;                   //  Offset value for Z values.
  int32_t     point_size;                 //  Point size in pixels
  float       rotation_increment;         //  Degrees to rotate when pressing the left and right arrow keys
  float       nearest_value;              //  Value of point nearest to the cursor when moving the cursor
  int32_t     point_limit;                //  Number of input points to be displayed per page
  int32_t     zoom_percent;               //  Zoom in/out percentage
  double      exaggeration;               //  Z exaggeration
  float       zx_rotation;                //  Rotation around the ZX axis to be saved for startup.
  float       y_rotation;                 //  Rotation around the Y axis to be saved for startup.
  int32_t     filter_type;                //  Filter type (0 = original area-based filter, 1 = Hockey Puck filter)
  float       filterSTD;                  //  Filter standard deviation for area-based filter
  double      hp_filter_radius;           //  Hockey puck filter horizontal radius
  double      hp_filter_height;           //  Hockey puck filter height 
  int32_t     hp_filter_neighbors;        //  Hockey puck filter number of neighbors required
  uint8_t     deep_filter_only;           //  Filter only in the downward direction (assumes positive is depth)
  uint8_t     kill_and_respawn;           //  Set this to NVTrue to kill the ancillary programs on exit and 
                                          //  respawn them on restart.  If set to NVFalse then the programs will remain
                                          //  running and reconnect to fileEdit3D when it restarts.  This is a blanket
                                          //  setting and will be enforced regardless of the value of "state[]".
  uint8_t     highlight_tip;              //  Set this to NVTrue to display the highlighted points tool tip
  int32_t     main_button_icon_size;      //  Main button icon size (16, 24, 32)
  uint8_t     draw_scale;                 //  Set this to draw the scale in the display.
  QString     buttonAccel[HOTKEYS];       //  Accelerator key sequences for some of the buttons
  QString     inputFilter;                //  Last used input file filter
  int32_t     undo_levels;                //  Number of undo levels
  int32_t     msg_width;                  //  Message window width
  int32_t     msg_height;                 //  Message window height
  int32_t     msg_window_x;               //  Message window x position
  int32_t     msg_window_y;               //  Message window y position
  int32_t     num_hp_threads;             //  Number of threads to use for the Hockey Puck filter.


  //  These relate to the ancillary programs

  QString     prog[NUMPROGS];             //  Ancillary program command strings with keywords
  QString     name[NUMPROGS];             //  Ancillary program name
  QString     hotkey[NUMPROGS];           //  Ancillary programs hot key (the extra 2 are for the freeze options)
  QString     action[NUMPROGS];           //  Ancillary programs action hot keys
  QToolButton *progButton[NUMPROGS];      //  If this program is attached to a button, this is the button (otherwise NULL)
  uint8_t     data_type[NUMPROGS][PFM_DATA_TYPES]; //  The data types associated with an ancillary program
  uint8_t     hk_poly_eligible[NUMPROGS]; //  Whether the key is eligible to be used in a polygon
  uint8_t     hk_poly_only[NUMPROGS];     //  Whether the key can only be used for hotkey polygons
  int32_t     state[NUMPROGS];            //  0 - program does not respawn on restart, 1 - program respawns but was
                                          //  not up when fileEdit3D last closed, 2 - program respawns and was up
  uint32_t    kill_switch[NUMPROGS];      //  Kill switch value to be used to terminate ancillary programs.
                                          //  This number will always be above 10,000,000.
  QString     description[NUMPROGS];      //  Ancillary program description
  QString     input_dir;                  //  Last directory searched for input files
  QString     wktString[10];              //  QStrings holding recently used WKT settings for LAS files
} OPTIONS;


// The POINT_DATA structure contains variables related to points in the PFM file.

typedef struct
{
  std::vector<double>     x;                         //  X position
  std::vector<double>     y;                         //  Y position
  std::vector<float>      z;                         //  Z value
  std::vector<uint32_t>   val;                       //  Validity
  std::vector<uint32_t>   oval;                      //  Original validity
  std::vector<uint32_t>   rec;                       //  Record (e.g. ping) number
  std::vector<int32_t>    sub;                       //  Subrecord (e.g. beam) number
  std::vector<double>     time;                      //  Time array pointer
  std::vector<uint8_t>    mask;                      //  Whether we are hiding this point or not
  std::vector<uint8_t>    fmask;                     //  Whether this point is filter masked or not
  int32_t                 count;                     //  Total number of points in section
  int32_t                 num_beams;                 //  Number of beams in swath
  double                  x_grid_size;
  double                  y_grid_size;
  NV_F64_XYMBC            bounds;
} POINT_DATA;


// The UNDO block structure.

typedef struct
{
  std::vector<uint32_t> val;                       //  Validity
  std::vector<uint32_t> num;                       //  Point number in the POINT_DATA array
  int32_t               count;                     //  Number of points in this undo block
} UNDO;


// General stuff.

typedef struct
{
  QString     file;                       //  Input file name
  uint8_t     no_edit;                    //  Do not allow edits
  uint8_t     area_drawn;                 //  Area has been displayed flag
  uint8_t     drawing;                    //  Area is being drawn flag
  int32_t     save_function;              //  Saved active function when non-edit functions in use
  int32_t     save_mode;                  //  Saved active edit mode for exit (only delete functions)
  int32_t     num_records;                //  Number of records per section
  int32_t     num_subrecords;             //  Number of subrecords per record (actual number of points per section
                                          //  (e.g. 250 pings * 128 beams = 32,000 points) is stored in the data structure
                                          //  in data->count)
  uint64_t    total_records;              //  Total number of records (pings, shots) in the file
  int32_t     start_record;               //  First record of the file (for GSF it's 1 for others usually 0)
  int32_t     end_record;                 //  Last record of the file
  double      start_time;                 //  Start time of the file
  double      end_time;                   //  End time of the file
  int32_t     data_type;                  //  File data type
  int32_t     poly_count;                 //  Polygon point count
  double      polygon_x[POLYGON_POINTS];  //  Polygon X
  double      polygon_y[POLYGON_POINTS];  //  Polygon Y
  int32_t     hotkey_poly_count;          //  hotkey polygon count
  int32_t     *hotkey_poly_x;             //  hotkey polygon X points (allocated)
  int32_t     *hotkey_poly_y;             //  hotkey polygon Y points (allocated)
  int32_t     rotate_angle;               //  Rotation of view angle at 1/10 degree
  float       ortho_scale;                //  X scale of rotated view 
  int32_t     ortho_angle;                //  Rotation of view angle at 1/10 degree
  int32_t     ortho_min;                  //  Minimum "Z" value when rotated
  int32_t     ortho_max;                  //  Maximum "Z" value when rotated
  int32_t     ortho_range;                //  Max "Z" - min "Z" when rotated
  uint8_t     slice;                      //  Set if we're "slicing" the data
  int32_t     slice_min;                  //  Min "Z" value in slice
  int32_t     slice_max;                  //  Max "Z" value in slice
  int32_t     slice_size;                 //  Size of slice in "Z" (0 = off)
  double      slice_meters;               //  Size of slice in meters
  float       sin_array[3600];            //  Pre-computed sine values at 1/10 degree
  float       cos_array[3600];            //  Pre-computed cosine values at 1/10 degree
  int32_t     highlight_count;            //  Number of highlighted points
  std::vector<int32_t> highlight;         //  Highlighted points
  int32_t     hotkey;                     //  function number if we just pushed a command hotkey (or -1)
  uint8_t     drawing_canceled;           //  set to cancel drawing
  QColor      track_color_array[NUMSHADES];//  Colors for points in the tracker map
  uint8_t     color_array[2][NUMSHADES][4];//  Colors for points (0 is full color, 1 is semi-transparent)
  int32_t     draw_area_width;            //  Width of map drawing area in pixels
  int32_t     draw_area_height;           //  Height of map drawing area in pixels
  NV_I32_COORD2 center_point;             //  center of drawing area in pixels
  int32_t     nearest_point;              //  point nearest to the cursor
  QIcon       buttonIcon[HOTKEYS];        //  Tool button icons for tools having accelerators
  QString     buttonText[HOTKEYS];        //  Tooltip text for the buttons that have editable accelerators
  QToolButton *button[HOTKEYS];           //  Buttons that have editable accelerators
  QString     shared_file;                //  Shared file name for hotkey polygons.
  QColor      widgetBackgroundColor;      //  The normal widget background color.
  QSharedMemory *abeShare;                //  ABE's shared memory pointer.
  ABE_SHARE   *abe_share;                 //  Pointer to the ABE_SHARE structure in shared memory.
  NV_F64_XYMBR displayed_area;            //  Currently displayed area
  NEAREST_STACK nearest_stack;            //  Nine points nearest to the cursor
  float       min_z;
  float       max_z;
  uint8_t     marker_mode;                //  Only used in DELETE_POINT mode:  0 - normal, 1 - center marker is frozen
                                          //  but others are normal (when waveMonitor is running), 2 - freeze all 
                                          //  markers (when waveMonitor is running).
  int32_t     frozen_point;               //  Use this instead of nearest point if we freeze the marker
  QString     qsettings_app;              //  Application name for QSettings
  uint8_t     mask_active;                //  Set if we are masking any data.
  int16_t     unique_type[PFM_DATA_TYPES];//  Unique data types being displayed.
  int16_t     unique_count;               //  Number of unique data types being displayed.
  QWidget     *map_widget;                //  Map widget
  uint8_t     filter_mask;                //  Set if we have any filtermasked points.
  std::vector<int32_t> filter_kill_list;  //  List of points that are set to be killed by the filter
  int32_t     filter_kill_count;          //  Number of points that are set to be killed by the filter
  uint8_t     filtered;                   //  NVTrue if area was filtered.
  uint8_t     hydro_lidar_present;        //  NVTrue if we have Hydro LIDAR data.
  uint8_t     lidar_present;              //  NVTrue if we have Hydro or Topo LIDAR data.
  uint8_t     las_present;                //  NVTrue if we have LAS data.
  uint8_t     gsf_present;                //  NVTrue if we have GSF data.
  int32_t     process_id;                 //  This program's process ID
  QStatusBar  *progStatus;                //  Progress (and message) status bar
  QProgressBar *statusProg;               //  Generic progress bar in status area
  QLabel      *statusProgLabel;           //  Generic progress bar lchrtr2l
  QPalette    statusProgPalette;          //  Background color palette for the progress bar lchrtr2l
  double      map_center_x;               //  Center of displayed area (lon)
  double      map_center_y;               //  Center of displayed area (lat)
  double      map_center_z;               //  Center of displayed area (z)
  std::vector<UNDO> undo;                 //  The undo block array
  int32_t     undo_count;                 //  The number of undo blocks
  scaleBox    *scale[NUM_SCALE_LEVELS];
  QSharedMemory *abeRegister;             //  ABE's process register
  ABE_REGISTER *abe_register;             //  Pointer to the ABE_REGISTER structure in shared memory.
  uint8_t     linked;                     //  Set if we have linked to another ABE application.
  char         progname[256];             /*  This is the program name.  It will be used in all output to stderr so that shelling programs (like abe)
                                              will know what program printed out the error message.  */
  QStringList *messages;                  //  Message list


  //  The following are for LAS WKT support.

  char        filename[1024];             //  Input filename
  char        wkt[8192];                  //  Well-known Text
  projPJ      pj_utm;                     //  Proj.4 UTM projection
  projPJ      pj_latlon;                  //  Proj.4 latlon projection
  uint8_t     projected;                  //  Flag set if LAS file is projected
} MISC;


int32_t check_bounds (OPTIONS *options, MISC *misc, POINT_DATA *data, int32_t index, uint8_t ck, uint8_t slice);
int32_t check_bounds (OPTIONS *options, MISC *misc, POINT_DATA *data, double x, double y, uint32_t v, uint8_t mask, uint8_t ck, uint8_t slice);
void set_area (nvMapGL *map, OPTIONS *options, MISC *misc, POINT_DATA *data, int32_t *x_bounds, int32_t *y_bounds,
               int32_t num_vertices, int32_t rect_flag);
void keep_area (nvMapGL *map, OPTIONS *options, MISC *misc, POINT_DATA *data, int32_t *x_bounds, int32_t *y_bounds,
                int32_t num_vertices);
void overlayData (nvMapGL *map, OPTIONS *options, MISC *misc, POINT_DATA *data);
void overlayFlag (nvMapGL *map, OPTIONS *options, MISC *misc, POINT_DATA *data);
void compute_ortho_values (nvMapGL *map, MISC *misc, OPTIONS *options, POINT_DATA *data, QScrollBar *sliceBar, uint8_t set_to_min);
void store_undo (MISC *misc, int32_t undo_levels, uint32_t val, uint32_t num);
void end_undo_block (MISC *misc);
void undo (MISC *misc, POINT_DATA *data);
uint8_t resize_undo (MISC *misc, OPTIONS *options, int32_t undo_levels);


#endif
