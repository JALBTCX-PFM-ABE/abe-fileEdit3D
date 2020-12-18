
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



#include "prefs.hpp"
#include "prefsHelp.hpp"
#include "io_data.hpp"

prefs::prefs (QWidget *pa, OPTIONS *op, MISC *mi, POINT_DATA *da):
  QDialog (pa, (Qt::WindowFlags) Qt::WA_DeleteOnClose)
{
  parent = pa;
  options = op;
  misc = mi;
  data = da;

  mod_share = *misc->abe_share;
  mod_options = *op;

  setModal (true);

  ancillaryProgramD = hotKeyD = NULL;


  dataChanged = NVFalse;


  setWindowTitle (tr ("fileEdit3D Preferences"));

  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->setMargin (5);
  vbox->setSpacing (5);

  QRadioButton *hdms = new QRadioButton (tr ("Hemisphere Degrees Minutes Seconds.decimal"));
  QRadioButton *hdm_ = new QRadioButton (tr ("Hemisphere Degrees Minutes.decimal"));
  QRadioButton *hd__ = new QRadioButton (tr ("Hemisphere Degrees.decimal"));
  QRadioButton *sdms = new QRadioButton (tr ("+/-Degrees Minutes Seconds.decimal"));
  QRadioButton *sdm_ = new QRadioButton (tr ("+/-Degrees Minutes.decimal"));
  QRadioButton *sd__ = new QRadioButton (tr ("+/-Degrees.decimal"));

  QGroupBox *formBox = new QGroupBox (this);
  QVBoxLayout *formBoxLayout = new QVBoxLayout;
  formBox->setLayout (formBoxLayout);

  bGrp = new QButtonGroup (this);
  bGrp->setExclusive (true);
  connect (bGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotPositionClicked (int)));

  bGrp->addButton (hdms, 0);
  bGrp->addButton (hdm_, 1);
  bGrp->addButton (hd__, 2);
  bGrp->addButton (sdms, 3);
  bGrp->addButton (sdm_, 4);
  bGrp->addButton (sd__, 5);

  formBoxLayout->addWidget (hdms);
  formBoxLayout->addWidget (hdm_);
  formBoxLayout->addWidget (hd__);
  formBoxLayout->addWidget (sdms);
  formBoxLayout->addWidget (sdm_);
  formBoxLayout->addWidget (sd__);

  formBoxLayout->addStretch (1);


  prefTab = new QTabWidget ();
  prefTab->setTabPosition (QTabWidget::North);

  prefTab->addTab (formBox, tr ("Position Format"));
  prefTab->setTabToolTip (0, tr ("Set the position display format"));
  prefTab->setTabWhatsThis (0, bGrpText);


  QGroupBox *cvBox = new QGroupBox (this);
  QVBoxLayout *cvBoxLayout = new QVBoxLayout;
  cvBox->setLayout (cvBoxLayout);


  QGroupBox *cbox = new QGroupBox (this);
  QHBoxLayout *cboxLayout = new QHBoxLayout;
  cbox->setLayout (cboxLayout);
  QVBoxLayout *cboxLeftLayout = new QVBoxLayout;
  QVBoxLayout *cboxRightLayout = new QVBoxLayout;
  cboxLayout->addLayout (cboxLeftLayout);
  cboxLayout->addLayout (cboxRightLayout);



  bTrackerColor = new QPushButton (tr ("Tracker"), this);
  bTrackerColor->setToolTip (tr ("Change tracker color"));
  bTrackerColor->setWhatsThis (trackerColorText);
  bTrackerPalette = bTrackerColor->palette ();
  connect (bTrackerColor, SIGNAL (clicked ()), this, SLOT (slotTrackerColor ()));
  cboxLeftLayout->addWidget (bTrackerColor);


  bBackgroundColor = new QPushButton (tr ("Background"), this);
  bBackgroundColor->setToolTip (tr ("Change display background color"));
  bBackgroundColor->setWhatsThis (backgroundColorText);
  bBackgroundPalette = bBackgroundColor->palette ();
  connect (bBackgroundColor, SIGNAL (clicked ()), this, SLOT (slotBackgroundColor ()));
  cboxLeftLayout->addWidget (bBackgroundColor);


  bScaleColor = new QPushButton (tr ("Scale"), this);
  bScaleColor->setToolTip (tr ("Change scale color"));
  bScaleColor->setWhatsThis (scaleColorText);
  bScalePalette = bScaleColor->palette ();
  connect (bScaleColor, SIGNAL (clicked ()), this, SLOT (slotScaleColor ()));
  cboxLeftLayout->addWidget (bScaleColor);


  cGrp = new QButtonGroup (this);
  connect (cGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotColorClicked (int)));


  for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++)
    {
      QString string = tr ("Cursor %1").arg (i, 2, 10, QChar ('0'));
      bWaveColor[i] = new QPushButton (string, this);
      string = tr ("Change color for cursor %1").arg (i, 2, 10, QChar ('0'));
      bWaveColor[i]->setToolTip (string);
      bWaveColor[i]->setWhatsThis (waveColorText);
      bWavePalette[i] = bWaveColor[i]->palette ();

      cGrp->addButton (bWaveColor[i], i);

      if (i > 2)
        {
          cboxRightLayout->addWidget (bWaveColor[i]);
        }
      else
        {
          cboxLeftLayout->addWidget (bWaveColor[i]);
        }
    }

  cvBoxLayout->addWidget (cbox);


  //  Check box for gray data outside of slice.

  graySliceCheck = new QCheckBox (tr ("Data outside slice set to gray"), cbox);
  graySliceCheck->setToolTip (tr ("Set all data outside of the slice area to gray, semi-transparent"));
  graySliceCheck->setWhatsThis (graySliceCheckText);
  cvBoxLayout->addWidget (graySliceCheck);


  prefTab->addTab (cvBox, tr ("Colors"));
  prefTab->setTabToolTip (1, tr ("Set the display colors"));
  prefTab->setTabWhatsThis (1, colorText);


  QGroupBox *conBox = new QGroupBox (this);
  QHBoxLayout *conBoxLayout = new QHBoxLayout;
  conBox->setLayout (conBoxLayout);
  QVBoxLayout *conBoxLeftLayout = new QVBoxLayout;
  QVBoxLayout *conBoxRightLayout = new QVBoxLayout;
  conBoxLayout->addLayout (conBoxLeftLayout);
  conBoxLayout->addLayout (conBoxRightLayout);


  QGroupBox *zBox = new QGroupBox (tr ("Scale"), this);
  QHBoxLayout *zBoxLayout = new QHBoxLayout;
  zBox->setLayout (zBoxLayout);
  ZFactor = new QComboBox (zBox);
  ZFactor->setToolTip (tr ("Change the Z scale factor"));
  ZFactor->setWhatsThis (ZFactorText);
  ZFactor->setEditable (true);
  ZFactor->addItem (tr ("%L1 (no scaling)").arg (1.00000, 0, 'f', 4));
  ZFactor->addItem (tr ("%L1 (meters to feet)").arg (3.28084, 0, 'f', 4));
  ZFactor->addItem (tr ("%L1 (feet to meters)").arg (0.34080, 0, 'f', 4));
  ZFactor->addItem (tr ("%L1 (meters to fathoms)").arg (0.54681, 0, 'f', 4));
  ZFactor->addItem (tr ("%L1 (fathoms to meters)").arg (1.82880, 0, 'f', 4));
  zBoxLayout->addWidget (ZFactor);
  conBoxLeftLayout->addWidget (zBox);


  QGroupBox *offBox = new QGroupBox (tr ("Offset"), this);
  QHBoxLayout *offBoxLayout = new QHBoxLayout;
  offBox->setLayout (offBoxLayout);
  offset = new QDoubleSpinBox (offBox);
  offset->setDecimals (2);
  offset->setRange (-1000.0, 1000.0);
  offset->setSingleStep (10.0);
  offset->setToolTip (tr ("Change the contour/Z offset value"));
  offset->setWhatsThis (offsetText);
  offBoxLayout->addWidget (offset);

  QPushButton *egmButton = new QPushButton (tr ("EGM08"));
  egmButton->setToolTip (tr ("Get the ellipsoid to geoid datum offset from EGM08 model"));
  egmButton->setWhatsThis (egmText);
  connect (egmButton, SIGNAL (clicked ()), this, SLOT (slotEgmClicked (void)));
  offBoxLayout->addWidget (egmButton);
  conBoxLeftLayout->addWidget (offBox);


  QGroupBox *zoomBox = new QGroupBox (tr ("Zoom percentage"), this);
  QHBoxLayout *zoomBoxLayout = new QHBoxLayout;
  zoomBox->setLayout (zoomBoxLayout);
  zoomPercent = new QSpinBox (zoomBox);
  zoomPercent->setRange (5, 50);
  zoomPercent->setSingleStep (5);
  zoomPercent->setToolTip (tr ("Change the zoom in/out percentage (10 - 50)"));
  zoomPercent->setWhatsThis (zoomPercentText);
  zoomBoxLayout->addWidget (zoomPercent);
  conBoxRightLayout->addWidget (zoomBox);


  QGroupBox *exagBox = new QGroupBox (tr ("Z exaggeration"), this);
  QHBoxLayout *exagBoxLayout = new QHBoxLayout;
  exagBox->setLayout (exagBoxLayout);
  exag = new QDoubleSpinBox (offBox);
  exag->setDecimals (1);
  exag->setRange (1.0, 10.0);
  exag->setSingleStep (1.0);
  exag->setToolTip (tr ("Change the Z exaggeration value"));
  exag->setWhatsThis (exagText);
  exagBoxLayout->addWidget (exag);
  conBoxRightLayout->addWidget (exagBox);

  QGroupBox *pixBox = new QGroupBox (tr ("Point size"), this);
  QHBoxLayout *pixBoxLayout = new QHBoxLayout;
  pixBox->setLayout (pixBoxLayout);
  pointSize = new QSpinBox (pixBox);
  pointSize->setRange (2, 10);
  pointSize->setSingleStep (1);
  pointSize->setToolTip (tr ("Change the point size (pixels) (2 - 10)"));
  pointSize->setWhatsThis (pointSizeText);
  pixBoxLayout->addWidget (pointSize);
  conBoxRightLayout->addWidget (pixBox);


  prefTab->addTab (conBox, tr ("Contour/Depth"));
  prefTab->setTabToolTip (2, tr ("Set the contour/depth options"));
  prefTab->setTabWhatsThis (2, conText);


  QGroupBox *filterBox = new QGroupBox (this);
  QVBoxLayout *filterBoxLayout = new QVBoxLayout;
  filterBox->setLayout (filterBoxLayout);


  QGroupBox *ftBox = new QGroupBox (tr ("Filter type"), this);
  QHBoxLayout *ftBoxLayout = new QHBoxLayout;
  ftBox->setLayout (ftBoxLayout);
  filterType = new QComboBox (ftBox);
  filterType->setToolTip (tr ("Select the type of filter"));
  filterType->setWhatsThis (filterTypeText);
  filterType->setEditable (false);
  filterType->addItem (tr ("Area-based statistical"));
  filterType->addItem (tr ("Hockey Puck"));
  ftBoxLayout->addWidget (filterType);
  filterBoxLayout->addWidget (ftBox);

  QGroupBox *sBox = new QGroupBox (tr ("Area-based filter standard deviation"), this);
  QHBoxLayout *sBoxLayout = new QHBoxLayout;
  sBox->setLayout (sBoxLayout);
  filterSTD = new QDoubleSpinBox (sBox);
  filterSTD->setDecimals (1);
  filterSTD->setRange (0.3, 4.0);
  filterSTD->setSingleStep (0.1);
  filterSTD->setToolTip (tr ("Change the area-based statistical filter standard deviation"));
  filterSTD->setWhatsThis (filterSTDText);
  sBoxLayout->addWidget (filterSTD);
  filterBoxLayout->addWidget (sBox);

  QGroupBox *dBox = new QGroupBox (tr ("Area-based filter deep filter only"), this);
  QHBoxLayout *dBoxLayout = new QHBoxLayout;
  dBox->setLayout (dBoxLayout);
  dFilter = new QCheckBox (dBox);
  dFilter->setToolTip (tr ("Toggle filtering of deep values only (for area-based statistical filter)"));
  dFilter->setWhatsThis (dFilterText);
  dBoxLayout->addWidget (dFilter);
  filterBoxLayout->addWidget (dBox);


  QGroupBox *hptBox = new QGroupBox (tr ("Hockey Puck number of threads"), this);
  QHBoxLayout *hptBoxLayout = new QHBoxLayout;
  hptBox->setLayout (hptBoxLayout);
  hpThreads = new QComboBox (hptBox);
  hpThreads->setToolTip (tr ("Change the number of threads used by the Hockey Puck filter"));
  hpThreads->setWhatsThis (hpThreadsText);
  hpThreads->setEditable (false);
  hpThreads->addItem ("1");
  hpThreads->addItem ("4");
  hpThreads->addItem ("16");
  hptBoxLayout->addWidget (hpThreads);
  filterBoxLayout->addWidget (hptBox);

  QGroupBox *hprBox = new QGroupBox (tr ("Hockey Puck radius"), this);
  QHBoxLayout *hprBoxLayout = new QHBoxLayout;
  hprBox->setLayout (hprBoxLayout);
  hpRadius = new QDoubleSpinBox (hprBox);
  hpRadius->setDecimals (2);
  hpRadius->setRange (HP_MIN_RADIUS, HP_MAX_RADIUS);
  hpRadius->setSingleStep (1.0);
  hpRadius->setToolTip (tr ("Change the Hockey Puck filter radius"));
  hpRadius->setWhatsThis (hpRadiusText);
  hprBoxLayout->addWidget (hpRadius);
  filterBoxLayout->addWidget (hprBox);

  QGroupBox *hphBox = new QGroupBox (tr ("Hockey Puck height"), this);
  QHBoxLayout *hphBoxLayout = new QHBoxLayout;
  hphBox->setLayout (hphBoxLayout);
  hpHeight = new QDoubleSpinBox (hphBox);
  hpHeight->setDecimals (2);
  hpHeight->setRange (HP_MIN_HEIGHT, HP_MAX_HEIGHT);
  hpHeight->setSingleStep (1.0);
  hpHeight->setToolTip (tr ("Change the Hockey Puck filter height"));
  hpHeight->setWhatsThis (hpHeightText);
  hphBoxLayout->addWidget (hpHeight);
  filterBoxLayout->addWidget (hphBox);

  QGroupBox *hpnBox = new QGroupBox (tr ("Hockey Puck neighbors"), this);
  QHBoxLayout *hpnBoxLayout = new QHBoxLayout;
  hpnBox->setLayout (hpnBoxLayout);
  hpNeighbors = new QSpinBox (hpnBox);
  hpNeighbors->setRange (HP_MIN_NEIGHBORS, HP_MAX_NEIGHBORS);
  hpNeighbors->setSingleStep (1);
  hpNeighbors->setToolTip (tr ("Change the Hockey Puck filter required number of neighbors"));
  hpNeighbors->setWhatsThis (hpNeighborsText);
  hpnBoxLayout->addWidget (hpNeighbors);
  filterBoxLayout->addWidget (hpnBox);


  prefTab->addTab (filterBox, tr ("Filter"));
  prefTab->setTabToolTip (3, tr ("Set the filter options"));
  prefTab->setTabWhatsThis (3, filtText);


  QGroupBox *miscBox = new QGroupBox (this);
  QHBoxLayout *miscBoxLayout = new QHBoxLayout;
  miscBox->setLayout (miscBoxLayout);
  QVBoxLayout *miscBoxLeftLayout = new QVBoxLayout;
  QVBoxLayout *miscBoxRightLayout = new QVBoxLayout;
  miscBoxLayout->addLayout (miscBoxLeftLayout);
  miscBoxLayout->addLayout (miscBoxRightLayout);


  QGroupBox *minBox = new QGroupBox (tr ("Minimum Z window size"), this);
  QHBoxLayout *minBoxLayout = new QHBoxLayout;
  minBox->setLayout (minBoxLayout);

  ihoOrder = new QComboBox (minBox);
  ihoOrder->setToolTip (tr ("Use IHO order to define minimum Z window size"));
  ihoOrder->setWhatsThis (ihoOrderText);
  ihoOrder->setEditable (true);
  ihoOrder->addItem ("Set Manually ->");
  ihoOrder->addItem ("IHO Special Order");
  ihoOrder->addItem ("IHO Order 1a/1b");
  ihoOrder->addItem ("IHO Order 2");
  minBoxLayout->addWidget (ihoOrder);


  minZ = new QDoubleSpinBox (minBox);
  minZ->setDecimals (1);
  minZ->setRange (0.1, 100.0);
  minZ->setSingleStep (1.0);
  minZ->setToolTip (tr ("Manually set the minimum Z window size"));
  minZ->setWhatsThis (minZText);
  minBoxLayout->addWidget (minZ);

  miscBoxLeftLayout->addWidget (minBox);


  QGroupBox *pointBox = new QGroupBox (tr ("Displayed point limit"), this);
  QHBoxLayout *pointBoxLayout = new QHBoxLayout;
  pointBox->setLayout (pointBoxLayout);

  pointLimit = new QSpinBox (this);
  pointLimit->setRange (10000, 1000000);
  pointLimit->setSingleStep (10000);
  pointLimit->setToolTip (tr ("Approximate number of points that you wish to have displayed"));
  pointLimit->setWhatsThis (pointLimitText);
  pointBoxLayout->addWidget (pointLimit);

  miscBoxLeftLayout->addWidget (pointBox);


  QGroupBox *rotBox = new QGroupBox (tr ("Rotation"), this);
  QHBoxLayout *rotBoxLayout = new QHBoxLayout;
  rotBox->setLayout (rotBoxLayout);
  rot = new QDoubleSpinBox (rotBox);
  rot->setDecimals (1);
  rot->setRange (1.0, 30.0);
  rot->setSingleStep (1.0);
  rot->setToolTip (tr ("Change the view rotation increment (1.0 - 30.0)"));
  rot->setWhatsThis (rotText);
  rotBoxLayout->addWidget (rot);
  miscBoxLeftLayout->addWidget (rotBox);


  QGroupBox *scaleBox = new QGroupBox (tr ("Draw scale"), this);
  QHBoxLayout *scaleBoxLayout = new QHBoxLayout;
  scaleBox->setLayout (scaleBoxLayout);
  scale = new QCheckBox (this);
  scale->setToolTip (tr ("If checked, the scale will be drawn on the screen"));
  scale->setWhatsThis (scaleText);
  scaleBoxLayout->addWidget (scale);
  miscBoxRightLayout->addWidget (scaleBox);


  QGroupBox *undoBox = new QGroupBox (tr ("Undo levels"), this);
  QHBoxLayout *undoBoxLayout = new QHBoxLayout;
  undoBox->setLayout (undoBoxLayout);
  undo = new QSpinBox (undoBox);
  undo->setRange (100, 1000000);
  undo->setSingleStep (100);
  undo->setToolTip (tr ("Change the maximum number of undo levels (100 - 1,000,000)"));
  undo->setWhatsThis (prefsUndoText);
  undoBoxLayout->addWidget (undo);
  miscBoxRightLayout->addWidget (undoBox);


  QGroupBox *tipBox = new QGroupBox (tr ("Highlighted point tooltip"), this);
  QHBoxLayout *tipBoxLayout = new QHBoxLayout;
  tipBox->setLayout (tipBoxLayout);
  tip = new QCheckBox (this);
  tip->setToolTip (tr ("If checked, tool tip will be displayed when highlighted points are present"));
  tip->setWhatsThis (tipText);
  tipBoxLayout->addWidget (tip);
  miscBoxRightLayout->addWidget (tipBox);


  prefTab->addTab (miscBox, tr ("Miscellaneous"));
  prefTab->setTabToolTip (4, tr ("Set miscellaneous options"));
  prefTab->setTabWhatsThis (4, miscText);


  QGroupBox *kbox = new QGroupBox (this);
  QVBoxLayout *kboxLayout = new QVBoxLayout;
  kbox->setLayout (kboxLayout);


  bHotKeys = new QPushButton (tr ("Main Buttons"), this);
  bHotKeys->setToolTip (tr ("Change hot key sequences for buttons"));
  bHotKeys->setWhatsThis (hotKeysText);
  connect (bHotKeys, SIGNAL (clicked ()), this, SLOT (slotHotKeys ()));
  kboxLayout->addWidget (bHotKeys);


  QGroupBox *iconBox = new QGroupBox (tr ("Main Button Icon Size"), this);
  QHBoxLayout *iconBoxLayout = new QHBoxLayout;
  iconBox->setLayout (iconBoxLayout);

  iconSize = new QComboBox (iconBox);
  iconSize->setToolTip (tr ("Set the size (in pixels) of the main button icons"));
  iconSize->setWhatsThis (iconSizeText);
  iconSize->setEditable (false);
  iconSize->addItem ("16");
  iconSize->addItem ("20");
  iconSize->addItem ("24");
  iconSize->addItem ("28");
  iconSize->addItem ("32");
  iconBoxLayout->addWidget (iconSize);


  kboxLayout->addWidget (iconBox);


  bAncillaryPrograms = new QPushButton (tr ("Ancillary Programs"), this);
  bAncillaryPrograms->setToolTip (tr ("Change ancillary program hot keys and action keys"));
  bAncillaryPrograms->setWhatsThis (ancillaryProgramsText);
  connect (bAncillaryPrograms, SIGNAL (clicked ()), this, SLOT (slotAncillaryPrograms ()));
  kboxLayout->addWidget (bAncillaryPrograms);


  prefTab->addTab (kbox, tr ("Hot Keys"));
  prefTab->setTabToolTip (5, tr ("Set hot keys"));
  prefTab->setTabWhatsThis (5, kboxText);


  vbox->addWidget (prefTab);


  setFields ();


  //  Connect this here so that it doesn't cause the IHO combo to go to Manual every time.

  connect (minZ, SIGNAL (valueChanged (double)), this, SLOT (slotMinZValueChanged (double)));


  QHBoxLayout *actions = new QHBoxLayout (0);
  vbox->addLayout (actions);

  QPushButton *bHelp = new QPushButton (this);
  bHelp->setIcon (QIcon (":/icons/contextHelp.png"));
  bHelp->setToolTip (tr ("Enter What's This mode for help"));
  connect (bHelp, SIGNAL (clicked ()), this, SLOT (slotHelp ()));
  actions->addWidget (bHelp);

  actions->addStretch (10);

  bRestoreDefaults = new QPushButton (tr ("Restore Defaults"), this);
  bRestoreDefaults->setToolTip (tr ("Restore all preferences to the default state"));
  bRestoreDefaults->setWhatsThis (restoreDefaultsText);
  connect (bRestoreDefaults, SIGNAL (clicked ()), this, SLOT (slotRestoreDefaults ()));
  actions->addWidget (bRestoreDefaults);

  QPushButton *applyButton = new QPushButton (tr ("OK"), this);
  applyButton->setToolTip (tr ("Accept changes and close dialog"));
  applyButton->setWhatsThis (applyPrefsText);
  connect (applyButton, SIGNAL (clicked ()), this, SLOT (slotApplyPrefs ()));
  actions->addWidget (applyButton);

  QPushButton *closeButton = new QPushButton (tr ("Cancel"), this);
  closeButton->setToolTip (tr ("Discard changes and close dialog"));
  closeButton->setWhatsThis (closePrefsText);
  connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotClosePrefs ()));
  actions->addWidget (closeButton);


  show ();
}



