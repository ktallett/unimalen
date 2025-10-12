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
        case CopicPastels:
            colors = getCopicPastelColors();
            break;
        case PantonePastels:
            colors = getPantonePastelColors();
            break;
        case CustomPastels:
            colors = getCustomPastelColors();
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

QList<QPair<QColor, QString>> ColorBar::getCopicPastelColors()
{
    // Copic marker pastel colors - popular choices for illustration
    return {
        // Pinks
        {QColor("#FFD6E0"), "Light Pink (RV10)"},
        {QColor("#FFBDD4"), "Baby Pink (RV21)"},
        {QColor("#FFA8C5"), "Salmon Pink (R20)"},

        // Peaches/Corals
        {QColor("#FFD4C4"), "Peach (YR20)"},
        {QColor("#FFCDB8"), "Coral (YR21)"},
        {QColor("#FFE5D9"), "Light Peach (YR00)"},

        // Yellows
        {QColor("#FFF4CC"), "Pale Yellow (Y11)"},
        {QColor("#FFECB3"), "Cream (Y21)"},
        {QColor("#FFF9E3"), "Egg Shell (YR30)"},

        // Greens
        {QColor("#D4EDD1"), "Pale Green (G20)"},
        {QColor("#C8E6C9"), "Mint Green (G21)"},
        {QColor("#B8DFC8"), "Seafoam (BG10)"},

        // Blues
        {QColor("#D6E9F5"), "Baby Blue (B21)"},
        {QColor("#C1DEF1"), "Sky Blue (B23)"},
        {QColor("#E3F2FD"), "Ice Blue (B00)"},

        // Purples/Lavenders
        {QColor("#E6D9ED"), "Lavender (V12)"},
        {QColor("#DFD3E8"), "Lilac (V15)"},
        {QColor("#F3E5F5"), "Pale Lavender (V04)"},

        // Neutrals
        {QColor("#F5F5F5"), "Cool Gray 1 (C1)"},
        {QColor("#FFF8F0"), "Warm Gray 1 (W1)"},
        {QColor("#FFFFFF"), "White"},
        {QColor("#000000"), "Black"}
    };
}

QList<QPair<QColor, QString>> ColorBar::getPantonePastelColors()
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

QList<QPair<QColor, QString>> ColorBar::getCustomPastelColors()
{
    // Custom curated pastel palette
    return {
        // Warm pastels
        {QColor("#FFD5D5"), "Blush"},
        {QColor("#FFE4D5"), "Peach Cream"},
        {QColor("#FFFAD5"), "Butter"},

        // Cool pastels
        {QColor("#D5FFD5"), "Mint Cream"},
        {QColor("#D5F0FF"), "Baby Blue"},
        {QColor("#E5D5FF"), "Mauve"},

        // Vibrant pastels
        {QColor("#FFB3D9"), "Bubblegum"},
        {QColor("#FFD9B3"), "Apricot"},
        {QColor("#FFFFB3"), "Vanilla"},

        // Nature pastels
        {QColor("#B3FFB3"), "Pistachio"},
        {QColor("#B3E5FF"), "Sky"},
        {QColor("#D9B3FF"), "Wisteria"},

        // Muted pastels
        {QColor("#E8D5C4"), "Sand"},
        {QColor("#D5E8E8"), "Mist"},
        {QColor("#E8D5E8"), "Dusty Rose"},

        // Neutrals
        {QColor("#F0F0F0"), "Pearl"},
        {QColor("#E0E0E0"), "Silver"},
        {QColor("#FFFFFF"), "White"},
        {QColor("#000000"), "Black"}
    };
}
