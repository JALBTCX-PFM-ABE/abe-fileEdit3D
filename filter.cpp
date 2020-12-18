
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
#include "hpFilterThread.hpp"


typedef struct
{
  uint32_t          rec;
  uint32_t          val;
} REC_VAL;


typedef struct
{
  std::vector<REC_VAL>  rv;
  uint32_t          num_soundings;
  float             standard_dev;
  float             avg_filtered_depth;
  uint8_t           data;
} BIN_DEPTH;


static BIN_DEPTH    **bin_depth;
NV_F64_XYMBR        bounds;


#define SQR(x) ((x)*(x))


/***************************************************************************/
/*!

  - Module Name:        compute_bin_values

  - Programmer(s):      Jan C. Depner

  - Date Written:       December 2000

  - Purpose:            Recomputes the bin record values from the depth
                        records.  Only computes the average filtered value,
                        the standard deviation, and the number of valid
                        depths.

  - Arguments:
                        - bin_depth       =   bin_depth record

  - Return Value:
                        - Number of valid points

****************************************************************************/

void compute_bin_values (BIN_DEPTH *bin_depth, POINT_DATA *data)
{
  double sum_filtered = 0.0, sum2_filtered = 0.0;
  int32_t filtered_count = 0;


  for (uint32_t i = 0 ; i < bin_depth->num_soundings ; i++)
    {
      //  DO NOT use records marked as deleted.

      if (!(bin_depth->rv[i].val & PFM_DELETED))
        {
          if (!(bin_depth->rv[i].val & PFM_INVAL))
            {
              sum_filtered += data->z[bin_depth->rv[i].rec];
              sum2_filtered += SQR (data->z[bin_depth->rv[i].rec]);

              filtered_count++;
            }
        }
    }


  if (!filtered_count)
    {
      bin_depth->data = NVFalse;
    }
  else
    {
      bin_depth->avg_filtered_depth = sum_filtered / (double) filtered_count; 
      if (filtered_count > 1)
        {
          bin_depth->standard_dev = sqrt ((sum2_filtered - ((double) filtered_count * (pow ((double) bin_depth->avg_filtered_depth, 2.0)))) / 
                                          ((double) filtered_count - 1.0));
        }
      else
        {
          bin_depth->standard_dev = 0.0;
        }
      bin_depth->data = NVTrue;
    }

  return;
}



/********************************************************************/
/*!
 
 - Function Name :  InitializeAreaFilter
 
 - Description : Perform any initialization specific to the area based
                 filter here.
 
*********************************************************************/

