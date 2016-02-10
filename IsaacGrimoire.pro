#-------------------------------------------------
#
# Project created by QtCreator 2016-01-22T21:04:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4):greaterThan(QT_MINOR_VERSION, 4) {
    macx:QMAKE_LFLAGS += -Wl -rpath @executable_path/../Frameworks
}
macx {
    QMAKE_LFLAGS += -F /System/Library/Frameworks/Security.framework/
    LIBS += -framework Security
}
TARGET = IsaacGrimoire
TEMPLATE = app

SOURCES +=\
    UI/main.cpp \
    UI/mainwindow.cpp \
    UI/options.cpp \
    UI/flowlayout.cpp \
    UI/qimagebox.cpp \
    Core/CheatSheet.cc \
    Core/Inquirer.cc \
    Core/Item.cc \
    Core/Memory.cc \
    Core/exception/get_process_error.cc \
    Core/exception/get_task_error.cc \
    Core/exception/not_in_game.cc \
    Core/exception/read_memory_error.cc \
    UI/about.cpp
macx {
    SOURCES += Core/MemoryMach.cc
}
win32 {
    SOURCES += Core/MemoryWin32.cc
}

HEADERS  += \
    UI/mainwindow.h \
    UI/options.h \
    UI/flowlayout.h \
    UI/qimagebox.h \
    Core/CheatSheet.h \
    Core/Inquirer.h \
    Core/Item.h \
    Core/Memory.h \
    Core/util.h \
    Core/exception/get_process_error.h \
    Core/exception/get_task_error.h \
    Core/exception/not_in_game.h \
    Core/exception/read_memory_error.h \
    UI/about.h \
    Core/version.h
macx {
    HEADERS += Core/MemoryMach.h
}
win32 {
    HEADERS += Core/MemoryWin32.h
}

FORMS    += \
    UI/mainwindow.ui \
    UI/options.ui \
    UI/about.ui

RESOURCES += \
    data.qrc

TRANSLATIONS += translations/grimoire_en.ts \
                translations/grimoire_ch.ts
