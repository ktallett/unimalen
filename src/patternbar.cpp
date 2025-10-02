#include "patternbar.h"
#include <QToolButton>
#include <QGridLayout>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QPainterPath>
#include <QScreen>
#include <QApplication>

PatternBar::PatternBar(QWidget *parent)
    : QWidget(parent)
    , m_currentPattern(Solid)
{
    setFixedHeight(110); // Height for two rows
    setStyleSheet("QWidget { background-color: #f0f0f0; border-top: 1px solid #ccc; }");

    m_layout = new QGridLayout(this);
    m_layout->setContentsMargins(5, 5, 5, 5);
    m_layout->setSpacing(5);
    m_layout->setAlignment(Qt::AlignLeft);

    // Create pattern buttons in 2x13 grid
    // Top row (row 0) - Basic and density patterns
    createPatternButton(Solid, Qt::SolidPattern, "Solid", 0, 0);
    createPatternButton(Dense1, Qt::Dense1Pattern, "Dense 1", 0, 1);
    createPatternButton(Dense2, Qt::Dense2Pattern, "Dense 2", 0, 2);
    createPatternButton(Dense3, Qt::Dense3Pattern, "Dense 3", 0, 3);
    createPatternButton(Dense4, Qt::Dense4Pattern, "Dense 4", 0, 4);
    createPatternButton(Dense5, Qt::Dense5Pattern, "Dense 5", 0, 5);
    createPatternButton(Dense6, Qt::Dense6Pattern, "Dense 6", 0, 6);
    createPatternButton(Dense7, Qt::Dense7Pattern, "Dense 7", 0, 7);
    createPatternButton(Horizontal, Qt::HorPattern, "Horizontal Lines", 0, 8);
    createPatternButton(Vertical, Qt::VerPattern, "Vertical Lines", 0, 9);
    createPatternButton(Cross, Qt::CrossPattern, "Cross Hatch", 0, 10);
    createPatternButton(BDiag, Qt::BDiagPattern, "Backward Diagonal", 0, 11);
    createPatternButton(FDiag, Qt::FDiagPattern, "Forward Diagonal", 0, 12);

    // Bottom row (row 1) - Diagonal and custom textured patterns
    createPatternButton(DiagCross, Qt::DiagCrossPattern, "Diagonal Cross", 1, 0);
    createPatternButton(Dots, Qt::SolidPattern, "Dots", 1, 1);
    createPatternButton(Grid, Qt::SolidPattern, "Grid", 1, 2);
    createPatternButton(Circles, Qt::SolidPattern, "Circles", 1, 3);
    createPatternButton(Waves, Qt::SolidPattern, "Waves", 1, 4);
    createPatternButton(Stars, Qt::SolidPattern, "Stars", 1, 5);
    createPatternButton(Brick, Qt::SolidPattern, "Brick", 1, 6);
    createPatternButton(Hexagons, Qt::SolidPattern, "Hexagons", 1, 7);
    createPatternButton(Scales, Qt::SolidPattern, "Scales", 1, 8);
    createPatternButton(Zigzag, Qt::SolidPattern, "Zigzag", 1, 9);
    createPatternButton(Checkerboard, Qt::SolidPattern, "Checkerboard", 1, 10);
    createPatternButton(Triangles, Qt::SolidPattern, "Triangles", 1, 11);
    createPatternButton(Noise, Qt::SolidPattern, "Noise", 1, 12);

    // Set solid as default selected
    m_solidButton->setChecked(true);
}

