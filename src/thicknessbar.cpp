#include "thicknessbar.h"
#include <QPainter>
#include <QPixmap>
#include <QIcon>
#include <QScreen>
#include <QApplication>

ThicknessBar::ThicknessBar(QWidget *parent)
    : QWidget(parent)
    , m_currentThickness(Medium)
{
    setFixedHeight(50);
    setStyleSheet("ThicknessBar { background-color: #f0f0f0; border: 1px solid #ccc; }");

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(5, 5, 5, 5);
    m_layout->setSpacing(3);

    // Create thickness buttons
    createThicknessButton(&m_dottedButton, "Dotted Line", Dotted);
    createThicknessButton(&m_thinButton, "Thin Line", Thin);
    createThicknessButton(&m_mediumButton, "Medium Line", Medium);
    createThicknessButton(&m_thickButton, "Thick Line", Thick);
    createThicknessButton(&m_extraThickButton, "Extra Thick Line", ExtraThick);

    // Connect signals
    connect(m_dottedButton, &QToolButton::clicked, this, &ThicknessBar::onDottedClicked);
    connect(m_thinButton, &QToolButton::clicked, this, &ThicknessBar::onThinClicked);
    connect(m_mediumButton, &QToolButton::clicked, this, &ThicknessBar::onMediumClicked);
    connect(m_thickButton, &QToolButton::clicked, this, &ThicknessBar::onThickClicked);
    connect(m_extraThickButton, &QToolButton::clicked, this, &ThicknessBar::onExtraThickClicked);

    // Set medium as default selected
    m_mediumButton->setChecked(true);
}

void ThicknessBar::createThicknessButton(QToolButton **button, const QString &tooltip, ThicknessType type)
{
    *button = new QToolButton(this);
    (*button)->setFixedSize(38, 38);
    (*button)->setCheckable(true);
    (*button)->setToolTip(tooltip);
    (*button)->setStyleSheet(
        "QToolButton { "
        "border: 1px solid #ccc; "
        "border-radius: 3px; "
        "background-color: white; "
        "} "
        "QToolButton:checked { "
        "background-color: #d0d0d0; "
        "border: 2px solid #666; "
        "} "
        "QToolButton:hover { "
        "background-color: #f5f5f5; "
        "}"
    );

    QPixmap icon = createThicknessIcon(type);
    (*button)->setIcon(QIcon(icon));
    (*button)->setIconSize(QSize(32, 32));

    m_layout->addWidget(*button);
}

QPixmap ThicknessBar::createThicknessIcon(ThicknessType type)
{
    QPixmap icon(36, 36);
    icon.fill(Qt::transparent);
    QPainter painter(&icon);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw line based on thickness type
    QPen pen(Qt::black);

    switch (type) {
        case Dotted:
            pen.setWidth(2);
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
            painter.drawLine(6, 18, 30, 18);
            break;

        case Thin:
            pen.setWidth(1);
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(6, 18, 30, 18);
            break;

        case Medium:
            pen.setWidth(3);
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(6, 18, 30, 18);
            break;

        case Thick:
            pen.setWidth(5);
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(6, 18, 30, 18);
            break;

        case ExtraThick:
            pen.setWidth(8);
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
            painter.drawLine(6, 18, 30, 18);
            break;
    }

    return icon;
}

void ThicknessBar::onDottedClicked()
{
    m_dottedButton->setChecked(true);
    m_thinButton->setChecked(false);
    m_mediumButton->setChecked(false);
    m_thickButton->setChecked(false);
    m_extraThickButton->setChecked(false);
    m_currentThickness = Dotted;
    emit thicknessSelected(Dotted);
}

void ThicknessBar::onThinClicked()
{
    m_dottedButton->setChecked(false);
    m_thinButton->setChecked(true);
    m_mediumButton->setChecked(false);
    m_thickButton->setChecked(false);
    m_extraThickButton->setChecked(false);
    m_currentThickness = Thin;
    emit thicknessSelected(Thin);
}

void ThicknessBar::onMediumClicked()
{
    m_dottedButton->setChecked(false);
    m_thinButton->setChecked(false);
    m_mediumButton->setChecked(true);
    m_thickButton->setChecked(false);
    m_extraThickButton->setChecked(false);
    m_currentThickness = Medium;
    emit thicknessSelected(Medium);
}

void ThicknessBar::onThickClicked()
{
    m_dottedButton->setChecked(false);
    m_thinButton->setChecked(false);
    m_mediumButton->setChecked(false);
    m_thickButton->setChecked(true);
    m_extraThickButton->setChecked(false);
    m_currentThickness = Thick;
    emit thicknessSelected(Thick);
}

void ThicknessBar::onExtraThickClicked()
{
    m_dottedButton->setChecked(false);
    m_thinButton->setChecked(false);
    m_mediumButton->setChecked(false);
    m_thickButton->setChecked(false);
    m_extraThickButton->setChecked(true);
    m_currentThickness = ExtraThick;
    emit thicknessSelected(ExtraThick);
}

void ThicknessBar::showAsFloatingWindow()
{
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Line Thickness"));
    show();

    // Position relative to parent window if it exists
    if (parentWidget()) {
        QRect parentRect = parentWidget()->geometry();
        int spacing = 10;
        int x = parentRect.right() + spacing;  // Right of parent with margin
        int y = parentRect.top() + 50;  // Top margin from parent
        move(x, y);
    } else {
        // Fallback to old positioning
        QScreen *screen = QApplication::primaryScreen();
        if (screen) {
            QRect screenGeometry = screen->availableGeometry();
            int x = screenGeometry.width() - width() - 10;  // Right edge with margin
            int y = 50;  // Top margin
            move(x, y);
        } else {
            move(1000, 50);  // Fallback position
        }
    }

    raise();
    activateWindow();
}

void ThicknessBar::closeEvent(QCloseEvent *event)
{
    emit thicknessBarClosed();
    QWidget::closeEvent(event);
}