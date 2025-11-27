/****************************************************************************
** Meta object code from reading C++ file 'ControlPanel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../include/ControlPanel.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControlPanel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_ControlPanel_t {
    uint offsetsAndSizes[52];
    char stringdata0[13];
    char stringdata1[21];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[11];
    char stringdata5[11];
    char stringdata6[20];
    char stringdata7[17];
    char stringdata8[4];
    char stringdata9[5];
    char stringdata10[7];
    char stringdata11[18];
    char stringdata12[6];
    char stringdata13[4];
    char stringdata14[19];
    char stringdata15[19];
    char stringdata16[20];
    char stringdata17[21];
    char stringdata18[20];
    char stringdata19[17];
    char stringdata20[18];
    char stringdata21[19];
    char stringdata22[19];
    char stringdata23[20];
    char stringdata24[18];
    char stringdata25[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ControlPanel_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ControlPanel_t qt_meta_stringdata_ControlPanel = {
    {
        QT_MOC_LITERAL(0, 12),  // "ControlPanel"
        QT_MOC_LITERAL(13, 20),  // "createGraphRequested"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 11),  // "numVertices"
        QT_MOC_LITERAL(47, 10),  // "isWeighted"
        QT_MOC_LITERAL(58, 10),  // "isDirected"
        QT_MOC_LITERAL(69, 19),  // "clearGraphRequested"
        QT_MOC_LITERAL(89, 16),  // "addEdgeRequested"
        QT_MOC_LITERAL(106, 3),  // "src"
        QT_MOC_LITERAL(110, 4),  // "dest"
        QT_MOC_LITERAL(115, 6),  // "weight"
        QT_MOC_LITERAL(122, 17),  // "findPathRequested"
        QT_MOC_LITERAL(140, 5),  // "start"
        QT_MOC_LITERAL(146, 3),  // "end"
        QT_MOC_LITERAL(150, 18),  // "loadGraphRequested"
        QT_MOC_LITERAL(169, 18),  // "saveGraphRequested"
        QT_MOC_LITERAL(188, 19),  // "autoLayoutRequested"
        QT_MOC_LITERAL(208, 20),  // "onCreateGraphClicked"
        QT_MOC_LITERAL(229, 19),  // "onClearGraphClicked"
        QT_MOC_LITERAL(249, 16),  // "onAddEdgeClicked"
        QT_MOC_LITERAL(266, 17),  // "onFindPathClicked"
        QT_MOC_LITERAL(284, 18),  // "onLoadGraphClicked"
        QT_MOC_LITERAL(303, 18),  // "onSaveGraphClicked"
        QT_MOC_LITERAL(322, 19),  // "onAutoLayoutClicked"
        QT_MOC_LITERAL(342, 17),  // "onWeightedChanged"
        QT_MOC_LITERAL(360, 5)   // "state"
    },
    "ControlPanel",
    "createGraphRequested",
    "",
    "numVertices",
    "isWeighted",
    "isDirected",
    "clearGraphRequested",
    "addEdgeRequested",
    "src",
    "dest",
    "weight",
    "findPathRequested",
    "start",
    "end",
    "loadGraphRequested",
    "saveGraphRequested",
    "autoLayoutRequested",
    "onCreateGraphClicked",
    "onClearGraphClicked",
    "onAddEdgeClicked",
    "onFindPathClicked",
    "onLoadGraphClicked",
    "onSaveGraphClicked",
    "onAutoLayoutClicked",
    "onWeightedChanged",
    "state"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ControlPanel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,  104,    2, 0x06,    1 /* Public */,
       6,    0,  111,    2, 0x06,    5 /* Public */,
       7,    3,  112,    2, 0x06,    6 /* Public */,
      11,    2,  119,    2, 0x06,   10 /* Public */,
      14,    0,  124,    2, 0x06,   13 /* Public */,
      15,    0,  125,    2, 0x06,   14 /* Public */,
      16,    0,  126,    2, 0x06,   15 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      17,    0,  127,    2, 0x08,   16 /* Private */,
      18,    0,  128,    2, 0x08,   17 /* Private */,
      19,    0,  129,    2, 0x08,   18 /* Private */,
      20,    0,  130,    2, 0x08,   19 /* Private */,
      21,    0,  131,    2, 0x08,   20 /* Private */,
      22,    0,  132,    2, 0x08,   21 /* Private */,
      23,    0,  133,    2, 0x08,   22 /* Private */,
      24,    1,  134,    2, 0x08,   23 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double,    8,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   25,

       0        // eod
};

Q_CONSTINIT const QMetaObject ControlPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ControlPanel.offsetsAndSizes,
    qt_meta_data_ControlPanel,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ControlPanel_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ControlPanel, std::true_type>,
        // method 'createGraphRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'clearGraphRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addEdgeRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'findPathRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'loadGraphRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveGraphRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'autoLayoutRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCreateGraphClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearGraphClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddEdgeClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFindPathClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onLoadGraphClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSaveGraphClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAutoLayoutClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onWeightedChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void ControlPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControlPanel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->createGraphRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 1: _t->clearGraphRequested(); break;
        case 2: _t->addEdgeRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3]))); break;
        case 3: _t->findPathRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->loadGraphRequested(); break;
        case 5: _t->saveGraphRequested(); break;
        case 6: _t->autoLayoutRequested(); break;
        case 7: _t->onCreateGraphClicked(); break;
        case 8: _t->onClearGraphClicked(); break;
        case 9: _t->onAddEdgeClicked(); break;
        case 10: _t->onFindPathClicked(); break;
        case 11: _t->onLoadGraphClicked(); break;
        case 12: _t->onSaveGraphClicked(); break;
        case 13: _t->onAutoLayoutClicked(); break;
        case 14: _t->onWeightedChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ControlPanel::*)(int , bool , bool );
            if (_t _q_method = &ControlPanel::createGraphRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ControlPanel::*)();
            if (_t _q_method = &ControlPanel::clearGraphRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ControlPanel::*)(int , int , double );
            if (_t _q_method = &ControlPanel::addEdgeRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ControlPanel::*)(int , int );
            if (_t _q_method = &ControlPanel::findPathRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ControlPanel::*)();
            if (_t _q_method = &ControlPanel::loadGraphRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ControlPanel::*)();
            if (_t _q_method = &ControlPanel::saveGraphRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ControlPanel::*)();
            if (_t _q_method = &ControlPanel::autoLayoutRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject *ControlPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ControlPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ControlPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void ControlPanel::createGraphRequested(int _t1, bool _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ControlPanel::clearGraphRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ControlPanel::addEdgeRequested(int _t1, int _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ControlPanel::findPathRequested(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ControlPanel::loadGraphRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ControlPanel::saveGraphRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ControlPanel::autoLayoutRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
