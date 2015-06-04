# TarotClub command line client
#
# TarotClub ( http://www.tarotclub.fr ) - This file is part of TarotClub
# Copyright (C) 2003-2999 - Anthony Rabine
# anthony@tarotclub.fr
#
# TarotClub is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# TarotClub is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with TarotClub.  If not, see <http://www.gnu.org/licenses/>.
#

# ------------------------------------------------------------------------------
# Directories for generated files and base directory
# ------------------------------------------------------------------------------
BASE_DIR = $${PWD}/../
TAROTCLUB_DIR = $${PWD}/../

CONFIG(debug, debug|release) {
debug:      DESTDIR = $$BASE_DIR/build-botclient/debug
}

CONFIG(release, debug|release) {
release:    DESTDIR = $$BASE_DIR/build-botclient/release
}

UI_DIR          = $$DESTDIR/ui
UI_HEADERS_DIR  = $$DESTDIR/include
UI_SOURCES_DIR  = $$DESTDIR/src
OBJECTS_DIR     = $$DESTDIR/obj
RCC_DIR         = $$DESTDIR/rcc
MOC_DIR         = $$DESTDIR/moc

# ------------------------------------------------------------------------------
# The search path to find supplied files
# ------------------------------------------------------------------------------
VPATH += $$TAROTCLUB_DIR/src
VPATH += $$TAROTCLUB_DIR/src/library
VPATH += $$TAROTCLUB_DIR/src/botclient
VPATH += $$TAROTCLUB_DIR/src/jsengine
VPATH += $$TAROTCLUB_DIR/src/json
VPATH += $$TAROTCLUB_DIR/lib
VPATH += $$TAROTCLUB_DIR/ai
VPATH += $$TAROTCLUB_DIR/ai/tarotlib
VPATH += $$TAROTCLUB_DIR/src/network
VPATH += $$TAROTCLUB_DIR/src/zip

# ------------------------------------------------------------------------------
# Where to find header files
# ------------------------------------------------------------------------------
INCLUDEPATH += $$TAROTCLUB_DIR/src
INCLUDEPATH += $$TAROTCLUB_DIR/src/library
INCLUDEPATH += $$TAROTCLUB_DIR/src/botclient
INCLUDEPATH += $$TAROTCLUB_DIR/src/jsengine
INCLUDEPATH += $$TAROTCLUB_DIR/src/json
INCLUDEPATH += $$TAROTCLUB_DIR/src/network
INCLUDEPATH += $$TAROTCLUB_DIR/src/zip

# ------------------------------------------------------------------------------
# Compiler definitions
# ------------------------------------------------------------------------------
CONFIG += qt console warn_on
QMAKE_CXXFLAGS += -std=c++11

# Mainly for Duktape, the only source code in C
QMAKE_CFLAGS_DEBUG      += -O0 -pedantic -std=c99 -fstrict-aliasing -ggdb
QMAKE_CFLAGS_RELEASE    += -Os -pedantic -std=c99 -fstrict-aliasing -fomit-frame-pointer

# ------------------------------------------------------------------------------
# Targer definitions
# ------------------------------------------------------------------------------
TARGET = botclient # name of the output executable

# Specific OS stuff
win32 {
    LIBS +=  libws2_32 -lpsapi
    DEFINES += USE_WINDOWS_OS
    # Let's make everything's static so that we don't need any DLL
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lpthread
}

unix {
    DEFINES += USE_UNIX_OS
}

# ------------------------------------------------------------------------------
# Library files
# ------------------------------------------------------------------------------
HEADERS += Log.h \
    Observer.h \
    Util.h \
    ThreadQueue.h \
    ByteStreamReader.h \
    ByteStreamWriter.h \
    ByteArray.h \
    TcpSocket.h \
    TcpServer.h \
    TcpClient.h \
    UniqueId.h \
    Value.h \
    GetOptions.h \
    Zip.h

SOURCES += Log.cpp \
    Util.cpp \
    ByteArray.cpp \
    ByteStreamReader.cpp \
    ByteStreamWriter.cpp \
    TcpSocket.cpp \
    TcpServer.cpp \
    TcpClient.cpp \
    UniqueId.cpp \
    Value.cpp \
    Zip.cpp

# ------------------------------------------------------------------------------
# JSEngine and JSON files
# ------------------------------------------------------------------------------

HEADERS += duktape.h \
    JsonWriter.h \
    JsonReader.h \
    JSEngine.h \
    IScriptEngine.h \
    JsonValue.h

SOURCES += duktape.c \
    JsonWriter.cpp \
    JsonReader.cpp \
    JSEngine.cpp \
    JsonValue.cpp

# -------------------------------------------------------------
# TarotClub core files
# -------------------------------------------------------------
HEADERS += ClientConfig.h \
    Deck.h \
    Card.h \
    Player.h \
    Client.h \
    Bot.h \
    Defines.h \
    Common.h \
    Deal.h \
    Identity.h \
    Score.h \
    Protocol.h \
    System.h

SOURCES += ClientConfig.cpp \
    Deck.cpp \
    Card.cpp \
    Player.cpp \
    Client.cpp \
    Bot.cpp \
    Common.cpp \
    Deal.cpp \
    Protocol.cpp \
    Score.cpp \
    System.cpp

# -------------------------------------------------------------
# Server files
# -------------------------------------------------------------
SOURCES += main.cpp

# End of project file