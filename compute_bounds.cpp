
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

uint8_t compute_bounds (nvMapGL *map __attribute__ ((unused)), OPTIONS *options, MISC *misc, POINT_DATA *data, int32_t *viewable_count, NV_F64_XYMBC *bounds)
{
  static NV_F64_XYMBC prev_bounds = {-999.0, -999.0, -999.0, -999.0, CHRTRNULL, -CHRTRNULL};


  //  Compute the Z range of the data.

  *viewable_count = 0;


  //  Compute the min and max values for defining the 3D space and optionally for coloring.

  for (int32_t i = 0 ; i < data->count ;  i++)
    {
      //  Note that we NEVER check for slicing here because we want the range to be the full range when we are slicing.
      //  This option to check_bounds turns off slice check--\/

      if (!check_bounds (options, misc, data, i, NVTrue, NVFalse))
        {
          (*viewable_count)++;

          bounds->min_x = qMin (bounds->min_x, data->x[i]);
          bounds->max_x = qMax (bounds->max_x, data->x[i]);

          bounds->min_y = qMin (bounds->min_y, data->y[i]);
          bounds->max_y = qMax (bounds->max_y, data->y[i]);


          bounds->min_z = qMin (bounds->min_z, (double) -data->z[i]);
          bounds->max_z = qMax (bounds->max_z, (double) -data->z[i]);
        }
    }


  /*  For later.

  float win_size = options->min_window_size;
  switch (options->iho_min_window)
    {
    case 0:
      win_size = options->min_window_size;
      break;

    case 1:
      win_size = 0.25 + 0.0075 * bounds->min_z;
      break;

    case 2:
      win_size = 0.5 + 0.013 * bounds->min_z;
      break;

    case 3:
      win_size = 1.0 + 0.023 * bounds->min_z;
      break;
    }
  map->setMinZExtents (win_size);
  */


  if (bounds->min_x != prev_bounds.min_x || bounds->min_y != prev_bounds.min_y || bounds->min_z != prev_bounds.min_z ||
      bounds->max_x != prev_bounds.max_x || bounds->max_y != prev_bounds.max_y || bounds->max_z != prev_bounds.max_z)
    {
      prev_bounds = *bounds;

      return (NVTrue);
    }

  return (NVFalse);
}
