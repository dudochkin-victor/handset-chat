### eqmake4 was here ###
CONFIG -= debug_and_release debug
CONFIG += release

include(../common.pri)
TEMPLATE = app
QT += dbus mobility
CONFIG +=  \
    meegotouch \
    link_pkgconfig \
    dcpconfig \
    mobility \
    debug
MOBILITY += contacts
PKGCONFIG += libmeegochat meegotouch
MOC_DIR = .moc
OBJECTS_DIR = .obj
MGEN_OUTDIR = .gen

include($$[QT_INSTALL_DATA]/mkspecs/dcpconfig.pri)

INCLUDEPATH += /usr/include/duicontrolpanel
LIBS += -lduicontrolpanel

DEFINES += THEMEDIR=\\\"\"$$M_PREFIX/share/themes/base/meegotouch/$$TARGET/\"\\\"
target.path = $$M_INSTALL_BIN

STYLE_HEADERS += contactlistitemstyle.h \
    contactmessagelistitemstyle.h \
    messagelistitemstyle.h \
    dialogpageheaderstyle.h \
    acctlistitemstyle.h

HEADERS += $$STYLE_HEADERS \
    main.h \
    chatapp.h \
    contactlistpage.h \
    contactlistitemcreator.h \
    contactlistitem.h \
    acctwizardpage.h \
    contactmessagelistitemcreator.h \
    contactmessagelistitem.h \
    dialogpage.h \
    messagelistitem.h \
    messagelistitemcreator.h \
    dialogpageheader.h \
    timeutil.h \
    acctlistitem.h \
    acctlistitemcreator.h \
    addcontactpage.h \
    approvecontactwidget.h \
    chatdbusadaptor.h

SOURCES += main.cpp \
    chatapp.cpp \
    contactlistpage.cpp \
    contactlistitemcreator.cpp \
    contactlistitem.cpp \
    acctwizardpage.cpp \
    contactmessagelistitemcreator.cpp \
    contactmessagelistitem.cpp \
    dialogpage.cpp \
    messagelistitemcreator.cpp \
    messagelistitem.cpp \
    dialogpageheader.cpp \
    timeutil.cpp \
    acctlistitem.cpp \
    acctlistitemcreator.cpp \
    addcontactpage.cpp \
    approvecontactwidget.cpp \
    chatdbusadaptor.cpp


MAKE_CLEAN += $$OBJECTS_DIR/*.o
MAKE_DISTCLEAN += $$MOC_DIR/* \
    $$MOC_DIR \
    $$OBJECTS_DIR/* \
    $$OBJECTS_DIR \
    $$MGEN_OUTDIR/* \
    $$MGEN_OUTDIR

# Install
INSTALLS += target
