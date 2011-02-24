/*
 * File:   PlotWidget.cc
 * Author: blueck
 *
 * Created on 17. August 2010, 12:24
 */

#include "PlotWidget.h"
#include "XMLConfigWriter.h"
#include <QtPlugin>
#include <qwt-qt4/qwt_picker.h>
#include <qwt-qt4/qwt_plot.h>
#include <QtCore/qdatetime.h>
#include <qwt-qt4/qwt_plot_curve.h>
#include <QtCore/qtimer.h>

//Q_EXPORT_PLUGIN2(PlotWidget,PlotWidget)

PlotWidget::PlotWidget(QWidget* parent) : QWidget(parent),
        xBottomSlider(NULL), yLeftSlider(NULL, Qt::Vertical), plottingWidget(this),
        zoomer(plottingWidget.canvas(), true), markers(100), curves(100), initialRect(-1, -1, -1, -1),
        optionsDialog(this),
        fileMenu(tr("&File")), plotMenu(tr("&Plot")), exportMenu(tr("&Export")), sliderMenu(tr("&Show Sliders")),
        gridMenu(tr("&Grid")), importMenu(tr("&Import")), clearMenu(tr("&Clear")), sizeMenu(tr("S&ize")),
        testMenu(tr("&Test")),
        exportImageAction(tr("Export as &image"), this), autoscrollAction(tr("&Autoscrolling"), this),
        fitAction(tr("&Fit to Graph"), this), optionsAction(tr("&Options..."), this),
        leftSliderAction(tr("&Left Slider"), this), exportCSVAction(tr("Export as &CSV"), this),
        bottomSliderAction(tr("&Bottom Slider"), this), xGridAction(tr("X"), this),
        yGridAction(tr("Y"), this), importCSVAction(tr("Import &CSV"), this), clearBorderLineAction(tr("Clear &Border Lines"), this),
        clearCurveAction(tr("Clear &Curves"), this), clearAllAction(tr("Clear &All"), this),
        loadProfileAction(tr("&Load Profile"), this), saveProfileAction(tr("&Save Profile"), this),
        autoscaleAction(tr("A&utoscaling"), this), fixedAction(tr("&Fixed"), this), testAction(tr("Start &Test"), this),
        sizeGroup(this)
{
    plotMarkerId = 0;
    curveId = 0;
    layout.addWidget(&menuBar, 0, 0, 1, 2);
    layout.addWidget(&yLeftSlider, 1, 0);
    layout.addWidget(&plottingWidget, 1, 1);
    layout.addWidget(&xBottomSlider, 2, 1);
    this->setLayout(&layout);
    dataManager = DataManager::getInstance();
    addMenu();
    this->enableSlider(X_BOTTOM, dataManager->isShowBottomSlider());
    this->enableSlider(Y_LEFT, dataManager->isShowLeftSlider());
    this->setDrawGrid(true, dataManager->isDrawXGrid(), dataManager->isDrawYGrid());
    dataManager->setBGColor(plottingWidget.canvasBackground());
    minXBottom = INT_MAX;
    maxXBottom = INT_MIN;
    minYLeft = INT_MAX;
    maxYLeft = INT_MIN;
    autoScale = true;
    isZoomed = false;
    autoscrolling = false;
    QObject::connect(&xBottomSlider, SIGNAL(valueChanged(double)), this, SLOT(xBottomSliderValueChanged(double)));
    QObject::connect(&yLeftSlider, SIGNAL(valueChanged(double)), this, SLOT(yLeftSliderValueChanged(double)));
    QObject::connect(&zoomer, SIGNAL(zoomed(const QwtDoubleRect&)), this, SLOT(zoomed(const QwtDoubleRect&)));
    QObject::connect(&plottingWidget, SIGNAL(mouseZoomed(const QwtDoubleRect&)), this, SLOT(zoomed(const QwtDoubleRect&)));
    QObject::connect(&optionsDialog, SIGNAL(accepted()), this, SLOT(optionsChanged()));
    zoomer.setTrackerMode(QwtPlotZoomer::AlwaysOn);
    legend.setItemMode(QwtLegend::CheckableItem);
    double lower = plottingWidget.axisScaleDiv(QwtPlot::xBottom)->lowerBound();
    double upper = plottingWidget.axisScaleDiv(QwtPlot::xBottom)->upperBound();
    dataManager->setMinX(lower);
    dataManager->setMaxX(upper);
//    this->minXBottom = lower;
//    this->maxXBottom = upper;
    xSpan = upper - lower;
    lower = plottingWidget.axisScaleDiv(QwtPlot::yLeft)->lowerBound();
    upper = plottingWidget.axisScaleDiv(QwtPlot::yLeft)->upperBound();
    dataManager->setMinY(lower);
    dataManager->setMaxY(upper);
    ySpan = upper - lower;
    this->minYLeft = lower;
    this->maxYLeft = upper;
    zoomer.setEnabled(true);
    connect(&plottingWidget, SIGNAL(legendChecked(QwtPlotItem *, bool)), this, SLOT(showCurve(QwtPlotItem *, bool)));
}

PlotWidget::~PlotWidget()
{
    for(unsigned int i=0;i<markers.size();i++)
    {
        delete(markers[i]);
    }
    markers.clear();
    for(unsigned int i=0;i<curves.size();i++)
    {
        delete(curves[i]);
    }
    curves.clear();
}

void PlotWidget::clearBorderLines()
{
    for(unsigned int i=0;i<markers.size();i++)
    {
        QwtPlotMarker* marker = markers[i];
        if(marker != NULL)
        {
            marker->detach();
            delete(marker);
        }
    }
    markers.clear();
    markers.resize(100);
    plotMarkerId = 0;
    plottingWidget.replot();
}

