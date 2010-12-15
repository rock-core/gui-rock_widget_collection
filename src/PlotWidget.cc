/*
 * File:   PlotWidget.cc
 * Author: blueck
 *
 * Created on 17. August 2010, 12:24
 */

#include "PlotWidget.h"
#include "XMLConfigWriter.h"
#include <QtPlugin>

Q_EXPORT_PLUGIN2(PlotWidget,PlotWidget)

PlotWidget::PlotWidget(QWidget* parent) : QWidget(parent),
        xBottomSlider(NULL), yLeftSlider(NULL, Qt::Vertical), plot(this),
        zoomer(plot.canvas(), true), markers(100), curves(100), initialRect(-1, -1, -1, -1),
        optionsDialog(this),
        fileMenu(tr("&File")), plotMenu(tr("&Plot")), exportMenu(tr("&Export")), sliderMenu(tr("&Show Sliders")),
        gridMenu(tr("&Grid")), importMenu(tr("&Import")), clearMenu(tr("&Clear")),
        exportImageAction(tr("Export as &image"), this), autoscrollAction(tr("&Autoscrolling"), this),
        fitAction(tr("&Fit to Graph"), this), optionsAction(tr("&Options..."), this),
        leftSliderAction(tr("&Left Slider"), this), exportCSVAction(tr("Export as &CSV"), this),
        bottomSliderAction(tr("&Bottom Slider"), this), xGridAction(tr("X"), this),
        yGridAction(tr("Y"), this), importCSVAction(tr("Import &CSV"), this), clearBorderLineAction(tr("Clear &Border Lines"), this),
        clearCurveAction(tr("Clear &Curves"), this), clearAllAction(tr("Clear &All"), this),
        loadProfileAction(tr("&Load Profile"), this), saveProfileAction(tr("&Save Profile"), this)
{
    plotMarkerId = 0;
    curveId = 0;
    layout.addWidget(&menuBar, 0, 0, 1, 2);
    layout.addWidget(&yLeftSlider, 1, 0);
    layout.addWidget(&plot, 1, 1);
    layout.addWidget(&xBottomSlider, 2, 1);
    this->setLayout(&layout);
    dataManager = DataManager::getInstance();
    addMenu();
    this->enableSlider(X_BOTTOM, dataManager->isShowBottomSlider());
    this->enableSlider(Y_LEFT, dataManager->isShowLeftSlider());
    this->setDrawGrid(true, dataManager->isDrawXGrid(), dataManager->isDrawYGrid());
    dataManager->setBGColor(plot.canvasBackground());
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
    QObject::connect(&plot, SIGNAL(mouseZoomed(const QwtDoubleRect&)), this, SLOT(zoomed(const QwtDoubleRect&)));
    QObject::connect(&optionsDialog, SIGNAL(accepted()), this, SLOT(optionsChanged()));
    zoomer.setTrackerMode(QwtPlotZoomer::AlwaysOn);
    legend.setItemMode(QwtLegend::CheckableItem);
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
    std::cout << markers.size() << std::endl;
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
    plot.replot();
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
    plot.replot();
}

void PlotWidget::clearAll()
{
    clearBorderLines();
    clearCurves();
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
  plotMenu.addAction(&autoscrollAction);
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
  std::cout << dataManager->isDrawXGrid() << std::endl;
  xGridAction.setChecked(dataManager->isDrawXGrid());
  yGridAction.setChecked(dataManager->isDrawYGrid());
  gridMenu.addAction(&xGridAction);
  gridMenu.addAction(&yGridAction);
  connect(&autoscrollAction, SIGNAL(triggered(bool)), this, SLOT(setAutoscrolling(bool)));
  connect(&fitAction, SIGNAL(triggered()), this, SLOT(fitPlotToGraph()));
  connect(&optionsAction, SIGNAL(triggered()), this, SLOT(showOptionsDialog()));
  connect(&leftSliderAction, SIGNAL(triggered()), this, SLOT(sliderActionChecked()));
  connect(&bottomSliderAction, SIGNAL(triggered()), this, SLOT(sliderActionChecked()));
  connect(&xGridAction, SIGNAL(triggered()), this, SLOT(gridChanged()));
  connect(&yGridAction, SIGNAL(triggered()), this, SLOT(gridChanged()));
  connect(&curveSelectionDialog, SIGNAL(accepted()), this, SLOT(curvesSelected()));
}

