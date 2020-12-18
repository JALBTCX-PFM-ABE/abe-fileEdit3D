
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



#include "fileEdit3DDef.hpp"

/***************************************************************************\
*                                                                           *
*   Module Name:        overlay_flag                                        *
*                                                                           *
*   Programmer(s):      Jan C. Depner                                       *
*                                                                           *
*   Date Written:       November 1995                                       *
*                                                                           *
*   Purpose:            Draw flag locations in the GC and the pixmap.       *
*                                                                           *
*   Arguments:          None                                                *
*                                                                           *
*   Return Value:       None                                                *
*                                                                           *
*   Calling Routines:   utility routine                                     *
*                                                                           * 
\***************************************************************************/

void overlayFlag (nvMapGL *map, OPTIONS *options, MISC *misc, POINT_DATA *data)
{
  QColor              tmp;


  //  Clear the marker points.

  map->clearMarkerPoints ();


  //  We can only display one group of markers (it gets too confusing to look at otherwise).  These are the filter kill points.

  if (misc->filter_kill_count)
    {
      //  Turn off highlighted points.

      if (misc->highlight_count)
        {
          misc->highlight.clear ();
          misc->highlight_count = 0;
        }


      for (int32_t i = 0 ; i < misc->filter_kill_count ; i++)
        {
          //  Negative one means we've masked the point after filtering.

          if (misc->filter_kill_list[i] >= 0)
            {
              int32_t j = misc->filter_kill_list[i];

              if (!check_bounds (options, misc, data, j, NVTrue, misc->slice))
                map->setMarkerPoints (data->x[j], data->y[j], -data->z[j], options->edit_color, 0.01, NVFalse);
            }
        }
      map->setMarkerPoints (0.0, 0.0, 0.0, Qt::black, 0, NVTrue);
    }


  //  These are the data type or PFM_USER flagged points.

  else if (options->flag_index)
    {
      //  Turn off highlighted points.

      if (misc->highlight_count)
        {
          misc->highlight.clear ();
          misc->highlight_count = 0;
        }


      uint32_t check = 0;


      switch (options->flag_index)
        {
        case 1:
          check = PFM_SUSPECT;
          break;

        case 2:
          check = PFM_INVAL;
          break;
        }

      for (int32_t i = 0 ; i < data->count ; i++)
        {
          if (!check_bounds (options, misc, data, i, NVTrue, misc->slice))
            {
              if (data->val[i] & check)
                map->setMarkerPoints (data->x[i], data->y[i], -data->z[i], options->edit_color, 0.01, NVFalse);
            }
        }
      map->setMarkerPoints (0.0, 0.0, 0.0, Qt::black, 0, NVTrue);
    }


  //  These are highlight points

  else
    {
      for (int32_t i = 0 ; i < misc->highlight_count ; i++)
        {
          int32_t j = misc->highlight[i];

          if (!check_bounds (options, misc, data, j, NVTrue, misc->slice))
            map->setMarkerPoints (data->x[j], data->y[j], -data->z[j], options->edit_color, 0.01, NVFalse);
        }
      map->setMarkerPoints (0.0, 0.0, 0.0, Qt::black, 0, NVTrue);
    }
}