void PlotWidget::clearCurves()
{
    for(unsigned int i=0;i<curves.size();i++)
    {
        QwtPlotCurve* curve = curves[i];
        if(curve != NULL)
        {
            curve->detach();
            delete(curve);
        }
    }
    curves.clear();
    curves.resize(100);
    timeMap.clear();
    minXBottom = INT_MAX;
    minYLeft = INT_MAX;
    maxXBottom = INT_MIN;
    maxYLeft = INT_MIN;
    plottingWidget.replot();
}

void PlotWidget::clearAll()
{
//    std::cout << "Clear" << std::endl;
    clearBorderLines();
    clearCurves();
}

void PlotWidget::clearCurveData(int curveId)
{
    QwtPlotCurve* curve = curves[curveId];
    if(curve != NULL)
    {
        QVector<double> xVector;
        QVector<double> yVector;
        curve->setData(xVector, yVector);
    }
    plottingWidget.replot();
}

void PlotWidget::addMenu()
{
  menuBar.addMenu(&fileMenu);
  // File Menu
  fileMenu.addMenu(&clearMenu);
  fileMenu.addMenu(&exportMenu);
  fileMenu.addMenu(&importMenu);
  clearMenu.addAction(&clearBorderLineAction);
  clearMenu.addAction(&clearCurveAction);
  clearMenu.addAction(&clearAllAction);
  exportMenu.addAction(&exportImageAction);
  exportMenu.addAction(&exportCSVAction);
  importMenu.addAction(&importCSVAction);
  fileMenu.addAction(&loadProfileAction);
  fileMenu.addAction(&saveProfileAction);
  connect(&exportImageAction, SIGNAL(triggered()), this, SLOT(exportPlotAsImage()));
  connect(&importCSVAction, SIGNAL(triggered()), this, SLOT(importFromCSV()));
  connect(&clearBorderLineAction, SIGNAL(triggered()), this, SLOT(clearBorderLines()));
  connect(&clearCurveAction, SIGNAL(triggered()), this, SLOT(clearCurves()));
  connect(&clearAllAction, SIGNAL(triggered()), this, SLOT(clearAll()));
  connect(&exportCSVAction, SIGNAL(triggered()), this, SLOT(exportAsCSV()));
  connect(&loadProfileAction, SIGNAL(triggered()), this, SLOT(loadProfile()));
  connect(&saveProfileAction, SIGNAL(triggered()), this, SLOT(saveProfile()));
  // Plot Menu
  menuBar.addMenu(&plotMenu);
  autoscrollAction.setCheckable(true);
  plotMenu.addMenu(&sizeMenu);
  sizeMenu.addAction(&autoscrollAction);
  sizeMenu.addAction(&autoscaleAction);
  sizeMenu.addAction(&fixedAction);
  autoscrollAction.setCheckable(true);
  autoscaleAction.setCheckable(true);
  fixedAction.setCheckable(true);
  if(dataManager->isAutoscaling())
  {
      autoscaleAction.setChecked(true);
  }
  else if(dataManager->isAutoscrolling())
  {
      autoscrollAction.setChecked(true);
  }
  else if(dataManager->isFixedSize())
  {
      fixedAction.setChecked(true);
  }
  autoscrollAction.setActionGroup(&sizeGroup);
  autoscaleAction.setActionGroup(&sizeGroup);
  fixedAction.setActionGroup(&sizeGroup);
  plotMenu.addAction(&fitAction);
  plotMenu.addMenu(&gridMenu);
  plotMenu.addMenu(&sliderMenu);
  plotMenu.addAction(&optionsAction);
  leftSliderAction.setCheckable(true);
  bottomSliderAction.setCheckable(true);
  leftSliderAction.setChecked(dataManager->isShowLeftSlider());
  bottomSliderAction.setChecked(dataManager->isShowBottomSlider());
  sliderMenu.addAction(&leftSliderAction);
  sliderMenu.addAction(&bottomSliderAction);
  xGridAction.setCheckable(true);
  yGridAction.setCheckable(true);
  xGridAction.setChecked(dataManager->isDrawXGrid());
  yGridAction.setChecked(dataManager->isDrawYGrid());
  gridMenu.addAction(&xGridAction);
  gridMenu.addAction(&yGridAction);
  setAxisAutoScale(0, true);
  setAxisAutoScale(1, true);

  // Test Menu deactivate when not in test mode TODO
  testMenu.addAction(&testAction);
//  menuBar.addMenu(&testMenu);

  connect(&autoscrollAction, SIGNAL(triggered(bool)), this, SLOT(setAutoscrolling(bool)));
  connect(&autoscaleAction, SIGNAL(triggered(bool)), this, SLOT(setAutoscale(bool)));
  connect(&fixedAction, SIGNAL(triggered(bool)), this, SLOT(setFixedSize(bool)));
  connect(&fitAction, SIGNAL(triggered()), this, SLOT(fitPlotToGraph()));
  connect(&optionsAction, SIGNAL(triggered()), this, SLOT(showOptionsDialog()));
  connect(&leftSliderAction, SIGNAL(triggered()), this, SLOT(sliderActionChecked()));
  connect(&bottomSliderAction, SIGNAL(triggered()), this, SLOT(sliderActionChecked()));
  connect(&xGridAction, SIGNAL(triggered()), this, SLOT(gridChanged()));
  connect(&yGridAction, SIGNAL(triggered()), this, SLOT(gridChanged()));
  connect(&curveSelectionDialog, SIGNAL(accepted()), this, SLOT(curvesSelected()));
  connect(&testAction, SIGNAL(triggered()), this, SLOT(doTesting()));
}

