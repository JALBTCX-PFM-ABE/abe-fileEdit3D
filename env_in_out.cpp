
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


double settings_version = 3.32;


//  Note to self - I'm not saving the contour intervals because they always get passed in from pfmView

/*!
  These functions store and retrieve the program settings (environment) from a .ini file.  On both Linux and Windows
  the file will be called "misc->qsettings_app".ini (the name of the application with a .ini extension) and will be
  stored in a directory called ABE.config.  On Linux, the ABE.config directory will be stored in your $HOME directory.
  On Windows, it will be stored in your $USERPROFILE folder.  If you make a change to the way a variable is used and
  you want to force the defaults to be restored just change the settings_version to a newer number (I've been using
  the program version number from version.hpp - which you should be updating EVERY time you make a change to the
  program!).  You don't need to change the settings_version though unless you want to force the program to go back
  to the defaults (which can annoy your users).  So, the settings_version won't always match the program version.
*/

uint8_t envin (OPTIONS *options, MISC *misc, QMainWindow *mainWindow)
{
  double saved_version = 0.0;
  QString buttonHotKey[HOTKEYS];
  QString progHotKey[NUMPROGS];


  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/" + QString (misc->qsettings_app) + ".ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/" + QString (misc->qsettings_app) + ".ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup (misc->qsettings_app);


  //  We're trying to save the hot keys since these are a pain for the user to change back if they've made major changes 
  //  to them.  We want to use whatever the user had saved even if we change the settings version.  We will still have to
  //  check for hot key conflicts though since there might be new keys with new deefault values.  To do this we're going
  //  to save the default hot keys (set in set_defaults.cpp) and then do a comparison.  If there are conflicts we'll replace 
  //  the user's setting with the default and then issue a warning.

  uint8_t hotkey_conflict = NVFalse;

  for (int32_t i = 0 ; i < HOTKEYS ; i++)
    {
      //  Save the default so we can check it later.

      buttonHotKey[i] = options->buttonAccel[i];


      QString string = misc->buttonText[i] + QString (" hot key");
      options->buttonAccel[i] = settings.value (string, options->buttonAccel[i]).toString ();
    }

  for (int32_t i = 0 ; i < NUMPROGS ; i++)
    {
      //  Save the defaults so we can check them later.

      progHotKey[i] = options->hotkey[i];


      QString name = options->name[i] + QString (" Hotkey");
      options->hotkey[i] = settings.value (name, options->hotkey[i]).toString ();

      name = options->name[i] + QString (" Action Key");
      options->action[i] = settings.value (name, options->action[i]).toString ();

      name = options->name[i] + QString (" State");
      options->state[i] = settings.value (name, options->state[i]).toInt ();
    }


  //  Make sure we have no hotkey duplications.  First, the buttons against the buttons.

  for (int32_t i = 0 ; i < HOTKEYS ; i++)
    {
      for (int32_t j = i + 1 ; j < HOTKEYS ; j++)
        {
          if (options->buttonAccel[i].toUpper () == options->buttonAccel[j].toUpper ())
            {
              hotkey_conflict = NVTrue;
              options->buttonAccel[i] = buttonHotKey[i];
              options->buttonAccel[j] = buttonHotKey[j];
            }
        }
    }


  //  Next, the programs against the programs.

  for (int32_t i = 0 ; i < NUMPROGS ; i++)
    {
      for (int32_t j = i + 1 ; j < NUMPROGS ; j++)
        {
          if (options->hotkey[i].toUpper () == options->hotkey[j].toUpper ())
            {
              hotkey_conflict = NVTrue;
              options->hotkey[i] = progHotKey[i];
              options->hotkey[j] = progHotKey[j];
            }
        }
    }


  //  Finally, the buttons against the programs.

  for (int32_t i = 0 ; i < HOTKEYS ; i++)
    {
      for (int32_t j = 0 ; j < NUMPROGS ; j++)
        {
          if (options->buttonAccel[i].toUpper () == options->hotkey[j].toUpper ())
            {
              hotkey_conflict = NVTrue;
              options->buttonAccel[i] = buttonHotKey[i];
              options->hotkey[j] = progHotKey[j];
            }
        }
    }


  if (hotkey_conflict)
    {
      QMessageBox::warning (0, QString ("fileEdit3D settings"),
                            QString ("Hot key conflicts have been detected when reaing program settings!<br><br>") +
                            QString ("Some of your hotkeys have been reset to default values!"));
    }


  saved_version = settings.value (QString ("settings version"), saved_version).toDouble ();


  //  If the settings version has changed we need to leave the values at the new defaults since they may have changed.

  if (settings_version != saved_version) return (NVFalse);


  options->flag_index = settings.value (QString ("flag index"), options->flag_index).toInt ();

  options->zoom_percent = settings.value (QString ("zoom percentage"), options->zoom_percent).toInt ();

  options->exaggeration = settings.value (QString ("Z exaggeration"), options->exaggeration).toDouble ();

  options->zx_rotation = (float) settings.value (QString ("ZX rotation"), (double) options->zx_rotation).toDouble ();

  options->y_rotation = (float) settings.value (QString ("Y rotation"), (double) options->y_rotation).toDouble ();

  options->position_form = settings.value (QString ("position form"), options->position_form).toInt ();

  options->point_size = settings.value (QString ("point size"), options->point_size).toInt ();

  options->point_limit = settings.value (QString ("point limit"), options->point_limit).toInt ();

  options->slice_percent = settings.value (QString ("slice percentage"), options->slice_percent).toInt ();

  options->slice_alpha = settings.value (QString ("slice alpha"), options->slice_alpha).toInt ();

  options->min_window_size = (float) settings.value (QString ("minimum Z window size"),
                                                          (double) options->min_window_size).toDouble ();

  options->iho_min_window = settings.value (QString ("IHO min window"), options->iho_min_window).toInt ();

  options->function = settings.value (QString ("function"), options->function).toInt ();

  options->num_hp_threads = settings.value (QString ("HP filter number of threads"), options->num_hp_threads).toInt ();

  int32_t red = settings.value (QString ("edit color/red"), options->edit_color.red ()).toInt ();
  int32_t green = settings.value (QString ("edit color/green"), options->edit_color.green ()).toInt ();
  int32_t blue = settings.value (QString ("edit color/blue"), options->edit_color.blue ()).toInt ();
  int32_t alpha = settings.value (QString ("edit color/alpha"), options->edit_color.alpha ()).toInt ();
  options->edit_color.setRgb (red, green, blue, alpha);

  red = settings.value (QString ("tracker color/red"), options->tracker_color.red ()).toInt ();
  green = settings.value (QString ("tracker color/green"), options->tracker_color.green ()).toInt ();
  blue = settings.value (QString ("tracker color/blue"), options->tracker_color.blue ()).toInt ();
  alpha = settings.value (QString ("tracker color/alpha"), options->tracker_color.alpha ()).toInt ();
  options->tracker_color.setRgb (red, green, blue, alpha);

  red = settings.value (QString ("background color/red"), options->background_color.red ()).toInt ();
  green = settings.value (QString ("background color/green"), options->background_color.green ()).toInt ();
  blue = settings.value (QString ("background color/blue"), options->background_color.blue ()).toInt ();
  alpha = settings.value (QString ("background color/alpha"), options->background_color.alpha ()).toInt ();
  options->background_color.setRgb (red, green, blue, alpha);

  red = settings.value (QString ("scale color/red"), options->scale_color.red ()).toInt ();
  green = settings.value (QString ("scale color/green"), options->scale_color.green ()).toInt ();
  blue = settings.value (QString ("scale color/blue"), options->scale_color.blue ()).toInt ();
  alpha = settings.value (QString ("scale color/alpha"), options->scale_color.alpha ()).toInt ();
  options->scale_color.setRgb (red, green, blue, alpha);


  for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++)
    {
      QString string;
      string.sprintf (QString ("Wave color %02d").toLatin1 (), i);
      red = settings.value (string + "/red", options->waveColor[i].red ()).toInt ();
      green = settings.value (string + "/green", options->waveColor[i].green ()).toInt ();
      blue = settings.value (string + "/blue", options->waveColor[i].blue ()).toInt ();
      alpha = settings.value (string + "/alpha", options->waveColor[i].alpha ()).toInt ();
      options->waveColor[i].setRgb (red, green, blue, alpha);

      misc->abe_share->mwShare.multiColors[i].r = red;
      misc->abe_share->mwShare.multiColors[i].g = green;
      misc->abe_share->mwShare.multiColors[i].b = blue;
      misc->abe_share->mwShare.multiColors[i].a = alpha;
    }


  options->slice_gray = settings.value (QString ("gray slice flag"), options->slice_gray).toBool ();

  options->draw_scale = settings.value (QString ("draw scale flag"), options->draw_scale).toBool ();

  options->filter_type = settings.value (QString ("filter type"), options->filter_type).toInt ();

  options->filterSTD = settings.value (QString ("filter standard deviation"), options->filterSTD).toDouble ();

  options->deep_filter_only = settings.value (QString ("deep filter only flag"), options->deep_filter_only).toBool ();

  options->hp_filter_radius = settings.value (QString ("HP filter radius"), options->hp_filter_radius).toDouble ();

  options->hp_filter_height = settings.value (QString ("HP filter height"), options->hp_filter_height).toDouble ();

  options->hp_filter_neighbors = settings.value (QString ("HP filter neighbors"), options->hp_filter_neighbors).toInt ();

  options->undo_levels = settings.value (QString ("undo levels"), options->undo_levels).toInt ();

  options->highlight_tip = settings.value (QString ("highlighted point tool tip flag"), options->highlight_tip).toBool ();

  options->input_dir = settings.value (QString ("input directory"), options->input_dir).toString ();

  options->inputFilter = settings.value (QString ("input filter"), options->inputFilter).toString ();


  options->msg_window_x = settings.value (QString ("message window x position"), options->msg_window_x).toInt ();
  options->msg_window_y = settings.value (QString ("message window y position"), options->msg_window_y).toInt ();
  options->msg_width = settings.value (QString ("message window width"), options->msg_width).toInt ();
  options->msg_height = settings.value (QString ("message window height"), options->msg_height).toInt ();


  mainWindow->restoreState (settings.value (QString ("main window state")).toByteArray (),
			    NINT (settings_version * 100.0L));

  mainWindow->restoreGeometry (settings.value (QString ("main window geometry")).toByteArray ());


  settings.endGroup ();


  //  We need to get the font from the global settings since it will be used in (hopefully) all of the ABE map GUI
  //  applications.  We also need to get any recent WKT values from the globalABE settings file.