void PatternBar::createPatternButton(PatternType type, Qt::BrushStyle style, const QString &tooltip, int row, int col)
{
    QToolButton *button = new QToolButton(this);
    button->setFixedSize(48, 48);
    button->setCheckable(true);
    button->setToolTip(tooltip);
    button->setStyleSheet(
        "QToolButton { "
        "border: 1px solid #ccc; "
        "border-radius: 3px; "
        "background-color: white; "
        "} "
        "QToolButton:checked { "
        "background-color: #d0d0d0; "
        "border: 2px solid #666; "
        "}"
    );

    // Create pattern icon
    QPixmap patternIcon(40, 40);
    patternIcon.fill(Qt::white);
    QPainter painter(&patternIcon);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw pattern sample
    if (type >= Dots) {
        // Custom texture patterns
        drawCustomPattern(painter, type, 2, 2, 36, 36);
    } else {
        // Standard Qt patterns
        QBrush patternBrush(Qt::black, style);
        painter.setBrush(patternBrush);
        painter.setPen(Qt::NoPen);
        painter.drawRect(2, 2, 36, 36);
    }

    // Add border
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(2, 2, 36, 36);

    button->setIcon(QIcon(patternIcon));
    button->setIconSize(QSize(40, 40));

    // Store button reference and connect signal
    switch (type) {
        case Solid:
            m_solidButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onSolidClicked);
            break;
        case Dense1:
            m_dense1Button = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onDense1Clicked);
            break;
        case Dense2:
            m_dense2Button = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onDense2Clicked);
            break;
        case Dense3:
            m_dense3Button = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onDense3Clicked);
            break;
        case Dense4:
            m_dense4Button = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onDense4Clicked);
            break;
        case Dense5:
            m_dense5Button = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onDense5Clicked);
            break;
        case Dense6:
            m_dense6Button = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onDense6Clicked);
            break;
        case Dense7:
            m_dense7Button = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onDense7Clicked);
            break;
        case Horizontal:
            m_horizontalButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onHorizontalClicked);
            break;
        case Vertical:
            m_verticalButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onVerticalClicked);
            break;
        case Cross:
            m_crossButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onCrossClicked);
            break;
        case BDiag:
            m_bdiagButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onBDiagClicked);
            break;
        case FDiag:
            m_fdiagButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onFDiagClicked);
            break;
        case DiagCross:
            m_diagCrossButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onDiagCrossClicked);
            break;
        case Dots:
            m_dotsButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onDotsClicked);
            break;
        case Grid:
            m_gridButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onGridClicked);
            break;
        case Circles:
            m_circlesButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onCirclesClicked);
            break;
        case Waves:
            m_wavesButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onWavesClicked);
            break;
        case Stars:
            m_starsButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onStarsClicked);
            break;
        case Brick:
            m_brickButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onBrickClicked);
            break;
        case Hexagons:
            m_hexagonsButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onHexagonsClicked);
            break;
        case Scales:
            m_scalesButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onScalesClicked);
            break;
        case Zigzag:
            m_zigzagButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onZigzagClicked);
            break;
        case Checkerboard:
            m_checkerboardButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onCheckerboardClicked);
            break;
        case Triangles:
            m_trianglesButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onTrianglesClicked);
            break;
        case Noise:
            m_noiseButton = button;
            connect(button, &QToolButton::clicked, this, &PatternBar::onNoiseClicked);
            break;
    }

    m_layout->addWidget(button, row, col);
}

