
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

void actionKey (fileEdit3D *parent, nvMapGL *map __attribute__ ((unused)), POINT_DATA *data __attribute__ ((unused)),
                OPTIONS *options __attribute__ ((unused)), MISC *misc, QString key, int32_t lock_point __attribute__ ((unused)))
{
  misc->abeShare->lock ();


  //  We set the modcode to 0 (i.e. nothing has changed)

  misc->abe_share->modcode = 0;


  //  We then set the action key for the external program (it should be monitoring
  //  misc->abe_share->key for it to change).

  misc->abe_share->key = (uint32_t) key.toLatin1()[0];


  misc->abeShare->unlock ();


  //  Then we wait at least one second for something to be done in the external program

  uint8_t hit = NVFalse;
  QTime t;
  t.start();
  do
    {
      //  The external program should set modcode to it's data type to signify that it has 
      //  done something

      if (misc->abe_share->modcode)
        {
          if (misc->abe_share->modcode == NO_ACTION_REQUIRED) break;


          //  After the external program sets modcode we want to lock shared memory.

          misc->abeShare->lock ();


          //  Unfortunately this is data type specific, which I really hate to put in here,
          //  but we have no other choice that I can see.

          switch (misc->abe_share->modcode)
            {
            default:
              break;
            }

          misc->abe_share->key = 0;


          //  Unlock it after we have processed the data.

          misc->abeShare->unlock ();


          break;
        }
    } while (t.elapsed () < 1000);


  if (hit) 
    {
      parent->redrawMap (NVFalse);
    }
}
