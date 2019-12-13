find_package(Qt5 REQUIRED Core Gui)
rock_vizkit_widget(stream_aligner_widget SHARED
    SOURCES StreamAlignerWidget.cpp StreamAlignerWidgetPlugin.cpp
    HEADERS StreamAlignerWidget.h StreamAlignerWidgetPlugin.h
    MOC StreamAlignerWidget.h StreamAlignerWidgetPlugin.h
    DEPS_PKGCONFIG base-types aggregator
    LIBS ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES}
    )

rock_executable(testWidget 
		test.cpp
		DEPS stream_aligner_widget
		MOC test.hpp)
