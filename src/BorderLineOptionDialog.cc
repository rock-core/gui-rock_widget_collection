/* 
 * File:   BorderLineOptionDialog.cc
 * Author: blueck
 * 
 * Created on 19. Oktober 2010, 14:36
 */

#include "BorderLineOptionDialog.h"

BorderLineOptionDialog::BorderLineOptionDialog() : QWidget(),
          addButton(tr("Add"))
{
    yValue = 0;
    currentIndex=0;
}


BorderLineOptionDialog::~BorderLineOptionDialog()
{
    for(unsigned int i=0;i<buttons.size();i++)
    {
        ColorPickerButton* button = buttons[i];
        if(button != NULL)
        {
            delete(button);
        }
    }
    buttons.clear();
    
    for(unsigned int i=0;i<xSpinBoxes.size();i++)
    {
        QDoubleSpinBox* spinBox = xSpinBoxes[i];
        if(spinBox != NULL)
        {
            delete(spinBox);
        }
    }
    xSpinBoxes.clear();
    
    for(unsigned int i=0;i<ySpinBoxes.size();i++)
    {
        QDoubleSpinBox* spinBox = ySpinBoxes[i];
        if(spinBox != NULL)
        {
            delete(spinBox);
        }
    }
    ySpinBoxes.clear();
    
    for(unsigned int i=0;i<comboBoxes.size();i++)
    {
        QComboBox* comboBox = comboBoxes[i];
        if(comboBox != NULL)
        {
            delete(comboBox);
        }
    }
    comboBoxes.clear();
    
    for(unsigned int i=0;i<styleBoxes.size();i++)
    {
        QComboBox* styleBox = styleBoxes[i];
        if(styleBox != NULL)
        {
            delete(styleBox);
        }
    }
    styleBoxes.clear();
    
    for(unsigned int i=0;i<weightBoxes.size();i++)
    {
        QSpinBox* weightBox = weightBoxes[i];
        if(weightBox != NULL)
        {
            delete(weightBox);
        }
    }
    weightBoxes.clear();
    newMarkers.clear();
    oldMarkers.clear();
}

void BorderLineOptionDialog::addMarker()
{
    QwtPlotMarker* marker = new QwtPlotMarker();
    std::cout << "Push Back" << std::endl;
    newMarkers.push_back(marker);
    layout.removeWidget(&addButton);
    buttons.reserve(buttons.size()+1);
    comboBoxes.reserve(comboBoxes.size()+1);
    xSpinBoxes.reserve(xSpinBoxes.size()+1);
    ySpinBoxes.reserve(ySpinBoxes.size()+1);
    styleBoxes.reserve(styleBoxes.size()+1);
    weightBoxes.reserve(weightBoxes.size() +1);
    addMarkerToLayout(marker, currentIndex);
    layout.addWidget(&addButton, yValue, 0, 5, 1);
    std::cout << "Done" << std::endl;
    currentIndex++;
}

std::vector<QwtPlotMarker*> BorderLineOptionDialog::getNewMarkers()
{
    for(unsigned int i=0;i<newMarkers.size();i++)
    {
        QwtPlotMarker* marker = newMarkers[i];
        if(marker != NULL)
        {
            QColor color = buttons[i + oldSize]->getDisplayColor();
            double xValue = xSpinBoxes[i + oldSize]->value();
            double yValue = ySpinBoxes[i + oldSize]->value();
            QwtPlotMarker::LineStyle style = (QwtPlotMarker::LineStyle)comboBoxes[i + oldSize]->currentIndex();
            Qt::PenStyle penStyle = (Qt::PenStyle)styleBoxes[i + oldSize]->currentIndex();
            QPen pen = marker->linePen();
            pen.setStyle(penStyle);
            pen.setColor(color);
            pen.setWidth(weightBoxes[i + oldSize]->value());
            marker->setLinePen(pen);
            marker->setXValue(xValue);
            marker->setYValue(yValue);
            marker->setLineStyle(style);
        }
    }
    std::cout << "Returning new Markers" << std::endl;
    return newMarkers;
}

