
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



#include "fileEdit3D.hpp"

void set_defaults (MISC *misc, OPTIONS *options, POINT_DATA *data, uint8_t restore)
{
  if (!restore)
    {
      misc->draw_area_width = 1220;
      misc->draw_area_height = 950;
      misc->drawing_canceled = NVFalse;
      misc->area_drawn = NVFalse;
      misc->poly_count = 0;
      misc->slice = NVFalse;
      misc->hotkey = -1;
      misc->drawing = NVFalse;
      misc->marker_mode = 0;
      misc->frozen_point = -1;
      misc->mask_active = NVFalse;
      misc->filter_mask = NVFalse;
      misc->filter_kill_count = 0;
      misc->filtered = NVFalse;
      misc->gsf_present = NVFalse;
      misc->hydro_lidar_present = NVFalse;
      misc->lidar_present = NVFalse;
      misc->process_id = getpid ();
      misc->highlight_count = 0;


      //  Set up the sine and cosine values for slicing.

      for (int32_t i = 0 ; i < 3600 ; i++)
        {
          misc->sin_array[i] = sinf (((float) i / 10.0) * NV_DEG_TO_RAD);
          misc->cos_array[i] = cosf (((float) i / 10.0) * NV_DEG_TO_RAD);
        }


      //  Tooltip text for the buttons that have editable accelerators

      misc->buttonText[OPEN_FILE_KEY] = fileEdit3D::tr ("Open file");
      misc->buttonText[QUIT_KEY] = fileEdit3D::tr ("Quit");
      misc->buttonText[RESET_KEY] = fileEdit3D::tr ("Reset to original view");
      misc->buttonText[DELETE_POINT_MODE_KEY] = fileEdit3D::tr ("Select delete subrecord/record edit mode");
      misc->buttonText[DELETE_RECTANGLE_MODE_KEY] = fileEdit3D::tr ("Select delete rectangle edit mode");
      misc->buttonText[DELETE_POLYGON_MODE_KEY] = fileEdit3D::tr ("Select delete polygon edit mode");
      misc->buttonText[FILTER_KEY] = fileEdit3D::tr ("Run the filter");
      misc->buttonText[RECTANGLE_FILTER_MASK_KEY] = fileEdit3D::tr ("Select rectangle filter mask mode");
      misc->buttonText[POLYGON_FILTER_MASK_KEY] = fileEdit3D::tr ("Select polygon filter mask mode");
      misc->buttonText[RUN_HOTKEY_POLYGON_MODE_KEY] = fileEdit3D::tr ("Select run hotkey program in polygon mode");
      misc->buttonText[UNDO_KEY] = fileEdit3D::tr ("Undo last edit operation");
      misc->buttonText[CLEAR_HIGHLIGHT_KEY] = fileEdit3D::tr ("Clear all highlighted/marked points");
      misc->buttonText[HIGHLIGHT_POLYGON_MODE_KEY] = fileEdit3D::tr ("Select highlight/mark points in polygon mode");
      misc->buttonText[CLEAR_POLYGON_MODE_KEY] = fileEdit3D::tr ("Select clear all highlighted/marked points in polygon mode");


      //  ToolButton icons for the buttons that have editable accelerators

      misc->buttonIcon[OPEN_FILE_KEY] = QIcon (":/icons/fileopen.png");
      misc->buttonIcon[QUIT_KEY] = QIcon (":/icons/quit.png");
      misc->buttonIcon[RESET_KEY] = QIcon (":/icons/reset_view.png");
      misc->buttonIcon[DELETE_POINT_MODE_KEY] = QIcon (":/icons/delete_point.png");
      misc->buttonIcon[DELETE_RECTANGLE_MODE_KEY] = QIcon (":/icons/delete_rect.png");
      misc->buttonIcon[DELETE_POLYGON_MODE_KEY] = QIcon (":/icons/delete_poly.png");
      misc->buttonIcon[FILTER_KEY] = QIcon (":/icons/filter.png");
      misc->buttonIcon[RECTANGLE_FILTER_MASK_KEY] = QIcon (":/icons/filter_mask_rect.png");
      misc->buttonIcon[POLYGON_FILTER_MASK_KEY] = QIcon (":/icons/filter_mask_poly.png");
      misc->buttonIcon[RUN_HOTKEY_POLYGON_MODE_KEY] = QIcon (":/icons/hotkey_poly.png");
      misc->buttonIcon[UNDO_KEY] = QIcon (":/icons/undo.png");
      misc->buttonIcon[CLEAR_HIGHLIGHT_KEY] = QIcon (":/icons/clear_highlight.png");
      misc->buttonIcon[HIGHLIGHT_POLYGON_MODE_KEY] = QIcon (":/icons/highlight_polygon.png");
      misc->buttonIcon[CLEAR_POLYGON_MODE_KEY] = QIcon (":/icons/clear_polygon.png");


      data->count = 0;

      misc->linked = NVFalse;

      misc->abe_share->key = 0;
      misc->abe_share->modcode = 0;
      misc->displayed_area.min_y = -91.0;

      misc->qsettings_app = "fileEdit3D";

      misc->undo_count = 0;


      //  This is a special case for an option.  We don't want to reset the undo levels if we called for a restore of defaults from the prefs dialog.

      options->undo_levels = 100;
    }


  /*

    Ancillary program command line substitutions:


    [INPUT_FILE] - input data file associated with the current point
    [SHARED_MEMORY_ID] - ABE shared memory ID (some programs like chartsPic require this)
    [Z_VALUE] - Z value of the current point
    [X_VALUE] - X value (usually longitude) associated with the current point
    [Y_VALUE] - Y value (usually latitude) associated with the current point
    [MIN_Y] - minimum Y value in the currently displayed area
    [MIN_X] - minimum X value in the currently displayed area
    [MAX_Y] - maximum Y value in the currently displayed area
    [MAX_X] - maximum X value in the currently displayed area
    [VALIDITY] - PFM validity word for the current point
    [RECORD] - input file record number associated with the current point
    [SUBRECORD] - input file subrecord (usually beam) number associated with the current point

    [CL] - run this program as a command line program (creates a dialog for output)
    [SHARED_MEMORY_KEY] - Add the shared memory ID so the outboard program can track it


    Note: the commands used to be options (i.e. user modifiable but it became too complicated).
    If you change these you must update the documentation in hotkeysHelp.cpp.

    Note: Polygon eligible commands cannot have an associated button.  Associated buttons are set in fileEdit3D.cpp.

  */


  for (int32_t i = 0 ; i < NUMPROGS ; i++)
    {
      options->kill_switch[i] = 10000000 + i;

      for (int32_t j = 0 ; j < PFM_DATA_TYPES ; j++) options->data_type[i][j] = NVFalse;

      switch (i)
        {
        case EXAMGSF:
          options->prog[i] = "examGSF --file [INPUT_FILE] --record [RECORD]";
          options->name[i] = "examGSF";
          options->description[i] = fileEdit3D::tr ("GSF non-graphical data viewer.  Displays data for nearest point.");
          options->data_type[i][PFM_GSF_DATA] = NVTrue;
          options->hotkey[i] = "e";
          options->action[i] = "";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 0;
          break;

        case GSFMONITOR:
          options->prog[i] = "gsfMonitor [SHARED_MEMORY_KEY]";
          options->name[i] = "gsfMonitor";
          options->description[i] = fileEdit3D::tr ("GSF non-graphical data viewer.  Displays ping data for nearest point.");
          options->data_type[i][PFM_GSF_DATA] = NVTrue;
          options->hotkey[i] = "g";
          options->action[i] = "";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 1;
          break;

        case WAVEFORMMONITOR:
          options->prog[i] = "waveformMonitor [SHARED_MEMORY_KEY]";
          options->name[i] = "waveformMonitor";
          options->description[i] = fileEdit3D::tr ("HOF waveform monitor (PMT, APD, IR, Raman).  Displays waveform for nearest point.");
          options->data_type[i][PFM_SHOALS_1K_DATA] = NVTrue;
          options->data_type[i][PFM_CHARTS_HOF_DATA] = NVTrue;
          options->hotkey[i] = "w";
          options->action[i] = "1,2,3,4";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 1;
          break;


          //  For historical reasons we are using "x" to kick this off.  It used to be the "extended" information
          //  for the waveMonitor program.

        case LIDARMONITOR:
          options->prog[i] = "lidarMonitor [SHARED_MEMORY_KEY]";
          options->name[i] = "lidarMonitor";
          options->description[i] = fileEdit3D::tr ("CZMIL, HOF, or TOF textual data viewer.  Displays entire record for nearest point.");
          options->data_type[i][PFM_SHOALS_1K_DATA] = NVTrue;
          options->data_type[i][PFM_CHARTS_HOF_DATA] = NVTrue;
          options->data_type[i][PFM_SHOALS_TOF_DATA] = NVTrue;
          options->data_type[i][PFM_CZMIL_DATA] = NVTrue;
          options->hotkey[i] = "x";
          options->action[i] = "";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 1;
          break;

        case CHARTSPIC:
          options->prog[i] = "chartsPic [SHARED_MEMORY_KEY]";
          options->name[i] = "chartsPic";
          options->description[i] = fileEdit3D::tr ("HOF and TOF down-looking image viewer.  Displays nearest down-looking photo.");
          options->data_type[i][PFM_SHOALS_1K_DATA] = NVTrue;
          options->data_type[i][PFM_CHARTS_HOF_DATA] = NVTrue;
          options->data_type[i][PFM_SHOALS_TOF_DATA] = NVTrue;
          options->data_type[i][PFM_CZMIL_DATA] = NVTrue;
          options->hotkey[i] = "i";
          options->action[i] = "r,t";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 1;
          break;

        case CZMILWAVEMONITOR:
          options->prog[i] = "CZMILwaveMonitor [SHARED_MEMORY_KEY]";
          options->name[i] = "CZMILwaveMonitor";
          options->description[i] = fileEdit3D::tr ("CZMIL waveform monitor.  Displays waveform(s) for nearest point(s).");
          options->data_type[i][PFM_CZMIL_DATA] = NVTrue;
          options->hotkey[i] = "Alt+w";
          options->action[i] = "n,0,1,2,3,4,5,6,7,8,9,c";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 1;
          break;

        case LASWAVEMONITOR:
          options->prog[i] = "LASwaveMonitor [SHARED_MEMORY_KEY]";
          options->name[i] = "LASwaveMonitor";
          options->description[i] = fileEdit3D::tr ("LAS waveform monitor.  Displays waveform for nearest point.");
          options->data_type[i][PFM_LAS_DATA] = NVTrue;
          options->hotkey[i] = "Alt+m";
          options->action[i] = "";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 1;
          break;

        case HOFRETURNKILL:
          options->prog[i] = "hofReturnKill [SHARED_MEMORY_KEY]";
          options->name[i] = "hofReturnKill (invalidate low slope returns)";
          options->description[i] = fileEdit3D::tr ("Kill HOF returns based on fore/back slope and amplitude.");
          options->data_type[i][PFM_CHARTS_HOF_DATA] = NVTrue;
          options->hotkey[i] = "!";
          options->action[i] = "";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVTrue;
          options->hk_poly_only[i] = NVTrue;
          options->state[i] = 0;
          break;

        case HOFRETURNKILL_SWA:
          options->prog[i] = "hofReturnKill -s [SHARED_MEMORY_KEY]";
          options->name[i] = "hofReturnKill (invalidate shallow water algorithm data)";
          options->description[i] = fileEdit3D::tr ("Kill HOF Shallow Water Algorithm or Shoreline Depth Swap data.");
          options->data_type[i][PFM_CHARTS_HOF_DATA] = NVTrue;
          options->hotkey[i] = "$";
          options->action[i] = "";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVTrue;
          options->hk_poly_only[i] = NVTrue;
          options->state[i] = 0;
          break;


          //  IMPORTANT NOTE: The following five functions are not ancillary programs.  If you have to add new ancillary programs,
          //  add them above these just for esthetics.

        case DELETE_SINGLE_POINT:
          options->prog[i] = "DELETE SINGLE POINT";
          options->name[i] = "DELETE SINGLE POINT";
          options->description[i] = fileEdit3D::tr ("Delete single point nearest cursor in DELETE_POINT mode.  This is an internal function.");
          options->data_type[i][PFM_UNDEFINED_DATA] = NVTrue;
          options->hotkey[i] = "d";
          options->action[i] = "";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 0;
          break;


        case ACCEPT_FILTER_HIGHLIGHTED:
          options->prog[i] = "DELETE FILTER HIGHLIGHTED";
          options->name[i] = "DELETE FILTER HIGHLIGHTED";
          options->description[i] = fileEdit3D::tr ("Delete points marked by the filter or highlighted.  This is an internal function.");
          options->data_type[i][PFM_UNDEFINED_DATA] = NVTrue;
          options->hotkey[i] = "Del";
          options->action[i] = "";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 0;
          break;

        case REJECT_FILTER_HIGHLIGHTED:
          options->prog[i] = "CLEAR FILTER HIGHLIGHTED";
          options->name[i] = "CLEAR FILTER HIGHLIGHTED";
          options->description[i] = fileEdit3D::tr ("Clear points marked by the filter/highlighted or restore highlighted invalid points.  This is an internal function.");
          options->data_type[i][PFM_UNDEFINED_DATA] = NVTrue;
          options->hotkey[i] = "Ins";
          options->action[i] = "";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 0;
          break;

        case FREEZE:
          options->prog[i] = "FREEZE";
          options->name[i] = "FREEZE MAIN BOX CURSOR";
          options->description[i] = fileEdit3D::tr ("Toggle freezing of the box cursor.  This is an internal function that affects ancillary programs.");
          options->data_type[i][PFM_UNDEFINED_DATA] = NVTrue;
          options->hotkey[i] = "f";
          options->action[i] = "";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 0;
          break;

        case FREEZE_ALL:
          options->prog[i] = "FREEZE ALL";
          options->name[i] = "FREEZE ALL BOX CURSORS";
          options->description[i] = fileEdit3D::tr ("Toggle freezing of all of the box cursors.  This is an internal function that affects ancillary programs.");
          options->data_type[i][PFM_UNDEFINED_DATA] = NVTrue;
          options->hotkey[i] = "Alt+F";
          options->action[i] = "";
          options->progButton[i] = NULL;
          options->hk_poly_eligible[i] = NVFalse;
          options->hk_poly_only[i] = NVFalse;
          options->state[i] = 0;
          break;
        }
    }


  options->position_form = 0;
  options->highlight_tip = NVTrue;
  options->min_window_size = 4.0;
  options->iho_min_window = 0;
  options->display_invalid = NVFalse;
  options->function = DELETE_POLYGON;
  options->slice_percent = 5;
  options->slice_alpha = 24;
  options->slice_gray = NVTrue;
  options->point_size = 3;
  options->edit_color = QColor (255, 255, 255, 255);
  options->tracker_color = QColor (255, 255, 0, 255);
  options->background_color = QColor (0, 0, 0, 255);
  options->scale_color = QColor (255, 255, 255, 255);
  options->font = QApplication::font ();
  options->main_button_icon_size = 24;
  options->rotation_increment = 5.0;
  options->flag_index = 0;
  options->point_limit = 100000;
  options->exaggeration = 3.0;
  options->zoom_percent = 10;
  options->draw_scale = NVTrue;
  options->filter_type = HP_FILTER;
  options->filterSTD = 2.4;
  options->hp_filter_radius = 1.0;
  options->hp_filter_height = 1.0;
  options->hp_filter_neighbors = 4;
  options->deep_filter_only = NVFalse;
  options->inputFilter = "*.d\?\?";
  options->input_dir = ".";
  options->zx_rotation = 0.0;
  options->y_rotation = 0.0;
  options->msg_window_x = 0;
  options->msg_window_y = 0;
  options->msg_width = 800;
  options->msg_height = 600;
  options->num_hp_threads = 4;


  // 
  // CHL Addition:        Set the multi-waveform cursor colors.  MAX_STACK_POINTS is 9 so we manually define 9 colors.
  //                                If MAX_STACK_POINTS gets changed you should add/subtract.
  //

  // white

  misc->abe_share->mwShare.multiColors[0].r = 255;
  misc->abe_share->mwShare.multiColors[0].g = 255;
  misc->abe_share->mwShare.multiColors[0].b = 255;
  misc->abe_share->mwShare.multiColors[0].a = 255;


  // red 

  misc->abe_share->mwShare.multiColors[1].r = 255;
  misc->abe_share->mwShare.multiColors[1].g = 0;
  misc->abe_share->mwShare.multiColors[1].b = 0;
  misc->abe_share->mwShare.multiColors[1].a = 255;


  // yellow 

  misc->abe_share->mwShare.multiColors[2].r = 255;
  misc->abe_share->mwShare.multiColors[2].g = 255;
  misc->abe_share->mwShare.multiColors[2].b = 0;
  misc->abe_share->mwShare.multiColors[2].a = 255;


  // green

  misc->abe_share->mwShare.multiColors[3].r = 0;
  misc->abe_share->mwShare.multiColors[3].g = 255;
  misc->abe_share->mwShare.multiColors[3].b = 0;
  misc->abe_share->mwShare.multiColors[3].a = 255;


  // cyan

  misc->abe_share->mwShare.multiColors[4].r = 0;
  misc->abe_share->mwShare.multiColors[4].g = 255;
  misc->abe_share->mwShare.multiColors[4].b = 255;
  misc->abe_share->mwShare.multiColors[4].a = 255;


  // blue

  misc->abe_share->mwShare.multiColors[5].r = 0;
  misc->abe_share->mwShare.multiColors[5].g = 0;
  misc->abe_share->mwShare.multiColors[5].b = 255;
  misc->abe_share->mwShare.multiColors[5].a = 255;


  // orange

  misc->abe_share->mwShare.multiColors[6].r = 153;
  misc->abe_share->mwShare.multiColors[6].g = 128;
  misc->abe_share->mwShare.multiColors[6].b = 0;
  misc->abe_share->mwShare.multiColors[6].a = 255;


  // magenta

  misc->abe_share->mwShare.multiColors[7].r = 128;
  misc->abe_share->mwShare.multiColors[7].g = 0;
  misc->abe_share->mwShare.multiColors[7].b = 255;
  misc->abe_share->mwShare.multiColors[7].a = 255;


  // dark green

  misc->abe_share->mwShare.multiColors[8].r = 0;
  misc->abe_share->mwShare.multiColors[8].g = 153;
  misc->abe_share->mwShare.multiColors[8].b = 0;
  misc->abe_share->mwShare.multiColors[8].a = 255;


  for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++)
    {
      options->waveColor[i].setRgb (misc->abe_share->mwShare.multiColors[i].r,
                                    misc->abe_share->mwShare.multiColors[i].g,
                                    misc->abe_share->mwShare.multiColors[i].b);
      options->waveColor[i].setAlpha (misc->abe_share->mwShare.multiColors[i].a);
    }


  options->buttonAccel[OPEN_FILE_KEY] = "Ctrl+o";
  options->buttonAccel[QUIT_KEY] = "Ctrl+q";
  options->buttonAccel[RESET_KEY] = "Ctrl+r";
  options->buttonAccel[DELETE_POINT_MODE_KEY] = "F3";
  options->buttonAccel[DELETE_RECTANGLE_MODE_KEY] = "F4";
  options->buttonAccel[DELETE_POLYGON_MODE_KEY] = "F5";
  options->buttonAccel[FILTER_KEY] = "F6";
  options->buttonAccel[RECTANGLE_FILTER_MASK_KEY] = "F7";
  options->buttonAccel[POLYGON_FILTER_MASK_KEY] = "F8";
  options->buttonAccel[RUN_HOTKEY_POLYGON_MODE_KEY] = "F9";
  options->buttonAccel[UNDO_KEY] = "Ctrl+z";
  options->buttonAccel[CLEAR_HIGHLIGHT_KEY] = "Ctrl+h";
  options->buttonAccel[HIGHLIGHT_POLYGON_MODE_KEY] = "Ctrl+p";
  options->buttonAccel[CLEAR_POLYGON_MODE_KEY] = "Ctrl+c";
}
