
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

void get_nearest_kill_point (MISC *misc, POINT_DATA *data, double lat, double lon, NV_F64_COORD3 *hot)
{
  //  Return if we're not in kill point mode.

  if (!misc->filter_kill_count) return;


  double min_dist = 9999999999.0;
  int32_t kill_point = -1;

  for (int32_t i = 0 ; i < misc->filter_kill_count ; i++)
    {
      //  Negative one means we've masked the point after filtering.

      if (misc->filter_kill_list[i] >= 0)
        {
          int32_t j = misc->filter_kill_list[i];


          double dist = sqrt ((double) ((lat - data->y[j]) * (lat - data->y[j])) + (double) ((lon - data->x[j]) * (lon - data->x[j])));


          if (dist < min_dist)
            {
              min_dist = dist;
              hot->x = data->x[j];
              hot->y = data->y[j];
              hot->z = data->z[j];
              kill_point = j;
            }
        }
    }


  misc->nearest_point = misc->nearest_stack.point[0] = kill_point;


  //  Force this point to not be replaced by compare_to_stack by setting it's distance to a negative.

  misc->nearest_stack.dist[0] = -999.0;


  return;
}
