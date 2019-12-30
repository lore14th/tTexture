/****************************************************************************
** Meta object code from reading C++ file 'EditorWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "pch.h"
#include "../../tTexture/Editor/UI/EditorWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EditorWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_tTexture__EditorWindow_t {
    QByteArrayData data[13];
    char stringdata0[347];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_tTexture__EditorWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_tTexture__EditorWindow_t qt_meta_stringdata_tTexture__EditorWindow = {
    {
QT_MOC_LITERAL(0, 0, 22), // "tTexture::EditorWindow"
QT_MOC_LITERAL(1, 23, 26), // "on_Texture2DButton_clicked"
QT_MOC_LITERAL(2, 50, 0), // ""
QT_MOC_LITERAL(3, 51, 23), // "on_HCrossButton_clicked"
QT_MOC_LITERAL(4, 75, 23), // "on_VCrossButton_clicked"
QT_MOC_LITERAL(5, 99, 32), // "on_EquirectangularButton_clicked"
QT_MOC_LITERAL(6, 132, 30), // "on_InputFilepathButton_clicked"
QT_MOC_LITERAL(7, 163, 38), // "on_InputChannelBox_currentInd..."
QT_MOC_LITERAL(8, 202, 34), // "on_FlipOnLoadCheckbox_stateCh..."
QT_MOC_LITERAL(9, 237, 31), // "on_OutputFilepathButton_clicked"
QT_MOC_LITERAL(10, 269, 33), // "on_PrefilterCheckbox_stateCha..."
QT_MOC_LITERAL(11, 303, 21), // "on_ButtonBox_accepted"
QT_MOC_LITERAL(12, 325, 21) // "on_ButtonBox_rejected"

    },
    "tTexture::EditorWindow\0"
    "on_Texture2DButton_clicked\0\0"
    "on_HCrossButton_clicked\0on_VCrossButton_clicked\0"
    "on_EquirectangularButton_clicked\0"
    "on_InputFilepathButton_clicked\0"
    "on_InputChannelBox_currentIndexChanged\0"
    "on_FlipOnLoadCheckbox_stateChanged\0"
    "on_OutputFilepathButton_clicked\0"
    "on_PrefilterCheckbox_stateChanged\0"
    "on_ButtonBox_accepted\0on_ButtonBox_rejected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_tTexture__EditorWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    0,   73,    2, 0x08 /* Private */,
       7,    0,   74,    2, 0x08 /* Private */,
       8,    0,   75,    2, 0x08 /* Private */,
       9,    0,   76,    2, 0x08 /* Private */,
      10,    0,   77,    2, 0x08 /* Private */,
      11,    0,   78,    2, 0x08 /* Private */,
      12,    0,   79,    2, 0x08 /* Private */,

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
    QMetaType::Void,

       0        // eod
};

void tTexture::EditorWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EditorWindow *_t = static_cast<EditorWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_Texture2DButton_clicked(); break;
        case 1: _t->on_HCrossButton_clicked(); break;
        case 2: _t->on_VCrossButton_clicked(); break;
        case 3: _t->on_EquirectangularButton_clicked(); break;
        case 4: _t->on_InputFilepathButton_clicked(); break;
        case 5: _t->on_InputChannelBox_currentIndexChanged(); break;
        case 6: _t->on_FlipOnLoadCheckbox_stateChanged(); break;
        case 7: _t->on_OutputFilepathButton_clicked(); break;
        case 8: _t->on_PrefilterCheckbox_stateChanged(); break;
        case 9: _t->on_ButtonBox_accepted(); break;
        case 10: _t->on_ButtonBox_rejected(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject tTexture::EditorWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_tTexture__EditorWindow.data,
      qt_meta_data_tTexture__EditorWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *tTexture::EditorWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *tTexture::EditorWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_tTexture__EditorWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int tTexture::EditorWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
