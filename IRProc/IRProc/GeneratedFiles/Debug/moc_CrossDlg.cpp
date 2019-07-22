/****************************************************************************
** Meta object code from reading C++ file 'CrossDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CrossDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CrossDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CrossDlg_t {
    QByteArrayData data[8];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CrossDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CrossDlg_t qt_meta_stringdata_CrossDlg = {
    {
QT_MOC_LITERAL(0, 0, 8), // "CrossDlg"
QT_MOC_LITERAL(1, 9, 10), // "updateHist"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 7), // "btn_red"
QT_MOC_LITERAL(4, 29, 8), // "btn_blue"
QT_MOC_LITERAL(5, 38, 9), // "btn_green"
QT_MOC_LITERAL(6, 48, 10), // "btn_yellow"
QT_MOC_LITERAL(7, 59, 9) // "btn_clear"

    },
    "CrossDlg\0updateHist\0\0btn_red\0btn_blue\0"
    "btn_green\0btn_yellow\0btn_clear"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CrossDlg[] = {

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
       7,    0,   49,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CrossDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CrossDlg *_t = static_cast<CrossDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateHist(); break;
        case 1: _t->btn_red(); break;
        case 2: _t->btn_blue(); break;
        case 3: _t->btn_green(); break;
        case 4: _t->btn_yellow(); break;
        case 5: _t->btn_clear(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CrossDlg::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CrossDlg.data,
      qt_meta_data_CrossDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CrossDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CrossDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CrossDlg.stringdata0))
        return static_cast<void*>(const_cast< CrossDlg*>(this));
    return QWidget::qt_metacast(_clname);
}

int CrossDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
