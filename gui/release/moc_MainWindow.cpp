/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../include/MainWindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_MainWindow_t {
    uint offsetsAndSizes[44];
    char stringdata0[11];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[11];
    char stringdata5[11];
    char stringdata6[13];
    char stringdata7[10];
    char stringdata8[4];
    char stringdata9[5];
    char stringdata10[7];
    char stringdata11[11];
    char stringdata12[6];
    char stringdata13[4];
    char stringdata14[12];
    char stringdata15[12];
    char stringdata16[11];
    char stringdata17[7];
    char stringdata18[13];
    char stringdata19[8];
    char stringdata20[6];
    char stringdata21[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MainWindow_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 13),  // "onCreateGraph"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 11),  // "numVertices"
        QT_MOC_LITERAL(38, 10),  // "isWeighted"
        QT_MOC_LITERAL(49, 10),  // "isDirected"
        QT_MOC_LITERAL(60, 12),  // "onClearGraph"
        QT_MOC_LITERAL(73, 9),  // "onAddEdge"
        QT_MOC_LITERAL(83, 3),  // "src"
        QT_MOC_LITERAL(87, 4),  // "dest"
        QT_MOC_LITERAL(92, 6),  // "weight"
        QT_MOC_LITERAL(99, 10),  // "onFindPath"
        QT_MOC_LITERAL(110, 5),  // "start"
        QT_MOC_LITERAL(116, 3),  // "end"
        QT_MOC_LITERAL(120, 11),  // "onLoadGraph"
        QT_MOC_LITERAL(132, 11),  // "onSaveGraph"
        QT_MOC_LITERAL(144, 10),  // "onNewGraph"
        QT_MOC_LITERAL(155, 6),  // "onExit"
        QT_MOC_LITERAL(162, 12),  // "onAutoLayout"
        QT_MOC_LITERAL(175, 7),  // "onError"
        QT_MOC_LITERAL(183, 5),  // "error"
        QT_MOC_LITERAL(189, 7)   // "onAbout"
    },
    "MainWindow",
    "onCreateGraph",
    "",
    "numVertices",
    "isWeighted",
    "isDirected",
    "onClearGraph",
    "onAddEdge",
    "src",
    "dest",
    "weight",
    "onFindPath",
    "start",
    "end",
    "onLoadGraph",
    "onSaveGraph",
    "onNewGraph",
    "onExit",
    "onAutoLayout",
    "onError",
    "error",
    "onAbout"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   80,    2, 0x08,    1 /* Private */,
       6,    0,   87,    2, 0x08,    5 /* Private */,
       7,    3,   88,    2, 0x08,    6 /* Private */,
      11,    2,   95,    2, 0x08,   10 /* Private */,
      14,    0,  100,    2, 0x08,   13 /* Private */,
      15,    0,  101,    2, 0x08,   14 /* Private */,
      16,    0,  102,    2, 0x08,   15 /* Private */,
      17,    0,  103,    2, 0x08,   16 /* Private */,
      18,    0,  104,    2, 0x08,   17 /* Private */,
      19,    1,  105,    2, 0x08,   18 /* Private */,
      21,    0,  108,    2, 0x08,   20 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double,    8,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSizes,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onCreateGraph'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onClearGraph'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddEdge'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onFindPath'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onLoadGraph'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSaveGraph'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNewGraph'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onExit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAutoLayout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onAbout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onCreateGraph((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 1: _t->onClearGraph(); break;
        case 2: _t->onAddEdge((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3]))); break;
        case 3: _t->onFindPath((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->onLoadGraph(); break;
        case 5: _t->onSaveGraph(); break;
        case 6: _t->onNewGraph(); break;
        case 7: _t->onExit(); break;
        case 8: _t->onAutoLayout(); break;
        case 9: _t->onError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->onAbout(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
