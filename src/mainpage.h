/**
 * @mainpage package plotting_widget
 *
 * @authors Bjoern Lueck
 *
 * @version 0.1
 *
 * @section intro Introduction
 * The plotting_widget can display plots by either getting data dircetly via coded
 * references or can import csv files with data. It displays the data as a plot trying to identify
 * the data and displaying it. The widget itself has multiple configuration options to change how
 * data is presented. Exporting data to csv or an image format is also possible. The data given can be
 * fixed, e.g. a huge list of x/y coordinates or one point can be added at a time.
 *
 * @section desc Description
 * The main class to work with is the class PlottingWidget. It can be used as a normal QWidget and all
 * methods to add data or other visual elements are provided there. The widget has its own menu and if
 * data shall be input by the user there is no need to do anything but add the widget in a container.
 * @subsection Data
 * Data is organized as a list of points with x and y coordinates. Note that currently only
 * the '.' is recognized as decimal point. Other locale specific decimal points are
 * not supported. If the data is exported it will
 * be exported as a simple csv file containing x,y values. If data shall be imported the same
 * format is assumed. an example goes like this:<p>
 * x1, y1<br>
 * y2,y2 <br>
 * ...,...<br>
 * xn,yn
 * @subsection Image Export
 * Instead of exporting the data as values, the plot can also be exported to an image. Available
 * image formats are:<br>
 * <ul>
 * <li>*.png</li>
 * <li>*.jpeg</li>
 * <li>*.bmp</li>
 * <li>*.svg</li>
 * <li>*.pdf</li>
 * </ul>
 * @subsection Profiles
 * When creating a configuration for a Test one can store the actual look of the plot to a XML File.
 * This will include everything <b>except</b> data. This includes colors, borderlines and the like. This is useful
 * for generating a profile for experiments which are repeated so the state of the plot without the data
 * can be restored easily. If the data shall also be stored one can use the usaul csv ex- and import methods. The
 * configuration.xml file in the source directory contains the schema for those profile files and can
 * be used to edit the profiles "by hand". It can also be used to extend the possibilities of the profiles.
 *
 * @todo further documentation
 * @todo some of the classes could be in an own module as they can be used in more contexts, like the export of widgets to images
 * 
 */

