
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



#include "io_data.hpp"
#include "LAS_WKT.hpp"


static int32_t              type, curr_record, end_record;
static FILE                 *fp;
static MISC                 *misc;
static uint8_t              no_edit;


//  CHARTS

static HYDRO_OUTPUT_T       hof_record;
static TOPO_OUTPUT_T        tof_record;
static HOF_HEADER_T         hof_header;
static TOF_HEADER_T         tof_header;
static int32_t              year, day, hour, minute;
static float                second;


//  CZMIL

static int32_t              cpf_handle;
static CZMIL_CPF_Data       cpf_record;
static CZMIL_CPF_Header     cpf_header;


//  GSF

static int32_t              gsf_handle;
static gsfDataID            gsf_data_id;
static gsfRecords           gsf_records;
static int32_t              last_rec = -1;


//  LAS

static LASreadOpener        lasreadopener;
static LASreader            *lasreader;
static LASheader            lasheader;
static SLAS_POINT_DATA      slas;
static FILE                 *las_fp;
static uint8_t              endian;


static QProgressBar *indexProg;


static void indexProgress (int state, int percent)
{
  static uint8_t second = NVFalse;

  if (state == 1) second = NVFalse;

  if (!second && state == 2) 
    {
      indexProg->reset ();
      second = NVTrue;
    }
      
  if (state == 2 || percent < 100) indexProg->setValue (percent);

  qApp->processEvents ();
}



