/****************************************************************************
** Meta object code from reading C++ file 'IRProc.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../IRProc.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IRProc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IRProc_t {
    QByteArrayData data[15];
    char stringdata0[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IRProc_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IRProc_t qt_meta_stringdata_IRProc = {
    {
QT_MOC_LITERAL(0, 0, 6), // "IRProc"
QT_MOC_LITERAL(1, 7, 12), // "userAreaFull"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 13), // "toolBarExpand"
QT_MOC_LITERAL(4, 35, 12), // "sysSettingOp"
QT_MOC_LITERAL(5, 48, 9), // "customize"
QT_MOC_LITERAL(6, 58, 9), // "imgChange"
QT_MOC_LITERAL(7, 68, 10), // "btnAnalyze"
QT_MOC_LITERAL(8, 79, 11), // "updateImage"
QT_MOC_LITERAL(9, 91, 15), // "colorTypeChange"
QT_MOC_LITERAL(10, 107, 14), // "changeWinWidth"
QT_MOC_LITERAL(11, 122, 7), // "setStep"
QT_MOC_LITERAL(12, 130, 9), // "setFilter"
QT_MOC_LITERAL(13, 140, 8), // "addPoint"
QT_MOC_LITERAL(14, 149, 7) // "addRect"

    },
    "IRProc\0userAreaFull\0\0toolBarExpand\0"
    "sysSettingOp\0customize\0imgChange\0"
    "btnAnalyze\0updateImage\0colorTypeChange\0"
    "changeWinWidth\0setStep\0setFilter\0"
    "addPoint\0addRect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IRProc[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x0a /* Public */,
       3,    0,   80,    2, 0x0a /* Public */,
       4,    0,   81,    2, 0x0a /* Public */,
       5,    0,   82,    2, 0x0a /* Public */,
       6,    0,   83,    2, 0x0a /* Public */,
       7,    0,   84,    2, 0x0a /* Public */,
       8,    0,   85,    2, 0x0a /* Public */,
       9,    0,   86,    2, 0x0a /* Public */,
      10,    0,   87,    2, 0x0a /* Public */,
      11,    0,   88,    2, 0x0a /* Public */,
      12,    1,   89,    2, 0x0a /* Public */,
      13,    0,   92,    2, 0x0a /* Public */,
      14,    0,   93,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void IRProc::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IRProc *_t = static_cast<IRProc *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->userAreaFull(); break;
        case 1: _t->toolBarExpand(); break;
        case 2: _t->sysSettingOp(); break;
        case 3: _t->customize(); break;
        case 4: _t->imgChange(); break;
        case 5: _t->btnAnalyze(); break;
        case 6: _t->updateImage(); break;
        case 7: _t->colorTypeChange(); break;
        case 8: _t->changeWinWidth(); break;
        case 9: _t->setStep(); break;
        case 10: _t->setFilter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->addPoint(); break;
        case 12: _t->addRect(); break;
        default: ;
        }
    }
}

const QMetaObject IRProc::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_IRProc.data,
      qt_meta_data_IRProc,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IRProc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IRProc::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IRProc.stringdata0))
        return static_cast<void*>(const_cast< IRProc*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int IRProc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
