rock_find_qt4(OPTIONAL)
rock_find_qt5(OPTIONAL)

if(ROCK_QT_VERSION_4)
rock_vizkit_widget(stream_aligner_widget SHARED
    SOURCES StreamAlignerWidget.cpp StreamAlignerWidgetPlugin.cpp
    HEADERS StreamAlignerWidget.h StreamAlignerWidgetPlugin.h
    MOC StreamAlignerWidget.h StreamAlignerWidgetPlugin.h
    DEPS_PKGCONFIG base-types aggregator
    )

rock_executable(testWidget
		test.cpp
		DEPS stream_aligner_widget
		MOC test.hpp)
endif()

if(ROCK_QT_VERSION_5)
rock_vizkit_widget(stream_aligner_widget-qt5 SHARED
    SOURCES StreamAlignerWidget.cpp StreamAlignerWidgetPlugin.cpp
    HEADERS StreamAlignerWidget.h StreamAlignerWidgetPlugin.h
    MOC5 StreamAlignerWidget.h StreamAlignerWidgetPlugin.h
    DEPS_PKGCONFIG base-types aggregator
    LIBS Qt5::Widgets Qt5::Gui
    )

rock_executable(testWidget-qt5
		test.cpp
		DEPS stream_aligner_widget-qt5
		MOC5 test.hpp)
endif()
