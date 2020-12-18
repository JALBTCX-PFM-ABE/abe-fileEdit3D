
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



//  fileEdit3D class.

/***************************************************************************\

    Module :        fileEdit3D

    Programmer :    Jan C. Depner

    Date :          01/13/10

    Purpose :       3D version of fileEdit

\***************************************************************************/

#include <unistd.h>
    
#include "fileEdit3D.hpp"
#include "fileEdit3DHelp.hpp"
#include "acknowledgments.hpp"


void envout (OPTIONS *options, MISC *misc, QMainWindow *mainWindow);



fileEdit3D::fileEdit3D (int *argc, char **argv, QWidget *parent):
  QMainWindow (parent, 0)
{
  extern char     *optarg;


  uint8_t envin (OPTIONS *options, MISC *misc, QMainWindow *mainWindow);
  void set_defaults (MISC *misc, OPTIONS *options, POINT_DATA *data, uint8_t restore);


  options.z_factor = 1.0;
  options.z_offset = 0.0;
  misc.no_edit = NVFalse;
  record_num = 0;
  direction = 1;


  QFileInfo fileInfo;
  int32_t option_index = 0;
  while (NVTrue) 
    {
      static struct option long_options[] = {{"file", required_argument, 0, 0},
                                             {"record", required_argument, 0, 0},
                                             {"z_factor", required_argument, 0, 0},
                                             {"z_offset", required_argument, 0, 0},
                                             {0, no_argument, 0, 0}};

      char c = (char) getopt_long (*argc, argv, "n", long_options, &option_index);
      if (c == -1) break;

      switch (c) 
        {
        case 0:

          switch (option_index)
            {
            case 0:
              misc.file = QString (optarg);
              fileInfo = QFileInfo (misc.file);
              this->setWindowTitle (QString ("fileEdit3D : ") + fileInfo.fileName ());
              break;

            case 1:
              sscanf (optarg, "%d", &record_num);
              break;

            case 2:
              sscanf (optarg, "%f", &options.z_factor);
              break;

            case 3:
              sscanf (optarg, "%f", &options.z_offset);
              break;
            }
          break;

        case 'n':
          misc.no_edit = NVTrue;
          break;
        }
    }


  //  This is so we can drag and drop files on the Desktop icon.

  if (*argc == 2 && misc.file == "")
    {
      misc.file = QString (argv[1]);
      fileInfo = QFileInfo (misc.file);
      this->setWindowTitle (QString ("fileEdit : ") + fileInfo.fileName ());
    }


  /******************************************* IMPORTANT NOTE ABOUT SHARED MEMORY **************************************** \

      This is a little note about the use of shared memory within the Area-Based Editor (ABE) programs.  If you read
      the Qt documentation (or anyone else's documentation) about the use of shared memory they will say "Dear [insert
      name of omnipotent being of your choice here], whatever you do, always lock shared memory when you use it!".
      The reason they say this is that access to shared memory is not atomic.  That is, reading shared memory and then
      writing to it is not a single operation.  An example of why this might be important - two programs are running,
      the first checks a value in shared memory, sees that it is a zero.  The second program checks the same location
      and sees that it is a zero.  These two programs have different actions they must perform depending on the value
      of that particular location in shared memory.  Now the first program writes a one to that location which was
      supposed to tell the second program to do something but the second program thinks it's a zero.  The second program
      doesn't do what it's supposed to do and it writes a two to that location.  The two will tell the first program 
      to do something.  Obviously this could be a problem.  In real life, this almost never occurs.  Also, if you write
      your program properly you can make sure this doesn't happen.  In ABE we almost never lock shared memory because
      something much worse than two programs getting out of sync can occur.  If we start a program and it locks shared
      memory and then dies, all the other programs will be locked up.  When you look through the ABE code you'll see
      that we very rarely lock shared memory, and then only for very short periods of time.  This is by design.

  \******************************************* IMPORTANT NOTE ABOUT SHARED MEMORY ****************************************/


    //  Get the shared memory area.  If it exists, delete it.  We're using the same shared 
    //  memory structure as PFM_ABE so that we can talk to the same ancillary programs.  

  QString skey;
  skey.sprintf ("%d_abe", getpid ());

  misc.abeShare = new QSharedMemory (skey);

  if (!misc.abeShare->create (sizeof (ABE_SHARE), QSharedMemory::ReadWrite)) misc.abeShare->attach (QSharedMemory::ReadWrite);

  misc.abe_share = (ABE_SHARE *) misc.abeShare->data ();


  //  Have to set the focus policy or keypress events don't work properly at first in Focus Follows Mouse mode

  setFocusPolicy (Qt::WheelFocus);
  setFocus (Qt::ActiveWindowFocusReason);
  

  //  Set a few defaults for startup

  strcpy (misc.progname, argv[0]);
  polygon_flip = NVFalse;
  prev_poly_x = -1;
  prev_poly_y = -1;
  popup_active = NVFalse;
  double_click = NVFalse;
  prefs_dialog = NULL;
  mv_tracker = -1;
  mv_2D_tracker = -1;
  mv_measure_anchor = -1;
  mv_measure = -1;
  rb_polygon = -1;
  rb_measure = -1;
  rb_rectangle = -1;
  active_window_id = getpid ();
  start_ctrl_x = -1;
  start_ctrl_y = -1;
  rotate_index = 0;
  lock_point = 0;
  slicing = NVFalse;
  filterMessage_slider_count = -1;
  filter_active = NVFalse;
  file_opened = NVFalse;
  messageD = NULL;
  misc.messages = new QStringList ();
  num_messages = 0;


  // 
  //                      Key flags are intialized here.
  //                      multiMode represents a waveform display mode in the waveMonitor program 
  //                      (0: nearest-neighbor, 1: single waveform).  multiNum refers to the number
  //                      of multiple waveforms at the current time to display.  This variable will
  //                      equal to MAX_STACK_POINTS if we are in nearest-neighbor mode.
  //

  if (!misc.abe_share->mwShare.waveMonitorRunning) misc.abe_share->mwShare.multiMode = 1;
  misc.abe_share->mwShare.multiNum = 0;

  for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++)
    {
      multi_marker[i] = -1;
      misc.abe_share->mwShare.multiPresent[i] = -1;
    }


  save_z = NULL;


  //  Set up the marker data list

  marker[0].x = -MARKER_W;
  marker[0].y = -MARKER_H;
  marker[1].x = MARKER_W;
  marker[1].y = -MARKER_H;
  marker[2].x = MARKER_W;
  marker[2].y = -MARKER_H;
  marker[3].x = MARKER_W;
  marker[3].y = MARKER_H;
  marker[4].x = MARKER_W;
  marker[4].y = MARKER_H;
  marker[5].x = -MARKER_W;
  marker[5].y = MARKER_H;
  marker[6].x = -MARKER_W;
  marker[6].y = MARKER_H;
  marker[7].x = -MARKER_W;
  marker[7].y = -MARKER_H;
  marker[8].x = -MARKER_W;
  marker[8].y = 0;
  marker[9].x = -(MARKER_W / 5);
  marker[9].y = 0;
  marker[10].x = (MARKER_W / 5);
  marker[10].y = 0;
  marker[11].x = MARKER_W;
  marker[11].y = 0;
  marker[12].x = 0;
  marker[12].y = -MARKER_H;
  marker[13].x = 0;
  marker[13].y = -(MARKER_W / 5);
  marker[14].x = 0;
  marker[14].y = (MARKER_W / 5);
  marker[15].x = 0;
  marker[15].y = MARKER_H;


  //  Make the 2D "tracker" cursor painter path.

  tracker_2D = QPainterPath ();

  tracker_2D.moveTo (0, 0);
  tracker_2D.lineTo (30, 0);
  tracker_2D.lineTo (30, 20);
  tracker_2D.lineTo (0, 20);
  tracker_2D.lineTo (0, 0);

  tracker_2D.moveTo (0, 10);
  tracker_2D.lineTo (12, 10);

  tracker_2D.moveTo (30, 10);
  tracker_2D.lineTo (18, 10);

  tracker_2D.moveTo (15, 0);
  tracker_2D.lineTo (15, 6);

  tracker_2D.moveTo (15, 20);
  tracker_2D.lineTo (15, 14);


  //  Set the main icon

  setWindowIcon (QIcon (":/icons/fileEdit3D.png"));


  //  Define all of the cursors

  deleteRectCursor = QCursor (QPixmap (":/icons/delete_rect_cursor.png"), 3, 0);
  deletePolyCursor = QCursor (QPixmap (":/icons/delete_poly_cursor.png"), 3, 0);
  restoreRectCursor = QCursor (QPixmap (":/icons/restore_rect_cursor.png"), 11, 0);
  restorePolyCursor = QCursor (QPixmap (":/icons/restore_poly_cursor.png"), 11, 0);
  hotkeyPolyCursor = QCursor (QPixmap (":/icons/hotkey_poly_cursor.png"), 0, 19);
  keepPolyCursor = QCursor (QPixmap (":/icons/keep_poly_cursor.png"), 5, 0);
  rotateCursor = QCursor (QPixmap (":/icons/rotate_cursor.png"), 17, 17);
  zoomCursor = QCursor (QPixmap (":/icons/zoom_cursor.png"), 11, 11);
  maskCursor = QCursor (QPixmap (":/icons/mask_cursor.png"), 1, 1);
  measureCursor = QCursor (QPixmap (":/icons/measure_cursor.png"), 1, 1);
  filterMaskRectCursor = QCursor (QPixmap (":/icons/filter_mask_rect_cursor.png"), 1, 1);
  filterMaskPolyCursor = QCursor (QPixmap (":/icons/filter_mask_poly_cursor.png"), 1, 1);
  highlightPolyCursor = QCursor (QPixmap (":/icons/highlight_polygon_cursor.png"), 1, 1);
  clearPolyCursor = QCursor (QPixmap (":/icons/clear_polygon_cursor.png"), 1, 1);


  for (int32_t i = 0 ; i < NUMPROGS ; i++) ancillaryProgram[i] = NULL;


  //  Set all of the defaults

  set_defaults (&misc, &options, &data, NVFalse);


  misc.abe_share->ppid = misc.process_id;


  //  We have to get the icon size out of sequence (the rest of the options are read in env_in_out.cpp)
  //  so that we'll have the proper sized icons for the toolbars.  Otherwise, they won't be placed correctly.

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/" + QString (misc.qsettings_app) + ".ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/" + QString (misc.qsettings_app) + ".ini";
#endif


  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup (misc.qsettings_app);
  options.main_button_icon_size = settings.value (tr ("main button icon size"), options.main_button_icon_size).toInt ();
  settings.endGroup ();


  //  Create View toolbar

  toolBar[0] = new QToolBar (tr ("View tool bar"));
  toolBar[0]->setToolTip (tr ("View tool bar"));
  addToolBar (toolBar[0]);
  toolBar[0]->setObjectName (tr ("View tool bar"));


  bExit = new QToolButton (this);
  misc.button[QUIT_KEY] = bExit;
  bExit->setIcon (misc.buttonIcon[QUIT_KEY]);
  connect (bExit, SIGNAL (clicked ()), this, SLOT (slotQuit ()));
  bExit->setWhatsThis (exitText);
  toolBar[0]->addWidget (bExit);

  bOpen = new QToolButton (this);
  misc.button[OPEN_FILE_KEY] = bOpen;
  bOpen->setIcon (misc.buttonIcon[OPEN_FILE_KEY]);
  bOpen->setWhatsThis (openText);
  bOpen->setShortcut (options.buttonAccel[0]);
  connect (bOpen, SIGNAL (clicked ()), this, SLOT (slotOpen ()));
  toolBar[0]->addWidget (bOpen);

  bReset = new QToolButton (this);
  misc.button[RESET_KEY] = bReset;
  bReset->setIcon (misc.buttonIcon[RESET_KEY]);
  bReset->setWhatsThis (resetText);
  connect (bReset, SIGNAL (clicked ()), this, SLOT (slotReset ()));
  toolBar[0]->addWidget (bReset);


  toolBar[0]->addSeparator ();
  toolBar[0]->addSeparator ();


  bDisplayInvalid = new QToolButton (this);
  bDisplayInvalid->setIcon (QIcon (":/icons/displayinvalid.png"));
  bDisplayInvalid->setToolTip (tr ("Display invalid data"));
  bDisplayInvalid->setWhatsThis (displayInvalidText);
  bDisplayInvalid->setCheckable (true);
  toolBar[0]->addWidget (bDisplayInvalid);

  toolBar[0]->addSeparator ();
  toolBar[0]->addSeparator ();


  flagIcon[0] = QIcon (":/icons/user_flag.png");
  flagIcon[1] = QIcon (":/icons/suspect.png");
  flagIcon[2] = QIcon (":/icons/flag_invalid.png");


  QMenu *flagMenu = new QMenu (this);

  QActionGroup *flagGrp = new QActionGroup (this);
  connect (flagGrp, SIGNAL (triggered (QAction *)), this, SLOT (slotFlagMenu (QAction *)));
  flagGrp->setExclusive (true);

  flag[0] = flagMenu->addAction (tr ("Turn off data flags"));
  flagGrp->addAction (flag[0]);
  flag[0]->setIcon (flagIcon[0]);
  flag[0]->setCheckable (true);

  flag[1] = flagMenu->addAction (tr ("Mark suspect data"));
  flagGrp->addAction (flag[1]);
  flag[1]->setIcon (flagIcon[1]);
  flag[1]->setCheckable (true);

  flag[2] = flagMenu->addAction (tr ("Mark invalid data"));
  flagGrp->addAction (flag[2]);
  flag[2]->setIcon (flagIcon[2]);
  flag[2]->setCheckable (true);

  bFlag = new QToolButton (this);
  bFlag->setIcon (QIcon (":/icons/user_flag.png"));
  bFlag->setToolTip (tr ("Toggle data flags on/off"));
  bFlag->setWhatsThis (flagText);
  bFlag->setPopupMode (QToolButton::InstantPopup);
  bFlag->setMenu (flagMenu);
  toolBar[0]->addWidget (bFlag);


  toolBar[0]->addSeparator ();
  toolBar[0]->addSeparator ();


  QButtonGroup *modeGrp = new QButtonGroup (this);
  connect (modeGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotMode (int)));
  modeGrp->setExclusive (true);


  bHighlightPoly = new QToolButton (this);
  misc.button[HIGHLIGHT_POLYGON_MODE_KEY] = bHighlightPoly;
  bHighlightPoly->setIcon (misc.buttonIcon[HIGHLIGHT_POLYGON_MODE_KEY]);
  modeGrp->addButton (bHighlightPoly, HIGHLIGHT_POLYGON);
  bHighlightPoly->setWhatsThis (highlightPolyText);
  bHighlightPoly->setCheckable (true);
  toolBar[0]->addWidget (bHighlightPoly);

  bClearPoly = new QToolButton (this);
  misc.button[CLEAR_POLYGON_MODE_KEY] = bClearPoly;
  bClearPoly->setIcon (misc.buttonIcon[CLEAR_POLYGON_MODE_KEY]);
  modeGrp->addButton (bClearPoly, CLEAR_POLYGON);
  bClearPoly->setWhatsThis (clearPolyText);
  bClearPoly->setCheckable (true);
  toolBar[0]->addWidget (bClearPoly);

  bClearHighlight = new QToolButton (this);
  misc.button[CLEAR_HIGHLIGHT_KEY] = bClearHighlight;
  bClearHighlight->setIcon (misc.buttonIcon[CLEAR_HIGHLIGHT_KEY]);
  bClearHighlight->setWhatsThis (clearHighlightText);
  connect (bClearHighlight, SIGNAL (clicked ()), this, SLOT (slotClearHighlight ()));
  toolBar[0]->addWidget (bClearHighlight);


  //  Create Utilities toolbar

  toolBar[1] = new QToolBar (tr ("Utilities tool bar"));
  toolBar[1]->setToolTip (tr ("Utilities tool bar"));
  addToolBar (toolBar[1]);
  toolBar[1]->setObjectName (tr ("Utilities tool bar"));


  bPrefs = new QToolButton (this);
  bPrefs->setIcon (QIcon (":/icons/prefs.png"));
  bPrefs->setToolTip (tr ("Change application preferences"));
  bPrefs->setWhatsThis (prefsText);
  connect (bPrefs, SIGNAL (clicked ()), this, SLOT (slotPrefs ()));
  toolBar[1]->addWidget (bPrefs);


  bLink = new QToolButton (this);
  bLink->setIcon (QIcon (":/icons/unlink.png"));
  bLink->setToolTip (tr ("Link to other ABE applications"));
  bLink->setWhatsThis (linkText);
  connect (bLink, SIGNAL (clicked ()), this, SLOT (slotLink ()));
  toolBar[1]->addWidget (bLink);


  toolBar[1]->addSeparator ();
  toolBar[1]->addSeparator ();


  bHelp = QWhatsThis::createAction (this);
  bHelp->setIcon (QIcon (":/icons/contextHelp.png"));
  toolBar[1]->addAction (bHelp);


  //  Create Edit toolbar

  toolBar[2] = new QToolBar (tr ("Edit tool bar"));
  toolBar[2]->setToolTip (tr ("Edit tool bar"));
  addToolBar (toolBar[2]);
  toolBar[2]->setObjectName (tr ("Edit tool bar"));

  bStop = new QToolButton (this);
  bStop->setIcon (QIcon (":/icons/stop.png"));
  bStop->setToolTip (tr ("Stop drawing"));
  bStop->setWhatsThis (stopText);
  bStop->setEnabled (false);
  connect (bStop, SIGNAL (clicked ()), this, SLOT (slotStop ()));
  toolBar[2]->addWidget (bStop);


  toolBar[2]->addSeparator ();
  toolBar[2]->addSeparator ();


  bDeletePoint = new QToolButton (this);
  misc.button[DELETE_POINT_MODE_KEY] = bDeletePoint;
  bDeletePoint->setIcon (misc.buttonIcon[DELETE_POINT_MODE_KEY]);
  modeGrp->addButton (bDeletePoint, DELETE_POINT);
  bDeletePoint->setWhatsThis (deletePointText);
  bDeletePoint->setCheckable (true);
  toolBar[2]->addWidget (bDeletePoint);


  toolBar[2]->addSeparator ();
  toolBar[2]->addSeparator ();


  bDeleteRect = new QToolButton (this);
  misc.button[DELETE_RECTANGLE_MODE_KEY] = bDeleteRect;
  bDeleteRect->setIcon (misc.buttonIcon[DELETE_RECTANGLE_MODE_KEY]);
  modeGrp->addButton (bDeleteRect, DELETE_RECTANGLE);
  bDeleteRect->setWhatsThis (deleteRectText);
  bDeleteRect->setCheckable (true);
  toolBar[2]->addWidget (bDeleteRect);

  bDeletePoly = new QToolButton (this);
  misc.button[DELETE_POLYGON_MODE_KEY] = bDeletePoly;
  bDeletePoly->setIcon (misc.buttonIcon[DELETE_POLYGON_MODE_KEY]);
  modeGrp->addButton (bDeletePoly, DELETE_POLYGON);
  bDeletePoly->setWhatsThis (deletePolyText);
  bDeletePoly->setCheckable (true);
  toolBar[2]->addWidget (bDeletePoly);


  toolBar[2]->addSeparator ();
  toolBar[2]->addSeparator ();


  bKeepPoly = new QToolButton (this);
  bKeepPoly->setIcon (QIcon (":/icons/keep_poly.png"));
  modeGrp->addButton (bKeepPoly, KEEP_POLYGON);
  bKeepPoly->setToolTip (tr ("Delete data outside polygon"));
  bKeepPoly->setWhatsThis (keepPolyText);
  bKeepPoly->setCheckable (true);
  toolBar[2]->addWidget (bKeepPoly);


  toolBar[2]->addSeparator ();
  toolBar[2]->addSeparator ();


  bRestoreRect = new QToolButton (this);
  bRestoreRect->setIcon (QIcon (":/icons/restore_rect.png"));
  modeGrp->addButton (bRestoreRect, RESTORE_RECTANGLE);
  bRestoreRect->setToolTip (tr ("Restore invalid data inside rectangle"));
  bRestoreRect->setWhatsThis (restoreRectText);
  bRestoreRect->setCheckable (true);
  toolBar[2]->addWidget (bRestoreRect);

  bRestorePoly = new QToolButton (this);
  bRestorePoly->setIcon (QIcon (":/icons/restore_poly.png"));
  modeGrp->addButton (bRestorePoly, RESTORE_POLYGON);
  bRestorePoly->setToolTip (tr ("Restore invalid data inside polygon"));
  bRestorePoly->setWhatsThis (restorePolyText);
  bRestorePoly->setCheckable (true);
  toolBar[2]->addWidget (bRestorePoly);


  bUndo = new QToolButton (this);
  misc.button[UNDO_KEY] = bUndo;
  bUndo->setIcon (misc.buttonIcon[UNDO_KEY]);
  bUndo->setToolTip (tr ("Undo last edit operation"));
  bUndo->setWhatsThis (undoText);
  connect (bUndo, SIGNAL (clicked ()), this, SLOT (slotUndo ()));
  toolBar[2]->addWidget (bUndo);


  toolBar[2]->addSeparator ();
  toolBar[2]->addSeparator ();


  bHotkeyPoly = new QToolButton (this);
  misc.button[RUN_HOTKEY_POLYGON_MODE_KEY] = bHotkeyPoly;
  bHotkeyPoly->setIcon (misc.buttonIcon[RUN_HOTKEY_POLYGON_MODE_KEY]);
  modeGrp->addButton (bHotkeyPoly, HOTKEY_POLYGON);
  bHotkeyPoly->setWhatsThis (hotkeyPolyText);
  bHotkeyPoly->setCheckable (true);
  toolBar[2]->addWidget (bHotkeyPoly);


  toolBar[2]->addSeparator ();
  toolBar[2]->addSeparator ();


  bFilter = new QToolButton (this);
  misc.button[FILTER_KEY] = bFilter;
  bFilter->setIcon (misc.buttonIcon[FILTER_KEY]);
  bFilter->setWhatsThis (filterText);
  connect (bFilter, SIGNAL (clicked ()), this, SLOT (slotFilter ()));
  toolBar[2]->addWidget (bFilter);

  bFilterRectMask = new QToolButton (this);
  misc.button[RECTANGLE_FILTER_MASK_KEY] = bFilterRectMask;
  bFilterRectMask->setIcon (misc.buttonIcon[RECTANGLE_FILTER_MASK_KEY]);
  bFilterRectMask->setWhatsThis (filterRectMaskText);
  modeGrp->addButton (bFilterRectMask, RECT_FILTER_MASK);
  bFilterRectMask->setCheckable (true);
  toolBar[2]->addWidget (bFilterRectMask);

  bFilterPolyMask = new QToolButton (this);
  misc.button[POLYGON_FILTER_MASK_KEY] = bFilterPolyMask;
  bFilterPolyMask->setIcon (misc.buttonIcon[POLYGON_FILTER_MASK_KEY]);
  bFilterPolyMask->setWhatsThis (filterPolyMaskText);
  modeGrp->addButton (bFilterPolyMask, POLY_FILTER_MASK);
  bFilterPolyMask->setCheckable (true);
  toolBar[2]->addWidget (bFilterPolyMask);

  bClearMasks = new QToolButton (this);
  bClearMasks->setIcon (QIcon (":/icons/clear_filter_masks.png"));
  bClearMasks->setToolTip (tr ("Clear filter masks"));
  bClearMasks->setWhatsThis (clearMasksText);
  connect (bClearMasks, SIGNAL (clicked ()), this, SLOT (slotClearFilterMasks ()));
  toolBar[2]->addWidget (bClearMasks);


  toolBar[2]->addSeparator ();
  toolBar[2]->addSeparator ();


  bMeasure = new QToolButton (this);
  bMeasure->setIcon (QIcon (":/icons/measure.png"));
  bMeasure->setToolTip (tr ("Measure distance, azimuth, and Z difference"));
  modeGrp->addButton (bMeasure, MEASURE);
  bMeasure->setWhatsThis (measureText);
  bMeasure->setCheckable (true);
  toolBar[2]->addWidget (bMeasure);


  //  Create Mask toolbar

  toolBar[3] = new QToolBar (tr ("Mask tool bar"));
  toolBar[3]->setToolTip (tr ("Mask tool bar"));
  addToolBar (toolBar[3]);
  toolBar[3]->setObjectName (tr ("Mask tool bar"));

  bMaskOutsideRect = new QToolButton (this);
  bMaskOutsideRect->setIcon (QIcon (":/icons/mask_outside_rect.png"));
  bMaskOutsideRect->setToolTip (tr ("Mask data outside a rectangle"));
  bMaskOutsideRect->setWhatsThis (maskOutsideRectText);
  bMaskOutsideRect->setCheckable (true);
  modeGrp->addButton (bMaskOutsideRect, MASK_OUTSIDE_RECTANGLE);
  toolBar[3]->addWidget (bMaskOutsideRect);

  bMaskOutsidePoly = new QToolButton (this);
  bMaskOutsidePoly->setIcon (QIcon (":/icons/mask_outside_poly.png"));
  bMaskOutsidePoly->setToolTip (tr ("Mask data outside a polygon"));
  bMaskOutsidePoly->setWhatsThis (maskOutsidePolyText);
  bMaskOutsidePoly->setCheckable (true);
  modeGrp->addButton (bMaskOutsidePoly, MASK_OUTSIDE_POLYGON);
  toolBar[3]->addWidget (bMaskOutsidePoly);


  toolBar[3]->addSeparator ();
  toolBar[3]->addSeparator ();


  bMaskInsideRect = new QToolButton (this);
  bMaskInsideRect->setIcon (QIcon (":/icons/mask_inside_rect.png"));
  bMaskInsideRect->setToolTip (tr ("Mask data inside a rectangle"));
  bMaskInsideRect->setWhatsThis (maskInsideRectText);
  bMaskInsideRect->setCheckable (true);
  modeGrp->addButton (bMaskInsideRect, MASK_INSIDE_RECTANGLE);
  toolBar[3]->addWidget (bMaskInsideRect);

  bMaskInsidePoly = new QToolButton (this);
  bMaskInsidePoly->setIcon (QIcon (":/icons/mask_inside_poly.png"));
  bMaskInsidePoly->setToolTip (tr ("Mask data inside a polygon"));
  bMaskInsidePoly->setWhatsThis (maskInsidePolyText);
  bMaskInsidePoly->setCheckable (true);
  modeGrp->addButton (bMaskInsidePoly, MASK_INSIDE_POLYGON);
  toolBar[3]->addWidget (bMaskInsidePoly);


  toolBar[3]->addSeparator ();
  toolBar[3]->addSeparator ();


  bMaskReset = new QToolButton (this);
  bMaskReset->setIcon (QIcon (":/icons/mask_reset.png"));
  bMaskReset->setToolTip (tr ("Clear data mask"));
  bMaskReset->setWhatsThis (maskResetText);
  connect (bMaskReset, SIGNAL (clicked ()), this, SLOT (slotMaskReset ()));
  toolBar[3]->addWidget (bMaskReset);



  //  Create LIDAR toolbar

  toolBar[4] = new QToolBar (tr ("LiDAR tool bar"));
  toolBar[4]->setToolTip (tr ("LiDAR tool bar"));
  addToolBar (toolBar[4]);
  toolBar[4]->setObjectName (tr ("LiDAR tool bar"));


  QButtonGroup *progGrp = new QButtonGroup (this);
  connect (progGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotProg (int)));
  progGrp->setExclusive (false);


  bLidarMonitor = new QToolButton (this);
  bLidarMonitor->setIcon (QIcon (":/icons/lidar_monitor.png"));
  bLidarMonitor->setWhatsThis (lidarMonitorText);
  bLidarMonitor->setCheckable (true);
  QString tip = options.description[LIDARMONITOR] + "  [" + options.hotkey[LIDARMONITOR] + "]";
  bLidarMonitor->setToolTip (tip);
  bLidarMonitor->setShortcut (QKeySequence (options.hotkey[LIDARMONITOR]));
  options.progButton[LIDARMONITOR] = bLidarMonitor;
  progGrp->addButton (bLidarMonitor, LIDARMONITOR);
  toolBar[4]->addWidget (bLidarMonitor);

  bChartsPic = new QToolButton (this);
  bChartsPic->setIcon (QIcon (":/icons/charts_pic.png"));
  bChartsPic->setWhatsThis (chartsPicText);
  bChartsPic->setCheckable (true);
  tip =  options.description[CHARTSPIC] + "  [" + options.hotkey[CHARTSPIC] + "]";
  bChartsPic->setToolTip (tip);
  bChartsPic->setShortcut (QKeySequence (options.hotkey[CHARTSPIC]));
  options.progButton[CHARTSPIC] = bChartsPic;
  progGrp->addButton (bChartsPic, CHARTSPIC);
  toolBar[4]->addWidget (bChartsPic);

  bWaveformMonitor = new QToolButton (this);
  bWaveformMonitor->setIcon (QIcon (":/icons/waveform_monitor.png"));
  bWaveformMonitor->setWhatsThis (waveformMonitorText);
  bWaveformMonitor->setCheckable (true);
  tip =  options.description[WAVEFORMMONITOR] + "  [" + options.hotkey[WAVEFORMMONITOR] + "]";
  bWaveformMonitor->setToolTip (tip);
  bWaveformMonitor->setShortcut (QKeySequence (options.hotkey[WAVEFORMMONITOR]));
  options.progButton[WAVEFORMMONITOR] = bWaveformMonitor;
  progGrp->addButton (bWaveformMonitor, WAVEFORMMONITOR);
  toolBar[4]->addWidget (bWaveformMonitor);

  bWaveMonitor = new QToolButton (this);
  bWaveMonitor->setIcon (QIcon (":/icons/wave_monitor.png"));
  bWaveMonitor->setWhatsThis (waveMonitorText);
  bWaveMonitor->setCheckable (true);
  tip =  options.description[CZMILWAVEMONITOR] + "  [" + options.hotkey[CZMILWAVEMONITOR] + "]";
  bWaveMonitor->setToolTip (tip);
  bWaveMonitor->setShortcut (QKeySequence (options.hotkey[CZMILWAVEMONITOR]));
  options.progButton[CZMILWAVEMONITOR] = bWaveMonitor;
  progGrp->addButton (bWaveMonitor, CZMILWAVEMONITOR);
  toolBar[4]->addWidget (bWaveMonitor);


  //  Create GSF toolbar

  toolBar[5] = new QToolBar (tr ("GSF tool bar"));
  toolBar[5]->setToolTip (tr ("GSF tool bar"));
  addToolBar (toolBar[5]);
  toolBar[5]->setObjectName (tr ("GSF tool bar"));

  bGSFMonitor = new QToolButton (this);
  bGSFMonitor->setIcon (QIcon (":/icons/gsf_monitor.png"));
  bGSFMonitor->setWhatsThis (gsfMonitorText);
  bGSFMonitor->setCheckable (true);
  tip =  options.description[GSFMONITOR] + "  [" + options.hotkey[GSFMONITOR] + "]";
  bGSFMonitor->setToolTip (tip);
  bGSFMonitor->setShortcut (options.hotkey[GSFMONITOR]);
  options.progButton[GSFMONITOR] = bGSFMonitor;
  progGrp->addButton (bGSFMonitor, GSFMONITOR);
  toolBar[5]->addWidget (bGSFMonitor);


  //  Create LAS toolbar

  toolBar[6] = new QToolBar (tr ("LAS tool bar"));
  toolBar[6]->setToolTip (tr ("LAS tool bar"));
  addToolBar (toolBar[6]);
  toolBar[6]->setObjectName (tr ("LAS tool bar"));


  bLASwaveMonitor = new QToolButton (this);
  bLASwaveMonitor->setIcon (QIcon (":/icons/LASwaveMonitor.png"));
  bLASwaveMonitor->setWhatsThis (LASwaveMonitorText);
  bLASwaveMonitor->setCheckable (true);
  tip =  options.description[LASWAVEMONITOR] + "  [" + options.hotkey[LASWAVEMONITOR] + "]";
  bLASwaveMonitor->setToolTip (tip);
  bLASwaveMonitor->setShortcut (QKeySequence (options.hotkey[LASWAVEMONITOR]));
  options.progButton[LASWAVEMONITOR] = bLASwaveMonitor;
  progGrp->addButton (bLASwaveMonitor, LASWAVEMONITOR);
  toolBar[6]->addWidget (bLASwaveMonitor);


  //  Set the icon sizes so that the tool bars will set up properly.

  prev_icon_size = options.main_button_icon_size;

  QSize mainButtonIconSize (options.main_button_icon_size, options.main_button_icon_size);

  for (int32_t i = 0 ; i < NUM_TOOLBARS ; i++) toolBar[i]->setIconSize (mainButtonIconSize);


  //  Get the user's defaults if available

  if (!envin (&options, &misc, this))
    {
      //  Set the geometry from defaults since we didn't get any from the saved settings.

      this->resize (misc.draw_area_width, misc.draw_area_height);
      this->move (0, 0);
    }


  // Set the application font

  QApplication::setFont (options.font);


  //  Allocate the undo blocks based on the max number of undo levels (user option).

  try
    {
      misc.undo.resize (options.undo_levels);
    }
  catch (std::bad_alloc&)
    {
      fprintf (stderr, "%s %s %s %d - undo - %s\n", misc.progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  memset (misc.undo.data (), 0, options.undo_levels * sizeof (UNDO));


  //  Allocate the colors.  Doing this here speeds up drawing.

  float hue_inc = 315.0 / (float) (NUMSHADES + 1);

  for (int32_t j = 0 ; j < NUMSHADES ; j++)
    {
      misc.track_color_array[j].setHsv ((int32_t) (j * hue_inc), 255, 255, 255); 
      misc.color_array[0][j][0] = misc.color_array[1][j][0] = misc.track_color_array[j].red ();
      misc.color_array[0][j][1] = misc.color_array[1][j][1] = misc.track_color_array[j].green ();
      misc.color_array[0][j][2] = misc.color_array[1][j][2] = misc.track_color_array[j].blue ();
      misc.color_array[0][j][3] = misc.track_color_array[j].alpha ();

      misc.color_array[1][j][3] = options.slice_alpha;
    }


  //  Use geometry to get the width and height.

  QRect tmp = this->geometry ();
  int32_t width = tmp.width ();
  int32_t height = tmp.height ();


  hotkey_message = "<ul>";

  for (int32_t i = 0 ; i < NUMPROGS ; i++)
    hotkey_message += "<li>" + options.hotkey[i] + " = " + options.prog[i] + tr (";  action keys = ") + options.action[i] + "</li>";

  hotkey_message += "</ul><br>" +
    tr ("For more information on data types and other restrictions on the use of hot keys see <b>Ancillary Programs</b> in the "
        "<b>Preferences</b> dialog - <img source=\":/icons/prefs.png\">");


  misc.save_mode = options.function;
  misc.save_function = options.function;


  //  Set the map values from the defaults

  mapdef.draw_width = width;
  mapdef.draw_height = height;
  mapdef.exaggeration = options.exaggeration;

  mapdef.min_z_extents = options.min_window_size;
  mapdef.mode = NVMAPGL_POINT_MODE;
  mapdef.auto_scale = NVTrue;
  mapdef.projected = 0;

  mapdef.coasts = NVFalse;
  mapdef.landmask = NVFalse;
  mapdef.light_model = GL_LIGHT_MODEL_AMBIENT;

  mapdef.coast_color = Qt::white;
  mapdef.background_color = options.background_color;
  mapdef.scale_color = options.scale_color;
  mapdef.zoom_percent = options.zoom_percent;
  mapdef.draw_scale = options.draw_scale;


  mapdef.initial_bounds.min_x = -180.0;
  mapdef.initial_bounds.min_y = -90.0;
  mapdef.initial_bounds.max_x = 180.0;
  mapdef.initial_bounds.max_y = 90.0;
  mapdef.initial_bounds.min_z = 999999999.0;
  mapdef.initial_bounds.max_z = -999999999.0;
  mapdef.initial_y_rotation = options.y_rotation;
  mapdef.initial_zx_rotation = options.zx_rotation;

  mapdef.renderMode = NVMAPGL_RENDER_POINT_MODE;
  mapdef.complexObjectWidth = 0.002;
  mapdef.complexDivisionals = 8;


  QFrame *frame = new QFrame (this, 0);

  setCentralWidget (frame);


  //  Make the map.

  map = new nvMapGL (this, &mapdef, "fileEdit3D");
  map->setWhatsThis (mapText);


  misc.map_widget = map;


  //  Connect to the signals from the map class.

  connect (map, SIGNAL (mousePressSignal (QMouseEvent *, double, double, double)), this,
           SLOT (slotMousePress (QMouseEvent *, double, double, double)));
  connect (map, SIGNAL (mouseDoubleClickSignal (QMouseEvent *, double, double, double)), this,
           SLOT (slotMouseDoubleClick (QMouseEvent *, double, double, double)));
  connect (map, SIGNAL (mouseReleaseSignal (QMouseEvent *, double, double, double)), this, 
           SLOT (slotMouseRelease (QMouseEvent *, double, double, double)));
  connect (map, SIGNAL (mouseMoveSignal (QMouseEvent *, double, double, double, NVMAPGL_DEF)), this, 
           SLOT (slotMouseMove (QMouseEvent *, double, double, double, NVMAPGL_DEF)));
  connect (map, SIGNAL (wheelSignal (QWheelEvent *, double, double, double)), this, 
           SLOT (slotWheel (QWheelEvent *, double, double, double)));
  connect (map, SIGNAL (resizeSignal (QResizeEvent *)), this, SLOT (slotResize (QResizeEvent *)));
  connect (map, SIGNAL (exaggerationChanged (float, float)), this, SLOT (slotExaggerationChanged (float, float)));


  //  Layouts, what fun!

  QVBoxLayout *vBox = new QVBoxLayout (frame);


  //  Set up the VBox for the 2D tracker box and the status information.

  QGroupBox *leftBox = new QGroupBox ();
  QVBoxLayout *leftBoxLayout = new QVBoxLayout ();
  leftBox->setLayout (leftBoxLayout);


  int32_t trackBoxWidth = 275, trackBoxHeight = 275;


  //  Set up the 2D tracker box.

  track_mapdef.projection = CYLINDRICAL_EQUIDISTANT;
  track_mapdef.draw_width = 100;
  track_mapdef.draw_height = 100;
  track_mapdef.overlap_percent = 0;
  track_mapdef.grid_inc_x = 0.0;
  track_mapdef.grid_inc_y = 0.0;

  track_mapdef.coasts = NVFalse;
  track_mapdef.landmask = NVFalse;

  track_mapdef.border = 0;
  track_mapdef.coast_color = Qt::white;
  track_mapdef.grid_color = Qt::white;
  track_mapdef.background_color = Qt::black;

  track_mapdef.initial_bounds.min_x = -180.0;
  track_mapdef.initial_bounds.min_y = -90.0;
  track_mapdef.initial_bounds.max_x = 180.0;
  track_mapdef.initial_bounds.max_y = 90.0;

  trackMap = new nvMap (this, &track_mapdef);
  trackMap->setMinimumSize (trackBoxWidth - 20, trackBoxHeight - 20);
  trackMap->setMaximumSize (trackBoxWidth - 20, trackBoxHeight - 20);


  trackMap->setWhatsThis (trackMapText);


  //  Connect to the signals from the map class.

  connect (trackMap, SIGNAL (mousePressSignal (QMouseEvent *, double, double)), this,
           SLOT (slotTrackMousePress (QMouseEvent *, double, double)));
  connect (trackMap, SIGNAL (mouseReleaseSignal (QMouseEvent *, double, double)), this, 
           SLOT (slotTrackMouseRelease (QMouseEvent *, double, double)));
  connect (trackMap, SIGNAL (mouseMoveSignal (QMouseEvent *, double, double)), this,
           SLOT (slotTrackMouseMove (QMouseEvent *, double, double)));
  connect (trackMap, SIGNAL (resizeSignal (QResizeEvent *)), this, SLOT (slotTrackResize (QResizeEvent *)));
  connect (trackMap, SIGNAL (preRedrawSignal (NVMAP_DEF)), this, SLOT (slotTrackPreRedraw (NVMAP_DEF)));
  connect (trackMap, SIGNAL (postRedrawSignal (NVMAP_DEF)), this, SLOT (slotTrackPostRedraw (NVMAP_DEF)));


  notebook = new QTabWidget ();
  notebook->setTabPosition (QTabWidget::North);
  notebook->setMinimumSize (trackBoxWidth, trackBoxHeight);
  notebook->setMaximumSize (trackBoxWidth, trackBoxHeight);


  notebook->addTab (trackMap, tr ("2D Tracker"));


  //  The following sets up the filter accept/reject notebook page.

  QGroupBox *fbox = new QGroupBox (this);
  QVBoxLayout *fboxLayout = new QVBoxLayout;
  fbox->setLayout (fboxLayout);


  filterBox = new QGroupBox (tr ("000000 points selected"), this);
  filterBox->setToolTip (tr ("Please use the What's This help button then click here for full documentation"));
  filterBox->setWhatsThis (filterBoxText);
  QHBoxLayout *filterBoxLayout = new QHBoxLayout;
  filterBox->setLayout (filterBoxLayout);

  
  QGroupBox *sbox = new QGroupBox (tr ("STD", "Abbreviation for standard deviation"), this);
  QHBoxLayout *sboxLayout = new QHBoxLayout;
  sbox->setLayout (sboxLayout);

  
  stdSlider = new QSlider ();
  stdSlider->setTracking (true);
  stdSlider->setRange (3, 40);
  stdSlider->setSingleStep (1);
  stdSlider->setPageStep (1);
  stdSlider->setInvertedControls (true);
  stdSlider->setInvertedAppearance (true);
  stdSlider->setValue (NINT (options.filterSTD * 10.0));
  stdSlider->setMinimumHeight (70);
  stdSlider->setToolTip (tr ("Filter standard deviation value"));
  stdSlider->setWhatsThis (stdSliderText);
  connect (stdSlider, SIGNAL (valueChanged (int)), this, SLOT (slotFilterStdChanged (int)));
  connect (stdSlider, SIGNAL (sliderReleased ()), this, SLOT (slotFilter ()));
  sboxLayout->addWidget (stdSlider, 10);


  stdValue = new QLineEdit ("2.4", this);
  stdValue->setAlignment (Qt::AlignCenter);
  QString std = QString ("%L1").arg (options.filterSTD, 0, 'f', 1);
  stdValue->setText (std);
  stdValue->setToolTip (tr ("Filter standard deviation value"));
  stdValue->setWhatsThis (stdSliderText);
  connect (stdValue, SIGNAL (returnPressed ()), this, SLOT (slotFilterStdReturn ()));
  sboxLayout->addWidget (stdValue);


  filterBoxLayout->addWidget (sbox);
  fboxLayout->addWidget (filterBox);


  QGroupBox *arbox = new QGroupBox (this);
  QHBoxLayout *arboxLayout = new QHBoxLayout;
  arbox->setLayout (arboxLayout);

  QPushButton *acceptButton = new QPushButton (tr ("Accept"), this);
  acceptButton->setToolTip (tr ("Accept filter results [%1]").arg (options.hotkey[ACCEPT_FILTER_HIGHLIGHTED]));
  acceptButton->setWhatsThis (tr ("Accept filter results and invalidate marked points"));
  connect (acceptButton, SIGNAL (clicked ()), this, SLOT (slotFilterAccept ()));
  arboxLayout->addWidget (acceptButton);


  QPushButton *rejectButton = new QPushButton (tr ("Reject"), this);
  rejectButton->setToolTip (tr ("Reject filter results [%1]").arg (options.hotkey[REJECT_FILTER_HIGHLIGHTED]));
  rejectButton->setWhatsThis (tr ("Reject filter results"));
  connect (rejectButton, SIGNAL (clicked ()), this, SLOT (slotFilterReject ()));
  arboxLayout->addWidget (rejectButton);


  fboxLayout->addWidget (arbox);


  notebook->addTab (fbox, tr ("Area-Based Filter"));
  notebook->setTabEnabled (1, false);


  //  The following sets up the Hockey Puck filter accept/reject notebook page.

  QGroupBox *hpbox = new QGroupBox (this);
  QVBoxLayout *hpboxLayout = new QVBoxLayout;
  hpbox->setLayout (hpboxLayout);


  hpFilterBox = new QGroupBox (tr ("000000 points selected"), this);
  hpFilterBox->setWhatsThis (hpFilterBoxText);
  QFormLayout *formLayout = new QFormLayout;
  hpFilterBox->setLayout (formLayout);
  
  hpRadiusValue = new QLineEdit (QString ("%L1").arg (1.0), this);
  hpRadiusValue->setAlignment (Qt::AlignCenter);
  QString rad = QString ("%L1").arg (options.hp_filter_radius, 0, 'f', 1);
  hpRadiusValue->setText (rad);
  hpRadiusValue->setToolTip (tr ("Hockey Puck filter radius value"));
  hpRadiusValue->setWhatsThis (hpRadiusValueText);
  connect (hpRadiusValue, SIGNAL (returnPressed ()), this, SLOT (slotHpFilterRadiusReturn ()));
  formLayout->addRow (tr ("&Radius:"), hpRadiusValue);

  hpHeightValue = new QLineEdit ("1.0", this);
  hpHeightValue->setAlignment (Qt::AlignCenter);
  QString ht = QString ("%L1").arg (options.hp_filter_height, 0, 'f', 1);
  hpHeightValue->setText (ht);
  hpHeightValue->setToolTip (tr ("Hockey Puck filter height value"));
  hpHeightValue->setWhatsThis (hpHeightValueText);
  connect (hpHeightValue, SIGNAL (returnPressed ()), this, SLOT (slotHpFilterHeightReturn ()));
  formLayout->addRow (tr ("&Height:"), hpHeightValue);

  hpNeighborsValue = new QLineEdit (" 000 ", this);
  hpNeighborsValue->setAlignment (Qt::AlignCenter);
  QString nb = QString ("%L1").arg (options.hp_filter_neighbors);
  hpNeighborsValue->setText (nb);
  hpNeighborsValue->setToolTip (tr ("Hockey Puck filter neighbors value"));
  hpNeighborsValue->setWhatsThis (hpNeighborsValueText);
  connect (hpNeighborsValue, SIGNAL (returnPressed ()), this, SLOT (slotHpFilterNeighborsReturn ()));
  formLayout->addRow (tr ("&Neighbors:"), hpNeighborsValue);


  hpboxLayout->addWidget (hpFilterBox);


  QGroupBox *hprbox = new QGroupBox (this);
  hprbox->setFlat (true);
  QHBoxLayout *hprboxLayout = new QHBoxLayout;
  hprbox->setLayout (hprboxLayout);

  QPushButton *hpFilterAccept = new QPushButton (tr ("Accept"), this);
  hpFilterAccept->setToolTip (tr ("Accept filter results [%1]").arg (options.hotkey[ACCEPT_FILTER_HIGHLIGHTED]));
  hpFilterAccept->setWhatsThis (tr ("Accept filter results and invalidate marked points"));
  connect (hpFilterAccept, SIGNAL (clicked ()), this, SLOT (slotFilterAccept ()));
  hprboxLayout->addWidget (hpFilterAccept);


  QPushButton *hpFilterReject = new QPushButton (tr ("Reject"), this);
  hpFilterReject->setToolTip (tr ("Reject filter results [%1]").arg (options.hotkey[REJECT_FILTER_HIGHLIGHTED]));
  hpFilterReject->setWhatsThis (tr ("Discard filter results"));
  connect (hpFilterReject, SIGNAL (clicked ()), this, SLOT (slotFilterReject ()));
  hprboxLayout->addWidget (hpFilterReject);


  hpboxLayout->addWidget (hprbox);


  notebook->addTab (hpbox, tr ("Hockey Puck Filter"));
  notebook->setTabEnabled (2, false);


  notebook->setCurrentIndex (0);


  leftBoxLayout->addWidget (notebook);


  //  Now we set up the information/status portion of the left side box.

  QGroupBox *numBox = new QGroupBox ();
  numBox->setMaximumWidth (trackBoxWidth);
  numBox->setMinimumWidth (trackBoxWidth);
  QVBoxLayout *numBoxLayout = new QVBoxLayout ();
  numBox->setLayout (numBoxLayout);

  numLabel = new QLabel (this);
  numLabel->setFrameStyle (QFrame::StyledPanel | QFrame::Sunken);
  numLabel->setToolTip (tr ("Number of points in current view"));
  numLabel->setWhatsThis (tr ("Number of points in current view"));
  numBoxLayout->addWidget (numLabel);


  leftBoxLayout->addWidget (numBox);


  QGroupBox *statBox = new QGroupBox ();
  QGridLayout *statBoxLayout = new QGridLayout;
  statBox->setLayout (statBoxLayout);
  statBox->setMaximumWidth (trackBoxWidth);


  for (int32_t i = 0 ; i < NUM_SCALE_LEVELS ; i++)
    {
      misc.scale[i] = new scaleBox (this);
      misc.scale[i]->setMinimumWidth (80);
      statBoxLayout->addWidget (misc.scale[i], i, 0, Qt::AlignLeft);
    }


  QLabel *latName = new QLabel ("Lat:", this);
  latName->setToolTip (tr ("Latitude of cursor"));
  latName->setMinimumSize (latName->sizeHint ());
  statBoxLayout->addWidget (latName, 0, 1, Qt::AlignLeft);

  latLabel = new QLabel ("N 90 00 00.00", this);
  latLabel->setToolTip (tr ("Latitude of cursor"));
  latLabel->setMinimumSize (latLabel->sizeHint ());
  statBoxLayout->addWidget (latLabel, 0, 2, Qt::AlignLeft);
  
  QLabel *lonName = new QLabel ("Lon:", this);
  lonName->setToolTip (tr ("Longitude of cursor"));
  lonName->setMinimumSize (lonName->sizeHint ());
  statBoxLayout->addWidget (lonName, 1, 1, Qt::AlignLeft);

  lonLabel = new QLabel ("N 90 00 00.00", this);
  lonLabel->setToolTip (tr ("Longitude of cursor"));
  lonLabel->setMinimumSize (lonLabel->sizeHint ());
  statBoxLayout->addWidget (lonLabel, 1, 2, Qt::AlignLeft);

  QLabel *recName = new QLabel ("Rec:", this);
  recName->setToolTip (tr ("Record number of current point"));
  recName->setMinimumSize (recName->sizeHint ());
  recName->setMaximumSize (recName->sizeHint ());
  statBoxLayout->addWidget (recName, 2, 1, Qt::AlignLeft);

  recLabel = new QLabel (this);
  recLabel->setToolTip (tr ("Record number of current point"));
  recLabel->setAutoFillBackground (true);
  statBoxLayout->addWidget (recLabel, 2, 2, Qt::AlignLeft);

  QLabel *subName = new QLabel ("Sub:", this);
  subName->setToolTip (tr ("Subrecord number of current point"));
  subName->setMinimumSize (subName->sizeHint ());
  subName->setMaximumSize (subName->sizeHint ());
  statBoxLayout->addWidget (subName, 3, 1, Qt::AlignLeft);

  subLabel = new QLabel (this);
  subLabel->setToolTip (tr ("Subrecord number of current point"));
  subLabel->setAutoFillBackground (true);
  statBoxLayout->addWidget (subLabel, 3, 2, Qt::AlignLeft);

  QLabel *zName = new QLabel ("Z:", this);
  zName->setToolTip (tr ("Current point Z value"));
  zName->setMinimumSize (zName->sizeHint ());
  statBoxLayout->addWidget (zName, 4, 1, Qt::AlignLeft);

  zLabel = new QLabel ("0000.00", this);
  zLabel->setToolTip (tr ("Current point Z value"));
  zLabel->setMinimumSize (zLabel->sizeHint ());
  zLabel->setAutoFillBackground (true);
  zPalette = zLabel->palette ();
  statBoxLayout->addWidget (zLabel, 4, 2, Qt::AlignLeft);

  QLabel *exagName = new QLabel ("Exag:", this);
  exagName->setToolTip (tr ("Vertical exaggeration"));
  exagName->setMinimumSize (exagName->sizeHint ());
  statBoxLayout->addWidget (exagName, 5, 1, Qt::AlignLeft);

  exagLabel = new QLabel (this);
  exagLabel->setToolTip (tr ("Vertical exaggeration"));
  exagLabel->setAutoFillBackground (true);
  statBoxLayout->addWidget (exagLabel, 5, 2, Qt::AlignLeft);
  QString lbl = QString ("%L1").arg (options.exaggeration, 5, 'f', 3, QChar ('0'));
  exagLabel->setText (lbl);


  statBoxLayout->setColumnStretch (1, 1);
  statBoxLayout->setRowStretch (NUM_SCALE_LEVELS + 1, 0);


  leftBoxLayout->addWidget (statBox, 1);


  fileBarEnabled = NVFalse;


  QGroupBox *fileBox = new QGroupBox (tr ("File"));
  QHBoxLayout *fileBoxLayout = new QHBoxLayout ();
  fileBox->setLayout (fileBoxLayout);

  fileBar = new QScrollBar (Qt::Vertical);
  fileBar->setTracking (false);
  fileBar->setToolTip (tr ("Move through the file (Page Down or Page Up)"));
  fileBar->setWhatsThis (fileBarText);
  filePalette.setColor (QPalette::Normal, QPalette::Base, options.background_color);
  filePalette.setColor (QPalette::Normal, QPalette::Window, options.background_color);
  filePalette.setColor (QPalette::Inactive, QPalette::Base, options.background_color);
  filePalette.setColor (QPalette::Inactive, QPalette::Window, options.background_color);
  fileBar->setPalette (filePalette);
  connect (fileBar, SIGNAL (valueChanged (int)), this, SLOT (slotFileValueChanged (int)));
  fileBoxLayout->addWidget (fileBar);

  exagBar = new QScrollBar (Qt::Vertical);
  exagBar->setTracking (true);
  exagBar->setInvertedAppearance (true);
  exagBar->setToolTip (tr ("Vertical exaggeration"));
  exagBar->setWhatsThis (exagBarText);
  exagPalette.setColor (QPalette::Normal, QPalette::Base, options.background_color);
  exagPalette.setColor (QPalette::Normal, QPalette::Window, options.background_color);
  exagPalette.setColor (QPalette::Inactive, QPalette::Base, options.background_color);
  exagPalette.setColor (QPalette::Inactive, QPalette::Window, options.background_color);
  exagBar->setPalette (exagPalette);
  exagBar->setMinimum (100);
  exagBar->setMaximum (10000);
  exagBar->setSingleStep (100);
  exagBar->setPageStep (500);
  connect (exagBar, SIGNAL (actionTriggered (int)), this, SLOT (slotExagTriggered (int)));
  connect (exagBar, SIGNAL (sliderReleased ()), this, SLOT (slotExagReleased ()));


  QGroupBox *sliceBox = new QGroupBox (tr ("Slice"));
  QVBoxLayout *sliceBoxLayout = new QVBoxLayout ();
  sliceBox->setLayout (sliceBoxLayout);


  QHBoxLayout *barBoxLayout = new QHBoxLayout ();


  sliceBar = new QScrollBar (Qt::Vertical);
  sliceBar->setTracking (true);
  sliceBar->setInvertedAppearance (true);
  sliceBar->setToolTip (tr ("Slice through the data (<b>Up Arrow</b> or <b>Down Arrow</b>)"));
  sliceBar->setWhatsThis (sliceBarText);
  slicePalette.setColor (QPalette::Normal, QPalette::Base, options.background_color);
  slicePalette.setColor (QPalette::Normal, QPalette::Window, options.background_color);
  slicePalette.setColor (QPalette::Inactive, QPalette::Base, options.background_color);
  slicePalette.setColor (QPalette::Inactive, QPalette::Window, options.background_color);
  sliceBar->setPalette (slicePalette);
  connect (sliceBar, SIGNAL (actionTriggered (int)), this, SLOT (slotSliceTriggered (int)));
  barBoxLayout->addWidget (sliceBar);


  QVBoxLayout *ctrlBox = new QVBoxLayout ();

  transBar = new QScrollBar (Qt::Vertical);
  transBar->setTracking (true);
  transBar->setInvertedAppearance (true);
  transBar->setToolTip (tr ("Transparency of data outside of slice"));
  transBar->setWhatsThis (transText);
  transPalette.setColor (QPalette::Normal, QPalette::Base, options.background_color);
  transPalette.setColor (QPalette::Normal, QPalette::Window, options.background_color);
  transPalette.setColor (QPalette::Inactive, QPalette::Base, options.background_color);
  transPalette.setColor (QPalette::Inactive, QPalette::Window, options.background_color);
  transBar->setPalette (transPalette);
  transBar->setMinimum (0);
  transBar->setMaximum (MAX_TRANS_VALUE);
  transBar->setSingleStep (1);
  transBar->setPageStep (8);
  transBar->setValue (options.slice_alpha);
  connect (transBar, SIGNAL (actionTriggered (int)), this, SLOT (slotTransTriggered (int)));
  ctrlBox->addWidget (transBar, 1);


  transLabel = new QLabel ("000", this);
  transLabel->setAlignment (Qt::AlignCenter);
  transLabel->setMinimumSize (transLabel->sizeHint ());
  transLabel->setToolTip (tr ("Transparency value"));
  transLabel->setWhatsThis (transText);
  transLabel->setFrameStyle (QFrame::Panel | QFrame::Sunken);
  transLabel->setLineWidth (2);
  QString string = QString ("%L1").arg (options.slice_alpha, 2, 10, QChar ('0'));
  transLabel->setText (string);
  ctrlBox->addWidget (transLabel);


  sizeBar = new QScrollBar (Qt::Vertical);
  sizeBar->setTracking (true);
  sizeBar->setInvertedAppearance (true);
  sizeBar->setToolTip (tr ("Size of slice as percentage of area"));
  sizeBar->setWhatsThis (sizeText);
  sizePalette.setColor (QPalette::Normal, QPalette::Base, options.background_color);
  sizePalette.setColor (QPalette::Normal, QPalette::Window, options.background_color);
  sizePalette.setColor (QPalette::Inactive, QPalette::Base, options.background_color);
  sizePalette.setColor (QPalette::Inactive, QPalette::Window, options.background_color);
  sizeBar->setPalette (sizePalette);
  sizeBar->setMinimum (1);
  sizeBar->setMaximum (MAX_SLICE_SIZE);
  sizeBar->setSingleStep (1);
  sizeBar->setPageStep (5);
  sizeBar->setValue (options.slice_percent);
  connect (sizeBar, SIGNAL (actionTriggered (int)), this, SLOT (slotSizeTriggered (int)));
  ctrlBox->addWidget (sizeBar, 1);

  sizeLabel = new QLabel ("00", this);
  sizeLabel->setAlignment (Qt::AlignCenter);
  sizeLabel->setMinimumSize (sizeLabel->sizeHint ());
  sizeLabel->setToolTip (tr ("Slice size as a percentage"));
  sizeLabel->setWhatsThis (sizeText);
  sizeLabel->setFrameStyle (QFrame::Panel | QFrame::Sunken);
  sizeLabel->setLineWidth (2);
  sizeLabel->setNum (options.slice_percent);
  ctrlBox->addWidget (sizeLabel);

  barBoxLayout->addLayout (ctrlBox);

  sliceBoxLayout->addLayout (barBoxLayout);


  meterLabel = new QLabel ("00000.0", this);
  meterLabel->setAlignment (Qt::AlignCenter);
  meterLabel->setMinimumSize (meterLabel->sizeHint ());
  meterLabel->setToolTip (tr ("Slice size in meters"));
  meterLabel->setFrameStyle (QFrame::Panel | QFrame::Sunken);
  meterLabel->setLineWidth (2);
  meterLabel->setText ("");

  sliceBoxLayout->addWidget (meterLabel);


  QHBoxLayout *hBox = new QHBoxLayout ();
  hBox->addWidget (leftBox);
  hBox->addWidget (fileBox);
  hBox->addWidget (exagBar);
  hBox->addWidget (map, 1);
  hBox->addWidget (sliceBox);
  vBox->addLayout (hBox, 1);


  //  Set View toolbar button states based on saved options.  We don't set the shortcut for the first three because they wil be triggered
  //  from the shortcuts in the pulldown menu.

  bDisplayInvalid->setChecked (options.display_invalid);
  connect (bDisplayInvalid, SIGNAL (clicked ()), this, SLOT (slotDisplayInvalid ()));


  //  Set Edit toolbar button states based on saved options

  if (options.function == DELETE_POINT) bDeletePoint->setChecked (true);

  if (options.function == DELETE_RECTANGLE) bDeleteRect->setChecked (true);

  if (options.function == DELETE_POLYGON) bDeletePoly->setChecked (true);

  if (options.function == KEEP_POLYGON) bKeepPoly->setChecked (true);

  if (options.function == RESTORE_RECTANGLE) bRestoreRect->setChecked (true);

  if (options.function == RESTORE_POLYGON) bRestorePoly->setChecked (true);


  //  Set all of the button hotkey tooltips and shortcuts

  for (int32_t i = 0 ; i < HOTKEYS ; i++) slotPrefHotKeyChanged (i);


  //  Right click popup menu

  popupMenu = new QMenu (map);
  connect (popupMenu, SIGNAL (triggered (QAction *)), this, SLOT (slotPopupMenu (QAction *)));

  for (int32_t i = 0 ; i < NUMPOPUPS ; i++) popup[i] = popupMenu->addAction (("            "));

  popupMenu->addSeparator ();
  popupHelp = popupMenu->addAction (tr ("Help"));
  connect (popupHelp, SIGNAL (triggered ()), this, SLOT (slotPopupHelp ()));


  //  Setup the file menu (don't set shortcuts for these menu items because they're handled by the keypress event with the associated buttons)

  QAction *openAction = new QAction (tr ("Open"), this);
  connect (openAction, SIGNAL (triggered ()), this, SLOT (slotOpen ()));

  QAction *exitAction = new QAction (tr ("Quit"), this);
  connect (exitAction, SIGNAL (triggered ()), this, SLOT (slotQuit ()));

  QMenu *fileMenu = menuBar ()->addMenu (tr ("&File"));
  fileMenu->addAction (openAction);
  fileMenu->addAction (exitAction);


  //  Setup the Edit menu

  QAction *prefsAct = new QAction (tr ("&Preferences"), this);
  prefsAct->setShortcut (QKeySequence (tr ("Ctrl+P", "Preferences shortcut key")));
  prefsAct->setStatusTip (tr ("Change program preferences"));
  prefsAct->setWhatsThis (prefsText);
  connect (prefsAct, SIGNAL (triggered ()), this, SLOT (slotPrefs ()));


  QMenu *editMenu = menuBar ()->addMenu (tr ("&Edit"));
  editMenu->addAction (prefsAct);


  //  Setup the View menu

  messageAction = new QAction (tr ("Display Messages"), this);
  messageAction->setToolTip (tr ("Display error messages from prgram and sub-processes"));
  messageAction->setWhatsThis (messageText);
  connect (messageAction, SIGNAL (triggered ()), this, SLOT (slotMessage ()));

  QMenu *viewMenu = menuBar ()->addMenu (tr ("View"));
  viewMenu->addAction (messageAction);


  //  Setup the help menu.

  QAction *hotHelp = new QAction (tr ("&Hot Keys and GUI control"), this);
  hotHelp->setShortcut (QKeySequence (tr ("Ctrl+H", "Hot Keys shortcut key")));
  hotHelp->setStatusTip (tr ("Help on GUI control and hot keys (fixed and user modifiable)"));
  connect (hotHelp, SIGNAL (triggered ()), this, SLOT (slotHotkeyHelp ()));

  QAction *toolHelp = new QAction (tr ("&Tool bars"), this);
  toolHelp->setShortcut (QKeySequence (tr ("Ctrl+T", "Tool bars shortcut key")));
  toolHelp->setStatusTip (tr ("Help on tool bars"));
  connect (toolHelp, SIGNAL (triggered ()), this, SLOT (slotToolbarHelp ()));

  QAction *whatsThisAct = QWhatsThis::createAction (this);
  whatsThisAct->setIcon (QIcon (":/icons/contextHelp.png"));

  QAction *aboutAct = new QAction (tr ("&About"), this);
  aboutAct->setShortcut (QKeySequence (tr ("Ctrl+A", "About shortcut key")));
  aboutAct->setStatusTip (tr ("Information about fileEdit3D"));
  connect (aboutAct, SIGNAL (triggered ()), this, SLOT (about ()));

  QAction *acknowledgments = new QAction (tr ("A&cknowledgments"), this);
  acknowledgments->setShortcut (QKeySequence (tr ("Ctrl+c", "Acknowledgmenst shortcut key")));
  acknowledgments->setStatusTip (tr ("Information about supporting libraries"));
  connect (acknowledgments, SIGNAL (triggered ()), this, SLOT (slotAcknowledgments ()));

  QAction *aboutQtAct = new QAction (tr ("About Qt"), this);
  aboutQtAct->setStatusTip (tr ("Information about Qt"));
  connect (aboutQtAct, SIGNAL (triggered ()), this, SLOT (aboutQt ()));

  QMenu *helpMenu = menuBar ()->addMenu (tr ("&Help"));
  helpMenu->addAction (hotHelp);
  helpMenu->addAction (toolHelp);
  helpMenu->addAction (whatsThisAct);
  helpMenu->addSeparator ();
  helpMenu->addAction (aboutAct);
  helpMenu->addSeparator ();
  helpMenu->addAction (acknowledgments);
  helpMenu->addAction (aboutQtAct);


  setMainButtons (NVTrue);


  //  Set up the status bars.

  QGroupBox *fpBox = new QGroupBox ();
  QGridLayout *fpBoxLayout = new QGridLayout;
  fpBox->setLayout (fpBoxLayout);


  misc.statusProgLabel = new QLabel (this);
  misc.statusProgLabel->setFrameStyle (QFrame::StyledPanel | QFrame::Sunken);
  misc.statusProgLabel->setToolTip (tr ("Active mode"));
  misc.statusProgLabel->setWhatsThis (miscLabelText);
  misc.statusProgLabel->setAutoFillBackground (true);
  misc.statusProgPalette = misc.statusProgLabel->palette ();
  fpBoxLayout->addWidget (misc.statusProgLabel, 0, 0);


  //  Progress status bar

  misc.progStatus = new QStatusBar ();
  misc.progStatus->setSizeGripEnabled (false);
  misc.progStatus->setToolTip (tr ("Progress bar and label"));
  misc.progStatus->setWhatsThis (progStatusText);
  fpBoxLayout->addWidget (misc.progStatus, 0, 1);


  misc.statusProg = new QProgressBar (this);
  misc.statusProg->setRange (0, 100);
  misc.statusProg->setValue (0);
  misc.statusProg->setTextVisible (false);
  misc.progStatus->addWidget (misc.statusProg, 10);


  vBox->addWidget (fpBox);


  //  Set the tracking timer function.  This will track the cursor in other ABE program map windows.
  //  Also used to limit the number of points in the polygon functions.

  trackCursor = new QTimer (this);
  connect (trackCursor, SIGNAL (timeout ()), this, SLOT (slotTrackCursor ()));
  trackCursor->start (50);


  //  Set the blink timer (half second) function.

  blinkTimer = new QTimer (this);
  connect (blinkTimer, SIGNAL (timeout ()), this, SLOT (slotBlinkTimer ()));
  blinkTimer->start (500);


  setFunctionCursor (options.function);


  show ();


  if (!misc.file.isEmpty ())
    {
      open_file ();
    }
  else
    {
      slotOpen ();
    }
}



fileEdit3D::~fileEdit3D ()
{
}



void 
fileEdit3D::slotOpen ()
{
  QStringList files, filters;

  if (!QDir (options.input_dir).exists ()) options.input_dir = ".";
 
  io_data_close ();


  QFileDialog *fd = new QFileDialog (this, tr ("fileEdit3D Open Input File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options.input_dir);


  filters << "GSF (*.d[0-9][0-9] *.gsf)"
          << "HOF (*.hof)"
          << "TOF (*.tof)"
          << "CZMIL (*.cpf)"
          << "LAS (*.las *.LAS)"
          << tr ("All files (*)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (options.inputFilter);

  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      misc.file = files.at (0);


      if (misc.file.isEmpty())
        {
          QMessageBox::warning (this, tr ("Open input file"), tr ("A file must be selected!"));
          return;
        }


      QFileInfo fileInfo = QFileInfo (misc.file);
      this->setWindowTitle (QString ("fileEdit3D : ") + fileInfo.fileName ());


      options.input_dir = fd->directory ().absolutePath ();
      options.inputFilter = fd->selectedNameFilter ();


      open_file ();
    }
}



void 
fileEdit3D::open_file ()
{
  if (io_data_open (this, &misc, &options))
    {
      QString tmp;
      tmp.sprintf (" - %s", strerror (errno));
      tmp.prepend (misc.file);

      QMessageBox::warning (this, tr ("fileEdit3D: Error opening input file"), tmp);
      return;
    }


  if (misc.linked)
    {
      char file[512];
      strcpy (file, misc.file.toLatin1 ());
      changeFileRegisterABE (misc.abeRegister, misc.abe_register, abe_register_group, active_window_id, file);

      QString title = tr ("fileEdit3D (ABE group %1) : %2").arg (abe_register_group, 2, 10, QChar ('0')).arg (file);
      this->setWindowTitle (title);
    }


  fileBarEnabled = NVFalse;

  fileBar->setMinimum (misc.start_record);
  fileBar->setMaximum (misc.end_record);
  fileBar->setSingleStep (misc.num_records);
  fileBar->setPageStep (misc.num_records);
  fileBar->setValue (misc.start_record);

  fileBarEnabled = NVTrue;

  first_screen = NVTrue;


  map->setCursor (Qt::WaitCursor);
  map->setToolTip ("");
  qApp->processEvents ();


  if (record_num)
    {
      int32_t beg_rec = qMax (record_num - (misc.num_records / 2), misc.start_record);
      fileBar->setValue (beg_rec);
    }
  else
    {
      data.count = 0;

      while (!data.count)
        {
          get_buffer (&data, &misc, misc.start_record);

          if ((misc.start_record + misc.num_records) < misc.end_record) misc.start_record += misc.num_records;
        }

      QString cntText = tr ("Number of points: %L1").arg (data.count);
      numLabel->setText (cntText);
    }


  strcpy (misc.abe_share->nearest_filename, misc.file.toLatin1 ());


  file_opened = NVTrue;


  map->setBounds (data.bounds);
  trackMap->resetBounds (data.bounds);


  redrawMap (NVTrue);


  //  Get the map center for the reset view slot.

  map->getMapCenter (&misc.map_center_x, &misc.map_center_y, &misc.map_center_z);


  map->enableSignals ();


  setMainButtons (NVTrue);
}



void 
fileEdit3D::slotLink ()
{
  if (misc.linked)
    {
      //  Unregister this application.

      unregisterABE (misc.abeRegister, misc.abe_register, abe_register_group, active_window_id);


      abe_register_group = -1;


      bLink->setToolTip (tr ("Connect to other ABE applications"));
      bLink->setWhatsThis (linkText);
      bLink->setIcon (QIcon (":/icons/unlink.png"));


      //  Unset the link flag.

      misc.linked = NVFalse;


      //  Remove the group id from the title

      this->setWindowTitle ("fileEdit3D : " + misc.file);


      setMainButtons (NVTrue);
    }
  else
    {
      //  Get the ABE_REGISTER shared memory area.

      char file[512];
      strcpy (file, misc.file.toLatin1 ());
      registerABE *registerABEDialog = new registerABE (this, "fileEdit3D", active_window_id, file, &misc.abeRegister, &misc.abe_register);


      connect (registerABEDialog, SIGNAL (keySetSignal (int32_t)), this, SLOT (slotRegisterABEKeySet (int32_t)));
    }
}



void 
fileEdit3D::slotRegisterABEKeySet (int32_t key)
{
  if (key >= 0)
    {
      abe_register_group = key;


      QString title = tr ("fileEdit3D (ABE group %1) : %2").arg (abe_register_group, 2, 10, QChar ('0')).arg (misc.file);
      this->setWindowTitle (title);


      misc.linked = NVTrue;


      bLink->setToolTip (tr ("Disconnect from other ABE applications"));
      bLink->setWhatsThis (unlinkText);
      bLink->setIcon (QIcon (":/icons/link.png"));
    }
}



void 
fileEdit3D::setMainButtons (uint8_t enable)
{
  //  If we changed the icon size in the preferences dialog we need to resize all of the buttons.

  if (options.main_button_icon_size != prev_icon_size)
    {
      QSize mainButtonIconSize (options.main_button_icon_size, options.main_button_icon_size);

      for (int32_t i = 0 ; i < NUM_TOOLBARS ; i++)
        {
          toolBar[i]->setIconSize (mainButtonIconSize);
          toolBar[i]->adjustSize ();
        }

      prev_icon_size = options.main_button_icon_size;
    }


  bOpen->setEnabled (enable);
  bExit->setEnabled (enable);
  bReset->setEnabled (enable);


  //  Only enable UNDO if we saved an undo block.

  if (misc.undo_count)
    {
      bUndo->setEnabled (enable);
    }
  else
    {
      bUndo->setEnabled (false);
    }


  //  Only enable the Display Messages option if we have messages to display.

  if (enable && misc.messages->size ())
    {
      messageAction->setEnabled (enable);
    }
  else
    {
      messageAction->setEnabled (false);
    }


  bDisplayInvalid->setEnabled (enable);


  bPrefs->setEnabled (enable);


  //  Stop is always the inverse.

  bStop->setEnabled (!enable);


  //  If we're displaying invalid data we want to disable all editing functions so the users won't get
  //  confused (too late!) unless we're also flagging invalid data.  Restores are allowed when displaying
  //  invalid data since that sort of makes sense.

  if (options.display_invalid && options.flag_index != 2)
    {
      bDeletePoint->setEnabled (false);
      bDeleteRect->setEnabled (false);
      bDeletePoly->setEnabled (false);
      bKeepPoly->setEnabled (false);
      bHotkeyPoly->setEnabled (false);
      bFilter->setEnabled (false);
    }
  else
    {
      bDeletePoint->setEnabled (enable);
      bDeleteRect->setEnabled (enable);
      bDeletePoly->setEnabled (enable);
      bKeepPoly->setEnabled (enable);
      bHotkeyPoly->setEnabled (enable);
      bFilter->setEnabled (enable);
    }


  bRestoreRect->setEnabled (enable);
  bRestorePoly->setEnabled (enable);


  //  Not allowed to filter mask if we're slicing.

  if (misc.slice)
    {
      bFilterRectMask->setEnabled (false);
      bFilterPolyMask->setEnabled (false);

      QString string;
      if (misc.slice_meters < 10.0)
        {
          string = QString ("%L1").arg (misc.slice_meters, 0, 'f', 2);
        }
      else if (misc.slice_meters < 100.0)
        {
          string = QString ("%L1").arg (misc.slice_meters, 0, 'f', 1);
        }
      else
        {
          string = QString ("%1").arg (NINT (misc.slice_meters));
        }
      meterLabel->setText (string);
    }
  else
    {
      bFilterRectMask->setEnabled (enable);
      bFilterPolyMask->setEnabled (enable);
      meterLabel->setText ("");
    }
  bClearMasks->setEnabled (enable);

  bFlag->setIcon (flagIcon[options.flag_index]);
  bFlag->setEnabled (enable);


  if (!misc.hydro_lidar_present)
    {
      bLidarMonitor->setEnabled (false);
      bWaveformMonitor->setEnabled (false);
      bWaveMonitor->setEnabled (false);
    }
  else
    {
      bLidarMonitor->setEnabled (enable);
      bWaveformMonitor->setEnabled (enable);
      bWaveMonitor->setEnabled (enable);
    }


  if (!misc.gsf_present)
    {
      bGSFMonitor->setEnabled (false);
    }
  else
    {
      bGSFMonitor->setEnabled (enable);
    }


  if (!misc.lidar_present && !misc.las_present)
    {
      bChartsPic->setEnabled (false);
      bLidarMonitor->setEnabled (false);
    }
  else
    {
      //  We don't want to enable the chartsPic button for (only) LAS data but we do want to enable the lidarMonitor button.

      if (misc.lidar_present)
        {
          bChartsPic->setEnabled (enable);
        }
      else
        {
          bChartsPic->setEnabled (false);
        }

      bLidarMonitor->setEnabled (enable);
    }

  if (!misc.las_present)
    {
      bLASwaveMonitor->setEnabled (false);
    }
  else
    {
      bLASwaveMonitor->setEnabled (enable);
    }

  if (!enable || (enable && misc.highlight_count))
    {
      bClearHighlight->setEnabled (enable);
      bClearPoly->setEnabled (enable);
    }
  bHighlightPoly->setEnabled (enable);

  bMeasure->setEnabled (enable);
  bMaskInsideRect->setEnabled (enable);
  bMaskInsidePoly->setEnabled (enable);
  bMaskOutsideRect->setEnabled (enable);
  bMaskOutsidePoly->setEnabled (enable);

  if (!enable || misc.mask_active) bMaskReset->setEnabled (enable);
}



void 
fileEdit3D::setFunctionCursor (int32_t function)
{
  //  Default to point mode for the map.

  map->setMapMode (NVMAPGL_POINT_MODE);


  discardMovableObjects ();


  switch (function)
    {
    case DELETE_RECTANGLE:
      misc.statusProgLabel->setText (tr ("Rectangular delete mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::red);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (deleteRectCursor);
      bDeleteRect->setChecked (true);
      break;

    case RESTORE_RECTANGLE:
      misc.statusProgLabel->setText (tr ("Rectangular restore mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::green);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (restoreRectCursor);
      bRestoreRect->setChecked (true);
      break;

    case DELETE_POLYGON:
      misc.statusProgLabel->setText (tr ("Polygon delete mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::red);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (deletePolyCursor);
      bDeletePoly->setChecked (true);
      break;

    case RESTORE_POLYGON:
      misc.statusProgLabel->setText (tr ("Polygon restore mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::green);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (restorePolyCursor);
      bRestorePoly->setChecked (true);
      break;

    case KEEP_POLYGON:
      misc.statusProgLabel->setText (tr ("Delete outside polygon mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::yellow);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (keepPolyCursor);
      bKeepPoly->setChecked (true);
      break;

    case HOTKEY_POLYGON:
      if (misc.hotkey_poly_count)
        {
          misc.statusProgLabel->setText (tr ("Press a hot key"));
        }
      else
        {
          misc.statusProgLabel->setText (tr ("Polygon hot key mode"));
        }
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::yellow);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (hotkeyPolyCursor);
      bHotkeyPoly->setChecked (true);
      break;

    case DELETE_POINT:
      misc.statusProgLabel->setText (tr ("Delete subrecord/record mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::red);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (Qt::PointingHandCursor);
      bDeletePoint->setChecked (true);
      break;

    case NOOP:
      misc.statusProgLabel->setText (tr ("No Op mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::cyan);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (Qt::PointingHandCursor);
      break;

    case ROTATE:
      misc.statusProgLabel->setText (tr ("Rotate data"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::green);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (rotateCursor);
      break;

    case ZOOM:
      misc.statusProgLabel->setText (tr ("Zoom in/out"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::green);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (zoomCursor);
      break;

    case RECT_FILTER_MASK:
      misc.statusProgLabel->setText (tr ("Rectangle filter mask mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::cyan);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (filterMaskRectCursor);
      bFilterRectMask->setChecked (true);
      break;

    case POLY_FILTER_MASK:
      misc.statusProgLabel->setText (tr ("Polygon filter mask mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::cyan);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (filterMaskPolyCursor);
      bFilterPolyMask->setChecked (true);
      break;

    case MASK_INSIDE_RECTANGLE:
      misc.statusProgLabel->setText (tr ("Mask inside rectangle mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::cyan);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (maskCursor);
      break;

    case MASK_OUTSIDE_RECTANGLE:
      misc.statusProgLabel->setText (tr ("Mask outside rectangle mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::cyan);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (maskCursor);
      break;

    case MASK_INSIDE_POLYGON:
      misc.statusProgLabel->setText (tr ("Mask inside polygon mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::cyan);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (maskCursor);
      break;

    case MASK_OUTSIDE_POLYGON:
      misc.statusProgLabel->setText (tr ("Mask outside polygon mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::cyan);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (maskCursor);
      break;

    case MEASURE:
      misc.statusProgLabel->setText (tr ("Measuring mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::green);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (measureCursor);
      break;

    case HIGHLIGHT_POLYGON:
      misc.statusProgLabel->setText (tr ("Highlight points in polygon mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::cyan);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (highlightPolyCursor);
      bHighlightPoly->setChecked (true);
      break;

    case CLEAR_POLYGON:
      misc.statusProgLabel->setText (tr ("Clear highlights in polygon mode"));
      misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::cyan);
      misc.statusProgLabel->setPalette (misc.statusProgPalette);
      map->setCursor (clearPolyCursor);
      bClearPoly->setChecked (true);
      break;
    }
}



void 
fileEdit3D::discardMovableObjects ()
{
  map->closeMovingList (&mv_tracker);
  map->closeMovingList (&mv_measure_anchor);
  trackMap->closeMovingPath (&mv_2D_tracker);
  map->closeMovingList (&mv_measure);
  map->discardRubberbandRectangle (&rb_rectangle);
  map->discardRubberbandPolygon (&rb_polygon);
  map->discardRubberbandLine (&rb_measure);


  for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++) map->closeMovingList (&(multi_marker[i]));
}



void 
fileEdit3D::leftMouse (int32_t mouse_x, int32_t mouse_y, double lon __attribute__ ((unused)),
                      double lat __attribute__ ((unused)), double z __attribute__ ((unused)))
{
  int32_t *px, *py;

  //  If the popup menu is up discard this mouse press


  if (popup_active)
    {
      //  Flush the OpenGL scene so the menu will disperse

      map->flush ();


      popup_active = NVFalse;
      return;
    }


  switch (options.function)
    {
    case NOOP:
    case DELETE_POINT:
      break;
    
    case DELETE_RECTANGLE:
    case RESTORE_RECTANGLE:
    case MASK_INSIDE_RECTANGLE:
    case MASK_OUTSIDE_RECTANGLE:
    case RECT_FILTER_MASK:

      if (map->rubberbandRectangleIsActive (rb_rectangle))
        {
          map->closeRubberbandRectangle (rb_rectangle, mouse_x, mouse_y, &px, &py);


          //  Invalidate, restore, or mask the area.

          qApp->setOverrideCursor (Qt::WaitCursor);
          qApp->processEvents ();

          set_area (map, &options, &misc, &data, px, py, 4, 1);

          qApp->restoreOverrideCursor ();


          map->discardRubberbandRectangle (&rb_rectangle);


          if (options.function == MASK_INSIDE_RECTANGLE || options.function == MASK_OUTSIDE_RECTANGLE) misc.mask_active = NVTrue;

          if (options.function == RECT_FILTER_MASK)
            {
              misc.filter_mask = NVTrue;



              //  If the filter message box is up then we are filter masking after the filter so we want to modify the 
              //  current filter points to remove those in masked areas.  We set the index to -1 to indicate that we've 
              //  removed the filter point.  We used to re-run the filter but, with the advent of the hofWaveFilter and
              //  the Hockey Puck filter this became too time consuming.

              if (misc.filter_kill_count)
                {
                  for (int32_t i = 0 ; i < misc.filter_kill_count ; i++)
                    {
                      if (data.fmask[misc.filter_kill_list[i]]) misc.filter_kill_list[i] = -1;
                    }
                }
            }


          redrawMap (NVTrue);


          //  Even though we do this in redrawMap we need to do it again here because otherwise the damn markers disappear.
          //  It's OpenGL, I have no idea why.

          if (options.function == RECT_FILTER_MASK) overlayData (map, &options, &misc, &data);


          setFunctionCursor (options.function);
        }
      else
        {
          map->anchorRubberbandRectangle (&rb_rectangle, mouse_x, mouse_y, options.edit_color, 2, Qt::SolidLine);
        }
      break;

    case DELETE_POLYGON:
    case RESTORE_POLYGON:
    case KEEP_POLYGON:
    case HOTKEY_POLYGON:
    case MASK_INSIDE_POLYGON:
    case MASK_OUTSIDE_POLYGON:
    case POLY_FILTER_MASK:
    case HIGHLIGHT_POLYGON:
    case CLEAR_POLYGON:

      if (map->rubberbandPolygonIsActive (rb_polygon))
        {
          int32_t count;
          map->closeRubberbandPolygon (rb_polygon, mouse_x, mouse_y, &count, &px, &py);


          //  Invalidate or restore the area.

          qApp->setOverrideCursor (Qt::WaitCursor);
          qApp->processEvents ();

          switch (options.function)
            {
            case DELETE_POLYGON:
            case RESTORE_POLYGON:
            case HIGHLIGHT_POLYGON:
            case CLEAR_POLYGON:
              set_area (map, &options, &misc, &data, px, py, count, 0);
              break;

            case POLY_FILTER_MASK:
              set_area (map, &options, &misc, &data, px, py, count, 0);
              misc.filter_mask = NVTrue;


              //  If the filter message box is up then we are filter masking after the filter so we want to modify the 
              //  current filter points to remove those in masked areas.  We set the index to -1 to indicate that we've 
              //  removed the filter point.  We used to re-run the filter but, with the advent of the hofWaveFilter and
              //  the Hockey Puck filter this became too time consuming.

              if (misc.filter_kill_count)
                {
                  for (int32_t i = 0 ; i < misc.filter_kill_count ; i++)
                    {
                      if (data.fmask[misc.filter_kill_list[i]]) misc.filter_kill_list[i] = -1;
                    }
                }
              break;

            case MASK_INSIDE_POLYGON:
            case MASK_OUTSIDE_POLYGON:
              set_area (map, &options, &misc, &data, px, py, count, 0);
              misc.mask_active = NVTrue;
              break;

            case KEEP_POLYGON:
              keep_area (map, &options, &misc, &data, px, py, count);
              break;

            case HOTKEY_POLYGON:
              misc.hotkey_poly_count = count;


              //  We have to check because we might have freed this memory in ancillaryProg but, on
              //  the other hand, the user may have aborted the operation so we still might need to
              //  free it here.

              if (misc.hotkey_poly_x)
                {
                  free (misc.hotkey_poly_x);
                  free (misc.hotkey_poly_y);
                }

              misc.hotkey_poly_x = (int32_t *) malloc (count * sizeof (int32_t));
              misc.hotkey_poly_y = (int32_t *) malloc (count * sizeof (int32_t));

              if (misc.hotkey_poly_y == NULL)
                {
                  fprintf (stderr, "%s %s %s %d - hotkey_poly_y - %s\n", misc.progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
                  clean_exit (-1);
                }

              for (int32_t i = 0 ; i < count ; i++)
                {
                  misc.hotkey_poly_x[i] = px[i];
                  misc.hotkey_poly_y[i] = py[i];
                }

              //  Now we wait for the user to press a hotkey.  Take a look at keyPressEvent and 
              //  ancillaryProg.

              break;
            }

          qApp->restoreOverrideCursor ();

          prev_poly_x = -1;


          //  Don't redraw if we're setting up for a hotkey polygon

          if (options.function != HOTKEY_POLYGON)
            {
              map->discardRubberbandPolygon (&rb_polygon);
              redrawMap (NVTrue);


              //  Even though we do this in redrawMap we need to do it again here because otherwise the damn markers disappear.
              //  It's OpenGL, I have no idea why.

              if (options.function == POLY_FILTER_MASK) overlayData (map, &options, &misc, &data);
            }
        }
      else
        {
          map->anchorRubberbandPolygon (&rb_polygon, mouse_x, mouse_y, options.edit_color, LINE_WIDTH, NVTrue, Qt::SolidLine);
          prev_poly_x = -1;
        }
      break;


    case MEASURE:
      if (map->rubberbandLineIsActive (rb_measure))
        {
          map->discardRubberbandLine (&rb_measure);
          map->closeMovingList (&mv_measure_anchor);
          map->closeMovingList (&mv_measure);
          map->flush ();
        }
      else
        {
          if (mv_measure_anchor < 0)
            {
              map->setMovingList (&mv_measure_anchor, marker, 16, mouse_x, mouse_y, 0.0, 2, options.edit_color);
            }
          else
            {
              map->setMovingList (&mv_measure, marker, 16, mouse_x, mouse_y, 0.0, 2, options.edit_color);
            }


          //  Snap the start of the line to the nearest point

          int32_t pix_x, pix_y;
          map->get2DCoords (data.x[misc.nearest_point], data.y[misc.nearest_point], -data.z[misc.nearest_point], &pix_x, &pix_y);

          map->anchorRubberbandLine (&rb_measure, pix_x, pix_y, options.edit_color, LINE_WIDTH, Qt::SolidLine);
          line_anchor.x = data.x[misc.nearest_point];
          line_anchor.y = data.y[misc.nearest_point];
          line_anchor.z = data.z[misc.nearest_point];
        }
      break;
    }
}



void 
fileEdit3D::midMouse (int32_t mouse_x __attribute__ ((unused)), int32_t mouse_y __attribute__ ((unused)),
                     double lon __attribute__ ((unused)), double lat __attribute__ ((unused)),
                     double z __attribute__ ((unused)))
{
  switch (options.function)
    {
    case NOOP:
      break;

    case DELETE_POINT:
      if (misc.nearest_point != -1)
        {
          uint32_t rec = data.rec[misc.nearest_point];

          for (int32_t i = 0 ; i < data.count ; i++)
            {
              if (data.rec[i] == rec)
                {
                  store_undo (&misc, options.undo_levels, data.val[i], i);
                  data.val[i] |= PFM_MANUALLY_INVAL;
                }
            }
          end_undo_block (&misc);


          //
          //                      if we are frozen, automatically unfreeze
          //

          if (misc.marker_mode) misc.marker_mode = 0;
        }

      redrawMap (NVTrue);
      break;

    case DELETE_RECTANGLE:
    case RESTORE_RECTANGLE:
    case MASK_INSIDE_RECTANGLE:
    case MASK_OUTSIDE_RECTANGLE:
    case RECT_FILTER_MASK:
      map->discardRubberbandRectangle (&rb_rectangle);
      break;

    case DELETE_POLYGON:
    case RESTORE_POLYGON:
    case KEEP_POLYGON:
    case HOTKEY_POLYGON:
    case MASK_INSIDE_POLYGON:
    case MASK_OUTSIDE_POLYGON:
    case POLY_FILTER_MASK:
    case HIGHLIGHT_POLYGON:
    case CLEAR_POLYGON:
      map->discardRubberbandPolygon (&rb_polygon);
      prev_poly_x = -1;
      break;

    case MEASURE:
      map->discardRubberbandLine (&rb_measure);
      map->closeMovingList (&mv_measure_anchor);
      map->closeMovingList (&mv_measure);
      break;
    }
  setFunctionCursor (options.function);
}



void 
fileEdit3D::slotMouseDoubleClick (QMouseEvent *e __attribute__ ((unused)), double lon __attribute__ ((unused)),
                                 double lat __attribute__ ((unused)), double z __attribute__ ((unused)))
{
  //  Lock in nearest point so mouse moves won't change it if we want to run one of the ancillary programs.

  lock_point = misc.nearest_point;


  //  Flip the double_click flag.  The right-click menu sets this to NVTrue so it will flip to NVFalse.
  //  Left-click sets it to NVFalse so it will filp to NVTrue;

  double_click = !double_click;


  //  Double click is hard-wired to turn off slicing.

  if (misc.slice) slotPopupMenu (popup[NUMPOPUPS - 1]);
}



//  Freakin' menus!

void 
fileEdit3D::rightMouse (int32_t mouse_x, int32_t mouse_y, double lon, double lat, double z __attribute__ ((unused)))
{
  QString tmp;


  //  If we already have a context menu up, flush the OpenGL scene so the previous one will go away

  if (popup_active) map->flush ();


  menu_cursor_lon = lon;
  menu_cursor_lat = lat;
  menu_cursor_x = mouse_x;
  menu_cursor_y = mouse_y;


  QPoint pos (mouse_x, mouse_y);


  //  Lock in nearest point so mouse moves won't change it if we want to run one of the ancillary programs.

  lock_point = misc.nearest_point;


  //  Popups need global positioning

  QPoint global_pos = map->mapToGlobal (pos);


  //  In most cases popup[2] through popup[NUMPOPUPS-1] are not used so we set them to false.  If we need them for specific buttons
  //  we'll set them to true in the switch.

  popup[0]->setVisible (true);
  popup[1]->setVisible (true);
  for (int32_t i = 2 ; i < NUMPOPUPS ; i++) popup[i]->setVisible (false);


  switch (options.function)
    {
    case NOOP:
      break;

    case DELETE_POINT:

      //  If you move the "Delete point" option from popup[0] look through the code for calls to slotPopupMenu with popup[0] as the argument.

      tmp = tr ("Delete point [%L1]").arg (options.hotkey[DELETE_SINGLE_POINT]);
      popup[0]->setText (tmp);
      popup[1]->setText (tr ("Delete record (e.g. ping)"));
      popup[2]->setText (tr ("Highlight point"));
      popup[2]->setVisible (true);


      //  Don't allow point marking if we have filter points or we are flagging some data.

      if (!misc.filter_kill_count && !options.flag_index)
        {
          popup[2]->setEnabled (true);
        }
      else
        {
          popup[2]->setEnabled (false);
        }


      //  If you need to change the freeze/unfreeze popup position you need to change the FREEZE_POPUP value in fileEdit3DDef.hpp

      if (misc.marker_mode)
        {
          popup[FREEZE_POPUP]->setText (tr ("Unfreeze marker"));
        }
      else
        {
          popup[FREEZE_POPUP]->setText (tr ("Freeze marker"));
        }
      popup[FREEZE_POPUP]->setVisible (true);


      //  Check for the data type of the nearest point and add the possible ancillary programs

      switch (misc.data_type)
        {
        case PFM_GSF_DATA:
          popup[5]->setText (tr ("Run %1 [%2]").arg (options.name[EXAMGSF]).arg (options.hotkey[EXAMGSF]));
          popup[5]->setVisible (true);
          popup_prog[5] = EXAMGSF;
          break;
        }

      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case DELETE_RECTANGLE:
      popup[0]->setText (tr ("Close rectangle and delete subrecords (beams)"));
      popup[1]->setText (tr ("Close rectangle and delete records (pings)"));
      popup[2]->setText (tr ("Discard rectangle"));
      popup[3]->setVisible (true);
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case RESTORE_RECTANGLE:
      popup[0]->setText (tr ("Close rectangle and restore data"));
      popup[1]->setText (tr ("Discard rectangle"));
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case DELETE_POLYGON:
      popup[0]->setText (tr ("Close polygon and delete subrecords (beams)"));
      popup[1]->setText (tr ("Close polygon and delete records (pings)"));
      popup[2]->setText (tr ("Discard polygon"));
      popup[3]->setVisible (true);
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case KEEP_POLYGON:
      popup[0]->setText (tr ("Close polygon and delete subrecords (beams)"));
      popup[1]->setText (tr ("Discard polygon"));
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case RESTORE_POLYGON:
      popup[0]->setText (tr ("Close polygon and restore data"));
      popup[1]->setText (tr ("Discard polygon"));
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case HOTKEY_POLYGON:


      //  If we have a defined hotkey polygon, check the data type and activate the entries for
      //  possible ancillary programs.

      if (!misc.hotkey_poly_count)
        {
          popup[0]->setText (tr ("Close polygon (press hot key to run program)"));
          popup[1]->setText (tr ("Discard polygon"));
        }
      else
        {
          popup[0]->setVisible (false);
          popup[1]->setVisible (false);

          switch (misc.data_type)
            {
            case PFM_CHARTS_HOF_DATA:
              popup[5]->setText (tr ("Run %1 [%2]").arg (options.name[HOFRETURNKILL]).arg (options.hotkey[HOFRETURNKILL]));
              popup[5]->setVisible (true);
              popup_prog[5] = HOFRETURNKILL;
              popup[6]->setText (tr ("Run %1 [%2]").arg (options.name[HOFRETURNKILL_SWA]).arg (options.hotkey[HOFRETURNKILL_SWA]));
              popup[6]->setVisible (true);
              popup_prog[6] = HOFRETURNKILL_SWA;
              break;

            case PFM_SHOALS_TOF_DATA:
              break;
            }
        }

      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case RECT_FILTER_MASK:
    case POLY_FILTER_MASK:
      if (options.function == RECT_FILTER_MASK)
        {
          popup[0]->setText (tr ("Close rectangle and save mask"));
          popup[1]->setText (tr ("Discard rectangle"));
        }
      else
        {
          popup[0]->setText (tr ("Close polygon and save mask"));
          popup[1]->setText (tr ("Discard polygon"));
        }
      popup[2]->setText (tr ("Clear all masks"));
      if (options.deep_filter_only)
        {
          tmp = tr ("Set deep filter only (Yes)");
        }
      else
        {
          tmp = tr ("Set deep filter only (No)");
        }
      popup[3]->setText (tmp);
      popup[2]->setVisible (true);
      popup[3]->setVisible (true);
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case MASK_INSIDE_RECTANGLE:
    case MASK_OUTSIDE_RECTANGLE:
      popup[0]->setText (tr ("Close rectangle and mask data"));
      popup[1]->setText (tr ("Discard rectangle"));
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case MASK_INSIDE_POLYGON:
    case MASK_OUTSIDE_POLYGON:
      popup[0]->setText (tr ("Close polygon and mask data"));
      popup[1]->setText (tr ("Discard polygon"));
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case MEASURE:
      popup[0]->setVisible (false);
      popup[1]->setVisible (false);
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case HIGHLIGHT_POLYGON:
      popup[0]->setText (tr ("Close polygon and highlight points"));
      popup[1]->setText (tr ("Discard polygon"));
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;

    case CLEAR_POLYGON:
      popup[0]->setText (tr ("Close polygon and un-highlight points"));
      popup[1]->setText (tr ("Discard polygon"));
      popup_active = NVTrue;
      popupMenu->popup (global_pos);
      break;
    }

  if (misc.slice)
    {
      popup[NUMPOPUPS - 1]->setText (tr ("Turn off slicing"));
      popup[NUMPOPUPS - 1]->setVisible (true);
    }
}



//  IMPORTANT NOTE:  Never use misc.nearest_point in slotPopupMenu.  Use lock_point instead.  This is set when we right-click,
//  double-click, or press a key so that subsequent mouse movement will not affect what we were trying to do when we clicked or pressed.

void 
fileEdit3D::slotPopupMenu (QAction *ac)
{
  int32_t value;
  QStringList lst, items;
  QString res, item, boxSize;
  bool ok;


  void kill_records (nvMapGL *map, OPTIONS *options, MISC *misc, POINT_DATA *data, int32_t *rb, int32_t x, int32_t y);


  //  Nice little debug statement for the next time I add a popup slot and forget to change this code.

  if (NUMPOPUPS > 8)
    {
      QString msg = QString ("%1 %2 %3 - Someone tell Jan to fix this function because NUMPOPUPS has changed.").arg (__FILE__).arg (__FUNCTION__).arg
        (__LINE__);
      misc.messages->append (msg);
    }


  popup_active = NVFalse;


  //  Clear the context menu by flushing the OpenGL scene

  map->flush ();


  //  Each of the following popup slots will execute whatever was set up in rightMouse above.  Instead of 
  //  commenting each piece and then having to change it each time I move something I'll let the rightMouse
  //  function serve as documentation for each piece.

  if (ac == popup[0])
    {
      switch (options.function)
        {
        case NOOP:
          break;

        case DELETE_POINT:
          if (lock_point != -1)
            {
              //
              //                      if we are frozen, automatically unfreeze
              //

              if (misc.marker_mode) misc.marker_mode = 0;


              //  Single point kills are a real waste of undo ;-)

              store_undo (&misc, options.undo_levels, data.val[lock_point], lock_point);
              data.val[lock_point] = PFM_MANUALLY_INVAL;
              end_undo_block (&misc);

              redrawMap (NVTrue);
            }

          break;

        case DELETE_RECTANGLE:
        case RESTORE_RECTANGLE:
        case DELETE_POLYGON:
        case RESTORE_POLYGON:
        case KEEP_POLYGON:
        case HOTKEY_POLYGON:
        case MASK_INSIDE_RECTANGLE:
        case MASK_OUTSIDE_RECTANGLE:
        case MASK_INSIDE_POLYGON:
        case MASK_OUTSIDE_POLYGON:
        case RECT_FILTER_MASK:
        case POLY_FILTER_MASK:
        case HIGHLIGHT_POLYGON:
        case CLEAR_POLYGON:
          leftMouse (menu_cursor_x, menu_cursor_y, menu_cursor_lon, menu_cursor_lat, menu_cursor_z);
          break;
        }
    }
  else if (ac == popup[1])
    {
      switch (options.function)
        {
        case DELETE_RECTANGLE:
          kill_records (map, &options, &misc, &data, &rb_rectangle, menu_cursor_x, menu_cursor_y);

          redrawMap (NVTrue);

          setFunctionCursor (options.function);
          break;

        case DELETE_POLYGON:
          kill_records (map, &options, &misc, &data, &rb_polygon, menu_cursor_x, menu_cursor_y);

          redrawMap (NVTrue);

          setFunctionCursor (options.function);
          break;

        default:
          midMouse (menu_cursor_x, menu_cursor_y, menu_cursor_lon, menu_cursor_lat, menu_cursor_z);
          break;
        }
    }
  else if (ac == popup[2])
    {
      switch (options.function)
        {
        case DELETE_POINT:
          try
            {
              misc.highlight.resize (misc.highlight_count + 1);
            }
          catch (std::bad_alloc&)
            {
              fprintf (stderr, "%s %s %s %d - highlight - %s\n", misc.progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
              exit (-1);
            }

          misc.highlight[misc.highlight_count] = lock_point;
          misc.highlight_count++;

          bClearHighlight->setEnabled (true);

          redrawMap (NVFalse);
          break;

        case DELETE_RECTANGLE:
        case DELETE_POLYGON:
          midMouse (menu_cursor_x, menu_cursor_y, menu_cursor_lon, menu_cursor_lat, menu_cursor_z);
          break;

        case RECT_FILTER_MASK:
        case POLY_FILTER_MASK:
          slotClearFilterMasks ();
          break;
        }
    }
  else if (ac == popup[3])
    {
      switch (options.function)
        {
        case RECT_FILTER_MASK:
        case POLY_FILTER_MASK:
          value = 0;
          if (!options.deep_filter_only) value = 1;

          lst << tr ("Yes") << tr ("No");

          res = QInputDialog::getItem (this, "fileEdit3D", tr ("Deep filter only:"), lst, value, false, &ok);

          if (ok)
            {
              if (res.contains (tr ("Yes")))
                {
                  options.deep_filter_only = NVTrue;
                }
              else
                {
                  options.deep_filter_only = NVFalse;
                }
            }
          break;


          //  This slot is called when the Freeze/Unfreeze action has been clicked. It will set the proper text for the right
          //  context menu based on whether we are in freeze mode or not.  If we are unfreezing, we'll set the context menu
          //  text to be Freeze Point and close the focus cursor moving path for the overplot as it will be handled in
          //  slotMouseMove.  If we are freezing, we put the Unfreeze Point text on the context menu and indicate that the focus
          //  cursor is present as a legitimate multi-waveform

        case DELETE_POINT:
        case DELETE_RECTANGLE:
        case DELETE_POLYGON:

          //  Save the marker position just in case we are freezing the marker.

          misc.frozen_point = lock_point;


          if (misc.marker_mode)
            {
              //  Just in case waveMonitor is running.

              misc.abe_share->mwShare.multiPresent[0] = -1;

              misc.marker_mode = 0;
            }
          else
            {
              //  Just in case waveMonitor is running.

              misc.abe_share->mwShare.multiPresent[0] = 0;

              misc.marker_mode = 1;
            }
          break;
        }
    }
  else if (ac == popup[4])
    {
      switch (options.function)
        {
        case RECT_FILTER_MASK:
        case POLY_FILTER_MASK:
          break;
        }
    }
  else if (ac == popup[5])
    {
      runPopupProg (5);
    }
  else if (ac == popup[6])
    {
      runPopupProg (6);
    }
  else if (ac == popup[NUMPOPUPS - 1])
    {
      //  Turn off slicing if it is enabled.

      misc.slice = NVFalse;

      misc.slice_min = misc.ortho_min;
      sliceBar->setValue (misc.ortho_min);


      //  Turn on depth testing in the map widget

      map->setDepthTest (NVTrue);


      redrawMap (NVFalse);
    }
}



void 
fileEdit3D::slotPopupHelp ()
{
  switch (options.function)
    {
    case DELETE_RECTANGLE:
      QWhatsThis::showText (QCursor::pos ( ), deleteRectText, map);
      break;

    case RESTORE_RECTANGLE:
      QWhatsThis::showText (QCursor::pos ( ), restoreRectText, map);
      break;

    case DELETE_POINT:
      QWhatsThis::showText (QCursor::pos ( ), deletePointText, map);
      break;

    case DELETE_POLYGON:
      QWhatsThis::showText (QCursor::pos ( ), deletePolyText, map);
      break;

    case RESTORE_POLYGON:
      QWhatsThis::showText (QCursor::pos ( ), restorePolyText, map);
      break;

    case KEEP_POLYGON:
      QWhatsThis::showText (QCursor::pos ( ), keepPolyText, map);
      break;

    case HOTKEY_POLYGON:
      QWhatsThis::showText (QCursor::pos ( ), hotkeyPolyText, map);
      break;

    case RECT_FILTER_MASK:
      QWhatsThis::showText (QCursor::pos ( ), filterRectMaskText, map);
      break;

    case POLY_FILTER_MASK:
      QWhatsThis::showText (QCursor::pos ( ), filterPolyMaskText, map);
      break;

    case MASK_INSIDE_RECTANGLE:
      QWhatsThis::showText (QCursor::pos ( ), maskInsideRectText, map);
      break;

    case MASK_OUTSIDE_RECTANGLE:
      QWhatsThis::showText (QCursor::pos ( ), maskOutsideRectText, map);
      break;

    case MASK_INSIDE_POLYGON:
      QWhatsThis::showText (QCursor::pos ( ), maskInsidePolyText, map);
      break;

    case MASK_OUTSIDE_POLYGON:
      QWhatsThis::showText (QCursor::pos ( ), maskOutsidePolyText, map);
      break;

    case MEASURE:
      QWhatsThis::showText (QCursor::pos ( ), measureText, map);
      break;

    case HIGHLIGHT_POLYGON:
      QWhatsThis::showText (QCursor::pos ( ), highlightPolyText, map);
      break;

    case CLEAR_POLYGON:
      QWhatsThis::showText (QCursor::pos ( ), clearPolyText, map);
      break;
    }

  if (options.function != DELETE_POINT) midMouse (menu_cursor_x, menu_cursor_y, menu_cursor_lon, menu_cursor_lat, menu_cursor_z);
}



void 
fileEdit3D::runPopupProg (int32_t prog)
{
  if (options.function == HOTKEY_POLYGON && misc.hotkey_poly_count)
    {
      uint8_t failed = NVFalse;
      hkp = new hotkeyPolygon (this, map, &data, &options, &misc, popup_prog[prog], &failed);
      if (failed)
        {
          redrawMap (NVTrue);
        }
      else
        {
          connect (hkp, SIGNAL (hotkeyPolygonDone ()), this, SLOT (slotHotkeyPolygonDone ()));
          connect (hkp, SIGNAL (readStandardErrorSignal (QProcess *)), this, SLOT (slotReadStandardError (QProcess *)));
        }
    }
  else
    {
      runProg (popup_prog[prog]);
    }
}



//  Timer - timeout signal.  Very much like an X workproc.  This tracks the cursor in associated
//  programs, not in this window.  This is active whenever the mouse leaves this window.

void
fileEdit3D::slotTrackCursor ()
{
  char                  ltstring[25], lnstring[25];
  QString               string;
  static NV_F64_COORD2  prev_xy;


  //  This is sort of ancillary to the track cursor function.  What we want to do is monitor the number of error messages in the
  //  error message QStringList and when it changes pop up the messages dialog.  This way was a lot easier than implementing signals
  //  and slots or trying to call parent methods from children.

  if (misc.messages->size () != num_messages)
    {
      num_messages = misc.messages->size ();
      displayMessage ();
      if (num_messages) messageAction->setEnabled (true);
    }


  if (misc.drawing) return;


  //  If we haven't frozen the marker(s) and we're not in this window, erase the marker(s).

  if (!misc.marker_mode && misc.abe_share->active_window_id != active_window_id)
    {
      for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++)
        {
          if (multi_marker[i] >= 0) map->closeMovingList (&(multi_marker[i]));                 
        }
    }


  //
  //                      if we've closed the waveMonitor module and we were not in single waveform mode,
  //                      close the moving paths of all the multiple waveforms and switch to single waveform
  //                      mode so we will only display our lone cursor in fileEdit3D.
  //

  if (!misc.abe_share->mwShare.waveMonitorRunning && !misc.abe_share->mwShare.multiMode)
    {
      for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++) map->closeMovingList (&(multi_marker[i]));
      misc.abe_share->mwShare.multiMode = 1;
    }


  NV_F64_COORD2 xy;
  xy.y = misc.abe_share->cursor_position.y;
  xy.x = misc.abe_share->cursor_position.x;

  if (misc.abe_share->active_window_id != active_window_id && xy.y >= misc.displayed_area.min_y && xy.y <= misc.displayed_area.max_y && 
      xy.x >= misc.displayed_area.min_x && xy.x <= misc.displayed_area.max_x && xy.y != prev_xy.y && xy.x != prev_xy.x)
    {
      //  Find the nearest point to the cursor's location in whatever window has the focus.

      double min_dist = 999999999.0;
      int32_t hit = -1;
      for (int32_t i = 0 ; i < data.count ; i++)
        {
          double dist = sqrt (((xy.y - data.y[i]) * (xy.y - data.y[i])) + ((xy.x - data.x[i]) * (xy.x - data.x[i])));

          if (dist < min_dist)
            {
              min_dist = dist;
              hit = i;
            }
        }


      if (hit >= 0)
        {
          //  First set the 3D tracker.

          int32_t pix_x, pix_y;
          map->get2DCoords (data.x[hit], data.y[hit], -data.z[hit], &pix_x, &pix_y);
          map->setMovingList (&mv_tracker, marker, 16, pix_x, pix_y, 0.0, 2, options.tracker_color);


          //  Then set the 2D tracker.

          QBrush b1;
          trackMap->setMovingPath (&mv_2D_tracker, tracker_2D, data.x[hit], data.y[hit], 2, options.tracker_color,
                                   b1, NVFalse, Qt::SolidLine);
        }


      double deg, min, sec;
      char       hem;

      strcpy (ltstring, fixpos (xy.y, &deg, &min, &sec, &hem, POS_LAT, options.position_form));
      strcpy (lnstring, fixpos (xy.x, &deg, &min, &sec, &hem, POS_LON, options.position_form));

      latLabel->setText (ltstring);
      lonLabel->setText (lnstring);
    }
  prev_xy = xy;
}



//  Blink timer slot.

void
fileEdit3D::slotBlinkTimer ()
{
  static int32_t     blink = 0;


  //  We want to blink the mask reset button (every half_second) if we have a mask set so that the user will know.

  if (misc.mask_active)
    {
      if (blink)
        {
          bMaskReset->setIcon (QIcon (":/icons/mask_reset.png"));
          blink = 0;
        }
      else
        {
          bMaskReset->setIcon (QIcon (":/icons/mask_reset_red.png"));
          blink = 1;
        }

      qApp->processEvents ();
    }


  //  Check to see if a child process (that was started from a button) has been killed externally.

  if (misc.abe_share->killed)
    {
      for (int32_t i = 0 ; i < NUMPROGS ; i++)
        {
          if (misc.abe_share->killed == options.kill_switch[i])
            {
              options.progButton[i]->setChecked (false);

              misc.abe_share->killed = 0;
            }
        }
    }


  //  Another silly timer thing.  If the filter slider move counter has exceeded 1 (i.e. 1 to 1.5 seconds since last Shift+wheel move),
  //  we want to trigger the filter and reset the counter to -1.

  if (filter_active && filterMessage_slider_count >= 0) filterMessage_slider_count++;

  if (filter_active && filterMessage_slider_count > 1)
    {
      filterMessage_slider_count = -1;

      slotFilter ();
    }
}



//  Mouse press signal from the map class.

void 
fileEdit3D::slotMousePress (QMouseEvent *e, double lon, double lat, double z)
{
  //  If we're drawing and a mouse button is pressed we want to quit drawing.

  if (misc.drawing) 
    {
      misc.drawing_canceled = NVTrue;
      return;
    }


  if (e->button () == Qt::LeftButton)
    {
      //  Check for the control key modifier.  If it's set, we want to rotate the image.

      if (e->modifiers () == Qt::ControlModifier)
        {
	  //  Get the cursor position at this time.

          start_ctrl_x = e->x ();
          start_ctrl_y = e->y ();

          misc.save_function = options.function;
          options.function = ROTATE;
          setFunctionCursor (options.function);
        }
      else
        {
          if (options.function == DELETE_POINT)
            {
              if (lock_point != -1)
                {
                  //  Check for the shift key modifier.  If it's set, we want to delete the point.

                  if (e->modifiers () == Qt::ShiftModifier)
                    {
                      //  If we are frozen, automatically unfreeze

                      if (misc.marker_mode) misc.marker_mode = 0;

                      store_undo (&misc, options.undo_levels, data.val[misc.nearest_point], misc.nearest_point);
                      data.val[misc.nearest_point] = PFM_MANUALLY_INVAL;
                      end_undo_block (&misc);
                    }
                }
            }

          leftMouse (e->x (), e->y (), lon, lat, z);
        }
    }

  if (e->button () == Qt::MidButton)
    {
      //  Check for the control key modifier.  If it's set, we want to center on the cursor.

      if (e->modifiers () == Qt::ControlModifier)
        {
          map->setMapCenter (lon, lat, z);

          overlayData (map, &options, &misc, &data);
        }
      else
        {
          midMouse (e->x (), e->y (), lon, lat, z);
        }
    }

  if (e->button () == Qt::RightButton)
    {
      //  Check for the control key modifier.  If it's set, we want to zoom based on cursor movement.

      if (e->modifiers () == Qt::ControlModifier)
        {
          start_ctrl_y = e->y ();

          misc.save_function = options.function;
          options.function = ZOOM;
          setFunctionCursor (options.function);
        }
      else
        {
          rightMouse (e->x (), e->y (), lon, lat, z);
        }
    }
}



//  Mouse release signal from the map class.

void 
fileEdit3D::slotMouseRelease (QMouseEvent *e, double lon __attribute__ ((unused)),
                           double lat __attribute__ ((unused)), double z __attribute__ ((unused)))
{
  if (e->button () == Qt::LeftButton)
    {
      if (options.function == ROTATE)
        {
          options.function = misc.save_function;
          setFunctionCursor (options.function);

          overlayData (map, &options, &misc, &data);
        }
      else
        {
          popup_active = NVFalse;
        }
    }

  if (e->button () == Qt::MidButton) popup_active = NVFalse;

  if (e->button () == Qt::RightButton)
    {
      if (options.function == ZOOM)
        {
          options.function = misc.save_function;
          setFunctionCursor (options.function);

          overlayData (map, &options, &misc, &data);
        }
    }
}



void 
fileEdit3D::geo_z_label (double lat, double lon, double z)
{
  char ltstring[25], lnstring[25], hem;
  QString geo_string, val_string;
  double deg, min, sec;
  static float prev_z_factor = 1.0, prev_z_offset = 0.0;


  strcpy (ltstring, fixpos (lat, &deg, &min, &sec, &hem, POS_LAT, options.position_form));
  strcpy (lnstring, fixpos (lon, &deg, &min, &sec, &hem, POS_LON, options.position_form));

  latLabel->setText (ltstring);
  lonLabel->setText (lnstring);


  val_string = QString ("%L1").arg (z * options.z_factor + options.z_offset, 0, 'f', 2);


  //  Set yellow background for scaled or offset data.

  if ((options.z_factor != 1.0 && prev_z_factor == 1.0) || (options.z_offset != 0.0 && prev_z_offset == 0.0))
    {
      zPalette.setColor (QPalette::Normal, QPalette::Window, Qt::yellow);
      zLabel->setPalette (zPalette);
      zLabel->setToolTip (tr ("Scaled/offset current point Z value"));
      zLabel->setWhatsThis (tr ("Scaled/offset current point Z value"));
    }
  else if ((options.z_factor == 1.0 && prev_z_factor != 1.0) || (options.z_offset == 0.0 && prev_z_offset != 0.0))
    {
      zPalette.setColor (QPalette::Normal, QPalette::Window, misc.widgetBackgroundColor);
      zLabel->setPalette (zPalette);
      zLabel->setToolTip (tr ("Current point Z value"));
      zLabel->setWhatsThis (tr ("Current point Z value"));
    }
  zLabel->setText (val_string);

  prev_z_factor = options.z_factor;
  prev_z_offset = options.z_offset;
}



//  Mouse wheel signal from the map class.

void
fileEdit3D::slotWheel (QWheelEvent *e, double lon __attribute__ ((unused)), double lat __attribute__ ((unused)),
                      double z __attribute__ ((unused)))
{
  if (e->delta () > 0)
    {
      //  Zoom in or out when pressing the Ctrl key and using the mouse wheel.

      if (e->modifiers () == Qt::ControlModifier)
        {
          map->zoomInPercent ();

          overlayData (map, &options, &misc, &data);
        }


      //  Change the filter dialog standard deviation when pressing the Shift key and using the mouse wheel.

      else if (e->modifiers () == Qt::ShiftModifier && filter_active)
        {
          switch (options.filter_type)
            {
            case AREA_BASED_FILTER:
              filterMessage_slider_count = 0;
              stdSlider->triggerAction (QAbstractSlider::SliderSingleStepSub);
              break;

            case HP_FILTER:
              break;
            }
        }


      //  Otherwise we're slicing.

      else
        {
          slotSliceTriggered (QAbstractSlider::SliderSingleStepSub);
        }
    }
  else
    {
      //  Zoom in or out when pressing the Ctrl key and using the mouse wheel.

      if (e->modifiers () == Qt::ControlModifier)
        {
          map->zoomOutPercent ();

          overlayData (map, &options, &misc, &data);
        }


      //  Change the filter dialog standard deviation when pressing the Shift key and using the mouse wheel.

      else if (e->modifiers () == Qt::ShiftModifier && filter_active)
        {
          switch (options.filter_type)
            {
            case AREA_BASED_FILTER:
              filterMessage_slider_count = 0;
              stdSlider->triggerAction (QAbstractSlider::SliderSingleStepAdd);
              break;

            case HP_FILTER:
              break;
            }
        }


      //  Otherwise we're slicing.

      else
        {
          slotSliceTriggered (QAbstractSlider::SliderSingleStepAdd);
        }
    }
}



//  Mouse move signal from the map class.

void
fileEdit3D::slotMouseMove (QMouseEvent *e, double lon, double lat, double z, NVMAPGL_DEF l_mapdef)
{
  NV_I32_COORD2             xy;
  NV_F64_COORD3             hot = {999.0, 999.0, 999.0};
  static NV_I32_COORD2      prev_xy = {-1, -1};
  static NV_F64_COORD3      prev_hot;
  double                    dist;
  static int32_t            prev_nearest_point = -1;


  void get_nearest_kill_point (MISC *misc, POINT_DATA *data, double lat, double lon, NV_F64_COORD3 *hot);
  uint8_t compare_to_stack (int32_t current_point, double dist, MISC *misc);


  //  Let other ABE programs know which window we're in.

  misc.abe_share->active_window_id = active_window_id;


  //  If it's still drawing don't do anything

  if (misc.drawing) return;


  //  Get rid of the tracking cursor from slotTrackCursor ().  But only if it already exists, otherwise we
  //  will be creating a new one (which we don't want to do).

  if (mv_tracker >= 0) map->closeMovingList (&mv_tracker);
  if (mv_2D_tracker >= 0) trackMap->closeMovingPath (&mv_2D_tracker);


  xy.x = e->x ();
  xy.y = e->y ();


  //  Track the cursor (not the marker) position for other ABE programs but only if we're in plan view

  misc.abe_share->cursor_position.y = lat;
  misc.abe_share->cursor_position.x = lon;
  misc.abe_share->cursor_position.z = z;


  //  Only process if the pointer position has changed pixels.

  if (xy.x == prev_xy.x && xy.y == prev_xy.y) return;


  //  If we've frozen the central cursor or all of the cursors we want to draw them regardless of the mode we're in.

  if (misc.marker_mode)
    {
      //  If we're in multicursor mode, draw all the cursors.

      if (misc.marker_mode == 2)
        {
          DrawMultiCursors ();
        }
      else
        { 
          int32_t pix_x, pix_y;
          map->get2DCoords (data.x[misc.frozen_point], data.y[misc.frozen_point], -data.z[misc.frozen_point], &pix_x, &pix_y);

          map->setMovingList (&(multi_marker[0]), marker, 16, pix_x, pix_y, 0.0, 2, misc.abe_share->mwShare.multiColors[0]);
        }
    }


  QString rec_string = "", sub_string = "", distance_string = "", h_string = "", v_string = "";
  double distance, azimuth;
  char nearest_line[512];


  switch (options.function)
    {
    case ROTATE:
      if (start_ctrl_x != xy.x || start_ctrl_y != xy.y)
	{
	  int32_t diff_x = xy.x - start_ctrl_x;


	  if (diff_x)
            {
              //  Flip the sign if we are above the center point looking at the top of the surface or below
              //  the center point looking at the bottom.  This allows the Y rotation from the mouse click/drag
              //  to be more intuitive.

              double zxrot = map->getZXRotation ();
              int32_t half = l_mapdef.draw_height / 2;


	      //  If we're within 15 degrees of flat rotate normally for the front side.

	      if ((zxrot < 15.0 && zxrot > -15.0) || (zxrot > 345.0) || (zxrot < -345.0))
		{
		  //  Don't do anything.
		}


	      //  Reverse rotate reverse for the back side.

	      else if ((zxrot > -195.0 && zxrot < -165.0) || (zxrot < 195.0 && zxrot > 165.0))
		{
		  diff_x = -diff_x;
		}


	      //  Otherwise, check to see which side is up.

	      else
		{
		  if ((xy.y < half && ((zxrot > 0.0 && zxrot < 180.0) || (zxrot < -180.0))) ||
		      (xy.y > half && ((zxrot < 0.0 && zxrot > -180.0) || (zxrot > 180.0)))) diff_x = -diff_x;
		}

              map->rotateY ((double) diff_x / 5.0);
            }

	  int32_t diff_y = xy.y - start_ctrl_y;

	  if (diff_y) map->rotateZX ((double) diff_y / 5.0);

	  start_ctrl_x = xy.x;
	  start_ctrl_y = xy.y;
	}
      break;

    case ZOOM:
      if (start_ctrl_y != xy.y)
	{
	  int32_t diff_y = xy.y - start_ctrl_y;


	  if (diff_y < -10)
            {
              map->zoomInPercent ();
              start_ctrl_y = xy.y;
            }
          else if (diff_y > 10)
            {
              map->zoomOutPercent ();
              start_ctrl_y = xy.y;
            }
	}
      break;

    case NOOP:
    case DELETE_POINT:
    case MEASURE:

      //  Draw the frozen multi_markers.

      if (misc.marker_mode == 2)
        {
          //  If we're in multicursor mode, draw all the cursors.

          if (!misc.abe_share->mwShare.multiMode)
            {
              DrawMultiCursors ();
            }
          else
            {
              int32_t pix_x, pix_y;
              map->get2DCoords (data.x[misc.frozen_point], data.y[misc.frozen_point], -data.z[misc.frozen_point], &pix_x, &pix_y);

              map->setMovingList (&(multi_marker[0]), marker, 16, pix_x, pix_y, 0.0, 2, misc.abe_share->mwShare.multiColors[0]);
            }
        }
      else
        {
          //  Clear the nearest point stack.

          for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++)
            {
              misc.nearest_stack.point[i] = -1;
              misc.nearest_stack.dist[i] = 9999999999.0;
            }


          misc.nearest_point = -1;


          //  If we've got filter kill points on screen we want to find the nearest kill point first and lock it in before finding
          //  the nearest stack points.  This allows us to snap to only kill points but then get the surrounding valid points
          //  whether they're kill points or not.  This will set the nearest_point, the hot.x and hot.y positions, and the stack[0]
          //  point and distance (forced to -999.0 so it can't be replaced).

          get_nearest_kill_point (&misc, &data, lat, lon, &hot);


          //  Find the valid point nearest (geographically) to the cursor.

          for (int32_t i = 0 ; i < data.count ; i++)
            {
              //  Do not use invalid points unless the display_invalid flag is set.
              //  Do not use masked points. Do not check points that are not on the display.

              if (!check_bounds (&options, &misc, &data, i, NVTrue, misc.slice))
                {
                  dist = sqrt ((double) ((lat - data.y[i]) * (lat - data.y[i])) + (double) ((lon - data.x[i]) * (lon - data.x[i])));


                  //  Check the points against the points in the nearest points stack.

                  if (compare_to_stack (i, dist, &misc))
                    {
                      //  If the return was true then this is the minimum distance so far.

                      misc.nearest_point = misc.nearest_stack.point[0];

                      hot.x = data.x[i];
                      hot.y = data.y[i];
                      hot.z = data.z[i];
                    }
                }
            }


          //  Only track the nearest position if we haven't frozen the marker.

          if (!misc.marker_mode)
            {
              misc.abe_share->cursor_position.y = data.y[misc.nearest_point];
              misc.abe_share->cursor_position.x = data.x[misc.nearest_point];
            }


          //  Update the status bars

          // 
          //                      if our point has moved or we have just selected a new overplot we will go
          //                      into this code block.
          //

          if (misc.nearest_point != -1 && misc.nearest_point != prev_nearest_point)
            {
              //  Show distance and azimuth to last highlighted point in status bar

              if (misc.highlight_count)
                {
                  invgp (NV_A0, NV_B0, data.y[misc.highlight[misc.highlight_count - 1]], data.x[misc.highlight[misc.highlight_count - 1]],
                         data.y[misc.nearest_point], data.x[misc.nearest_point], &distance, &azimuth);

                  if (distance < 0.01) azimuth = 0.0;

                  distance_string = tr ("Distance : %L1 (m)  Azimuth : %L2 (degrees)").arg (distance, 0, 'f', 2).arg (azimuth, 0, 'f', 2);

                  misc.statusProgLabel->setToolTip (tr ("Distance and azimuth from last highlighted point"));
                  misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::cyan);
                  misc.statusProgLabel->setPalette (misc.statusProgPalette);
                  misc.statusProgLabel->setText (distance_string);
                  misc.statusProgLabel->setVisible (true);
                }
              else
                {
                  misc.statusProgLabel->setToolTip ("");
                }


              //  Locking shared memory.

              misc.abeShare->lock ();


              //  Only change the nearest filename if we aren't locked.  External programs monitor
              //  abe_share->nearest_point to trigger updates of their windows.  They use
              //  abe_share->mwShare.multiRecord[0] for the current record to display.  In this way
              //  we can lock the main screen by not updating abe_share->mwShare.multiRecord[0] and still
              //  trigger the external programs to update (such as for multiple waveforms in waveMonitor or
              //  CZMILwaveMonitor).  When we do this we don't want nearest_filename or nearest_line to change
              //  either since that is where the external programs get the filename to use with 
              //  abe_share->mwShare.multiRecord[0].

              if (!misc.marker_mode) strcpy (nearest_line, misc.file.toLatin1 ());


              //  Push the nearest stack points into the ABE shared memory for use by waveMonitor, CZMILwaveMonitor,
              //  and lidarMonitor.

              for (int32_t j = 0 ; j < MAX_STACK_POINTS ; j++)
                {
                  //  Normal, unfrozen mode.

                  if (!misc.marker_mode)
                    {
                      misc.abe_share->mwShare.multiSubrecord[j] = data.sub[misc.nearest_stack.point[j]];
                      misc.abe_share->mwShare.multiPoint[j].x = data.x[misc.nearest_stack.point[j]];
                      misc.abe_share->mwShare.multiPoint[j].y = data.y[misc.nearest_stack.point[j]];
                      misc.abe_share->mwShare.multiPoint[j].z = data.z[misc.nearest_stack.point[j]];
                      misc.abe_share->mwShare.multiLocation[j] = misc.nearest_stack.point[j];
                      misc.abe_share->mwShare.multiType[j] = misc.data_type;
                      misc.abe_share->mwShare.multiPresent[j] = data.rec[misc.nearest_stack.point[j]];
                      misc.abe_share->mwShare.multiRecord[j] = data.rec[misc.nearest_stack.point[j]];
                    }


                  //  If we're frozen, we don't want to replace the [0] record and we want to push the nearest
                  //  MAX_STACK_POINTS - 1 records up one level.

                  else
                    {
                      if (j)
                        {
                          misc.abe_share->mwShare.multiSubrecord[j] = data.sub[misc.nearest_stack.point[j - 1]];
                          misc.abe_share->mwShare.multiPoint[j].x = data.x[misc.nearest_stack.point[j - 1]];
                          misc.abe_share->mwShare.multiPoint[j].y = data.y[misc.nearest_stack.point[j - 1]];
                          misc.abe_share->mwShare.multiPoint[j].z = data.z[misc.nearest_stack.point[j - 1]];
                          misc.abe_share->mwShare.multiLocation[j] = misc.nearest_stack.point[j - 1];
                          misc.abe_share->mwShare.multiType[j] = misc.data_type;
                          misc.abe_share->mwShare.multiPresent[j] = data.rec[misc.nearest_stack.point[j - 1]];
                          misc.abe_share->mwShare.multiRecord[j] = data.rec[misc.nearest_stack.point[j - 1]];
                        }
                    }
                }
              misc.abe_share->mwShare.multiNum = MAX_STACK_POINTS;


              //  Trigger some of the external applications to update.

              misc.abe_share->nearest_point = misc.nearest_point;


              //  Unlock shared memory.

              misc.abeShare->unlock ();


              geo_z_label (data.y[misc.nearest_point],  data.x[misc.nearest_point], data.z[misc.nearest_point]);


              rec_string = QString ("%L1").arg (misc.abe_share->mwShare.multiRecord[0]);
              sub_string = QString ("%L1").arg (misc.abe_share->mwShare.multiSubrecord[0]);

              recLabel->setText (rec_string);
              subLabel->setText (sub_string);
            }


          //  Set the delete, set, unset point marker.

          if (!misc.abe_share->mwShare.multiMode && (options.function == DELETE_POINT || options.function == NOOP))
            {
              DrawMultiCursors ();
            }
          else
            {
              //  If we're frozen, use the frozen point for the marker position.

              if (misc.marker_mode)
                {
                  hot.x = data.x[misc.frozen_point];
                  hot.y = data.y[misc.frozen_point];
                  hot.z = data.z[misc.frozen_point];
                }

              int32_t pix_x, pix_y;
              map->get2DCoords (hot.x, hot.y, -hot.z, &pix_x, &pix_y);

              map->setMovingList (&(multi_marker[0]), marker, 16, pix_x, pix_y, 0.0, 2, 
                                  misc.abe_share->mwShare.multiColors[0]);
            }


          if (options.function == MEASURE)
            {
              if (map->rubberbandLineIsActive (rb_measure))
                {
                  //  Snap the end of the line to the nearest point

                  int32_t pix_x, pix_y;
                  map->get2DCoords (data.x[misc.nearest_point], data.y[misc.nearest_point], -data.z[misc.nearest_point],
                                    &pix_x, &pix_y);

                  map->dragRubberbandLine (rb_measure, pix_x, pix_y);


                  invgp (NV_A0, NV_B0, data.y[misc.nearest_point], data.x[misc.nearest_point],
                         line_anchor.y, line_anchor.x, &distance, &azimuth);

                  if (distance < 0.01) azimuth = 0.0;

                  distance_string = tr ("Dist: %L1 (m)   Az: %L2 (degrees)  deltaZ: %L3").arg (distance, 0, 'f', 2).arg (azimuth, 0, 'f', 2).arg 
                    (line_anchor.z - data.z[misc.nearest_point], 0, 'f', 2);

                  misc.statusProgLabel->setToolTip (tr ("Distance and azimuth from last highlighted point"));
                  misc.statusProgPalette.setColor (QPalette::Normal, QPalette::Window, Qt::cyan);
                  misc.statusProgLabel->setPalette (misc.statusProgPalette);
                  misc.statusProgLabel->setText (distance_string);
                  misc.statusProgLabel->setVisible (true);
                }
            }
        }


      //  Set the previous nearest point positions.
             
      prev_hot = hot;
      break;


    case DELETE_RECTANGLE:
    case RESTORE_RECTANGLE:
    case MASK_INSIDE_RECTANGLE:
    case MASK_OUTSIDE_RECTANGLE:
    case RECT_FILTER_MASK:
      if (map->rubberbandRectangleIsActive (rb_rectangle)) map->dragRubberbandRectangle (rb_rectangle, xy.x, xy.y);

      geo_z_label (lat, lon, z);
      recLabel->setText (rec_string);
      subLabel->setText (sub_string);
      break;

    case DELETE_POLYGON:
    case RESTORE_POLYGON:
    case KEEP_POLYGON:
    case HOTKEY_POLYGON:
    case MASK_INSIDE_POLYGON:
    case MASK_OUTSIDE_POLYGON:
    case POLY_FILTER_MASK:
    case HIGHLIGHT_POLYGON:
    case CLEAR_POLYGON:

      if (map->rubberbandPolygonIsActive (rb_polygon))
        {
          if (!polygon_flip)
            {
              polygon_flip = NVTrue;

              if (prev_poly_x > -1)
                {
                  map->vertexRubberbandPolygon (rb_polygon, prev_poly_x, prev_poly_y);
                }
              else
                {
                  map->vertexRubberbandPolygon (rb_polygon, xy.x, xy.y);
                }
            }
          else
            {
              map->dragRubberbandPolygon (rb_polygon, xy.x, xy.y);
              prev_poly_x = xy.x;
              prev_poly_y = xy.y;
              polygon_flip = NVFalse;
            }
        }

      geo_z_label (lat, lon, z);
      recLabel->setText (rec_string);
      subLabel->setText (sub_string);

      break;
    }


  //  Track the cursor in the 2D tracker box.

  QBrush b1;
  trackMap->setMovingPath (&mv_2D_tracker, tracker_2D, lon, lat, 2, options.tracker_color, b1, NVFalse, Qt::SolidLine);


  //  Set the previous cursor.

  prev_xy = xy;

  prev_nearest_point = misc.nearest_point;
}



//
//                      DrawMultiCursors
//
//                      This method will loop through either the valid overplots or the 
//                      nearest neighbors, and place the multi-cursor on the proper shot
//                      whether we are in a normal top-down view or a sliced view.
//

void 
fileEdit3D::DrawMultiCursors ()
{
  uint8_t withinSlice;                     //       boolean check to see if shot is within current volumetric slice

  uint8_t init2D = NVTrue;
  for (int32_t i = MAX_STACK_POINTS - 1 ; i >= 0 ; i--) 
    {
      withinSlice = NVTrue;
     
      if ((misc.abe_share->mwShare.multiPoint[i].x >= misc.displayed_area.min_x) && 
          (misc.abe_share->mwShare.multiPoint[i].x <= misc.displayed_area.max_x) && 
          (misc.abe_share->mwShare.multiPoint[i].y >= misc.displayed_area.min_y) && 
          (misc.abe_share->mwShare.multiPoint[i].y <= misc.displayed_area.max_y) &&
          (misc.abe_share->mwShare.multiPresent[i] != -1) && (withinSlice))
        {
          int32_t pix_x, pix_y;
          map->get2DCoords (misc.abe_share->mwShare.multiPoint[i].x, misc.abe_share->mwShare.multiPoint[i].y, -misc.abe_share->mwShare.multiPoint[i].z,
                            &pix_x, &pix_y, &init2D);
          map->setMovingList (&(multi_marker[i]), marker, 16, pix_x, pix_y, 0.0, 2, misc.abe_share->mwShare.multiColors[i]);
        }
      else
        {
          map->closeMovingList (&(multi_marker[i]));
        }
    }
}



/*!
  Resize signal from the map class.  With Qt 4.6.2 this seems to be redundant (and can cause all kinds of problems) so
  this is just a stub at the moment.
*/

void
fileEdit3D::slotResize (QResizeEvent *e __attribute__ ((unused)))
{
#if QT_VERSION < 0x040602
  redrawMap (NVTrue);
#endif
}



//  This is where we load all of our data in to OpenGL display lists (in nvMapGL.cpp).

void
fileEdit3D::redrawMap (uint8_t redraw2D)
{
  void setScale (float actual_min, float actual_max, MISC *misc);
  uint8_t compute_bounds (nvMapGL *map, OPTIONS *options, MISC *misc, POINT_DATA *data, int32_t *viewable_count, NV_F64_XYMBC *bounds);


  //  If we don't have a file opened we don't want to try to draw anything.

  if (!file_opened) return;


  misc.drawing = NVTrue;


  qApp->setOverrideCursor (Qt::WaitCursor);
  qApp->processEvents ();


  discardMovableObjects ();


  //  Erase all text from the map.

  map->clearText ();


  //  Erase all objects (like spheres) from the map.

  map->clearDisplayLists ();


  //  Erase all data points from the map.

  map->clearDataPoints ();


  setMainButtons (NVFalse);


  NVMAPGL_DEF l_mapdef = map->getMapdef ();


  misc.draw_area_height = l_mapdef.draw_height;
  misc.draw_area_width = l_mapdef.draw_width;

  map->setCursor (Qt::WaitCursor);

  misc.drawing_canceled = NVFalse;


  //  Compute the Z range of the data.

  int32_t viewable_count;

  bounds = data.bounds;
  bounds.min_z = CHRTRNULL;
  bounds.max_z = -CHRTRNULL;


  //  Compute the min and max values for defining the 3D space and optionally for coloring.

  uint8_t bounds_changed = compute_bounds (map, &options, &misc, &data, &viewable_count, &bounds);


  //  Only display if there are viewable points.

  if (viewable_count)
    {
      setScale (bounds.min_z, bounds.max_z, &misc);


      range_x = bounds.max_x - bounds.min_x;
      range_y = bounds.max_y - bounds.min_y;
      range_z = bounds.max_z - bounds.min_z;


      //  Add 10 percent to the X, Y, and Z ranges.

      bounds.min_x -= (range_x * 0.10);
      bounds.max_x += (range_x * 0.10);

      bounds.min_y -= (range_y * 0.10);
      bounds.max_y += (range_y * 0.10);

      bounds.min_z -= (range_z * 0.10);
      bounds.max_z += (range_z * 0.10);
      range_z = bounds.max_z - bounds.min_z;


      if (bounds_changed)
        {
          map->setBounds (bounds);
          trackMap->resetBounds (bounds);
        }


      if (!misc.slice) compute_ortho_values (map, &misc, &options, &data, sliceBar, NVTrue);


      int32_t c_index = -1;
      uint8_t save_min[4];
      float z_value;


      for (int32_t i = 0 ; i < data.count ; i++)
        {
          //  Check against the viewing area.

          int32_t trans = 0;
          if ((trans = check_bounds (&options, &misc, &data, i, NVTrue, misc.slice)) < 2)
            {
              z_value = data.z[i];


              //  Color by depth

              c_index = NINT ((NUMSHADES - 1) - ((-data.z[i]) - bounds.min_z) / range_z * (float) (NUMSHADES - 1));


              uint8_t tmp[4];


              //  If the color index was out of range, use white.

              if (c_index < 0)
                {
                  tmp[0] = 255;
                  tmp[1] = 255;
                  tmp[2] = 255;
                  tmp[3] = 255;
                }
              else
                {
                  //  If the point is outside the slice (transparent) and the gray option is set...

                  if (trans && options.slice_gray)
                    {
                      tmp[0] = 127;
                      tmp[1] = 127;
                      tmp[2] = 127;
                      tmp[3] = options.slice_alpha;
                    }
                  else
                    {
                      memcpy (tmp, misc.color_array[trans][c_index], 4);
                    }
                }

              map->setDataPoints (data.x[i], data.y[i], -z_value, tmp, 0, NVFalse);
            }
        }


      map->setDataPoints (0.0, 0.0, 0.0, save_min, options.point_size, NVTrue);


      if (!misc.drawing_canceled)
        {
          overlayData (map, &options, &misc, &data);
          overlayFlag (map, &options, &misc, &data);


          //  If we have highlighted or filter kill points set the map tooltip so the user knows what to do.

          if (options.highlight_tip && (misc.highlight_count || misc.filter_kill_count))
            {
              map->setToolTip (tr ("<p style='white-space:pre'>Press <b>Del</b> to delete points or <b>Ins</b> to clear points")); 
            }
          else
            {
              map->setToolTip ("");
            } 
        }
    }


  //  We want to flush the map just in case we didn't have any points to draw.

  map->flush ();


  //  Redraw the 2D tracker unless this redraw was caused by an operation that has no effect on the 2D map.

  trackMap->enableSignals ();
  if (redraw2D) trackMap->redrawMap (NVTrue);


  misc.drawing = NVFalse;

  setMainButtons (NVTrue);

  setFunctionCursor (options.function);

  qApp->restoreOverrideCursor ();
}



void 
fileEdit3D::slotMaskReset ()
{
  for (int32_t i = 0 ; i < data.count ; i++) data.mask[i] = NVFalse;
  misc.mask_active = NVFalse;
  bMaskReset->setIcon (QIcon (":/icons/mask_reset.png"));


  //  Turn off slicing if it was on (this will also call redrawMap ()).

  slotPopupMenu (popup[NUMPOPUPS - 1]);
}



void 
fileEdit3D::slotReset ()
{
  misc.poly_count = 0;

  misc.slice = NVFalse;

  misc.slice_min = misc.ortho_min;
  sliceBar->setValue (misc.ortho_min);


  //  Turn on depth testing in the map widget

  map->setDepthTest (NVTrue);


  discardMovableObjects ();


  map->setMapCenter (misc.map_center_x, misc.map_center_y, misc.map_center_z);


  map->resetPOV ();


  redrawMap (NVFalse);
}



void 
fileEdit3D::slotFileValueChanged (int value)
{
  if (fileBarEnabled)
    {
      map->setCursor (Qt::WaitCursor);
      qApp->processEvents ();


      //  Clear out the undo memory since we've moved on to another buffer.

      for (int32_t i = 0 ; i < misc.undo_count ; i++)
        {
          if (misc.undo[i].count)
            {
              misc.undo[i].val.clear ();
              misc.undo[i].num.clear ();
            }
        }
      misc.undo_count = 0;


      get_buffer (&data, &misc, value);
      if (!data.count)
        {
          if (direction)
            {
              if (value + misc.num_records < misc.end_record) fileBar->setValue (value + misc.num_records);
            }
          else
            {
              if (value - misc.num_records > misc.start_record) fileBar->setValue (value - misc.num_records);
            }
        }


      QString cntText = tr ("Number of points: %L1").arg (data.count);
      numLabel->setText (cntText);


      map->setBounds (data.bounds);
      trackMap->resetBounds (data.bounds);


      //  Turn off slicing.

      misc.slice = NVFalse;

      misc.slice_min = misc.ortho_min;
      sliceBar->setValue (misc.ortho_min);

      redrawMap (NVTrue);


      //  Get the map center for the reset view slot.

      map->getMapCenter (&misc.map_center_x, &misc.map_center_y, &misc.map_center_z);
    }
}



void 
fileEdit3D::slotSliceTriggered (int action)
{
  if (!misc.drawing)
    {
      //  Have to compute the ortho values and set the slider to minimum before we slice for the first time.

      if (!misc.slice) compute_ortho_values (map, &misc, &options, &data, sliceBar, NVTrue);


      misc.slice = NVTrue;
      map->setDepthTest (NVFalse);

      switch (action)
        {
        case QAbstractSlider::SliderSingleStepAdd:
        case QAbstractSlider::SliderPageStepAdd:
          misc.slice_min -= misc.slice_size;
          misc.slice_max = misc.slice_min + misc.slice_size;
          break;

        case QAbstractSlider::SliderSingleStepSub:
        case QAbstractSlider::SliderPageStepSub:
          misc.slice_max += misc.slice_size;
          misc.slice_min = misc.slice_max - misc.slice_size;
          break;

        case QAbstractSlider::SliderMove:
          misc.slice_min = sliceBar->value ();
          misc.slice_max = misc.slice_min + misc.slice_size;
          break;
        }


      if (misc.slice_min < misc.ortho_min)
        {
          misc.slice_min = misc.ortho_min;
          misc.slice_max = misc.ortho_min + misc.slice_size;
        }

      if (action != QAbstractSlider::SliderMove) sliceBar->setValue (misc.slice_min);

      slicing = NVTrue;
      redrawMap (NVFalse);
      slicing = NVFalse;


      //  Even though we do this in redrawMap we need to do it again here because otherwise the damn markers disappear.
      //  It's OpenGL, I have no idea why.

      overlayData (map, &options, &misc, &data);
    }
}



void 
fileEdit3D::slotSizeTriggered (int action)
{
  if (!misc.drawing)
    {
      switch (action)
        {
        case QAbstractSlider::SliderSingleStepAdd:
        case QAbstractSlider::SliderPageStepAdd:
          options.slice_percent--;
          if (options.slice_percent <= 0) options.slice_percent = 1;
          break;

        case QAbstractSlider::SliderSingleStepSub:
        case QAbstractSlider::SliderPageStepSub:
          options.slice_percent++;
          if (options.slice_percent > MAX_SLICE_SIZE) options.slice_percent = MAX_SLICE_SIZE;
          break;

        case QAbstractSlider::SliderMove:
          options.slice_percent = sizeBar->value ();
          break;
        }


      if (action != QAbstractSlider::SliderMove) sizeBar->setValue (options.slice_percent);


      int32_t val = sliceBar->value ();

      sizeLabel->setNum (options.slice_percent);

      compute_ortho_values (map, &misc, &options, &data, sliceBar, NVFalse);
      map->setDepthTest (NVFalse);

      misc.slice_min = val;
      misc.slice_max = misc.slice_min + misc.slice_size;

      if (misc.slice_max > misc.ortho_max)
        {
          misc.slice_min = misc.ortho_max - misc.slice_size;
          misc.slice_max = misc.ortho_max;
        }

      slicing = NVTrue;
      redrawMap (NVFalse);
      slicing = NVFalse;


      //  Even though we do this in redrawMap we need to do it again here because otherwise the damn markers disappear.
      //  It's OpenGL, I have no idea why.

      overlayData (map, &options, &misc, &data);
    }
}



void 
fileEdit3D::slotTransTriggered (int action)
{
  if (!misc.drawing)
    {
      switch (action)
        {
        case QAbstractSlider::SliderSingleStepAdd:
        case QAbstractSlider::SliderPageStepAdd:
          options.slice_alpha--;
          if (options.slice_alpha < 0) options.slice_alpha = 1;
          break;

        case QAbstractSlider::SliderSingleStepSub:
        case QAbstractSlider::SliderPageStepSub:
          options.slice_alpha++;
          if (options.slice_alpha > MAX_TRANS_VALUE) options.slice_alpha = MAX_TRANS_VALUE;
          break;

        case QAbstractSlider::SliderMove:
          options.slice_alpha = transBar->value ();
          break;
        }


      if (action != QAbstractSlider::SliderMove) transBar->setValue (options.slice_alpha);


      //  Set the semi-transparent colors for slicing.

      float hue_inc = 315.0 / (float) (NUMSHADES + 1);

      for (int32_t j = 0 ; j < NUMSHADES ; j++)
	{
	  QColor color;
	  color.setHsv ((int32_t) (j * hue_inc), 255, 255, 255);
	  misc.color_array[0][j][0] = misc.color_array[1][j][0] = color.red ();
	  misc.color_array[0][j][1] = misc.color_array[1][j][1] = color.green ();
	  misc.color_array[0][j][2] = misc.color_array[1][j][2] = color.blue ();
	  misc.color_array[0][j][3] = color.alpha ();

	  misc.color_array[1][j][3] = options.slice_alpha;
	}


      QString string = QString ("%1").arg (options.slice_alpha, 0, 10, QChar ('0'));
      transLabel->setText (string);

      redrawMap (NVFalse);
    }
}



void 
fileEdit3D::slotExagTriggered (int action)
{
  if (!misc.drawing)
    {
      switch (action)
        {
        case QAbstractSlider::SliderSingleStepAdd:
          if (options.exaggeration >= 1.0)
            {
              options.exaggeration -= 1.0;

              if (options.exaggeration < 1.0) options.exaggeration = 1.0;

              map->setExaggeration (options.exaggeration);

              redrawMap (NVFalse);
            }
          break;

        case QAbstractSlider::SliderPageStepAdd:
          if (options.exaggeration >= 5.0)
            {
              options.exaggeration -= 5.0;

              if (options.exaggeration < 1.0) options.exaggeration = 1.0;

              map->setExaggeration (options.exaggeration);

              redrawMap (NVFalse);
            }
          break;

        case QAbstractSlider::SliderSingleStepSub:
          options.exaggeration += 1.0;
          if (options.exaggeration > 100.0) options.exaggeration = 100.0;
          map->setExaggeration (options.exaggeration);

          redrawMap (NVFalse);

          break;

        case QAbstractSlider::SliderPageStepSub:
          options.exaggeration += 5.0;
          if (options.exaggeration > 100.0) options.exaggeration = 100.0;
          map->setExaggeration (options.exaggeration);

          redrawMap (NVFalse);

          break;

        case QAbstractSlider::SliderMove:
          QString lbl = QString ("%L1").arg ((float) exagBar->value () / 100.0, 5, 'f', 3, QChar ('0'));
          exagLabel->setText (lbl);
          break;
        }


      //  Even though we do this in redrawMap we need to do it again here because otherwise the damn markers disappear.
      //  It's OpenGL, I have no idea why.

      overlayData (map, &options, &misc, &data);
    }
}



void 
fileEdit3D::slotExagReleased ()
{
  options.exaggeration = (float) exagBar->value () / 100.0;
  map->setExaggeration (options.exaggeration);

  redrawMap (NVFalse);
}



void 
fileEdit3D::slotExaggerationChanged (float exaggeration, float apparent_exaggeration)
{
  if (exaggeration < 1.0)
    {
      exagBar->setEnabled (false);
    }
  else
    {
      exagBar->setEnabled (true);
    }

  QString lbl = QString ("%L1").arg (apparent_exaggeration, 5, 'f', 3, QChar ('0'));
  exagLabel->setText (lbl);

  options.exaggeration = exaggeration;

  disconnect (exagBar, SIGNAL (actionTriggered (int)), this, SLOT (slotExagTriggered (int)));
  exagBar->setValue (NINT (options.exaggeration * 100.0));
  connect (exagBar, SIGNAL (actionTriggered (int)), this, SLOT (slotExagTriggered (int)));
}



void
fileEdit3D::keyPressEvent (QKeyEvent *e)
{
  void actionKey (fileEdit3D *parent, nvMapGL *map, POINT_DATA *data, OPTIONS *options, MISC *misc, QString key, int32_t lock_point);


  //  Lock in nearest point so mouse moves won't change it.

  lock_point = misc.nearest_point;


  //  If we're drawing and a key is pressed we want to cancel drawing.

  if (misc.drawing)
    {
      misc.drawing_canceled = NVTrue;
      return;
    }


  //  First check for the hard-wired keys.

  switch (e->key ())
    {
    case Qt::Key_Escape:

      //  Turn off slicing if it was on (this will also call redrawMap ()).

      if (misc.slice) slotPopupMenu (popup[NUMPOPUPS - 1]);

      return;
      break;

    case Qt::Key_Return:
      return;
      break;

    case Qt::Key_Left:
      map->rotateY (-options.rotation_increment);
      return;
      break;

    case Qt::Key_PageUp:
      if (e->modifiers () == Qt::ControlModifier)
        {
          slotExagTriggered (QAbstractSlider::SliderSingleStepSub);
        }
      else
        {
          fileBar->triggerAction (QAbstractSlider::SliderPageStepSub);
        }

      return;
      break;

    case Qt::Key_Right:
      map->rotateY (options.rotation_increment);
      return;
      break;

    case Qt::Key_PageDown:
      if (e->modifiers () == Qt::ControlModifier)
        {
          slotExagTriggered (QAbstractSlider::SliderSingleStepAdd);
        }
      else
        {
          fileBar->triggerAction (QAbstractSlider::SliderPageStepAdd);
        }

      return;
      break;

    case Qt::Key_Up:

      if (e->modifiers () == Qt::ControlModifier)
        {
          map->rotateZX (-options.rotation_increment);
        }
      else
        {
          slotSliceTriggered (QAbstractSlider::SliderSingleStepSub);
        }
      return;
      break;

    case Qt::Key_Down:

      if (e->modifiers () == Qt::ControlModifier)
        {
          map->rotateZX (options.rotation_increment);
        }
      else
        {
          //  If we're not already slicing, do nothing.

          if (misc.slice) slotSliceTriggered (QAbstractSlider::SliderSingleStepAdd);
        }
      return;
      break;
    }


  //  Looking for ancillary program hot keys and action keys.

  uint8_t hit = NVFalse;
  QString key = e->text ();


  //  Get the keystring.  We need this for Del, Ins, Home, or End if we want to use them for ancillary program hot keys.

  QKeySequence keySeq = e->key ();
  QString keyString = keySeq.toString ();
  if (keyString == "Del" || keyString == "Ins" || keyString == "Home" || keyString == "End") key = keyString;


  //  If key is NULL this is a modifier so we don't want to check it against the hot/action keys.

  if (key == "") return;


  //  Concatenate the (single) modifier and the key value.  There's probably a better way to do this but I don't know
  //  what it is at the moment.

  QString modifier = "";
  if (e->modifiers () == Qt::AltModifier) modifier = "Alt+";
  if (e->modifiers () == Qt::ControlModifier) modifier = "Ctrl+";
  if (e->modifiers () == Qt::MetaModifier) modifier = "Meta+";


  if (!modifier.isEmpty ()) key = modifier + keyString;


  //  Next, check for special purpose keys not associated with an actual program

  if (!key.compare (options.hotkey[ACCEPT_FILTER_HIGHLIGHTED], Qt::CaseInsensitive))
    {
      if (filter_active)
        {
          slotFilterAccept ();
          return;
        }


      //  If we have highlighted points, kill them.

      if (misc.highlight_count)
        {
          for (int32_t i = 0 ; i < misc.highlight_count ; i++)
            {
              int32_t j = misc.highlight[i];

              store_undo (&misc, options.undo_levels, data.val[j], j);
              data.val[j] |= PFM_MANUALLY_INVAL;
            }
          end_undo_block (&misc);

          misc.highlight.clear ();
          misc.highlight_count = 0;

          options.function = misc.save_function;

          redrawMap (NVTrue);
        }

      return;
    }


  if (!key.compare (options.hotkey[REJECT_FILTER_HIGHLIGHTED], Qt::CaseInsensitive))
    {
      if (filter_active)
        {
          slotFilterReject ();
          return;
        }


      //  If we have highlighted points, check for invalid points and validate them.

      if (misc.highlight_count)
        {
          for (int32_t i = 0 ; i < misc.highlight_count ; i++)
            {
              int32_t j = misc.highlight[i];

              store_undo (&misc, options.undo_levels, data.val[j], j);
              if (data.val[j] & PFM_INVAL)
                {
                  data.val[j] &= ~PFM_MANUALLY_INVAL;
                  data.val[j] &= ~PFM_FILTER_INVAL;
                }
            }
          end_undo_block (&misc);

          misc.highlight.clear ();
          misc.highlight_count = 0;

          options.function = misc.save_function;


          //  Just in case we were flagging data and we decided to delete it we want to unset the flag index.

          options.flag_index = 0;
          bFlag->setIcon (flagIcon[options.flag_index]);
          disconnect (flagGrp, SIGNAL (triggered (QAction *)), this, SLOT (slotFlagMenu (QAction *)));
          flag[0]->setChecked (true);
          connect (flagGrp, SIGNAL (triggered (QAction *)), this, SLOT (slotFlagMenu (QAction *)));

          redrawMap (NVTrue);
        }

      return;
    }


  //  Delete a single point.  Note that we're calling slotPopupMenu with popup[0].  If you change the right click menu DELETE_POINT
  //  option you have to change this as well.

  if ((options.function == DELETE_POINT || options.function == NOOP) && key == options.hotkey[DELETE_SINGLE_POINT])
    {
      slotPopupMenu (popup[0]);
      return;
    }


  //  Toggle freezing of the main marker and the multi...[0] data that is shared with other programs.

  if ((options.function == DELETE_POINT || options.function == MEASURE || options.function == NOOP) && key == options.hotkey[FREEZE_ALL])
    {
      slotPopupMenu (popup[FREEZE_POPUP]);
      return;
    }


  //  Toggle freezing of all of the markers and all of the multi... data that is shared with other programs.

  if ((options.function == DELETE_POINT || options.function == MEASURE || options.function == NOOP) && key == options.hotkey[FREEZE])
    {
      //  Only do the overall freeze if multiMode is not set to single.

      if (!misc.abe_share->mwShare.multiMode)
        {
          if (misc.marker_mode)
            {
              misc.marker_mode = 0;
            }
          else
            {
              misc.marker_mode = 2;
            }


          //  Save the marker position

          misc.frozen_point = misc.nearest_point;

          redrawMap (NVFalse);
        }


      //  If you only have one cursor, this will freeze it.

      else
        {
          slotPopupMenu (popup[FREEZE_POPUP]);
        }

      return;
    }


  //  Finally check against the ancillary programs.

  if (options.function != HOTKEY_POLYGON)
    {
      for (int32_t i = 0 ; i < NUMPROGS ; i++)
        {
          //  If option.data_type[i][0] for the requested program is set (no data type) we want to allow key presses.

          if (options.function == DELETE_POINT || options.function == MEASURE || options.data_type[i][0])
            {

              //  Check type of nearest point against acceptable types for each program.  This will
              //  actually let us use a hot key in more than one place (probably not a good idea
              //  anyway).

              if (options.data_type[i][misc.data_type] || options.data_type[i][0])
                {
                  int32_t hot_action_type = 0;
                  if (key == options.hotkey[i]) hot_action_type = 1;


                  //  Check for an occurrence of any of possible multiple action keys

                  if (!options.action[i].isEmpty ())
                    {
                      for (int32_t j = 0 ; j < 10 ; j++)
                        {
                          QString cut = options.action[i].section (',', j, j);
                          if (cut.isEmpty ()) break;
                          if (key == cut) hot_action_type = 2;
                        }
                    }

                  if (hot_action_type == 1)
                    {
                      //  Don't do anything if this is a polygon only program.

                      if (options.hk_poly_only[i]) 
                        {
                          QString message;
                          message = tr ("The key <b><i>%1</i></b> is only defined for hot key polygon mode. "
                                        "You may have been trying to run another program. "
                                        "The available programs are:<br><br>").arg (e->text ());

                          QMessageBox::warning (this, tr ("Delete Point Hot Key"), message + hotkey_message);
                          hit = NVTrue;
                          break;
                        }


                      //  Kick off the program.

                      runProg (i);
                      hit = NVTrue;
                      break;
                    }
                  else if (hot_action_type == 2)
                    {
                      //  Have to check for the mosaicView zoom key so that we can tell pfmView to tell any linked apps
                      //  that we're zooming.

                      if (key == options.action[8].section (',', 0, 0)) misc.abe_share->zoom_requested = NVTrue;


                      actionKey (this, map, &data, &options, &misc, key, lock_point);
                      hit = NVTrue;
                      break;
                    }
                }
            }
        }


      if (!hit)
        {
          QString message;
          message = tr ("The key <b><i>%1</i></b> either is not defined for ancillary program operations or the data"
                        "type of the nearest point does not match the approved data types for "
                        "this key.  The available programs are:<br><br>").arg (e->text ());

          QMessageBox::warning (this, tr ("Delete Point Hot Key"), message + hotkey_message);
        }
    }
  else
    {
      for (int32_t i = 0 ; i < NUMPROGS ; i++)
        {
          if (key == options.hotkey[i] && options.hk_poly_eligible[i])
            {
              uint8_t failed = NVFalse;
              hkp = new hotkeyPolygon (this, map, &data, &options, &misc, i, &failed);
              if (failed)
                {
                  redrawMap (NVFalse);
                }
              else
                {
                  connect (hkp, SIGNAL (hotkeyPolygonDone ()), this, SLOT (slotHotkeyPolygonDone ()));
                  connect (hkp, SIGNAL (readStandardErrorSignal (QProcess *)), this, SLOT (slotReadStandardError (QProcess *)));
                }

              hit = NVTrue;
              break;
            }
        }


      if (!hit) 
        {
          QString message;
          message = tr ("The key <b><i>%1</i></b> is not defined for hot key polygon operations. "
                        "You may have been trying to use another program. "
                        "The available programs are:<br><br>").arg (e->text ());

          QMessageBox::warning (this, tr ("Polygon Hot Key"), message + hotkey_message);
        }
    }

  e->accept ();
}



void 
fileEdit3D::slotRedrawMap ()
{
  redrawMap (NVTrue);
}



void 
fileEdit3D::slotHotkeyPolygonDone ()
{
  redrawMap (NVTrue);
}



void 
fileEdit3D::closeEvent (QCloseEvent *event __attribute__ ((unused)))
{
  slotQuit ();
}



//  A bunch of slots.

void 
fileEdit3D::slotQuit ()
{
  qApp->setOverrideCursor (Qt::WaitCursor);
  qApp->processEvents ();


  //  Clear up any undo memory we've used.

  for (int32_t i = 0 ; i < misc.undo_count ; i++)
    {
      if (misc.undo[i].count)
        {
          misc.undo[i].val.clear ();
          misc.undo[i].num.clear ();
        }
    }
  misc.undo_count = 0;


  //  Clear up any highlight memory we had.

  if (misc.highlight_count)
    {
      misc.highlight.clear ();
      misc.highlight_count = 0;
    }


  //  Flush all modified points in the last buffer.

  misc.statusProg->setRange (0, data.count);
  misc.statusProgLabel->setText (fileEdit3D::tr ("Writing..."));
  misc.statusProgLabel->setVisible (true);
  misc.statusProg->setRange (0, data.count);
  misc.statusProg->setTextVisible (true);
  qApp->processEvents();

  for (int32_t i = 0 ; i < data.count ; i++)
    {
      if (data.oval[i] != data.val[i])
        {
          misc.statusProg->setValue (i);

          io_data_write (data.val[i], data.rec[i], data.sub[i], data.time[i]);
	}
    }

  misc.statusProg->reset ();
  misc.statusProg->setRange (0, 100);
  misc.statusProg->setValue (0);
  misc.statusProgLabel->setVisible (false);
  misc.statusProg->setTextVisible (false);
  qApp->processEvents ();



  //  Make sure the default function is one of the delete functions

  options.function = misc.save_mode;


  //  Get the last rotation values

  options.zx_rotation = map->getZXRotation ();
  options.y_rotation = map->getYRotation ();


  //  Save the user's defaults

  envout (&options, &misc, this);


  io_data_close ();


  //  Kill any running ancillary programs since they may be using process ID keyed shared memory.

  for (int32_t i = 0 ; i < NUMPROGS ; i++)
    {
      if (ancillaryProgram[i] && ancillaryProgram[i]->state () == QProcess::Running)
        {
          ancillaryProgram[i]->kill ();
          delete (ancillaryProgram[i]);
        }
    }


  //  Get rid of the shared memory.

  misc.abeShare->detach ();

  if (misc.linked) unregisterABE (misc.abeRegister, misc.abe_register, abe_register_group, active_window_id);


  exit (0);
}



void 
fileEdit3D::slotPrefs ()
{
  if (prefs_dialog) prefs_dialog->close ();

  prefs_dialog = new prefs (this, &options, &misc, &data);


  //  Put the dialog in the middle of the screen.

  prefs_dialog->move (x () + width () / 2 - prefs_dialog->width () / 2, y () + height () / 2 - prefs_dialog->height () / 2);

  connect (prefs_dialog, SIGNAL (dataChangedSignal ()), this, SLOT (slotPrefDataChanged ()));
  connect (prefs_dialog, SIGNAL (hotKeyChangedSignal (int32_t)), this, SLOT (slotPrefHotKeyChanged (int32_t)));
}



//  Changed some of the preferences

void 
fileEdit3D::slotPrefDataChanged ()
{
  if (options.function == DELETE_POINT) bDeletePoint->setChecked (true);
  if (options.function == DELETE_RECTANGLE) bDeleteRect->setChecked (true);
  if (options.function == DELETE_POLYGON) bDeletePoly->setChecked (true);
  if (options.function == KEEP_POLYGON) bKeepPoly->setChecked (true);


  //  Set the semi-transparent colors for slicing.

  float hue_inc = 315.0 / (float) (NUMSHADES + 1);

  for (int32_t j = 0 ; j < NUMSHADES ; j++) 
    {
      QColor color;
      color.setHsv ((int32_t) (j * hue_inc), 255, 255, options.slice_alpha);

      misc.color_array[1][j][0] = color.red ();
      misc.color_array[1][j][0] = color.green ();
      misc.color_array[1][j][0] = color.blue ();
      misc.color_array[1][j][0] = options.slice_alpha;
    }


  map->setBackgroundColor (options.background_color);
  map->setScaleColor (options.scale_color);
  map->enableScale (options.draw_scale);
  map->setZoomPercent (options.zoom_percent);

  slicePalette.setColor (QPalette::Normal, QPalette::Base, options.background_color);
  slicePalette.setColor (QPalette::Normal, QPalette::Window, options.background_color);
  slicePalette.setColor (QPalette::Inactive, QPalette::Base, options.background_color);
  slicePalette.setColor (QPalette::Inactive, QPalette::Window, options.background_color);
  sliceBar->setPalette (slicePalette);


  //  Check for changes to ancillary programs that have associated buttons.

  for (int32_t i = 0 ; i < NUMPROGS ; i++)
    {
      if (options.progButton[i])
        {
          QString tip =  options.description[i] + "  [" + options.hotkey[i] + "]";
          options.progButton[i]->setToolTip (tip);
          options.progButton[i]->setShortcut (QKeySequence (options.hotkey[i]));
        }
    }


  for (int32_t i = 0 ; i < HOTKEYS ; i++) slotPrefHotKeyChanged (i);


  //  Save the user's defaults

  envout (&options, &misc, this);


  //  Redraw the data (which is needed because we may have changed the exaggeration or the point_limit).

  if (!misc.drawing) redrawMap (NVFalse);
}



void 
fileEdit3D::slotPrefHotKeyChanged (int32_t i)
{
  //  Set the filter button tooltip based on the filter_type

  switch (options.filter_type)
    {
    case AREA_BASED_FILTER:
      misc.buttonText[FILTER_KEY] = tr ("Run the area-based statistical filter");
      break;

    case HP_FILTER:
      misc.buttonText[FILTER_KEY] = tr ("Run the Hockey Puck filter");
      break;
    }


  QString tip = misc.buttonText[i] + "  [" + options.buttonAccel[i] + "]";
  misc.button[i]->setToolTip (tip);
  misc.button[i]->setShortcut (QKeySequence (options.buttonAccel[i]));
}



//  Slot for children to call for standard error output.

void 
fileEdit3D::slotReadStandardError (QProcess *proc)
{
  readStandardError (proc);
}



//  Generic standard error reader that puts info into the message list.

void
fileEdit3D::readStandardError (QProcess *proc)
{
  static QString resp_string = "";


  QByteArray response = proc->readAllStandardError ();


  //  Parse the return response for line feeds

  for (int32_t i = 0 ; i < response.length () ; i++)
    {
      if (response.at (i) == '\n')
        {
          //  Let's not print out the "QProcess: Destroyed while process is still running" messages that come from
          //  killing ancillary programs.  Also ignore the pfm io cache setting message.

          if (!resp_string.contains ("Destroyed while") && !resp_string.contains ("Setting MAXIMUM Cache Size") &&
              !resp_string.contains ("failed to add paths") && !resp_string.contains ("DrvShareLists") &&
              !resp_string.contains ("OpenGL Warning"))
            {
              misc.messages->append (resp_string);
            }

          resp_string = "";
        }
      else
        {
          resp_string += response.at (i);
        }
    }
}



//  This is just a slot so that we can start it from the View menu.  The actual work is done in displayMessage.

void
fileEdit3D::slotMessage ()
{
  displayMessage ();
}



void fileEdit3D::displayMessage ()
{
  if (!messageD)
    {
      messageD = new QDialog (this, (Qt::WindowFlags) Qt::WA_DeleteOnClose);
      messageD->setWhatsThis (tr ("Error and warning messages from sub-programs are displayed here."));
      messageD->setWindowTitle (tr ("fileEdit3D Messages"));

      QVBoxLayout *vbox = new QVBoxLayout (messageD);
      vbox->setMargin (5);
      vbox->setSpacing (5);

      messageBox = new QListWidget (this);

      vbox->addWidget (messageBox);


      QHBoxLayout *actions = new QHBoxLayout (0);
      vbox->addLayout (actions);

      QPushButton *bClear = new QPushButton (tr ("Clear"), this);
      bClear->setToolTip (tr ("Clear all messages"));
      connect (bClear, SIGNAL (clicked ()), this, SLOT (slotClearMessage ()));
      actions->addWidget (bClear);

      QPushButton *bSaveMessage = new QPushButton (tr ("Save"), this);
      bSaveMessage->setToolTip (tr ("Save messages to text file"));
      connect (bSaveMessage, SIGNAL (clicked ()), this, SLOT (slotSaveMessage ()));
      actions->addWidget (bSaveMessage);

      actions->addStretch (10);

      QPushButton *closeButton = new QPushButton (tr ("Close"), messageD);
      closeButton->setToolTip (tr ("Close the message dialog"));
      connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotCloseMessage ()));
      actions->addWidget (closeButton);


      messageD->resize (options.msg_width, options.msg_height);
      messageD->move (options.msg_window_x, options.msg_window_y);


      messageD->show ();
    }


  if (misc.messages->size ())
    {
      messageBox->clear ();

      for (int32_t i = 0 ; i < misc.messages->size () ; i++)
        {
          QListWidgetItem *cur = new QListWidgetItem (misc.messages->at (i));

          messageBox->addItem (cur);
          messageBox->setCurrentItem (cur);
          messageBox->scrollToItem (cur);
        }
      messageBox->show ();
    }
}



void
fileEdit3D::slotCloseMessage ()
{
  //  Use frame geometry to get the absolute x and y.

  QRect tmp = messageD->frameGeometry ();
  options.msg_window_x = tmp.x ();
  options.msg_window_y = tmp.y ();


  //  Use geometry to get the width and height.

  tmp = messageD->geometry ();
  options.msg_width = tmp.width ();
  options.msg_height = tmp.height ();


  messageD->close ();
  messageD = NULL;
}



void
fileEdit3D::slotClearMessage ()
{
  num_messages = 0;
  messageBox->clear ();
  misc.messages->clear ();
  messageAction->setEnabled (false);
}



void
fileEdit3D::slotSaveMessage ()
{
  FILE *fp = NULL;
  QStringList files, filters;
  QString file;

  QFileDialog *fd = new QFileDialog (this, tr ("fileEdit3D Save error messages"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options.input_dir);

  filters << tr ("Text (*.txt)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::AnyFile);
  fd->selectNameFilter (tr ("Text (*.txt)"));

  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      file = files.at (0);


      if (!file.isEmpty())
        {
          //  Add .txt to filename if not there.

          if (!file.endsWith (".txt")) file.append (".txt");
 
          char fname[1024];
          strcpy (fname, file.toLatin1 ());


          if ((fp = fopen (fname, "w")) != NULL)
            {
              for (int32_t i = 0 ; i < misc.messages->size () ; i++)
                {
                  char string[2048];
                  strcpy (string, misc.messages->at (i).toLatin1 ());

                  fprintf (fp, "%s\n", string);
                }

              fclose (fp);
            }

          slotClearMessage ();
          slotCloseMessage ();
        }
    }
}



void
fileEdit3D::slotDisplayInvalid ()
{
  static uint8_t noop_set;
  static int32_t noop_function = 0;


  if (bDisplayInvalid->isChecked ())
    {
      noop_set = NVFalse;

      options.display_invalid = NVTrue;


      //  Don't allow editing when displaying invalid data.

      if (options.function == DELETE_POINT || options.function == DELETE_RECTANGLE || options.function == DELETE_POLYGON ||
          options.function == KEEP_POLYGON || options.function == HOTKEY_POLYGON)
        {
          noop_function = options.function;
          options.function = NOOP;
          noop_set = NVTrue;
        }
    }
  else
    {
      options.display_invalid = NVFalse;


      //  Restore the function if we were in an edit mode when we turned display_invalid on.

      if (noop_set) options.function = noop_function;
    }

  redrawMap (NVTrue);
}



void 
fileEdit3D::slotClearHighlight ()
{
  if (misc.highlight_count)
    {
      misc.highlight.clear ();
      misc.highlight_count = 0;
    }

  bClearHighlight->setEnabled (false);
  redrawMap (NVFalse);
}



void 
fileEdit3D::slotMode (int id)
{
  misc.save_function = options.function;
  options.function = id;
  setFunctionCursor (options.function);


  switch (options.function)
    {
      //  Kill any flags that may be on so we won't sit there wondering why we're not highlighting stuff.  DOH!!!

    case HIGHLIGHT_POLYGON:
      slotFlagMenu (flag[0]);
      break;


      //  Save the last edit mode in case we exit

    case DELETE_RECTANGLE:
    case DELETE_POLYGON:
    case DELETE_POINT:
    case KEEP_POLYGON:
      misc.save_mode = options.function;
      break;
    }
}



void
fileEdit3D::slotStop ()
{
  misc.drawing_canceled = NVTrue;
}



void
fileEdit3D::slotFlagMenu (QAction *action)
{
  for (int32_t i = 0 ; i < 5 ; i++)
    {
      if (action == flag[i])
        {
          options.flag_index = i;
          break;
        }
    }


  bFlag->setIcon (flagIcon[options.flag_index]);

  redrawMap (NVFalse);
}



void
fileEdit3D::slotProg (int id)
{
  if (options.progButton[id]->isChecked ())
    {
      runProg (id);


      //  Switch to DELETE_POINT mode.

      bDeletePoint->click ();
    }
  else
    {
      killProg (id);
    }
}



void
fileEdit3D::slotHotkeyHelp ()
{
  hotkeyHelp *hk = new hotkeyHelp (this, &options, &misc);


  //  Put the dialog in the middle of the screen.

  hk->move (x () + width () / 2 - hk->width () / 2, y () + height () / 2 - hk->height () / 2);

  hk->show ();
}


void
fileEdit3D::slotToolbarHelp ()
{
  QWhatsThis::showText (QCursor::pos ( ), toolbarText, map);
}


void 
fileEdit3D::slotFilter ()
{
  uint8_t Filter (OPTIONS *options, MISC *misc, double *mx, double *my, int32_t count, POINT_DATA *data);

  notebook->setTabEnabled (options.filter_type + 1, true);
  notebook->setCurrentIndex (options.filter_type + 1);

  filter_active = NVTrue;

  if (misc.filter_kill_count)
    {
      misc.filter_kill_list.clear ();
      misc.filter_kill_count = 0;


      //  Clear the marker points.

      map->clearMarkerPoints ();
    }


  double mx[4], my[4];

  mx[0] = data.bounds.min_x;
  my[0] = data.bounds.min_y;
  mx[1] = data.bounds.min_x;
  my[1] = data.bounds.max_y;
  mx[2] = data.bounds.max_x;
  my[2] = data.bounds.max_y;
  mx[3] = data.bounds.max_x;
  my[3] = data.bounds.min_y;


  setMainButtons (NVFalse);

  qApp->setOverrideCursor (Qt::WaitCursor);
  qApp->processEvents ();

  Filter (&options, &misc, mx, my, 4, &data);

  qApp->restoreOverrideCursor ();
  qApp->processEvents ();


  QString tmp;

  switch (options.filter_type)
    {
    case AREA_BASED_FILTER:
      if (misc.filter_kill_count == 1)
        {
          filterBox->setTitle (tr ("1 point selected"));
        }
      else
        {
          filterBox->setTitle (tr ("%L1 points selected").arg (misc.filter_kill_count));
        }

      tmp = QString ("%L1").arg (options.filterSTD, 0, 'f', 1);
      stdValue->setText (tmp);
      break;

    case HP_FILTER:
      if (misc.filter_kill_count == 1)
        {
          hpFilterBox->setTitle (tr ("1 point selected"));
        }
      else
        {
          hpFilterBox->setTitle (tr ("%L1 points selected").arg (misc.filter_kill_count));
        }

      tmp = QString ("%L1").arg (options.hp_filter_radius, 0, 'f', 1);
      hpRadiusValue->setText (tmp);
      tmp = QString ("%L1").arg (options.hp_filter_height, 0, 'f', 1);
      hpHeightValue->setText (tmp);
      tmp = QString ("%1").arg (options.hp_filter_neighbors);
      hpNeighborsValue->setText (tmp);
      break;
    }


  if (misc.filter_kill_count)
    {
      redrawMap (NVTrue);


      //  Force redrawing of the filter kill points

      overlayFlag (map, &options, &misc, &data);
    }
}


void 
fileEdit3D::slotClearFilterMasks ()
{
  //  Clear any filter masked areas.
  
  for (int32_t i = 0 ; i < data.count ; i++) data.fmask[i] = NVFalse;

  misc.filter_mask = NVFalse;


  //  If the filter message box is up then we are clearing the filter masks after the filter so we want to discard the current filter points
  //  then filter again.

  if (filter_active) slotFilter ();


  redrawMap (NVFalse);
}



void
fileEdit3D::slotFilterStdChanged (int value)
{
  options.filterSTD = (float) value / 10.0;
  QString std = QString ("%L1").arg (options.filterSTD, 0, 'f', 1);
  stdValue->setText (std);
}



void
fileEdit3D::slotFilterStdReturn ()
{
  float value = (float) stdValue->text ().toDouble ();
  int32_t ivalue = NINT (value * 10.0);

  if (ivalue < stdSlider->minimum () || ivalue > stdSlider->maximum ()) ivalue = stdSlider->value ();

  stdSlider->setValue (ivalue);

  slotFilter ();

  map->setFocus (Qt::OtherFocusReason);
}



void
fileEdit3D::slotHpFilterRadiusReturn ()
{
  float value = (float) hpRadiusValue->text ().toDouble ();

  if (value < HP_MIN_RADIUS) value = HP_MIN_RADIUS;
  if (value > HP_MAX_RADIUS) value = HP_MAX_RADIUS;

  options.hp_filter_radius = value;

  slotFilter ();

  map->setFocus (Qt::OtherFocusReason);
}



void
fileEdit3D::slotHpFilterHeightReturn ()
{
  float value = (float) hpHeightValue->text ().toDouble ();

  if (value < HP_MIN_HEIGHT) value = HP_MIN_HEIGHT;
  if (value > HP_MAX_HEIGHT) value = HP_MAX_HEIGHT;

  options.hp_filter_height = value;

  slotFilter ();

  map->setFocus (Qt::OtherFocusReason);
}



void
fileEdit3D::slotHpFilterNeighborsReturn ()
{
  int32_t value = hpNeighborsValue->text ().toInt ();

  if (value < HP_MIN_NEIGHBORS) value = HP_MIN_NEIGHBORS;
  if (value > HP_MAX_NEIGHBORS) value = HP_MAX_NEIGHBORS;

  options.hp_filter_neighbors = value;

  slotFilter ();

  map->setFocus (Qt::OtherFocusReason);
}



void
fileEdit3D::slotFilterAccept ()
{
  filter_active = NVFalse;


  //  Use PFM_MANUALLY_INVAL flag since the user has to confirm.

  for (int32_t i = 0 ; i < misc.filter_kill_count ; i++)
    {
      //  Negative one means we've masked the point after filtering.

      if (misc.filter_kill_list[i] >= 0)
        {
          store_undo (&misc, options.undo_levels, data.val[misc.filter_kill_list[i]], misc.filter_kill_list[i]);
          data.val[misc.filter_kill_list[i]] |= PFM_MANUALLY_INVAL;
        }
    }
  end_undo_block (&misc);
  misc.filtered = NVTrue;


  misc.filter_kill_list.clear ();
  misc.filter_kill_count = 0;


  //  Clear the marker points.

  map->clearMarkerPoints ();


  notebook->setTabEnabled (1, false);
  notebook->setTabEnabled (2, false);
  notebook->setCurrentIndex (0);

  redrawMap (NVTrue);
}



void
fileEdit3D::slotFilterReject ()
{
  filter_active = NVFalse;

  misc.filter_kill_list.clear ();
  misc.filter_kill_count = 0;


  //  Clear the marker points.

  map->clearMarkerPoints ();


  notebook->setTabEnabled (1, false);
  notebook->setTabEnabled (2, false);
  notebook->setCurrentIndex (0);

  redrawMap (NVTrue);
}



//  Undo the last edit operation.

void 
fileEdit3D::slotUndo ()
{
  undo (&misc, &data);

  redrawMap (NVTrue);
}



//  Kill ancillary programs.

uint8_t 
fileEdit3D::killProg (int id)
{
  if (ancillaryProgram[id] && ancillaryProgram[id]->state () == QProcess::Running)
    {
      if (options.state[id])
        {
          misc.abeShare->lock ();
          misc.abe_share->key = options.kill_switch[id];
          misc.abeShare->unlock ();


          //  Give the ancillary process time enough to see the kill flag.

#ifdef NVWIN3X
          Sleep (1000);
#else
          sleep (1);
#endif


          misc.abe_share->key = 0;
          options.state[id] = 1;
        }


      //  Try to kill it even if we kill switched it.

      ancillaryProgram[id]->kill ();

      delete (ancillaryProgram[id]);
      ancillaryProgram[id] = NULL;

      return (NVTrue);
    }


  //  Check for the state set to 2.  This would mean that we started the editor and the program was already up and running.

  if (options.state[id] == 2)
    {
      misc.abeShare->lock ();
      misc.abe_share->key = options.kill_switch[id];
      misc.abeShare->unlock ();


      //  Give the ancillary process time enough to see the kill flag.

#ifdef NVWIN3X
      Sleep (1000);
#else
      sleep (1);
#endif


      misc.abe_share->key = 0;
      options.state[id] = 1;
    }

  return (NVFalse);
}



//  Run ancillary programs (not in hotkey polygon mode).

void 
fileEdit3D::runProg (int id)
{
  //  We want to toggle external GUI programs that are capable of kill and respawn.

  uint8_t killed = killProg (id);


  //  If the program is not a "kill/respawn" GUI or it is and it wasn't killed immediately
  //  prior to this, we want to start the program.

  if (!options.state[id] || (options.state[id] == 1 && !killed))
    {
      ancillaryProgram[id] = new ancillaryProg (this, map, &data, &options, &misc, id, lock_point);
      connect (ancillaryProgram[id], SIGNAL (redrawSignal ()), this, SLOT (slotRedrawMap ()));
      connect (ancillaryProgram[id], SIGNAL (readStandardErrorSignal (QProcess *)), this, SLOT (slotReadStandardError (QProcess *)));
      options.state[id] = 1;
    }
}



//  2D track map slots

//  Mouse press signal from the map class.

void 
fileEdit3D::slotTrackMousePress (QMouseEvent *e __attribute__ ((unused)), double lon __attribute__ ((unused)), double lat __attribute__ ((unused)))
{
  if (e->button () == Qt::LeftButton) {};
  if (e->button () == Qt::MidButton) {};
  if (e->button () == Qt::RightButton) {};
}



//  Mouse release signal from the map class.  Right now we don't really need this.

void 
fileEdit3D::slotTrackMouseRelease (QMouseEvent *e, double lon __attribute__ ((unused)), double lat __attribute__ ((unused)))
{
  if (e->button () == Qt::LeftButton) {};
  if (e->button () == Qt::MidButton) {};
  if (e->button () == Qt::RightButton) {};
}



//  Mouse move signal from the map class.

void
fileEdit3D::slotTrackMouseMove (QMouseEvent *e __attribute__ ((unused)), double lon, double lat)
{
  char                      ltstring[25], lnstring[25], hem;
  double                    deg, min, sec;
  QString                   y_string, x_string;
  double                    dist;
  static int32_t            prev_nearest_point = -1;


  uint8_t compare_to_stack (int32_t current_point, double dist, MISC *misc);


  //  Track the cursor position for other ABE programs

  misc.abe_share->cursor_position.y = lat;
  misc.abe_share->cursor_position.x = lon;


  //  Clear the nearest point stack.

  for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++)
    {
      misc.nearest_stack.point[i] = -1;
      misc.nearest_stack.dist[i] = 9999999999.0;
    }


  misc.nearest_point = -1;


  for (int32_t i = 0 ; i < data.count ; i++)
    {
      //  Do not use null points.  Do not use invalid points unless the display_man_invalid, display_flt_invalid, or
      //  display_null flag is set.  Do not use masked points. Do not check points that are not on the display.

      int32_t trans = 0;
      if ((trans = check_bounds (&options, &misc, &data, i, NVTrue, misc.slice)) < 2)
        {
          dist = sqrt ((double) ((lat - data.y[i]) * (lat - data.y[i])) + (double) ((lon - data.x[i]) * (lon - data.x[i])));


          //  Check the points against the points in the nearest points stack.

          if (compare_to_stack (i, dist, &misc))
            {
              //  If the return was true then this is the minimum distance so far.

              misc.nearest_point = misc.nearest_stack.point[0];
            }
        }
    }


  QString rec_string = "", sub_string = "", distance_string = "";


  //  Update the status bars

  // 
  //                      if our point has moved or we have just selected a new overplot we will go
  //                      into this code block.
  //

  if (misc.nearest_point != -1 && misc.nearest_point != prev_nearest_point)
    {
      //  Locking shared memory.

      misc.abeShare->lock ();


      //  Only change the nearest filename if we aren't locked.  External programs monitor
      //  abe_share->nearest_point to trigger updates of their windows.  They use
      //  abe_share->mwShare.multiRecord[0] for the current record to display.  In this way
      //  we can lock the main screen by not updating abe_share->mwShare.multiRecord[0] and still
      //  trigger the external programs to update (such as for multiple waveforms in waveMonitor or
      //  CZMILwaveMonitor).  When we do this we don't want nearest_filename or nearest_line to change
      //  either since that is where the external programs get the filename to use with 
      //  abe_share->mwShare.multiRecord[0].


      //  Push the nearest stack points into the ABE shared memory for use by waveMonitor, CZMILwaveMonitor,
      //  and lidarMonitor.

      for (int32_t j = 0 ; j < MAX_STACK_POINTS ; j++)
        {
          //  Normal, unfrozen mode.

          if (!misc.marker_mode)
            {
              misc.abe_share->mwShare.multiSubrecord[j] = data.sub[misc.nearest_stack.point[j]];
              misc.abe_share->mwShare.multiPoint[j].x = data.x[misc.nearest_stack.point[j]];
              misc.abe_share->mwShare.multiPoint[j].y = data.y[misc.nearest_stack.point[j]];
              misc.abe_share->mwShare.multiPoint[j].z = data.z[misc.nearest_stack.point[j]];
              misc.abe_share->mwShare.multiLocation[j] = misc.nearest_stack.point[j];
              misc.abe_share->mwShare.multiType[j] = misc.data_type;
              misc.abe_share->mwShare.multiPresent[j] = data.rec[misc.nearest_stack.point[j]];
              misc.abe_share->mwShare.multiRecord[j] = data.rec[misc.nearest_stack.point[j]];
            }


          //  If we're frozen, we don't want to replace the [0] record and we want to push the nearest
          //  MAX_STACK_POINTS - 1 records up one level.

          else
            {
              if (j)
                {
                  misc.abe_share->mwShare.multiSubrecord[j] = data.sub[misc.nearest_stack.point[j - 1]];
                  misc.abe_share->mwShare.multiPoint[j].x = data.x[misc.nearest_stack.point[j - 1]];
                  misc.abe_share->mwShare.multiPoint[j].y = data.y[misc.nearest_stack.point[j - 1]];
                  misc.abe_share->mwShare.multiPoint[j].z = data.z[misc.nearest_stack.point[j - 1]];
                  misc.abe_share->mwShare.multiLocation[j] = misc.nearest_stack.point[j - 1];
                  misc.abe_share->mwShare.multiType[j] = misc.data_type;
                  misc.abe_share->mwShare.multiPresent[j] = data.rec[misc.nearest_stack.point[j - 1]];
                  misc.abe_share->mwShare.multiRecord[j] = data.rec[misc.nearest_stack.point[j - 1]];
                }
            }
        }
      misc.abe_share->mwShare.multiNum = MAX_STACK_POINTS;


      //  Trigger some of the external applications to update.

      misc.abe_share->nearest_point = misc.nearest_point;


      //  Unlock shared memory.

      misc.abeShare->unlock ();


      rec_string = QString ("%L1").arg (misc.abe_share->mwShare.multiRecord[0]);
      sub_string = QString ("%L1").arg (misc.abe_share->mwShare.multiSubrecord[0]);


      recLabel->setText (rec_string);
      subLabel->setText (sub_string);
    }


  prev_nearest_point = misc.nearest_point;


  //  First set the 3D tracker.

  int32_t pix_x, pix_y;
  map->get2DCoords (data.x[misc.nearest_point], data.y[misc.nearest_point], -data.z[misc.nearest_point], &pix_x, &pix_y);


  //  This particular situation caused a lingering of shot markers so a clearing of markers is called here.  This is a weird situation.

  discardMovableObjects ();


  map->setMovingList (&mv_tracker, marker, 16, pix_x, pix_y, 0.0, 2, options.tracker_color);


  //  Then set the 2D tracker.

  QBrush b1;
  trackMap->setMovingPath (&mv_2D_tracker, tracker_2D, data.x[misc.nearest_point], data.y[misc.nearest_point], 2, options.tracker_color, b1,
                           NVFalse, Qt::SolidLine);


  strcpy (ltstring, fixpos (lat, &deg, &min, &sec, &hem, POS_LAT, options.position_form));
  strcpy (lnstring, fixpos (lon, &deg, &min, &sec, &hem, POS_LON, options.position_form));

  latLabel->setText (ltstring);
  lonLabel->setText (lnstring);
}



//  Resize signal from the map class.

void
fileEdit3D::slotTrackResize (QResizeEvent *e __attribute__ ((unused)))
{
}



//  Prior to drawing coastline signal from the map class.  This is where we do our heavy lifting.

void 
fileEdit3D::slotTrackPreRedraw (NVMAP_DEF l_mapdef __attribute__ ((unused)))
{
  qApp->setOverrideCursor (Qt::WaitCursor);
  qApp->processEvents ();

  for (int32_t i = 0 ; i < data.count ;  i++)
    {
      int32_t xyz_x, xyz_y, xyz_z;
      double dummy;
      if (!(data.val[i] & (PFM_DELETED | PFM_INVAL)))
        {
          //  Check against the viewing area.

          int32_t trans = 0;
          if ((trans = check_bounds (&options, &misc, &data, i, NVFalse, NVFalse)) < 2)
            {
              int32_t c_index = NINT ((NUMSHADES - 1) - ((-data.z[i]) - bounds.min_z) / range_z * (float) (NUMSHADES - 1));


              //  Check for out of range data.

              if (c_index < 0) c_index = 0;
              if (c_index > NUMSHADES - 1) c_index = NUMSHADES - 1;


              QColor tmp = misc.track_color_array[c_index];

              trackMap->map_to_screen (1, &data.x[i], &data.y[i], &dummy, &xyz_x, &xyz_y, &xyz_z);
              trackMap->fillRectangle (xyz_x, xyz_y, 2, 2, tmp, NVFalse);
            }
        }
    }

  trackMap->flush ();
}



//  Post grid drawing signal from map class.  Mostly just cleanup work.

void 
fileEdit3D::slotTrackPostRedraw (NVMAP_DEF mapdef __attribute__ ((unused)))
{
  qApp->restoreOverrideCursor ();
}



//  Clean exit routine

void 
fileEdit3D::clean_exit (int32_t ret)
{
  //  Have to close the GL widget or it stays on screen in VirtualBox

  map->close ();

  exit (ret);
}



void
fileEdit3D::about ()
{
  QMessageBox::about (this, VERSION, fileEdit3DAboutText);
}



void
fileEdit3D::slotAcknowledgments ()
{
  QMessageBox::about (this, VERSION, acknowledgmentsText);
}



void
fileEdit3D::aboutQt ()
{
  QMessageBox::aboutQt (this, VERSION);
}
