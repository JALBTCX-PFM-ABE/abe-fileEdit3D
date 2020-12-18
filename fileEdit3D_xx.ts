<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1">
<context>
    <name>LAS_WKT</name>
    <message>
        <location filename="LAS_WKT.cpp" line="170"/>
        <location filename="LAS_WKT.cpp" line="426"/>
        <source>%1 %2 %3 %4 - Error initializing UTM projection!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="180"/>
        <location filename="LAS_WKT.cpp" line="443"/>
        <source>%1 %2 %3 %4 - Error initializing latlon projection!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="463"/>
        <location filename="LAS_WKT.cpp" line="686"/>
        <location filename="LAS_WKT.cpp" line="723"/>
        <source>%1 Well-known Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="481"/>
        <source>The LAS file &lt;b&gt;%1&lt;/b&gt; is version 1.4.  The &lt;b&gt;global_encoding&lt;/b&gt; field of the LAS header indicates that there is a Coordinate System Well-known Text (WKT) record in the file however, it appears to be missing.  This means that the LAS file does not meet the LAS 1.4 specification.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="490"/>
        <source>The LAS file &lt;b&gt;%1&lt;/b&gt; is version 1.4.  The &lt;b&gt;global_encoding&lt;/b&gt; field of the LAS header indicates that there is not a Coordinate System Well-known Text (WKT) record in the file.  Since the Point Data Record Format for this file is greater than 5, the use of the WKT record is required.  This LAS file does not meet the LAS 1.4 specification.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="500"/>
        <source>The LAS file &lt;b&gt;%1&lt;/b&gt; is version 1.%2.  There are no Variable Length Records (VLR) defined for this file.  According to the LAS 1.%2 specification, a 34735 (GeoKeyDirectoryTag) VLR is required.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="509"/>
        <source>The LAS file &lt;b&gt;%1&lt;/b&gt; has a 34735 Variable Length Record (VLR) but the &lt;b&gt;GTModelTypeGeoKey&lt;/b&gt; isn&apos;t &lt;b&gt;ModelTypeProjected&lt;/b&gt; or &lt;b&gt;ModelTypeGeographic&lt;/b&gt;.  &lt;b&gt;%2&lt;/b&gt; only handles those two types.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="517"/>
        <source>The LAS file &lt;b&gt;%1&lt;/b&gt; is version 1.%2.  The required 34735 (GeoKeyDirectoryTag) Variable Length Record (VLR) is missing.  According to the LAS 1.%2 specification, a 34735 (GeoKeyDirectoryTag) VLR is required</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="525"/>
        <source>The LAS file &lt;b&gt;%1&lt;/b&gt; has a 34735 Variable Length Record (VLR) with the &lt;b&gt;GTModelTypeGeoKey&lt;/b&gt; set to &lt;b&gt;ModelTypeProjected&lt;/b&gt;.  &lt;b&gt;%2&lt;/b&gt; has determined that the &lt;b&gt;Geographic CS Type&lt;/b&gt; is not &lt;b&gt;GCS_NAD83&lt;/b&gt; (4269) or &lt;b&gt;GCS_WGS_84&lt;/b&gt; (4326).  &lt;b&gt;%2&lt;/b&gt; only handles those two types.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="531"/>
        <source>&lt;br&gt;&lt;br&gt;Due to the above listed condition &lt;b&gt;%1&lt;/b&gt; is unable to convert positions for the input LAS data to latitude and longitude.  If you would like to know why the positions are being converted to latitude and longitude you can read &lt;a href=&quot;http://pfmabe.software/a-note-about-utm-vs-latlon/&quot;&gt;this article&lt;/a&gt; on pfmabe.software.&lt;br&gt;&lt;br&gt;This dialog will allow you to enter Well-known Text (WKT) to be used to convert the positions.  WKT for many different areas is available on &lt;a href=&quot;http://spatialreference.org/&quot;&gt;spatialreference.org&lt;/a&gt; (for example, &lt;a href=&quot;http://spatialreference.org/ref/epsg/2451/&quot;&gt;EPSG:2451&lt;/a&gt;).  Only GEOGCS or PROJCS WKT is supported.&lt;br&gt;&lt;br&gt;Simply copy the &lt;b&gt;OGC WKT&lt;/b&gt; that best matches the data in the above LAS file and paste it into the text box below.&lt;br&gt;&lt;br&gt;Do not be overly concerned about whether the WKT is exactly correct for your data.  The Area-Based Editor (ABE) is only used to modify status (e.g. &lt;b&gt;Withheld&lt;/b&gt;, &lt;b&gt;Synthetic&lt;/b&gt;, &lt;b&gt;Key-point&lt;/b&gt;) and/or classification (e.g. &lt;b&gt;Low Vegetation&lt;/b&gt;, &lt;b&gt;Building&lt;/b&gt;, &lt;b&gt;Road Surface&lt;/b&gt;) in the LAS files (when unloaded from ABE).  The positions in the LAS files are not modified.  Because of this, the absolute accuracy of the positions in ABE is not really important.  It is much more important that the positions are accurate relative to each other and fairly accurate in relation to the globe (in order to use Google Earth for external reference).&lt;br&gt;&lt;br&gt;If the data in the LAS file is projected, &lt;b&gt;%1&lt;/b&gt; will convert the input positions to WGS84 latitude and longitude.&lt;br&gt;&lt;br&gt;If you do not enter WKT data and click the &lt;b&gt;Close&lt;/b&gt; button, &lt;b&gt;%1&lt;/b&gt; will be terminated.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="549"/>
        <source>Click &lt;b&gt;Help&lt;/b&gt; button then click here for an even more verbose explanation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="565"/>
        <location filename="LAS_WKT.cpp" line="566"/>
        <source>Select a previously used WKT string (hover to see WKT in tool tip)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="610"/>
        <source>Enter the Well-known Text (WKT) for the LAS input file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="611"/>
        <source>Enter the Well-known Text (WKT) coordinate system definition for the LAS input file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="612"/>
        <source>Well-known Text (WKT)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="622"/>
        <source>Enter What&apos;s This mode for help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="628"/>
        <source>Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="629"/>
        <source>Discard WKT text, close this dialog, and exit the program</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="633"/>
        <source>Accept</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="634"/>
        <source>Accept the WKT text that has been entered, close this dialog, and process the files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="687"/>
        <source>The WKT data entered does not appear to be correct and/or supported.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKT.cpp" line="723"/>
        <source>The WKT text input field is empty.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="LAS_WKTHelp.hpp" line="41"/>
        <source>&lt;b&gt;&lt;/b&gt;So, the problem with many LAS files is that the coordinate system is not defined properly.  Even though the LAS specification requires a 34735 (GeoKeyDirectoryTag) Variable Length Record (VLR) many people don&apos;t put one in.  If you&apos;re using LAS 1.4 and you use point data formats above 5 you are required to insert a Well-known Text (WKT) VLR or Extended VLR (eVLR).  Apparently many people think this is a waste of time.  I find that rather interesting, especially with projected data.  For example, I recently saw some data from a LiDAR system in use in Japan that was in LAS 1.4 format using point data format 6.  This requires a WKT VLR or eVLR but there wasn&apos;t one in the file.  The global_encoding field was set to indicate that there was a 34735 VLR. That is an error.  There wasn&apos;t even a 34735 VLR anyway.  Since the file appeared to be UTM what we had was a whole bunch of measurements (northing and easting, presumably in meters) from nowhere (or possibly OZ).&lt;br&gt;&lt;br&gt;Even when people insert the 34735 record they often leave out important information.  For instance, I&apos;ve seen files that proudly proclaim that they are NAD83 UTM but then they don&apos;t put the zone information into the record.  Once again, meters (we hope) from nowhere.&lt;br&gt;&lt;br&gt;Another pet peeve of mine - why do people use any vertical measurement other than meters (or metres if you will)?  Measuring things based on the length of some king&apos;s body parts seems a bit old school.  Wait until you&apos;ve finished processing the data before you convert it to cubits or fathoms or furlongs or feet or yards for the great unwashed masses to gaze upon.  ABE assumes that the input elevations are in meters.  If the data in your LAS file isn&apos;t in meters it doesn&apos;t really matter.  Just look at the pretty numbers and say to yourself &lt;b&gt;&apos;self, that number is really feet even though it says meters&apos;&lt;/b&gt;.  If you&apos;re building PFMs from LAS files I wouldn&apos;t recommend trying to extract (pfmExtract or pfmGeotiff) the data (meaning X, Y, and Z, not validity and/or classification).&lt;br&gt;&lt;br&gt;Now, on to the fun stuff, if you&apos;re not familiar with WKT you can look it up on the interwebs.  To quote Wikipedia &lt;b&gt;&apos;Well-known text (WKT) is a text markup language for representing vector geometry objects on a map, spatial reference systems of spatial objects and transformations between spatial reference systems&apos;&lt;/b&gt;. The world has moved past the GeoKeyDirectoryTag and is moving to WKT. Since this program uses the GDAL/OGR library it only supports projected (PROJCS) or geographic (GEOGCS) coordinate systems.  That&apos;s probably fine since that&apos;s what most people use (the other possibility being (GEOCCS) geocentric).&lt;br&gt;&lt;br&gt;This program will try to decipher the crazy quilt LAS oordinate system information but it won&apos;t try very hard.  If you are using geographic data it will probably make it through fine (unless you&apos;ve completely hosed or forgotten your 34735).  Projected data is much more complicated. In any event, if the program can&apos;t figure it out it will ask you for a WKT describing the coordinate system for your data.  You can find a bunch of WKT over on &lt;a href=&quot;http://spatialreference.org/&quot;&gt;spatialreference.org&lt;/a&gt;.&lt;br&gt;&lt;br&gt;Once you&apos;ve entered one or more WKT definitions the program will try to keep track of the 10 most recently used ones. These will be available in any of the ABE programs that have LAS inputs.  Hopefully you&apos;ll be using the same few for quite a while before you have to change to another area and/or coordinate system.  To see what each recent WKT string looks like just open the combo box on the bottom left and let the cursor hover over each entry.  The tool tip will be the full text of the WKT in human readable form.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: In the pfmLoad or pfmLoader program, if you input LAS files and have to input WKT to load them, the program will keep track of the WKT and will use it again if you append to the existing PFM.&lt;/b&gt;&lt;br&gt;&lt;br&gt;Jan Depner, 3 May, 2016</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../../../PFM_ABEv7.0.0/include/acknowledgments.hpp" line="39"/>
        <source>&lt;center&gt;&lt;br&gt;This program was built using some, if not all, of the following Open Source or public domain resources:&lt;br&gt;&lt;br&gt;&lt;/center&gt;&lt;ul&gt;&lt;li&gt;&lt;a href=http://beecrypt.sourceforge.net&gt;BeeCrypt&lt;/a&gt; - Cryptography library&lt;/li&gt;&lt;li&gt;&lt;a href=http://www.gdal.org&gt;GDAL&lt;/a&gt; - Geospatial Data Abstraction Library&lt;/li&gt;&lt;li&gt;&lt;a href=https://www.leidos.com/maritime/gsf&gt;GSF&lt;/a&gt; - Generic Sensor Format Library&lt;/li&gt;&lt;li&gt;&lt;a href=http://www.hdfgroup.org/HDF5&gt;HDF5&lt;/a&gt; - Heirarchical Data Format library&lt;/li&gt;&lt;li&gt;&lt;a href=http://www.ijg.org&gt;libjpeg&lt;/a&gt; - JPEG library&lt;/li&gt;&lt;li&gt;&lt;a href=http://rapidlasso.com&gt;LASlib&lt;/a&gt; - LAS I/O library&lt;/li&gt;&lt;li&gt;&lt;a href=http://www.opennavsurf.org&gt;OpenNS&lt;/a&gt; - Open Navigation Surface API&lt;/li&gt;&lt;li&gt;&lt;a href=http://trac.osgeo.org/proj&gt;PROJ.4&lt;/a&gt; - Cartographic Projections Library&lt;/li&gt;&lt;li&gt;&lt;a href=http://qt-project.org&gt;Qt&lt;/a&gt; - A cross-platform application and UI framework&lt;/li&gt;&lt;li&gt;&lt;a href=http://shapelib.maptools.org&gt;SHAPELIB&lt;/a&gt; - Shapefile C Library&lt;/li&gt;&lt;li&gt;&lt;a href=https://sqlite.org&gt;SQLite&lt;/a&gt; - SQL database engine API&lt;/li&gt;&lt;li&gt;&lt;a href=http://xmlsoft.org&gt;XML2&lt;/a&gt; - XML C parser&lt;/li&gt;&lt;li&gt;&lt;a href=http://www.zlib.net&gt;ZLIB&lt;/a&gt; - Compression Library&lt;/li&gt;&lt;li&gt;&lt;a href=http://tango.freedesktop.org/Tango_Icon_Library&gt;Tango Icon Library&lt;/a&gt; - Icon set for the Tango Desktop Project&lt;/li&gt;&lt;li&gt;&lt;a href=http://www.smashingmagazine.com/2009/05/20/flavour-extended-the-ultimate-icon-set-for-web-designers&gt;Flavour Extended&lt;/a&gt; - Icon set by Oliver Twardowski&lt;/li&gt;&lt;li&gt;&lt;a href=http://ddrdark.deviantart.com/art/WIP-32px-mania-iconset-106090029&gt;32pxmania&lt;/a&gt; - Icon set by DDrDark&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;&lt;ul&gt;&lt;li&gt;BeeCrypt, GSF, LASlib, Qt, and SHAPELIB are licensed under the &lt;a href=http://www.gnu.org/copyleft/lesser.html&gt;GNU LGPL&lt;/a&gt;&lt;/li&gt;&lt;li&gt;GDAL is licensed under an X/MIT style open source license&lt;/li&gt;&lt;li&gt;libjpeg, OpenNS, ZLIB, and HDF5 are licensed under their own open source licenses&lt;/li&gt;&lt;li&gt;PROJ.4 and XML2 are licensed under an MIT open source license&lt;/li&gt;&lt;li&gt;SQLite and the Tango Icon Set are released in the public domain&lt;/li&gt;&lt;li&gt;Flavour Extended is free for any private or commercial use without any restrictions&lt;/li&gt;&lt;li&gt;32pxmania is free for personal and commercial use with the proviso that the icon set can not be sold as a standalone icon set&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;&lt;br&gt;Many thanks to the authors of these and all of their supporting projects.  For more information on each project please visit their websites using the links above.&lt;br&gt;&lt;center&gt;Jan C. Depner&lt;br&gt;&lt;br&gt;&lt;/center&gt;</source>
        <comment>This file is included in almost all ABE GUI programs (e.g. pfmView, pfmEdit3D, areaCheck) so it may have already been translated.  If there is a .ts file in one of the ABE GUI programs for the language that you are translating to, a translation for this text may already be done.  Please run linguist on one of those files to check.</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="slas.cpp" line="86"/>
        <location filename="slas.cpp" line="96"/>
        <location filename="slas.cpp" line="476"/>
        <location filename="slas.cpp" line="486"/>
        <source>Record number %L1 out of range :
