
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

int32_t pseudo_dist_from_viewer (MISC *misc, POINT_DATA *data, double x, double y);

/***************************************************************************\
*                                                                           *
*   Module Name:        check_bounds                                        *
*                                                                           *
*   Programmer(s):      Jan C. Depner                                       *
*                                                                           *
*   Date Written:       July 2000 (On vacation in Disney World, no less)    *
*                                                                           *
*   Purpose:            Check all of the conditions for displaying a beam.  *
*                                                                           *
*   Arguments:          ck       -   NVFalse - no validity check            *
*                       slice    -   NVFalse - no slice check               *
*                                                                           *
*   Return Value:       2 - don't display, 1 - display translucent,         *
*                       0 - display in full color                           *
*                                                                           *
\***************************************************************************/

int32_t check_bounds (OPTIONS *options, MISC *misc, POINT_DATA *data, double x, double y, uint32_t v, uint8_t mask, uint8_t ck, uint8_t slice)
{
  int32_t          iz;


  //  Easy checks first, that way, if we can eliminate this point we don't have to do 
  //  the edge or inside check.  Check for displaying invalid, masked, then displaying class data.

  //  If this point is masked...

  if (mask) return (2);


  //  If the point is invalid (only if we're not displaying invalid).

  if (ck && !options->display_invalid && ((v & PFM_INVAL))) return (2);


  //  Now check the bounds (this will always be an MBR).

  if (x < data->bounds.min_x || x > data->bounds.max_x || y < data->bounds.min_y || y > data->bounds.max_y) return (2);


  //  Check for slicing if it is enabled.

  if (slice)
    {
      iz = pseudo_dist_from_viewer (misc, data, x, y);
      if (iz < misc->slice_min || iz > misc->slice_max) return (1);
    }


  return (0);
}



int32_t check_bounds (OPTIONS *options, MISC *misc, POINT_DATA *data, int32_t index, uint8_t ck, uint8_t slice)
{
  double           x = data->x[index];
  double           y = data->y[index];
  uint32_t         v = data->val[index];
  uint8_t          m = data->mask[index];

  return (check_bounds (options, misc, data, x, y, v, m, ck, slice));
}
