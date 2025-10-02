/****************************************************************************
** Meta object code from reading C++ file 'patternbar.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/patternbar.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'patternbar.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10PatternBarE_t {};
} // unnamed namespace

template <> constexpr inline auto PatternBar::qt_create_metaobjectdata<qt_meta_tag_ZN10PatternBarE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PatternBar",
        "patternSelected",
        "",
        "PatternType",
        "pattern",
        "patternBarClosed",
        "onSolidClicked",
        "onDense1Clicked",
        "onDense2Clicked",
        "onDense3Clicked",
        "onDense4Clicked",
        "onDense5Clicked",
        "onDense6Clicked",
        "onDense7Clicked",
        "onHorizontalClicked",
        "onVerticalClicked",
        "onCrossClicked",
        "onBDiagClicked",
        "onFDiagClicked",
        "onDiagCrossClicked",
        "onDotsClicked",
        "onGridClicked",
        "onCirclesClicked",
        "onWavesClicked",
        "onStarsClicked",
        "onBrickClicked",
        "onHexagonsClicked",
        "onScalesClicked",
        "onZigzagClicked",
        "onCheckerboardClicked",
        "onTrianglesClicked",
        "onNoiseClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'patternSelected'
        QtMocHelpers::SignalData<void(enum PatternType)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'patternBarClosed'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onSolidClicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDense1Clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDense2Clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDense3Clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDense4Clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDense5Clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDense6Clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDense7Clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onHorizontalClicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onVerticalClicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCrossClicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBDiagClicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFDiagClicked'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDiagCrossClicked'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDotsClicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onGridClicked'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCirclesClicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onWavesClicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onStarsClicked'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBrickClicked'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onHexagonsClicked'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onScalesClicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onZigzagClicked'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCheckerboardClicked'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTrianglesClicked'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNoiseClicked'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PatternBar, qt_meta_tag_ZN10PatternBarE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PatternBar::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10PatternBarE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10PatternBarE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10PatternBarE_t>.metaTypes,
    nullptr
} };

void PatternBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PatternBar *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->patternSelected((*reinterpret_cast< std::add_pointer_t<enum PatternType>>(_a[1]))); break;
        case 1: _t->patternBarClosed(); break;
        case 2: _t->onSolidClicked(); break;
        case 3: _t->onDense1Clicked(); break;
        case 4: _t->onDense2Clicked(); break;
        case 5: _t->onDense3Clicked(); break;
        case 6: _t->onDense4Clicked(); break;
        case 7: _t->onDense5Clicked(); break;
        case 8: _t->onDense6Clicked(); break;
        case 9: _t->onDense7Clicked(); break;
        case 10: _t->onHorizontalClicked(); break;
        case 11: _t->onVerticalClicked(); break;
        case 12: _t->onCrossClicked(); break;
        case 13: _t->onBDiagClicked(); break;
        case 14: _t->onFDiagClicked(); break;
        case 15: _t->onDiagCrossClicked(); break;
        case 16: _t->onDotsClicked(); break;
        case 17: _t->onGridClicked(); break;
        case 18: _t->onCirclesClicked(); break;
        case 19: _t->onWavesClicked(); break;
        case 20: _t->onStarsClicked(); break;
        case 21: _t->onBrickClicked(); break;
        case 22: _t->onHexagonsClicked(); break;
        case 23: _t->onScalesClicked(); break;
        case 24: _t->onZigzagClicked(); break;
        case 25: _t->onCheckerboardClicked(); break;
        case 26: _t->onTrianglesClicked(); break;
        case 27: _t->onNoiseClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PatternBar::*)(PatternType )>(_a, &PatternBar::patternSelected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PatternBar::*)()>(_a, &PatternBar::patternBarClosed, 1))
            return;
    }
}

const QMetaObject *PatternBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PatternBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10PatternBarE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PatternBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 28)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void PatternBar::patternSelected(PatternType _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void PatternBar::patternBarClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