uint8_t InitializeAreaFilter (int32_t width, int32_t height, POINT_DATA *data, MISC *misc)
{
  //  Allocate the memory for all of the bins.

  bin_depth = (BIN_DEPTH **) calloc (height, sizeof (BIN_DEPTH *));

  if (bin_depth == NULL)
    {
      QMessageBox::critical (0, fileEdit3D::tr ("fileEdit3D filter"), fileEdit3D::tr ("Unable to allocate memory for filtering!"));
      return (NVFalse);
    }

  for (int32_t i = 0 ; i < height ; i++)
    {
      bin_depth[i] = (BIN_DEPTH *) calloc (width, sizeof (BIN_DEPTH));

      if (bin_depth[i] == NULL)
        {
          free (bin_depth);
          QMessageBox::critical (0, fileEdit3D::tr ("fileEdit3D filter"), fileEdit3D::tr ("Unable to allocate memory for filtering!"));
          return (NVFalse);
        }
    }


  //  Populate all of the bins with depth data.

  for (int32_t i = 0 ; i < data->count ; i++)
    {
      int32_t row = NINT ((data->y[i] - bounds.min_y) / data->y_grid_size);
      int32_t col = NINT ((data->x[i] - bounds.min_x) / data->x_grid_size);


      //  This should never happen.

      if (row < 0 || col < 0)
        {
          for (int32_t j = 0 ; j < height ; j++)
            {
              if (bin_depth[j] != NULL) 
                {
                  for (int32_t k = 0 ; k < width ; k++)
                    {
                      bin_depth[j][k].rv.clear ();
                    }
                  free (bin_depth[j]);
                }
            }
          return (NVFalse);
        }

      int32_t recnum = bin_depth[row][col].num_soundings;

      try
        {
          bin_depth[row][col].rv.resize (recnum + 1);
        }
      catch (std::bad_alloc&)
        {
          fprintf (stderr, "%s %s %s %d - bin_depth[row][col].rv - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
          exit (-1);
        }


      bin_depth[row][col].rv[recnum].val = data->val[i];
      bin_depth[row][col].rv[recnum].rec = i;

      bin_depth[row][col].num_soundings++;
    }


  //  Now compute the average and standard deviation for the bin.

  for (int32_t i = 0 ; i < height ; i++)
    {
      for (int32_t j = 0 ; j < width ; j++)
        {
          compute_bin_values (&bin_depth[i][j], data);
        }
    }

  return (NVTrue);
}



/********************************************************************/
/*!

 - Function Name : AreaFilter
 
 - Description :   Buffers three rows of PFM data and filters it.
 
 - Inputs :
                   - pfm          = PFM layer number
                   - coord        = current cell coordinate that we are processing.
                   - bin_diagonal = diagonal distance between bins in meters
 
 - Method :        If the slope is low (< 1 degree) we'll use an average of the cell standard
                   deviations to beat the depths against.  Otherwise, we'll compute the
                   standard deviation from the cell averages.  Since we're using the average
                   of the computed standard deviations of all of the nine cells or the 
                   standard deviations of the averages of all nine cells we multiply the
                   resulting standard deviation (?) by two to get a reasonable result,
                   otherwise the standard deviation surface is too smooth and we end up
                   cutting out too much good data.  I must admit I arrived at these numbers
                   by playing with the filter using em3000 shallow water data and em121a deep
                   water data but they appear to work properly.  This way three sigma seems
                   to cut out what you would expect three sigma to cut out.  If you leave it
                   as is it cuts out about 30%.  This is called empirically determining a
                   value (From Nero's famous statement "I'm the emperor and I can do what I
                   damn well please, now hand me my fiddle.").   JCD

********************************************************************/

uint8_t AreaFilter (OPTIONS *options, MISC *misc, double bin_diagonal, int32_t row, int32_t col, double *mx, double *my, int32_t poly_count,
                    POINT_DATA *data)
{
  //  If the center cell has no valid data, return.

  if (!bin_depth[row][col].data) return (NVFalse);


  double avgsum = 0.0;
  double stdsum = 0.0;
  double sum2 = 0.0;
  int32_t sumcount = 0;


  //  Get the information from the 8 cells surrounding this cell (and save the center cell data).

  for (int32_t i = row - 1 ; i <= row + 1 ; i++)
    {
      for (int32_t j = col - 1 ; j <= col + 1 ; j++)
        {
          if (bin_depth[i][j].data)
            {
              avgsum += bin_depth[i][j].avg_filtered_depth;
              stdsum += bin_depth[i][j].standard_dev;
              sum2 += SQR (bin_depth[i][j].avg_filtered_depth);

              sumcount++;
            }
        }
    }


  //  Make sure that at least 5 of the surrounding cells have valid data.  This saves us from deleting a lot of edge points
  //  without any real evidence that they were bad.

  if (sumcount < 5) return (NVFalse);


  //  Compute the eight slopes from the center cell to find out if it's flat enough to use the average of the
  //  standard deviations or if we need to use the standard deviation of the averages.

  uint8_t flat = NVTrue;
  double dx, slope;

  for (int32_t i = row - 1, k = 0 ; i <= row + 1 ; i++, k++)
    {
      for (int32_t j = col - 1 ; j <= col ; j++)
        {
          if (i != row || j != col)
            {
              if (bin_depth[i][j].data)
                {
                  dx = bin_diagonal;

                  slope = (fabs (bin_depth[row][col].avg_filtered_depth - bin_depth[i][j].avg_filtered_depth)) / dx;

                  if (slope > 1.0)
                    {
                      flat = NVFalse;
                      break;
                    }
                }
            }
        }
    }


  int32_t count = 0;

  /*
      If the slope is low (< 1 degree) we'll use an average of the cell standard deviations to beat the 
      depths against.  Otherwise, we'll compute the standard deviation from the cell averages.  Since 
      we're using the average of the computed standard deviations of all of the nine cells or the 
      standard deviations of the averages of all nine cells we multiply the resulting standard
      deviation (?) by two to get a reasonable result, otherwise the standard deviation surface is too
      smooth and we end up cutting out too much good data.  I must admit I arrived at these numbers by
      playing with the filter using em3000 shallow water data and em121a deep water data but they appear
      to work properly.  This way three sigma seems to cut out what you would expect three sigma to cut
      out.  If you leave it as is it cuts out about 30%.  This is called empirically determining a value
      (From Nero's famous statement "I'm the emperor and I can do what I damn well please, now hand me
      my fiddle.").   JCD
  */

  double avg = avgsum / (double) sumcount;
  double std;
  if (flat)
    {
      std = (stdsum / (double) sumcount) * 2.0;
    }
  else
    {
      std = (sqrt ((sum2 - ((double) sumcount * SQR (avg))) / ((double) sumcount - 1.0))) * 2.0;
    }

  double BinSigmaFilter = options->filterSTD * std;


  for (uint32_t i = 0 ; i < bin_depth[row][col].num_soundings ; i++)
    {
      //  Only check those that haven't been checked before.

      if (!(bin_depth[row][col].rv[i].val & (PFM_INVAL | PFM_DELETED)))
        {
          double depth = data->z[bin_depth[row][col].rv[i].rec];


          //  Check in both directions first

          if (fabs (depth - avg) >= BinSigmaFilter)
            {
              //  Check the deep filter only flag and, if set, check in the deep direction only

              if (!data->fmask[bin_depth[row][col].rv[i].rec] && (!options->deep_filter_only || (depth - avg) >= BinSigmaFilter))
                {
                  bin_depth[row][col].rv[i].val |= PFM_MANUALLY_INVAL;


                  //  Only update the record if the point is within the polygon

                  if (inside_polygon2 (mx, my, poly_count, data->x[bin_depth[row][col].rv[i].rec], data->y[bin_depth[row][col].rv[i].rec]))
                    {
                      //  Check for all of the limits on this point.

                      if (!check_bounds (options, misc, data, bin_depth[row][col].rv[i].rec, NVFalse, misc->slice))
                        {
                          try
                            {
                              misc->filter_kill_list.resize (misc->filter_kill_count + 1);
                            }
                          catch (std::bad_alloc&)
                            {
                              fprintf (stderr, "%s %s %s %d - filter_kill_list - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__,
                                       strerror (errno));
                              exit (-1);
                            }

                          misc->filter_kill_list[misc->filter_kill_count] = bin_depth[row][col].rv[i].rec;
                          misc->filter_kill_count++;
                        }
                    }
                }
            }
        }

      count++;
    }


  //  Recompute the bin record based on the modified contents of the depth array.

  compute_bin_values (&bin_depth[row][col], data);


  return (NVTrue);
}



//!  This kicks off the pseudo-statistical filtering of a polygonal area.

uint8_t filterPolyArea (OPTIONS *options, MISC *misc, double *mx, double *my, int32_t count, POINT_DATA *data)
{
  //  Compute the bin diagonal distance for the filter.

  double bin_diagonal = sqrt (SQR (data->x_grid_size) + SQR (data->y_grid_size));


  //  Figure out the width, start row, start column, end row, and end column.

  bounds.min_x = 999999.0;
  bounds.min_y = 999999.0;
  bounds.max_x = -999999.0;
  bounds.max_y = -999999.0;
  for (int32_t i = 0 ; i < count ; i++)
    {
      bounds.min_x = qMin (bounds.min_x, mx[i]);
      bounds.min_y = qMin (bounds.min_y, my[i]);
      bounds.max_x = qMax (bounds.max_x, mx[i]);
      bounds.max_y = qMax (bounds.max_y, my[i]);
    }


  //  Add one grid cell distance around the area just to make sure we get everything

  bounds.min_y -= data->y_grid_size;
  bounds.max_y += data->y_grid_size;
  bounds.min_x -= data->x_grid_size;
  bounds.max_x += data->x_grid_size;


  int32_t width = (NINT ((bounds.max_x - bounds.min_x) / data->x_grid_size)) + 1;
  int32_t height = (NINT ((bounds.max_y - bounds.min_y) / data->y_grid_size)) + 1;


  if (!InitializeAreaFilter (width, height, data, misc)) return (NVFalse);
      

  misc->statusProgLabel->setText (fileEdit3D::tr ("Filtering..."));
  misc->statusProgLabel->setVisible (true);
  misc->statusProg->setRange (0, height);
  misc->statusProg->setTextVisible (true);
  qApp->processEvents();


  //  Note that we're not filtering the edge cells since we won't have surrounding data.

  for (int32_t i = 1 ; i < height - 1 ; i++)
    {
      misc->statusProg->setValue (i);


      for (int32_t j = 1 ; j < width - 1 ; j++)
        {
          AreaFilter (options, misc, bin_diagonal, i, j, mx, my, count, data);
        }


      //  Check the event queue to see if the user wants to interrupt the filter.  Only 
      //  check every 10th row so it doesn't slow things down too much.  */

      if (!(i % 10))
        {
          if (qApp->hasPendingEvents ())
            {
              qApp->processEvents();
              if (misc->drawing_canceled)
                {
                  misc->filter_kill_count = 0;
                  misc->statusProg->reset ();
                  misc->statusProg->setRange (0, 100);
                  misc->statusProg->setValue (0);
                  misc->statusProgLabel->setVisible (false);
                  misc->statusProg->setTextVisible (false);
                  misc->drawing_canceled = NVFalse;
                  qApp->processEvents();
                  return (NVFalse);
                }
            }
        }
    }

  misc->statusProg->reset ();
  misc->statusProg->setRange (0, 100);
  misc->statusProg->setValue (0);
  misc->statusProgLabel->setVisible (false);
  misc->statusProg->setTextVisible (false);

  qApp->processEvents();


  //  Free the memory used by the filter.

  for (int32_t i = 0 ; i < height ; i++)
    {
      if (bin_depth[i] != NULL) 
        {
          for (int32_t j = 0 ; j < width ; j++)
            {
              bin_depth[i][j].rv.clear ();
            }
          free (bin_depth[i]);
        }
    }

  return (NVTrue);
}



uint8_t HPFilter (OPTIONS *options, MISC *misc, POINT_DATA *data)
{
  typedef struct
  {
    float          x;
    float          y;
    int32_t        count;
    uint8_t        killed;
  } POS_DATA;

  typedef struct
  {
    int32_t        count;
    std::vector<int32_t>  data;
  } BIN_DATA;


  POS_DATA *pos = (POS_DATA *) calloc (data->count, sizeof (POS_DATA));

  if (pos == NULL)
    {
      fprintf (stderr, "%s %s %s %d - pos - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }


  //  Populate the position array with the position of the point in meters from the southwest corner of the area.  This allows us to
  //  directly compute distance between points using just the sqrt function.  Since we will be comparing distances between every point
  //  in a 9 bin area against every other point in a 9 bin area, and doing that for every bin in the entire area, this will save loads
  //  of time.

  for (int32_t i = 0 ; i < data->count ; i++)
    {
      double x, y, az;
      invgp (NV_A0, NV_B0, data->bounds.min_y, data->bounds.min_x, data->bounds.min_y, data->x[i],
             &x, &az);
      pos[i].x = (float) x;
      invgp (NV_A0, NV_B0, data->bounds.min_y, data->bounds.min_x, data->y[i], data->bounds.min_x,
             &y, &az);
      pos[i].y = (float) y;

      pos[i].count = 0;
      pos[i].killed = NVFalse;
    }


  //  We need to build an array of bins (twice the size of the search radius) so that we can efficiently perform the dreaded
  //  Hockey Puck of Confidence (TM) proximity valid point search.

  double search_bin_size_meters = options->hp_filter_radius * 2.0;
  double width_meters, height_meters, az;


  invgp (NV_A0, NV_B0, data->bounds.min_y, data->bounds.min_x, data->bounds.max_y,
         data->bounds.min_x, &height_meters, &az);
  invgp (NV_A0, NV_B0, data->bounds.min_y, data->bounds.min_x, data->bounds.min_y,
         data->bounds.max_x, &width_meters, &az);


  int32_t rows = (int32_t) (height_meters / search_bin_size_meters) + 1;
  int32_t cols = (int32_t) (width_meters / search_bin_size_meters) + 1;

  BIN_DATA **bin_data = (BIN_DATA **) calloc (rows, sizeof (BIN_DATA *));
  if (bin_data == NULL)
    {
      fprintf (stderr, "%s %s %s %d - bin_data - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  for (int32_t i = 0 ; i < rows ; i++)
    {
      bin_data[i] = (BIN_DATA *) calloc (cols, sizeof (BIN_DATA));
      if (bin_data[i] == NULL)
        {
          fprintf (stderr, "%s %s %s %d - bin_data[i] - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
          exit (-1);
        }
    }


  float hp_half_height = options->hp_filter_height / 2.0;


  //  Now let's load the record pointers into the bin array.

  for (int32_t i = 0 ; i < data->count ; i++)
    {
      int32_t row = (int32_t) (pos[i].y / search_bin_size_meters);
      int32_t col = (int32_t) (pos[i].x / search_bin_size_meters);

      try
        {
          bin_data[row][col].data.resize (bin_data[row][col].count + 1);
        }
      catch (std::bad_alloc&)
        {
          fprintf (stderr, "%s %s %s %d - bin_data[row][col].data - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__,
                   strerror (errno));
          exit (-1);
        }

      bin_data[row][col].data[bin_data[row][col].count] = i;
      bin_data[row][col].count++;
    }


  uint8_t ret = NVFalse;


  //  Determine which points need to be deleted.  This uses the dreaded Hockey Puck of Confidence (TM).  We only want
  //  to search in one bin around the current bin.  This means we'll search 9 total bins and that should give us enough
  //  nearby data for any point in the center bin.  We have to run through this process iteratively since a point may
  //  be deemed good based on nearby points that might later be deemed invalid.  To do this we run the loop until we
  //  don't kill any more points.

  int32_t new_killed = 0;
  do
    {
      new_killed = 0;
      for (int32_t i = 0 ; i < data->count ; i++) pos[i].count = 0;

      for (int32_t i = 0 ; i < rows ; i++)
        {
          //  Compute the start and end Y bins for the 9 bin block (min = 0, max = rows - 1).

          int32_t start_y = qMax (0, i - 1);
          int32_t end_y = qMin (rows - 1, i + 1);

          for (int32_t j = 0 ; j < cols ; j++)
            {
              //  No point in checking if we have no data points in the bin.

              if (bin_data[i][j].count)
                {
                  //  Compute the start and end X bins for the 9 bin block (min = 0, max = cols - 1).

                  int32_t start_x = qMax (0, j - 1);
                  int32_t end_x = qMin (cols - 1, j + 1);


                  //  Loop through the current bin checking against all points in any of the 9 bins.

                  for (int32_t k = 0 ; k < bin_data[i][j].count ; k++)
                    {
                      int32_t ndx = bin_data[i][j].data[k];


                      //  If the point is already invalid we certainly don't need to check it.

                      if (!(data->val[ndx] & (PFM_INVAL | PFM_DELETED | PFM_REFERENCE)) && !pos[ndx].killed)
                        {
                          //  Y bin block loop.

                          for (int32_t m = start_y ; m <= end_y ; m++)
                            {
                              //  X bin block loop.

                              for (int32_t n = start_x ; n <= end_x ; n++)
                                {
                                  //  Loop though all points in the bin.

                                  for (int32_t p = 0 ; p < bin_data[m][n].count ; p++)
                                    {
                                      int32_t indx = bin_data[m][n].data[p];


                                      //  Don't check against itself and don't check against invalid data.

                                      if (ndx != indx && (!(data->val[indx] & (PFM_INVAL | PFM_DELETED | PFM_REFERENCE))) && (!pos[indx].killed))
                                        {
                                          //  Simple check for exceeding distance in X or Y direction (prior to a radius check).

                                          double diff_x = fabs (pos[ndx].x - pos[indx].x);
                                          double diff_y = fabs (pos[ndx].y - pos[indx].y);


                                          //  The radius plus both horizontal uncertainties.

                                          double dist = options->hp_filter_radius;// + data->herr[ndx] + data->herr[indx];


                                          //  Check the Z difference.

                                          if (fabs (data->z[ndx] - data->z[indx]) < hp_half_height)
                                            {
                                              //  Simple check for either X or Y exceeding our distance (so we don't do too many SQRTs)

                                              if (diff_x <= dist && diff_y <= dist)
                                                {
                                                  //  Check the distance.

                                                  if (sqrt (diff_x * diff_x + diff_y * diff_y) <= dist)
                                                    {
                                                      pos[ndx].count++;


                                                      //  If we've met our quota for validity let's go ahead and stop checking points.

                                                      if (pos[ndx].count >= options->hp_filter_neighbors) break;
                                                    }
                                                }
                                            }
                                        }
                                    }


                                  //  Already met our quota so stop the X bin block loop.

                                  if (pos[ndx].count >= options->hp_filter_neighbors) break;
                                }


                              //  Already met our quota so stop the Y bin block loop.

                              if (pos[ndx].count >= options->hp_filter_neighbors) break;
                            }


                          //  If we don't meet the HP filter criteria, add this point to the filter kill list and set it's temporary invalid flag
                          //  (killed) so we don't use it to validate other points.

                          if (pos[ndx].count < options->hp_filter_neighbors)
                            {
                              try
                                {
                                  misc->filter_kill_list.resize (misc->filter_kill_count + 1);
                                }
                              catch (std::bad_alloc&)
                                {
                                  fprintf (stderr, "%s %s %s %d - filter_kill_list - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__,
                                           strerror (errno));
                                  exit (-1);
                                }

                              misc->filter_kill_list[misc->filter_kill_count] = ndx;
                              misc->filter_kill_count++;

                              pos[ndx].killed = NVTrue;


                              //  Add one to the while loop's kill count so we'll know when to stop.

                              new_killed++;


                              ret = NVTrue;
                            }
                        }
                    }
                }
            }
        }
    } while (new_killed);


  //  Free all of the memory we allocated.

  for (int32_t i = 0 ; i < rows ; i++)
    {
      for (int32_t j = 0 ; j < cols ; j++)
        {
          bin_data[i][j].data.clear ();
        }
      free (bin_data[i]);
    }
  free (bin_data);

  free (pos);

  return (ret);
}



uint8_t startHpFilterThread (OPTIONS *options, MISC *misc, POINT_DATA *data)
{
  POS_DATA *pos = (POS_DATA *) calloc (data->count, sizeof (POS_DATA));

  if (pos == NULL)
    {
      fprintf (stderr, "%s %s %s %d - pos - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }


  //  Populate the position array with the position of the point in meters from the southwest corner of the area.  This allows us to
  //  directly compute distance between points using just the sqrt function.  Since we will be comparing distances between every point
  //  in a 9 bin area against every other point in a 9 bin area, and doing that for every bin in the entire area, this will save loads
  //  of time.

  for (int32_t i = 0 ; i < data->count ; i++)
    {
      double x, y, az;
      invgp (NV_A0, NV_B0, data->bounds.min_y, data->bounds.min_x, data->bounds.min_y, data->x[i],
             &x, &az);
      pos[i].x = (float) x;
      invgp (NV_A0, NV_B0, data->bounds.min_y, data->bounds.min_x, data->y[i], data->bounds.min_x,
             &y, &az);
      pos[i].y = (float) y;

      pos[i].count = 0;
      pos[i].killed = NVFalse;
    }


  //  We need to build an array of bins (twice the size of the search radius) so that we can efficiently perform the dreaded
  //  Hockey Puck of Confidence (TM) proximity valid point search.

  double search_bin_size_meters = options->hp_filter_radius * 2.0;
  double width_meters, height_meters, az;


  invgp (NV_A0, NV_B0, data->bounds.min_y, data->bounds.min_x, data->bounds.max_y,
         data->bounds.min_x, &height_meters, &az);
  invgp (NV_A0, NV_B0, data->bounds.min_y, data->bounds.min_x, data->bounds.min_y,
         data->bounds.max_x, &width_meters, &az);


  int32_t rows = (int32_t) (height_meters / search_bin_size_meters) + 1;
  int32_t cols = (int32_t) (width_meters / search_bin_size_meters) + 1;

  BIN_DATA **bin_data = (BIN_DATA **) calloc (rows, sizeof (BIN_DATA *));
  if (bin_data == NULL)
    {
      fprintf (stderr, "%s %s %s %d - bin_data - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  for (int32_t i = 0 ; i < rows ; i++)
    {
      bin_data[i] = (BIN_DATA *) calloc (cols, sizeof (BIN_DATA));
      if (bin_data[i] == NULL)
        {
          fprintf (stderr, "%s %s %s %d - bin_data[i] - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
          exit (-1);
        }
    }


  //  Now let's load the record pointers into the bin array.

  for (int32_t i = 0 ; i < data->count ; i++)
    {
      int32_t row = (int32_t) (pos[i].y / search_bin_size_meters);
      int32_t col = (int32_t) (pos[i].x / search_bin_size_meters);

      try
        {
          bin_data[row][col].data.resize (bin_data[row][col].count + 1);
        }
      catch (std::bad_alloc&)
        {
          fprintf (stderr, "%s %s %s %d - bin_data[row][col].data - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__,
                   strerror (errno));
          exit (-1);
        }

      bin_data[row][col].data[bin_data[row][col].count] = i;
      bin_data[row][col].count++;
    }


  //  We want to create the maximum possible number of threads.

  hpFilterThread hp_filter_thread[4][4];


  //  Here we're going to make sure that the box is large enough in bins (rows/columns) to thread it.  There's no point in trying to
  //  break 4 bins up into two sections.  Our cutoff is 8 bins or 16 bins (for 4 or 16 threads respectively).

  int32_t width = 0, height = 0;

  if (rows < 8)
    {
      height = 1;
    }
  else if (rows < 18)
    {
      height = 2;
    }

  if (cols < 8)
    {
      width = 1;
    }
  else if (cols < 18)
    {
      width = 2;
    }


  //  If we haven't limited our width or height compute the number of blocks in width and height.  Since num_hp_threads will either be
  //  4 or 16 this will be either 2 or 4 respectively.

  int32_t num_half_blocks = NINT (sqrt ((double) options->num_hp_threads));

  if (!width) width = num_half_blocks;
  if (!height) height = num_half_blocks;


  int32_t adjusted_num_threads = width * height;


  std::vector<int32_t> *kill_list = (std::vector<int32_t> *) calloc (adjusted_num_threads, sizeof (std::vector<int32_t>));

  if (kill_list == NULL)
    {
      fprintf (stderr, "%s %s %s %d - kill_list - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  int32_t *kill_count = (int32_t *) calloc (adjusted_num_threads, sizeof (int32_t));

  if (kill_count == NULL)
    {
      fprintf (stderr, "%s %s %s %d - kill_count - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }


  //  We're starting all adjusted_num_threads filter passes concurrently.
 
  for (int32_t i = 0 ; i < height ; i++)
    {
      for (int32_t j = 0 ; j < width ; j++)
        {
          int32_t ndx = i * width + j;

          hp_filter_thread[i][j].filter (options, misc, data, bin_data, pos, rows, cols, width, height, i, j, &kill_list[ndx], &kill_count[ndx]);
        }
    }


  //  Wait until all of the threads are finished.  Since the Hockey Puck filter is an iterative process we can't really give a status so
  //  there's no need to play araound with signals from the threadds.

  for (int32_t i = 0 ; i < height ; i++)
    {
      for (int32_t j = 0 ; j < width ; j++)
        {
          hp_filter_thread[i][j].wait ();
        }
    }


  //  Copy the individual thread kill lists into the filter_kill_list.

  for (int32_t i = 0 ; i < height ; i++)
    {
      for (int32_t j = 0 ; j < width ; j++)
        {
          int32_t ndx = i * width + j;

          for (int32_t k = 0 ; k < kill_count[ndx] ; k++)
            {
              try
                {
                  misc->filter_kill_list.resize (misc->filter_kill_count + 1);
                }
              catch (std::bad_alloc&)
                {
                  fprintf (stderr, "%s %s %s %d - filter_kill_list - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__,
                           strerror (errno));
                  exit (-1);
                }

              misc->filter_kill_list[misc->filter_kill_count] = kill_list[ndx][k];
              misc->filter_kill_count++;
            }

          kill_list[ndx].clear ();
        }
    }


  //  Free all of the memory we allocated.

  free (kill_list);
  free (kill_count);

  for (int32_t i = 0 ; i < rows ; i++)
    {
      for (int32_t j = 0 ; j < cols ; j++)
        {
          bin_data[i][j].data.clear ();
        }
      free (bin_data[i]);
    }
  free (bin_data);

  free (pos);

  return (NVTrue);
}



uint8_t Filter (OPTIONS *options, MISC *misc, double *mx, double *my, int32_t count, POINT_DATA *data)
{
  uint8_t ret = NVFalse;


  switch (options->filter_type)
    {
    case AREA_BASED_FILTER:
      ret = filterPolyArea (options, misc, mx, my, count, data);
      break;

    case HP_FILTER:
      if (options->num_hp_threads == 1)
        {
          ret = HPFilter (options, misc, data);
        }
      else
        {
          ret = startHpFilterThread (options, misc, data);
        }
      break;
    }


  return (ret);
}
