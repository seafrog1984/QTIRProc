/****************************************************************************
** Meta object code from reading C++ file 'SetAuthDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SetAuthDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SetAuthDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SetAuthDlg_t {
    QByteArrayData data[8];
    char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetAuthDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetAuthDlg_t qt_meta_stringdata_SetAuthDlg = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SetAuthDlg"
QT_MOC_LITERAL(1, 11, 11), // "btn_getUser"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 7), // "addUser"
QT_MOC_LITERAL(4, 32, 7), // "delUser"
QT_MOC_LITERAL(5, 40, 10), // "changeUser"
QT_MOC_LITERAL(6, 51, 11), // "btn_process"
QT_MOC_LITERAL(7, 63, 7) // "selUser"

    },
    "SetAuthDlg\0btn_getUser\0\0addUser\0delUser\0"
    "changeUser\0btn_process\0selUser"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetAuthDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    1,   49,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    2,

       0        // eod
};

void SetAuthDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SetAuthDlg *_t = static_cast<SetAuthDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->btn_getUser(); break;
        case 1: _t->addUser(); break;
        case 2: _t->delUser(); break;
        case 3: _t->changeUser(); break;
        case 4: _t->btn_process(); break;
        case 5: _t->selUser((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject SetAuthDlg::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SetAuthDlg.data,
      qt_meta_data_SetAuthDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SetAuthDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetAuthDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SetAuthDlg.stringdata0))
        return static_cast<void*>(const_cast< SetAuthDlg*>(this));
    return QWidget::qt_metacast(_clname);
}

int SetAuthDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
