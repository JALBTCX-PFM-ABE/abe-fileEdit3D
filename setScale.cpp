
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

void setScale (float actual_min, float actual_max, MISC *misc)
{
  //  Set the scale colors for the current range.

  float numshades = (float) (NUMSHADES - 1);
  float inc = 1.0, min_z, max_z, range;
  float value = 0.0;


  min_z = actual_min;
  max_z = actual_max;


  range = max_z - min_z;

  if (fabs (range) < 0.0000001) range = 1.0;

  inc = range / (float) (NUM_SCALE_LEVELS - 1);


  for (int32_t i = 0 ; i < NUM_SCALE_LEVELS ; i++)
    {
      value = min_z + (float) i * inc;

      int32_t c_index = 0;


      //  Color by depth

      c_index = NINT ((max_z - value) / range * numshades);


      QColor clr;
      clr.setRed (misc->color_array[0][c_index][0]);
      clr.setGreen (misc->color_array[0][c_index][1]);
      clr.setBlue (misc->color_array[0][c_index][2]);
      clr.setAlpha (misc->color_array[0][c_index][3]);

      misc->scale[(NUM_SCALE_LEVELS - 1) - i]->setContents (clr, -value, 0);
    }
}
