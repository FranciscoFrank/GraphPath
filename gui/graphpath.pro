QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# Output configuration
TARGET = graphpath-gui
TEMPLATE = app
DESTDIR = ../build

# Include paths
INCLUDEPATH += ../include \
               include

# Source files
SOURCES += src/main.cpp \
           src/MainWindow.cpp \
           src/GraphEditorWidget.cpp \
           src/ControlPanel.cpp \
           src/ResultsWidget.cpp \
           src/GraphWrapper.cpp \
           ../src/graph.c \
           ../src/bfs.c \
           ../src/dfs.c \
           ../src/dijkstra.c

# Header files
HEADERS += include/MainWindow.h \
           include/GraphEditorWidget.h \
           include/ControlPanel.h \
           include/ResultsWidget.h \
           include/GraphWrapper.h \
           ../include/graph.h \
           ../include/bfs.h \
           ../include/dfs.h \
           ../include/dijkstra.h

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Compiler flags for C files
QMAKE_CFLAGS += -std=c11 -Wall -Wextra

# Enable debugging
CONFIG += debug_and_release
