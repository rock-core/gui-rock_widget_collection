#include "SonarWidget.h"
#include "./SonarPlot.h"
#include <iostream>




SonarWidget::SonarWidget(QWidget *parent)
    : QWidget(parent)
{
    resize(1020,670);
    
    plot = new SonarPlot(this);
    plot->setGeometry (10,10,1000,600);
    connect(this,SIGNAL(rangeChanged(int)),plot,SLOT(rangeChanged(int)));
    
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, plot->palette().color(QPalette::Background));
    setAutoFillBackground(true);
    setPalette(Pal);
    
    lbGain = new QLabel(this);
    lbGain->setGeometry(10,height()-40,50,20);
    Pal=lbGain->palette();
    Pal.setColor(QPalette::Foreground,Qt::white);
    lbGain->setPalette(Pal);
    lbGain->setText("Gain:");
    slGain = new QSlider(Qt::Horizontal,this);
    slGain->setGeometry(70,height()-40,150,20);
    slGain->setMinimum(0);
    slGain->setMaximum(20);   
    edGain = new QLineEdit(this);
    edGain->setGeometry(230,height()-40,50,20);
    edGain->setAlignment(Qt::AlignRight);
    Pal=edGain->palette();
    Pal.setColor(QPalette::Base, plot->palette().color(QPalette::Background));
    Pal.setColor(QPalette::Text,Qt::white);
    setAutoFillBackground(true);
    edGain->setPalette(Pal);
    connect(slGain,SIGNAL(valueChanged(int)),this,SLOT(onSlGainChanged(int)));

    lbRange = new QLabel(this);
    lbRange->setGeometry(320,height()-40,50,20);
    Pal=lbRange->palette();
    Pal.setColor(QPalette::Foreground,Qt::white);
    lbRange->setPalette(Pal);
    lbRange->setText("Range:");
    comboRange = new QComboBox(this);
    comboRange->setGeometry(380,height()-40,70,20);
    Pal=comboRange->palette();
    Pal.setColor(comboRange->backgroundRole(), plot->palette().color(QPalette::Background));
    Pal.setColor(comboRange->foregroundRole(),Qt::white);
    setAutoFillBackground(true);
    comboRange->setPalette(Pal);
    comboRange->insertItem(comboRange->count()+1,"5");
    comboRange->insertItem(comboRange->count()+1,"10");
    comboRange->insertItem(comboRange->count()+1,"20");
    comboRange->insertItem(comboRange->count()+1,"30");
    comboRange->insertItem(comboRange->count()+1,"40");
    comboRange->insertItem(comboRange->count()+1,"50");
    comboRange->insertItem(comboRange->count()+1,"60");
    comboRange->insertItem(comboRange->count()+1,"80");
    comboRange->insertItem(comboRange->count()+1,"100");
    comboRange->insertItem(comboRange->count()+1,"150");
    comboRange->insertItem(comboRange->count()+1,"200");
    comboRange->insertItem(comboRange->count()+1,"250");
    comboRange->insertItem(comboRange->count()+1,"300");
    connect(comboRange,SIGNAL(currentIndexChanged(QString)),this,SLOT(onComboRangeChanged(QString)));
    
    show();
}

void SonarWidget::resizeEvent ( QResizeEvent * event )
{
  plot->setGeometry (10,10,width()-20,height()-70);
  lbGain->setGeometry(10,height()-40,50,20);
  slGain->setGeometry(70,height()-40,150,20);
  edGain->setGeometry(230,height()-40,50,20);
  lbRange->setGeometry(320,height()-40,50,20);
  comboRange->setGeometry(380,height()-40,70,20);
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
  plot->setData(scan);
}

void SonarWidget::setGain(int value)
{
  slGain->setValue(value);
}

void SonarWidget::setRange(int value)
{
  QString str;
  str.setNum(value);
  comboRange->setCurrentIndex(comboRange->findText(str));
}


void SonarWidget::onSlGainChanged(int value)
{
  QString str;
  str.setNum(value);
  edGain->setText(str);
  gainChanged(value);
}

void SonarWidget::onComboRangeChanged(QString str)
{
  rangeChanged(str.toInt());
}
