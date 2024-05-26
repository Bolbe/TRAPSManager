QT += network qml quick sql
CONFIG += c++11

unix:!macx {
        QMAKE_LFLAGS += -no-pie
}


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
        ../../../View/dialogbox.h \
        ../../../View/dialogboxqueue.h


SOURCES += \
        main.cpp \
        ../../../View/dialogbox.cpp \
        ../../../View/dialogboxqueue.cpp

RESOURCES += \
    ../../../tests.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
