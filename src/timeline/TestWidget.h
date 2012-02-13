#ifndef TESTWIDGET_H
#define	TESTWIDGET_H

#include <QtGui>
#include "Timeline.h"
#include <iostream>

class TestWidget : public QWidget {
    Q_OBJECT
public slots:
    void updateSliderIndex() {
        int value = ledit->text().toInt();
        tw->setSliderIndex(value);
        updateValueDisplay();
    }
    
    void updateStartMarkerIndex() {
        int value = ledit->text().toInt();
        tw->setStartMarkerIndex(value);
        updateValueDisplay();
    }
    
    void updateEndMarkerIndex() {
        int value = ledit->text().toInt();
        tw->setEndMarkerIndex(value);
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
    TestWidget(Timeline *tw) {
        this->tw = tw;
        
        connect(tw, SIGNAL(timeout()), this, SLOT(updateValueDisplay()));
        
        resize(300,300);

        QVBoxLayout vbox(this);
        vbox.setAlignment(Qt::AlignTop);

        // Text entering field
        ledit = new QLineEdit();
        
        butSetIndexSlider = new QPushButton("Set IndexSlider");
        butSetStartMarker = new QPushButton("Set start marker index");
        butSetEndMarker = new QPushButton("Set end marker index");
        butAddBookmark = new QPushButton("Add bookmark");
        butRemoveBookmark = new QPushButton("Remove bookmark");
        butRemoveAllBookmarks = new QPushButton("Remove all bookmarks");
        
        vbox.addWidget(ledit);
        vbox.addWidget(butSetIndexSlider);
        vbox.addWidget(butSetStartMarker);
        vbox.addWidget(butSetEndMarker);
        vbox.addWidget(butAddBookmark);
        vbox.addWidget(butRemoveBookmark);
        vbox.addWidget(butRemoveAllBookmarks);

        QFormLayout formLayout(this);
        vbox.addLayout(&formLayout);
        formLayout.addRow("Start marker index:", startMarkerIndex = new QLabel("value"));
        formLayout.addRow("End marker index:", endMarkerIndex = new QLabel("value"));
        formLayout.addRow("Index slider index:", sliderIndex = new QLabel("value"));
        
        connect(butSetIndexSlider, SIGNAL(pressed()), this, SLOT(updateSliderIndex()));
        connect(butSetStartMarker, SIGNAL(pressed()), this, SLOT(updateStartMarkerIndex()));
        connect(butSetEndMarker, SIGNAL(pressed()), this, SLOT(updateEndMarkerIndex()));
        connect(butAddBookmark, SIGNAL(pressed()), this, SLOT(addBookmarkRequest()));
        connect(butRemoveBookmark, SIGNAL(pressed()), this, SLOT(removeBookmarkRequest()));
        connect(butRemoveAllBookmarks, SIGNAL(pressed()), tw, SLOT(removeAllBookmarks()));

        updateValueDisplay();
        
        show();
        
    }
    

private:
    Timeline *tw;
    QLineEdit *ledit;
    QPushButton *butSetIndexSlider;
    QPushButton *butSetStartMarker;
    QPushButton *butSetEndMarker;
    QPushButton *butAddBookmark;
    QPushButton *butRemoveBookmark;
    QPushButton *butRemoveAllBookmarks;
    QLabel *startMarkerIndex;
    QLabel *endMarkerIndex;
    QLabel *sliderIndex;
    
};

#endif	/* TESTWIDGET_H */

