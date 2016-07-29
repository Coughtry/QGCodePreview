TARGET   = gcodepreview

TEMPLATE = app

MOC_DIR = .moc
OBJECTS_DIR = .obj

HEADERS     = \
    mainwin.h \
    settings_dlg.h
    

SOURCES     = \
    main.cpp \
    mainwin.cpp \
    settings_dlg.cpp


FORMS += \
    mainwin.ui settings.ui

LIBS += -lQGLViewer -lQGCodeEditor -lQGCodeViewer

QT      *= opengl xml gui core


