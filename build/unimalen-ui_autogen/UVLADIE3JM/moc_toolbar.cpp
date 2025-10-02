/****************************************************************************
** Meta object code from reading C++ file 'toolbar.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/toolbar.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'toolbar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN7ToolBarE_t {};
} // unnamed namespace

template <> constexpr inline auto ToolBar::qt_create_metaobjectdata<qt_meta_tag_ZN7ToolBarE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ToolBar",
        "pencilToolSelected",
        "",
        "textToolSelected",
        "sprayToolSelected",
        "diameter",
        "brushToolSelected",
        "eraserToolSelected",
        "lineToolSelected",
        "bezierToolSelected",
        "scissorsToolSelected",
        "fillToolSelected",
        "lassoToolSelected",
        "squareToolSelected",
        "filledSquareToolSelected",
        "roundedSquareToolSelected",
        "filledRoundedSquareToolSelected",
        "ovalToolSelected",
        "filledOvalToolSelected",
        "toolBarClosed",
        "onPencilClicked",
        "onTextClicked",
        "onSprayClicked",
        "onBrushClicked",
        "onEraserClicked",
        "onLineClicked",
        "onBezierClicked",
        "onScissorsClicked",
        "onFillClicked",
        "onLassoClicked",
        "onSquareClicked",
        "onFilledSquareClicked",
        "onRoundedSquareClicked",
        "onFilledRoundedSquareClicked",
        "onOvalClicked",
        "onFilledOvalClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'pencilToolSelected'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'textToolSelected'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sprayToolSelected'
        QtMocHelpers::SignalData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Signal 'brushToolSelected'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Signal 'eraserToolSelected'
        QtMocHelpers::SignalData<void(int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Signal 'lineToolSelected'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'bezierToolSelected'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'scissorsToolSelected'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'fillToolSelected'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'lassoToolSelected'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'squareToolSelected'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'filledSquareToolSelected'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'roundedSquareToolSelected'
        QtMocHelpers::SignalData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'filledRoundedSquareToolSelected'
        QtMocHelpers::SignalData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'ovalToolSelected'
        QtMocHelpers::SignalData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'filledOvalToolSelected'
        QtMocHelpers::SignalData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'toolBarClosed'
        QtMocHelpers::SignalData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onPencilClicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTextClicked'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSprayClicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBrushClicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onEraserClicked'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLineClicked'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBezierClicked'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onScissorsClicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFillClicked'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLassoClicked'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSquareClicked'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFilledSquareClicked'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRoundedSquareClicked'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFilledRoundedSquareClicked'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onOvalClicked'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFilledOvalClicked'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ToolBar, qt_meta_tag_ZN7ToolBarE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ToolBar::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7ToolBarE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7ToolBarE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7ToolBarE_t>.metaTypes,
    nullptr
} };

void ToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ToolBar *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->pencilToolSelected(); break;
        case 1: _t->textToolSelected(); break;
        case 2: _t->sprayToolSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->brushToolSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->eraserToolSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->lineToolSelected(); break;
        case 6: _t->bezierToolSelected(); break;
        case 7: _t->scissorsToolSelected(); break;
        case 8: _t->fillToolSelected(); break;
        case 9: _t->lassoToolSelected(); break;
        case 10: _t->squareToolSelected(); break;
        case 11: _t->filledSquareToolSelected(); break;
        case 12: _t->roundedSquareToolSelected(); break;
        case 13: _t->filledRoundedSquareToolSelected(); break;
        case 14: _t->ovalToolSelected(); break;
        case 15: _t->filledOvalToolSelected(); break;
        case 16: _t->toolBarClosed(); break;
        case 17: _t->onPencilClicked(); break;
        case 18: _t->onTextClicked(); break;
        case 19: _t->onSprayClicked(); break;
        case 20: _t->onBrushClicked(); break;
        case 21: _t->onEraserClicked(); break;
        case 22: _t->onLineClicked(); break;
        case 23: _t->onBezierClicked(); break;
        case 24: _t->onScissorsClicked(); break;
        case 25: _t->onFillClicked(); break;
        case 26: _t->onLassoClicked(); break;
        case 27: _t->onSquareClicked(); break;
        case 28: _t->onFilledSquareClicked(); break;
        case 29: _t->onRoundedSquareClicked(); break;
        case 30: _t->onFilledRoundedSquareClicked(); break;
        case 31: _t->onOvalClicked(); break;
        case 32: _t->onFilledOvalClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::pencilToolSelected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::textToolSelected, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)(int )>(_a, &ToolBar::sprayToolSelected, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)(int )>(_a, &ToolBar::brushToolSelected, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)(int )>(_a, &ToolBar::eraserToolSelected, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::lineToolSelected, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::bezierToolSelected, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::scissorsToolSelected, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::fillToolSelected, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::lassoToolSelected, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::squareToolSelected, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::filledSquareToolSelected, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::roundedSquareToolSelected, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::filledRoundedSquareToolSelected, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::ovalToolSelected, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::filledOvalToolSelected, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (ToolBar::*)()>(_a, &ToolBar::toolBarClosed, 16))
            return;
    }
}

const QMetaObject *ToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7ToolBarE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 33)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 33;
    }
    return _id;
}

// SIGNAL 0
void ToolBar::pencilToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ToolBar::textToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ToolBar::sprayToolSelected(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void ToolBar::brushToolSelected(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void ToolBar::eraserToolSelected(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void ToolBar::lineToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ToolBar::bezierToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void ToolBar::scissorsToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void ToolBar::fillToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void ToolBar::lassoToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void ToolBar::squareToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void ToolBar::filledSquareToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void ToolBar::roundedSquareToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void ToolBar::filledRoundedSquareToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void ToolBar::ovalToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void ToolBar::filledOvalToolSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void ToolBar::toolBarClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}
QT_WARNING_POP
