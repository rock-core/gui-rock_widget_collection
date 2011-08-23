#include <RockSlider.h>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>

RockSlider::RockSlider(QWidget *parent):
    QSlider(parent)
{
}

RockSlider::~RockSlider()
{
}


void RockSlider::addMarker(int position, std::string comment){
    Entry e;
    e.position = position;
    e.comment = comment;
    markers.push_back(e);
}

int RockSlider::rangeValueToPixelPos(int pos) const
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    int sliderMin, sliderMax, sliderLength;

    if (orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    //TODO Dosn't fit perfect, a few pixel misaligment is there, reason unknown...
    return QStyle::sliderPositionFromValue(minimum(), maximum(), pos, sliderMax - sliderMin, opt.upsideDown) + sliderLength/2.0;
}

void RockSlider::paint_p(QPainter *painter)
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
//    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    
    for(int i=0;i<markers.size();i++){
        int pos = rangeValueToPixelPos(markers[i].position);
        if(orientation() == Qt::Horizontal){
            painter->drawLine(pos,gr.top(),pos,gr.bottom());
        }else{
            painter->drawLine(gr.left(),pos,gr.right(),pos);
        }
    }
}


void RockSlider::paintEvent(QPaintEvent *e)
{
    QSlider::paintEvent(e);
    QPainter p(this);
    paint_p(&p);
}