void PlotWidget::testMethod()
{
    srand ( std::time(NULL) );
    double number = rand() % 6;
    if(number > 3)
    {
        number = number - 6;
    }
    addDataWithTime(number, 2);
}


void PlotWidget::doTesting()
{
    std::cout << "Start Test" << std::endl;
    testTimer.setInterval(1000);
    connect(&testTimer, SIGNAL(timeout()), this, SLOT(testMethod()));
    testTimer.start();
//    static bool doSecond = true;
//    QTime time;
//    time.start();
////    clearAll();
//    srand ( std::time(NULL) );
//    if(doSecond)
//    {
//        QList<double> xpoints;
//        QList<double> ypoints;
//        for(int i=0;i<100;i++)
//        {
//            double number = rand() % 1000;
//            double secondNumber = rand() % 500;
//            xpoints.append(number);
//            ypoints.append(secondNumber);
////            addData(number, secondNumber, 1);
////            std::cout << i << "took: " << time.elapsed() << std::endl;
//            time.restart();
//        }
//        addPoints(xpoints, ypoints, 1);
//        xpoints.clear();
//        ypoints.clear();
//        for(int i=0;i<100;i++)
//        {
//            double number = rand() % 500;
//            double secondNumber = rand() % 1000;
//            xpoints.append(number);
//            ypoints.append(secondNumber);
////            addData(number, secondNumber, 2);
////            std::cout << i << "took: " << time.elapsed() << std::endl;
//            time.restart();
//        }
//        addPoints(xpoints, ypoints, 2);
//        this->setDataStyle(2, QPen(QColor(255, 0, 0)), 1);
//        setLegendNameForCurve(QString("Test here"), 1);
//        fitPlotToGraph();
//        this->clearCurveData(2);
//        setEnableLegend(true);
//        doSecond = false;
//    }
//    else
//    {
//        QList<double> xpoints;
//        QList<double> ypoints;
////        clearAll();
////        for(int i=0;i<100;i++)
////        {
////            double number = rand() % 500;
////            double secondNumber = rand() % 500;
////            xpoints.append(number);
////            ypoints.append(secondNumber);
//////            addData(number, secondNumber, 1);
//////            std::cout << i << "took: " << time.elapsed() << std::endl;
////            time.restart();
////        }
////        addPoints(xpoints, ypoints, 1);
////        xpoints.clear();
////        ypoints.clear();
//        for(int i=0;i<100;i++)
//        {
//            double number = rand() % 100;
//            double secondNumber = rand() % 400;
//            xpoints.append(number);
//            ypoints.append(secondNumber);
////            addData(number, secondNumber, 2);
//            std::cout << i << "took: " << time.elapsed() << std::endl;
//            time.restart();
//        }
//        addPoints(xpoints, ypoints, 2);
//        this->setDataStyle(1, QPen(QColor(0, 0, 255)), 1);
//        fitPlotToGraph();
//        doSecond = true;
//    }
////    QList<double> xList;
////    QList<double> yList;
////    for(int i=0;i<102;i++)
////    {
////        xList.append(i);
////        yList.append(4.3);
////    }
////    time.restart();
////    addData(xList, yList, 87);
}

void PlotWidget::loadProfile()
{
    QString filter;
    QString filename = QFileDialog::getOpenFileName(this, tr("Open XML File"), QDir::homePath(), tr("XML document (*.xml)"), &filter);
    PlotXMLReader reader;
    reader.readXMLFile(filename.toStdString().c_str());
    std::vector<QwtPlotMarker*> newMarkers = reader.getMarker();
    for(unsigned int i=0;i<newMarkers.size();i++)
    {
        newMarkers[i]->attach(&plottingWidget);
        markers[plotMarkerId] = newMarkers[i];
        plotMarkerId++;
    }
    int minX = reader.getMinX();
    int maxX = reader.getMaxX();
    int minY = reader.getMinY();
    int maxY = reader.getMaxY();
    setAxisBoundaries(X_BOTTOM, minX, maxX);
    setAxisBoundaries(Y_LEFT, minY, maxY);
    refreshFromDataManager();
    plottingWidget.replot();
}

void PlotWidget::saveProfile()
{
    // Set up the possible graphics formats
    QString types( "XML file (*.xml)");
    // Image type filter
    QString filter;
    // Suffix for the files
    QString xmlExt=".xml";
    QString fn = QFileDialog::getSaveFileName(this, QString("Save Image"), QDir::homePath(), types, &filter);
    if(!fn.isEmpty())
    {
      // If filename is not a null
      // Remove file extension which are already there
      if(fn.contains(xmlExt))
      {
        fn.remove(xmlExt);
      }
      // save according to filter
      if(filter.contains(xmlExt))
      {
        fn += xmlExt;
      }
      else
      {
          std::cerr << "Have no File type which matches xml files." << std::endl;
          return;
      }
    }
    PlotXMLWriter writer;
    writer.setRanges(0, 1000, 0, 1000);
    writer.setMarker(markers);
    writer.writeConfigFile((char*)fn.toStdString().c_str());
}

void PlotWidget::setBackgroundColor(QColor color)
{
    dataManager->setBGColor(color);
    plottingWidget.setCanvasBackground(color);
    plottingWidget.replot();
}

QColor PlotWidget::getBackgroundColor()
{
    return dataManager->getBGColor();
}

void PlotWidget::setEnableLegend(bool enable)
{
    dataManager->setDrawLegend(enable);
    refreshFromDataManager();
}

bool PlotWidget::isEnableLegend()
{
    return dataManager->isDrawLegend();
}

