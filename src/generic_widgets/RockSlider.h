#ifndef ROCK_SLIDER_H
#define ROCK_SLIDER_H

#include <QSlider>
#include <vector>

class RockSlider : public QSlider{
    Q_OBJECT
    struct Entry{
        int position;
        std::string comment;
    };

public:
    RockSlider(QWidget *parent);
    ~RockSlider();

public slots:
    void addMarker(int position, std::string comment = "");
		
protected:
    void paint_p(QPainter *painter);
    int rangeValueToPixelPos(int pos) const;
    void paintEvent(QPaintEvent *);
    std::vector<Entry> markers;
private:
};

#endif
