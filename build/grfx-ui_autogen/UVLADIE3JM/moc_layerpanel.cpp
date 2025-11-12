/****************************************************************************
** Meta object code from reading C++ file 'layerpanel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/layerpanel.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'layerpanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.3. It"
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
struct qt_meta_tag_ZN10LayerPanelE_t {};
} // unnamed namespace

template <> constexpr inline auto LayerPanel::qt_create_metaobjectdata<qt_meta_tag_ZN10LayerPanelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "LayerPanel",
        "layerSelected",
        "",
        "index",
        "layerAdded",
        "layerDeleted",
        "layerDuplicated",
        "layerMoved",
        "fromIndex",
        "toIndex",
        "layerVisibilityChanged",
        "visible",
        "layerOpacityChanged",
        "opacity",
        "layerBlendModeChanged",
        "Layer::BlendMode",
        "mode",
        "layerRenamed",
        "name",
        "onCurrentItemChanged",
        "QListWidgetItem*",
        "current",
        "previous",
        "onAddLayerClicked",
        "onDeleteLayerClicked",
        "onDuplicateLayerClicked",
        "onOpacityChanged",
        "value",
        "onBlendModeChanged",
        "onItemChanged",
        "item"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'layerSelected'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'layerAdded'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'layerDeleted'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'layerDuplicated'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'layerMoved'
        QtMocHelpers::SignalData<void(int, int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 8 }, { QMetaType::Int, 9 },
        }}),
        // Signal 'layerVisibilityChanged'
        QtMocHelpers::SignalData<void(int, bool)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Bool, 11 },
        }}),
        // Signal 'layerOpacityChanged'
        QtMocHelpers::SignalData<void(int, qreal)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QReal, 13 },
        }}),
        // Signal 'layerBlendModeChanged'
        QtMocHelpers::SignalData<void(int, Layer::BlendMode)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { 0x80000000 | 15, 16 },
        }}),
        // Signal 'layerRenamed'
        QtMocHelpers::SignalData<void(int, const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 18 },
        }}),
        // Slot 'onCurrentItemChanged'
        QtMocHelpers::SlotData<void(QListWidgetItem *, QListWidgetItem *)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 20, 21 }, { 0x80000000 | 20, 22 },
        }}),
        // Slot 'onAddLayerClicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDeleteLayerClicked'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDuplicateLayerClicked'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onOpacityChanged'
        QtMocHelpers::SlotData<void(int)>(26, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 27 },
        }}),
        // Slot 'onBlendModeChanged'
        QtMocHelpers::SlotData<void(int)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'onItemChanged'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 20, 30 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<LayerPanel, qt_meta_tag_ZN10LayerPanelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject LayerPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10LayerPanelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10LayerPanelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10LayerPanelE_t>.metaTypes,
    nullptr
} };

void LayerPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<LayerPanel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->layerSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->layerAdded(); break;
        case 2: _t->layerDeleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->layerDuplicated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->layerMoved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 5: _t->layerVisibilityChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 6: _t->layerOpacityChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qreal>>(_a[2]))); break;
        case 7: _t->layerBlendModeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Layer::BlendMode>>(_a[2]))); break;
        case 8: _t->layerRenamed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->onCurrentItemChanged((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[2]))); break;
        case 10: _t->onAddLayerClicked(); break;
        case 11: _t->onDeleteLayerClicked(); break;
        case 12: _t->onDuplicateLayerClicked(); break;
        case 13: _t->onOpacityChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->onBlendModeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->onItemChanged((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int )>(_a, &LayerPanel::layerSelected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)()>(_a, &LayerPanel::layerAdded, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int )>(_a, &LayerPanel::layerDeleted, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int )>(_a, &LayerPanel::layerDuplicated, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int , int )>(_a, &LayerPanel::layerMoved, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int , bool )>(_a, &LayerPanel::layerVisibilityChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int , qreal )>(_a, &LayerPanel::layerOpacityChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int , Layer::BlendMode )>(_a, &LayerPanel::layerBlendModeChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int , const QString & )>(_a, &LayerPanel::layerRenamed, 8))
            return;
    }
}

const QMetaObject *LayerPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LayerPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10LayerPanelE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LayerPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void LayerPanel::layerSelected(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void LayerPanel::layerAdded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void LayerPanel::layerDeleted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void LayerPanel::layerDuplicated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void LayerPanel::layerMoved(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void LayerPanel::layerVisibilityChanged(int _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void LayerPanel::layerOpacityChanged(int _t1, qreal _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}

// SIGNAL 7
void LayerPanel::layerBlendModeChanged(int _t1, Layer::BlendMode _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2);
}

// SIGNAL 8
void LayerPanel::layerRenamed(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2);
}
namespace {
struct qt_meta_tag_ZN15LayerItemWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto LayerItemWidget::qt_create_metaobjectdata<qt_meta_tag_ZN15LayerItemWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "LayerItemWidget",
        "visibilityToggled",
        "",
        "index",
        "visible",
        "onVisibilityClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'visibilityToggled'
        QtMocHelpers::SignalData<void(int, bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Bool, 4 },
        }}),
        // Slot 'onVisibilityClicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<LayerItemWidget, qt_meta_tag_ZN15LayerItemWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject LayerItemWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15LayerItemWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15LayerItemWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15LayerItemWidgetE_t>.metaTypes,
    nullptr
} };

void LayerItemWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<LayerItemWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->visibilityToggled((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 1: _t->onVisibilityClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (LayerItemWidget::*)(int , bool )>(_a, &LayerItemWidget::visibilityToggled, 0))
            return;
    }
}

const QMetaObject *LayerItemWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LayerItemWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15LayerItemWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LayerItemWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void LayerItemWidget::visibilityToggled(int _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}
QT_WARNING_POP