QwtPlot::LegendPosition PlotWidget::getLegendPosition()
{
    return (QwtPlot::LegendPosition)dataManager->getLegendPosition();
}

void PlotWidget::setLegendPosition(QwtPlot::LegendPosition position)
{
    dataManager->setLegendPosition((int)position);
    refreshFromDataManager();
}

void PlotWidget::setLegendNameForCurve(const QString name, int curveId)
{
    QwtPlotCurve* curve = curves[curveId];
    if(curve != NULL)
    {
        curve->setTitle(name);
    }
}

void PlotWidget::refreshFromDataManager()
{
    plottingWidget.setCanvasBackground(dataManager->getBGColor());
    setAxisTitles(dataManager->getXAxisTitle(), dataManager->getYAxisTitle());
    if(dataManager->isDrawLegend())
    {
        QList<QWidget*> legendItems = legend.legendItems();
        for(int i=0;i<legendItems.size();i++)
        {
            legendItems[i]->setVisible(true);
        }
        legend.setVisible(true);
        plottingWidget.insertLegend(&legend, (QwtPlot::LegendPosition)dataManager->getLegendPosition());
    }
    else
    {
        QList<QWidget*> legendItems = legend.legendItems();
        for(int i=0;i<legendItems.size();i++)
        {
            legendItems[i]->setVisible(false);
        }
        legend.setVisible(false);
    }
    setAutoscrolling(dataManager->isAutoscrolling());
    setAutoscale(dataManager->isAutoscaling());
    setFixedSize(dataManager->isFixedSize());
    setDrawGrid(true, dataManager->isDrawXGrid(), dataManager->isDrawYGrid());
    enableSlider(X_BOTTOM, dataManager->isShowBottomSlider());
    enableSlider(Y_LEFT, dataManager->isShowLeftSlider());
}

void PlotWidget::importFromCSV()
{
    QString filter;
    QString filename = QFileDialog::getOpenFileName(this, tr("Open CSV File"), QDir::homePath(), tr("Comma seperated values (*.csv)"), &filter);
    std::vector< std::vector<double> > points = CSVImporter::getDoubleArrayFromFile(filename.toStdString(), dataManager->getCSVDelimter());
    QList<double> xPoints;
    QList<double> yPoints;
    for(unsigned int i=0;i<points.size();i++)
    {
        std::vector<double> linePoints = points[i];
        if(linePoints.size() > 1)
        {
            xPoints.append(linePoints[0]);
            yPoints.append(linePoints[1]);
        }
    }
    //only if we have something, else do nothing
    if(xPoints.size() > 0)
    {
        this->addPoints(xPoints, yPoints);
    }
}

void PlotWidget::gridChanged()
{
    bool xGrid = xGridAction.isChecked();
    bool yGrid = yGridAction.isChecked();
    this->setDrawGrid(true, xGrid, yGrid);
    dataManager->setDrawXGrid(xGrid);
    dataManager->setDrawYGrid(yGrid);
}


void PlotWidget::sliderActionChecked()
{
    bool left = leftSliderAction.isChecked();
    bool bottom = bottomSliderAction.isChecked();
    enableSlider(Y_LEFT, left);
    enableSlider(X_BOTTOM, bottom);
    dataManager->setShowBottomSlider(bottom);
    dataManager->setShowLeftSlider(left);
}

void PlotWidget::exportAsCSV()
{
    curveSelectionDialog.initializeLayout(curves);
    curveSelectionDialog.show();
}

void PlotWidget::curvesSelected()
{
    std::vector<QwtPlotCurve*> selectedCurves = curveSelectionDialog.getSelectedCurves();
    if(selectedCurves.size() > 0)
    {
        CSVExporter::exportCurveAsCSV(selectedCurves, dataManager->getCSVDelimter());
    }
}


void PlotWidget::fitPlotToGraph()
{
//    std::cout << "Fit" << std::endl;
  setAxisAutoScale(X_BOTTOM, true);
  setAxisAutoScale(Y_LEFT, true);
  double lower = plottingWidget.axisScaleDiv(QwtPlot::xBottom)->lowerBound();
  double upper = plottingWidget.axisScaleDiv(QwtPlot::xBottom)->upperBound();
//  setAxisBoundaries(X_BOTTOM, lower, upper);
//  std::cout << minXBottom << "|" << maxXBottom << std::endl;
  setAxisBoundaries(X_BOTTOM, minXBottom, maxXBottom);
  lower = plottingWidget.axisScaleDiv(QwtPlot::yLeft)->lowerBound();
  upper = plottingWidget.axisScaleDiv(QwtPlot::yLeft)->upperBound();
  setAxisBoundaries(Y_LEFT, minYLeft, maxYLeft);
  yLeftSlider.setValue(yLeftSlider.minValue());
  xBottomSlider.setValue(xBottomSlider.minValue());
  plottingWidget.replot();
}

void PlotWidget::showOptionsDialog()
{
  optionsDialog.initializeLayout(curves, markers);
  optionsDialog.show();
}



