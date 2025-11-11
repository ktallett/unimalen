#include "toolbar.h"
#include <QToolButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QScrollArea>
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
    // Create main layout for this widget
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setMinimumWidth(160);  // Account for scrollbar width

    // Create content widget for the buttons
    QWidget *contentWidget = new QWidget();
    contentWidget->setMinimumWidth(140);

    m_layout = new QGridLayout(contentWidget);
    m_layout->setContentsMargins(12, 12, 12, 12);
    m_layout->setHorizontalSpacing(24);  // Extra horizontal spacing between columns
    m_layout->setVerticalSpacing(12);    // Vertical spacing between rows
    m_layout->setAlignment(Qt::AlignTop);

    m_pencilButton = new QToolButton(contentWidget);
    m_pencilButton->setFixedSize(44, 44);
    m_pencilButton->setCheckable(true);
    m_pencilButton->setChecked(true);

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
    m_pencilButton->setToolTip(tr("Pencil - Draw thin freehand lines"));

    connect(m_pencilButton, &QToolButton::clicked, this, &ToolBar::onPencilClicked);

    m_textButton = new QToolButton(contentWidget);
    m_textButton->setFixedSize(44, 44);
    m_textButton->setCheckable(true);

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
    m_textButton->setToolTip(tr("Text - Add text to your canvas"));

    connect(m_textButton, &QToolButton::clicked, this, &ToolBar::onTextClicked);

    // Create spray paint tool with menu
    m_sprayButton = new QToolButton(contentWidget);
    m_sprayButton->setFixedSize(44, 44);
    m_sprayButton->setCheckable(true);

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
    m_sprayButton->setToolTip(tr("Spray - Spray paint effect"));

    m_currentSpraySize = 20; // Default to medium

    connect(m_sprayButton, &QToolButton::clicked, this, &ToolBar::onSprayClicked);

    // Create paintbrush tool with menu
    m_brushButton = new QToolButton(contentWidget);
    m_brushButton->setFixedSize(44, 44);
    m_brushButton->setCheckable(true);

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
    m_brushButton->setToolTip(tr("Brush - Paint with brush strokes"));

    connect(m_brushButton, &QToolButton::clicked, this, &ToolBar::onBrushClicked);

    // Create marker tool
    m_markerButton = new QToolButton(contentWidget);
    m_markerButton->setFixedSize(44, 44);
    m_markerButton->setCheckable(true);

    // Create marker icon
    QPixmap markerIcon(50, 50);
    markerIcon.fill(Qt::transparent);
    QPainter markerPainter(&markerIcon);
    markerPainter.setRenderHint(QPainter::Antialiasing);

    // Draw marker body (cylindrical shape)
    markerPainter.setPen(QPen(Qt::black, 2));
    markerPainter.setBrush(QBrush(QColor(255, 100, 50))); // Orange marker
    markerPainter.drawRoundedRect(20, 10, 10, 28, 2, 2);

    // Draw marker cap
    markerPainter.setBrush(QBrush(QColor(200, 80, 40))); // Darker orange cap
    markerPainter.drawRoundedRect(20, 10, 10, 10, 2, 2);

    // Draw marker tip
    markerPainter.setPen(QPen(Qt::black, 2));
    markerPainter.setBrush(QBrush(QColor(100, 100, 100))); // Gray tip
    QPolygon tipPolygon;
    tipPolygon << QPoint(23, 38) << QPoint(27, 38) << QPoint(25, 43);
    markerPainter.drawPolygon(tipPolygon);

    // Draw a marker stroke to show it draws
    markerPainter.setPen(QPen(QColor(255, 100, 50, 180), 4, Qt::SolidLine, Qt::RoundCap));
    markerPainter.drawLine(8, 18, 16, 26);

    m_markerButton->setIcon(QIcon(markerIcon));
    m_markerButton->setIconSize(QSize(40, 40));
    m_markerButton->setToolTip(tr("Marker - Draw with semi-transparent marker strokes"));

    connect(m_markerButton, &QToolButton::clicked, this, &ToolBar::onMarkerClicked);

    // Create eraser tool with menu
    m_eraserButton = new QToolButton(contentWidget);
    m_eraserButton->setFixedSize(44, 44);
    m_eraserButton->setCheckable(true);

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
    m_eraserButton->setToolTip(tr("Eraser - Erase parts of your drawing"));

    connect(m_eraserButton, &QToolButton::clicked, this, &ToolBar::onEraserClicked);

    // Create line tool
    m_lineButton = new QToolButton(contentWidget);
    m_lineButton->setFixedSize(44, 44);
    m_lineButton->setCheckable(true);

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
    m_lineButton->setToolTip(tr("Line - Draw straight lines"));

    connect(m_lineButton, &QToolButton::clicked, this, &ToolBar::onLineClicked);

    // Create bezier curve tool
    m_bezierButton = new QToolButton(contentWidget);
    m_bezierButton->setFixedSize(44, 44);
    m_bezierButton->setCheckable(true);

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
    m_bezierButton->setToolTip(tr("Bezier - Draw smooth curves"));

    connect(m_bezierButton, &QToolButton::clicked, this, &ToolBar::onBezierClicked);

    // Create scissors tool
    m_scissorsButton = new QToolButton(contentWidget);
    m_scissorsButton->setFixedSize(44, 44);
    m_scissorsButton->setCheckable(true);

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
    m_scissorsButton->setToolTip(tr("Scissors - Cut out selections"));

    connect(m_scissorsButton, &QToolButton::clicked, this, &ToolBar::onScissorsClicked);

    // Create fill tool
    m_fillButton = new QToolButton(contentWidget);
    m_fillButton->setFixedSize(44, 44);
    m_fillButton->setCheckable(true);

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
    m_fillButton->setToolTip(tr("Fill - Fill enclosed areas with color"));

    connect(m_fillButton, &QToolButton::clicked, this, &ToolBar::onFillClicked);

    // Create lasso tool
    m_lassoButton = new QToolButton(contentWidget);
    m_lassoButton->setFixedSize(44, 44);
    m_lassoButton->setCheckable(true);

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
    m_lassoButton->setToolTip(tr("Lasso - Select irregular areas"));

    connect(m_lassoButton, &QToolButton::clicked, this, &ToolBar::onLassoClicked);

    // Create rectangular selection tool
    m_rectSelectButton = new QToolButton(contentWidget);
    m_rectSelectButton->setFixedSize(44, 44);
    m_rectSelectButton->setCheckable(true);

    // Create rectangular selection icon
    QPixmap rectSelectIcon(50, 50);
    rectSelectIcon.fill(Qt::transparent);
    QPainter rectSelectPainter(&rectSelectIcon);
    rectSelectPainter.setRenderHint(QPainter::Antialiasing);

    // Draw dashed rectangle for selection
    rectSelectPainter.setPen(QPen(Qt::black, 2, Qt::DashLine));
    rectSelectPainter.setBrush(Qt::NoBrush);
    rectSelectPainter.drawRect(12, 12, 26, 26);

    // Draw small selection handles at corners
    rectSelectPainter.setPen(QPen(Qt::black, 1));
    rectSelectPainter.setBrush(QBrush(Qt::white));
    rectSelectPainter.drawRect(10, 10, 4, 4);  // Top-left
    rectSelectPainter.drawRect(36, 10, 4, 4);  // Top-right
    rectSelectPainter.drawRect(10, 36, 4, 4);  // Bottom-left
    rectSelectPainter.drawRect(36, 36, 4, 4);  // Bottom-right

    m_rectSelectButton->setIcon(QIcon(rectSelectIcon));
    m_rectSelectButton->setIconSize(QSize(40, 40));
    m_rectSelectButton->setToolTip(tr("Rectangle Select - Select rectangular areas"));

    connect(m_rectSelectButton, &QToolButton::clicked, this, &ToolBar::onRectSelectClicked);

    // Create eyedropper/color picker tool
    m_eyedropperButton = new QToolButton(contentWidget);
    m_eyedropperButton->setFixedSize(44, 44);
    m_eyedropperButton->setCheckable(true);

    // Create eyedropper icon
    QPixmap eyedropperIcon(50, 50);
    eyedropperIcon.fill(Qt::transparent);
    QPainter eyedropperPainter(&eyedropperIcon);
    eyedropperPainter.setRenderHint(QPainter::Antialiasing);

    // Draw eyedropper tool
    eyedropperPainter.setPen(QPen(Qt::black, 2));
    eyedropperPainter.setBrush(Qt::NoBrush);

    // Dropper bulb
    eyedropperPainter.drawEllipse(28, 10, 10, 10);

    // Dropper stem
    QPainterPath dropperPath;
    dropperPath.moveTo(28, 15);
    dropperPath.lineTo(15, 28);
    dropperPath.lineTo(18, 31);
    dropperPath.lineTo(31, 18);
    dropperPath.lineTo(28, 15);
    eyedropperPainter.drawPath(dropperPath);

    // Dropper tip
    eyedropperPainter.drawLine(15, 28, 10, 33);
    eyedropperPainter.drawLine(18, 31, 13, 36);

    // Color dot being picked up
    eyedropperPainter.setBrush(QBrush(Qt::blue));
    eyedropperPainter.drawEllipse(8, 34, 6, 6);

    m_eyedropperButton->setIcon(QIcon(eyedropperIcon));
    m_eyedropperButton->setIconSize(QSize(40, 40));
    m_eyedropperButton->setToolTip(tr("Eyedropper - Pick colors from the canvas"));

    connect(m_eyedropperButton, &QToolButton::clicked, this, &ToolBar::onEyedropperClicked);

    // Create square outline tool
    m_squareButton = new QToolButton(contentWidget);
    m_squareButton->setFixedSize(44, 44);
    m_squareButton->setCheckable(true);

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
    m_squareButton->setToolTip(tr("Rectangle - Draw rectangle outlines"));

    connect(m_squareButton, &QToolButton::clicked, this, &ToolBar::onSquareClicked);

    // Create filled square tool
    m_filledSquareButton = new QToolButton(contentWidget);
    m_filledSquareButton->setFixedSize(44, 44);
    m_filledSquareButton->setCheckable(true);

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
    m_filledSquareButton->setToolTip(tr("Filled Rectangle - Draw filled rectangles"));

    connect(m_filledSquareButton, &QToolButton::clicked, this, &ToolBar::onFilledSquareClicked);

    // Create rounded square tool
    m_roundedSquareButton = new QToolButton(contentWidget);
    m_roundedSquareButton->setFixedSize(44, 44);
    m_roundedSquareButton->setCheckable(true);

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
    m_roundedSquareButton->setToolTip(tr("Rounded Rectangle - Draw rounded corner rectangles"));

    connect(m_roundedSquareButton, &QToolButton::clicked, this, &ToolBar::onRoundedSquareClicked);

    // Create filled rounded square tool
    m_filledRoundedSquareButton = new QToolButton(contentWidget);
    m_filledRoundedSquareButton->setFixedSize(44, 44);
    m_filledRoundedSquareButton->setCheckable(true);

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
    m_filledRoundedSquareButton->setToolTip(tr("Filled Rounded Rectangle - Draw filled rounded corner rectangles"));

    connect(m_filledRoundedSquareButton, &QToolButton::clicked, this, &ToolBar::onFilledRoundedSquareClicked);

    // Create oval tool
    m_ovalButton = new QToolButton(contentWidget);
    m_ovalButton->setFixedSize(44, 44);
    m_ovalButton->setCheckable(true);

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
    m_ovalButton->setToolTip(tr("Ellipse - Draw ellipse outlines"));

    connect(m_ovalButton, &QToolButton::clicked, this, &ToolBar::onOvalClicked);

    // Create filled oval tool
    m_filledOvalButton = new QToolButton(contentWidget);
    m_filledOvalButton->setFixedSize(44, 44);
    m_filledOvalButton->setCheckable(true);

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
    m_filledOvalButton->setToolTip(tr("Filled Ellipse - Draw filled ellipses"));

    connect(m_filledOvalButton, &QToolButton::clicked, this, &ToolBar::onFilledOvalClicked);

    // Arrange tools in grid - now 9 rows for 19 tools
    m_layout->addWidget(m_pencilButton, 0, 0);
    m_layout->addWidget(m_textButton, 0, 1);
    m_layout->addWidget(m_sprayButton, 1, 0);
    m_layout->addWidget(m_brushButton, 1, 1);
    m_layout->addWidget(m_markerButton, 2, 0);
    m_layout->addWidget(m_eraserButton, 2, 1);
    m_layout->addWidget(m_lineButton, 3, 0);
    m_layout->addWidget(m_fillButton, 3, 1);
    m_layout->addWidget(m_lassoButton, 4, 0);
    m_layout->addWidget(m_rectSelectButton, 4, 1);
    m_layout->addWidget(m_eyedropperButton, 5, 0);
    m_layout->addWidget(m_squareButton, 5, 1);
    m_layout->addWidget(m_filledSquareButton, 6, 0);
    m_layout->addWidget(m_roundedSquareButton, 6, 1);
    m_layout->addWidget(m_filledRoundedSquareButton, 7, 0);
    m_layout->addWidget(m_ovalButton, 7, 1);
    m_layout->addWidget(m_filledOvalButton, 8, 0);
    m_layout->addWidget(m_bezierButton, 8, 1);
    m_layout->addWidget(m_scissorsButton, 9, 0);

    // Wire up scroll area
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
    emit sprayToolSelected(m_currentSpraySize);
}


void ToolBar::onBrushClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(true);
    m_markerButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
    emit brushToolSelected(m_currentBrushSize);
}

void ToolBar::onMarkerClicked()
{
    m_pencilButton->setChecked(false);
    m_textButton->setChecked(false);
    m_sprayButton->setChecked(false);
    m_brushButton->setChecked(false);
    m_markerButton->setChecked(true);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
    emit markerToolSelected();
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(false);
    emit filledOvalToolSelected();
}

void ToolBar::onRectSelectClicked()
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
    m_rectSelectButton->setChecked(true);
    m_eyedropperButton->setChecked(false);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit rectSelectToolSelected();
}

void ToolBar::onEyedropperClicked()
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
    m_rectSelectButton->setChecked(false);
    m_eyedropperButton->setChecked(true);
    m_squareButton->setChecked(false);
    m_filledSquareButton->setChecked(false);
    m_roundedSquareButton->setChecked(false);
    m_filledRoundedSquareButton->setChecked(false);
    m_ovalButton->setChecked(false);
    m_filledOvalButton->setChecked(false);
    emit eyedropperToolSelected();
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