static void resize_record (POINT_DATA *data, int32_t num)
{
  try
    {
      data->x.resize (num + 1);
    }
  catch (std::bad_alloc&)
    {
      fprintf (stderr, "%s %s %s %d - data.x - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  try
    {
      data->y.resize (num + 1);
    }
  catch (std::bad_alloc&)
    {
      fprintf (stderr, "%s %s %s %d - data.y - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  try
    {
      data->z.resize (num + 1);
    }
  catch (std::bad_alloc&)
    {
      fprintf (stderr, "%s %s %s %d - data.z - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  try
    {
      data->val.resize (num + 1);
    }
  catch (std::bad_alloc&)
    {
      fprintf (stderr, "%s %s %s %d - data.val - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  try
    {
      data->oval.resize (num + 1);
    }
  catch (std::bad_alloc&)
    {
      fprintf (stderr, "%s %s %s %d - data.oval - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  try
    {
      data->rec.resize (num + 1);
    }
  catch (std::bad_alloc&)
    {
      fprintf (stderr, "%s %s %s %d - data.rec - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  try
    {
      data->sub.resize (num + 1);
    }
  catch (std::bad_alloc&)
    {
      fprintf (stderr, "%s %s %s %d - data.sub - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  try
    {
      data->time.resize (num + 1);
    }
  catch (std::bad_alloc&)
    {
      fprintf (stderr, "%s %s %s %d - data.time - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  try
    {
      data->mask.resize (num + 1);
    }
  catch (std::bad_alloc&)
    {
      fprintf (stderr, "%s %s %s %d - data.mask - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }

  try
    {
      data->fmask.resize (num + 1);
    }
  catch (std::bad_alloc&)
    {
      fprintf (stderr, "%s %s %s %d - data.fmask - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      exit (-1);
    }
}



int32_t io_data_open (QWidget *parent, MISC *msc, OPTIONS *options)
{
  time_t sec;
  long nsec;


  misc = msc;

  strcpy (misc->filename, misc->file.toLatin1 ());

  misc->data_type = -1;

  no_edit = misc->no_edit;


  //  Check for HOF file.

  if (misc->file.endsWith (".hof"))
    {
      if ((fp = open_hof_file (misc->filename)) == NULL) return (-1);

      hof_read_header (fp, &hof_header);

      if (strstr (hof_header.text.file_type, "Hydrographic Output File"))
        {
          misc->data_type = type = PFM_CHARTS_HOF_DATA;
          misc->lidar_present = misc->hydro_lidar_present = NVTrue;
        }
      else
        {
          fclose (fp);
          return (-1);
        }


      fclose (fp);
    }


  //  Check for TOF file.

  if (misc->file.endsWith (".tof"))
    {
      if ((fp = open_tof_file (misc->filename)) == NULL) return (-1);

      tof_read_header (fp, &tof_header);

      if (strstr (tof_header.text.file_type, "Topographic Output File"))
        {
          misc->data_type = type = PFM_SHOALS_TOF_DATA;
          misc->lidar_present = NVTrue;
          misc->hydro_lidar_present = NVFalse;
        }
      else
        {
          fclose (fp);
          return (-1);
        }

      fclose (fp);
    }


  //  Check for CZMIL CPF file.

  if (misc->file.endsWith (".cpf"))
    {
      if ((cpf_handle = czmil_open_cpf_file (misc->filename, &cpf_header, CZMIL_UPDATE)) >= 0)
        {
          misc->data_type = type = PFM_CZMIL_DATA;

          misc->lidar_present = misc->hydro_lidar_present = NVTrue;

          czmil_close_cpf_file (cpf_handle);
        }
    }


  //  Check for LAS file.

  if (misc->file.endsWith (".las") || misc->file.endsWith (".LAS"))
    {
      //  Open the LAS file.

      lasreadopener.set_file_name (misc->filename);
      lasreader = lasreadopener.open ();
      if (!lasreader)
        {
          fprintf (stderr, "\n\n*** ERROR ***\nUnable to open LAS file %s\n", misc->filename);
          fflush (stderr);
          return (NVFalse);
        }


      endian = big_endian ();

      misc->data_type = type = PFM_LAS_DATA;

      misc->lidar_present = misc->las_present = NVTrue;

      lasreader->close ();
    }


  if (misc->data_type == PFM_CHARTS_HOF_DATA)
    {
      if ((fp = open_hof_file (misc->filename)) == NULL) return (-1);


      hof_read_header (fp, &hof_header);

      misc->num_subrecords = 2;
      misc->num_records = options->point_limit / misc->num_subrecords;

      charts_cvtime (hof_header.text.start_timestamp, &year, &day, &hour, &minute, &second);
      misc->start_time = normtime (year, day, hour, minute, second);

      charts_cvtime (hof_header.text.end_timestamp, &year, &day, &hour, &minute, &second);
      misc->end_time = normtime (year, day, hour, minute, second);

      misc->total_records = hof_header.text.number_shots;

      misc->start_record = 1;
      misc->end_record = end_record = misc->total_records;

      curr_record = 1;
    }
  else if (misc->data_type == PFM_SHOALS_TOF_DATA)
    {
      if ((fp = open_tof_file (misc->filename)) == NULL) return (-1);

      tof_read_header (fp, &tof_header);


      misc->num_subrecords = 2;
      misc->num_records = options->point_limit / misc->num_subrecords;

      charts_cvtime (tof_header.text.start_timestamp, &year, &day, &hour, &minute, &second);
      misc->start_time = normtime (year, day, hour, minute, second);

      charts_cvtime (tof_header.text.end_timestamp, &year, &day, &hour, &minute, &second);
      misc->end_time = normtime (year, day, hour, minute, second);

      misc->total_records = tof_header.text.number_shots;

      misc->start_record = 1;
      misc->end_record = end_record = misc->total_records;

      curr_record = 1;
    }
  else if (misc->data_type == PFM_CZMIL_DATA)
    {
      if ((cpf_handle = czmil_open_cpf_file (misc->filename, &cpf_header, CZMIL_UPDATE)) < 0) return (-1);


      //  We're guessing at the average number of subrecords here.

      misc->num_subrecords = 16;
      misc->num_records = options->point_limit / misc->num_subrecords;

      czmil_cvtime (cpf_header.flight_start_timestamp, &year, &day, &hour, &minute, &second);
      misc->start_time = normtime (year, day, hour, minute, second);


      czmil_cvtime (cpf_header.flight_end_timestamp, &year, &day, &hour, &minute, &second);
      misc->end_time = normtime (year, day, hour, minute, second);


      misc->total_records = cpf_header.number_of_records;

      misc->start_record = 0;
      misc->end_record = end_record = misc->total_records - 1;

      curr_record = 0;
    }
  else if (misc->data_type == PFM_LAS_DATA)
    {
      lasreadopener.set_file_name (misc->filename);
      lasreader = lasreadopener.open ();
      if (!lasreader)
        {
          fprintf (stderr, "\n\n*** ERROR ***\nUnable to open LAS file %s\n", misc->filename);
          fflush (stderr);
          return (-1);
        }


      lasheader = lasreader->header;


      //  Check to see if we can figure out what the hell kind of coordinate system the LAS file is using.

      LAS_WKT (parent, options, misc, &lasheader);


      //  Now close it since all we really wanted was the header.

      lasreader->close ();


      if ((las_fp = fopen64 (misc->filename, "rb+")) == NULL)
        {
          fprintf (stderr, "\n\n*** ERROR ***\nUnable to open LAS file %s\n", misc->filename);
          fflush (stderr);
          return (-1);
        }

      misc->num_subrecords = 1;
      misc->num_records = options->point_limit / misc->num_subrecords;


      //  Get the correct number of point records based on the LAS version.

      misc->total_records = (uint64_t) lasheader.number_of_point_records;
      if (lasheader.version_minor == 4) misc->total_records = lasheader.extended_number_of_point_records;


      misc->start_record = 0;
      misc->end_record = end_record = misc->total_records - 1;

      curr_record = 0;
    }
  else
    {
      indexProg = misc->statusProg;
      indexProg->setRange (0, 100);

      gsf_register_progress_callback (indexProgress);

      if (gsfOpen (misc->filename, GSF_UPDATE_INDEX, &gsf_handle)) return (-1);


      //  Read one record to get the number of beams available.

      gsf_data_id.record_number = 1;
      if ((gsfRead (gsf_handle, GSF_RECORD_SWATH_BATHYMETRY_PING, &gsf_data_id, &gsf_records, NULL, 0)) != -1)
        {
          misc->num_subrecords = gsf_records.mb_ping.number_beams;
        }
      else
        {
          misc->num_subrecords = 1024;
        }
      misc->num_records = options->point_limit / misc->num_subrecords;


      indexProg->setValue (100);


      misc->data_type = type = PFM_GSF_DATA;


      misc->gsf_present = NVTrue;


      gsfIndexTime (gsf_handle, GSF_RECORD_SWATH_BATHYMETRY_PING, 1, &sec, &nsec);
      misc->start_time = norm_posix_time (sec, nsec);
        
      misc->total_records = gsfIndexTime (gsf_handle, GSF_RECORD_SWATH_BATHYMETRY_PING, -1, &sec, &nsec) + 1;
      misc->end_time = norm_posix_time (sec, nsec);

      misc->start_record = 1;
      misc->end_record = end_record = misc->total_records;

      curr_record = 1;
    }


  strcpy (misc->abe_share->nearest_filename, misc->filename);


  return (0);
}




void io_data_close ()
{
  switch (type)
    {
    case PFM_CHARTS_HOF_DATA:
    case PFM_SHOALS_TOF_DATA:
      fclose (fp);
      break;

    case PFM_CZMIL_DATA:
      czmil_close_cpf_file (cpf_handle);
      break;

    case PFM_LAS_DATA:
      fclose (las_fp);
      break;

    case PFM_GSF_DATA:

      //  Flush the last write buffer.

      if (last_rec != -1 && !no_edit)
        {
          gsf_data_id.record_number = last_rec;
          gsfWrite (gsf_handle, &gsf_data_id, &gsf_records);
          last_rec = -1;
        }
      gsfClose (gsf_handle);
      break;
    }
}


    

void io_data_read (int32_t num_records, POINT_DATA *data, QProgressBar *prog)
{
  int32_t i, j, k;
  double lat, lon, ang1, ang2, nlat, nlon, lateral, lat2, lon2;
  uint8_t nominal = NVFalse;


  if (num_records == 0)
    {
      printf("\n\n***YOU MUST OPEN A FILE BEFORE SCROLLING FORWARD***\n\n");
      exit(-1);
    }
        


  int32_t end = qMin (curr_record + num_records, end_record) - curr_record;
  
  k = 0;
  prog->setRange (0, end);
  int32_t inc = end / 10;

  for (j = 0 ; j < end ; j++)
    {
      if (!(j % inc)) prog->setValue (j);

      switch (type)
        {
        case PFM_SHOALS_TOF_DATA:
          tof_read_record (fp, curr_record, &tof_record);


          //  First return.

          if (tof_record.elevation_first != -998.0)
            {
              resize_record (data, k);

              data->y[k] = tof_record.latitude_first;
              data->x[k] = tof_record.longitude_first;
              data->z[k] = -tof_record.elevation_first;

              charts_cvtime (tof_record.timestamp, &year, &day, &hour, &minute, &second);

              data->time[k] = normtime (year, day, hour, minute, second);
              data->val[k] = 0;
              data->rec[k] = curr_record;
              data->sub[k] = 0;
              data->mask[k] = 0;
              data->fmask[k] = NVFalse;

              if (tof_record.conf_first < 50) data->val[k] = PFM_FILTER_INVAL;
              k++;
            }


            if (tof_record.elevation_last != -998.0) 
              {
                resize_record (data, k);

                data->y[k] = tof_record.latitude_last;
                data->x[k] = tof_record.longitude_last;
                data->z[k] = -tof_record.elevation_last;

                data->time[k] = normtime (year, day, hour, minute, second);
                data->val[k] = 0;
                data->rec[k] = curr_record;
                data->sub[k] = 1;
                data->mask[k] = 0;
                data->fmask[k] = NVFalse;

                if (tof_record.conf_last < 50) data->val[k] = PFM_FILTER_INVAL;
                k++;
              }
            curr_record++;
            break;


          case PFM_CHARTS_HOF_DATA:
            hof_read_record (fp, curr_record, &hof_record);

            if (hof_record.abdc)
              {
                resize_record (data, k);

                data->y[k] = hof_record.latitude;
                data->x[k] = hof_record.longitude;
                data->z[k] = -hof_record.correct_depth;

                charts_cvtime (hof_record.timestamp, &year, &day, &hour, &minute, &second);

                data->time[k] = normtime (year, day, hour, minute, second);
                data->val[k] = 0;
                data->rec[k] = curr_record;
                data->sub[k] = 0;
                data->mask[k] = 0;
                data->fmask[k] = NVFalse;

                if (hof_record.status & AU_STATUS_DELETED_BIT) data->val[k] = PFM_MANUALLY_INVAL;
                if (hof_record.abdc < 70) data->val[k] = PFM_FILTER_INVAL;


                if (hof_record.suspect_status & SUSPECT_STATUS_SUSPECT_BIT) data->val[k] |= PFM_SUSPECT;
                if (hof_record.suspect_status & SUSPECT_STATUS_FEATURE_BIT) data->val[k] |= PFM_SELECTED_FEATURE;


                if (hof_record.data_type) data->val[k] |= PFM_USER_01;
                if (hof_record.status & AU_STATUS_SWAPPED_BIT) data->val[k] |= PFM_USER_02;
                if (hof_record.abdc == 70) data->val[k] |= PFM_USER_03;
                k++;
              }


            if (hof_record.correct_sec_depth != -998.0)
              {
                resize_record (data, k);

                data->y[k] = hof_record.sec_latitude;
                data->x[k] = hof_record.sec_longitude;
                data->z[k] = -hof_record.correct_sec_depth;

                data->time[k] = normtime (year, day, hour, minute, second);
                data->val[k] = 0;
                data->rec[k] = curr_record;
                data->sub[k] = 1;
                data->mask[k] = 0;
                data->fmask[k] = NVFalse;

                if (hof_record.sec_abdc < 70) data->val[k] = PFM_FILTER_INVAL;

                k++;
              }


            curr_record++;
            break;


          case PFM_CZMIL_DATA:
            if (czmil_read_cpf_record (cpf_handle, curr_record, &cpf_record) < 0)
              {
                QString msg = QString ("%1 %2 %3 - Error reading CPF record - %4").arg (__FILE__).arg (__FUNCTION__).arg (__LINE__).arg (czmil_strerror ());
                misc->messages->append (msg);
              }
            else
              {
                for (int32_t chan = 0 ; chan < 9 ; chan++)
                  {
                    if (chan != CZMIL_IR_CHANNEL)
                      {
                        for (int32_t ret = 0 ; ret < cpf_record.returns[chan] ; ret++)
                          {
                            if (cpf_record.channel[chan][ret].elevation != cpf_header.null_z_value)
                              {
                                resize_record (data, k);

                                data->y[k] = cpf_record.channel[chan][ret].latitude;
                                data->x[k] = cpf_record.channel[chan][ret].longitude;
                                data->z[k] = -cpf_record.channel[chan][ret].elevation;

                                czmil_cvtime (cpf_record.timestamp, &year, &day, &hour, &minute, &second);

                                data->time[k] = normtime (year, day, hour, minute, second);
                                data->val[k] = 0;
                                data->rec[k] = curr_record;
                                data->sub[k] = chan * 100 + ret;
                                data->mask[k] = 0;
                                data->fmask[k] = NVFalse;

                                if (cpf_record.channel[chan][ret].status)
                                  {
                                    if (cpf_record.channel[chan][ret].status & CZMIL_RETURN_MANUALLY_INVAL) data->val[k] = PFM_MANUALLY_INVAL;
                                    if (cpf_record.channel[chan][ret].status & CZMIL_RETURN_FILTER_INVAL) data->val[k] = PFM_FILTER_INVAL;
                                    if (cpf_record.channel[chan][ret].status & CZMIL_RETURN_REFERENCE) data->val[k] |= PFM_REFERENCE;
                                    if (cpf_record.channel[chan][ret].status & CZMIL_RETURN_SUSPECT) data->val[k] |= PFM_SUSPECT;
                                    if (cpf_record.channel[chan][ret].status & CZMIL_RETURN_CLASSIFICATION_MODIFIED) data->val[k] |= PFM_SELECTED_SOUNDING;
                                    if (cpf_record.channel[chan][ret].status & CZMIL_RETURN_SELECTED_FEATURE) data->val[k] |= PFM_SELECTED_FEATURE;
                                    if (cpf_record.channel[chan][ret].status & CZMIL_RETURN_DESIGNATED_SOUNDING) data->val[k] |= PFM_DESIGNATED_SOUNDING;
                                  }
                                k++;
                              }
                          }
                      }
                  }

                curr_record++;
              }
            break;


          case PFM_LAS_DATA:
            if (slas_read_point_data (las_fp, curr_record, &lasheader, endian, &slas))
              {
                QString msg = QString ("%1 %2 %3 - Error reading LAS record - %4").arg (__FILE__).arg (__FUNCTION__).arg (__LINE__).arg (strerror (errno));
                misc->messages->append (msg);
              }
            else
              {
                resize_record (data, k);


                if (misc->projected)
                  {
                    lon = slas.x;
                    lat = slas.y;
                    pj_transform (misc->pj_utm, misc->pj_latlon, 1, 1, &lon, &lat, NULL);
                    lon *= NV_RAD_TO_DEG;
                    lat *= NV_RAD_TO_DEG;
                  }
                else
                  {
                    lon = slas.x;
                    lat = slas.y;
                  }

                data->y[k] = lat;
                data->x[k] = lon;
                data->z[k] = -slas.z;


                data->time[k] = 0.0;
                data->val[k] = 0;
                data->rec[k] = curr_record;
                data->sub[k] = 0;
                data->mask[k] = 0;
                data->fmask[k] = NVFalse;

                if (slas.synthetic) data->val[k] |= PFM_REFERENCE;
                if (slas.keypoint) data->val[k] |= PFM_SELECTED_FEATURE;
                if (slas.withheld) data->val[k] |= PFM_MANUALLY_INVAL;

                k++;

                curr_record++;
              }
            break;


          case PFM_GSF_DATA:

            //  Flush the last write buffer.
    
            if (last_rec != -1)
              {
                gsf_data_id.record_number = last_rec;
                gsfWrite (gsf_handle, &gsf_data_id, &gsf_records);
                last_rec = -1;
              }
        

            //  Read a GSF record.
        
            gsf_data_id.record_number = curr_record;
            if ((gsfRead (gsf_handle, GSF_RECORD_SWATH_BATHYMETRY_PING, &gsf_data_id, &gsf_records, NULL, 0)) != -1)
              {
                if (!(gsf_records.mb_ping.ping_flags & GSF_IGNORE_PING))
                  {
                    //  If all true depth values are zero or there is no true depth field use nominal depth.

                    if (gsf_records.mb_ping.depth != NULL)
                      {
                        uint8_t hit = NVFalse;
                        for (i = 0 ; i < gsf_records.mb_ping.number_beams ; i++)
                          {
                            if (gsf_records.mb_ping.depth[i] != 0.0)
                              {
                                hit = NVTrue;
                                break;
                              }
                          }

                        if (hit && gsf_records.mb_ping.nominal_depth != NULL)
                          {
                            nominal = NVFalse;
                          }
                        else
                          {
                            nominal = NVTrue;
                          }
                      }
                    else
                      {
                        nominal = NVTrue;
                      }


                    //  Snag the lat, lon, and heading.
            
                    lat  = gsf_records.mb_ping.latitude;
                    lon  = gsf_records.mb_ping.longitude;
                    ang1 = gsf_records.mb_ping.heading + 90.0;
                    ang2 = gsf_records.mb_ping.heading;


                    //  Check for the ever popular bad nav data.

                    if (lat <= 90.0 && lon <= 180.0)
                      {
                        //  Georeference each beam.
            
                        for (i = 0 ; i < gsf_records.mb_ping.number_beams ; i++)
                          {
                            if (!check_flag (gsf_records.mb_ping.beam_flags[i], NV_GSF_IGNORE_NULL_BEAM))
                              {
                                //  If the across track array is present then use it.

                                nlat = lat;
                                nlon = lon;
                                if (gsf_records.mb_ping.across_track != (double *) NULL)
                                  {
                                    lateral = gsf_records.mb_ping.across_track[i];
                                    newgp (lat, lon, ang1, lateral, &nlat, &nlon);
                                  }


                                //  If the along track array is present then use it.

                                if (gsf_records.mb_ping.along_track != (double *) NULL)
                                  {
                                    lat2=nlat;
                                    lon2=nlon;
                                    lateral = gsf_records.mb_ping.along_track[i];
                                    newgp (lat2, lon2, ang2, lateral, &nlat, &nlon);
                                  }


                                resize_record (data, k);


                                //  Load the arrays.

                                data->y[k] = nlat;
                                data->x[k] = nlon;
                                if (nominal && gsf_records.mb_ping.nominal_depth != NULL)
                                  {
                                    data->z[k] = gsf_records.mb_ping.nominal_depth[i];
                                  }
                                else
                                  {
                                    data->z[k] = gsf_records.mb_ping.depth[i];
                                  }
                                data->time[k] = norm_posix_time (gsf_records.mb_ping.ping_time.tv_sec, gsf_records.mb_ping.ping_time.tv_nsec);
                                data->val[k] = 0;
                                data->rec[k] = curr_record;
                                data->sub[k] = i;
                                data->mask[k] = 0;
                                data->fmask[k] = NVFalse;

                                gsf_to_pfm_flags (&data->val[k], gsf_records.mb_ping.beam_flags[i]);

                                k++;
                              }
                          }
                      }
                  }
              }
            curr_record++;

            break;
        }
    }


  prog->setValue (end);
  qApp->processEvents ();

  data->count = k;
}




void io_data_write (uint32_t val, int32_t rec, int32_t sub, double time)
{
    int64_t       timestamp;
    int32_t       chan, ret;

    if (no_edit) return;


    switch (type)
      {
      case PFM_SHOALS_TOF_DATA:

        //  Read a record.  You don't have enough information to rebuild the record so you have 
        //  to read it.

        tof_read_record (fp, rec, &tof_record);
        timestamp = tof_record.timestamp;

        charts_cvtime (timestamp, &year, &day, &hour, &minute, &second);


        //  Test the time passed back from the program against the one just read in
        //  for the same rec.  If they don't match, you've got problems.
        
        if (fabs (normtime (year, day, hour, minute, second) - time) > 0.000001)
          {
            printf ("Time mismatch in io_data_write %f %f %d\n",
                    normtime (year, day, hour, minute, second), time, rec);
            exit (-1);
          }


        if (sub)
          {
            if (val & PFM_INVAL)
              {
                if (tof_record.conf_last > 0) tof_record.conf_last = -tof_record.conf_last;
              }
            else
              {
                if (tof_record.conf_last < 0) tof_record.conf_last = -tof_record.conf_last;
              }
          }
        else
          {
            if (val & PFM_INVAL)
              {
                if (tof_record.conf_first > 0) tof_record.conf_first = -tof_record.conf_first;
              }
            else
              {
                if (tof_record.conf_first < 0) tof_record.conf_first = -tof_record.conf_first;
              }
          }


        tof_write_record (fp, rec, &tof_record);

        break;


      case PFM_CHARTS_HOF_DATA:

        //  Read a record.  You don't have enough information to rebuild the record so you have 
        //  to read it.

        hof_read_record (fp, rec, &hof_record);
        timestamp = hof_record.timestamp;


        charts_cvtime (timestamp, &year, &day, &hour, &minute, &second);


        //  Test the time passed back from the program against the one just read in
        //  for the same rec.  If they don't match, you've got problems.
        
        if (fabs (normtime (year, day, hour, minute, second) - time) > 0.000001)
          {
            printf ("Time mismatch in io_data_write %f %f %d\n",
                    normtime (year, day, hour, minute, second), time, rec);
            exit (-1);
          }

        /*  HOF uses the lower three bits of the status field for status thusly :

        bit 0 = deleted    (1) 
        bit 1 = kept       (2) 
        bit 2 = swapped    (4)      
        */

        hof_record.status = 0;
        if (val & PFM_INVAL) hof_record.status |= AU_STATUS_DELETED_BIT;


        hof_record.suspect_status = 0;
        if (val & PFM_SUSPECT) hof_record.suspect_status = SUSPECT_STATUS_SUSPECT_BIT;
        if (val & PFM_SELECTED_FEATURE) hof_record.suspect_status |= SUSPECT_STATUS_FEATURE_BIT;


        //  Another thing with the hof data - if the invalid bit is set we also set the abdc to a 
        //  negative (or vice-versa).

        if (val & PFM_INVAL)
          {
            if (hof_record.abdc > 0) hof_record.abdc = -hof_record.abdc;
          }
        else
          {
            if (hof_record.abdc < 0) hof_record.abdc = -hof_record.abdc;
          }

        hof_write_record (fp, rec, &hof_record);

        break;


      case PFM_CZMIL_DATA:

        //  Read a record.  You don't have enough information to rebuild the record so you have to read it.

        czmil_read_cpf_record (cpf_handle, rec, &cpf_record);

        czmil_cvtime (cpf_record.timestamp, &year, &day, &hour, &minute, &second);


        //  Test the time passed back from the program against the one just read in
        //  for the same rec.  If they don't match, you've got problems.
        
        if (fabs (normtime (year, day, hour, minute, second) - time) > 0.000001)
          {
            printf ("Time mismatch in io_data_write %f %f %d\n", normtime (year, day, hour, minute, second), time, rec);
            exit (-1);
          }


        chan = sub / 100;
        ret = sub % 100;


        if (val & PFM_MANUALLY_INVAL) cpf_record.channel[chan][ret].status = CZMIL_RETURN_MANUALLY_INVAL;
        if (val & PFM_FILTER_INVAL) cpf_record.channel[chan][ret].status = CZMIL_RETURN_FILTER_INVAL;
        if (val & PFM_SUSPECT) cpf_record.channel[chan][ret].status |= CZMIL_RETURN_SUSPECT;
        if (val & PFM_REFERENCE) cpf_record.channel[chan][ret].status |= CZMIL_RETURN_REFERENCE;
        if (val & PFM_SELECTED_SOUNDING) cpf_record.channel[chan][ret].status |= CZMIL_RETURN_CLASSIFICATION_MODIFIED;
        if (val & PFM_SELECTED_FEATURE) cpf_record.channel[chan][ret].status |= CZMIL_RETURN_SELECTED_FEATURE;
        if (val & PFM_DESIGNATED_SOUNDING) cpf_record.channel[chan][ret].status |= CZMIL_RETURN_DESIGNATED_SOUNDING;
        if (val & CZMIL_RETURN_REPROCESSED) cpf_record.channel[chan][ret].status |= CZMIL_RETURN_REPROCESSED;

        czmil_update_cpf_record (cpf_handle, rec, &cpf_record);

        break;


      case PFM_LAS_DATA:

        if (slas_read_point_data (las_fp, rec, &lasheader, endian, &slas))
          {
            QString msg = QString ("%1 %2 %3 - Error reading LAS record - %4").arg (__FILE__).arg (__FUNCTION__).arg (__LINE__).arg (strerror (errno));
            misc->messages->append (msg);
          }
        else
          {
            //  Set the "synthetic" flag.

            if (val & PFM_REFERENCE)
              {
                slas.synthetic = 1;
              }
            else
              {
                slas.synthetic = 0;
              }


            //  Set the "keypoint" flag.

            if (val & PFM_SELECTED_FEATURE)
              {
                slas.keypoint = 1;
              }
            else
              {
                slas.keypoint = 0;
              }


            //  Set the "withheld" flag.

            if (val & PFM_MANUALLY_INVAL)
              {
                slas.withheld = 1;
              }
            else
              {
                slas.withheld = 0;
              }


            if (slas_update_point_data (las_fp, rec, &lasheader, endian, &slas))
              {
                fprintf (stderr, "\nError %s updating record %d in file %s : %s %s %d\n\n", strerror (errno), rec, misc->filename, __FILE__, __FUNCTION__,
                         __LINE__);
                fflush (stderr);
                fclose (las_fp);
                exit (-1);
              }
          }

        break;


      case PFM_GSF_DATA:

        //  If the rec number has changed, flush the last write buffer.
    
        if (rec != last_rec)
          {
            //  Don't write the first time through.
        
            if (last_rec != -1)
              {
                gsf_data_id.record_number = last_rec;
                gsfWrite (gsf_handle, &gsf_data_id, &gsf_records);
              }


            //  Read a record.  You don't have enough information to rebuild the record so you 
            //  have to read it.

            gsf_data_id.record_number = rec;
            if ((gsfRead (gsf_handle, GSF_RECORD_SWATH_BATHYMETRY_PING, &gsf_data_id, &gsf_records, NULL, 0)) == -1)
              {
                fprintf (stderr, "%s %s %s %d - Invalid rec number - %d\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, rec);
                exit (-1);
              }

            //  Save the last rec number.
        
            last_rec = rec;
          }


        //  Test the time passed back from the program against the one just read in
        //  for the same rec.  If they don't match, you've got problems.
        
        if (fabs (norm_posix_time (gsf_records.mb_ping.ping_time.tv_sec, 
                                   gsf_records.mb_ping.ping_time.tv_nsec) - time) > 0.000001)
          {
            printf ("Time mismatch in io_data_write %f %f %d\n",
                    norm_posix_time (gsf_records.mb_ping.ping_time.tv_sec,
                                     gsf_records.mb_ping.ping_time.tv_nsec), time, rec);
            exit (-1);
          }


        //  Convert the PFM validity data to GSF validity data (we're buffering by ping so we'll write it out later).

        //  Don't do anything if the entire ping was bad.
    
        if (!(gsf_records.mb_ping.ping_flags & GSF_IGNORE_PING))
          {
            //  Set the beam flags.  Don't write the record yet, we'll do that if/when the record changes

            pfm_to_gsf_flags (val, &gsf_records.mb_ping.beam_flags[sub]);
        }
        break;
    }
}



int32_t get_record ()
{
    /*  REQUIRED    */
    /*  Return the current record number in the swath file. */
    
    return (curr_record);
}



void set_record (int32_t record)
{
    /*  REQUIRED    */
    /*  Set the current record number or position to the specified record.  */
    
    curr_record = record;
}



double rec_time (int32_t record)
{
  time_t sec;
  long nsec;
  HYDRO_OUTPUT_T hof;
  TOPO_OUTPUT_T tof;
  double time = 0.0;


  switch (type)
    {
    case PFM_SHOALS_TOF_DATA:
      tof_read_record (fp, record, &tof);

      charts_cvtime (tof.timestamp, &year, &day, &hour, &minute, &second);

      time = normtime (year, day, hour, minute, second);
      break;

    case PFM_CHARTS_HOF_DATA:
      hof_read_record (fp, record, &hof);

      charts_cvtime (hof.timestamp, &year, &day, &hour, &minute, &second);

      time = normtime (year, day, hour, minute, second);
      break;

    case PFM_GSF_DATA:
      gsfIndexTime (gsf_handle, GSF_RECORD_SWATH_BATHYMETRY_PING, record, &sec, &nsec);
      time = norm_posix_time (sec, nsec);
      break;
    }

  return (time);
}
