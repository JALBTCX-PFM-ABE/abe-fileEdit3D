
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



/*  fileEdit3D class definitions.  */

#ifndef __FILEEDIT3D_H__
#define __FILEEDIT3D_H__

#include "fileEdit3DDef.hpp"
#include "prefs.hpp"
#include "hotkeyHelp.hpp"
#include "version.hpp"

#include "ancillaryProg.hpp"
#include "hotkeyPolygon.hpp"
#include "io_data.hpp"


void get_buffer (POINT_DATA *data, MISC *misc, int32_t record);


class fileEdit3D:public QMainWindow
{
  Q_OBJECT 


 public:

  fileEdit3D (int *argc = 0, char **argv = 0, QWidget *parent = 0);
  ~fileEdit3D ();

  void redrawMap (uint8_t reset_orientation);


 protected:

  POINT_DATA      data;

  OPTIONS         options;

  MISC            misc;

  float           *save_z;

  prefs           *prefs_dialog;

  NV_I32_COORD2   marker[16];

  QPainterPath    tracker_2D;

  QTimer          *trackCursor, *blinkTimer;

  ancillaryProg   *ancillaryProgram[NUMPROGS];

  hotkeyPolygon   *hkp;

  QDialog         *messageD;

  QListWidget     *messageBox;

  QLabel          *latLabel, *lonLabel, *zLabel, *recLabel, *subLabel, *numLabel, *exagLabel, *countLabel, *miscLabel, *transLabel, *sizeLabel, *meterLabel;

  QScrollBar      *fileBar, *sliceBar, *transBar, *sizeBar, *exagBar;

  QStatusBar      *statusBar[3];

  nvMapGL         *map;

  nvMap           *trackMap;

  NVMAPGL_DEF     mapdef;

  NVMAP_DEF       track_mapdef;

  QToolBar        *toolBar[NUM_TOOLBARS];

  QButtonGroup    *exitGrp, *progGrp;

  QAction         *bHelp, *popup[NUMPOPUPS], *popupHelp, *flag[5], *messageAction;;

  QToolButton     *bOpen, *bExit, *bReset, *bMeasure, *bDisplayInvalid, *bPrefs, *bLink, *bStop, *bDeletePoint, *bDeleteRect, *bDeletePoly,
                  *bKeepPoly, *bRestoreRect, *bRestorePoly, *bUndo, *bHotkeyPoly, *bFlag, *bHighlightPoly, *bClearPoly, *bClearHighlight, *bFilter,
                  *bFilterRectMask, *bFilterPolyMask, *bClearMasks, *bMaskInsideRect, *bMaskOutsideRect, *bMaskInsidePoly, *bMaskOutsidePoly,
                  *bMaskReset, *bLidarMonitor, *bChartsPic, *bWaveformMonitor, *bWaveMonitor, *bGSFMonitor, *bLASwaveMonitor;

  QActionGroup    *flagGrp;

  QIcon           flagIcon[5];

  QCursor         rotateCursor, zoomCursor, maskCursor, deleteRectCursor, deletePolyCursor, restoreRectCursor, restorePolyCursor, hotkeyPolyCursor,
                  measureCursor, keepPolyCursor, filterMaskRectCursor, filterMaskPolyCursor, highlightPolyCursor, clearPolyCursor;

  QTabWidget      *notebook;

  QGroupBox       *filterBox, *hpFilterBox;

  QLineEdit       *stdValue, *hpRadiusValue, *hpHeightValue, *hpNeighborsValue;

  QSlider         *stdSlider;

  QMenu           *popupMenu;

  QPalette        zPalette, filePalette, slicePalette, transPalette, sizePalette, exagPalette;

  QMessageBox     *filt;

  uint8_t         polygon_flip, finishing, slicing, fileBarEnabled, first_screen, double_click, redraw2D, filter_active, popup_active, file_opened,
                  direction;