prefs::~prefs ()
{
}



void
prefs::slotTrackerColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (mod_options.tracker_color, this, tr ("fileEdit3D Tracker Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) mod_options.tracker_color = clr;

  setFields ();
}



void
prefs::slotBackgroundColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (mod_options.background_color, this, tr ("fileEdit3D Background Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) mod_options.background_color = clr;


  setFields ();
}



void
prefs::slotScaleColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (mod_options.scale_color, this, tr ("fileEdit3D Scale Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) mod_options.scale_color = clr;

  setFields ();
}



void
prefs::slotColorClicked (int id)
{
  QString name = tr ("Waveform/Marker %1").arg (id);

  QColor clr;

  clr = QColorDialog::getColor (mod_options.waveColor[id], this, tr ("fileEdit3D %1 Color").arg (name), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) mod_options.waveColor[id] = clr;

  setFields ();
}



void
prefs::slotAncillaryPrograms ()
{
  if (ancillaryProgramD) ancillaryProgramD->close ();


  ancillaryProgramD = new QDialog (this, (Qt::WindowFlags) Qt::WA_DeleteOnClose);
  ancillaryProgramD->setWindowTitle (tr ("fileEdit3D Ancillary Programs"));

  QVBoxLayout *vbox = new QVBoxLayout (ancillaryProgramD);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  QGroupBox *progBox = new QGroupBox (ancillaryProgramD);
  QHBoxLayout *progBoxLayout = new QHBoxLayout;
  progBox->setLayout (progBoxLayout);
  QVBoxLayout *progBoxLeftLayout = new QVBoxLayout;
  QVBoxLayout *progBoxRightLayout = new QVBoxLayout;
  progBoxLayout->addLayout (progBoxLeftLayout);
  progBoxLayout->addLayout (progBoxRightLayout);

  QGroupBox *programs[NUMPROGS];
  QHBoxLayout *programsLayout[NUMPROGS];

  for (int32_t i = 0 ; i < NUMPROGS ; i++)
    {
      programs[i] = new QGroupBox (ancillaryProgramD);
      programsLayout[i] = new QHBoxLayout;
      programs[i]->setLayout (programsLayout[i]);
      
      program[i] = new QLineEdit (ancillaryProgramD);
      program[i]->setReadOnly (NVTrue);
      program[i]->setToolTip (options->description[i]);
      program[i]->setWhatsThis (programText);
      program[i]->setText (mod_options.name[i]);
      program[i]->setCursorPosition (0);
      programsLayout[i]->addWidget (program[i], 10);

      hotkey[i] = new QLineEdit (ancillaryProgramD);
      hotkey[i]->setToolTip (tr ("Hot key"));
      hotkey[i]->setWhatsThis (hotkeyText);
      hotkey[i]->setText (mod_options.hotkey[i]);
      hotkey[i]->setMinimumWidth (50);
      connect (hotkey[i], SIGNAL (textEdited (const QString &)), this, SLOT (slotApplyAncillaryChanges (const QString &)));
      programsLayout[i]->addWidget (hotkey[i], 1);

      actionkey[i] = new QLineEdit (ancillaryProgramD);
      actionkey[i]->setToolTip (tr ("Action key(s)"));
      actionkey[i]->setWhatsThis (actionkeyText);
      actionkey[i]->setText (mod_options.action[i]);
      actionkey[i]->setMinimumWidth (70);
      connect (actionkey[i], SIGNAL (textEdited (const QString &)), this, SLOT (slotApplyAncillaryChanges (const QString &)));
      programsLayout[i]->addWidget (actionkey[i], 1);


      if (!(i % 2))
        {
          progBoxLeftLayout->addWidget (programs[i]);
        }
      else
        {
          progBoxRightLayout->addWidget (programs[i]);
        }
    }


  vbox->addWidget (progBox);


  QHBoxLayout *actions = new QHBoxLayout (0);
  vbox->addLayout (actions);

  QPushButton *bHelp = new QPushButton (ancillaryProgramD);
  bHelp->setIcon (QIcon (":/icons/contextHelp.png"));
  bHelp->setToolTip (tr ("Enter What's This mode for help"));
  connect (bHelp, SIGNAL (clicked ()), this, SLOT (slotHelp ()));
  actions->addWidget (bHelp);

  actions->addStretch (10);

  QPushButton *closeButton = new QPushButton (tr ("Close"), ancillaryProgramD);
  closeButton->setToolTip (tr ("Close the ancillary program dialog"));
  closeButton->setWhatsThis (closeAncillaryProgramText);
  connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotCloseAncillaryProgram ()));
  actions->addWidget (closeButton);


  //  Put the dialog in the middle of the screen.

  ancillaryProgramD->move (x () + width () / 2 - ancillaryProgramD->width () / 2, y () + height () / 2 - ancillaryProgramD->height () / 2);

  ancillaryProgramD->show ();
}



//  Cheating again - any change will cause everything to update.  It only takes a second anyway.

void 
prefs::slotApplyAncillaryChanges (const QString &text __attribute__ ((unused)))
{
  for (int32_t i = 0 ; i < NUMPROGS ; i++)
    {
      mod_options.hotkey[i] = hotkey[i]->text ();
      mod_options.action[i] = actionkey[i]->text ();
    }
}



void 
prefs::slotCloseAncillaryProgram ()
{
  ancillaryProgramD->close ();
}



void
prefs::slotHelp ()
{
  QWhatsThis::enterWhatsThisMode ();
}



void
prefs::slotRestoreDefaults ()
{
  void set_defaults (MISC *misc, OPTIONS *options, POINT_DATA *data, uint8_t restore);


  set_defaults (misc, options, data, NVTrue);


  mod_options = *options;
  mod_share = *misc->abe_share;


  setFields ();


  *misc->abe_share = mod_share;
  *options = mod_options;


  //  Let the calling program (pfmView) know that we've changed some things that it uses.

  if (misc->abe_share != NULL)
    {
      misc->abe_share->settings_changed = NVTrue;
      misc->abe_share->position_form = options->position_form;
      misc->abe_share->z_factor = options->z_factor;
      misc->abe_share->z_offset = options->z_offset;


      //  Save the mosaic viewer program name and options.

      for (int32_t i = 0 ; i < NUMPROGS ; i++)
        {
          if (options->prog[i].contains ("mosaicView"))
            {
              strcpy (misc->abe_share->mosaic_prog, options->prog[i].toLatin1 ());
              strcpy (misc->abe_share->mosaic_actkey, options->action[i].toLatin1 ());
              strcpy (misc->abe_share->mosaic_hotkey, options->hotkey[i].toLatin1 ());
              break;
            }
        }


      misc->abe_share->settings_changed = NVTrue;
    }


  emit dataChangedSignal ();

  close ();
}



void
prefs::slotPositionClicked (int id)
{
  mod_options.position_form = id;
}



void 
prefs::slotHotKeys ()
{
  if (hotKeyD) slotCloseHotKeys ();


  hotKeyD = new QDialog (this, (Qt::WindowFlags) Qt::WA_DeleteOnClose);
  hotKeyD->setWindowTitle (tr ("fileEdit3D Hot Keys"));

  QVBoxLayout *vbox = new QVBoxLayout (hotKeyD);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  hotKeyTable = new QTableWidget (HOTKEYS, 2, this);
  hotKeyTable->setWhatsThis (hotKeyTableText);
  hotKeyTable->setAlternatingRowColors (true);
  QTableWidgetItem *bItemH = new QTableWidgetItem (tr ("Button"));
  hotKeyTable->setHorizontalHeaderItem (0, bItemH);
  QTableWidgetItem *kItemH = new QTableWidgetItem (tr ("Key"));
  hotKeyTable->setHorizontalHeaderItem (1, kItemH);

  QTableWidgetItem *bItem[HOTKEYS], *kItem[HOTKEYS];


  for (int32_t i = 0 ; i < HOTKEYS ; i++)
    {
      bItem[i] = new QTableWidgetItem (misc->buttonIcon[i], misc->buttonText[i]);
      bItem[i]->setFlags (Qt::ItemIsEnabled);

      hotKeyTable->setItem (i, 0, bItem[i]);
      kItem[i] = new QTableWidgetItem (mod_options.buttonAccel[i]);
      hotKeyTable->setItem (i, 1, kItem[i]);
    }
  hotKeyTable->resizeColumnsToContents ();
  hotKeyTable->resizeRowsToContents ();

  vbox->addWidget (hotKeyTable, 1);

  int32_t w = qMin (800, hotKeyTable->columnWidth (0) + hotKeyTable->columnWidth (1) + 40);
  int32_t h = qMin (600, hotKeyTable->rowHeight (0) * HOTKEYS + 75);
  hotKeyD->resize (w, h);


  QHBoxLayout *actions = new QHBoxLayout (0);
  vbox->addLayout (actions);

  QPushButton *bHelp = new QPushButton (hotKeyD);
  bHelp->setIcon (QIcon (":/icons/contextHelp"));
  bHelp->setToolTip (tr ("Enter What's This mode for help"));
  connect (bHelp, SIGNAL (clicked ()), this, SLOT (slotHelp ()));
  actions->addWidget (bHelp);

  actions->addStretch (10);

  QPushButton *closeButton = new QPushButton (tr ("Close"), hotKeyD);
  closeButton->setToolTip (tr ("Close the hot key dialog"));
  closeButton->setWhatsThis (closeHotKeyText);
  connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotCloseHotKeys ()));
  actions->addWidget (closeButton);


  //  Put the dialog in the middle of the screen.

  hotKeyD->move (x () + width () / 2 - hotKeyD->width () / 2, y () + height () / 2 - hotKeyD->height () / 2);

  hotKeyD->show ();
}



