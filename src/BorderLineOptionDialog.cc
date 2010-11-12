/* 
 * File:   BorderLineOptionDialog.cc
 * Author: blueck
 * 
 * Created on 19. Oktober 2010, 14:36
 */

#include <QtCore/qnamespace.h>
#include <QtGui/qwidget.h>
#include <QtGui/qlabel.h>
#include <QtGui/qpushbutton.h>
#include <qwt-qt4/qwt_plot_item.h>
#include <qwt-qt4/qwt_plot_marker.h>
#include <QtGui/qspinbox.h>

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

void BorderLineOptionDialog::linkActivated()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Do you really want to delete this border line?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
    {
        // hide the frame and mark it as deleted
        QLabel* sender = (QLabel*)QObject::sender();
        for(int i=0;i<oldMarkers.size();i++)
        {
            QwtPlotMarker* marker = oldMarkers[i];
            if(marker != NULL)
            {
                if(sender == exitLabels[i])
                {
                    // remove
                    std::cout << "Remove old" << std::endl;
                    frames[i]->setVisible(false);
                    markerDeleted[i] = true;
                }
            }
        }
        for(int i=0;i<newMarkers.size();i++)
        {
            QwtPlotMarker* marker = newMarkers[i+oldSize];
            if(marker != NULL)
            {
                if(sender == exitLabels[i+oldSize])
                {
                    // remove
                    std::cout << "Remove new" << std::endl;
                    frames[i+oldSize]->setVisible(false);
                    markerDeleted[i+oldSize] = true;
                }
            }
        }
    }
}

void BorderLineOptionDialog::addMarker()
{
    QwtPlotMarker* marker = new QwtPlotMarker();
    // set default values here
    QPen pen = marker->linePen();
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    marker->setLinePen(pen);
    marker->setLineStyle(QwtPlotMarker::HLine);
    std::cout << "Push Back" << std::endl;
    newMarkers.push_back(marker);
    layout.removeWidget(&addButton);
    buttons.reserve(buttons.size()+1);
    comboBoxes.reserve(comboBoxes.size()+1);
    xSpinBoxes.reserve(xSpinBoxes.size()+1);
    ySpinBoxes.reserve(ySpinBoxes.size()+1);
    styleBoxes.reserve(styleBoxes.size()+1);
    weightBoxes.reserve(weightBoxes.size() +1);
    exitLabels.reserve(exitLabels.size() + 1);
    markerDeleted.resize(markerDeleted.size() + 1, false);
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
            bool deleted = markerDeleted[i + oldSize];
            std::cout << "Deleted: " << deleted << std::endl;
            if(!deleted)
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
                marker->setVisible(hiddenBoxes[i]->isChecked());
            }
            else
            {
                newMarkers[i] = NULL;
            }
        }
    }
    std::cout << "Returning new Markers" << std::endl;
    return newMarkers;
}

std::vector<QwtPlotMarker*> BorderLineOptionDialog::getDeletedMarkers()
{
    std::vector<QwtPlotMarker*> toDelete;
    for(unsigned int i=0;i<oldMarkers.size();i++)
    {
        QwtPlotMarker* marker = oldMarkers[i];
        if(marker != NULL)
        {
            if(markerDeleted[i])
            {
                toDelete.push_back(oldMarkers[i]);
            }
        }
    }
    return toDelete;
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
            marker->setVisible(hiddenBoxes[i]->isChecked());

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
    ySpinBox->setDecimals(4);
    ySpinBox->setMaximum(10000.00);
    ySpinBox->setValue(marker->value().y());
    ySpinBoxes[i] = ySpinBox;

    QDoubleSpinBox* xSpinBox = new QDoubleSpinBox();
    xSpinBox->setValue(marker->value().x());
    xSpinBox->setSingleStep(0.01);
    xSpinBox->setDecimals(4);
    xSpinBox->setMaximum(10000.00);
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

    QCheckBox* hiddenBox = new QCheckBox();
    hiddenBox->setChecked(marker->isVisible());
    hiddenBoxes[i] = hiddenBox;

    topLevelLayout.setAlignment(Qt::AlignTop);

    frameLayout->addWidget(label, yValue, 0, 1, 2);
    frameLayout->addWidget(hiddenBox, yValue+1, 0, 1, 2);
    frameLayout->addWidget(xSpinBox, yValue, 4, 1, 2);
    frameLayout->addWidget(ySpinBox, yValue+1, 4, 1, 2);
    frameLayout->addWidget(combo, yValue, 6, 1, 3);
    frameLayout->addWidget(styleBox, yValue+1, 6, 1, 3);
    frameLayout->addWidget(weightBox, yValue, 9, 1, 2);
    frameLayout->addWidget(button, yValue+1, 9, 1, 2);
    

    // BEGIN TEST

    QLabel* exitLabel = new QLabel(QString("<a href=\"1\"><img src=\"/home/blueck/temp/window-close.png\"></a>"));
    exitLabel->setAlignment(Qt::AlignRight);
    frameLayout->addWidget(exitLabel, yValue, 11, 1, 1);
    exitLabels[i] = exitLabel;
    connect(exitLabel, SIGNAL(linkActivated(const QString&)), this, SLOT(linkActivated()));
    // END TEST
    frame->setLayout(frameLayout);
    topLevelLayout.addWidget(frame, yValue, 0, 1, 1);
    frames[i] = frame;
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
    exitLabels.reserve(size);
    hiddenBoxes.reserve(size);
    frames.reserve(size);
    markerDeleted.resize(size, false);
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


