QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TemperatureCompensation
RC_ICONS =
CONFIG += c++20 embed_translations

QMAKE_CXXFLAGS += -std=c++0x

windows{
    VERSION = 0.0.0.0
    QMAKE_TARGET_DESCRIPTION = "Temperature Compensation"
    QMAKE_TARGET_PRODUCT = "Temperature Compensation 0.0.0.0"
    QMAKE_TARGET_COMPANY = "Shanghai Hezhi Electric Equipment Lt. Co."
    QMAKE_TARGET_COPYRIGHT = "Copyright (C)2023 Shanghai Hezhi Electric Equipment Lt. Co."
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

RESOURCES += res.qrc

include(./log/log.pri)
include(./setting/setting.pri)
include(./ui/ui.pri)
include(./algorithm/algorithm.pri)

CONFIG(debug, debug|release){
    DESTDIR = ../bin_debug
}
CONFIG(release, debug|release){
    DESTDIR = ../bin
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