void PlotWidget::optionsChanged()
{
    plottingWidget.setCanvasBackground(dataManager->getBGColor());
    setAxisTitles(dataManager->getXAxisTitle(), dataManager->getYAxisTitle());
    std::vector<QwtPlotMarker*> toDelete = optionsDialog.getDeletedMarkers();
    for(unsigned int i=0;i<toDelete.size();i++)
    {
        QwtPlotMarker* deleteMarker = toDelete[i];
        for(unsigned int j=0;j<markers.size();j++)
        {
            if(deleteMarker == markers[j])
            {
                markers[j]->detach();
                delete(markers[i]);
                markers[i] = NULL;
            }
        }
    }
    std::vector<QwtPlotMarker*> newMarkers = optionsDialog.getNewMarkers();
    for(unsigned int i=0;i<newMarkers.size();i++)
    {
        QwtPlotMarker* marker = newMarkers[i];
        if(marker != NULL)
        {
            marker->attach(&plottingWidget);
            markers[plotMarkerId] = marker;
            plotMarkerId++;
        }
    }
    if(dataManager->isDrawLegend())
    {
        QList<QWidget*> legendItems = legend.legendItems();
        for(int i=0;i<legendItems.size();i++)
        {
            legendItems[i]->setVisible(true);
        }
        legend.setVisible(true);
        plottingWidget.insertLegend(&legend, (QwtPlot::LegendPosition)dataManager->getLegendPosition());
    }
    else
    {
        QList<QWidget*> legendItems = legend.legendItems();
        for(int i=0;i<legendItems.size();i++)
        {
            legendItems[i]->setVisible(false);
        }
        legend.setVisible(false);
    }
    // put into refresh?
    if(dataManager->isAutoscrolling() || dataManager->isFixedSize())
    {
        setAxisBoundaries(X_BOTTOM, dataManager->getMinX(), dataManager->getMaxX());
        setAxisBoundaries(Y_LEFT, dataManager->getMinY(), dataManager->getMaxY());
    }
    refreshFromDataManager();
    plottingWidget.replot();
}

QwtPlot::Axis PlotWidget::getAxisForInt(int axis)
{
  switch(axis)
    {
    case X_BOTTOM:
      return QwtPlot::xBottom;
    case Y_LEFT:
      return QwtPlot::yLeft;
    case X_TOP:
    {
        return QwtPlot::xTop;
    }
    case Y_RIGHT:
      return QwtPlot::yRight;
    }
  return QwtPlot::xBottom;
}

void PlotWidget::setDrawGrid(bool drawGrid, bool enableX, bool enableY)
{
    if(drawGrid)
    {
        grid.enableX(enableX);
        grid.enableY(enableY);
        xGridAction.setChecked(enableX);
        yGridAction.setChecked(enableY);
        grid.attach(&plottingWidget);
    }
    else
    {
        xGridAction.setChecked(false);
        yGridAction.setChecked(false);
        grid.detach();
    }
    plottingWidget.replot();
}

void PlotWidget::setDrawXGrid(bool drawXGrid)
{
    setDrawGrid(true, drawXGrid, dataManager->isDrawYGrid());
    dataManager->setDrawXGrid(drawXGrid);
}

void PlotWidget::setDrawYGrid(bool drawYGrid)
{
    setDrawGrid(true, dataManager->isDrawYGrid(), drawYGrid);
    dataManager->setDrawYGrid(drawYGrid);
}

bool PlotWidget::isDrawXGrid()
{
    return dataManager->isDrawXGrid();
}

bool PlotWidget::isDrawYGrid()
{
    return dataManager->isDrawYGrid();
}

int PlotWidget::addBorderLine(double value, Qt::Orientation orientation, QPen pen)
{
    markers.reserve(plotMarkerId + 1);
    QwtPlotMarker* marker = new QwtPlotMarker();
    if(orientation == Qt::Horizontal)
    {
        marker->setLineStyle(QwtPlotMarker::HLine);
        marker->setYValue(value);
    }
    else
    {
        marker->setLineStyle(QwtPlotMarker::VLine);
        marker->setXValue(value);
    }
    marker->setLinePen(pen);
    marker->attach(&plottingWidget);
    markers[plotMarkerId] = marker;
    return plotMarkerId++;
}

void PlotWidget::enableSlider(int axisId, bool enable)
{
    QwtPlot::Axis axis = getAxisForInt(axisId);
    if(axis == QwtPlot::xBottom)
    {
        xBottomSlider.setVisible(enable);
        bottomSliderAction.setChecked(enable);
    }
    else
    {
        yLeftSlider.setVisible(enable);
        leftSliderAction.setChecked(enable);
    }
}

void PlotWidget::setEnableXSlider(bool enable)
{
    enableSlider(X_BOTTOM, enable);
    dataManager->setShowBottomSlider(enable);
}

void PlotWidget::setEnableYSlider(bool enable)
{
    enableSlider(Y_LEFT, enable);
    dataManager->setShowLeftSlider(enable);
}

bool PlotWidget::isEnableXSlider()
{
    return dataManager->isShowBottomSlider();
}

bool PlotWidget::isEnableYSlider()
{
    return dataManager->isShowLeftSlider();
}

void PlotWidget::zoomed(const QwtDoubleRect& rect)
{
    if(rect == zoomer.zoomBase())
    {
        isZoomed = false;
        // set the slider positions to its original values
        xBottomSlider.blockSignals(true);
        yLeftSlider.blockSignals(true);
        xBottomSlider.setValue(rect.bottomLeft().x());
        yLeftSlider.setValue(rect.topLeft().y());
        xBottomSlider.blockSignals(false);
        yLeftSlider.blockSignals(false);
        xBottomSlider.setEnabled(true);
        yLeftSlider.setEnabled(true);
        // if the initial rect is not the same as the zoombase
        // zoom back to the original width.
        // if we dont do this one cant zoom the rect in parts
        // of the plot which are not visible at the beginning, e.g.
        // out of the zoombase of the zoomer
        if(initialRect.width() != -1)
        {
            zoomer.zoom(initialRect);
        }
        setSliderValues();
    }
    else
    {
        isZoomed = true;
        zoomXSpan = rect.bottomRight().x() - rect.bottomLeft().x();
        zoomYSpan = rect.bottomLeft().y() - rect.topLeft().y();
        xBottomSlider.blockSignals(true);
        yLeftSlider.blockSignals(true);
        xBottomSlider.setValue(rect.bottomLeft().x());
        yLeftSlider.setValue(rect.topLeft().y());
        xBottomSlider.blockSignals(false);
        yLeftSlider.blockSignals(false);
    }
}

