/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "newFile",
        "",
        "openFile",
        "saveFile",
        "saveAsFile",
        "insertImage",
        "convertToBlackAndWhite",
        "newTab",
        "closeTab",
        "setPageSize",
        "Unimalen::PageSize",
        "size",
        "setScale1x",
        "setScale2x",
        "setScale4x",
        "togglePixelZoom",
        "enabled",
        "toggleCoordinates",
        "onPencilSelected",
        "onTextSelected",
        "onSpraySelected",
        "diameter",
        "onBrushSelected",
        "onEraserSelected",
        "onLineSelected",
        "onBezierSelected",
        "onScissorsSelected",
        "onFillSelected",
        "onLassoSelected",
        "onSquareSelected",
        "onFilledSquareSelected",
        "onRoundedSquareSelected",
        "onFilledRoundedSquareSelected",
        "onOvalSelected",
        "onFilledOvalSelected",
        "onCut",
        "onCopy",
        "onPaste",
        "onFontChanged",
        "fontFamily",
        "onFontSizeChanged",
        "fontSize",
        "onThicknessSelected",
        "thickness",
        "toggleToolBar",
        "visible",
        "togglePatternBar",
        "toggleThicknessBar",
        "toggleLayerPanel",
        "toggleColorBar",
        "onTabChanged",
        "index",
        "switchToTab",
        "tabNumber",
        "onLayerAdded",
        "onLayerDeleted",
        "onLayerDuplicated",
        "onLayerSelected",
        "onLayerVisibilityChanged",
        "onLayerOpacityChanged",
        "opacity",
        "onLayerBlendModeChanged",
        "mode",
        "onLayerRenamed",
        "name",
        "onLayersChanged",
        "onCurrentLayerChanged",
        "onPageAdded",
        "onPageDeleted",
        "onPageDuplicated",
        "nextPage",
        "previousPage",
        "updatePageIndicator"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'newFile'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'openFile'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveFile'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveAsFile'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'insertImage'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'convertToBlackAndWhite'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'newTab'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'closeTab'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setPageSize'
        QtMocHelpers::SlotData<void(Unimalen::PageSize)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'setScale1x'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setScale2x'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setScale4x'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'togglePixelZoom'
        QtMocHelpers::SlotData<void(bool)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 17 },
        }}),
        // Slot 'toggleCoordinates'
        QtMocHelpers::SlotData<void(bool)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 17 },
        }}),
        // Slot 'onPencilSelected'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTextSelected'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSpraySelected'
        QtMocHelpers::SlotData<void(int)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'onBrushSelected'
        QtMocHelpers::SlotData<void(int)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'onEraserSelected'
        QtMocHelpers::SlotData<void(int)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'onLineSelected'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBezierSelected'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onScissorsSelected'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFillSelected'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLassoSelected'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSquareSelected'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFilledSquareSelected'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRoundedSquareSelected'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFilledRoundedSquareSelected'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onOvalSelected'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFilledOvalSelected'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCut'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCopy'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPaste'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFontChanged'
        QtMocHelpers::SlotData<void(const QString &)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 40 },
        }}),
        // Slot 'onFontSizeChanged'
        QtMocHelpers::SlotData<void(int)>(41, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 42 },
        }}),
        // Slot 'onThicknessSelected'
        QtMocHelpers::SlotData<void(int)>(43, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 44 },
        }}),
        // Slot 'toggleToolBar'
        QtMocHelpers::SlotData<void(bool)>(45, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 46 },
        }}),
        // Slot 'togglePatternBar'
        QtMocHelpers::SlotData<void(bool)>(47, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 46 },
        }}),
        // Slot 'toggleThicknessBar'
        QtMocHelpers::SlotData<void(bool)>(48, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 46 },
        }}),
        // Slot 'toggleLayerPanel'
        QtMocHelpers::SlotData<void(bool)>(49, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 46 },
        }}),
        // Slot 'toggleColorBar'
        QtMocHelpers::SlotData<void(bool)>(50, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 46 },
        }}),
        // Slot 'onTabChanged'
        QtMocHelpers::SlotData<void(int)>(51, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 52 },
        }}),
        // Slot 'switchToTab'
        QtMocHelpers::SlotData<void(int)>(53, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 54 },
        }}),
        // Slot 'onLayerAdded'
        QtMocHelpers::SlotData<void()>(55, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLayerDeleted'
        QtMocHelpers::SlotData<void(int)>(56, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 52 },
        }}),
        // Slot 'onLayerDuplicated'
        QtMocHelpers::SlotData<void(int)>(57, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 52 },
        }}),
        // Slot 'onLayerSelected'
        QtMocHelpers::SlotData<void(int)>(58, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 52 },
        }}),
        // Slot 'onLayerVisibilityChanged'
        QtMocHelpers::SlotData<void(int, bool)>(59, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 52 }, { QMetaType::Bool, 46 },
        }}),
        // Slot 'onLayerOpacityChanged'
        QtMocHelpers::SlotData<void(int, qreal)>(60, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 52 }, { QMetaType::QReal, 61 },
        }}),
        // Slot 'onLayerBlendModeChanged'
        QtMocHelpers::SlotData<void(int, int)>(62, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 52 }, { QMetaType::Int, 63 },
        }}),
        // Slot 'onLayerRenamed'
        QtMocHelpers::SlotData<void(int, const QString &)>(64, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 52 }, { QMetaType::QString, 65 },
        }}),
        // Slot 'onLayersChanged'
        QtMocHelpers::SlotData<void()>(66, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCurrentLayerChanged'
        QtMocHelpers::SlotData<void(int)>(67, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 52 },
        }}),
        // Slot 'onPageAdded'
        QtMocHelpers::SlotData<void()>(68, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPageDeleted'
        QtMocHelpers::SlotData<void()>(69, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPageDuplicated'
        QtMocHelpers::SlotData<void()>(70, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'nextPage'
        QtMocHelpers::SlotData<void()>(71, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'previousPage'
        QtMocHelpers::SlotData<void()>(72, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updatePageIndicator'
        QtMocHelpers::SlotData<void()>(73, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->newFile(); break;
        case 1: _t->openFile(); break;
        case 2: _t->saveFile(); break;
        case 3: _t->saveAsFile(); break;
        case 4: _t->insertImage(); break;
        case 5: _t->convertToBlackAndWhite(); break;
        case 6: _t->newTab(); break;
        case 7: _t->closeTab(); break;
        case 8: _t->setPageSize((*reinterpret_cast< std::add_pointer_t<Unimalen::PageSize>>(_a[1]))); break;
        case 9: _t->setScale1x(); break;
        case 10: _t->setScale2x(); break;
        case 11: _t->setScale4x(); break;
        case 12: _t->togglePixelZoom((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 13: _t->toggleCoordinates((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 14: _t->onPencilSelected(); break;
        case 15: _t->onTextSelected(); break;
        case 16: _t->onSpraySelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->onBrushSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->onEraserSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->onLineSelected(); break;
        case 20: _t->onBezierSelected(); break;
        case 21: _t->onScissorsSelected(); break;
        case 22: _t->onFillSelected(); break;
        case 23: _t->onLassoSelected(); break;
        case 24: _t->onSquareSelected(); break;
        case 25: _t->onFilledSquareSelected(); break;
        case 26: _t->onRoundedSquareSelected(); break;
        case 27: _t->onFilledRoundedSquareSelected(); break;
        case 28: _t->onOvalSelected(); break;
        case 29: _t->onFilledOvalSelected(); break;
        case 30: _t->onCut(); break;
        case 31: _t->onCopy(); break;
        case 32: _t->onPaste(); break;
        case 33: _t->onFontChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 34: _t->onFontSizeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 35: _t->onThicknessSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 36: _t->toggleToolBar((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 37: _t->togglePatternBar((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 38: _t->toggleThicknessBar((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 39: _t->toggleLayerPanel((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 40: _t->toggleColorBar((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 41: _t->onTabChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 42: _t->switchToTab((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 43: _t->onLayerAdded(); break;
        case 44: _t->onLayerDeleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 45: _t->onLayerDuplicated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 46: _t->onLayerSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 47: _t->onLayerVisibilityChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 48: _t->onLayerOpacityChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qreal>>(_a[2]))); break;
        case 49: _t->onLayerBlendModeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 50: _t->onLayerRenamed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 51: _t->onLayersChanged(); break;
        case 52: _t->onCurrentLayerChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 53: _t->onPageAdded(); break;
        case 54: _t->onPageDeleted(); break;
        case 55: _t->onPageDuplicated(); break;
        case 56: _t->nextPage(); break;
        case 57: _t->previousPage(); break;
        case 58: _t->updatePageIndicator(); break;
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
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 59)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 59;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 59)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 59;
    }
    return _id;
}
QT_WARNING_POP
