#ifndef TESTWIDGET_H
#define	TESTWIDGET_H

#include <QtGui>
#include "TimelineWidget.h"
#include <iostream>

class TestWidget : public QWidget {
    Q_OBJECT
public slots:
    void updateSliderIndex() {
        int value = ledit->text().toInt();
        std::cout << "line edit value: " << value << std::endl;
        tw->setSliderIndex(value);
        updateValueDisplay();
    }
    
    void updateValueDisplay() {
        startMarkerIndex->setText(QString::number(tw->getStartMarkerIndex()));
        endMarkerIndex->setText(QString::number(tw->getEndMarkerIndex()));
        sliderIndex->setText(QString::number(tw->getSliderIndex()));
    }
    
    void addBookmarkRequest() {
        tw->addBookmark(ledit->text().toInt());
    }
    
    void removeBookmarkRequest() {
        tw->removeBookmark(ledit->text().toInt());
    }
    
public:
    TestWidget(TimelineWidget *tw) {
        this->tw = tw;
        
        connect(tw, SIGNAL(timeout()), this, SLOT(updateValueDisplay()));
        
        resize(300,300);

        QVBoxLayout vbox(this);
        vbox.setAlignment(Qt::AlignTop);

        // Text entering field
        ledit = new QLineEdit();
        
        butSetIndexSlider = new QPushButton("Set IndexSlider");
        butAddBookmark = new QPushButton("Add bookmark");
        butRemoveBookmark = new QPushButton("Remove bookmark");
        
        vbox.addWidget(ledit);
        vbox.addWidget(butSetIndexSlider);
        vbox.addWidget(butAddBookmark);
        vbox.addWidget(butRemoveBookmark);

        QFormLayout formLayout(this);
        vbox.addLayout(&formLayout);
        formLayout.addRow("Start marker index:", startMarkerIndex = new QLabel("value"));
        formLayout.addRow("End marker index:", endMarkerIndex = new QLabel("value"));
        formLayout.addRow("Index slider index:", sliderIndex = new QLabel("value"));
        
        connect(butSetIndexSlider, SIGNAL(pressed()), this, SLOT(updateSliderIndex()));
        connect(butAddBookmark, SIGNAL(pressed()), this, SLOT(addBookmarkRequest()));
        connect(butRemoveBookmark, SIGNAL(pressed()), this, SLOT(removeBookmarkRequest()));

        updateValueDisplay();
        
        show();
        
    }
    


    
private:
    TimelineWidget *tw;
    QLineEdit *ledit;
    QPushButton *butSetIndexSlider;
    QPushButton *butAddBookmark;
    QPushButton *butRemoveBookmark;
    QLabel *startMarkerIndex;
    QLabel *endMarkerIndex;
    QLabel *sliderIndex;
    
};

#endif	/* TESTWIDGET_H */

