
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
*   Module Name:        kill_records                                        *
*                                                                           *
*   Programmer(s):      Jan C. Depner                                       *
*                                                                           *
*   Date Written:       November 2009                                       *
*                                                                           *
*   Purpose:            Invalidates records (pings) that are contained      *
*                       within a rectangle or polygon.  Unlike set_area,    *
*                       this function determines which records (pings) are  *
*                       contained within the rectangle or polygon and then  *
*                       invalidates all visible records (pings) in the      *
*                       displayed data.  Even those outside of the rectangle*
*                       or polygon.                                         *
*                                                                           *
*   Arguments:          map             -   nvmap object to draw on         *
*                       options         -   OPTIONS structure               *
*                       misc            -   MISC structure                  *
*                       data            -   point data used to define bounds*
*                                                                           *
*   Return Value:       None                                                *
*                                                                           *
\***************************************************************************/

void kill_records (nvMapGL *map, OPTIONS *options, MISC *misc, POINT_DATA *data, int32_t *rb, int32_t x, int32_t y)
{
  int32_t         count = 0, record_count = 0, *px, *py, min_x = 0, max_x = 0, min_y = 0, max_y = 0;
  std::vector<uint32_t> records;
  double          *mx = NULL, *my = NULL;
  uint8_t         scan = NVFalse, hit = NVFalse;


  //  First, get the area to be scanned based on the rectangle or polygon.

  switch (options->function)
    {
    case DELETE_RECTANGLE:
      if (map->rubberbandRectangleIsActive (*rb))
        {
          map->closeRubberbandRectangle (*rb, x, y, &px, &py);

          map->discardRubberbandRectangle (rb);
        }
      count = 4;


      //  Get the minimum bounding rectangle (X and Y in pixels).

      min_y = 9999999;
      max_y = -1;
      min_x = 9999999;
      max_x = -1;
      for (int32_t j = 0 ; j < count ; j++)
        {
          min_y = qMin (min_y, py[j]);
          max_y = qMax (max_y, py[j]);
          min_x = qMin (min_x, px[j]);
          max_x = qMax (max_x, px[j]);
        }

      break;

    case DELETE_POLYGON:

      if (map->rubberbandPolygonIsActive (*rb))
        {
          map->closeRubberbandPolygon (*rb, x, y, &count, &px, &py);


          //  If it's a polygon we have to convert to double so that the "inside" function will work.

          mx = (double *) malloc (count * sizeof (double));

          if (mx == NULL)
            {
              fprintf (stderr, "%s %s %s %d - mx - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
              exit (-1);
            }

          my = (double *) malloc (count * sizeof (double));

          if (my == NULL)
            {
              fprintf (stderr, "%s %s %s %d - my - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
              exit (-1);
            }

          for (int32_t i = 0 ; i < count ; i++)
            {
              mx[i] = (double) px[i];
              my[i] = (double) py[i];
            }

          map->discardRubberbandPolygon (rb);
        }
      break;
    }



  //  Second, scan the area for record numbers and save them in the records array (record_count).

  int32_t cx = -1, cy = -1;
  uint8_t init2D = NVTrue;

  for (int32_t i = 0 ; i < data->count ; i++)
    {
      //  Check against the displayed minimum bounding rectangle.  This is all that is needed for 
      //  a rectangle and it cuts down on the computations for a polygon.

      if (!check_bounds (options, misc, data, i, NVFalse, misc->slice))
        {
          //  Convert the X, Y, and Z value to a projected pixel position

          map->get2DCoords (data->x[i], data->y[i], -data->z[i], &cx, &cy, &init2D);


          scan = NVFalse;

          switch (options->function)
            {
            case DELETE_RECTANGLE:
              if (cx >= min_x && cx <= max_x && cy >= min_y && cy <= max_y) scan = NVTrue;
              break;

            case DELETE_POLYGON:
              if (inside_polygon2 (mx, my, count, (double) cx, (double) cy)) scan = NVTrue;
              break;
            }


          if (scan)
            {
              hit = NVFalse;

              for (int32_t k = 0 ; k < record_count ; k++)
                {
                  if (data->rec[i] == records[k])
                    {
                      hit = NVTrue;
                      break;
                    }
                }

              if (!hit)
                {
                  try
                    {
                      records.resize (record_count + 1);
                    }
                  catch (std::bad_alloc&)
                    {
                      fprintf (stderr, "%s %s %s %d - records - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
                      exit (-1);
                    }

                  records[record_count] = data->rec[i];

                  record_count++;
                }
            }
        }
    }


  if (mx != NULL)
    {
      free (mx);
      free (my);
    }


  //  Third, cycle through all of the visible data and invalidate subrecords of records that showed up in the scanned area.

  if (record_count)
    {
      for (int32_t i = 0 ; i < data->count ; i++)
        {
          for (int32_t j = 0 ; j < record_count ; j++)
            {
              if (data->rec[i] == records[j])
                {
                  //  Save the undo information.

                  store_undo (misc, options->undo_levels, data->val[i], i);


                  data->val[i] |= PFM_MANUALLY_INVAL;
                  break;
                }
            }
        }

      records.clear ();
    }
}