void PlotWidget::enableBorderLine(int borderLineId, bool enable)
{
    QwtPlotMarker* marker = markers[borderLineId];
    if(marker != NULL)
    {
        if(enable)
        {
            marker->attach(&plottingWidget);
        }
        else
        {
            marker->detach();
        }
    }
}


void PlotWidget::setBorderLineStyle(int borderLineId, QPen pen)
{
    QwtPlotMarker* marker = markers[borderLineId];
    if(marker != NULL)
    {
        marker->setLinePen(pen);
    }
}

void PlotWidget::setAxisAutoScale(int axisId, bool enable)
{
    QwtPlot::Axis axis = getAxisForInt(axisId);
    if(enable)
    {
        plottingWidget.setAxisAutoScale(axis);
        initialRect.setRect(-1, -1, -1, -1);
    }
    else
    {
        double lower = plottingWidget.axisScaleDiv(axis)->lowerBound();
        double upper = plottingWidget.axisScaleDiv(axis)->upperBound();
        setAxisBoundaries(axis, lower, upper);
    }
}

void PlotWidget::setAxisTitles(QString xAxisTitle, QString yAxisTitle)
{
    plottingWidget.setAxisTitle(QwtPlot::yLeft, yAxisTitle);
    plottingWidget.setAxisTitle(QwtPlot::xBottom, xAxisTitle);
    dataManager->setXAxisTitle(xAxisTitle);
    dataManager->setYAxisTitle(yAxisTitle);
}

void PlotWidget::setXAxisTitle(QString title)
{
    setAxisTitles(title, dataManager->getYAxisTitle());
}

void PlotWidget::setYAxisTitle(QString title)
{
    setAxisTitles(dataManager->getXAxisTitle(), title);
}

QString PlotWidget::getXAxisTitle()
{
    return dataManager->getXAxisTitle();
}

QString PlotWidget::getYAxisTitle()
{
    return dataManager->getYAxisTitle();
}

void PlotWidget::setAxisBoundaries(int axisId, double lower, double upper, double step)
{
    QwtPlot::Axis axis = getAxisForInt(axisId);
    plottingWidget.enableAxis(axis);
    plottingWidget.setAxisScale(axis, lower, upper, step);
    plottingWidget.replot();
    if(axis == QwtPlot::xBottom)
    {
        xSpan = upper - lower;
        dataManager->setMinX(lower);
        dataManager->setMaxX(upper);
        this->maxXBottom = upper;
    }
    else if(axis == QwtPlot::yLeft)
    {
        ySpan = upper -lower;
        dataManager->setMinY(lower);
        dataManager->setMaxY(upper);
        this->maxYLeft = upper;
    }
    setZoomBase();
//    autoScale = false;
}

void PlotWidget::setFastAxisBoundaries(int axisId, double lower, double upper, double step)
{
    QwtPlot::Axis axis = getAxisForInt(axisId);
    plottingWidget.setAxisScale(axis, lower, upper, step);
    if(axis == QwtPlot::xBottom)
    {
        xSpan = upper - lower;
        dataManager->setMinX(lower);
        dataManager->setMaxX(upper);
        this->maxXBottom = upper;
    }
    else if(axis == QwtPlot::yLeft)
    {
        ySpan = upper -lower;
        dataManager->setMinY(lower);
        dataManager->setMaxY(upper);
        this->maxYLeft = upper;
    }
}

void PlotWidget::setSliderValues()
{
    xBottomSlider.setScalePosition(QwtSlider::BottomScale);
    double currentMinX = plottingWidget.axisScaleDiv(QwtPlot::xBottom)->lowerBound();
    double currentMaxX = plottingWidget.axisScaleDiv(QwtPlot::xBottom)->upperBound();
    double currentMinY = plottingWidget.axisScaleDiv(QwtPlot::yLeft)->lowerBound();
    double currentMaxY = plottingWidget.axisScaleDiv(QwtPlot::yLeft)->upperBound();
    xBottomSlider.setRange(minXBottom < currentMinX ? minXBottom : currentMinX, maxXBottom > currentMaxX ? maxXBottom : currentMaxX);
    yLeftSlider.setScalePosition(QwtSlider::LeftScale);
    yLeftSlider.setRange(minYLeft < currentMinY ? minYLeft : currentMinY, maxYLeft > currentMaxY ? maxYLeft : currentMaxY);
}

void PlotWidget::setZoomBase()
{
    double xLower = plottingWidget.axisScaleDiv(QwtPlot::xBottom)->lowerBound();
    double width = plottingWidget.axisScaleDiv(QwtPlot::xBottom)->upperBound() - plottingWidget.axisScaleDiv(QwtPlot::xBottom)->lowerBound();
    double height = plottingWidget.axisScaleDiv(QwtPlot::yLeft)->upperBound() - plottingWidget.axisScaleDiv(QwtPlot::yLeft)->lowerBound();
    double yLower = plottingWidget.axisScaleDiv(QwtPlot::yLeft)->lowerBound();
    initialRect.setRect(xLower, yLower, width, height);
    zoomer.setZoomBase(QwtDoubleRect(minXBottom, maxYLeft, maxXBottom + zoomXSpan, maxYLeft + zoomYSpan));
}

void PlotWidget::setNumberOfDecimals(int decimals)
{
    dataManager->setZoomerDecimalSize(decimals);
}

