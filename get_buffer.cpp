
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
*   Module Name:        get_buffer                                          *
*                                                                           *
*   Programmer(s):      Jan C. Depner                                       *
*                                                                           *
*   Date Written:       May 1999                                            *
*                                                                           *
*   Purpose:            Get the rectangle of data from the file.            *
*                                                                           *
*   Return Value:       None                                                *
*                                                                           *
\***************************************************************************/

void get_buffer (POINT_DATA *data, MISC *misc, int32_t record)
{
  int32_t get_bounds (POINT_DATA *data, MISC *misc);


  misc->min_z = 99999999.0;
  misc->max_z = -99999999.0;


  //  Flush all modified points in the last buffer.
    
  misc->statusProgLabel->setText (fileEdit3D::tr ("Writing..."));
  misc->statusProgLabel->setVisible (true);
  misc->statusProg->setRange (0, data->count);
  misc->statusProg->setTextVisible (true);
  qApp->processEvents();

  for (int32_t i = 0 ; i < data->count ; i++)
    {
      if (data->oval[i] != data->val[i])
        {
          misc->statusProg->setValue (i);

          io_data_write (data->val[i], data->rec[i], data->sub[i], data->time[i]);
	}
    }

  misc->statusProg->reset ();
  misc->statusProg->setRange (0, 100);
  misc->statusProg->setValue (0);
  misc->statusProgLabel->setVisible (false);
  misc->statusProg->setTextVisible (false);
  qApp->processEvents ();


  //  Set the record number.

  set_record (record);


  //  Read the buffer.

  misc->statusProgLabel->setText (fileEdit3D::tr ("Reading..."));
  misc->statusProgLabel->setVisible (true);
  misc->statusProg->setTextVisible (true);
  qApp->processEvents();

  io_data_read (misc->num_records, data, misc->statusProg);

  misc->statusProg->reset ();
  misc->statusProg->setRange (0, 100);
  misc->statusProg->setValue (0);
  misc->statusProgLabel->setVisible (false);
  misc->statusProg->setTextVisible (false);
  qApp->processEvents ();


  //  Compute the total area bounds.

  get_bounds (data, misc);


  //  Set the 'modified' records.

  for (int32_t i = 0 ; i < data->count ; i++) data->oval[i] = data->val[i];
}