void BorderLineOptionDialog::updateExistingMarkers()
{
    std::cout << oldMarkers.size() << "|" << buttons.size() << std::endl;
    for(unsigned int i=0;i<oldMarkers.size();i++)
    {
        QwtPlotMarker* marker = oldMarkers[i];
        if(marker != NULL)
        {
            std::cout << i << std::endl;
            std::cout << "True:" << (buttons[i] == NULL) << std::endl;
            QColor color = buttons[i]->getDisplayColor();
            double xValue = xSpinBoxes[i]->value();
            double yValue = ySpinBoxes[i]->value();
            QwtPlotMarker::LineStyle style = (QwtPlotMarker::LineStyle)comboBoxes[i]->currentIndex();
            Qt::PenStyle penStyle = (Qt::PenStyle)styleBoxes[i]->currentIndex();
            QPen pen = marker->linePen();
            pen.setColor(color);
            pen.setStyle(penStyle);
            pen.setWidth(weightBoxes[i]->value());
            marker->setLinePen(pen);
            marker->setXValue(xValue);
            marker->setYValue(yValue);
            marker->setLineStyle(style);
        }
    }
    std::cout << "Update success" << std::endl;
}

void BorderLineOptionDialog::addMarkerToLayout(QwtPlotMarker* marker, int i)
{
    QFrame* frame = new QFrame();
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    frame->setLineWidth(2);
    QGridLayout* frameLayout = new QGridLayout();
    

    QLabel* label = new QLabel(QString::number(i));

    QPen pen = marker->linePen();

    ColorPickerButton* button = new ColorPickerButton(pen.color());
    buttons[i] = button;

    QSpinBox* weightBox = new QSpinBox();
    weightBox->setValue(pen.width());
    weightBoxes[i] = weightBox;

    QDoubleSpinBox* ySpinBox = new QDoubleSpinBox();
    ySpinBox->setSingleStep(0.01);
    ySpinBox->setValue(marker->value().y());
    ySpinBoxes[i] = ySpinBox;

    QDoubleSpinBox* xSpinBox = new QDoubleSpinBox();
    xSpinBox->setValue(marker->value().x());
    xSpinBox->setSingleStep(0.01);
    xSpinBoxes[i] = xSpinBox;

    QComboBox* combo = new QComboBox();
    combo->addItem(tr("No Line"), QVariant(0));
    combo->addItem(tr("Horizontal"), QVariant(1));
    combo->addItem(tr("Vertical"), QVariant(2));
    combo->addItem(tr("Cross"), QVariant(3));
    int current = marker->lineStyle();
    combo->setCurrentIndex(current);
    comboBoxes[i] = combo;

    QComboBox* styleBox = new QComboBox();
    styleBox->addItem(tr("No Style"), QVariant(0));
    styleBox->addItem(tr("Solid"), QVariant(1));
    styleBox->addItem(tr("Dashed"), QVariant(2));
    styleBox->addItem(tr("Dots"), QVariant(3));
    styleBox->addItem(tr("Dash-Dot"), QVariant(4));
    styleBox->addItem(tr("Dash-Dot-Dot"), QVariant(5));
    current = marker->linePen().style();
    styleBox->setCurrentIndex(current);
    styleBoxes[i] = styleBox;

    topLevelLayout.setAlignment(Qt::AlignTop);

    frameLayout->addWidget(label, yValue, 0, 2, 1);
    frameLayout->addWidget(xSpinBox, yValue, 3, 1, 1);
    frameLayout->addWidget(ySpinBox, yValue+1, 3, 1, 1);
    frameLayout->addWidget(combo, yValue, 4, 1, 2);
    frameLayout->addWidget(styleBox, yValue+1, 4, 1, 2);
    frameLayout->addWidget(weightBox, yValue, 6, 1, 1);
    frameLayout->addWidget(button, yValue+1, 6, 1, 1);

    frame->setLayout(frameLayout);
    topLevelLayout.addWidget(frame, yValue, 0, 1, 1);

    yValue += 2;

}

void BorderLineOptionDialog::initializeLayout(std::vector<QwtPlotMarker*> markers)
{
    oldSize = 0;
    oldMarkers = markers;
    int size = markers.size();
    buttons.reserve(size);
    comboBoxes.reserve(size);
    xSpinBoxes.reserve(size);
    ySpinBoxes.reserve(size);
    styleBoxes.reserve(size);
    weightBoxes.reserve(size);
    for(int i=0;i<size;i++)
    {
        QwtPlotMarker* marker = markers[i];
        if(marker != NULL)
        {
            addMarkerToLayout(marker, currentIndex);
            currentIndex++;
            oldSize++;
        }
    }
    topLevelLayout.setSizeConstraint(QLayout::SetMinAndMaxSize);
    layoutFrame.setLayout(&topLevelLayout);
    layoutFrame.setMinimumSize(320, 250);
    scrollArea.setWidget(&layoutFrame);
    scrollArea.setWidgetResizable(true);
    layout.addWidget(&scrollArea, 0, 0, 1, 1);
    layout.addWidget(&addButton, 1, 0, 1, 1);
    connect(&addButton, SIGNAL(clicked()), this, SLOT(addMarker()));
    setLayout(&layout);
}


