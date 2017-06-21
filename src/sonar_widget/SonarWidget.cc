#include "SonarWidget.h"
#include "./SonarPlot.h"
#include <iostream>




SonarWidget::SonarWidget(QWidget *parent)
    : QWidget(parent)
{
    resize(1020,670);

    plot = new SonarPlot(this);
    plot->setGeometry (10,10,600,1300);
    connect(this,SIGNAL(rangeChanged(int)),plot,SLOT(rangeChanged(int)));
    connect(this,SIGNAL(sonarPaletteChanged(int)),plot,SLOT(sonarPaletteChanged(int)));
    connect(this,SIGNAL(gridChanged(bool)),plot,SLOT(gridChanged(bool)));
    connect(this,SIGNAL(interpolationChanged(bool)),plot,SLOT(interpolationChanged(bool)));

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, plot->palette().color(QPalette::Background));
    setAutoFillBackground(true);
    setPalette(Pal);

    // initialize qt components
    createGainComponent();
    createRangeComponent();
    createPaletteComponent();
    createGridComponent();
    createInterpolationComponent();

    show();
}

void SonarWidget::resizeEvent ( QResizeEvent * event )
{
  plot->setGeometry (10,10,width()-20,height()-70);
  lbGain->setGeometry(10,height()-70,80,20);
  slGain->setGeometry(100,height()-70,150,20);
  edGain->setGeometry(250,height()-70,50,20);
  lbRange->setGeometry(10,height()-40,80,20);
  slRange->setGeometry(100,height()-40,150,20);
  edRange->setGeometry(250,height()-40,50,20);
  lbPalette->setGeometry(width()-160,height()-40,50,20);
  comboPalette->setGeometry(width()-100,height()-40,80,20);
  lbGrid->setGeometry(width()-160,height()-70,50,20);
  boxGrid->setGeometry(width()-100,height()-70,80,20);
  lbInterpolation->setGeometry(10, height() - 100, 80, 20);
  boxInterpolation->setGeometry(100, height() - 100, 20, 20);
  QWidget::resizeEvent (event);
}

SonarWidget::~SonarWidget()
{
  if(plot){
    delete plot;
  }
}

void SonarWidget::setData(const base::samples::SonarScan scan)
{
    base::samples::Sonar sonar(scan);
    setData(sonar);
}

void SonarWidget::setData(const base::samples::Sonar sonar)
{
    plot->setData(sonar);
}

void SonarWidget::setGain(int value)
{
  slGain->setValue(value);
}

void SonarWidget::setRange(int value)
{
  slRange->setValue(value);
}

void SonarWidget::setSectorScan(bool continuous, base::Angle left, base::Angle right)
{
    plot->setSectorScan(continuous, left, right);
}

void SonarWidget::setMinRange(int value)
{
  slRange->setMinimum(value);
}

void SonarWidget::setMaxRange(int value)
{
  slRange->setMaximum(value);
}

void SonarWidget::setSonarPalette(int value)
{
  comboPalette->setCurrentIndex(value);
}

void SonarWidget::enableAutoRanging(bool value)
{
  slRange->setEnabled(!value);
}

void SonarWidget::onSlGainChanged(int value)
{
  QString str;
  str.setNum(value);
  edGain->setText(str + " %");
  gainChanged(value);
}

void SonarWidget::onSlRangeChanged(int value)
{
  QString str;
  str.setNum(value);
  edRange->setText(str + " m");
  rangeChanged(value);
}

void SonarWidget::onComboPaletteChanged(int value)
{
    sonarPaletteChanged(value);
}

void SonarWidget::onCheckboxGridChanged(bool value)
{
    gridChanged(value);
}

void SonarWidget::onCheckboxInterpolationChanged(bool value)
{
    interpolationChanged(value);
}

void SonarWidget::createGainComponent() {
    lbGain = new QLabel(this);
    lbGain->setGeometry(10, height() - 70, 80, 20);
    QPalette Pal = lbGain->palette();
    Pal.setColor(QPalette::Foreground, Qt::white);
    lbGain->setPalette(Pal);
    lbGain->setText("Gain:");
    lbGain->setAlignment(Qt::AlignRight);

    slGain = new QSlider(Qt::Horizontal, this);
    slGain->setGeometry(100, height() - 70, 150, 20);
    slGain->setMinimum(0);
    slGain->setMaximum(100);
    slGain->setValue(50);

    edGain = new QLineEdit(this);
    edGain->setGeometry(250, height() - 70, 50, 20);
    edGain->setAlignment(Qt::AlignRight);
    Pal = edGain->palette();
    Pal.setColor(QPalette::Base, plot->palette().color(QPalette::Background));
    Pal.setColor(QPalette::Text, Qt::white);
    setAutoFillBackground(true);
    edGain->setPalette(Pal);
    edGain->setReadOnly(true);
    edGain->setText("50 %");
    connect(slGain,SIGNAL(valueChanged(int)),this,SLOT(onSlGainChanged(int)));
}

