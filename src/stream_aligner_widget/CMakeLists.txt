rock_find_qt4(OPTIONAL)
rock_find_qt5(OPTIONAL)

include(RockQt)

rock_qt_vizkit_widget(
    TARGETPREFIX stream_aligner_widget
    QT4_SUFFIX ""
    MISSINGQTDEPS_NOBUILD
    SOURCES StreamAlignerWidget.cpp StreamAlignerWidgetPlugin.cpp
    HEADERS StreamAlignerWidget.h StreamAlignerWidgetPlugin.h
    MOC StreamAlignerWidget.h StreamAlignerWidgetPlugin.h
    DEPS_PKGCONFIG base-types aggregator
    LIBS_QT5 Qt5::Widgets Qt5::Gui
    )

rock_qt_executable(
		TARGETPREFIX testWidget
		QT4_SUFFIX ""
		MISSINGQTDEPS_NOBUILD
		SOURCES test.cpp
		DEPS_QT stream_aligner_widget
		MOC test.hpp)

