
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

/***************************************************************************\
*                                                                           *
*   Module Name:        get_bounds                                          *
*                                                                           *
*   Programmer(s):      Jan C. Depner                                       *
*                                                                           *
*   Date Written:       November 1995                                       *
*                                                                           *
*   Purpose:            Computes the boundaries of the section (page)       *
*                       based on the data in memory.                        *
*                                                                           *
*   Arguments:          None                                                *
*                                                                           *
*   Return Value:       0 if no non-null points found, else count           *
*                                                                           *
\***************************************************************************/

int32_t get_bounds (POINT_DATA *data, MISC *misc)
{
  //  Define the total display area.

  data->bounds.min_x = 999999999.0;
  data->bounds.max_x = -999999999.0;
  data->bounds.min_y = 999999999.0;
  data->bounds.max_y = -999999999.0;
  data->bounds.min_z = CHRTRNULL;
  data->bounds.max_z = -CHRTRNULL;

  double sum_x = 0.0, sum_y = 0.0;
  int32_t count = 0, sum_count = 0;


  //  Loop through the data in memory looking for min and max.

  for (int32_t i = 0 ; i < data->count ; i++)
    {
      //  Always use invalid data for the X and Y extents otherwise you can't turn it back on when you need it.

      data->bounds.min_x = qMin (data->x[i], data->bounds.min_x);
      data->bounds.max_x = qMax (data->x[i], data->bounds.max_x);
      data->bounds.min_y = qMin (data->y[i], data->bounds.min_y);
      data->bounds.max_y = qMax (data->y[i], data->bounds.max_y);
      data->bounds.min_z = qMin ((double) data->z[i], data->bounds.min_z);
      data->bounds.max_z = qMax ((double) data->z[i], data->bounds.max_z);

      switch (misc->data_type)
        {
        case PFM_GSF_DATA:
          if (i && data->rec[i] == data->rec[i - 1] && abs (data->sub[i] - data->sub[i - 1]) < 3)
            {
              sum_x += fabs (data->x[i] - data->x[i - 1]);
              sum_y += fabs (data->y[i] - data->y[i - 1]);
              sum_count++;
            }
          break;

        case PFM_CHARTS_HOF_DATA:
        case PFM_SHOALS_TOF_DATA:
        case PFM_CZMIL_DATA:
        case PFM_LAS_DATA:
          if (i && abs ((int32_t) data->rec[i] - (int32_t) data->rec[i - 1]) < 3)
            {
              sum_x += fabs (data->x[i] - data->x[i - 1]);
              sum_y += fabs (data->y[i] - data->y[i - 1]);
              sum_count++;
            }
          break;
        }

      count ++;
    }


  if (count)
    {
      /*  Add 5% to the window size.  */
        
      double width_5 = (data->bounds.max_x - data->bounds.min_x) * 0.05;
      double height_5 = (data->bounds.max_y - data->bounds.min_y) * 0.05;
      data->bounds.min_y -= height_5;
      data->bounds.max_y += height_5;
      data->bounds.min_x -= width_5;
      data->bounds.max_x += width_5;


      //  The grid size is based on 2 times the average distance between points that are within one or two points of each other.

      data->x_grid_size = (sum_x / (double) sum_count) * 2.0;
      data->y_grid_size = (sum_y / (double) sum_count) * 2.0;
    }

  return (count);
}