#ifdef NVWIN3X
  QString ini_file2 = QString (getenv ("USERPROFILE")) + "/ABE.config/" + "globalABE.ini";
#else
  QString ini_file2 = QString (getenv ("HOME")) + "/ABE.config/" + "globalABE.ini";
#endif

  QSettings settings2 (ini_file2, QSettings::IniFormat);
  settings2.beginGroup ("globalABE");


  QString defaultFont = options->font.toString ();
  QString fontString = settings2.value (QString ("ABE map GUI font"), defaultFont).toString ();
  options->font.fromString (fontString);


  for (int32_t i = 0 ; i < 10 ; i++)
    {
      options->wktString[i] = "";

      QString name = QString ("Recent WKT %1").arg (i);
      options->wktString[i] = settings2.value (name, options->wktString[i]).toString ();
    }

  settings2.endGroup ();


  return (NVTrue);
}


void envout (OPTIONS *options, MISC *misc, QMainWindow *mainWindow)
{
  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/" + QString (misc->qsettings_app) + ".ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/" + QString (misc->qsettings_app) + ".ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup (misc->qsettings_app);

  for (int32_t i = 0 ; i < HOTKEYS ; i++)
    {
      QString string = misc->buttonText[i] + QString (" hot key");
      settings.setValue (string, options->buttonAccel[i]);
    }

  for (int32_t i = 0 ; i < NUMPROGS ; i++)
    {
      QString name = options->name[i] + QString (" Hotkey");
      settings.setValue (name, options->hotkey[i]);

      name = options->name[i] + QString (" Action Key");
      settings.setValue (name, options->action[i]);

      name = options->name[i] + QString (" State");
      settings.setValue (name, options->state[i]);
    }

  settings.setValue (QString ("settings version"), settings_version);

  settings.setValue (QString ("flag index"), options->flag_index);

  settings.setValue (QString ("zoom percentage"), options->zoom_percent);

  settings.setValue (QString ("Z exaggeration"), options->exaggeration);

  settings.setValue (QString ("ZX rotation"), (double) options->zx_rotation);

  settings.setValue (QString ("Y rotation"), (double) options->y_rotation);

  settings.setValue (QString ("main button icon size"), options->main_button_icon_size);

  settings.setValue (QString ("position form"), options->position_form);

  settings.setValue (QString ("point size"), options->point_size);

  settings.setValue (QString ("point limit"), options->point_limit);

  settings.setValue (QString ("slice percentage"), options->slice_percent);

  settings.setValue (QString ("slice alpha"), options->slice_alpha);

  settings.setValue (QString ("minimum Z window size"), (double) options->min_window_size);

  settings.setValue (QString ("IHO min window"), options->iho_min_window);

  settings.setValue (QString ("function"), options->function);

  settings.setValue (QString ("HP filter number of threads"), options->num_hp_threads);

  settings.setValue (QString ("edit color/red"), options->edit_color.red ());
  settings.setValue (QString ("edit color/green"), options->edit_color.green ());
  settings.setValue (QString ("edit color/blue"), options->edit_color.blue ());
  settings.setValue (QString ("edit color/alpha"), options->edit_color.alpha ());

  settings.setValue (QString ("tracker color/red"), options->tracker_color.red ());
  settings.setValue (QString ("tracker color/green"), options->tracker_color.green ());
  settings.setValue (QString ("tracker color/blue"), options->tracker_color.blue ());
  settings.setValue (QString ("tracker color/alpha"), options->tracker_color.alpha ());

  settings.setValue (QString ("background color/red"), options->background_color.red ());
  settings.setValue (QString ("background color/green"), options->background_color.green ());
  settings.setValue (QString ("background color/blue"), options->background_color.blue ());
  settings.setValue (QString ("background color/alpha"), options->background_color.alpha ());

  settings.setValue (QString ("scale color/red"), options->scale_color.red ());
  settings.setValue (QString ("scale color/green"), options->scale_color.green ());
  settings.setValue (QString ("scale color/blue"), options->scale_color.blue ());
  settings.setValue (QString ("scale color/alpha"), options->scale_color.alpha ());


  for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++)
    {
      QString string;
      string.sprintf (QString ("Wave color %02d").toLatin1 (), i);
      settings.setValue (string + "/red", options->waveColor[i].red ());
      settings.setValue (string + "/green", options->waveColor[i].green ());
      settings.setValue (string + "/blue", options->waveColor[i].blue ());
      settings.setValue (string + "/alpha", options->waveColor[i].alpha ());
    }


  settings.setValue (QString ("gray slice flag"), options->slice_gray);

  settings.setValue (QString ("draw scale flag"), options->draw_scale);

  settings.setValue (QString ("deep filter only flag"), options->deep_filter_only);

  settings.setValue (QString ("filter type"), options->filter_type);

  settings.setValue (QString ("filter standard deviation"), options->filterSTD);

  settings.setValue (QString ("HP filter radius"), options->hp_filter_radius);

  settings.setValue (QString ("HP filter height"), options->hp_filter_height);

  settings.setValue (QString ("HP filter neighbors"), options->hp_filter_neighbors);

  settings.setValue (QString ("undo levels"), options->undo_levels);

  settings.setValue (QString ("highlighted point tool tip flag"), options->highlight_tip);

  settings.setValue (QString ("input directory"), options->input_dir);

  settings.setValue (QString ("input filter"), options->inputFilter);


  settings.setValue (QString ("message window x position"), options->msg_window_x);
  settings.setValue (QString ("message window y position"), options->msg_window_y);
  settings.setValue (QString ("message window width"), options->msg_width);
  settings.setValue (QString ("message window height"), options->msg_height);


  settings.setValue (QString ("main window state"), mainWindow->saveState (NINT (settings_version * 100.0L)));

  settings.setValue (QString ("main window geometry"), mainWindow->saveGeometry ());


  settings.endGroup ();


  //  We need to save the WKT strings to the globalABE settings file.  This file is used to store things that
  //  may be used by multiple ABE programs.

#ifdef NVWIN3X
  QString ini_file2 = QString (getenv ("USERPROFILE")) + "/ABE.config/" + "globalABE.ini";
#else
  QString ini_file2 = QString (getenv ("HOME")) + "/ABE.config/" + "globalABE.ini";
#endif

  QSettings settings2 (ini_file2, QSettings::IniFormat);
  settings2.beginGroup ("globalABE");


  for (int32_t i = 0 ; i < 10 ; i++)
    {
      QString name = QString ("Recent WKT %1").arg (i);
      settings2.setValue (name, options->wktString[i]);
    }

  settings2.endGroup ();
}