void PatternBar::drawCustomPattern(QPainter &painter, PatternType type, int x, int y, int width, int height)
{
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::black, 1));

    switch (type) {
        case Dots:
            // Draw small dots in a grid pattern
            for (int i = x + 4; i < x + width - 2; i += 6) {
                for (int j = y + 4; j < y + height - 2; j += 6) {
                    painter.setBrush(Qt::black);
                    painter.setPen(Qt::NoPen);
                    painter.drawEllipse(i, j, 2, 2);
                }
            }
            break;

        case Grid:
            // Draw grid lines
            painter.setPen(QPen(Qt::black, 1));
            for (int i = x; i <= x + width; i += 6) {
                painter.drawLine(i, y, i, y + height);
            }
            for (int j = y; j <= y + height; j += 6) {
                painter.drawLine(x, j, x + width, j);
            }
            break;

        case Circles:
            // Draw small circles in a grid pattern
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(Qt::black, 1));
            for (int i = x + 3; i < x + width - 3; i += 8) {
                for (int j = y + 3; j < y + height - 3; j += 8) {
                    painter.drawEllipse(i, j, 6, 6);
                }
            }
            break;

        case Waves:
            // Draw wavy lines
            painter.setPen(QPen(Qt::black, 1));
            for (int j = y + 4; j < y + height; j += 8) {
                QPainterPath wave;
                wave.moveTo(x, j);
                for (int i = x; i < x + width; i += 4) {
                    wave.quadTo(i + 2, j + ((i / 4) % 2 == 0 ? -3 : 3), i + 4, j);
                }
                painter.drawPath(wave);
            }
            break;

        case Stars:
            // Draw small star shapes
            painter.setBrush(Qt::black);
            painter.setPen(Qt::NoPen);
            for (int i = x + 6; i < x + width - 6; i += 12) {
                for (int j = y + 6; j < y + height - 6; j += 12) {
                    // Draw a simple 4-pointed star
                    painter.drawRect(i - 1, j - 3, 2, 6);
                    painter.drawRect(i - 3, j - 1, 6, 2);
                }
            }
            break;

        case Brick:
        {
            // Draw brick pattern
            painter.setPen(QPen(Qt::black, 1));
            bool offset = false;
            for (int j = y; j < y + height; j += 8) {
                int startX = offset ? x - 6 : x;
                for (int i = startX; i < x + width; i += 12) {
                    painter.drawRect(i, j, 12, 8);
                }
                offset = !offset;
            }
            break;
        }

        case Hexagons:
        {
            // Draw hexagon pattern
            painter.setPen(QPen(Qt::black, 1));
            painter.setBrush(Qt::NoBrush);
            for (int j = y + 4; j < y + height - 4; j += 12) {
                for (int i = x + 6; i < x + width - 6; i += 14) {
                    int offsetY = ((i - x) / 14) % 2 == 0 ? 0 : 6;
                    QPolygon hexagon;
                    hexagon << QPoint(i, j + offsetY + 3)
                            << QPoint(i + 3, j + offsetY)
                            << QPoint(i + 9, j + offsetY)
                            << QPoint(i + 12, j + offsetY + 3)
                            << QPoint(i + 9, j + offsetY + 6)
                            << QPoint(i + 3, j + offsetY + 6);
                    painter.drawPolygon(hexagon);
                }
            }
            break;
        }

        case Scales:
        {
            // Draw fish scale pattern
            painter.setPen(QPen(Qt::black, 1));
            painter.setBrush(Qt::NoBrush);
            for (int j = y; j < y + height; j += 6) {
                for (int i = x; i < x + width; i += 8) {
                    int offsetX = (j / 6) % 2 == 0 ? 0 : 4;
                    painter.drawArc(i + offsetX - 4, j - 3, 8, 6, 0, 180 * 16);
                }
            }
            break;
        }

        case Zigzag:
        {
            // Draw zigzag pattern
            painter.setPen(QPen(Qt::black, 1));
            for (int j = y + 6; j < y + height; j += 12) {
                QPainterPath zigzag;
                zigzag.moveTo(x, j);
                for (int i = x; i < x + width; i += 6) {
                    zigzag.lineTo(i + 3, j + ((i / 6) % 2 == 0 ? -3 : 3));
                    zigzag.lineTo(i + 6, j);
                }
                painter.drawPath(zigzag);
            }
            break;
        }

        case Checkerboard:
        {
            // Draw checkerboard pattern
            painter.setPen(Qt::NoPen);
            painter.setBrush(Qt::black);
            for (int j = y; j < y + height; j += 6) {
                for (int i = x; i < x + width; i += 6) {
                    if ((i / 6 + j / 6) % 2 == 0) {
                        painter.drawRect(i, j, 6, 6);
                    }
                }
            }
            break;
        }

        case Triangles:
        {
            // Draw triangle pattern
            painter.setPen(QPen(Qt::black, 1));
            painter.setBrush(Qt::NoBrush);
            for (int j = y + 8; j < y + height; j += 10) {
                for (int i = x + 5; i < x + width - 5; i += 10) {
                    QPolygon triangle;
                    triangle << QPoint(i, j - 6)
                             << QPoint(i - 4, j + 2)
                             << QPoint(i + 4, j + 2);
                    painter.drawPolygon(triangle);
                }
            }
            break;
        }

        case Noise:
        {
            // Draw random noise pattern
            painter.setPen(Qt::NoPen);
            painter.setBrush(Qt::black);
            // Simple pseudo-random pattern based on position
            for (int j = y; j < y + height; j += 2) {
                for (int i = x; i < x + width; i += 2) {
                    // Simple hash-like function for pseudo-random
                    int hash = ((i * 73) + (j * 37)) % 100;
                    if (hash < 25) {
                        painter.drawRect(i, j, 1, 1);
                    }
                }
            }
            break;
        }

        default:
            break;
    }
}

void PatternBar::onSolidClicked()
{
    // Uncheck all other buttons
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_solidButton->setChecked(true);
    m_currentPattern = Solid;
    emit patternSelected(Solid);
}

void PatternBar::onDense1Clicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_dense1Button->setChecked(true);
    m_currentPattern = Dense1;
    emit patternSelected(Dense1);
}

void PatternBar::onDense2Clicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_dense2Button->setChecked(true);
    m_currentPattern = Dense2;
    emit patternSelected(Dense2);
}

void PatternBar::onDense3Clicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_dense3Button->setChecked(true);
    m_currentPattern = Dense3;
    emit patternSelected(Dense3);
}

void PatternBar::onDense4Clicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_dense4Button->setChecked(true);
    m_currentPattern = Dense4;
    emit patternSelected(Dense4);
}

void PatternBar::onDense5Clicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_dense5Button->setChecked(true);
    m_currentPattern = Dense5;
    emit patternSelected(Dense5);
}

void PatternBar::onDense6Clicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_dense6Button->setChecked(true);
    m_currentPattern = Dense6;
    emit patternSelected(Dense6);
}

void PatternBar::onDense7Clicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_dense7Button->setChecked(true);
    m_currentPattern = Dense7;
    emit patternSelected(Dense7);
}

void PatternBar::onHorizontalClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_horizontalButton->setChecked(true);
    m_currentPattern = Horizontal;
    emit patternSelected(Horizontal);
}

void PatternBar::onVerticalClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_verticalButton->setChecked(true);
    m_currentPattern = Vertical;
    emit patternSelected(Vertical);
}

