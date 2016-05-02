find_package(Qt4 REQUIRED QtCore QtGui)
rock_vizkit_widget(stream_aligner_widget SHARED
    SOURCES StreamAlignerWidget.cpp StreamAlignerWidgetPlugin.cpp
    HEADERS StreamAlignerWidget.h StreamAlignerWidgetPlugin.h
    MOC StreamAlignerWidget.h StreamAlignerWidgetPlugin.h
    DEPS_PKGCONFIG base-types aggregator
    LIBS ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
    )

rock_executable(testWidget 
		test.cpp
		DEPS stream_aligner_widget
		MOC test.hpp)
