#pragma once

#include <QString>
#include "DesignTokens.h"

namespace StyleSheet {

inline QString getApplicationStyleSheet() {
    using namespace DesignTokens;

    return QString(R"(
/* ===== Global Styles ===== */
QWidget {
    background-color: %1;
    color: %2;
    font-family: %3;
    font-size: %4pt;
}

/* ===== Main Window ===== */
QMainWindow {
    background-color: %1;
}

QMainWindow::separator {
    background: %5;
    width: 2px;
    height: 2px;
}

/* ===== Buttons (NumWorks-inspired flat design) ===== */
QPushButton {
    background-color: %6;
    color: white;
    border: 2px solid transparent;
    border-radius: %7px;
    padding: %8px %9px;
    font-weight: 500;
    min-height: %10px;
}

QPushButton:hover {
    background-color: #3574A0;
    border: 2px solid %6;
}

QPushButton:pressed {
    background-color: #2A5A7F;
}

QPushButton:disabled {
    background-color: %11;
    color: %12;
}

/* Secondary Button Style */
QPushButton[class="secondary"] {
    background-color: white;
    color: %2;
    border: 2px solid %5;
}

QPushButton[class="secondary"]:hover {
    background-color: %13;
    border-color: %6;
}

/* Danger Button Style */
QPushButton[class="danger"] {
    background-color: %14;
    color: white;
    border: 2px solid transparent;
}

QPushButton[class="danger"]:hover {
    background-color: #D32F2F;
}

/* ===== Tool Buttons (Toolbar) ===== */
QToolButton {
    background-color: transparent;
    color: %15;
    border: 2px solid transparent;
    border-radius: %16px;
    padding: %17px;
    min-width: %10px;
    min-height: %10px;
}

QToolButton:hover {
    background-color: %11;
    color: %2;
}

QToolButton:pressed, QToolButton:checked {
    background-color: transparent;
    color: %2;
    border: 3px solid %6;
}

/* ===== Input Fields ===== */
QLineEdit, QTextEdit, QPlainTextEdit {
    background-color: white;
    color: %2;
    border: 2px solid %5;
    border-radius: %7px;
    padding: %8px %18px;
    selection-background-color: %6;
    selection-color: white;
}

QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {
    border-color: %6;
    outline: none;
}

QLineEdit:disabled, QTextEdit:disabled, QPlainTextEdit:disabled {
    background-color: %11;
    color: %12;
}

/* ===== Spin Boxes (Number inputs with monospace) ===== */
QSpinBox, QDoubleSpinBox {
    background-color: white;
    color: %2;
    border: 2px solid %5;
    border-radius: %7px;
    padding: %8px %18px;
    font-family: %19;
    min-width: 80px;
}

QSpinBox:focus, QDoubleSpinBox:focus {
    border-color: %6;
}

QSpinBox::up-button, QDoubleSpinBox::up-button,
QSpinBox::down-button, QDoubleSpinBox::down-button {
    background-color: %11;
    border: none;
    width: 20px;
}

QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
    background-color: %5;
}

/* ===== Combo Box / Dropdown ===== */
QComboBox {
    background-color: white;
    color: %2;
    border: 2px solid %5;
    border-radius: %7px;
    padding: %8px %18px;
    min-height: 38px;
}

QComboBox:hover {
    border-color: %6;
}

QComboBox::drop-down {
    border: none;
    width: 30px;
}

QComboBox::down-arrow {
    image: none;
    border-left: 5px solid transparent;
    border-right: 5px solid transparent;
    border-top: 6px solid %2;
    margin-right: 8px;
}

QComboBox QAbstractItemView {
    background-color: white;
    border: 2px solid %5;
    selection-background-color: %6;
    selection-color: white;
    outline: none;
}

/* ===== Sliders (Progress bars) ===== */
QSlider::groove:horizontal {
    height: 8px;
    background-color: %11;
    border: 1px solid %5;
    border-radius: %16px;
}

QSlider::handle:horizontal {
    background-color: %6;
    border: 2px solid %6;
    width: 20px;
    height: 20px;
    margin: -7px 0;
    border-radius: 10px;
}

QSlider::handle:horizontal:hover {
    background-color: #3574A0;
    border-color: #3574A0;
}

/* ===== Scroll Bars (Minimal design) ===== */
QScrollBar:vertical {
    background-color: %13;
    width: 12px;
    border: none;
}

QScrollBar::handle:vertical {
    background-color: %20;
    border-radius: %16px;
    min-height: 30px;
}

QScrollBar::handle:vertical:hover {
    background-color: %15;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QScrollBar:horizontal {
    background-color: %13;
    height: 12px;
    border: none;
}

QScrollBar::handle:horizontal {
    background-color: %20;
    border-radius: %16px;
    min-width: 30px;
}

QScrollBar::handle:horizontal:hover {
    background-color: %15;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

/* ===== Tabs (Navigation) ===== */
QTabWidget::pane {
    border: 2px solid %21;
    border-radius: %22px;
    background-color: white;
}

QTabBar::tab {
    background-color: transparent;
    color: %15;
    border: none;
    border-bottom: 3px solid transparent;
    padding: %18px 20px;
    font-weight: 500;
}

QTabBar::tab:hover {
    color: %2;
    background-color: %13;
}

QTabBar::tab:selected {
    color: %6;
    border-bottom-color: %6;
    font-weight: 600;
}

/* ===== Dock Widgets (Cards/Panels) ===== */
QDockWidget {
    background-color: %13;
    border: 2px solid %21;
    border-radius: %22px;
    titlebar-close-icon: none;
    titlebar-normal-icon: none;
}

QDockWidget::title {
    background-color: %13;
    color: %6;
    padding: %18px;
    font-weight: 600;
    font-size: %23pt;
    border-bottom: 1px solid %21;
    text-align: left;
}

QDockWidget::close-button, QDockWidget::float-button {
    background-color: transparent;
    border: none;
    padding: 4px;
}

QDockWidget::close-button:hover, QDockWidget::float-button:hover {
    background-color: %11;
    border-radius: %16px;
}

/* ===== Menu Bar ===== */
QMenuBar {
    background-color: %1;
    color: %2;
    border-bottom: 1px solid %21;
    spacing: 3px;
    padding: 4px;
}

QMenuBar::item {
    background-color: transparent;
    padding: %8px %24px;
    border-radius: %16px;
}

QMenuBar::item:selected {
    background-color: %11;
}

QMenuBar::item:pressed {
    background-color: %6;
    color: white;
}

/* ===== Menus ===== */
QMenu {
    background-color: white;
    border: 2px solid %21;
    border-radius: %22px;
    padding: %17px;
}

QMenu::item {
    background-color: transparent;
    padding: %8px 24px;
    border-radius: %16px;
}

QMenu::item:selected {
    background-color: %6;
    color: white;
}

QMenu::separator {
    height: 1px;
    background-color: %21;
    margin: %17px 0;
}

/* ===== Status Bar ===== */
QStatusBar {
    background-color: %13;
    color: %15;
    border-top: 1px solid %21;
    padding: %17px;
}

QStatusBar::item {
    border: none;
}

QStatusBar QLabel {
    background-color: transparent;
    padding: 4px %18px;
    border-radius: %16px;
}

/* ===== Group Box (Cards) ===== */
QGroupBox {
    background-color: %13;
    border: 2px solid %21;
    border-radius: %22px;
    padding: %25px;
    margin-top: %24px;
    font-weight: 600;
    color: %6;
}

QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    padding: 0 %17px;
    color: %6;
    font-size: %23pt;
}

/* ===== List Widget ===== */
QListWidget {
    background-color: white;
    border: 2px solid %21;
    border-radius: %22px;
    padding: %17px;
}

QListWidget::item {
    background-color: transparent;
    color: %2;
    padding: %8px %18px;
    border-radius: %16px;
    margin: 2px 0;
}

QListWidget::item:hover {
    background-color: %13;
}

QListWidget::item:selected {
    background-color: %6;
    color: white;
}

/* ===== Tooltip ===== */
QToolTip {
    background-color: %26;
    color: white;
    border: none;
    border-radius: %16px;
    padding: %17px %18px;
    font-size: %27pt;
}

/* ===== Progress Bar ===== */
QProgressBar {
    background-color: %11;
    border: 1px solid %5;
    border-radius: %7px;
    height: 8px;
    text-align: center;
}

QProgressBar::chunk {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                stop:0 %6, stop:1 %28);
    border-radius: %16px;
}

/* ===== Checkboxes and Radio Buttons ===== */
QCheckBox, QRadioButton {
    spacing: %8px;
    color: %2;
}

QCheckBox::indicator, QRadioButton::indicator {
    width: 20px;
    height: 20px;
    border: 2px solid %5;
    background-color: white;
}

QCheckBox::indicator {
    border-radius: %16px;
}

QRadioButton::indicator {
    border-radius: 10px;
}

QCheckBox::indicator:hover, QRadioButton::indicator:hover {
    border-color: %6;
}

QCheckBox::indicator:checked, QRadioButton::indicator:checked {
    background-color: %6;
    border-color: %6;
}

QCheckBox::indicator:checked {
    image: none;
}

/* ===== Splitter ===== */
QSplitter::handle {
    background-color: %21;
}

QSplitter::handle:horizontal {
    width: 2px;
}

QSplitter::handle:vertical {
    height: 2px;
}

QSplitter::handle:hover {
    background-color: %6;
}
    )")
    .arg(Colors::BgPrimary.name())              // %1
    .arg(Colors::FgPrimary.name())              // %2
    .arg(Typography::PrimaryFont)               // %3
    .arg(Typography::FontSizeBase)              // %4
    .arg(Colors::BorderMedium.name())           // %5
    .arg(Colors::AccentPrimary.name())          // %6
    .arg(Layout::RadiusBase)                    // %7
    .arg(Layout::ButtonPaddingV)                // %8
    .arg(Layout::ButtonPaddingH)                // %9
    .arg(Layout::ButtonMinHeight)               // %10
    .arg(Colors::BgTertiary.name())             // %11
    .arg(Colors::FgTertiary.name())             // %12
    .arg(Colors::BgSecondary.name())            // %13
    .arg(Colors::Error.name())                  // %14
    .arg(Colors::FgSecondary.name())            // %15
    .arg(Layout::RadiusSM)                      // %16
    .arg(Layout::Spacing2)                      // %17
    .arg(Layout::Spacing2)                      // %18
    .arg(Typography::MonoFont)                  // %19
    .arg(Colors::BorderDark.name())             // %20
    .arg(Colors::BorderLight.name())            // %21
    .arg(Layout::RadiusMD)                      // %22
    .arg(Typography::FontSizeMD)                // %23
    .arg(Layout::Spacing3)                      // %24
    .arg(Layout::Spacing3)                      // %25
    .arg(Colors::FgPrimary.name())              // %26 - Tooltip bg
    .arg(Typography::FontSizeSM)                // %27
    .arg(Colors::AccentSecondary.name());       // %28
}

} // namespace StyleSheet
