/****************************************************************************
** Meta object code from reading C++ file 'serial.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Serial_Graph_03/serial.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serial.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_serial_t {
    QByteArrayData data[21];
    char stringdata0[300];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_serial_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_serial_t qt_meta_stringdata_serial = {
    {
QT_MOC_LITERAL(0, 0, 6), // "serial"
QT_MOC_LITERAL(1, 7, 11), // "dataChanged"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 4), // "data"
QT_MOC_LITERAL(4, 25, 10), // "detectPort"
QT_MOC_LITERAL(5, 36, 14), // "getSwitchNames"
QT_MOC_LITERAL(6, 51, 8), // "portInfo"
QT_MOC_LITERAL(7, 60, 4), // "port"
QT_MOC_LITERAL(8, 65, 23), // "getSecondarySwitchNames"
QT_MOC_LITERAL(9, 89, 30), // "getSecondarySwitchesSelections"
QT_MOC_LITERAL(10, 120, 12), // "QVector<int>"
QT_MOC_LITERAL(11, 133, 30), // "setSecondarySwitchesSelections"
QT_MOC_LITERAL(12, 164, 18), // "setPrimarySwitches"
QT_MOC_LITERAL(13, 183, 11), // "sendCommand"
QT_MOC_LITERAL(14, 195, 10), // "getCanMode"
QT_MOC_LITERAL(15, 206, 10), // "setCanMode"
QT_MOC_LITERAL(16, 217, 15), // "manageFunctions"
QT_MOC_LITERAL(17, 233, 12), // "initTextArea"
QT_MOC_LITERAL(18, 246, 13), // "initTextField"
QT_MOC_LITERAL(19, 260, 16), // "initInfoTextArea"
QT_MOC_LITERAL(20, 277, 22) // "displayPerformanceInfo"

    },
    "serial\0dataChanged\0\0data\0detectPort\0"
    "getSwitchNames\0portInfo\0port\0"
    "getSecondarySwitchNames\0"
    "getSecondarySwitchesSelections\0"
    "QVector<int>\0setSecondarySwitchesSelections\0"
    "setPrimarySwitches\0sendCommand\0"
    "getCanMode\0setCanMode\0manageFunctions\0"
    "initTextArea\0initTextField\0initInfoTextArea\0"
    "displayPerformanceInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_serial[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   97,    2, 0x0a /* Public */,
       5,    0,   98,    2, 0x0a /* Public */,
       6,    1,   99,    2, 0x0a /* Public */,
       8,    1,  102,    2, 0x0a /* Public */,
       9,    1,  105,    2, 0x0a /* Public */,
      11,    3,  108,    2, 0x0a /* Public */,
      12,    2,  115,    2, 0x0a /* Public */,
      13,    2,  120,    2, 0x0a /* Public */,
      14,    0,  125,    2, 0x0a /* Public */,
      15,    1,  126,    2, 0x0a /* Public */,
      16,    0,  129,    2, 0x0a /* Public */,
      17,    1,  130,    2, 0x0a /* Public */,
      18,    1,  133,    2, 0x0a /* Public */,
      19,    1,  136,    2, 0x0a /* Public */,
      20,    0,  139,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::QStringList,
    QMetaType::QStringList,
    QMetaType::QString, QMetaType::QString,    7,
    QMetaType::QStringList, QMetaType::Int,    2,
    0x80000000 | 10, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QObjectStar,    2,
    QMetaType::Void, QMetaType::QObjectStar,    2,
    QMetaType::Void, QMetaType::QObjectStar,    2,
    QMetaType::Void,

       0        // eod
};

void serial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<serial *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: { QStringList _r = _t->detectPort();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 2: { QStringList _r = _t->getSwitchNames();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->portInfo((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QStringList _r = _t->getSecondarySwitchNames((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 5: { QVector<int> _r = _t->getSecondarySwitchesSelections((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVector<int>*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->setSecondarySwitchesSelections((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: _t->setPrimarySwitches((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->sendCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: { bool _r = _t->getCanMode();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->setCanMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->manageFunctions(); break;
        case 12: _t->initTextArea((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 13: _t->initTextField((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 14: _t->initInfoTextArea((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 15: _t->displayPerformanceInfo(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (serial::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&serial::dataChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject serial::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_serial.data,
    qt_meta_data_serial,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *serial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *serial::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_serial.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int serial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void serial::dataChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
