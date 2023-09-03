QT += quick

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        MD5.cpp \
        cdatetime_lib.cpp \
        function.cpp \
        log_lib_cplus.cpp \
        main.cpp \
        operproxy.cpp \
        xdb_searcher.cpp \
        function.cpp \
        MD5.cpp MD5New.cpp \
        procfuncinfo.cpp

RESOURCES += qml.qrc
QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += widgets

LIBS+=-LF:\code\assign\chinese\proxyserver\newproxyinfo\WpdPack_4_1_2\WpdPack\Lib -lwpcap -lPacket -lAdvapi32
INCLUDEPATH+=F:\code\assign\chinese\proxyserver\newproxyinfo\WpdPack_4_1_2\WpdPack\Include


HEADERS += \
    MD5.h \
    cdatetime_lib.h \
    function.h \
    log_lib_cplus.h \
    operproxy.h \
    procfuncinfo.h \
    public_lib.h \
    xdb_searcher.h \
    function.h \
    MD5.h MD5New.h

DISTFILES +=