void PatternBar::onCrossClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_crossButton->setChecked(true);
    m_currentPattern = Cross;
    emit patternSelected(Cross);
}

void PatternBar::onBDiagClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_bdiagButton->setChecked(true);
    m_currentPattern = BDiag;
    emit patternSelected(BDiag);
}

void PatternBar::onFDiagClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_fdiagButton->setChecked(true);
    m_currentPattern = FDiag;
    emit patternSelected(FDiag);
}

void PatternBar::onDiagCrossClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_diagCrossButton->setChecked(true);
    m_currentPattern = DiagCross;
    emit patternSelected(DiagCross);
}

void PatternBar::onDotsClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);

    m_dotsButton->setChecked(true);
    m_currentPattern = Dots;
    emit patternSelected(Dots);
}

void PatternBar::onGridClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);

    m_gridButton->setChecked(true);
    m_currentPattern = Grid;
    emit patternSelected(Grid);
}

void PatternBar::onCirclesClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);

    m_circlesButton->setChecked(true);
    m_currentPattern = Circles;
    emit patternSelected(Circles);
}

void PatternBar::onWavesClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);

    m_wavesButton->setChecked(true);
    m_currentPattern = Waves;
    emit patternSelected(Waves);
}

void PatternBar::onStarsClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_brickButton->setChecked(false);

    m_starsButton->setChecked(true);
    m_currentPattern = Stars;
    emit patternSelected(Stars);
}

void PatternBar::onBrickClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);

    m_brickButton->setChecked(true);
    m_currentPattern = Brick;
    emit patternSelected(Brick);
}

void PatternBar::onHexagonsClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_hexagonsButton->setChecked(true);
    m_currentPattern = Hexagons;
    emit patternSelected(Hexagons);
}

void PatternBar::onScalesClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_scalesButton->setChecked(true);
    m_currentPattern = Scales;
    emit patternSelected(Scales);
}

void PatternBar::onZigzagClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_zigzagButton->setChecked(true);
    m_currentPattern = Zigzag;
    emit patternSelected(Zigzag);
}

void PatternBar::onCheckerboardClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_trianglesButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_checkerboardButton->setChecked(true);
    m_currentPattern = Checkerboard;
    emit patternSelected(Checkerboard);
}

void PatternBar::onTrianglesClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_noiseButton->setChecked(false);

    m_trianglesButton->setChecked(true);
    m_currentPattern = Triangles;
    emit patternSelected(Triangles);
}

void PatternBar::onNoiseClicked()
{
    // Uncheck all other buttons
    m_solidButton->setChecked(false);
    m_dense1Button->setChecked(false);
    m_dense2Button->setChecked(false);
    m_dense3Button->setChecked(false);
    m_dense4Button->setChecked(false);
    m_dense5Button->setChecked(false);
    m_dense6Button->setChecked(false);
    m_dense7Button->setChecked(false);
    m_horizontalButton->setChecked(false);
    m_verticalButton->setChecked(false);
    m_crossButton->setChecked(false);
    m_bdiagButton->setChecked(false);
    m_fdiagButton->setChecked(false);
    m_diagCrossButton->setChecked(false);
    m_dotsButton->setChecked(false);
    m_gridButton->setChecked(false);
    m_circlesButton->setChecked(false);
    m_wavesButton->setChecked(false);
    m_starsButton->setChecked(false);
    m_brickButton->setChecked(false);
    m_hexagonsButton->setChecked(false);
    m_scalesButton->setChecked(false);
    m_zigzagButton->setChecked(false);
    m_checkerboardButton->setChecked(false);
    m_trianglesButton->setChecked(false);

    m_noiseButton->setChecked(true);
    m_currentPattern = Noise;
    emit patternSelected(Noise);
}

void PatternBar::showAsFloatingWindow()
{
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Patterns"));
    show();

    // Position relative to parent window if it exists
    if (parentWidget()) {
        QRect parentRect = parentWidget()->geometry();
        int spacing = 10;
        int x = parentRect.left() + (parentRect.width() - width()) / 2;  // Center horizontally
        int y = parentRect.bottom() + spacing;  // Below parent with margin
        move(x, y);
    } else {
        // Fallback to old positioning
        QScreen *screen = QApplication::primaryScreen();
        if (screen) {
            QRect screenGeometry = screen->availableGeometry();
            int x = (screenGeometry.width() - width()) / 2;  // Center horizontally
            int y = screenGeometry.height() - height() - 50;  // Bottom edge with margin
            move(x, y);
        } else {
            move(300, 600);  // Fallback position
        }
    }

    raise();
    activateWindow();
}

void PatternBar::closeEvent(QCloseEvent *event)
{
    emit patternBarClosed();
    QWidget::closeEvent(event);
}