  int32_t         popup_prog[NUMPOPUPS], filterMessage_slider_count, menu_cursor_x, menu_cursor_y, record_num, rb_rectangle, rb_polygon, rb_measure,
                  mv_measure, mv_measure_anchor, multi_marker[MAX_STACK_POINTS], prev_poly_x, prev_poly_y, active_window_id, abe_register_group,
                  lock_point, prev_icon_size, start_ctrl_x, start_ctrl_y, rotate_index, mv_tracker, mv_2D_tracker, num_messages;

  double          menu_cursor_lat, menu_cursor_lon, menu_cursor_z, range_x, range_y, range_z;

  NV_F64_COORD3   line_anchor;

  NV_F64_XYMBC    bounds;

  float           prev_z_factor, prev_z_offset;

  QString         hotkey_message;


  void open_file ();
  void closeEvent (QCloseEvent *event);
  void setFunctionCursor (int32_t function);
  void setMainButtons (uint8_t enable);
  void geo_z_label (double lat, double lon, double z);
  void readStandardError (QProcess *proc);
  void displayMessage ();

  void discardMovableObjects ();

  void midMouse (int32_t mouse_x, int32_t mouse_y, double lon, double lat, double z);
  void leftMouse (int32_t mouse_x, int32_t mouse_y, double lon, double lat, double z);
  void rightMouse (int32_t mouse_x, int32_t mouse_y, double lon, double lat, double z);
  void runPopupProg (int32_t prog);
  void clean_exit (int32_t ret);


protected slots:

  void slotOpen ();

  void slotPopupMenu (QAction *ac);
  void slotPopupHelp ();

  void slotMouseDoubleClick (QMouseEvent *e, double lon, double lat, double z);
  void slotMousePress (QMouseEvent *e, double lon, double lat, double z);
  void slotMouseRelease (QMouseEvent *e, double lon, double lat, double z);
  void slotWheel (QWheelEvent *e, double lon, double lat, double z);
  void slotMouseMove (QMouseEvent *e, double lon, double lat, double z, NVMAPGL_DEF l_mapdef);
  void slotResize (QResizeEvent *e);

  void slotTrackMousePress (QMouseEvent *e, double lon, double lat);
  void slotTrackMouseRelease (QMouseEvent *e, double lon, double lat);
  void slotTrackMouseMove (QMouseEvent *e, double lon, double lat);
  void slotTrackResize (QResizeEvent *e);
  void slotTrackPreRedraw (NVMAP_DEF mapdef);
  void slotTrackPostRedraw (NVMAP_DEF mapdef);

  void slotLink ();
  void slotRegisterABEKeySet (int32_t key);
  void slotTrackCursor ();
  void slotBlinkTimer ();

  void slotQuit ();

  void slotMaskReset ();
  void slotReset ();

  void slotFileValueChanged (int value);

  void slotSliceTriggered (int action);
  void slotSizeTriggered (int action);
  void slotTransTriggered (int action);

  void slotExagTriggered (int action);
  void slotExagReleased ();
  void slotExaggerationChanged (float exaggeration, float apparent_exaggeration);

  void slotDisplayInvalid ();

  void slotClearHighlight ();

  void slotFlagMenu (QAction *action);

  void slotProg (int id);

  void slotRedrawMap ();

  void slotHotkeyPolygonDone ();

  void slotMode (int id);

  void slotStop ();

  void slotHotkeyHelp ();
  void slotToolbarHelp ();

  void slotFilter ();
  void slotClearFilterMasks ();
  void slotFilterStdChanged (int value);
  void slotFilterStdReturn ();
  void slotHpFilterRadiusReturn ();
  void slotHpFilterHeightReturn ();
  void slotHpFilterNeighborsReturn ();
  void slotFilterAccept ();
  void slotFilterReject ();

  void slotUndo ();

  void slotPrefs ();
  void slotPrefDataChanged ();
  void slotPrefHotKeyChanged (int32_t i);

  void slotReadStandardError (QProcess *proc);
  void slotMessage ();
  void slotCloseMessage ();
  void slotClearMessage ();
  void slotSaveMessage ();

  void about ();
  void slotAcknowledgments ();
  void aboutQt ();


 private:

  void keyPressEvent (QKeyEvent *e);
  void DrawMultiCursors (void);
  void runProg (int id);
  uint8_t killProg (int id);

};

#endif
