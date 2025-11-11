#include "colorbar.h"
#include <QVBoxLayout>
#include <QLabel>

ColorBar::ColorBar(QWidget *parent, PaletteType palette)
    : QWidget(parent)
    , m_currentColor(Qt::black)
    , m_selectedButton(nullptr)
{
    setWindowTitle("Colors");

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Create palette widget
    QWidget *paletteWidget = new QWidget();
    m_layout = new QGridLayout(paletteWidget);
    m_layout->setSpacing(6);
    m_layout->setContentsMargins(12, 12, 12, 12);

    createPalette(palette);

    // Wire up scroll area
    scrollArea->setWidget(paletteWidget);
    mainLayout->addWidget(scrollArea);

    setMinimumWidth(200);  // Account for scrollbar
    setMinimumHeight(200);
}

void ColorBar::createPalette(PaletteType palette)
{
    QList<QPair<QColor, QString>> colors;

    switch (palette) {
        case Pastels:
            colors = getPastelColors();
            break;
        case Markers:
            colors = getMarkerColors();
            break;
    }

    int row = 0;
    int col = 0;
    const int maxCols = 3;

    for (const auto &colorPair : colors) {
        addColorButton(colorPair.first, colorPair.second);
        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }
}

void ColorBar::addColorButton(const QColor &color, const QString &name)
{
    QPushButton *button = new QPushButton(this);
    button->setFixedSize(44, 44);
    button->setToolTip(name);
    button->setProperty("color", color);

    // Just set the background color using a property; border styling handled by global stylesheet
    QString styleSheet = QString("background-color: %1;").arg(color.name());
    button->setStyleSheet(styleSheet);

    connect(button, &QPushButton::clicked, this, &ColorBar::onColorButtonClicked);

    m_colorButtons.append(button);

    // Calculate grid position
    int index = m_colorButtons.size() - 1;
    int row = index / 3;
    int col = index % 3;
    m_layout->addWidget(button, row, col);
}

void ColorBar::onColorButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QColor color = button->property("color").value<QColor>();
    setCurrentColor(color);
    emit colorSelected(color);
}

void ColorBar::setCurrentColor(const QColor &color)
{
    m_currentColor = color;

    // Update button appearance to show selection
    for (QPushButton *btn : m_colorButtons) {
        QColor btnColor = btn->property("color").value<QColor>();
        if (btnColor == color) {
            // Selected button gets thicker border (handled by setting property)
            QString styleSheet = QString("background-color: %1; border: 3px solid #4482B4;").arg(btnColor.name());
            btn->setStyleSheet(styleSheet);
            m_selectedButton = btn;
        } else {
            // Normal buttons just get background color
            QString styleSheet = QString("background-color: %1;").arg(btnColor.name());
            btn->setStyleSheet(styleSheet);
        }
    }
}

QList<QPair<QColor, QString>> ColorBar::getPastelColors()
{
    // Pantone pastel palette
    return {
        // Pinks
        {QColor("#F7CAC9"), "Rose Quartz"},
        {QColor("#FADADD"), "Pink Lavender"},
        {QColor("#FCCDE2"), "Orchid Smoke"},

        // Peaches
        {QColor("#FFD1A9"), "Peach Nougat"},
        {QColor("#FFE5CC"), "Apricot Ice"},
        {QColor("#F8D5B0"), "Bellini"},

        // Yellows
        {QColor("#FAE8C8"), "Lemon Chiffon"},
        {QColor("#FFF5BA"), "Pastel Yellow"},
        {QColor("#FFFACD"), "Lemonade"},

        // Greens
        {QColor("#D5F4E6"), "Aqua Glass"},
        {QColor("#C7DDCC"), "Pastel Mint"},
        {QColor("#88D8B0"), "Ocean Wave"},

        // Blues
        {QColor("#B0E0E6"), "Powder Blue"},
        {QColor("#D4E4F7"), "Serenity"},
        {QColor("#A2D5F2"), "Chambray Blue"},

        // Purples
        {QColor("#E6CEE6"), "Lilac Breeze"},
        {QColor("#D5C9DF"), "Orchid Hush"},
        {QColor("#CDBEE3"), "Lavender Fog"},

        // Neutrals
        {QColor("#F5F5DC"), "Beige"},
        {QColor("#E8E8E8"), "Vapor"},
        {QColor("#FFFFFF"), "White"},
        {QColor("#000000"), "Black"}
    };
}

QList<QPair<QColor, QString>> ColorBar::getMarkerColors()
{
    // Professional marker colors - vibrant, saturated palette for illustration
    return {
        // Reds & Pinks
        {QColor("#E60012"), "Lipstick Red"},
        {QColor("#C3272B"), "Cardinal Red"},
        {QColor("#FF6B9D"), "Hot Pink"},

        // Oranges & Corals
        {QColor("#FF6600"), "Vibrant Orange"},
        {QColor("#FF9E1B"), "Marigold"},
        {QColor("#FF7F50"), "Coral"},

        // Yellows & Golds
        {QColor("#FFD700"), "Golden Yellow"},
        {QColor("#FFF200"), "Lemon Yellow"},
        {QColor("#F4C430"), "Saffron"},

        // Greens
        {QColor("#00A86B"), "Jade Green"},
        {QColor("#7CB342"), "Fresh Green"},
        {QColor("#009B77"), "Emerald"},

        // Blues
        {QColor("#0066CC"), "Brilliant Blue"},
        {QColor("#00A8E1"), "Cyan Blue"},
        {QColor("#003DA5"), "Royal Blue"},

        // Purples & Violets
        {QColor("#9B59B6"), "Amethyst"},
        {QColor("#6A1B9A"), "Deep Purple"},
        {QColor("#8E44AD"), "Violet"},

        // Browns & Warm Neutrals
        {QColor("#8B4513"), "Saddle Brown"},
        {QColor("#D2691E"), "Burnt Sienna"},
        {QColor("#CD853F"), "Peru"},

        // Grays & Neutrals
        {QColor("#5A5A5A"), "Cool Gray 9"},
        {QColor("#9E9E9E"), "Medium Gray"},
        {QColor("#FFFFFF"), "White"},
        {QColor("#000000"), "Black"}
    };
}