int PlotWidget::getNumberOfDecimals()
{
    return dataManager->getZoomerDecimalSize();
}

void PlotWidget::xBottomSliderValueChanged(double newValue)
{
    double value = xBottomSlider.value();
    if(!isZoomed)
    {
        plottingWidget.setAxisScale(QwtPlot::xBottom, value, value + xSpan);
    }
    else
    {
        plottingWidget.setAxisScale(QwtPlot::xBottom, value, value + zoomXSpan);
    }
    plottingWidget.replot();
}

void PlotWidget::yLeftSliderValueChanged(double newValue)
{
    double value = yLeftSlider.value();
    if(!isZoomed)
    {
        plottingWidget.setAxisScale(QwtPlot::yLeft, value, value + ySpan);
    }
    else
    {
        plottingWidget.setAxisScale(QwtPlot::yLeft, value, value + zoomYSpan);
    }
    plottingWidget.replot();
}

void PlotWidget::setAxisShown(int axisId, bool enable)
{
    QwtPlot::Axis axis = getAxisForInt(axisId);
    plottingWidget.enableAxis(axis, enable);
}

void PlotWidget::setAutoscrolling(bool enable)
{
    this->autoscrolling = enable;
    autoscrollAction.setChecked(enable);
    xBottomSlider.setEnabled(!enable);
    yLeftSlider.setEnabled(!enable);
    zoomer.setEnabled(!enable);
    plottingWidget.setMouseWheelZoomAxis(!enable, !enable);
    autoScale = !enable;
    if(enable)
    {
        dataManager->setAutoscaling(false);
        dataManager->setAutoscrolling(true);
        dataManager->setIsFixedSize(false);
        autoscrollAction.setChecked(true);
    }
}

void PlotWidget::setAutoscale(bool autoscale)
{
    dataManager->setAutoscaling(autoscale);
    if(autoscale)
    {
        zoomer.setEnabled(true);
        plottingWidget.setMouseWheelZoomAxis(true, true);
        autoscaleAction.setChecked(true);
        dataManager->setIsFixedSize(false);
        dataManager->setAutoscrolling(false);
        xBottomSlider.setEnabled(true);
        yLeftSlider.setEnabled(true);
    }
}

bool PlotWidget::isAutoscale()
{
    return dataManager->isAutoscaling();
}

void PlotWidget::setFixedSize(bool fixedSize)
{
    dataManager->setIsFixedSize(fixedSize);
    if(fixedSize)
    {
        zoomer.setEnabled(true);
        plottingWidget.setMouseWheelZoomAxis(true, true);
        dataManager->setAutoscaling(false);
        dataManager->setAutoscrolling(false);
        fixedAction.setChecked(true);
        xBottomSlider.setEnabled(true);
        yLeftSlider.setEnabled(true);
    }
}

bool PlotWidget::isFixedSize()
{
    return dataManager->isFixedSize();
}

int PlotWidget::addPoints(const QList<double> &xPoints,const  QList<double> &yPoints, int dataId,
        int xAxisId, int yAxisId) throw(std::exception)
{
  return addData(xPoints.toVector().data(), yPoints.toVector().data(), xPoints.size(), dataId, xAxisId, yAxisId);
}

int PlotWidget::addData(const double xPoint,const  double yPoint, int dataId,
        int xAxisId, int yAxisId) throw(std::exception)
{
  double xPoints[1];
  double yPoints[1];
  xPoints[0] = xPoint;
  yPoints[0] = yPoint;
  return addData(xPoints, yPoints, 1, dataId, xAxisId, yAxisId);
}

int PlotWidget::addDataWithTime(double yPoint, int dataId,
		int xAxisId, int yAxisId) throw(std::exception)
{
    double yPoints[1];
    double xPoints[1];
    yPoints[0] = yPoint;
    if(dataId > 100)
    {
        throw (IdTooLargeException());
    }
    if(dataId < 0 || curves[dataId] == NULL)
    {
        // new object set x to zero and store the time
        QTime* time = new QTime();
        time->start();
        xPoints[0] = 0;
        timeMap[dataId] = time;
    }
    else
    {
        QTime* time = timeMap[dataId];
        if(dataManager->getTimeDivisor() == DataManager::SECONDS)
        {
            xPoints[0] = ((double)time->elapsed())/1000.0f;
        }
        else if(dataManager->getTimeDivisor() == DataManager::MINUTES)
        {
            xPoints[0] = ((double)time->elapsed())/60000.0f;
        }
        else
        {
            xPoints[0] = time->elapsed();
        }

    }
    std::cout << yPoint << std::endl;
    return addData(xPoints, yPoints, 1, dataId, xAxisId, yAxisId);
}