void SonarWidget::createRangeComponent() {
    lbRange = new QLabel(this);
    lbRange->setGeometry(10,height()-40,80,20);
    QPalette Pal=lbRange->palette();
    Pal.setColor(QPalette::Foreground,Qt::white);
    lbRange->setPalette(Pal);
    lbRange->setText("Range:");
    lbRange->setAlignment(Qt::AlignRight);

    slRange = new QSlider(Qt::Horizontal, this);
    slRange->setGeometry(100,height()-40,150,20);
    slRange->setMinimum(1);
    slRange->setMaximum(150);
    slRange->setValue(5);

    edRange = new QLineEdit(this);
    edRange->setGeometry(250,height()-40,50,20);
    edRange->setAlignment(Qt::AlignRight);
    Pal=edRange->palette();
    Pal.setColor(QPalette::Base, plot->palette().color(QPalette::Background));
    Pal.setColor(QPalette::Text,Qt::white);
    setAutoFillBackground(true);
    edRange->setPalette(Pal);
    edRange->setReadOnly(true);
    edRange->setText("5 m");
    connect(slRange,SIGNAL(valueChanged(int)),this,SLOT(onSlRangeChanged(int)));
}

void SonarWidget::createPaletteComponent() {
    lbPalette = new QLabel(this);
    lbPalette->setGeometry(width()-160, height() - 40, 50, 20);
    lbPalette->setGeometry(width()-160, height() - 40, 50, 20);
    lbPalette->setText("Palette:");
    lbPalette->setStyleSheet("background-color: blue; color: white;");
    lbPalette->setAlignment(Qt::AlignRight);

    comboPalette = new QComboBox(this);
    comboPalette->setGeometry(width()-100,height()-40,80,20);
    QPalette Pal = comboPalette->palette();
    Pal.setColor(comboPalette->backgroundRole(), plot->palette().color(QPalette::Background));
    Pal.setColor(comboPalette->foregroundRole(),Qt::white);
    setAutoFillBackground(true);
    comboPalette->setPalette(Pal);
    comboPalette->insertItem(comboPalette->count()+1,"Jet");
    comboPalette->insertItem(comboPalette->count()+1,"Hot");
    comboPalette->insertItem(comboPalette->count()+1,"Gray");
    comboPalette->insertItem(comboPalette->count()+1,"Bronze");
    connect(comboPalette,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboPaletteChanged(int)));
}

void SonarWidget::createGridComponent() {
    lbGrid = new QLabel(this);
    lbGrid->setGeometry(width() - 160, height() - 70, 50, 20);
    lbGrid->setText("Grid:");
    lbGrid->setAlignment(Qt::AlignRight);
    lbGrid->setStyleSheet("background-color: blue; color: white;");

    boxGrid = new QCheckBox(this);
    boxGrid->setGeometry(width() - 100, height() - 70, 20, 20);
    boxGrid->setStyleSheet("background-color: blue;");
    boxGrid->setChecked(true);
    connect(boxGrid,SIGNAL(clicked(bool)), this, SLOT(onCheckboxGridChanged(bool)));
}

void SonarWidget::createInterpolationComponent() {
    lbInterpolation = new QLabel(this);
    lbInterpolation->setGeometry(10, height() - 100, 80, 20);
    lbInterpolation->setText("Interpolation:");
    lbInterpolation->setAlignment(Qt::AlignRight);
    lbInterpolation->setStyleSheet("background-color: blue; color: white;");

    boxInterpolation = new QCheckBox(this);
    boxInterpolation->setGeometry(100, height() - 100, 20, 20);
    boxInterpolation->setStyleSheet("background-color: blue;");
    boxInterpolation->setChecked(true);
    connect(boxInterpolation,SIGNAL(clicked(bool)), this, SLOT(onCheckboxInterpolationChanged(bool)));
}
