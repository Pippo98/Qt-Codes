/****************************************************************************
** Meta object code from reading C++ file 'graph.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Serial_Graph_03/graph.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_graph_t {
    QByteArrayData data[12];
    char stringdata0[150];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_graph_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_graph_t qt_meta_stringdata_graph = {
    {
QT_MOC_LITERAL(0, 0, 5), // "graph"
QT_MOC_LITERAL(1, 6, 12), // "graphChanged"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 7), // "setAxis"
QT_MOC_LITERAL(4, 28, 12), // "updateSeries"
QT_MOC_LITERAL(5, 41, 16), // "setGeneralYRange"
QT_MOC_LITERAL(6, 58, 17), // "setSpecificYRange"
QT_MOC_LITERAL(7, 76, 13), // "getAxisValues"
QT_MOC_LITERAL(8, 90, 14), // "QAbstractAxis*"
QT_MOC_LITERAL(9, 105, 14), // "setSeriesArray"
QT_MOC_LITERAL(10, 120, 16), // "QAbstractSeries*"
QT_MOC_LITERAL(11, 137, 12) // "setFrameRate"

    },
    "graph\0graphChanged\0\0setAxis\0updateSeries\0"
    "setGeneralYRange\0setSpecificYRange\0"
    "getAxisValues\0QAbstractAxis*\0"
    "setSeriesArray\0QAbstractSeries*\0"
    "setFrameRate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_graph[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    2,   55,    2, 0x0a /* Public */,
       4,    0,   60,    2, 0x0a /* Public */,
       5,    0,   61,    2, 0x0a /* Public */,
       6,    0,   62,    2, 0x0a /* Public */,
       7,    4,   63,    2, 0x0a /* Public */,
       9,    3,   72,    2, 0x0a /* Public */,
      11,    1,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 8, 0x80000000 | 8,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void graph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<graph *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->graphChanged(); break;
        case 1: _t->setAxis((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->updateSeries(); break;
        case 3: _t->setGeneralYRange(); break;
        case 4: _t->setSpecificYRange(); break;
        case 5: _t->getAxisValues((*reinterpret_cast< QAbstractAxis*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 6: _t->setSeriesArray((*reinterpret_cast< QAbstractSeries*(*)>(_a[1])),(*reinterpret_cast< QAbstractAxis*(*)>(_a[2])),(*reinterpret_cast< QAbstractAxis*(*)>(_a[3]))); break;
        case 7: _t->setFrameRate((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractAxis* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractAxis* >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSeries* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (graph::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&graph::graphChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject graph::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_graph.data,
    qt_meta_data_graph,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *graph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *graph::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_graph.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int graph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void graph::graphChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
