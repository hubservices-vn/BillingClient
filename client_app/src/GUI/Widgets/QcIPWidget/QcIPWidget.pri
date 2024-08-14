TEMPLATE    = lib
CONFIG      += designer plugin debug_and_release
DESTDIR = $$[QT_INSTALL_PLUGINS]/designer

CONFIG(release, debug|release){
}
CONFIG(debug, debug|release){
}

HEADERS     = \
    QcIPWidgetPlugin.h \
    QcIPWidget.h \
    QcIPSectorWidget.h \
    ConstConfig.h

SOURCES     = \
    QcIPWidgetPlugin.cpp \
    QcIPWidget.cpp \
    QcIPSectorWidget.cpp

RESOURCES   = icons.qrc

FORMS += \
    QcIPWidget.ui


