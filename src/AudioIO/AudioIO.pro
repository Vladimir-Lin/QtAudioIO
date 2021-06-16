NAME         = AudioIO
TARGET       = $${NAME}

QT           = core
QT          -= gui
QT          += network
QT          += sql
QT          += script
QT          += multimedia
QT          += QtFFmpeg
QT          += QtAudio
QT          += Essentials
QT          += Mathematics

load(qt_build_config)
load(qt_module)

INCLUDEPATH += $${PWD}/../../include/$${NAME}

HEADERS     += $${PWD}/../../include/$${NAME}/qtaudioio.h

include ($${PWD}/Audio/Audio.pri)
include ($${PWD}/MFF/MFF.pri)
include ($${PWD}/Samples/Samples.pri)

OTHER_FILES += $${PWD}/../../include/$${NAME}/headers.pri

include ($${PWD}/../../doc/Qt/Qt.pri)

TRNAME       = $${NAME}
include ($${PWD}/../../Translations.pri)

win32 {
LIBS        += -lavcodec
LIBS        += -lavdevice
LIBS        += -lavfilter
LIBS        += -lavformat
LIBS        += -lavutil
LIBS        += -lpostproc
LIBS        += -lswresample
LIBS        += -lswscale
LIBS        += -luser32
LIBS        += -lwsock32
LIBS        += -lAdvapi32
LIBS        += -lole32
LIBS        += -ldsound
LIBS        += -lsetupapi
LIBS        += -lwinmm
}