int PlotWidget::addData(const double* xPoints,const double* yPoints, int length, int dataId,
        int xAxisId, int yAxisId) throw(std::exception)
{
    bool needRepaint = false;
    QwtPlot::Axis xAxis = getAxisForInt(xAxisId);
    QwtPlot::Axis yAxis = getAxisForInt(yAxisId);
    // new data
    if(dataId > 100)
    {
        throw (IdTooLargeException());
    }

//    std::cout << "----------------------" << std::endl;
//    for(int i=0;i<length;i++)
//    {
//        std::cout << xPoints[i] << "|"  << yPoints[i] << std::endl;
//    }
//    std::cout << "----------------------" << std::endl;


    if(dataId < 0 || curves[dataId] == NULL)
    {
//        std::cout << "New" << std::endl;
        needRepaint = true;
        if(dataId < 0 && curveId >= 100)
        {
            throw (IdTooLargeException());
        }
        QwtPlotCurve* curve = new QwtPlotCurve();
        curve->setStyle(QwtPlotCurve::Dots);
        QPen pen;
        pen.setWidth(5);
        curve->setPen(pen);
        curve->setData(xPoints, yPoints, length);
        curve->attach(&plottingWidget);
        curve->setAxis(xAxis, yAxis);
        if(dataManager->isAutoscrolling() || dataManager->isFixedSize())
        {
            setAxisBoundaries(X_BOTTOM, dataManager->getMinX(), dataManager->getMaxX());
            setAxisBoundaries(Y_LEFT, dataManager->getMinY(), dataManager->getMaxY());
        }
        for(int i=0;i<length;i++)
        {
            setMinMaxPoints(xPoints[i], yPoints[i]);
        }
	if(dataId > 0)
	  {
	    curves[dataId] = curve;
	    curveId++;
	  }
	else
	  {
	    curves[curveId] = curve;
	    dataId = curveId++;
	  }
        curve->setTitle(QString::number(dataId));
    }
    // existing data
    else
    {
        QwtPlotCurve* curve = curves[dataId];
        QwtArrayData& currentData  = (QwtArrayData&)curve->data();
        QVector<double> xVector = currentData.xData();
        QVector<double> yVector = currentData.yData();
        int oldLength = xVector.size();
        for(int i=0;i<length;i++)
        {
            setMinMaxPoints(xPoints[i], yPoints[i]);
            xVector.push_back(xPoints[i]);
            yVector.push_back(yPoints[i]);
        }
        curve->setData(xVector, yVector);
        curve->setAxis(xAxis, yAxis);
        // if autoscrolling is set to true
        // rescale the axis
        if(dataManager->isAutoscrolling())
        {
            // set the max to the last value and add 5% off the total span
            std::cout << maxXBottom << "|" << maxYLeft << std::endl;
            double finalMaxX = maxXBottom*1.05;
            double finalMaxY = maxYLeft * 1.05;
	    if(maxXBottom > plottingWidget.axisScaleDiv(QwtPlot::xBottom)->upperBound())
	      {
                needRepaint = true;
		plottingWidget.setAxisScale(QwtPlot::xBottom, finalMaxX - xSpan, finalMaxX);
                dataManager->setMinX(finalMaxX - xSpan);
                dataManager->setMaxX(finalMaxX);
	      }
	    if(maxYLeft > plottingWidget.axisScaleDiv(QwtPlot::yLeft)->upperBound())
	      {
                needRepaint = true;
		plottingWidget.setAxisScale(QwtPlot::yLeft, finalMaxY - ySpan, finalMaxY);
                dataManager->setMinY(finalMaxY - ySpan);
                dataManager->setMaxY(finalMaxY);
	      }
        }
        if(!needRepaint)
        {
            curve->draw(oldLength-1, xVector.size()+1);
        }
    }
    setSliderValues();
    if(dataManager->isAutoscaling())
    {
        if(maxYLeft > plottingWidget.axisScaleDiv(QwtPlot::yLeft)->upperBound())
	{
            needRepaint = true;
            setFastAxisBoundaries(Y_LEFT, minYLeft, maxYLeft);
        }
        if(maxXBottom > plottingWidget.axisScaleDiv(QwtPlot::xBottom)->upperBound())
	{
            needRepaint = true;
            setFastAxisBoundaries(X_BOTTOM, minXBottom, maxXBottom);
        }
        // TODO: set x and y spans here
    }
    if(needRepaint)
    {
        plottingWidget.replot();
        setZoomBase();
    }
    emit notifyUpdate();
    return dataId;
}

void PlotWidget::showCurve(QwtPlotItem* item, bool checked)
{
    item->setVisible(!checked);
    QWidget *w = plottingWidget.legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
    {
         ((QwtLegendItem*)w)->setChecked(checked);
    }
    plottingWidget.replot();
}

void PlotWidget::setMinMaxPoints(double xPoint, double yPoint)
{
    if(xPoint < minXBottom)
    {
        minXBottom = xPoint;
    }
    if(xPoint > maxXBottom)
    {
        maxXBottom = xPoint;
    }
    if(yPoint < minYLeft)
    {
        minYLeft = yPoint;
    }
    if(yPoint > maxYLeft)
    {
        maxYLeft = yPoint;
    }
}

void PlotWidget::enableData(int dataId, bool enable)
{
    QwtPlotCurve* curve = curves[dataId];
    if(curve == NULL)
    {
        if(enable)
        {
            curve->attach(&plottingWidget);
        }
        else
        {
            curve->detach();
        }
    }
}

void PlotWidget::setDataStyle(int dataId, QPen pen, int curveStyle)
{
    QwtPlotCurve* curve = curves[dataId];
    if(curve != NULL)
    {
        QPen cPen = curve->pen();
        cPen.setColor(pen.color());
        curve->setPen(cPen);
	switch(curveStyle)
	  {
	  case NO_CURVE:
	    curve->setStyle(QwtPlotCurve::NoCurve);
	    break;
	  case LINES:
	    curve->setStyle(QwtPlotCurve::Lines);
	    break;
	  case STICKS:
	    curve->setStyle(QwtPlotCurve::Sticks);
	    break;
	  case STEPS:
	    curve->setStyle(QwtPlotCurve::Steps);
	    break;
	  case DOTS:
	    curve->setStyle(QwtPlotCurve::Dots);
	    break;
	  default:
	    curve->setStyle(QwtPlotCurve::UserCurve);
	  }
    }
}


void PlotWidget::exportPlotAsImage()
{
    QtExporter::exportWidgetAsImagWithDialog(&plottingWidget);
}
