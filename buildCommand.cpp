
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
    [KILL_SWITCH] - Add a kill switch value so that we can set the shared key to kill the outboard program.
                    This option requires [SHARED_MEMORY_KEY] but not all shared programs will have a kill switch.
                    In fact, only "kill and respawn" GUI programs (those with state set) will do this.


    Note: the commands used to be options (i.e. user modifiable but it became too complicated).
    If you change these you must update the documentation in hotkeysHelp.cpp.

*/


int32_t buildCommand (QString progString, QString actionString, POINT_DATA *data, MISC *misc, int32_t nearest_point,
                       QString *cmd, QStringList *args, int32_t kill_switch)
{
  int32_t              ret;
  QString              command, cut, num;


  cut = progString;
  args->clear ();

  ret = 1;
  if (cut.contains ("[CL]")) ret = 2;
  cut.remove (QString ("[CL]"));

  *cmd = cut.section (' ', 0, 0);

  command = cut.simplified ();


  for (int32_t i = 1 ; i < 999 ; i++)
    {
      cut = command.section (' ', i, i);
      if (cut.isEmpty ()) break;

      cut.replace (QString ("[MIN_Y]"), num.setNum (misc->displayed_area.min_y));
      cut.replace (QString ("[MIN_X]"), num.setNum (misc->displayed_area.min_x));
      cut.replace (QString ("[MAX_Y]"), num.setNum (misc->displayed_area.max_y));
      cut.replace (QString ("[MAX_X]"), num.setNum (misc->displayed_area.max_x));
      cut.replace (QString ("[SHARED_MEMORY_KEY]"), QString ("--shared_memory_key=%1").arg (misc->abe_share->ppid));
      cut.replace (QString ("[KILL_SWITCH]"), QString ("--kill_switch=%1").arg (kill_switch));


      //  Don't check for these if we're using a shared file.
      //  Nearest point set to negative indicates a shared file command.

      if (nearest_point >= 0)
        {
          if (cut.contains ("[INPUT_FILE]"))
            {
              cut.replace (QString ("[INPUT_FILE]"), QString (misc->file));
            }
          cut.replace (QString ("[Z_VALUE]"), num.setNum (data->z[nearest_point]));
          cut.replace (QString ("[X_VALUE]"), num.setNum (data->x[nearest_point]));
          cut.replace (QString ("[Y_VALUE]"), num.setNum (data->y[nearest_point]));
          cut.replace (QString ("[VALIDITY]"), num.setNum (data->val[nearest_point], 16));
          cut.replace (QString ("[RECORD]"), num.setNum (data->rec[nearest_point]));
          cut.replace (QString ("[SUBRECORD]"), num.setNum (data->sub[nearest_point]));
        }

      *args += cut;
    }



  //  Check for action keys and add them to the command as action arguments

  if (!actionString.isEmpty ())
    {
      for (int32_t i = 0 ; i < 999 ; i++)
        {
          cut = actionString.section (',', i, i);
          if (cut.isEmpty ()) break;

          QString key;
          key.sprintf ("--actionkey%02d=", i);
          key += cut;

          *args += key;
        }
    }

  return (ret);
}