Function: %2, Line: %3
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="slas.cpp" line="109"/>
        <location filename="slas.cpp" line="397"/>
        <location filename="slas.cpp" line="501"/>
        <location filename="slas.cpp" line="753"/>
        <source>Error on fseek :
%1
Function: %2, Line: %3
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="slas.cpp" line="124"/>
        <location filename="slas.cpp" line="515"/>
        <source>Error reading LAS record :
%1
Function: %2, Line: %3
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="slas.cpp" line="411"/>
        <source>Error reading LAS waveform data :
%1
Function: %2, Line: %3
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="slas.cpp" line="598"/>
        <source>Classification value %1 out of bounds :
Function: %2, Line: %3
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="slas.cpp" line="764"/>
        <source>Error writing LAS record :
%1
Function: %2, Line: %3
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ancillaryProg</name>
    <message>
        <location filename="ancillaryProg.cpp" line="109"/>
        <source>Enter What&apos;s This mode for help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ancillaryProg.cpp" line="115"/>
        <source>Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ancillaryProg.cpp" line="116"/>
        <source>Close the command line program dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ancillaryProg.cpp" line="175"/>
        <location filename="ancillaryProg.cpp" line="201"/>
        <location filename="ancillaryProg.cpp" line="205"/>
        <location filename="ancillaryProg.cpp" line="209"/>
        <location filename="ancillaryProg.cpp" line="213"/>
        <location filename="ancillaryProg.cpp" line="217"/>
        <location filename="ancillaryProg.cpp" line="221"/>
        <source>fileEdit3D ancillary program</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ancillaryProg.cpp" line="175"/>
        <source>The program %1 has been started but may take some time to appear.
