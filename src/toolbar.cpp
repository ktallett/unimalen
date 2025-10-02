#include "toolbar.h"
#include <QToolButton>
#include <QGridLayout>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QPainterPath>

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
    , m_currentSpraySize(20)
    , m_currentBrushSize(15)
    , m_currentEraserSize(12)
{
    setFixedWidth(120);
    setStyleSheet("QWidget { background-color: #f0f0f0; border-right: 1px solid #ccc; }");

    m_layout = new QGridLayout(this);
    m_layout->setContentsMargins(5, 5, 5, 5);
    m_layout->setSpacing(5);
    m_layout->setAlignment(Qt::AlignTop);

    m_pencilButton = new QToolButton(this);
    m_pencilButton->setFixedSize(42, 42);
    m_pencilButton->setCheckable(true);
    m_pencilButton->setChecked(true);
    m_pencilButton->setStyleSheet(
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

    QPixmap pencilIcon(50, 50);
    pencilIcon.fill(Qt::transparent);
    QPainter painter(&pencilIcon);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(10, 40, 40, 10);
    painter.setPen(QPen(Qt::darkYellow, 3));
    painter.drawLine(10, 40, 15, 35);
    painter.setPen(QPen(Qt::lightGray, 2));
    painter.drawLine(35, 15, 40, 10);

    m_pencilButton->setIcon(QIcon(pencilIcon));
    m_pencilButton->setIconSize(QSize(40, 40));

    connect(m_pencilButton, &QToolButton::clicked, this, &ToolBar::onPencilClicked);

    m_textButton = new QToolButton(this);
    m_textButton->setFixedSize(42, 42);
    m_textButton->setCheckable(true);
    m_textButton->setStyleSheet(
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

    QPixmap textIcon(50, 50);
    textIcon.fill(Qt::transparent);
    QPainter textPainter(&textIcon);
    textPainter.setRenderHint(QPainter::Antialiasing);
    QFont font("Arial", 16, QFont::Bold);
    textPainter.setFont(font);
    textPainter.setPen(Qt::black);
    textPainter.drawText(QRect(5, 5, 40, 40), Qt::AlignCenter, "A");

    m_textButton->setIcon(QIcon(textIcon));
    m_textButton->setIconSize(QSize(40, 40));

    connect(m_textButton, &QToolButton::clicked, this, &ToolBar::onTextClicked);

    // Create spray paint tool with menu
    m_sprayButton = new QToolButton(this);
    m_sprayButton->setFixedSize(42, 42);
    m_sprayButton->setCheckable(true);
    m_sprayButton->setStyleSheet(
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

    // Create spray can icon
    QPixmap sprayCanIcon(50, 50);
    sprayCanIcon.fill(Qt::transparent);
    QPainter sprayCanPainter(&sprayCanIcon);
    sprayCanPainter.setRenderHint(QPainter::Antialiasing);

    // Draw spray can body
    sprayCanPainter.setPen(QPen(Qt::black, 2));
    sprayCanPainter.setBrush(QBrush(Qt::lightGray));
    sprayCanPainter.drawRoundedRect(15, 20, 12, 25, 2, 2);

    // Draw spray can top
    sprayCanPainter.setBrush(QBrush(Qt::darkGray));
    sprayCanPainter.drawRect(18, 15, 6, 8);

    // Draw spray nozzle
    sprayCanPainter.setPen(QPen(Qt::black, 1));
    sprayCanPainter.drawRect(20, 12, 2, 3);

    // Draw spray pattern
    sprayCanPainter.setPen(QPen(Qt::black, 1));
    for (int i = 0; i < 12; ++i) {
        int x = 30 + (rand() % 15);
        int y = 10 + (rand() % 15);
        sprayCanPainter.drawPoint(x, y);
    }

    m_sprayButton->setIcon(QIcon(sprayCanIcon));
    m_sprayButton->setIconSize(QSize(40, 40));

    m_currentSpraySize = 20; // Default to medium

    connect(m_sprayButton, &QToolButton::clicked, this, &ToolBar::onSprayClicked);

    // Create paintbrush tool with menu
    m_brushButton = new QToolButton(this);
    m_brushButton->setFixedSize(42, 42);
    m_brushButton->setCheckable(true);
    m_brushButton->setStyleSheet(
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

    // Create paintbrush icon
    QPixmap brushIcon(50, 50);
    brushIcon.fill(Qt::transparent);
    QPainter brushPainter(&brushIcon);
    brushPainter.setRenderHint(QPainter::Antialiasing);

    // Draw brush handle
    brushPainter.setPen(QPen(Qt::darkYellow, 3));
    brushPainter.drawLine(35, 35, 45, 45);

    // Draw brush ferrule (metal part)
    brushPainter.setPen(QPen(Qt::darkGray, 2));
    brushPainter.drawLine(30, 30, 35, 35);

    // Draw brush bristles
    brushPainter.setPen(QPen(Qt::black, 1));
    for (int i = 0; i < 8; ++i) {
        int x1 = 25 + (i % 3);
        int y1 = 25 + (i / 3);
        int x2 = 30 + (i % 3);
        int y2 = 30 + (i / 3);
        brushPainter.drawLine(x1, y1, x2, y2);
    }

    // Draw paint stroke
    brushPainter.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap));
    brushPainter.drawLine(10, 15, 20, 25);

    m_brushButton->setIcon(QIcon(brushIcon));
    m_brushButton->setIconSize(QSize(40, 40));

    connect(m_brushButton, &QToolButton::clicked, this, &ToolBar::onBrushClicked);

    // Create eraser tool with menu
    m_eraserButton = new QToolButton(this);
    m_eraserButton->setFixedSize(42, 42);
    m_eraserButton->setCheckable(true);
    m_eraserButton->setStyleSheet(
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

    // Create eraser icon
    QPixmap eraserIcon(50, 50);
    eraserIcon.fill(Qt::transparent);
    QPainter eraserPainter(&eraserIcon);
    eraserPainter.setRenderHint(QPainter::Antialiasing);

    // Draw eraser body (pink rectangle)
    eraserPainter.setPen(QPen(Qt::black, 2));
    eraserPainter.setBrush(QBrush(QColor(255, 182, 193))); // Light pink
    eraserPainter.drawRoundedRect(18, 15, 14, 25, 2, 2);

    // Draw metal ferrule
    eraserPainter.setBrush(QBrush(Qt::darkGray));
    eraserPainter.drawRect(18, 38, 14, 4);

    // Draw eraser marks (showing it erasing)
    eraserPainter.setPen(QPen(Qt::lightGray, 2));
    eraserPainter.drawLine(8, 12, 15, 18);
    eraserPainter.drawLine(10, 8, 17, 14);
    eraserPainter.drawLine(12, 4, 19, 10);

    m_eraserButton->setIcon(QIcon(eraserIcon));
    m_eraserButton->setIconSize(QSize(40, 40));

    connect(m_eraserButton, &QToolButton::clicked, this, &ToolBar::onEraserClicked);

    // Create line tool
    m_lineButton = new QToolButton(this);
    m_lineButton->setFixedSize(42, 42);
    m_lineButton->setCheckable(true);
    m_lineButton->setStyleSheet(
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

    // Create line icon
    QPixmap lineIcon(50, 50);
    lineIcon.fill(Qt::transparent);
    QPainter linePainter(&lineIcon);
    linePainter.setRenderHint(QPainter::Antialiasing);

    // Draw line icon - diagonal line with endpoints
    linePainter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
    linePainter.drawLine(10, 40, 40, 10);

    // Draw small circles at endpoints to indicate precise line drawing
    linePainter.setPen(QPen(Qt::black, 2));
    linePainter.setBrush(QBrush(Qt::black));
    linePainter.drawEllipse(8, 38, 4, 4);
    linePainter.drawEllipse(38, 8, 4, 4);

    m_lineButton->setIcon(QIcon(lineIcon));
    m_lineButton->setIconSize(QSize(40, 40));

    connect(m_lineButton, &QToolButton::clicked, this, &ToolBar::onLineClicked);

    // Create bezier curve tool
    m_bezierButton = new QToolButton(this);
    m_bezierButton->setFixedSize(42, 42);
    m_bezierButton->setCheckable(true);
    m_bezierButton->setStyleSheet(
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

    // Create bezier curve icon
    QPixmap bezierIcon(50, 50);
    bezierIcon.fill(Qt::transparent);
    QPainter bezierPainter(&bezierIcon);
    bezierPainter.setRenderHint(QPainter::Antialiasing);

    // Draw bezier curve icon with control points
    QPainterPath bezierPath;
    bezierPath.moveTo(10, 40);
    bezierPath.cubicTo(15, 10, 35, 10, 40, 40);

    bezierPainter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
    bezierPainter.drawPath(bezierPath);

    // Draw control points
    bezierPainter.setPen(QPen(Qt::gray, 1, Qt::DotLine));
    bezierPainter.drawLine(10, 40, 15, 10);
    bezierPainter.drawLine(40, 40, 35, 10);

    // Draw small circles at control points
    bezierPainter.setPen(QPen(Qt::blue, 2));
    bezierPainter.setBrush(QBrush(Qt::blue));
    bezierPainter.drawEllipse(13, 8, 4, 4);
    bezierPainter.drawEllipse(33, 8, 4, 4);

    m_bezierButton->setIcon(QIcon(bezierIcon));
    m_bezierButton->setIconSize(QSize(40, 40));

    connect(m_bezierButton, &QToolButton::clicked, this, &ToolBar::onBezierClicked);

    // Create scissors tool
    m_scissorsButton = new QToolButton(this);
    m_scissorsButton->setFixedSize(42, 42);
    m_scissorsButton->setCheckable(true);
    m_scissorsButton->setStyleSheet(
        "QToolButton { "
        "border: 1px solid #ccc; "
        "border-radius: 3px; "
        "background-color: white; "
        "} "
        "QToolButton:checked { "
        "background-color: #d0d0d0; "
        "border: 1px solid #666; "
        "} "
        "QToolButton:hover { "
        "background-color: #e8e8e8; "
        "}"
    );

    // Create scissors icon
    QPixmap scissorsIcon(50, 50);
    scissorsIcon.fill(Qt::transparent);
    QPainter scissorsPainter(&scissorsIcon);
    scissorsPainter.setRenderHint(QPainter::Antialiasing);

    // Draw scissors - two blades crossing
    scissorsPainter.setPen(QPen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap));

    // Left blade
    scissorsPainter.drawLine(15, 35, 30, 20);
    scissorsPainter.drawEllipse(12, 32, 6, 6);

    // Right blade
    scissorsPainter.drawLine(35, 35, 30, 20);
    scissorsPainter.drawEllipse(32, 32, 6, 6);

    // Pivot point
    scissorsPainter.setPen(QPen(Qt::black, 2));
    scissorsPainter.setBrush(QBrush(Qt::black));
    scissorsPainter.drawEllipse(28, 18, 4, 4);

    // Cut line (dashed red)
    scissorsPainter.setPen(QPen(Qt::red, 2, Qt::DashLine));
    scissorsPainter.drawLine(10, 10, 40, 10);

    m_scissorsButton->setIcon(QIcon(scissorsIcon));
    m_scissorsButton->setIconSize(QSize(40, 40));

    connect(m_scissorsButton, &QToolButton::clicked, this, &ToolBar::onScissorsClicked);

    // Create fill tool
    m_fillButton = new QToolButton(this);
    m_fillButton->setFixedSize(42, 42);
    m_fillButton->setCheckable(true);
    m_fillButton->setStyleSheet(
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

    // Create paint bucket icon - improved design
    QPixmap fillIcon(50, 50);
    fillIcon.fill(Qt::transparent);
    QPainter fillPainter(&fillIcon);
    fillPainter.setRenderHint(QPainter::Antialiasing);

    // Draw paint bucket body - main cylinder
    fillPainter.setPen(QPen(Qt::black, 2));
    fillPainter.setBrush(QBrush(QColor(200, 200, 200))); // Light gray

    // Bucket bottom (base)
    fillPainter.drawEllipse(16, 36, 18, 6); // Elliptical bottom for 3D effect

    // Bucket main body (cylindrical)
    fillPainter.drawRect(16, 20, 18, 16);

    // Bucket top rim
    fillPainter.setBrush(QBrush(QColor(180, 180, 180))); // Slightly darker gray
    fillPainter.drawEllipse(15, 18, 20, 6);

    // Bucket inner rim (to show depth)
    fillPainter.setBrush(QBrush(QColor(160, 160, 160))); // Darker gray
    fillPainter.drawEllipse(17, 20, 16, 4);

    // Paint inside bucket
    fillPainter.setBrush(QBrush(QColor(50, 100, 200))); // Blue paint
    fillPainter.drawEllipse(18, 22, 14, 3);

    // Paint bucket handle - more realistic wire handle
    fillPainter.setPen(QPen(Qt::darkGray, 2));
    fillPainter.setBrush(Qt::NoBrush);

    // Handle attachment points
    fillPainter.setPen(QPen(Qt::black, 1));
    fillPainter.setBrush(QBrush(Qt::darkGray));
    fillPainter.drawEllipse(32, 24, 3, 3); // Right attachment
    fillPainter.drawEllipse(15, 24, 3, 3); // Left attachment

    // Handle arc
    fillPainter.setPen(QPen(Qt::darkGray, 2));
    fillPainter.setBrush(Qt::NoBrush);
    fillPainter.drawArc(10, 18, 30, 16, 30 * 16, 120 * 16); // Curved handle

    // Paint drips/drops coming out - more dynamic
    fillPainter.setPen(QPen(QColor(50, 100, 200), 1)); // Blue paint color
    fillPainter.setBrush(QBrush(QColor(50, 100, 200)));

    // Larger drops
    fillPainter.drawEllipse(10, 8, 4, 6);   // Main drip
    fillPainter.drawEllipse(14, 12, 3, 4);  // Medium drip
    fillPainter.drawEllipse(18, 6, 2, 4);   // Small drip
    fillPainter.drawEllipse(38, 10, 3, 5);  // Side drip

    // Paint splash effect
    fillPainter.setPen(QPen(QColor(50, 100, 200), 1));
    fillPainter.drawPoint(8, 14);
    fillPainter.drawPoint(20, 4);
    fillPainter.drawPoint(40, 8);
    fillPainter.drawPoint(6, 10);

    m_fillButton->setIcon(QIcon(fillIcon));
    m_fillButton->setIconSize(QSize(40, 40));

    connect(m_fillButton, &QToolButton::clicked, this, &ToolBar::onFillClicked);

    // Create lasso tool
    m_lassoButton = new QToolButton(this);
    m_lassoButton->setFixedSize(42, 42);
    m_lassoButton->setCheckable(true);
    m_lassoButton->setStyleSheet(
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

    // Create lasso icon
    QPixmap lassoIcon(50, 50);
    lassoIcon.fill(Qt::transparent);
    QPainter lassoPainter(&lassoIcon);
    lassoPainter.setRenderHint(QPainter::Antialiasing);

    // Draw lasso selection outline (curved irregular shape)
    lassoPainter.setPen(QPen(Qt::black, 2, Qt::DashLine));
    lassoPainter.setBrush(Qt::NoBrush);

    // Create an irregular curved selection shape
    QPainterPath lassoPath;
    lassoPath.moveTo(15, 20);
    lassoPath.cubicTo(10, 15, 20, 10, 30, 15);
    lassoPath.cubicTo(40, 12, 42, 25, 35, 30);
    lassoPath.cubicTo(32, 35, 25, 38, 18, 35);
    lassoPath.cubicTo(12, 32, 10, 25, 15, 20);
    lassoPainter.drawPath(lassoPath);

    // Draw small selection handles
    lassoPainter.setPen(QPen(Qt::black, 1));
    lassoPainter.setBrush(QBrush(Qt::white));
    lassoPainter.drawRect(13, 18, 4, 4);
    lassoPainter.drawRect(28, 13, 4, 4);
    lassoPainter.drawRect(33, 28, 4, 4);

    m_lassoButton->setIcon(QIcon(lassoIcon));
    m_lassoButton->setIconSize(QSize(40, 40));

    connect(m_lassoButton, &QToolButton::clicked, this, &ToolBar::onLassoClicked);

    // Create square outline tool
    m_squareButton = new QToolButton(this);
    m_squareButton->setFixedSize(42, 42);
    m_squareButton->setCheckable(true);
    m_squareButton->setStyleSheet(
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

    // Create square outline icon
    QPixmap squareIcon(50, 50);
    squareIcon.fill(Qt::transparent);
    QPainter squarePainter(&squareIcon);
    squarePainter.setRenderHint(QPainter::Antialiasing);

    // Draw square outline
    squarePainter.setPen(QPen(Qt::black, 2));
    squarePainter.setBrush(Qt::NoBrush);
    squarePainter.drawRect(15, 15, 20, 20);

    m_squareButton->setIcon(QIcon(squareIcon));
    m_squareButton->setIconSize(QSize(40, 40));

    connect(m_squareButton, &QToolButton::clicked, this, &ToolBar::onSquareClicked);

    // Create filled square tool
    m_filledSquareButton = new QToolButton(this);
    m_filledSquareButton->setFixedSize(42, 42);
    m_filledSquareButton->setCheckable(true);
    m_filledSquareButton->setStyleSheet(
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

    // Create filled square icon
    QPixmap filledSquareIcon(50, 50);
    filledSquareIcon.fill(Qt::transparent);
    QPainter filledSquarePainter(&filledSquareIcon);
    filledSquarePainter.setRenderHint(QPainter::Antialiasing);

    // Draw filled square
    filledSquarePainter.setPen(QPen(Qt::black, 2));
    filledSquarePainter.setBrush(QBrush(Qt::black));
    filledSquarePainter.drawRect(15, 15, 20, 20);

    m_filledSquareButton->setIcon(QIcon(filledSquareIcon));
    m_filledSquareButton->setIconSize(QSize(40, 40));

    connect(m_filledSquareButton, &QToolButton::clicked, this, &ToolBar::onFilledSquareClicked);

    // Create rounded square tool
    m_roundedSquareButton = new QToolButton(this);
    m_roundedSquareButton->setFixedSize(42, 42);
    m_roundedSquareButton->setCheckable(true);
    m_roundedSquareButton->setStyleSheet(
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

    // Create rounded square outline icon
    QPixmap roundedSquareIcon(50, 50);
    roundedSquareIcon.fill(Qt::transparent);
    QPainter roundedSquarePainter(&roundedSquareIcon);
    roundedSquarePainter.setRenderHint(QPainter::Antialiasing);

    // Draw rounded square outline
    roundedSquarePainter.setPen(QPen(Qt::black, 2));
    roundedSquarePainter.setBrush(Qt::NoBrush);
    roundedSquarePainter.drawRoundedRect(15, 15, 20, 20, 4, 4);

    m_roundedSquareButton->setIcon(QIcon(roundedSquareIcon));
    m_roundedSquareButton->setIconSize(QSize(40, 40));

    connect(m_roundedSquareButton, &QToolButton::clicked, this, &ToolBar::onRoundedSquareClicked);

    // Create filled rounded square tool
    m_filledRoundedSquareButton = new QToolButton(this);
    m_filledRoundedSquareButton->setFixedSize(42, 42);
    m_filledRoundedSquareButton->setCheckable(true);
    m_filledRoundedSquareButton->setStyleSheet(
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

    // Create filled rounded square icon
    QPixmap filledRoundedSquareIcon(50, 50);
    filledRoundedSquareIcon.fill(Qt::transparent);
    QPainter filledRoundedSquarePainter(&filledRoundedSquareIcon);
    filledRoundedSquarePainter.setRenderHint(QPainter::Antialiasing);

    // Draw filled rounded square
    filledRoundedSquarePainter.setPen(QPen(Qt::black, 2));
    filledRoundedSquarePainter.setBrush(QBrush(Qt::black));
    filledRoundedSquarePainter.drawRoundedRect(15, 15, 20, 20, 4, 4);

    m_filledRoundedSquareButton->setIcon(QIcon(filledRoundedSquareIcon));
    m_filledRoundedSquareButton->setIconSize(QSize(40, 40));

    connect(m_filledRoundedSquareButton, &QToolButton::clicked, this, &ToolBar::onFilledRoundedSquareClicked);

    // Create oval tool
    m_ovalButton = new QToolButton(this);
    m_ovalButton->setFixedSize(42, 42);
    m_ovalButton->setCheckable(true);
    m_ovalButton->setStyleSheet(
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

    // Create oval outline icon
    QPixmap ovalIcon(50, 50);
    ovalIcon.fill(Qt::transparent);
    QPainter ovalPainter(&ovalIcon);
    ovalPainter.setRenderHint(QPainter::Antialiasing);

    // Draw oval outline
    ovalPainter.setPen(QPen(Qt::black, 2));
    ovalPainter.setBrush(Qt::NoBrush);
    ovalPainter.drawEllipse(15, 15, 20, 20);

    m_ovalButton->setIcon(QIcon(ovalIcon));
    m_ovalButton->setIconSize(QSize(40, 40));

    connect(m_ovalButton, &QToolButton::clicked, this, &ToolBar::onOvalClicked);

    // Create filled oval tool
    m_filledOvalButton = new QToolButton(this);
    m_filledOvalButton->setFixedSize(42, 42);
    m_filledOvalButton->setCheckable(true);
    m_filledOvalButton->setStyleSheet(
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

    // Create filled oval icon
    QPixmap filledOvalIcon(50, 50);
    filledOvalIcon.fill(Qt::transparent);
    QPainter filledOvalPainter(&filledOvalIcon);
    filledOvalPainter.setRenderHint(QPainter::Antialiasing);

    // Draw filled oval
    filledOvalPainter.setPen(QPen(Qt::black, 2));
    filledOvalPainter.setBrush(QBrush(Qt::black));
    filledOvalPainter.drawEllipse(15, 15, 20, 20);

    m_filledOvalButton->setIcon(QIcon(filledOvalIcon));
    m_filledOvalButton->setIconSize(QSize(40, 40));

    connect(m_filledOvalButton, &QToolButton::clicked, this, &ToolBar::onFilledOvalClicked);

    // Arrange tools in grid - now 7 rows for 14 tools
    m_layout->addWidget(m_pencilButton, 0, 0);
    m_layout->addWidget(m_textButton, 0, 1);
    m_layout->addWidget(m_sprayButton, 1, 0);
    m_layout->addWidget(m_brushButton, 1, 1);
    m_layout->addWidget(m_eraserButton, 2, 0);
    m_layout->addWidget(m_lineButton, 2, 1);
    m_layout->addWidget(m_fillButton, 3, 0);
    m_layout->addWidget(m_lassoButton, 3, 1);
    m_layout->addWidget(m_squareButton, 4, 0);
    m_layout->addWidget(m_filledSquareButton, 4, 1);
    m_layout->addWidget(m_roundedSquareButton, 5, 0);
    m_layout->addWidget(m_filledRoundedSquareButton, 5, 1);
    m_layout->addWidget(m_ovalButton, 6, 0);
    m_layout->addWidget(m_filledOvalButton, 6, 1);
    m_layout->addWidget(m_bezierButton, 7, 0);
    m_layout->addWidget(m_scissorsButton, 7, 1);
}

void ToolBar::onPencilClicked()
{
    m_pencilButton->setChecked(true);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_scissorsButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit pencilToolSelected();
}

void ToolBar::onTextClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(true);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit textToolSelected();
}

void ToolBar::onSprayClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(true);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit sprayToolSelected(m_currentSpraySize);
}


void ToolBar::onBrushClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(true);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit brushToolSelected(m_currentBrushSize);
}


void ToolBar::onEraserClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(true);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit eraserToolSelected(m_currentEraserSize);
}


void ToolBar::onLineClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(true);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit lineToolSelected();
}

void ToolBar::onBezierClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(true);
    m_scissorsButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit bezierToolSelected();
}

void ToolBar::onScissorsClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_scissorsButton->setChecked(true);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit scissorsToolSelected();
}

void ToolBar::onFillClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_fillButton->setChecked(true);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit fillToolSelected();
}

void ToolBar::onLassoClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(true);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit lassoToolSelected();
}

void ToolBar::onSquareClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(true);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit squareToolSelected();
}

void ToolBar::onFilledSquareClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(true);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    emit filledSquareToolSelected();
}

void ToolBar::onRoundedSquareClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(true);
    m_filledRoundedSquareButton->setChecked(false);
    emit roundedSquareToolSelected();
}

void ToolBar::onFilledRoundedSquareClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(true);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit filledRoundedSquareToolSelected();
}

void ToolBar::onOvalClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(true);
    m_filledOvalButton->setChecked(false);
    emit ovalToolSelected();
}

void ToolBar::onFilledOvalClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_eraserButton->setChecked(false);
    m_lineButton->setChecked(false);
    m_bezierButton->setChecked(false);
    m_fillButton->setChecked(false);
    m_lassoButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(true);
    emit filledOvalToolSelected();
}

void ToolBar::showAsFloatingWindow()
{
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Tools"));
    show();

    // Position relative to parent window if it exists
    if (parentWidget()) {
        QRect parentRect = parentWidget()->geometry();
        int spacing = 10;
        move(parentRect.left() - width() - spacing, parentRect.top() + 50);
    } else {
        // Fallback to old positioning
        move(10, 50);
    }
    raise();
    activateWindow();
}

void ToolBar::closeEvent(QCloseEvent *event)
{
    emit toolBarClosed();
    QWidget::closeEvent(event);
}