void PlotWidget::loadProfile()
{
    QString filter;
    QString filename = QFileDialog::getOpenFileName(this, tr("Open XML File"), QDir::homePath(), tr("XML document (*.xml)"), &filter);
    PlotXMLReader reader;
    // create an extra validatioon entry or make validation configurable
//    if(reader.validateXMLFile((char*)filename.toStdString().c_str(), "/home/blueck/temp/configuration.xml"))
//    {
//        std::cout << "Hurray" << std::endl;
//    }
//    else
//    {
//        std::cout << "Naaay" << std::endl;
//        // print error message dialog
//        return;
//    }
    reader.readXMLFile(filename.toStdString().c_str());
    std::vector<QwtPlotMarker*> newMarkers = reader.getMarker();
    for(unsigned int i=0;i<newMarkers.size();i++)
    {
        newMarkers[i]->attach(&plot);
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
    plot.replot();
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

void PlotWidget::refreshFromDataManager()
{
    plot.setCanvasBackground(dataManager->getBGColor());
    setAxisTitles(dataManager->getXAxisTitle(), dataManager->getYAxisTitle());
    if(dataManager->isDrawLegend())
    {
        QList<QWidget*> legendItems = legend.legendItems();
        for(int i=0;i<legendItems.size();i++)
        {
            legendItems[i]->setVisible(true);
        }
        legend.setVisible(true);
        plot.insertLegend(&legend, (QwtPlot::LegendPosition)dataManager->getLegendPosition());
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
    setDrawGrid(true, dataManager->isDrawXGrid(), dataManager->isDrawYGrid());
    std::cout << dataManager->isShowBottomSlider() << "|" << dataManager->isShowLeftSlider() << std::endl;
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
            std::cout << linePoints[0] << "|" << linePoints[1] << std::endl;
            xPoints.append(linePoints[0]);
            yPoints.append(linePoints[1]);
        }
    }
    //only if we have something, else do nothing
    if(xPoints.size() > 0)
    {
        this->addData(xPoints, yPoints);
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
  setAxisAutoScale(X_BOTTOM, true);
  setAxisAutoScale(Y_LEFT, true);
  double lower = plot.axisScaleDiv(QwtPlot::xBottom)->lowerBound();
  double upper = plot.axisScaleDiv(QwtPlot::xBottom)->upperBound();
  setAxisBoundaries(X_BOTTOM, lower, upper);
  lower = plot.axisScaleDiv(QwtPlot::yLeft)->lowerBound();
  upper = plot.axisScaleDiv(QwtPlot::yLeft)->upperBound();
  setAxisBoundaries(Y_LEFT, lower, upper);
  plot.replot();
}

void PlotWidget::showOptionsDialog()
{
  optionsDialog.initializeLayout(curves, markers);
  optionsDialog.show();
}



void PlotWidget::optionsChanged()
{
    std::cout << "Changed" << std::endl;
    plot.setCanvasBackground(dataManager->getBGColor());
    setAxisTitles(dataManager->getXAxisTitle(), dataManager->getYAxisTitle());
    std::vector<QwtPlotMarker*> toDelete = optionsDialog.getDeletedMarkers();
    std::cout << toDelete.size() << std::endl;
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
            marker->attach(&plot);
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
        plot.insertLegend(&legend, (QwtPlot::LegendPosition)dataManager->getLegendPosition());
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
    plot.replot();
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
        grid.attach(&plot);
    }
    else
    {
        xGridAction.setChecked(false);
        yGridAction.setChecked(false);
        grid.detach();
    }
    plot.replot();
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
    marker->attach(&plot);
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

void PlotWidget::zoomed(const QwtDoubleRect& rect)
{
    if(rect == zoomer.zoomBase())
    {
        isZoomed = false;
        // set the slider positions to its original values
        xBottomSlider.blockSignals(true);
        yLeftSlider.blockSignals(true);
        setSliderValues();
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
            marker->attach(&plot);
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
        plot.setAxisAutoScale(axis);
        initialRect.setRect(-1, -1, -1, -1);
    }
    else
    {
        double lower = plot.axisScaleDiv(axis)->lowerBound();
        double upper = plot.axisScaleDiv(axis)->upperBound();
        setAxisBoundaries(axis, lower, upper);
    }
}

void PlotWidget::setAxisTitles(QString xAxisTitle, QString yAxisTitle)
{
    plot.setAxisTitle(QwtPlot::yLeft, yAxisTitle);
    plot.setAxisTitle(QwtPlot::xBottom, xAxisTitle);
    dataManager->setXAxisTitle(xAxisTitle);
    dataManager->setYAxisTitle(yAxisTitle);
}

void PlotWidget::setAxisBoundaries(int axisId, double lower, double upper, double step)
{
    QwtPlot::Axis axis = getAxisForInt(axisId);
    plot.enableAxis(axis);
    plot.setAxisScale(axis, lower, upper, step);
    plot.replot();
    if(axis == QwtPlot::xBottom)
    {
        xSpan = upper - lower;
    }
    else if(axis == QwtPlot::yLeft)
    {
        ySpan = upper -lower;
    }
    setZoomBase();
    autoScale = false;
}

void PlotWidget::setSliderValues()
{
    xBottomSlider.setScalePosition(QwtSlider::BottomScale);
    double currentMinX = plot.axisScaleDiv(QwtPlot::xBottom)->lowerBound();
    double currentMaxX = plot.axisScaleDiv(QwtPlot::xBottom)->upperBound();
    double currentMinY = plot.axisScaleDiv(QwtPlot::yLeft)->lowerBound();
    double currentMaxY = plot.axisScaleDiv(QwtPlot::yLeft)->upperBound();
    xBottomSlider.setRange(minXBottom < currentMinX ? minXBottom : currentMinX, maxXBottom > currentMaxX ? maxXBottom : currentMaxX);
    yLeftSlider.setScalePosition(QwtSlider::LeftScale);
    yLeftSlider.setRange(minYLeft < currentMinY ? minYLeft : currentMinY, maxYLeft > currentMaxY ? maxYLeft : currentMaxY);
}

void PlotWidget::setZoomBase()
{
    double xLower = plot.axisScaleDiv(QwtPlot::xBottom)->lowerBound();
    double width = plot.axisScaleDiv(QwtPlot::xBottom)->upperBound() - plot.axisScaleDiv(QwtPlot::xBottom)->lowerBound();
    double height = plot.axisScaleDiv(QwtPlot::yLeft)->upperBound() - plot.axisScaleDiv(QwtPlot::yLeft)->lowerBound();
    double yLower = plot.axisScaleDiv(QwtPlot::yLeft)->lowerBound();
    initialRect.setRect(xLower, yLower, width, height);
    zoomer.setZoomBase(QwtDoubleRect(minXBottom, maxYLeft, maxXBottom + zoomXSpan, maxYLeft + zoomYSpan));
}

void PlotWidget::xBottomSliderValueChanged(double newValue)
{
    double value = xBottomSlider.value();
    if(!isZoomed)
    {
        plot.setAxisScale(QwtPlot::xBottom, value, value + xSpan);
    }
    else
    {
        plot.setAxisScale(QwtPlot::xBottom, value, value + zoomXSpan);
    }
    plot.replot();
}

void PlotWidget::yLeftSliderValueChanged(double newValue)
{
    double value = yLeftSlider.value();
    if(!isZoomed)
    {
        plot.setAxisScale(QwtPlot::yLeft, value, value + ySpan);
    }
    else
    {
        plot.setAxisScale(QwtPlot::yLeft, value, value + zoomYSpan);
    }
    plot.replot();
}

void PlotWidget::setAxisShown(int axisId, bool enable)
{
    QwtPlot::Axis axis = getAxisForInt(axisId);
    plot.enableAxis(axis, enable);
}

void PlotWidget::setAutoscrolling(bool enable)
{
    this->autoscrolling = enable;
    autoscrollAction.setChecked(enable);
    xBottomSlider.setEnabled(!enable);
    yLeftSlider.setEnabled(!enable);
    zoomer.setEnabled(!enable);
    plot.setMouseWheelZoomAxis(!enable, !enable);
}

int PlotWidget::addData(QList<double> xPoints, QList<double> yPoints, int dataId,
        int xAxisId, int yAxisId)
{
    std::cout << xAxisId << "|" << yAxisId << std::endl;
  return addData(xPoints.toVector().data(), yPoints.toVector().data(), xPoints.size(), dataId, xAxisId, yAxisId);
}

int PlotWidget::addData(double xPoint, double yPoint, int dataId,
        int xAxisId, int yAxisId)
{
  std::cout << "Point at:" << xPoint << "|" << yPoint << std::endl;
  double xPoints[1];
  double yPoints[1];
  xPoints[0] = xPoint;
  yPoints[0] = yPoint;
  return addData(xPoints, yPoints, 1, dataId, xAxisId, yAxisId);
}

int PlotWidget::addData(double* xPoints, double* yPoints, int length, int dataId,
        int xAxisId, int yAxisId)
{
  QwtPlot::Axis xAxis = getAxisForInt(xAxisId);
  QwtPlot::Axis yAxis = getAxisForInt(yAxisId);
  std::cout << xAxisId << "|" << yAxisId << std::endl;
    // new data
    if(dataId < 0 || curves[dataId] == NULL)
    {
        QwtPlotCurve* curve = new QwtPlotCurve();
        curve->setStyle(QwtPlotCurve::Dots);
        QPen pen;
        pen.setWidth(5);
        curve->setPen(pen);
        curve->setData(xPoints, yPoints, length);
        curve->attach(&plot);
        curve->setAxis(xAxis, yAxis);
        for(int i=0;i<length;i++)
        {
            setMinMaxPoints(xPoints[i], yPoints[i]);
        }
        plot.replot();
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
        if(autoscrolling)
        {
            // set the max to the last value and add 5% off the total span
            double finalMaxX = maxXBottom*1.05;
            double finalMaxY = maxYLeft * 1.05;
	    if(maxXBottom > plot.axisScaleDiv(QwtPlot::xBottom)->upperBound())
	      {
		plot.setAxisScale(QwtPlot::xBottom, finalMaxX - xSpan, finalMaxX);
	      }
	    if(maxYLeft > plot.axisScaleDiv(QwtPlot::yLeft)->upperBound())
	      {
		plot.setAxisScale(QwtPlot::yLeft, finalMaxY - ySpan, finalMaxY);
	      }
        }
        plot.replot();
    }
    setSliderValues();
    if(autoScale)
    {
        setAxisBoundaries(Y_LEFT, minYLeft, maxYLeft*1.05);
        setAxisBoundaries(X_BOTTOM, minXBottom, maxXBottom*1.05);
        // TODO: set x and y spans here
        
    }
    setZoomBase();
    connect(&plot, SIGNAL(legendChecked(QwtPlotItem *, bool)), this, SLOT(showCurve(QwtPlotItem *, bool)));
    return dataId;
}

void PlotWidget::showCurve(QwtPlotItem* item, bool checked)
{
    item->setVisible(!checked);
    QWidget *w = plot.legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
    {
         ((QwtLegendItem*)w)->setChecked(checked);
    }
    plot.replot();
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
            curve->attach(&plot);
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
    QtExporter::exportWidgetAsImagWithDialog(&plot);
}