Please be patient, it will appear eventually.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ancillaryProg.cpp" line="201"/>
        <source>Unable to start the ancillary program!
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ancillaryProg.cpp" line="205"/>
        <source>The ancillary program crashed!
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ancillaryProg.cpp" line="209"/>
        <source>The ancillary program timed out!
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ancillaryProg.cpp" line="213"/>
        <source>There was a write error to the ancillary program!
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ancillaryProg.cpp" line="217"/>
        <source>There was a read error from the ancillary program!
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="ancillaryProg.cpp" line="221"/>
        <source>The ancillary program died with an unknown error!
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>fileEdit3D</name>
    <message>
        <location filename="fileEdit3D.cpp" line="333"/>
        <source>main button icon size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="339"/>
        <location filename="fileEdit3D.cpp" line="340"/>
        <location filename="fileEdit3D.cpp" line="342"/>
        <source>View tool bar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="374"/>
        <source>Display invalid data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="394"/>
        <source>Turn off data flags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="399"/>
        <source>Mark suspect data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="404"/>
        <source>Mark invalid data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="411"/>
        <source>Toggle data flags on/off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="453"/>
        <location filename="fileEdit3D.cpp" line="454"/>
        <location filename="fileEdit3D.cpp" line="456"/>
        <source>Utilities tool bar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="461"/>
        <source>Change application preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="469"/>
        <source>Link to other ABE applications</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="486"/>
        <location filename="fileEdit3D.cpp" line="487"/>
        <location filename="fileEdit3D.cpp" line="489"/>
        <source>Edit tool bar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="493"/>
        <source>Stop drawing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="541"/>
        <source>Delete data outside polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="554"/>
        <source>Restore invalid data inside rectangle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="562"/>
        <source>Restore invalid data inside polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="571"/>
        <location filename="set_defaults.cpp" line="87"/>
        <source>Undo last edit operation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="619"/>
        <source>Clear filter masks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="631"/>
        <source>Measure distance, azimuth, and Z difference</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="640"/>
        <location filename="fileEdit3D.cpp" line="641"/>
        <location filename="fileEdit3D.cpp" line="643"/>
        <source>Mask tool bar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="647"/>
        <source>Mask data outside a rectangle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="655"/>
        <source>Mask data outside a polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="668"/>
        <source>Mask data inside a rectangle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="676"/>
        <source>Mask data inside a polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="689"/>
        <source>Clear data mask</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="698"/>
        <location filename="fileEdit3D.cpp" line="699"/>
        <location filename="fileEdit3D.cpp" line="701"/>
        <source>LiDAR tool bar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="756"/>
        <location filename="fileEdit3D.cpp" line="757"/>
        <location filename="fileEdit3D.cpp" line="759"/>
        <source>GSF tool bar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="775"/>
        <location filename="fileEdit3D.cpp" line="776"/>
        <location filename="fileEdit3D.cpp" line="778"/>
        <source>LAS tool bar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="859"/>
        <source>;  action keys = </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="862"/>
        <source>For more information on data types and other restrictions on the use of hot keys see &lt;b&gt;Ancillary Programs&lt;/b&gt; in the &lt;b&gt;Preferences&lt;/b&gt; dialog - &lt;img source=&quot;:/icons/prefs.png&quot;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1000"/>
        <source>2D Tracker</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1010"/>
        <location filename="fileEdit3D.cpp" line="1084"/>
        <source>000000 points selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1011"/>
        <source>Please use the What&apos;s This help button then click here for full documentation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1017"/>
        <source>STD</source>
        <comment>Abbreviation for standard deviation</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1031"/>
        <location filename="fileEdit3D.cpp" line="1042"/>
        <source>Filter standard deviation value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1056"/>
        <location filename="fileEdit3D.cpp" line="1125"/>
        <source>Accept</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1057"/>
        <location filename="fileEdit3D.cpp" line="1126"/>
        <source>Accept filter results [%1]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1058"/>
        <location filename="fileEdit3D.cpp" line="1127"/>
        <source>Accept filter results and invalidate marked points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1063"/>
        <location filename="fileEdit3D.cpp" line="1132"/>
        <source>Reject</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1064"/>
        <location filename="fileEdit3D.cpp" line="1133"/>
        <source>Reject filter results [%1]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1065"/>
        <source>Reject filter results</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1073"/>
        <source>Area-Based Filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1093"/>
        <source>Hockey Puck filter radius value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1096"/>
        <source>&amp;Radius:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1102"/>
        <source>Hockey Puck filter height value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1105"/>
        <source>&amp;Height:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1111"/>
        <source>Hockey Puck filter neighbors value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1114"/>
        <source>&amp;Neighbors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1134"/>
        <source>Discard filter results</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1142"/>
        <source>Hockey Puck Filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1162"/>
        <location filename="fileEdit3D.cpp" line="1163"/>
        <source>Number of points in current view</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1185"/>
        <location filename="fileEdit3D.cpp" line="1190"/>
        <source>Latitude of cursor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1195"/>
        <location filename="fileEdit3D.cpp" line="1200"/>
        <source>Longitude of cursor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1205"/>
        <location filename="fileEdit3D.cpp" line="1211"/>
        <source>Record number of current point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1216"/>
        <location filename="fileEdit3D.cpp" line="1222"/>
        <source>Subrecord number of current point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1227"/>
        <location filename="fileEdit3D.cpp" line="1232"/>
        <location filename="fileEdit3D.cpp" line="3438"/>
        <location filename="fileEdit3D.cpp" line="3439"/>
        <source>Current point Z value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1239"/>
        <location filename="fileEdit3D.cpp" line="1244"/>
        <location filename="fileEdit3D.cpp" line="1280"/>
        <source>Vertical exaggeration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1261"/>
        <source>File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1267"/>
        <source>Move through the file (Page Down or Page Up)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1295"/>
        <source>Slice</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1306"/>
        <source>Slice through the data (&lt;b&gt;Up Arrow&lt;/b&gt; or &lt;b&gt;Down Arrow&lt;/b&gt;)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1322"/>
        <source>Transparency of data outside of slice</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1341"/>
        <source>Transparency value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1353"/>
        <source>Size of slice as percentage of area</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1371"/>
        <source>Slice size as a percentage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1386"/>
        <source>Slice size in meters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1438"/>
        <source>Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1444"/>
        <source>Open</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1447"/>
        <location filename="set_defaults.cpp" line="78"/>
        <source>Quit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1450"/>
        <source>&amp;File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1457"/>
        <source>&amp;Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1458"/>
        <source>Ctrl+P</source>
        <comment>Preferences shortcut key</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1459"/>
        <source>Change program preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1464"/>
        <source>&amp;Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1470"/>
        <source>Display Messages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1471"/>
        <source>Display error messages from prgram and sub-processes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1475"/>
        <source>View</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1481"/>
        <source>&amp;Hot Keys and GUI control</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1482"/>
        <source>Ctrl+H</source>
        <comment>Hot Keys shortcut key</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1483"/>
        <source>Help on GUI control and hot keys (fixed and user modifiable)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1486"/>
        <source>&amp;Tool bars</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1487"/>
        <source>Ctrl+T</source>
        <comment>Tool bars shortcut key</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1488"/>
        <source>Help on tool bars</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1494"/>
        <source>&amp;About</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1495"/>
        <source>Ctrl+A</source>
        <comment>About shortcut key</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1496"/>
        <source>Information about fileEdit3D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1499"/>
        <source>A&amp;cknowledgments</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1500"/>
        <source>Ctrl+c</source>
        <comment>Acknowledgmenst shortcut key</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1501"/>
        <source>Information about supporting libraries</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1504"/>
        <source>About Qt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1505"/>
        <source>Information about Qt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1508"/>
        <source>&amp;Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1531"/>
        <source>Active mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1542"/>
        <source>Progress bar and label</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1606"/>
        <source>fileEdit3D Open Input File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1621"/>
        <source>All files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1636"/>
        <source>Open input file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1636"/>
        <source>A file must be selected!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1664"/>
        <source>fileEdit3D: Error opening input file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1675"/>
        <location filename="fileEdit3D.cpp" line="1798"/>
        <source>fileEdit3D (ABE group %1) : %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1714"/>
        <location filename="fileEdit3D.cpp" line="4337"/>
        <source>Number of points: %L1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1758"/>
        <source>Connect to other ABE applications</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="1805"/>
        <source>Disconnect from other ABE applications</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2020"/>
        <source>Rectangular delete mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2028"/>
        <source>Rectangular restore mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2036"/>
        <source>Polygon delete mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2044"/>
        <source>Polygon restore mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2052"/>
        <source>Delete outside polygon mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2062"/>
        <source>Press a hot key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2066"/>
        <source>Polygon hot key mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2075"/>
        <source>Delete subrecord/record mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2083"/>
        <source>No Op mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2090"/>
        <source>Rotate data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2097"/>
        <source>Zoom in/out</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2104"/>
        <source>Rectangle filter mask mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2112"/>
        <source>Polygon filter mask mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2120"/>
        <source>Mask inside rectangle mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2127"/>
        <source>Mask outside rectangle mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2134"/>
        <source>Mask inside polygon mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2141"/>
        <source>Mask outside polygon mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2148"/>
        <source>Measuring mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2155"/>
        <source>Highlight points in polygon mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2163"/>
        <source>Clear highlights in polygon mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2578"/>
        <source>Delete point [%L1]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2580"/>
        <source>Delete record (e.g. ping)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2581"/>
        <source>Highlight point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2601"/>
        <source>Unfreeze marker</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2605"/>
        <source>Freeze marker</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2615"/>
        <location filename="fileEdit3D.cpp" line="2683"/>
        <location filename="fileEdit3D.cpp" line="2686"/>
        <source>Run %1 [%2]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2626"/>
        <source>Close rectangle and delete subrecords (beams)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2627"/>
        <source>Close rectangle and delete records (pings)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2628"/>
        <location filename="fileEdit3D.cpp" line="2636"/>
        <location filename="fileEdit3D.cpp" line="2705"/>
        <location filename="fileEdit3D.cpp" line="2731"/>
        <source>Discard rectangle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2635"/>
        <source>Close rectangle and restore data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2642"/>
        <location filename="fileEdit3D.cpp" line="2651"/>
        <source>Close polygon and delete subrecords (beams)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2643"/>
        <source>Close polygon and delete records (pings)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2644"/>
        <location filename="fileEdit3D.cpp" line="2652"/>
        <location filename="fileEdit3D.cpp" line="2659"/>
        <location filename="fileEdit3D.cpp" line="2673"/>
        <location filename="fileEdit3D.cpp" line="2710"/>
        <location filename="fileEdit3D.cpp" line="2739"/>
        <location filename="fileEdit3D.cpp" line="2753"/>
        <location filename="fileEdit3D.cpp" line="2760"/>
        <source>Discard polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2658"/>
        <source>Close polygon and restore data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2672"/>
        <source>Close polygon (press hot key to run program)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2704"/>
        <source>Close rectangle and save mask</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2709"/>
        <source>Close polygon and save mask</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2712"/>
        <source>Clear all masks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2715"/>
        <source>Set deep filter only (Yes)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2719"/>
        <source>Set deep filter only (No)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2730"/>
        <source>Close rectangle and mask data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2738"/>
        <source>Close polygon and mask data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2752"/>
        <source>Close polygon and highlight points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2759"/>
        <source>Close polygon and un-highlight points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2768"/>
        <source>Turn off slicing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2926"/>
        <location filename="fileEdit3D.cpp" line="2932"/>
        <source>Yes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2926"/>
        <source>No</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="2928"/>
        <source>Deep filter only:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="3431"/>
        <location filename="fileEdit3D.cpp" line="3432"/>
        <source>Scaled/offset current point Z value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="3778"/>
        <source>Distance : %L1 (m)  Azimuth : %L2 (degrees)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="3780"/>
        <location filename="fileEdit3D.cpp" line="3917"/>
        <source>Distance and azimuth from last highlighted point</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="3914"/>
        <source>Dist: %L1 (m)   Az: %L2 (degrees)  deltaZ: %L3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="4224"/>
        <source>&lt;p style=&apos;white-space:pre&apos;&gt;Press &lt;b&gt;Del&lt;/b&gt; to delete points or &lt;b&gt;Ins&lt;/b&gt; to clear points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="4945"/>
        <source>The key &lt;b&gt;&lt;i&gt;%1&lt;/i&gt;&lt;/b&gt; is only defined for hot key polygon mode. You may have been trying to run another program. The available programs are:&lt;br&gt;&lt;br&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="4949"/>
        <location filename="fileEdit3D.cpp" line="4985"/>
        <source>Delete Point Hot Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="4981"/>
        <source>The key &lt;b&gt;&lt;i&gt;%1&lt;/i&gt;&lt;/b&gt; either is not defined for ancillary program operations or the datatype of the nearest point does not match the approved data types for this key.  The available programs are:&lt;br&gt;&lt;br&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5015"/>
        <source>The key &lt;b&gt;&lt;i&gt;%1&lt;/i&gt;&lt;/b&gt; is not defined for hot key polygon operations. You may have been trying to use another program. The available programs are:&lt;br&gt;&lt;br&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5019"/>
        <source>Polygon Hot Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5086"/>
        <location filename="get_buffer.cpp" line="67"/>
        <source>Writing...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5247"/>
        <source>Run the area-based statistical filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5251"/>
        <source>Run the Hockey Puck filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5326"/>
        <source>Error and warning messages from sub-programs are displayed here.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5327"/>
        <source>fileEdit3D Messages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5341"/>
        <source>Clear</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5342"/>
        <source>Clear all messages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5346"/>
        <source>Save</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5347"/>
        <source>Save messages to text file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5353"/>
        <source>Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5354"/>
        <source>Close the message dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5426"/>
        <source>fileEdit3D Save error messages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5435"/>
        <location filename="fileEdit3D.cpp" line="5439"/>
        <source>Text (*.txt)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5684"/>
        <location filename="fileEdit3D.cpp" line="5698"/>
        <source>1 point selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3D.cpp" line="5688"/>
        <location filename="fileEdit3D.cpp" line="5702"/>
        <source>%L1 points selected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="40"/>
        <source>&lt;img source=&quot;:/icons/fileopen.png&quot;&gt; Click this button to open an input file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="43"/>
        <source>&lt;img source=&quot;:/icons/quit.png&quot;&gt; Click this button to exit from the editor.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="46"/>
        <source>&lt;img source=&quot;:/icons/reset_view.png&quot;&gt; Click this button to reset to the original scale.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="50"/>
        <source>&lt;img source=&quot;:/icons/displayinvalid.png&quot;&gt; Click this button to display previously invalidated data points.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="54"/>
        <source>&lt;img source=&quot;:/icons/highlight_polygon.png&quot;&gt; Click this button to allow you to select a polygonal area inside of which you wish to highlight data points.  When selected the cursor will become the highlight polygon cursor &lt;img source=&quot;:/icons/highlight_polygon_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the polygon start point.  Moving the cursor will draw a continuous line.  To close the polygon and highlight the data in the polygon simply left click again.  If, at any time during the operation, you wish to discard the polygon and abort the operation simply click the middle mouse button or right click and select a menu option.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: If you would like to delete highlighted data points just press &lt;i&gt;Return&lt;/i&gt; or whatever hot key that has been set for &lt;i&gt;DELETE FILTER HIGHLIGHTED&lt;/i&gt; in the Preferences dialog.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="64"/>
        <source>&lt;img source=&quot;:/icons/clear_polygon.png&quot;&gt; Click this button to allow you to select a polygonal area inside of which you wish to un-highlight data points.  When selected the cursor will become the clear polygon cursor &lt;img source=&quot;:/icons/clear_polygon_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the polygon start point.  Moving the cursor will draw a continuous line.  To close the polygon and un-highlight the data in the polygon simply left click again.  If, at any time during the operation, you wish to discard the polygon and abort the operation simply click the middle mouse button or right click and select a menu option.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: If you would like to delete highlighted data points just press &lt;i&gt;Return&lt;/i&gt; or whatever hot key that has been set for &lt;i&gt;DELETE FILTER HIGHLIGHTED&lt;/i&gt; in the Preferences dialog.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="74"/>
        <source>&lt;img source=&quot;:/icons/clear_highlight.png&quot;&gt; Click this button to clear all highlighted points from the display.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="78"/>
        <source>&lt;img source=&quot;:/icons/prefs.png&quot;&gt; Click this button to change program preferences.  This includes colors, minimum Z window size, position display format, and all hot keys.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="82"/>
        <source>This menu option will display a dialog containing any nonfatal error messages from this program and/or error messages from ancillary programs that were started by this program.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="86"/>
        <source>&lt;img source=&quot;:/icons/stop.png&quot;&gt; Click this button to cancel drawing of the data.  A much easier way to do this though is to click any mouse button in the display area or press any key on the keyboard.  The stop button is really just there so that the interface looks similar to the viewer and also to provide a place for help on how to stop the drawing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="93"/>
        <source>&lt;img source=&quot;:/icons/delete_point.png&quot;&gt; Click this button to select delete subrecord/record mode.  In this mode you can place the cursor on a subrecord (beam, shot, point) and delete the subrecord by pressing the &lt;Shift&gt; key and then left clicking (or via the right click menu) or delete the record (ping, shot) by pressing the middle mouse button.  Information about the current point will be displayed in the status bars at the bottom of the window.&lt;br&gt;&lt;br&gt;You can also highlight a point by using the right-click popup menu.  Note that the &lt;b&gt;Highlight point&lt;/b&gt; option may be disabled if you have filter kill points up or are flagging data using any of the flag buttons &lt;img source=&quot;:/icons/user_flag.png&quot;&gt;&lt;br&gt;&lt;br&gt;Hot keys and action keys are available when in this mode.  Please check the &lt;b&gt;Preferences&lt;b&gt; dialog &lt;img source=&quot;:/icons/prefs.png&quot;&gt;, specifically the &lt;b&gt;Ancillary Programs&lt;/b&gt; dialog, to see what action keys are available for each ancillary program.  To find out what the keys do in the ancillary program you must use the help in the ancillary program.&lt;p&gt;&lt;b&gt;&lt;i&gt;IMPORTANT NOTE: Only in this mode can you use hot keys to launch ancillary programs.&lt;/i&gt;&lt;/b&gt;&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="108"/>
        <source>&lt;img source=&quot;:/icons/delete_rect.png&quot;&gt; Click this button to allow you to select a rectangular area to invalidate.  When selected the cursor will become the invalidate rectangle cursor &lt;img source=&quot;:/icons/delete_rect_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the rectangle anchor point.  Moving the cursor will cause a rectangle to appear.  To finalize the rectangle and invalidate the data left click again.  If, at any time during the operation, you wish to discard the rectangle and abort the operation simply click the middle mouse button or right click and select one of the menu options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="116"/>
        <source>&lt;img source=&quot;:/icons/delete_poly.png&quot;&gt; Click this button to allow you to select a polygonal area to invalidate.  When selected the cursor will become the invalidate polygon cursor &lt;img source=&quot;:/icons/delete_poly_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the polygon start point.  Moving the cursor will draw a continuous line.  To close the polygon and invalidate the data in the polygon simply left click again.  If, at any time during the operation, you wish to discard the polygon and abort the operation simply click the middle mouse button or right click and select a menu option.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="124"/>
        <source>&lt;img source=&quot;:/icons/keep_poly.png&quot;&gt; Click this button to allow you to select a polygonal area and invalidate all data &lt;b&gt;outside&lt;/b&gt; of the polygon.  When selected the cursor will become the invalidate outside polygon cursor &lt;img source=&quot;:/icons/keep_poly_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the polygon start point.  To close the polygon and invalidate the data outside of the polygon simply left click again.  If, at any time during the operation, you wish to discard the polygon and abort the operation simply click the middle mouse button or right click and select a menu option.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="132"/>
        <source>&lt;img source=&quot;:/icons/restore_rect.png&quot;&gt; Click this button to allow you to select a rectangular area in which to restore invalidated data.  When selected the cursor will become the restore rectangle cursor &lt;img source=&quot;:/icons/restore_rect_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the rectangle anchor point.  Moving the cursor will cause a rectangle to appear.  To finalize the rectangle and restore the invalid data simply left click again.  If, at any time during the operation, you wish to discard the rectangle and abort the operation simply click the middle mouse button or right click and select one of the menu options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="140"/>
        <source>&lt;img source=&quot;:/icons/restore_poly.png&quot;&gt; Click this button to allow you to select a polygonal area in which to restore invalidated data.  When selected the cursor will become the restore polygon cursor &lt;img source=&quot;:/icons/restore_poly_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the polygon start point.  To close the polygon and restore invalid data in the polygon simply left click again.  If, at any time during the operation, you wish to discard the polygon and abort the operation simply click the middle mouse button or right click and select a menu option.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="148"/>
        <source>&lt;img source=&quot;:/icons/hotkey_poly.png&quot;&gt; Click this button to allow you to select a polygonal area in which to run an ancillary program that is started with a hotkey.  When selected the cursor will become the hotkey polygon cursor &lt;img source=&quot;:/icons/hotkey_poly_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the polygon start point.  Moving the cursor will draw a continuous line.  To close the polygon in preparation for pressing a hotkey simply left click again.  After defining the polygon just press the hotkey that is associated with the ancillary program that you wish to run.  To see the available ancillary programs look under preferences &lt;img source=&quot;:/icons/prefs.png&quot;&gt;.  If, at any time during the operation, you wish to discard the polygon and abort the operation simply click the middle mouse button or right click and select a menu option.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="161"/>
        <source>The fileEdit3D program is used to display and edit data points in data files.  The currently available file types are GSF, HOF, TOF, and CZMIL (.cpf).  The various editing and viewing options are initiated from the tool bar buttons.  Help is available for each button by clicking on the What&apos;s This button &lt;img source=&quot;:/icons/contextHelp.png&quot;&gt; then clicking on the item of interest.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="167"/>
        <source>This scroll bar can be used to move forward and backward through the file.  It is much easier to use the Page Down and Page Up keys instead of trying to click the scroll bar arrows.  Page Down moves forward in the file and Page Up moves back (towards the top of the file).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="172"/>
        <source>This is the 2D tracking map.  It will show you where your cursor is in relation to the 3D cursor position.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="175"/>
        <source>This scroll bar controls/monitors the vertical exaggeration of the data.  pressing the &lt;b&gt;Ctrl&lt;/b&gt; key and then simultaneously pressing the &lt;b&gt;PageUp&lt;/b&gt; or &lt;b&gt;PageDown&lt;/b&gt; button will change the exaggeration by 1.0.  Clicking the mouse while the cursor is in the trough will change the value by 5.0.  Dragging the slider will display the value in the &lt;b&gt;Exag:&lt;/b&gt; field in the status area on the left but the exaggeration will not change until the slider is released.  The maximum value is 100.0 and the minimum value is 1.0.  If the exaggeration has been set to less than 1.0 due to large vertical data range the scroll bar will be inactive.  The exaggeration can also be changed by pressing the &lt;b&gt;Ctrl&lt;/b&gt; key and then simultaneously pressing the &lt;b&gt;PageUp&lt;/b&gt; or &lt;b&gt;PageDown&lt;/b&gt; button.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="185"/>
        <source>This scroll bar controls/monitors the slice as it moves through the data.  Pressing the up and down arrow keys will move the slice through the data.  The slice size will be the percent of the viewable data that is set with the slice size scroll bar.  When you press the up arrow the view will move into the data one slice.  When displaying a slice, any edits will only effect data that is in full color (non-translucent).  Slicing is extremely handy in dealing with very bumpy surfaces to allow you to clean up hidden flyers.  You can turn off slicing by right clicking and selecting &lt;b&gt;Turn off slicing&lt;/b&gt;, by double clicking anywhere in the window, or by pressing the &lt;b&gt;Esc&lt;/b&gt; key.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="194"/>
        <source>Set the slice size using this scroll bar.  To understand how slicing works try to visualize the surface as it would be in plan view but with the bottom (leading) edge defined by the angle of view.  The slice size will be this percentage (default is 5 percent or 1/20th) of the displayed data.  When you press one of the up or down arrow keys or click one of the slice bar arrow buttons the slice will move by this amount.  The range for this scroll bar is 1 to 50.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="202"/>
        <source>Set the transparency value to be used for data that is not in the current slice.  If the slider is moved to the bottom, data outside the slice will be invisible.  If it is moved to the top the data will be almost completely opaque.  The range for this scroll bar is 0 (transparent) to 64 (semi-transparent).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="208"/>
        <source>&lt;center&gt;fileEdit3D&lt;br&gt;&lt;br&gt;Author : Jan C. Depner (area.based.editor@gmail.com)&lt;br&gt;Date : 31 January 2005&lt;br&gt;&lt;br&gt;&lt;/center&gt;fileEdit3D is a 3D along track editor for GSF, CHARTS HOF, CHARTS TOF, CZMIL CPF, and LAS files.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="215"/>
        <source>This area displays the currently active mode or information about the operation in progress.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="218"/>
        <location filename="fileEdit3DHelp.hpp" line="221"/>
        <source>The status bar is used to display the progress of the current operation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="224"/>
        <source>Click this button to access a pulldown menu allowing you to select coloring options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="227"/>
        <source>Click this button to access a pulldown menu allowing you to select data flagging options.  The flagging options are:&lt;br&gt;&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;img source=&quot;:/icons/suspect.png&quot;&gt; - mark suspect data&lt;/li&gt;&lt;li&gt;&lt;img source=&quot;:/icons/invalid.png&quot;&gt; - mark invalid data&lt;/li&gt;&lt;/ul&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="235"/>
        <source>&lt;img source=&quot;:/icons/lidar_monitor.png&quot;&gt; Click this button to run (or kill) the lidarMonitor program.  The lidarMonitor program will display HOF, TOF, or CZMIL record data for the current point nearest the cursor in a text format.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The hotkey for this button can be changed by changing the associated ancillary program hotkey in the Preferences &lt;img source=&quot;:/icons/preferences.png&quot;&gt; dialog.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="242"/>
        <source>&lt;img source=&quot;:/icons/charts_pic.png&quot;&gt; Click this button to run (or kill) the LIDAR chartsPic program.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The hotkey for this button can be changed by changing the associated ancillary program hotkey in the Preferences &lt;img source=&quot;:/icons/preferences.png&quot;&gt; dialog.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="247"/>
        <source>&lt;img source=&quot;:/icons/waveform_monitor.png&quot;&gt; Click this button to run (or kill) the LIDAR waveformMonitor program.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The hotkey for this button can be changed by changing the associated ancillary program hotkey in the Preferences &lt;img source=&quot;:/icons/preferences.png&quot;&gt; dialog.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="252"/>
        <source>&lt;img source=&quot;:/icons/wave_monitor.png&quot;&gt; Click this button to run (or kill) the LIDAR waveMonitor program.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The hotkey for this button can be changed by changing the associated ancillary program hotkey in the Preferences &lt;img source=&quot;:/icons/preferences.png&quot;&gt; dialog.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="257"/>
        <source>&lt;img source=&quot;:/icons/gsf_monitor.png&quot;&gt; Click this button to run (or kill) the gsfMonitor program.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The hotkey for this button can be changed by changing the associated ancillary program hotkey in the Preferences &lt;img source=&quot;:/icons/preferences.png&quot;&gt; dialog.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="262"/>
        <source>&lt;img source=&quot;:/icons/LASwaveMonitor.png&quot;&gt; Click this button to run (or kill) the LASwaveMonitor program.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The hotkey for this button can be changed by changing the associated ancillary program hotkey in the Preferences &lt;img source=&quot;:/icons/prefs.png&quot;&gt; dialog.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="267"/>
        <source>There are seven available tool bars in the fileEdit3D program.  They are&lt;br&gt;&lt;br&gt;&lt;ul&gt;&lt;li&gt;View tool bar - contains buttons to change the view&lt;/li&gt;&lt;li&gt;Utilities tool bar - contains buttons to modify the preferences and get context sensitive help&lt;/li&gt;&lt;li&gt;Edit tool bar - contains buttons to set editing modes&lt;/li&gt;&lt;li&gt;Mask tool bar - contains buttons to mask data inside or outside of rectangles or polygons&lt;/li&gt;&lt;/ul&gt;The tool bars may be turned on or off and relocated to any location on the screen.  You may click and drag the tool bars using the handle to the left of (or above) the tool bar.  Right clicking in the tool bar or in the menu bar will pop up a menu allowing you to hide or show the individual tool bars.  The location, orientation, and visibility of the tool bars will be saved on exit.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="280"/>
        <source>&lt;img source=&quot;:/icons/mask_inside_rect.png&quot;&gt; Click this button to allow you to select a rectangular area inside of which you want to hide the data.  When selected the cursor will become the mask cursor &lt;img source=&quot;:/icons/mask_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the rectangle anchor point.  Moving the cursor will cause a rectangle to appear.  To finalize the rectangle and mask the data, left click again.  If, at any time during the operation, you wish to discard the rectangle and abort the operation simply click the middle mouse button or right click and select one of the menu options.  To clear all masking press the mask reset button &lt;img source=&quot;:/icons/mask_reset.png&quot;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="289"/>
        <source>&lt;img source=&quot;:/icons/mask_outside_rect.png&quot;&gt; Click this button to allow you to select a rectangular area outside of which you want to hide the data.  When selected the cursor will become the mask cursor &lt;img source=&quot;:/icons/mask_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the rectangle anchor point.  Moving the cursor will cause a rectangle to appear.  To finalize the rectangle and mask the data, left click again.  If, at any time during the operation, you wish to discard the rectangle and abort the operation simply click the middle mouse button or right click and select one of the menu options.  To clear all masking press the mask reset button &lt;img source=&quot;:/icons/mask_reset.png&quot;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="298"/>
        <source>&lt;img source=&quot;:/icons/mask_inside_poly.png&quot;&gt; Click this button to allow you to select a polygonal area inside of which you want to hide the data.  When selected the cursor will become the mask cursor &lt;img source=&quot;:/icons/mask_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the polygon start point.  Moving the cursor will draw a continuous line.  To close the polygon and mask the data simply left click again.  If, at any time during the operation, you wish to discard the polygon and abort the operation simply click the middle mouse button or right click and select a menu option.  To clear all masking press the mask reset button &lt;img source=&quot;:/icons/mask_reset.png&quot;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="307"/>
        <source>&lt;img source=&quot;:/icons/mask_outside_poly.png&quot;&gt; Click this button to allow you to select a polygonal area outside of which you want to hide the data.  When selected the cursor will become the mask cursor &lt;img source=&quot;:/icons/mask_cursor.png&quot;&gt;.  Left clicking on a location will cause that point to be the polygon start point.  Moving the cursor will draw a continuous line.  To close the polygon and mask the data simply left click again.  If, at any time during the operation, you wish to discard the polygon and abort the operation simply click the middle mouse button or right click and select a menu option.  To clear all masking press the mask reset button &lt;img source=&quot;:/icons/mask_reset.png&quot;&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="316"/>
        <source>&lt;img source=&quot;:/icons/mask_reset.png&quot;&gt; Click this button to clear all data masking.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="318"/>
        <source>&lt;img source=&quot;:/icons/measure.png&quot;&gt; Click this button to measure distances and depth differences.  When this is clicked the cursor will become the measure cursor &lt;img source=&quot;:/icons/measure_cursor.png&quot;&gt;.  Simply click in a location and then move the cursor.  The distance, azimuth, and deltaZ will be displayed in the lower right corner status bar.  When you are finished click any mouse button to begin measuring again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="324"/>
        <source>&lt;img source=&quot;:/icons/filter.png&quot;&gt; Click this button to filter the data points.  The filter parameters can be modified in the &lt;b&gt;Preferences&lt;/b&gt; dialog &lt;img source=&quot;:/icons/preferences.png&quot;&gt;&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The filter operation will show you all of the points that are to be filtered and a confirmation dialog prior to actually invalidating the filtered points.  If you really know what you&apos;re doing and don&apos;t want to spend the time answering the dialog you can turn off filter confirmation in the preferences dialog.  This should not be a problem since you can always undo &lt;img source=&quot;:/icons/undo.png&quot;&gt; the filter operation but you&apos;ll have to pay close attention to what is getting deleted if there are features in the area.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="334"/>
        <source>&lt;img source=&quot;:/icons/filter_mask_rect.png&quot;&gt; Click this button to define temporary areas to protect (mask) from subsequent filter operations &lt;img source=&quot;:/icons/filter.png&quot;&gt;.&lt;br&gt;&lt;br&gt;After clicking the button the cursor will change to the rectangle mask cursor &lt;img source=&quot;:/icons/filter_mask_rect_cursor.png&quot;&gt;.  Click the left mouse button to define a starting point for the rectangle.  Moving the mouse will draw a rectangle.  Left click to end rectangle definition and mask the area.  To abort the operation click the middle mouse button.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The filter mask areas are only temporary.  If you change the area of the PFM that you are viewing these will be discarded.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="343"/>
        <source>&lt;img source=&quot;:/icons/filter_mask_poly.png&quot;&gt; Click this button to define temporary areas to protect (mask) from subsequent filter operations &lt;img source=&quot;:/icons/filter.png&quot;&gt;.&lt;br&gt;&lt;br&gt;After clicking the button the cursor will change to the polygon mask cursor &lt;img source=&quot;:/icons/filter_mask_poly_cursor.png&quot;&gt;.  Click the left mouse button to define a starting point for a polygon.  Moving the mouse will draw a line.  Left click to define the next vertex of the polygon.  Double click to define the last vertex of the polygon.  To abort the operation click the middle mouse button.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The filter mask areas are only temporary.  If you change the area of the PFM that you are viewing these will be discarded.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="353"/>
        <source>&lt;img source=&quot;:/icons/clear_filter_masks.png&quot;&gt; Click this button to clear all currently defined filter masks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="356"/>
        <source>&lt;img source=&quot;:/icons/set_filter_std.png&quot;&gt; Click this button to bring up a dialog to allow you to change the area-based statistical filter standard deviation value.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="360"/>
        <source>&lt;img source=&quot;:/icons/undo.png&quot;&gt; Click this button to undo the last edit operation.  When there are no edit operations to be undone this button will not be enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="364"/>
        <source>&lt;img source=&quot;:/icons/unlink.png&quot;&gt; Click this button to choose from other ABE or geographical GUI programs to connect to.  At present the only possible applications are pfmView, fileEdit, and areaCheck.  Hopefully, in the near future, we will be adding CNILE to the list.  The connection allows cursor tracking and limited data exchange between the applications.  If there are no available ABE groups to link to you can create a new group and add yourself to it so that other applications can link to this one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="372"/>
        <source>&lt;img source=&quot;:/icons/link.png&quot;&gt; Click this button to disconnect from another ABE or geographical GUI program.  This option is not available unless you have already linked to another program.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="377"/>
        <source>This tab page becomes active after running the area-based filter &lt;img source=&quot;:/icons/filter.png&quot;&gt;&lt;br&gt;&lt;br&gt;When this page is active you can adjust the results of the filter by moving the standard deviation slider up or down.  You may also change the view of the data to get a better perspective on the points that have been selected to be invalidated.  If there are areas of the selection that you do not want to invalidate you may use the filter mask buttons, &lt;img source=&quot;:/icons/filter_mask_rect.png&quot;&gt; and/or &lt;img source=&quot;:/icons/filter_mask_poly.png&quot;&gt;, to mask out those areas.  After you are completely satisfied with the final results of the filtering operation you may accept the results by pressing the &lt;b&gt;Accept&lt;/b&gt; button or, more simply, by just pressing the &lt;b&gt;Del&lt;/b&gt; key (or whatever hot key that has been set for &lt;i&gt;DELETE FILTER HIGHLIGHTED&lt;/i&gt; in the Preferences dialog).&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT TIP: Holding down the &lt;b&gt;Shift&lt;/b&gt; key and moving the mouse wheel will change the standard deviation value.  Approximately 1 to 1.5 seconds after you stop moving the mouse wheel or release the &lt;b&gt;Shift&lt;/b&gt; key the filter will be re-run and the new filter results will be displayed.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="391"/>
        <source>This tab page becomes active after running the Hockey Puck filter &lt;img source=&quot;:/icons/filter.png&quot;&gt;&lt;br&gt;&lt;br&gt;The Hockey Puck filter works by checking every point in the point cloud against nearby points to determine if there are &lt;b&gt;Neighbors&lt;/b&gt; points within the specified horizontal &lt;b&gt;Radius&lt;/b&gt; and vertical &lt;b&gt;Height&lt;/b&gt; (i.e. the hockey puck).  If there aren&apos;t enough points within the hockey puck then the point will be marked for deletion.&lt;br&gt;&lt;br&gt;When this page is active you can adjust the results of the filter by changing the value of the &lt;b&gt;Radius&lt;/b&gt;, &lt;b&gt;Height&lt;/b&gt;, and/or &lt;b&gt;Neighbors&lt;/b&gt; field and then pressing &lt;b&gt;Enter&lt;/b&gt;.  You may also change the view of the data to get a better perspective on the points that have been selected to be invalidated.  If there are areas of the selection that you do not want to invalidate you may use the filter mask buttons, &lt;img source=&quot;:/icons/filter_mask_rect.png&quot;&gt; and/or &lt;img source=&quot;:/icons/filter_mask_poly.png&quot;&gt;, to mask out those areas.  After you are completely satisfied with the final results of the filtering operation you may accept the results by pressing the &lt;b&gt;Accept&lt;/b&gt; button or, more simply, by just pressing the &lt;b&gt;Del&lt;/b&gt; key (or whatever hot key that has been set for &lt;i&gt;DELETE FILTER HIGHLIGHTED&lt;/i&gt; in the Preferences dialog).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="406"/>
        <source>Move this slider up to increase the amount of filtering (i.e. decrease the standard deviation value) or move it down to decrease the amount of filtering (i.e. increase the standard deviation value).  When the slider is released the filter is re-run on the visible data and the number of points to be rejected is modified.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT TIP: Holding down the &lt;b&gt;Shift&lt;/b&gt; key and moving the mouse wheel will change the standard deviation value.  Approximately 1 to 1.5 seconds after you stop moving the mouse wheel or release the &lt;Shift&gt; key the filter will be re-run and the new filter results will be displayed.&lt;br&gt;&lt;br&gt;You can also type the value you want into the text box and press &lt;i&gt;Enter&lt;/i&gt;.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="416"/>
        <source>Change the X/Y &lt;b&gt;Radius&lt;/b&gt; (in meters) to be used for the Hockey Puck filter.  Increasing this value will decrease the amount of filtering.  After changing the value and pressing &lt;b&gt;Enter&lt;/b&gt; the filter is re-run on the visible data and the number of points to be rejected is modified.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="421"/>
        <source>Change the vertical &lt;b&gt;Height&lt;/b&gt; (in meters) to be used for the Hockey Puck filter.  Increasing this value will decrease the amount of filtering.  After changing the value and pressing &lt;b&gt;Enter&lt;/b&gt; the filter is re-run on the visible data and the number of points to be rejected is modified.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="fileEdit3DHelp.hpp" line="426"/>
        <source>Change the number of &lt;b&gt;Neighbors&lt;/b&gt; required within the hroizontal &lt;b&gt;Radius&lt;/b&gt; and vertical &lt;b&gt;Height&lt;/b&gt; (i.e. the hockey puck) of the Hockey Puck filter.  Increasing this value will increase the amount of filtering.  After changing the value and pressing &lt;b&gt;Enter&lt;/b&gt; the filter is re-run on the visible data and the number of points to be rejected is modified.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="filter.cpp" line="154"/>
        <location filename="filter.cpp" line="165"/>
        <source>fileEdit3D filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="filter.cpp" line="154"/>
        <location filename="filter.cpp" line="165"/>
        <source>Unable to allocate memory for filtering!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="filter.cpp" line="458"/>
        <source>Filtering...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="get_buffer.cpp" line="98"/>
        <source>Reading...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="77"/>
        <source>Open file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="79"/>
        <source>Reset to original view</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="80"/>
        <source>Select delete subrecord/record edit mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="81"/>
        <source>Select delete rectangle edit mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="82"/>
        <source>Select delete polygon edit mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="83"/>
        <source>Run the filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="84"/>
        <source>Select rectangle filter mask mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="85"/>
        <source>Select polygon filter mask mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="86"/>
        <source>Select run hotkey program in polygon mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="88"/>
        <source>Clear all highlighted/marked points</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="89"/>
        <source>Select highlight/mark points in polygon mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="90"/>
        <source>Select clear all highlighted/marked points in polygon mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="171"/>
        <source>GSF non-graphical data viewer.  Displays data for nearest point.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="184"/>
        <source>GSF non-graphical data viewer.  Displays ping data for nearest point.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="197"/>
        <source>HOF waveform monitor (PMT, APD, IR, Raman).  Displays waveform for nearest point.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="215"/>
        <source>CZMIL, HOF, or TOF textual data viewer.  Displays entire record for nearest point.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="231"/>
        <source>HOF and TOF down-looking image viewer.  Displays nearest down-looking photo.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="247"/>
        <source>CZMIL waveform monitor.  Displays waveform(s) for nearest point(s).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="260"/>
        <source>LAS waveform monitor.  Displays waveform for nearest point.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="273"/>
        <source>Kill HOF returns based on fore/back slope and amplitude.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="286"/>
        <source>Kill HOF Shallow Water Algorithm or Shoreline Depth Swap data.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="303"/>
        <source>Delete single point nearest cursor in DELETE_POINT mode.  This is an internal function.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="317"/>
        <source>Delete points marked by the filter or highlighted.  This is an internal function.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="330"/>
        <source>Clear points marked by the filter/highlighted or restore highlighted invalid points.  This is an internal function.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="343"/>
        <source>Toggle freezing of the box cursor.  This is an internal function that affects ancillary programs.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="set_defaults.cpp" line="356"/>
        <source>Toggle freezing of all of the box cursors.  This is an internal function that affects ancillary programs.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="undo.cpp" line="86"/>
        <location filename="undo.cpp" line="97"/>
        <location filename="undo.cpp" line="138"/>
        <location filename="undo.cpp" line="149"/>
        <source>fileEdit3D store undo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="undo.cpp" line="87"/>
        <location filename="undo.cpp" line="139"/>
        <source>Unable to allocate UNDO validity memory!  Reason : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="undo.cpp" line="98"/>
        <location filename="undo.cpp" line="150"/>
        <source>Unable to allocate UNDO num memory!  Reason : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="undo.cpp" line="260"/>
        <source>fileEdit3D resize undo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="undo.cpp" line="261"/>
        <source>Resizing to less than already existing undo blocks is not allowed!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>hotkeyHelp</name>
    <message>
        <location filename="hotkeyHelp.cpp" line="48"/>
        <source>Hot Key Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="55"/>
        <source>&lt;br&gt;&lt;br&gt;The Graphical User Interface (GUI) can be controlled as follows:&lt;br&gt;&lt;br&gt;Point of view is controlled by holding down the &lt;b&gt;Ctrl&lt;/b&gt; key and clicking and dragging with the left mouse button.  It can also be controlled by holding down the &lt;b&gt;Ctrl&lt;/b&gt; key and using the left, right, up, and down arrow keys.  Zoom is controlled by holding down the &lt;b&gt;Ctrl&lt;/b&gt; key and using the mouse wheel or holding down the &lt;b&gt;Ctrl&lt;/b&gt; key, pressing the right mouse button, and moving the cursor up or down.  To reposition the center of the view just place the cursor at the desired location, hold down the &lt;b&gt;Ctrl&lt;/b&gt; key, and click the middle mouse button.  Z exaggeration can be changed by pressing &lt;b&gt;Ctrl-Page Up&lt;/b&gt; or &lt;b&gt;Ctrl-Page Down&lt;/b&gt; or by editing the exaggeration value in the Preferences dialog &lt;img source=&quot;:/icons/prefs.png&quot;&gt;&lt;br&gt;&lt;br&gt;&lt;br&gt;Slicing of the data is initiated by using the mouse wheel, dragging the slider in the scroll bar on the right of the display, by clicking the up or down arrow keys (without holding down the &lt;b&gt;Ctrl&lt;/b&gt; key), or pressing the up and down arrow buttons in the slice scroll bar.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="77"/>
        <source>Icon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="80"/>
        <source>Action</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="83"/>
        <location filename="hotkeyHelp.cpp" line="115"/>
        <source>Hot key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="109"/>
        <source>Program name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="112"/>
        <source>Description</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="118"/>
        <source>Action keys</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="121"/>
        <source>Polygon eligible</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="124"/>
        <source>Polygon only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="150"/>
        <location filename="hotkeyHelp.cpp" line="162"/>
        <source>Yes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="154"/>
        <location filename="hotkeyHelp.cpp" line="166"/>
        <source>No</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="175"/>
        <source>User view control (Non-modifiable)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="176"/>
        <source>Main Buttons (Preferences)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="177"/>
        <source>Ancillary Programs (Preferences)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyHelp.cpp" line="186"/>
        <source>Close</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>hotkeyPolygon</name>
    <message>
        <location filename="hotkeyPolygon.cpp" line="110"/>
        <source>Reading...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="222"/>
        <source>Writing...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="264"/>
        <source>Processing...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="299"/>
        <source>Unable to open shared data file %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="300"/>
        <source>Ancillary Program</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="310"/>
        <source>No points found that match ancillary program data type for:
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="311"/>
        <source>fileEdit3D Ancillary Program</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="343"/>
        <location filename="hotkeyPolygon.cpp" line="347"/>
        <location filename="hotkeyPolygon.cpp" line="351"/>
        <location filename="hotkeyPolygon.cpp" line="355"/>
        <location filename="hotkeyPolygon.cpp" line="359"/>
        <location filename="hotkeyPolygon.cpp" line="363"/>
        <source>fileEdit3D polygon program</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="343"/>
        <source>Unable to start the polygon program!
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="347"/>
        <source>The polygon program crashed!
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="351"/>
        <source>The polygon program timed out!
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="355"/>
        <source>There was a write error to the polygon program!
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="359"/>
        <source>There was a read error from the polygon program!
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="363"/>
        <source>The polygon program died with an unknown error!
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="hotkeyPolygon.cpp" line="376"/>
        <source>Retrieving...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>prefs</name>
    <message>
        <location filename="prefs.cpp" line="62"/>
        <source>fileEdit3D Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="68"/>
        <source>Hemisphere Degrees Minutes Seconds.decimal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="69"/>
        <source>Hemisphere Degrees Minutes.decimal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="70"/>
        <source>Hemisphere Degrees.decimal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="71"/>
        <source>+/-Degrees Minutes Seconds.decimal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="72"/>
        <source>+/-Degrees Minutes.decimal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="73"/>
        <source>+/-Degrees.decimal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="103"/>
        <source>Position Format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="104"/>
        <source>Set the position display format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="123"/>
        <source>Tracker</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="124"/>
        <source>Change tracker color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="131"/>
        <source>Background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="132"/>
        <source>Change display background color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="139"/>
        <location filename="prefs.cpp" line="197"/>
        <source>Scale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="140"/>
        <source>Change scale color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="153"/>
        <source>Cursor %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="155"/>
        <source>Change color for cursor %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="177"/>
        <source>Data outside slice set to gray</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="178"/>
        <source>Set all data outside of the slice area to gray, semi-transparent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="183"/>
        <source>Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="184"/>
        <source>Set the display colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="201"/>
        <source>Change the Z scale factor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="204"/>
        <source>%L1 (no scaling)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="205"/>
        <source>%L1 (meters to feet)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="206"/>
        <source>%L1 (feet to meters)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="207"/>
        <source>%L1 (meters to fathoms)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="208"/>
        <source>%L1 (fathoms to meters)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="213"/>
        <source>Offset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="220"/>
        <source>Change the contour/Z offset value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="224"/>
        <source>EGM08</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="225"/>
        <source>Get the ellipsoid to geoid datum offset from EGM08 model</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="232"/>
        <source>Zoom percentage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="238"/>
        <source>Change the zoom in/out percentage (10 - 50)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="244"/>
        <source>Z exaggeration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="251"/>
        <source>Change the Z exaggeration value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="256"/>
        <source>Point size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="262"/>
        <source>Change the point size (pixels) (2 - 10)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="268"/>
        <source>Contour/Depth</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="269"/>
        <source>Set the contour/depth options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="278"/>
        <source>Filter type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="282"/>
        <source>Select the type of filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="285"/>
        <source>Area-based statistical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="286"/>
        <source>Hockey Puck</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="290"/>
        <source>Area-based filter standard deviation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="297"/>
        <source>Change the area-based statistical filter standard deviation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="302"/>
        <source>Area-based filter deep filter only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="306"/>
        <source>Toggle filtering of deep values only (for area-based statistical filter)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="312"/>
        <source>Hockey Puck number of threads</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="316"/>
        <source>Change the number of threads used by the Hockey Puck filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="325"/>
        <source>Hockey Puck radius</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="332"/>
        <source>Change the Hockey Puck filter radius</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="337"/>
        <source>Hockey Puck height</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="344"/>
        <source>Change the Hockey Puck filter height</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="349"/>
        <source>Hockey Puck neighbors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="355"/>
        <source>Change the Hockey Puck filter required number of neighbors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="361"/>
        <source>Filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="362"/>
        <source>Set the filter options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="375"/>
        <source>Minimum Z window size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="380"/>
        <source>Use IHO order to define minimum Z window size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="394"/>
        <source>Manually set the minimum Z window size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="401"/>
        <source>Displayed point limit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="408"/>
        <source>Approximate number of points that you wish to have displayed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="415"/>
        <source>Rotation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="422"/>
        <source>Change the view rotation increment (1.0 - 30.0)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="428"/>
        <source>Draw scale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="432"/>
        <source>If checked, the scale will be drawn on the screen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="438"/>
        <source>Undo levels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="444"/>
        <source>Change the maximum number of undo levels (100 - 1,000,000)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="450"/>
        <source>Highlighted point tooltip</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="454"/>
        <source>If checked, tool tip will be displayed when highlighted points are present</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="460"/>
        <source>Miscellaneous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="461"/>
        <source>Set miscellaneous options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="470"/>
        <source>Main Buttons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="471"/>
        <source>Change hot key sequences for buttons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="477"/>
        <source>Main Button Icon Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="482"/>
        <source>Set the size (in pixels) of the main button icons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="496"/>
        <source>Ancillary Programs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="497"/>
        <source>Change ancillary program hot keys and action keys</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="503"/>
        <source>Hot Keys</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="504"/>
        <source>Set hot keys</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="524"/>
        <location filename="prefs.cpp" line="694"/>
        <location filename="prefs.cpp" line="858"/>
        <source>Enter What&apos;s This mode for help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="530"/>
        <source>Restore Defaults</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="531"/>
        <source>Restore all preferences to the default state</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="536"/>
        <source>OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="537"/>
        <source>Accept changes and close dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="542"/>
        <source>Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="543"/>
        <source>Discard changes and close dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="565"/>
        <source>fileEdit3D Tracker Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="579"/>
        <source>fileEdit3D Background Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="594"/>
        <source>fileEdit3D Scale Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="606"/>
        <source>Waveform/Marker %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="610"/>
        <source>fileEdit3D %1 Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="626"/>
        <source>fileEdit3D Ancillary Programs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="659"/>
        <source>Hot key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="667"/>
        <source>Action key(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="700"/>
        <location filename="prefs.cpp" line="864"/>
        <source>Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="701"/>
        <source>Close the ancillary program dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="816"/>
        <source>fileEdit3D Hot Keys</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="826"/>
        <source>Button</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="828"/>
        <source>Key</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="865"/>
        <source>Close the hot key dialog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="912"/>
        <source>Get EGM08 datum offset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="912"/>
        <source>Error retrieving EGM08 ellipsoid to geoid offset value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="1209"/>
        <location filename="prefs.cpp" line="1234"/>
        <location filename="prefs.cpp" line="1259"/>
        <source>fileEdit3D preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="1209"/>
        <source>Hot key for &lt;b&gt;%1&lt;/b&gt; button conflicts with hotkey for &lt;b&gt;%2&lt;/b&gt; button!&lt;br&gt;&lt;br&gt;Resetting button hotkeys for these two buttons.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="1234"/>
        <source>Hot key for &lt;b&gt;%1&lt;/b&gt; program conflicts with hotkey for &lt;b&gt;%2&lt;/b&gt; program!&lt;br&gt;&lt;br&gt;Resetting program hotkeys for these two programs.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefs.cpp" line="1259"/>
        <source>Hot key for &lt;b&gt;%1&lt;/b&gt; button conflicts with hotkey for &lt;b&gt;%2&lt;/b&gt; program!&lt;br&gt;&lt;br&gt;Resetting button and program hotkeys.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="40"/>
        <source>Select the format in which you want all geographic positions to be displayed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="43"/>
        <source>Set the contour interval to be used by pfmView and also fileEdit3D.  Setting this to 0.0 will cause the user defined contour levels to be used.  No contours will be plotted unless the plot contour button &lt;img source=&quot;:/icons/contour.png&quot;&gt; has been toggled on.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="49"/>
        <source>Set the contour smoothing factor by pfmView and also fileEdit3D.  This value goes from 0 (no smoothing) to 10 (max smoothing).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="53"/>
        <source>Set the scaling factor for contours &lt;b&gt;&lt;i&gt;and&lt;/i&gt;&lt;/b&gt; all displayed depth values in pfmView and fileEdit3D.  The actual values aren&apos;t changed just the displayed values.  This is useful for comparing depths to GeoTIFFs of scanned charts from NOAA (feet) or the US Navy (fathoms).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="59"/>
        <source>Set the offset value for contours &lt;b&gt;&lt;i&gt;and&lt;/i&gt;&lt;/b&gt; all displayed depth values in pfmView and fileEdit3D.  The actual values aren&apos;t changed just the displayed values.  This is useful for applying estimated ellipsoid offsets prior to the final datum shift being applied to the data.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The offset value is &lt;i&gt;added&lt;/i&gt; to the Z value and is applied &lt;i&gt;after&lt;/i&gt; scaling so the units are in scaled units.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="67"/>
        <source>Press this button to retrieve the EGM08 model value of the ellipsoid to geoid (approximate mean sea level) datum offset for the point nearest the center of the minimum bounding rectangle of the displayed area.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="72"/>
        <source>Set the contour line width/thickness in pixels.  Index contours (if set) will be twice this size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="75"/>
        <source>Set the size of each displayed point in pixels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="78"/>
        <source>Set the approximate number of points that you wish to display per section.  The minimum is 10,000 and the maximum is 1,000,000.  The default is 100,000.  The actual number of points loaded is dependent on the number of subrecords in each record.  For GSF this would be the number of beams per ping.  For CZMIL it will be the number of returns per channel (9 channels per shot).  Since the number of returns can vary between 0 and 31 we use an approximate average of 2 returns per channel (16 returns, not counting IR).  This means that the actual number of points displayed can get fairly large.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="85"/>
        <source>Check this box to draw the scale on the screen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="88"/>
        <source>Set the minimum Z extents.  This will be in the units that the data is in (almost always meters).  The purpose is to keep data with little Z variability from filling the screen with &lt;b&gt;&lt;i&gt;fuzz&lt;/i&gt;&lt;/b&gt; by limiting the self scaling of the Z axis to this minimum value.&lt;br&gt;&lt;br&gt;&lt;b&gt;Important note: If the combo box to the left of this is set to anything other than &lt;i&gt;Set Manually -&gt;&lt;/i&gt; then the max of this value and the computed IHO TVU will be used as the minimum Z window size.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="95"/>
        <source>Set the minimum Z extents manually or based on IHO order.  If this is set to anything other than &lt;b&gt;Set Manually -&gt;&lt;/b&gt; the minimum Z extents will be set based on the S44 IHO order TVU computations.  These are:&lt;br&gt;&lt;br&gt;&lt;ul&gt;&lt;li&gt;Special Order   -    0.25m + 0.0075 * depth&lt;/li&gt;&lt;li&gt;Order 1a/1b     -    0.50m + 0.0130 * depth&lt;/li&gt;&lt;li&gt;Order 2         -    1.00m + 0.0230 * depth&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;&lt;br&gt;The depth value used in the above equations will be the minimum depth displayed.&lt;br&gt;&lt;br&gt;&lt;b&gt;Important note: The maximum of the computed TVU and the manually set minimum Z window size will be used so that, in very shallow water, we don&apos;t shut the window down too far.  If you always want to use the computed TVU set the minimum value to 0.1 or some other very small number.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="109"/>
        <source>Set the size of the main window buttons.  The options are 16, 20, 24, 28, or 32 pixels.&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The icons size will change when you click the &lt;i&gt;Apply&lt;/i&gt; button but the locations will not be entirely correct until you have exited and restarted the program.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="114"/>
        <source>Set the view rotation increment in degrees.  This value is the amount the alternate views will rotate when the left or right arrow keys are pressed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="118"/>
        <source>Set the percentage value for zooming the data view in or out.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="121"/>
        <source>Set the Z exaggeration value (1.0 - 100.0).  This can be modified more easily by pressing &lt;b&gt;Ctrl-Page UP&lt;/b&gt; or &lt;b&gt;Ctrl-Page Down&lt;/b&gt; when viewing the main window.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="126"/>
        <source>Set the maximum number of undo levels.  This is the number of edit operations that can be &lt;b&gt;&lt;i&gt;undone&lt;/i&gt;&lt;/b&gt;. The value ranges from a reasonable 100 to a ridiculous 1,000,000.  The higher this value is set, the more memory you use to save the undo information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="132"/>
        <source>Check this box if you would like ancillary programs to exit when you close the fileEdit3D window and then respawn when you start it again.  This only works for the following LIDAR programs:&lt;br&gt;&lt;br&gt;&lt;ul&gt;&lt;li&gt;waveMonitor&lt;/li&gt;&lt;li&gt;chartsPic&lt;/li&gt;&lt;li&gt;lidarMonitor&lt;/li&gt;&lt;li&gt;CZMILwaveMonitor&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: The programs should respawn in the same locations, however, if there is overlap, the order of the windows may not be preserved.  Also, if you are running Compiz (desktop effects) on Linux using dual monitors, the program locations may not restore correctly.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="146"/>
        <source>Check this box if you would like to see the delete/clear highlighted points tool tip when highlighted or filter kill points are present on the screen.  If you already know the hot keys for these actions and the tool tip is annoying the hell out of you, uncheck this box.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="151"/>
        <source>Set the type of filter to be used.  The original area-based pseudo-statistical filter or the Hockey Puck filter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="154"/>
        <source>Set the standard deviation value for the filter.  This value can be anywhere from 0.3 to 4.0.  A value of 2.0 or below is an extreme filter and should be used only for deep water, flat area smoothing.  A value of 2.4 standard deviations should filter about 5 percent of the data while a value of 4.0 should only filter out about 0.5 percent.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="160"/>
        <source>Set this check box to indicate that data will only be filtered in the downward direction when running the filter.  Downward in this case implies a larger positive value.  The assumption is that the data we are working with consists of depths not elevations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="165"/>
        <source>This is the number of threads to be used to run the Hockey Puck filter.  Most modern systems have at least 4 cores available so the default of 4 is probably fine.  If you have a system with 16 or more cores you can go ahead and set this to 16.  The more threads you use the faster the filter runs.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="170"/>
        <source>Set the horizontal radius (in meters) that will be used to search for neighbors when using the Hockey Puck filter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="174"/>
        <source>Set the vertical height (in meters) that will be used to search for neighbors when using the Hockey Puck filter.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="178"/>
        <source>Set the number of neighbors that must be inside the radius and height (i.e. the hockey puck) defined for the Hockey Puck filter in order for a point to be deemed valid.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="182"/>
        <source>Set individual contour levels.  Pressing this button will set the standard contour interval to 0.0 and bring up a table dialog to allow the user to insert contour levels (one per line, ascending).  The default levels are IHO standard contour levels.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="188"/>
        <source>Use this table to set individual contour levels.  Levels should be set in ascending order.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="193"/>
        <source>Click this button to change the accelerator key sequences associated with some of the function buttons.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="197"/>
        <source>Select the data types that the command will work on.  If you want the command to work regardless of the data type of the current point (for instance, in the case of the mosaic file) choose Undefined.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="202"/>
        <source>Click this button to close the data types dialog.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="205"/>
        <source>Accelerator key sequences (hot keys) for some of the buttons in the main window may be changed in this dialog.  Up to four comma separated key values with modifiers may be used.  The format is illustrated by the following examples:&lt;br&gt;&lt;br&gt;&lt;ul&gt;&lt;li&gt;q&lt;/li&gt;&lt;li&gt;Shift+q&lt;/li&gt;&lt;li&gt;Ctrl+q&lt;/li&gt;&lt;li&gt;Meta+q&lt;/li&gt;&lt;li&gt;Ctrl+Alt+q&lt;/li&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="216"/>
        <source>Click this button to close the hot key dialog.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="220"/>
        <source>This button closes the contour level dialog.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="223"/>
        <source>Set color options in this tab.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="226"/>
        <source>Check this box to set data outside of the slice to gray, semi-transparent instead of colored (based on the color-by option). Transparency is still set by the slice transparency slider on the right side of the window.  Sometimes it is easier to see the data in the slice when using gray for the other data.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="231"/>
        <source>Set depth display options in this tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="234"/>
        <source>Set the hot keys in this tab.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="237"/>
        <source>Set the statistical filter options in this tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="240"/>
        <source>This tab contains settings for a number of miscellaneous options including the number of undo levels, the feature size, and the rotation increment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="244"/>
        <source>Click this button to change the color that is used to plot contours.  After selecting a color the map will be redrawn.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="248"/>
        <source>Click this button to change the color that is used to plot the &lt;i&gt;other ABE window&lt;/i&gt; tracker.  After selecting a color the map will be redrawn.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="252"/>
        <source>Click this button to change the color that is used as background.  After selecting a color the map will be redrawn.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="256"/>
        <source>Click this button to change the color that is used for the scale.  After selecting a color the map will be redrawn.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="260"/>
        <source>Click any of the numbered cursor buttons to change the color of the multiple cursors that are associated with the waveMonitor and CZMILwaveMonitor programs.  The &lt;b&gt;0&lt;/b&gt; color will be the color of the single cursor/marker.  The waveform colors in waveMonitor and CZMILwaveMonitor will use these colors as well.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="266"/>
        <source>Click this button to restore all of the preferences to the original default values.  This includes &lt;b&gt;&lt;i&gt;all&lt;/i&gt;&lt;/b&gt; of the system preferences, not just those in the preferences dialog.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="271"/>
        <source>Click this button to discard all preference changes and close the dialog.  The main map and coverage map will not be redrawn.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="275"/>
        <source>Click this button to apply all changes and close the dialog.  The main map and coverage map will be redrawn.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="279"/>
        <source>Click this button to change ancillary programs, hot keys, action keys, and data types associated with each program.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="283"/>
        <source>This is the external program that will be run when you press the associated hot key.  The hot keys and action keys may be changed but they must be unique.  For information on the programs look at &lt;b&gt;Hot Keys&lt;/b&gt; in the main program&apos;s &lt;b&gt;Help&lt;/b&gt; menu.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="288"/>
        <source>The hot key is the single key that will initiate the associated program.  These must be unique.  Keys with modifiers may be used.  The format is illustrated by the following examples:&lt;br&gt;&lt;br&gt;&lt;ul&gt;&lt;li&gt;q&lt;/li&gt;&lt;li&gt;Shift+q&lt;/li&gt;&lt;li&gt;Ctrl+q&lt;/li&gt;&lt;li&gt;Meta+q&lt;/li&gt;&lt;li&gt;Ctrl+Alt+q&lt;/li&gt;&lt;/ul&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="299"/>
        <source>The action key is a single key or a group of comma (,) separated single keys that will be passed to the associated program as command line arguments of the form --actionkeyNN=key.  Action keys are used via shared memory to cause some form of action to be taken in the ancillary program and fileEdit3D.  Programs that use action keys are specially built to access PFM shared memory.  The following are examples of action keys:&lt;br&gt;&lt;br&gt;&lt;ul&gt;&lt;li&gt;q&lt;/li&gt;&lt;li&gt;Shift+q&lt;/li&gt;&lt;li&gt;Ctrl+q&lt;/li&gt;&lt;li&gt;Meta+q&lt;/li&gt;&lt;li&gt;Ctrl+Alt+q&lt;/li&gt;&lt;li&gt;q,s,1,2,3,4&lt;/li&gt;&lt;li&gt;Ctrl+q,Alt+s&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;&lt;br&gt;&lt;b&gt;IMPORTANT NOTE: If you change the action keys for an ancillary program that is already running you must kill the ancillary program and restart it for the new action keys to take effect.&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="prefsHelp.hpp" line="317"/>
        <source>Click this button to close the ancillary programs dialog.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