void 
prefs::slotCloseHotKeys ()
{
  for (int32_t i = 0 ; i < HOTKEYS ; i++) mod_options.buttonAccel[i] = hotKeyTable->item (i, 1)->text ();

  hotKeyD->close ();
}



void 
prefs::slotEgmClicked ()
{
  double lat, lon;

  lat = data->bounds.min_y + (data->bounds.max_y - data->bounds.min_y) / 2.0;
  lon = data->bounds.min_x + (data->bounds.max_x - data->bounds.min_x) / 2.0;

  qApp->setOverrideCursor (Qt::WaitCursor);
  qApp->processEvents ();

  float datum_offset = get_egm08 (lat, lon);
  cleanup_egm08 ();

  qApp->restoreOverrideCursor ();

  if (datum_offset < 999999.0) 
    {
      offset->setValue ((double) datum_offset);
    }
  else
    {
      QMessageBox::warning (this, tr ("Get EGM08 datum offset"), tr ("Error retrieving EGM08 ellipsoid to geoid offset value"));
    }
}



void 
prefs::slotMinZValueChanged (double value __attribute__ ((unused)))
{
  //  We manually modified the Z value so we want to set the IHO combo box to "Manual".

  ihoOrder->setCurrentIndex (0);
}



void 
prefs::setFields ()
{
  QString string;

  bGrp->button (mod_options.position_form)->setChecked (true);


  int32_t hue, sat, val;

  for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++)
    {
      mod_options.waveColor[i].getHsv (&hue, &sat, &val);
      if (val < 128)
        {
          bWavePalette[i].setColor (QPalette::Normal, QPalette::ButtonText, Qt::white);
          bWavePalette[i].setColor (QPalette::Inactive, QPalette::ButtonText, Qt::white);
        }
      else
        {
          bWavePalette[i].setColor (QPalette::Normal, QPalette::ButtonText, Qt::black);
          bWavePalette[i].setColor (QPalette::Inactive, QPalette::ButtonText, Qt::black);
        }
      bWavePalette[i].setColor (QPalette::Normal, QPalette::Button, mod_options.waveColor[i]);
      bWavePalette[i].setColor (QPalette::Inactive, QPalette::Button, mod_options.waveColor[i]);
      bWaveColor[i]->setPalette (bWavePalette[i]);
    }


  mod_options.tracker_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      bTrackerPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::white);
      bTrackerPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::white);
    }
  else
    {
      bTrackerPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::black);
      bTrackerPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::black);
    }
  bTrackerPalette.setColor (QPalette::Normal, QPalette::Button, mod_options.tracker_color);
  bTrackerPalette.setColor (QPalette::Inactive, QPalette::Button, mod_options.tracker_color);
  bTrackerColor->setPalette (bTrackerPalette);


  mod_options.background_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      bBackgroundPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::white);
      bBackgroundPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::white);
    }
  else
    {
      bBackgroundPalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::black);
      bBackgroundPalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::black);
    }
  bBackgroundPalette.setColor (QPalette::Normal, QPalette::Button, mod_options.background_color);
  bBackgroundPalette.setColor (QPalette::Inactive, QPalette::Button, mod_options.background_color);
  bBackgroundColor->setPalette (bBackgroundPalette);


  mod_options.scale_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      bScalePalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::white);
      bScalePalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::white);
    }
  else
    {
      bScalePalette.setColor (QPalette::Normal, QPalette::ButtonText, Qt::black);
      bScalePalette.setColor (QPalette::Inactive, QPalette::ButtonText, Qt::black);
    }
  bScalePalette.setColor (QPalette::Normal, QPalette::Button, mod_options.scale_color);
  bScalePalette.setColor (QPalette::Inactive, QPalette::Button, mod_options.scale_color);
  bScaleColor->setPalette (bScalePalette);


  graySliceCheck->setChecked (mod_options.slice_gray);


  string = QString ("%L1").arg (mod_options.z_factor, 0, 'f', 5);
  ZFactor->lineEdit ()->setText (string);

  offset->setValue (mod_options.z_offset);

  pointSize->setValue (mod_options.point_size);
  pointLimit->setValue (mod_options.point_limit);

  minZ->setValue (mod_options.min_window_size);

  ihoOrder->setCurrentIndex (mod_options.iho_min_window);

  filterType->setCurrentIndex (mod_options.filter_type);

  filterSTD->setValue (mod_options.filterSTD);

  dFilter->setChecked (mod_options.deep_filter_only);

  hpThreads->setCurrentIndex (NINT (log10 ((double) mod_options.num_hp_threads) / log10 (4.0)));

  hpRadius->setValue (mod_options.hp_filter_radius);

  hpHeight->setValue (mod_options.hp_filter_height);

  hpNeighbors->setValue (mod_options.hp_filter_neighbors);

  switch (mod_options.main_button_icon_size)
    {
    case 16:
      iconSize->setCurrentIndex (0);
      break;

    case 20:
      iconSize->setCurrentIndex (1);
      break;

    case 24:
    default:
      iconSize->setCurrentIndex (2);
      break;

    case 28:
      iconSize->setCurrentIndex (3);
      break;

    case 32:
      iconSize->setCurrentIndex (4);
      break;
    }

      
  zoomPercent->setValue (mod_options.zoom_percent);
  rot->setValue (mod_options.rotation_increment);
  exag->setValue (mod_options.exaggeration);
  undo->setValue (mod_options.undo_levels);
  tip->setChecked (mod_options.highlight_tip);
  scale->setChecked (mod_options.draw_scale);
}



void
prefs::slotApplyPrefs ()
{
  int32_t status;
  float tmp_f;

  void get_buffer (POINT_DATA *data, MISC *misc, int32_t record);


  if (mod_options.position_form != options->position_form) dataChanged = NVTrue;


  QString tmp = ZFactor->currentText ();
  status = sscanf (tmp.toLatin1 (), "%f", &tmp_f);
  if (status == 1) mod_options.z_factor = tmp_f;
  if (mod_options.z_factor != options->z_factor) dataChanged = NVTrue;


  mod_options.z_offset = offset->value ();
  if (mod_options.z_offset != options->z_offset) dataChanged = NVTrue;


  mod_options.point_size = pointSize->value ();
  if (mod_options.point_size != options->point_size) dataChanged = NVTrue;

  mod_options.point_limit = pointLimit->value ();
  if (mod_options.point_limit != options->point_limit)
    {
      options->point_limit = mod_options.point_limit;

      int32_t record = get_record () - misc->num_records;
      io_data_close ();
      io_data_open (parent, misc, options);

      get_buffer (data, misc, record);

      dataChanged = NVTrue;
    }

  mod_options.min_window_size = minZ->value ();
  if (mod_options.min_window_size != options->min_window_size) dataChanged = NVTrue;

  mod_options.iho_min_window = ihoOrder->currentIndex ();
  if (mod_options.iho_min_window != options->iho_min_window) dataChanged = NVTrue;

  mod_options.filter_type = filterType->currentIndex ();
  if (mod_options.filter_type != options->filter_type) dataChanged = NVTrue;


  //  We don't need to say that the data changed if we just mod the filter options.

  mod_options.filterSTD = filterSTD->value ();
  if (mod_options.filterSTD != options->filterSTD) options->filterSTD = mod_options.filterSTD;

  mod_options.deep_filter_only = dFilter->isChecked ();
  if (mod_options.deep_filter_only != options->deep_filter_only) options->deep_filter_only = mod_options.deep_filter_only;

  mod_options.num_hp_threads = NINT (pow (4.0, (double) hpThreads->currentIndex ()));
  if (mod_options.num_hp_threads != options->num_hp_threads) dataChanged = NVTrue;

  mod_options.hp_filter_radius = hpRadius->value ();
  if (mod_options.hp_filter_radius != options->hp_filter_radius) options->hp_filter_radius = mod_options.hp_filter_radius;

  mod_options.hp_filter_height = hpHeight->value ();
  if (mod_options.hp_filter_height != options->hp_filter_height) options->hp_filter_height = mod_options.hp_filter_height;

  mod_options.hp_filter_neighbors = hpNeighbors->value ();
  if (mod_options.hp_filter_neighbors != options->hp_filter_neighbors) options->hp_filter_neighbors = mod_options.hp_filter_neighbors;

  switch (iconSize->currentIndex ())
    {
    case 0:
      mod_options.main_button_icon_size = 16;
      break;

    case 1:
      mod_options.main_button_icon_size = 20;
      break;

    case 2:
      mod_options.main_button_icon_size = 24;
      break;

    case 3:
      mod_options.main_button_icon_size = 28;
      break;

    case 4:
      mod_options.main_button_icon_size = 32;
      break;
    }
  if (mod_options.main_button_icon_size != options->main_button_icon_size) dataChanged = NVTrue;

  mod_options.rotation_increment = rot->value ();
  if (mod_options.rotation_increment != options->rotation_increment) dataChanged = NVTrue;

  mod_options.exaggeration = exag->value ();
  if (mod_options.exaggeration != options->exaggeration) dataChanged = NVTrue;

  mod_options.zoom_percent = zoomPercent->value ();
  if (mod_options.zoom_percent != options->zoom_percent) dataChanged = NVTrue;

  mod_options.undo_levels = undo->value ();
  if (mod_options.undo_levels != options->undo_levels && resize_undo (misc, options, mod_options.undo_levels)) dataChanged = NVTrue;

  mod_options.highlight_tip = tip->isChecked ();
  if (mod_options.highlight_tip != options->highlight_tip) dataChanged = NVTrue;

  mod_options.draw_scale = scale->isChecked ();
  if (mod_options.draw_scale != options->draw_scale) dataChanged = NVTrue;

  if (mod_options.tracker_color != options->tracker_color) dataChanged = NVTrue;
  if (mod_options.background_color != options->background_color) dataChanged = NVTrue;
  if (mod_options.scale_color != options->scale_color) dataChanged = NVTrue;


  for (int32_t i = 0 ; i < MAX_STACK_POINTS ; i++)
    {
      if (mod_options.waveColor[i] != options->waveColor[i])
        {
          mod_share.mwShare.multiColors[i].r = mod_options.waveColor[i].red ();
          mod_share.mwShare.multiColors[i].g = mod_options.waveColor[i].green ();
          mod_share.mwShare.multiColors[i].b = mod_options.waveColor[i].blue ();
          mod_share.mwShare.multiColors[i].a = mod_options.waveColor[i].alpha ();
          dataChanged = NVTrue;
        }
    }

  mod_options.slice_gray = graySliceCheck->isChecked ();
  if (mod_options.slice_gray != options->slice_gray) dataChanged = NVTrue;


  //  Make sure we have no hotkey duplications.  First, the buttons against the buttons.

  for (int32_t i = 0 ; i < HOTKEYS ; i++)
    {
      for (int32_t j = i + 1 ; j < HOTKEYS ; j++)
        {
          if (mod_options.buttonAccel[i].toUpper () == mod_options.buttonAccel[j].toUpper ())
            {
              QMessageBox::warning (0, tr ("fileEdit3D preferences"), tr ("Hot key for <b>%1</b> button conflicts with hotkey for <b>%2</b> button!<br><br>"
                                                                          "Resetting button hotkeys for these two buttons.").arg 
                                    (misc->buttonText[i]).arg (misc->buttonText[j]));

              mod_options.buttonAccel[i] = options->buttonAccel[i];
              mod_options.buttonAccel[j] = options->buttonAccel[j];
              dataChanged = NVFalse;

              hotKeyD->close ();
              hotKeyD = NULL;

              return;
            }
        }
    }


  //  Next, the programs against the programs.

  for (int32_t i = 0 ; i < NUMPROGS ; i++)
    {
      for (int32_t j = i + 1 ; j < NUMPROGS ; j++)
        {
          if (mod_options.hotkey[i].toUpper () == mod_options.hotkey[j].toUpper ())
            {
              QMessageBox::warning (0, tr ("fileEdit3D preferences"), tr ("Hot key for <b>%1</b> program conflicts with hotkey for <b>%2</b> program!<br><br>"
                                                                          "Resetting program hotkeys for these two programs.").arg
                                    (options->name[i]).arg (options->name[j]));

              mod_options.hotkey[i] = options->hotkey[i];
              mod_options.hotkey[j] = options->hotkey[j];
              dataChanged = NVFalse;

              ancillaryProgramD->close ();
              ancillaryProgramD = NULL;

              return;
            }
        }
    }


  //  Finally, the buttons against the programs.

  for (int32_t i = 0 ; i < HOTKEYS ; i++)
    {
      for (int32_t j = 0 ; j < NUMPROGS ; j++)
        {
          if (mod_options.buttonAccel[i].toUpper () == mod_options.hotkey[j].toUpper ())
            {
              QMessageBox::warning (0, tr ("fileEdit3D preferences"), tr ("Hot key for <b>%1</b> button conflicts with hotkey for <b>%2</b> program!<br><br>"
                                                                          "Resetting button and program hotkeys.").arg
                                    (misc->buttonText[i]).arg (options->name[j]));

              mod_options.buttonAccel[i] = options->buttonAccel[i];
              mod_options.hotkey[j] = options->hotkey[j];
              dataChanged = NVFalse;

              hotKeyD->close ();
              hotKeyD = NULL;
              ancillaryProgramD->close ();
              ancillaryProgramD = NULL;

              return;
            }
        }
    }


  //  Now check for changes to the hotkeys.

  for (int32_t i = 0 ; i < HOTKEYS ; i++)
    {
      if (mod_options.buttonAccel[i] != options->buttonAccel[i])
        {
          dataChanged = NVTrue;
          emit hotKeyChangedSignal (i);
          break;
        }
    }


  for (int32_t i = 0 ; i < NUMPROGS ; i++)
    {
      if (mod_options.hotkey[i] != options->hotkey[i] ||
          mod_options.action[i] != options->action[i])
        {
          dataChanged = NVTrue;
          break;
        }

      for (int32_t j = 0 ; j < PFM_DATA_TYPES ; j++)
        {
          if (mod_options.data_type[i][j] != options->data_type[i][j])
            {
              dataChanged = NVTrue;
              break;
            }
        }

      if (dataChanged) break;
    }


  setFields ();


  hide ();


  if (dataChanged)
    {
      *misc->abe_share = mod_share;
      *options = mod_options;


      emit dataChangedSignal ();
    }

  close ();
}



void
prefs::slotClosePrefs ()
{
  close ();
}
