#include "canvas.h"
#include <QPaintEvent>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QFontMetrics>
#include <QDir>
#include <QTemporaryDir>
#include <QProcess>
#include <QFileInfo>
#include <QTextStream>
#include <cmath>
#include <cstdlib>
#include <QStack>
#include <QApplication>
#include <QRegion>
#include <QMenu>
#include <QAction>
#include <QUndoStack>
#include <QUndoCommand>

// Undo command for canvas operations
class CanvasUndoCommand : public QUndoCommand
{
public:
    CanvasUndoCommand(Canvas *canvas, const QPixmap &oldPixmap, const QPixmap &newPixmap, const QString &text = "Edit")
        : m_canvas(canvas), m_oldPixmap(oldPixmap), m_newPixmap(newPixmap)
    {
        setText(text);
    }

    void undo() override
    {
        m_canvas->setCanvasPixmap(m_oldPixmap);
    }

    void redo() override
    {
        m_canvas->setCanvasPixmap(m_newPixmap);
    }

private:
    Canvas *m_canvas;
    QPixmap m_oldPixmap;
    QPixmap m_newPixmap;
};

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , m_document(new Document())
    , m_scaleFactor(1)
    , m_drawing(false)
    , m_pixelZoomMode(false)
    , m_pixelCursor(288, 360)  // Center of canvas
    , m_pixelZoomFactor(32)
    , m_zoomCenter(288, 360)  // Center of canvas
    , m_magnifierRadius(150)
    , m_magnifierPosition(400, 200)
    , m_drawingInMagnifier(false)
    , m_showCoordinates(false)
    , m_mousePosition(0, 0)
    , m_pencilMode(true)
    , m_textMode(false)
    , m_sprayMode(false)
    , m_brushMode(false)
    , m_eraserMode(false)
    , m_lineMode(false)
    , m_fillMode(false)
    , m_lassoMode(false)
    , m_squareMode(false)
    , m_filledSquareMode(false)
    , m_roundedSquareMode(false)
    , m_filledRoundedSquareMode(false)
    , m_ovalMode(false)
    , m_filledOvalMode(false)
    , m_bezierMode(false)
    , m_scissorsMode(false)
    , m_bezierClickCount(0)
    , m_sprayDiameter(10)
    , m_brushDiameter(15)
    , m_eraserDiameter(12)
    , m_showLinePreview(false)
    , m_showSquarePreview(false)
    , m_showRoundedSquarePreview(false)
    , m_showOvalPreview(false)
    , m_drawingScissors(false)
    , m_hasScissorsPieces(false)
    , m_draggingPiece1(false)
    , m_draggingPiece2(false)
    , m_hasSelection(false)
    , m_drawingLasso(false)
    , m_draggingSelection(false)
    , m_textEdit(nullptr)
    , m_textMovable(false)
    , m_movingText(false)
    , m_currentPattern(PatternBar::Solid)
    , m_currentColor(Qt::black)
    , m_undoStack(new QUndoStack(this))
{
    setAttribute(Qt::WA_StaticContents);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    // Enable input method support for international text input
    setAttribute(Qt::WA_InputMethodEnabled, true);

    // Set up font with international character support
    m_textFont = QFont("Open Sans", 12);
    // Add fallback fonts for international characters
    QStringList fontFamilies;
    fontFamilies << "Open Sans" << "Noto Sans" << "Noto Sans CJK" << "DejaVu Sans" << "Liberation Sans";
    m_textFont.setFamilies(fontFamilies);

    newCanvas();
}

void Canvas::newCanvas()
{
    // Initialize the document
    delete m_document;
    m_document = new Document();

    // Composite layers to main canvas
    compositeAllLayers();

    updateCanvasSize();
    emit layersChanged();
    emit currentLayerChanged(m_document->currentLayerIndex());
    update();
}

void Canvas::setScaleFactor(int factor)
{
    if (factor == 1 || factor == 2 || factor == 4) {
        m_scaleFactor = factor;
        updateCanvasSize();
        update();
    }
}

bool Canvas::loadCanvas(const QString &fileName)
{
    QFileInfo fileInfo(fileName);
    QString extension = fileInfo.suffix().toLower();

    bool success;
    if (extension == "ora") {
        success = m_document->loadFromORA(fileName);
    } else {
        success = m_document->loadFromPNG(fileName);
    }

    if (success) {
        compositeAllLayers();
        emit layersChanged();
        emit currentLayerChanged(m_document->currentLayerIndex());
        update();
    }

    return success;
}

bool Canvas::saveCanvas(const QString &fileName)
{
    QFileInfo fileInfo(fileName);
    QString extension = fileInfo.suffix().toLower();

    if (extension == "ora") {
        return m_document->saveAsORA(fileName);
    } else {
        return m_document->saveAsPNG(fileName);
    }
}

bool Canvas::saveAsORA(const QString &fileName)
{
    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        return false;
    }

    QString tempPath = tempDir.path();

    // Create directory structure
    QDir().mkpath(tempPath + "/data");
    QDir().mkpath(tempPath + "/Thumbnails");

    // Save the main layer as PNG
    QString layerPath = tempPath + "/data/layer1.png";
    if (!m_canvas.save(layerPath, "PNG")) {
        return false;
    }

    // Create mimetype file
    QFile mimeFile(tempPath + "/mimetype");
    if (!mimeFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    mimeFile.write("image/openraster");
    mimeFile.close();

    // Create stack.xml
    QFile stackFile(tempPath + "/stack.xml");
    if (!stackFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stackStream(&stackFile);
    stackStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    stackStream << "<image version=\"0.0.3\" w=\"" << m_document->width() << "\" h=\"" << m_document->height() << "\" xres=\"" << DPI << "\" yres=\"" << DPI << "\">\n";
    stackStream << "  <stack>\n";
    stackStream << "    <layer name=\"Layer 1\" src=\"data/layer1.png\" x=\"0\" y=\"0\" opacity=\"1.0\" visibility=\"visible\" composite-op=\"svg:src-over\"/>\n";
    stackStream << "  </stack>\n";
    stackStream << "</image>\n";
    stackFile.close();

    // Create thumbnail
    QPixmap thumbnail = m_canvas.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (!thumbnail.save(tempPath + "/Thumbnails/thumbnail.png", "PNG")) {
        return false;
    }

    // Create ZIP file using system zip command
    QProcess zipProcess;
    zipProcess.setWorkingDirectory(tempPath);

    QStringList args;
    args << "-r" << fileName << ".";

    zipProcess.start("zip", args);
    zipProcess.waitForFinished();

    return zipProcess.exitCode() == 0;
}

bool Canvas::loadFromORA(const QString &fileName)
{
    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        return false;
    }

    QString tempPath = tempDir.path();

    // Extract ZIP file using system unzip command
    QProcess unzipProcess;
    QStringList args;
    args << fileName << "-d" << tempPath;

    unzipProcess.start("unzip", args);
    unzipProcess.waitForFinished();

    if (unzipProcess.exitCode() != 0) {
        return false;
    }

    // Check if it's a valid ORA file
    QFile mimeFile(tempPath + "/mimetype");
    if (!mimeFile.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray mimeContent = mimeFile.readAll();
    mimeFile.close();

    if (mimeContent != "image/openraster") {
        return false;
    }

    // Load the first layer (simplified - just load layer1.png)
    QString layerPath = tempPath + "/data/layer1.png";
    QPixmap loadedCanvas;
    if (!loadedCanvas.load(layerPath)) {
        return false;
    }

    // Ensure the canvas is the right size
    if (loadedCanvas.size() != QSize(m_document->width(), m_document->height())) {
        QPixmap newCanvas(m_document->width(), m_document->height());
        newCanvas.fill(Qt::white);
        QPainter painter(&newCanvas);
        painter.drawPixmap(0, 0, loadedCanvas);
        m_canvas = newCanvas;
    } else {
        m_canvas = loadedCanvas;
    }

    update();
    return true;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();

    if (m_pixelZoomMode) {
        // Draw the normal canvas first (scaled)
        painter.scale(m_scaleFactor, m_scaleFactor);
        QRect canvasRect = dirtyRect;
        canvasRect.setTopLeft(canvasRect.topLeft() / m_scaleFactor);
        canvasRect.setBottomRight(canvasRect.bottomRight() / m_scaleFactor);
        painter.drawPixmap(canvasRect, m_canvas, canvasRect);
        painter.resetTransform();

        // Create circular magnifying glass overlay
        QPainterPath magnifierPath;
        magnifierPath.addEllipse(m_magnifierPosition.x() - m_magnifierRadius,
                                m_magnifierPosition.y() - m_magnifierRadius,
                                m_magnifierRadius * 2, m_magnifierRadius * 2);

        // Create a clipping region for the circular area
        painter.setClipPath(magnifierPath);

        // Fill the magnifier background with white
        painter.fillRect(m_magnifierPosition.x() - m_magnifierRadius,
                        m_magnifierPosition.y() - m_magnifierRadius,
                        m_magnifierRadius * 2, m_magnifierRadius * 2, Qt::white);

        // Draw magnified pixels in the circular area
        int visiblePixels = 9; // Show 9x9 pixel area in magnifier
        int halfVisible = visiblePixels / 2;
        int pixelSize = (m_magnifierRadius * 2) / visiblePixels;

        for (int dy = -halfVisible; dy <= halfVisible; dy++) {
            for (int dx = -halfVisible; dx <= halfVisible; dx++) {
                int pixelX = m_zoomCenter.x() + dx;
                int pixelY = m_zoomCenter.y() + dy;

                if (pixelX >= 0 && pixelX < m_document->width() && pixelY >= 0 && pixelY < m_document->height()) {
                    QColor pixelColor = m_canvas.toImage().pixelColor(pixelX, pixelY);

                    int screenX = m_magnifierPosition.x() - m_magnifierRadius + (dx + halfVisible) * pixelSize;
                    int screenY = m_magnifierPosition.y() - m_magnifierRadius + (dy + halfVisible) * pixelSize;

                    // Only draw if within circle
                    QPoint pixelCenter(screenX + pixelSize/2, screenY + pixelSize/2);
                    if (QLineF(pixelCenter, m_magnifierPosition).length() <= m_magnifierRadius - 10) {
                        painter.fillRect(screenX + 1, screenY + 1, pixelSize - 2, pixelSize - 2, pixelColor);

                        // Draw grid lines
                        painter.setPen(QPen(Qt::lightGray, 1));
                        painter.drawRect(screenX + 1, screenY + 1, pixelSize - 2, pixelSize - 2);
                    }
                }
            }
        }

        // Highlight the center pixel (current zoom center)
        int centerScreenX = m_magnifierPosition.x() - pixelSize/2;
        int centerScreenY = m_magnifierPosition.y() - pixelSize/2;
        painter.setPen(QPen(Qt::red, 2));
        painter.drawRect(centerScreenX + 1, centerScreenY + 1, pixelSize - 2, pixelSize - 2);

        // Remove clipping
        painter.setClipping(false);

        // Draw magnifying glass border
        painter.setPen(QPen(Qt::darkGray, 8));
        painter.drawEllipse(m_magnifierPosition.x() - m_magnifierRadius,
                           m_magnifierPosition.y() - m_magnifierRadius,
                           m_magnifierRadius * 2, m_magnifierRadius * 2);

        // Draw connection line from magnifier to editing area
        QPoint screenZoomCenter = QPoint(m_zoomCenter.x() * m_scaleFactor, m_zoomCenter.y() * m_scaleFactor);
        painter.setPen(QPen(Qt::darkGray, 2, Qt::DashLine));
        painter.setOpacity(0.6);

        // Calculate line start point on magnifier edge closest to zoom center
        QLineF connectionLine(m_magnifierPosition, screenZoomCenter);
        connectionLine.setLength(m_magnifierRadius);
        QPoint lineStart = connectionLine.p2().toPoint();

        painter.drawLine(lineStart, screenZoomCenter);
        painter.setOpacity(1.0);

        // Draw magnifying glass handle
        painter.setPen(QPen(QColor(139, 69, 19), 6)); // Brown handle
        int handleStartX = m_magnifierPosition.x() + m_magnifierRadius * 0.7;
        int handleStartY = m_magnifierPosition.y() + m_magnifierRadius * 0.7;
        int handleEndX = handleStartX + 30;
        int handleEndY = handleStartY + 30;
        painter.drawLine(handleStartX, handleStartY, handleEndX, handleEndY);

        return;
    }

    // Normal mode - draw current page only
    painter.scale(m_scaleFactor, m_scaleFactor);

    QRect canvasRect = dirtyRect;
    canvasRect.setTopLeft(canvasRect.topLeft() / m_scaleFactor);
    canvasRect.setBottomRight(canvasRect.bottomRight() / m_scaleFactor);

    painter.drawPixmap(canvasRect, m_canvas, canvasRect);

    // Reset transform to draw coordinate system in screen coordinates
    painter.resetTransform();

    // Draw coordinate system if enabled
    if (m_showCoordinates) {
        painter.setPen(QPen(Qt::darkGray, 1));
        painter.setFont(QFont("Arial", 8));

        // Draw rulers on the edges
        int rulerSize = 20;

        // Top ruler (X-axis)
        painter.fillRect(0, 0, width(), rulerSize, QColor(240, 240, 240));
        for (int x = 0; x < m_document->width(); x += 50) {
            int screenX = x * m_scaleFactor;
            if (screenX < width()) {
                painter.drawLine(screenX, 0, screenX, rulerSize - 5);
                painter.drawText(screenX + 2, 12, QString::number(x));
            }
        }

        // Left ruler (Y-axis)
        painter.fillRect(0, rulerSize, rulerSize, height() - rulerSize, QColor(240, 240, 240));
        for (int y = 0; y < m_document->height(); y += 50) {
            int screenY = y * m_scaleFactor + rulerSize;
            if (screenY < height()) {
                painter.drawLine(0, screenY, rulerSize - 5, screenY);
                painter.save();
                painter.translate(5, screenY - 2);
                painter.rotate(-90);
                painter.drawText(0, 0, QString::number(y));
                painter.restore();
            }
        }

        // Draw mouse position coordinates
        if (m_mousePosition.x() >= 0 && m_mousePosition.x() < m_document->width() &&
            m_mousePosition.y() >= 0 && m_mousePosition.y() < m_document->height()) {

            QString coordText = QString("X: %1, Y: %2").arg(m_mousePosition.x()).arg(m_mousePosition.y());
            QRect textRect = painter.fontMetrics().boundingRect(coordText);

            // Position coordinate display in top-right corner
            int textX = width() - textRect.width() - 10;
            int textY = 15;

            painter.fillRect(textX - 5, textY - textRect.height() - 2,
                           textRect.width() + 10, textRect.height() + 6,
                           QColor(255, 255, 255, 200));
            painter.setPen(m_currentColor);
            painter.drawText(textX, textY, coordText);
        }

        // Draw crosshair at mouse position
        if (m_mousePosition.x() >= 0 && m_mousePosition.x() < m_document->width() &&
            m_mousePosition.y() >= 0 && m_mousePosition.y() < m_document->height()) {

            int mouseScreenX = m_mousePosition.x() * m_scaleFactor;
            int mouseScreenY = m_mousePosition.y() * m_scaleFactor + rulerSize;

            painter.setPen(QPen(Qt::red, 1, Qt::DashLine));
            painter.drawLine(mouseScreenX, rulerSize, mouseScreenX, height());
            painter.drawLine(rulerSize, mouseScreenY, width(), mouseScreenY);
        }

        // Restore scale for drawing other overlays
        painter.scale(m_scaleFactor, m_scaleFactor);
    } else {
        // Restore scale for drawing overlays
        painter.scale(m_scaleFactor, m_scaleFactor);
    }

    // Draw line preview overlay
    if (m_showLinePreview && m_lineMode) {
        painter.setPen(QPen(Qt::black, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(m_lineStartPoint, m_lineCurrentPoint);
    }

    // Draw bezier curve preview overlay
    if (m_bezierMode && m_bezierClickCount > 0) {
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

        // Show clicked points
        for (int i = 0; i < m_bezierClickCount; i++) {
            painter.setBrush(Qt::blue);
            painter.drawEllipse(m_bezierPoints[i], 3, 3);
        }

        // Show construction lines for control points
        if (m_bezierClickCount >= 2) {
            painter.setPen(QPen(Qt::gray, 1, Qt::DotLine));
            painter.drawLine(m_bezierPoints[0], m_bezierPoints[1]);
        }
        if (m_bezierClickCount >= 4) {
            painter.drawLine(m_bezierPoints[2], m_bezierPoints[3]);
        }
        if (m_bezierClickCount >= 3) {
            painter.setPen(QPen(Qt::red, 2, Qt::DashLine));
            // Draw partial bezier curve preview (we'll implement this)
            if (m_bezierClickCount == 4) {
                QPainterPath path;
                path.moveTo(m_bezierPoints[0]);
                path.cubicTo(m_bezierPoints[1], m_bezierPoints[2], m_bezierPoints[3]);
                painter.drawPath(path);
            }
        }
    }

    // Draw square preview overlay
    if (m_showSquarePreview && (m_squareMode || m_filledSquareMode)) {
        painter.setPen(QPen(Qt::black, 2, Qt::DashLine));
        if (m_filledSquareMode) {
            QBrush patternBrush;
            if (isCustomPattern(m_currentPattern)) {
                patternBrush = createCustomPatternBrush(m_currentPattern);
            } else {
                Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
                patternBrush = QBrush(m_currentColor, brushStyle);
            }
            painter.setBrush(patternBrush);
        } else {
            painter.setBrush(Qt::NoBrush);
        }
        QRect squareRect(m_squareStartPoint, m_squareCurrentPoint);
        painter.drawRect(squareRect.normalized());
    }

    // Draw rounded square preview overlay
    if (m_showRoundedSquarePreview && (m_roundedSquareMode || m_filledRoundedSquareMode)) {
        painter.setPen(QPen(Qt::black, 2, Qt::DashLine));
        painter.setRenderHint(QPainter::Antialiasing);
        if (m_filledRoundedSquareMode) {
            QBrush patternBrush;
            if (isCustomPattern(m_currentPattern)) {
                patternBrush = createCustomPatternBrush(m_currentPattern);
            } else {
                Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
                patternBrush = QBrush(m_currentColor, brushStyle);
            }
            painter.setBrush(patternBrush);
        } else {
            painter.setBrush(Qt::NoBrush);
        }
        QRect squareRect(m_roundedSquareStartPoint, m_roundedSquareCurrentPoint);
        QRect normalizedRect = squareRect.normalized();

        // Use same corner radius calculation as in drawRoundedSquare
        int cornerRadius = qMin(normalizedRect.width(), normalizedRect.height()) / 8;
        cornerRadius = qMax(cornerRadius, 3);
        cornerRadius = qMin(cornerRadius, 12);

        painter.drawRoundedRect(normalizedRect, cornerRadius, cornerRadius);
    }

    // Draw oval preview overlay
    if (m_showOvalPreview && (m_ovalMode || m_filledOvalMode)) {
        painter.setPen(QPen(Qt::black, 2, Qt::DashLine));
        painter.setRenderHint(QPainter::Antialiasing);
        if (m_filledOvalMode) {
            QBrush patternBrush;
            if (isCustomPattern(m_currentPattern)) {
                patternBrush = createCustomPatternBrush(m_currentPattern);
            } else {
                Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
                patternBrush = QBrush(m_currentColor, brushStyle);
            }
            painter.setBrush(patternBrush);
        } else {
            painter.setBrush(Qt::NoBrush);
        }
        QRect ovalRect(m_ovalStartPoint, m_ovalCurrentPoint);
        QRect normalizedRect = ovalRect.normalized();

        painter.drawEllipse(normalizedRect);
    }

    // Draw scissors cut line preview
    if (m_drawingScissors && !m_scissorsCutLine.isEmpty()) {
        painter.setPen(QPen(Qt::red, 3, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);
        painter.drawPolyline(m_scissorsCutLine);
    }

    // Draw scissors pieces if they exist
    if (m_hasScissorsPieces) {
        // Draw piece 1
        painter.drawPixmap(m_piece1Offset, m_scissorsPiece1);
        // Draw border around piece 1
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);
        QRect piece1Rect = m_scissorsRegion1.boundingRect().translated(m_piece1Offset);
        painter.drawRect(piece1Rect);

        // Draw piece 2
        painter.drawPixmap(m_piece2Offset, m_scissorsPiece2);
        // Draw border around piece 2
        painter.setPen(QPen(Qt::green, 2, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);
        QRect piece2Rect = m_scissorsRegion2.boundingRect().translated(m_piece2Offset);
        painter.drawRect(piece2Rect);
    }

    // Draw lasso selection
    if (!m_lassoPolygon.isEmpty()) {
        if (m_drawingLasso) {
            // Show preview while drawing - solid line with different color
            painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine));
            painter.setBrush(Qt::NoBrush);
            painter.drawPolyline(m_lassoPolygon); // Use polyline for drawing in progress
        } else if (m_hasSelection) {
            // Show completed selection - dashed line
            painter.setPen(QPen(Qt::black, 1, Qt::DashLine));
            painter.setBrush(Qt::NoBrush);
            painter.drawPolygon(m_lassoPolygon);

            // If dragging, show the selected pixels at the current position
            if (m_draggingSelection && !m_selectedPixmap.isNull()) {
                QRect newBoundingRect = m_lassoPolygon.boundingRect();

                // Create a mask from the current polygon position
                QPixmap mask(newBoundingRect.size());
                mask.fill(Qt::black);

                QPainter maskPainter(&mask);
                maskPainter.setBrush(Qt::white);
                maskPainter.setPen(Qt::NoPen);

                // Translate polygon to mask coordinates
                QPolygon translatedPolygon;
                for (const QPoint &point : m_lassoPolygon) {
                    translatedPolygon << (point - newBoundingRect.topLeft());
                }
                maskPainter.drawPolygon(translatedPolygon);

                // Scale and mask the selected pixmap
                QPixmap scaledPixmap = m_selectedPixmap.scaled(newBoundingRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                scaledPixmap.setMask(mask.createHeuristicMask());

                // Draw the masked pixmap at the current position
                painter.setRenderHint(QPainter::Antialiasing, false);
                painter.drawPixmap(newBoundingRect.topLeft(), scaledPixmap);
            }
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Check if clicking on scissors pieces to drag them
        if (m_hasScissorsPieces && !m_scissorsMode) {
            QPoint clickPos = mapToCanvas(event->position().toPoint());

            // Check piece 2 first (drawn on top)
            QRect piece2Rect = m_scissorsRegion2.boundingRect().translated(m_piece2Offset);
            if (piece2Rect.contains(clickPos)) {
                m_draggingPiece2 = true;
                m_pieceDragStart = clickPos - m_piece2Offset;
                return;
            }

            // Check piece 1
            QRect piece1Rect = m_scissorsRegion1.boundingRect().translated(m_piece1Offset);
            if (piece1Rect.contains(clickPos)) {
                m_draggingPiece1 = true;
                m_pieceDragStart = clickPos - m_piece1Offset;
                return;
            }

            // Clicked outside both pieces - commit them
            QPainter painter(&currentLayer().pixmap());
            painter.setRenderHint(QPainter::Antialiasing, false);

            painter.drawPixmap(m_piece1Offset, m_scissorsPiece1);
            painter.drawPixmap(m_piece2Offset, m_scissorsPiece2);

            // Clear scissors state
            m_hasScissorsPieces = false;
            m_scissorsPiece1 = QPixmap();
            m_scissorsPiece2 = QPixmap();

            compositeAllLayers();
            update();
            // Don't return - continue with normal click handling
        }

        // Handle pixel zoom mode
        if (m_pixelZoomMode) {
            QPoint clickPos = event->position().toPoint();

            // Check if click is within the magnifying glass area
            QLineF distanceToMagnifier(clickPos, m_magnifierPosition);
            if (distanceToMagnifier.length() <= m_magnifierRadius) {
                // Click is in magnifier - convert to pixel coordinates for drawing
                int visiblePixels = 9;
                int halfVisible = visiblePixels / 2;
                int pixelSize = (m_magnifierRadius * 2) / visiblePixels;

                // Calculate which pixel was clicked in the magnifier
                int relativeX = clickPos.x() - (m_magnifierPosition.x() - m_magnifierRadius);
                int relativeY = clickPos.y() - (m_magnifierPosition.y() - m_magnifierRadius);

                int pixelOffsetX = (relativeX / pixelSize) - halfVisible;
                int pixelOffsetY = (relativeY / pixelSize) - halfVisible;

                int targetPixelX = m_zoomCenter.x() + pixelOffsetX;
                int targetPixelY = m_zoomCenter.y() + pixelOffsetY;

                // Draw on the target pixel if it's valid
                if (targetPixelX >= 0 && targetPixelX < m_document->width() &&
                    targetPixelY >= 0 && targetPixelY < m_document->height()) {

                    if (m_pencilMode) {
                        saveCanvasState();
                        m_drawingInMagnifier = true;
                        QPainter painter(&currentLayer().pixmap());
                        // Use pen color set by caller;
                        painter.drawPoint(targetPixelX, targetPixelY);
                        compositeAllLayers();
                        update();
                    }
                }
            } else {
                // Click is outside magnifier - select new zoom area
                QPoint canvasPoint = mapToCanvas(clickPos);
                if (canvasPoint.x() >= 0 && canvasPoint.x() < m_document->width() &&
                    canvasPoint.y() >= 0 && canvasPoint.y() < m_document->height()) {
                    setZoomCenter(canvasPoint);
                }
            }
            return;
        }

        if (m_pencilMode) {
            // Save canvas state before drawing
            m_canvasBeforeEdit = m_canvas;
            m_lastPoint = mapToCanvas(event->position().toPoint());
            m_drawing = true;
        } else if (m_textMode) {
            if (m_textEdit && m_textMovable) {
                // Check if clicking on the text box to drag it
                QRect textRect = m_textEdit->geometry();
                if (textRect.contains(event->position().toPoint())) {
                    m_movingText = true;
                    m_textDragStart = event->position().toPoint() - m_textEdit->pos();
                } else {
                    // Clicking outside commits the text
                    commitText();
                    startTextInput(mapToCanvas(event->position().toPoint()));
                }
            } else if (m_textEdit) {
                // Already typing but not movable yet
                finishTextInput();
                startTextInput(mapToCanvas(event->position().toPoint()));
            } else {
                startTextInput(mapToCanvas(event->position().toPoint()));
            }
        } else if (m_sprayMode) {
            m_canvasBeforeEdit = m_canvas;
            m_drawing = true;
            sprayPaint(mapToCanvas(event->position().toPoint()));
        } else if (m_brushMode) {
            m_canvasBeforeEdit = m_canvas;
            m_lastPoint = mapToCanvas(event->position().toPoint());
            m_drawing = true;
            brushPaint(mapToCanvas(event->position().toPoint()));
        } else if (m_eraserMode) {
            m_canvasBeforeEdit = m_canvas;
            m_lastPoint = mapToCanvas(event->position().toPoint());
            m_drawing = true;
            eraserPaint(mapToCanvas(event->position().toPoint()));
        } else if (m_lineMode) {
            m_canvasBeforeEdit = m_canvas;
            m_lineStartPoint = mapToCanvas(event->position().toPoint());
            m_lineCurrentPoint = m_lineStartPoint;
            m_showLinePreview = true;
            m_drawing = true;
        } else if (m_bezierMode) {
            if (m_bezierClickCount == 0) {
                m_canvasBeforeEdit = m_canvas;
            }
            m_bezierPoints[m_bezierClickCount] = mapToCanvas(event->position().toPoint());
            m_bezierClickCount++;

            if (m_bezierClickCount == 4) {
                // Draw the bezier curve with all 4 points
                drawBezierCurve(m_bezierPoints[0], m_bezierPoints[1], m_bezierPoints[2], m_bezierPoints[3]);
                m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, "Bezier Curve"));
                m_bezierClickCount = 0;  // Reset for next curve
            }
            update();
        } else if (m_fillMode) {
            m_canvasBeforeEdit = m_canvas;
            floodFill(mapToCanvas(event->position().toPoint()), m_currentColor);
            m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, "Fill"));
        } else if (m_lassoMode) {
            QPoint clickPoint = mapToCanvas(event->position().toPoint());

            // Check if clicking inside existing selection to start dragging
            if (m_hasSelection && !m_drawingLasso && isPointInSelection(clickPoint)) {
                m_draggingSelection = true;
                m_dragStartPoint = clickPoint;
                m_drawing = true;
                setCursor(Qt::ClosedHandCursor);

                // Save canvas state for undo
                m_canvasBeforeEdit = m_canvas;

                // Extract the selected pixels
                QRect boundingRect = m_lassoPolygon.boundingRect();
                m_selectedPixmap = m_canvas.copy(boundingRect);
                m_selectionOffset = boundingRect.topLeft();

                // Clear the selected area from canvas
                QPainter painter(&currentLayer().pixmap());
                painter.setCompositionMode(QPainter::CompositionMode_Clear);
                painter.setClipRegion(QRegion(m_lassoPolygon));
                painter.fillRect(currentLayer().pixmap().rect(), Qt::black);
                compositeAllLayers();
                update();
            } else {
                // Start new lasso selection
                m_lassoPolygon.clear();
                m_lassoPolygon << clickPoint;
                m_hasSelection = false; // Selection is in progress
                m_drawingLasso = true; // Show preview while drawing
                m_drawing = true;
                m_draggingSelection = false;
            }
        } else if (m_squareMode || m_filledSquareMode) {
            m_canvasBeforeEdit = m_canvas;
            m_squareStartPoint = mapToCanvas(event->position().toPoint());
            m_squareCurrentPoint = m_squareStartPoint;
            m_showSquarePreview = true;
            m_drawing = true;
        } else if (m_roundedSquareMode || m_filledRoundedSquareMode) {
            m_canvasBeforeEdit = m_canvas;
            m_roundedSquareStartPoint = mapToCanvas(event->position().toPoint());
            m_roundedSquareCurrentPoint = m_roundedSquareStartPoint;
            m_showRoundedSquarePreview = true;
            m_drawing = true;
        } else if (m_ovalMode || m_filledOvalMode) {
            m_canvasBeforeEdit = m_canvas;
            m_ovalStartPoint = mapToCanvas(event->position().toPoint());
            m_ovalCurrentPoint = m_ovalStartPoint;
            m_showOvalPreview = true;
            m_drawing = true;
        } else if (m_scissorsMode) {
            m_canvasBeforeEdit = m_canvas;
            m_scissorsCutLine.clear();
            m_scissorsCutLine << mapToCanvas(event->position().toPoint());
            m_drawingScissors = true;
            m_drawing = true;
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    // Handle scissors piece dragging
    if (m_draggingPiece1) {
        QPoint currentPos = mapToCanvas(event->position().toPoint());
        m_piece1Offset = currentPos - m_pieceDragStart;
        update();
        return;
    }

    if (m_draggingPiece2) {
        QPoint currentPos = mapToCanvas(event->position().toPoint());
        m_piece2Offset = currentPos - m_pieceDragStart;
        update();
        return;
    }

    // Handle text dragging
    if (m_movingText && m_textEdit) {
        QPoint newPos = event->position().toPoint() - m_textDragStart;
        m_textEdit->move(newPos);
        return;
    }

    // Update mouse position for coordinate display
    if (m_showCoordinates) {
        m_mousePosition = mapToCanvas(event->position().toPoint());
        update(); // Trigger repaint to update coordinate display
    }

    // Handle pixel zoom mode dragging within magnifier
    if (m_pixelZoomMode && (event->buttons() & Qt::LeftButton)) {
        QPoint clickPos = event->position().toPoint();

        // Check if drag is within the magnifying glass area
        QLineF distanceToMagnifier(clickPos, m_magnifierPosition);
        if (distanceToMagnifier.length() <= m_magnifierRadius) {
            // Drag is in magnifier - convert to pixel coordinates for drawing
            int visiblePixels = 9;
            int halfVisible = visiblePixels / 2;
            int pixelSize = (m_magnifierRadius * 2) / visiblePixels;

            // Calculate which pixel is being dragged over
            int relativeX = clickPos.x() - (m_magnifierPosition.x() - m_magnifierRadius);
            int relativeY = clickPos.y() - (m_magnifierPosition.y() - m_magnifierRadius);

            int pixelOffsetX = (relativeX / pixelSize) - halfVisible;
            int pixelOffsetY = (relativeY / pixelSize) - halfVisible;

            int targetPixelX = m_zoomCenter.x() + pixelOffsetX;
            int targetPixelY = m_zoomCenter.y() + pixelOffsetY;

            // Draw on the target pixel if it's valid and different from last
            if (targetPixelX >= 0 && targetPixelX < m_document->width() &&
                targetPixelY >= 0 && targetPixelY < m_document->height()) {

                if (m_pencilMode && m_drawingInMagnifier) {
                    QPainter painter(&currentLayer().pixmap());
                    // Use pen color set by caller;
                    painter.drawPoint(targetPixelX, targetPixelY);
                    compositeAllLayers();
                    update();
                }
            }
        }
        return;
    }

    QPoint canvasPoint = mapToCanvas(event->position().toPoint());

    // Handle cursor changes when not drawing
    if (!(event->buttons() & Qt::LeftButton)) {
        if (m_hasSelection && !m_drawingLasso && isPointInSelection(canvasPoint)) {
            setCursor(Qt::OpenHandCursor);
        } else if (m_draggingSelection) {
            setCursor(Qt::ClosedHandCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }
    }

    if ((event->buttons() & Qt::LeftButton) && m_drawing) {
        if (m_pencilMode) {
            drawLineTo(mapToCanvas(event->position().toPoint()));
        } else if (m_sprayMode) {
            sprayPaint(mapToCanvas(event->position().toPoint()));
        } else if (m_brushMode) {
            QPoint currentPoint = mapToCanvas(event->position().toPoint());
            // Draw connected brush strokes for smooth painting
            QPoint step = currentPoint - m_lastPoint;
            int distance = qMax(qAbs(step.x()), qAbs(step.y()));
            for (int i = 0; i <= distance; i += 2) {
                QPoint interpolated = m_lastPoint + (step * i / qMax(distance, 1));
                brushPaint(interpolated);
            }
            m_lastPoint = currentPoint;
        } else if (m_eraserMode) {
            QPoint currentPoint = mapToCanvas(event->position().toPoint());
            // Draw connected eraser strokes for smooth erasing
            QPoint step = currentPoint - m_lastPoint;
            int distance = qMax(qAbs(step.x()), qAbs(step.y()));
            for (int i = 0; i <= distance; i += 2) {
                QPoint interpolated = m_lastPoint + (step * i / qMax(distance, 1));
                eraserPaint(interpolated);
            }
            m_lastPoint = currentPoint;
        } else if (m_lineMode && m_showLinePreview) {
            m_lineCurrentPoint = mapToCanvas(event->position().toPoint());
            update(); // Trigger repaint to show updated preview line
        } else if (m_lassoMode && !m_draggingSelection) {
            // Add points to lasso polygon
            QPoint currentPoint = mapToCanvas(event->position().toPoint());
            m_lassoPolygon << currentPoint;
            update(); // Trigger repaint to show updated lasso
        } else if ((m_squareMode || m_filledSquareMode) && m_showSquarePreview) {
            m_squareCurrentPoint = mapToCanvas(event->position().toPoint());
            update(); // Trigger repaint to show updated square preview
        } else if ((m_roundedSquareMode || m_filledRoundedSquareMode) && m_showRoundedSquarePreview) {
            m_roundedSquareCurrentPoint = mapToCanvas(event->position().toPoint());
            update(); // Trigger repaint to show updated rounded square preview
        } else if ((m_ovalMode || m_filledOvalMode) && m_showOvalPreview) {
            m_ovalCurrentPoint = mapToCanvas(event->position().toPoint());
            update(); // Trigger repaint to show updated oval preview
        } else if (m_scissorsMode && m_drawingScissors) {
            // Add points to scissors cut line
            QPoint currentPoint = mapToCanvas(event->position().toPoint());
            m_scissorsCutLine << currentPoint;
            update(); // Trigger repaint to show scissors line
        }
    }

    // Handle selection dragging
    if (m_draggingSelection && (event->buttons() & Qt::LeftButton)) {
        QPoint currentPoint = mapToCanvas(event->position().toPoint());
        QPoint offset = currentPoint - m_dragStartPoint;

        // Update selection polygon position
        QPolygon newPolygon;
        for (const QPoint &point : m_lassoPolygon) {
            newPolygon << (point + offset);
        }
        m_lassoPolygon = newPolygon;

        m_dragStartPoint = currentPoint;
        update(); // Trigger repaint to show updated selection position
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Handle scissors piece dragging completion
        if (m_draggingPiece1 || m_draggingPiece2) {
            // Commit the pieces to the layer at their current positions
            QPainter painter(&currentLayer().pixmap());
            painter.setRenderHint(QPainter::Antialiasing, false);

            painter.drawPixmap(m_piece1Offset, m_scissorsPiece1);
            painter.drawPixmap(m_piece2Offset, m_scissorsPiece2);

            // Clear scissors state
            m_hasScissorsPieces = false;
            m_draggingPiece1 = false;
            m_draggingPiece2 = false;
            m_scissorsPiece1 = QPixmap();
            m_scissorsPiece2 = QPixmap();

            compositeAllLayers();
            update();
            return;
        }

        // Handle text dragging completion
        if (m_movingText) {
            m_movingText = false;
            return;
        }

        // Handle pixel zoom mode drawing completion
        if (m_drawingInMagnifier) {
            m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, "Pixel Edit"));
            m_drawingInMagnifier = false;
            return;
        }
    }

    if (event->button() == Qt::LeftButton && m_drawing) {
        if (m_pencilMode) {
            drawLineTo(mapToCanvas(event->position().toPoint()));
            // Create undo command for pencil drawing
            m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, "Pencil"));
        } else if (m_lineMode) {
            drawStraightLine(m_lineStartPoint, mapToCanvas(event->position().toPoint()));
            m_showLinePreview = false; // Hide preview after drawing final line
            m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, "Line"));
        } else if (m_lassoMode) {
            if (m_draggingSelection) {
                // Complete selection dragging - place the selected pixels at new location
                QRect newBoundingRect = m_lassoPolygon.boundingRect();

                // Draw the selected pixels at the new location
                QPainter painter(&currentLayer().pixmap());
                painter.setRenderHint(QPainter::Antialiasing, false);

                // Create a mask from the new polygon position
                QPixmap mask(newBoundingRect.size());
                mask.fill(Qt::black);

                QPainter maskPainter(&mask);
                maskPainter.setBrush(Qt::white);
                maskPainter.setPen(Qt::NoPen);

                // Translate polygon to mask coordinates
                QPolygon translatedPolygon;
                for (const QPoint &point : m_lassoPolygon) {
                    translatedPolygon << (point - newBoundingRect.topLeft());
                }
                maskPainter.drawPolygon(translatedPolygon);

                // Scale the selected pixmap if needed
                QPixmap scaledPixmap = m_selectedPixmap.scaled(newBoundingRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                scaledPixmap.setMask(mask.createHeuristicMask());

                // Draw the masked pixmap at the new location
                painter.drawPixmap(newBoundingRect.topLeft(), scaledPixmap);

                compositeAllLayers();

                // Create undo command for the move operation
                m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, "Move Selection"));

                m_draggingSelection = false;
                setCursor(Qt::ArrowCursor);
                update();
            } else {
                // Complete lasso selection
                m_drawingLasso = false; // Stop showing drawing preview
                if (m_lassoPolygon.size() > 2) {
                    m_hasSelection = true;
                    update(); // Show completed selection with dashed outline
                } else {
                    m_lassoPolygon.clear();
                    m_hasSelection = false;
                    update(); // Clear any preview
                }
            }
        } else if (m_squareMode || m_filledSquareMode) {
            drawSquare(m_squareStartPoint, mapToCanvas(event->position().toPoint()), m_filledSquareMode);
            m_showSquarePreview = false; // Hide preview after drawing final square
            m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, m_filledSquareMode ? "Filled Rectangle" : "Rectangle"));
        } else if (m_roundedSquareMode || m_filledRoundedSquareMode) {
            drawRoundedSquare(m_roundedSquareStartPoint, mapToCanvas(event->position().toPoint()), m_filledRoundedSquareMode);
            m_showRoundedSquarePreview = false; // Hide preview after drawing final rounded square
            m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, m_filledRoundedSquareMode ? "Filled Rounded Rectangle" : "Rounded Rectangle"));
        } else if (m_ovalMode || m_filledOvalMode) {
            drawOval(m_ovalStartPoint, mapToCanvas(event->position().toPoint()), m_filledOvalMode);
            m_showOvalPreview = false; // Hide preview after drawing final oval
            m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, m_filledOvalMode ? "Filled Oval" : "Oval"));
        } else if (m_scissorsMode && m_drawingScissors) {
            m_scissorsCutLine << mapToCanvas(event->position().toPoint());
            performScissorsCut(m_scissorsCutLine);
            m_drawingScissors = false;
            m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, "Scissors Cut"));
        }

        // Add undo commands for continuous drawing tools
        if (m_sprayMode || m_brushMode || m_eraserMode) {
            QString toolName = m_sprayMode ? "Spray" : (m_brushMode ? "Brush" : "Eraser");
            m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, toolName));
        }

        m_drawing = false;
    }
}

QPoint Canvas::mapToCanvas(const QPoint &point)
{
    return QPoint(point.x() / m_scaleFactor, point.y() / m_scaleFactor);
}

void Canvas::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&currentLayer().pixmap());

    if (m_currentPattern == PatternBar::Solid) {
        // For solid pattern, use regular line drawing
        painter.setPen(QPen(m_currentColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(m_lastPoint, endPoint);
    } else {
        // For patterns, draw a thicker line with pattern fill
        QBrush patternBrush;
        if (isCustomPattern(m_currentPattern)) {
            patternBrush = createCustomPatternBrush(m_currentPattern);
        } else {
            Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
            patternBrush = QBrush(m_currentColor, brushStyle);
        }

        painter.setBrush(patternBrush);
        painter.setPen(QPen(m_currentColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        // Draw a path that can be filled with pattern
        QPainterPath path;
        path.moveTo(m_lastPoint);
        path.lineTo(endPoint);

        QPainterPathStroker stroker;
        stroker.setWidth(3);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);

        QPainterPath strokedPath = stroker.createStroke(path);
        painter.setPen(Qt::NoPen);
        painter.drawPath(strokedPath);
    }

    compositeAllLayers();

    int rad = (m_currentPattern == PatternBar::Solid) ? 1 : 2;
    QRect updateRect = QRect(m_lastPoint, endPoint).normalized()
                       .adjusted(-rad, -rad, +rad, +rad);
    updateRect = QRect(updateRect.topLeft() * m_scaleFactor,
                      updateRect.bottomRight() * m_scaleFactor);
    update(updateRect);

    m_lastPoint = endPoint;
}

void Canvas::sprayPaint(const QPoint &position)
{
    QPainter painter(&currentLayer().pixmap());

    // Set up brush with current pattern
    QBrush patternBrush;
    if (isCustomPattern(m_currentPattern)) {
        patternBrush = createCustomPatternBrush(m_currentPattern);
    } else {
        Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
        patternBrush = QBrush(m_currentColor, brushStyle);
    }
    painter.setBrush(patternBrush);
    painter.setPen(Qt::NoPen);

    // Calculate number of dots based on diameter
    int numDots = m_sprayDiameter / 2;
    if (numDots < 5) numDots = 5;
    if (numDots > 50) numDots = 50;

    for (int i = 0; i < numDots; ++i) {
        // Generate random position within spray diameter
        double angle = (double(rand()) / RAND_MAX) * 2.0 * M_PI;
        double radius = (double(rand()) / RAND_MAX) * (m_sprayDiameter / 2.0);

        int x = position.x() + int(radius * cos(angle));
        int y = position.y() + int(radius * sin(angle));

        // Make sure the point is within canvas bounds
        if (x >= 0 && x < m_document->width() && y >= 0 && y < m_document->height()) {
            // Draw a small pattern-filled rectangle instead of a point
            int dotSize = (m_currentPattern == PatternBar::Solid) ? 1 : 2;
            painter.drawRect(x - dotSize/2, y - dotSize/2, dotSize, dotSize);
        }
    }

    compositeAllLayers();

    // Update the affected area
    int updateRadius = m_sprayDiameter / 2 + 2;
    QRect updateRect = QRect(position.x() - updateRadius, position.y() - updateRadius,
                            updateRadius * 2, updateRadius * 2);
    updateRect = QRect(updateRect.topLeft() * m_scaleFactor,
                      updateRect.bottomRight() * m_scaleFactor);
    update(updateRect);
}

void Canvas::brushPaint(const QPoint &position)
{
    QPainter painter(&currentLayer().pixmap());
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up brush properties with current pattern
    QBrush patternBrush;
    if (isCustomPattern(m_currentPattern)) {
        patternBrush = createCustomPatternBrush(m_currentPattern);
    } else {
        Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
        patternBrush = QBrush(m_currentColor, brushStyle);
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(patternBrush);

    // Draw a circle for the brush stroke
    int radius = m_brushDiameter / 2;
    QRect brushRect(position.x() - radius, position.y() - radius,
                    m_brushDiameter, m_brushDiameter);

    // Make sure brush stroke is within canvas bounds
    if (brushRect.intersects(QRect(0, 0, m_document->width(), m_document->height()))) {
        painter.drawEllipse(brushRect);
    }

    compositeAllLayers();

    // Update the affected area
    int updateRadius = radius + 2;
    QRect updateRect = QRect(position.x() - updateRadius, position.y() - updateRadius,
                            updateRadius * 2, updateRadius * 2);
    updateRect = QRect(updateRect.topLeft() * m_scaleFactor,
                      updateRect.bottomRight() * m_scaleFactor);
    update(updateRect);
}

void Canvas::eraserPaint(const QPoint &position)
{
    QPainter painter(&currentLayer().pixmap());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);

    // Set up eraser properties - clear pixels to transparent
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));

    // Draw a circle for the eraser stroke
    int radius = m_eraserDiameter / 2;
    QRect eraserRect(position.x() - radius, position.y() - radius,
                     m_eraserDiameter, m_eraserDiameter);

    // Make sure eraser stroke is within canvas bounds
    if (eraserRect.intersects(QRect(0, 0, m_document->width(), m_document->height()))) {
        painter.drawEllipse(eraserRect);
    }

    compositeAllLayers();

    // Update the affected area
    int updateRadius = radius + 2;
    QRect updateRect = QRect(position.x() - updateRadius, position.y() - updateRadius,
                            updateRadius * 2, updateRadius * 2);
    updateRect = QRect(updateRect.topLeft() * m_scaleFactor,
                      updateRect.bottomRight() * m_scaleFactor);
    update(updateRect);
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    // Handle Escape key for scissors pieces
    if (event->key() == Qt::Key_Escape && m_hasScissorsPieces) {
        // Commit the pieces at their current positions
        QPainter painter(&currentLayer().pixmap());
        painter.setRenderHint(QPainter::Antialiasing, false);

        painter.drawPixmap(m_piece1Offset, m_scissorsPiece1);
        painter.drawPixmap(m_piece2Offset, m_scissorsPiece2);

        // Clear scissors state
        m_hasScissorsPieces = false;
        m_draggingPiece1 = false;
        m_draggingPiece2 = false;
        m_scissorsPiece1 = QPixmap();
        m_scissorsPiece2 = QPixmap();

        compositeAllLayers();
        update();
        return;
    }

    if (event->key() == Qt::Key_Escape && m_textEdit) {
        if (m_textMovable) {
            commitText();
        } else {
            finishTextInput();
        }
        return;
    }

    // Handle pixel zoom navigation
    if (m_pixelZoomMode) {
        switch (event->key()) {
            case Qt::Key_Left:
                movePixelCursor(-1, 0);
                return;
            case Qt::Key_Right:
                movePixelCursor(1, 0);
                return;
            case Qt::Key_Up:
                movePixelCursor(0, -1);
                return;
            case Qt::Key_Down:
                movePixelCursor(0, 1);
                return;
            case Qt::Key_Space:
                // Paint current pixel with selected color/pattern
                if (m_pencilMode) {
                    saveCanvasState();
                    QPainter painter(&currentLayer().pixmap());
                    // Use pen color set by caller;
                    painter.drawPoint(m_zoomCenter);
                    update();
                }
                return;
        }
    }

    QWidget::keyPressEvent(event);
}

void Canvas::startTextInput(const QPoint &position)
{
    m_textPosition = position;

    m_textEdit = new QLineEdit(this);
    m_textEdit->setFont(m_textFont);
    m_textEdit->setStyleSheet("QLineEdit { border: 1px solid black; background: white; }");

    // Enable input method support for international characters
    m_textEdit->setAttribute(Qt::WA_InputMethodEnabled, true);
    m_textEdit->setInputMethodHints(Qt::ImhNone); // Allow all input types

    QFontMetrics fm(m_textFont);
    int textHeight = fm.height();

    QPoint widgetPos = QPoint(position.x() * m_scaleFactor, (position.y() - textHeight + fm.descent()) * m_scaleFactor);
    m_textEdit->move(widgetPos);
    m_textEdit->resize(200 * m_scaleFactor, textHeight * m_scaleFactor);
    m_textEdit->show();
    m_textEdit->setFocus();

    connect(m_textEdit, &QLineEdit::returnPressed, this, &Canvas::finishTextInput);
    connect(m_textEdit, &QLineEdit::editingFinished, this, &Canvas::finishTextInput);
}

void Canvas::finishTextInput()
{
    if (!m_textEdit) {
        return;
    }

    QString text = m_textEdit->text();
    if (!text.isEmpty()) {
        // Make text movable after pressing Enter
        m_textMovable = true;
        m_textEdit->setReadOnly(true);
        m_textEdit->setStyleSheet("QLineEdit { border: 2px dashed blue; background: rgba(255,255,255,180); }");
        return; // Don't commit yet, allow moving
    }

    m_textEdit->deleteLater();
    m_textEdit = nullptr;
    m_textMovable = false;
}

void Canvas::commitText()
{
    if (!m_textEdit) {
        return;
    }

    QString text = m_textEdit->text();
    if (!text.isEmpty()) {
        // Update text position based on current widget position
        QPoint widgetPos = m_textEdit->pos();
        m_textPosition = QPoint(widgetPos.x() / m_scaleFactor, widgetPos.y() / m_scaleFactor);

        // Save canvas state for undo
        m_canvasBeforeEdit = m_canvas;

        QPainter painter(&currentLayer().pixmap());
        painter.setFont(m_textFont);
        painter.setPen(m_currentColor);

        // Enable text antialiasing for better Unicode character rendering
        painter.setRenderHint(QPainter::TextAntialiasing, true);

        QFontMetrics fm(m_textFont);
        QPoint textBaseline = QPoint(m_textPosition.x(), m_textPosition.y() + fm.ascent());
        painter.drawText(textBaseline, text);

        compositeAllLayers();

        // Create undo command for text input
        m_undoStack->push(new CanvasUndoCommand(this, m_canvasBeforeEdit, m_canvas, "Text"));

        update();
    }

    m_textEdit->deleteLater();
    m_textEdit = nullptr;
    m_textMovable = false;
    m_movingText = false;
}

void Canvas::floodFill(const QPoint &position, const QColor &fillColor)
{
    // Check if position is within canvas bounds
    if (position.x() < 0 || position.x() >= m_document->width() ||
        position.y() < 0 || position.y() >= m_document->height()) {
        return;
    }

    // Get the target color (color to be replaced)
    QImage image = m_canvas.toImage();
    QColor targetColor = image.pixelColor(position.x(), position.y());

    // If target color is same as fill color, nothing to do
    if (targetColor == fillColor) {
        return;
    }

    // Create a mask to identify the region to fill
    QImage mask(m_document->width(), m_document->height(), QImage::Format_ARGB32);
    mask.fill(Qt::transparent);

    // Stack-based flood fill algorithm to create the mask
    QStack<QPoint> stack;
    stack.push(position);

    while (!stack.isEmpty()) {
        QPoint currentPoint = stack.pop();
        int x = currentPoint.x();
        int y = currentPoint.y();

        // Check bounds
        if (x < 0 || x >= m_document->width() || y < 0 || y >= m_document->height()) {
            continue;
        }

        // Check if this pixel needs to be filled
        if (image.pixelColor(x, y) != targetColor || mask.pixelColor(x, y) != Qt::transparent) {
            continue;
        }

        // Mark this pixel in the mask
        mask.setPixelColor(x, y, Qt::white);

        // Add neighboring pixels to stack
        stack.push(QPoint(x + 1, y));     // Right
        stack.push(QPoint(x - 1, y));     // Left
        stack.push(QPoint(x, y + 1));     // Down
        stack.push(QPoint(x, y - 1));     // Up
    }

    // Now paint the masked region with the selected pattern
    QPainter painter(&currentLayer().pixmap());
    painter.setRenderHint(QPainter::Antialiasing, false); // Keep pixels crisp for patterns

    QBrush patternBrush;
    if (isCustomPattern(m_currentPattern)) {
        patternBrush = createCustomPatternBrush(m_currentPattern);
    } else {
        Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
        patternBrush = QBrush(fillColor, brushStyle);
    }
    painter.setBrush(patternBrush);
    painter.setPen(Qt::NoPen);

    // Create a clipping region from the mask
    QRegion clipRegion;
    for (int y = 0; y < m_document->height(); ++y) {
        for (int x = 0; x < m_document->width(); ++x) {
            if (mask.pixelColor(x, y) == Qt::white) {
                clipRegion += QRect(x, y, 1, 1);
            }
        }
    }

    painter.setClipRegion(clipRegion);
    painter.drawRect(0, 0, m_document->width(), m_document->height());

    compositeAllLayers();
    update();
}

void Canvas::setTextFont(const QString &fontFamily)
{
    // Set primary font family and maintain fallback fonts for international support
    QStringList fontFamilies;
    fontFamilies << fontFamily;

    // Add fallback fonts based on the selected font
    if (fontFamily.contains("Noto Sans CJK") || fontFamily.contains("CJK")) {
        // Prioritize CJK fonts
        fontFamilies << "Noto Sans CJK SC" << "Noto Sans CJK TC" << "Noto Sans CJK JP" << "Noto Sans CJK KR"
                     << "Noto Sans" << "DejaVu Sans" << "Liberation Sans";
    } else if (fontFamily.contains("DejaVu") || fontFamily.contains("Cyrillic")) {
        // Prioritize Cyrillic support
        fontFamilies << "DejaVu Sans" << "Liberation Sans" << "Noto Sans" << "Noto Sans CJK";
    } else {
        // General fallback chain for other fonts
        fontFamilies << "Noto Sans" << "Noto Sans CJK" << "DejaVu Sans" << "Liberation Sans";
    }

    m_textFont.setFamilies(fontFamilies);
}

void Canvas::setTextFontSize(int fontSize)
{
    m_textFont.setPointSize(fontSize);
}

void Canvas::clearSelection()
{
    m_hasSelection = false;
    m_drawingLasso = false;
    m_draggingSelection = false;
    m_lassoPolygon.clear();
    setCursor(Qt::ArrowCursor);
    update();
}

void Canvas::cutSelection()
{
    if (!m_hasSelection || m_lassoPolygon.isEmpty()) {
        return;
    }

    copySelection(); // First copy to clipboard

    // Create a region from the polygon for masking
    QRegion region(m_lassoPolygon);
    QImage image = m_canvas.toImage();

    // Fill the selected area with white (background color)
    QPainter painter(&image);
    painter.setClipRegion(region);
    painter.fillRect(image.rect(), Qt::white);

    m_canvas = QPixmap::fromImage(image);
    clearSelection();
    update();
}

void Canvas::copySelection()
{
    if (!m_hasSelection || m_lassoPolygon.isEmpty()) {
        return;
    }

    // Get bounding rectangle of selection
    QRect boundingRect = m_lassoPolygon.boundingRect();

    // Create a pixmap of the selected area
    QPixmap selectedArea = m_canvas.copy(boundingRect);

    // Create a mask from the polygon
    QPixmap mask(boundingRect.size());
    mask.fill(Qt::black);

    QPainter maskPainter(&mask);
    maskPainter.setBrush(Qt::white);
    maskPainter.setPen(Qt::NoPen);

    // Translate polygon to mask coordinates
    QPolygon translatedPolygon;
    for (const QPoint &point : m_lassoPolygon) {
        translatedPolygon << (point - boundingRect.topLeft());
    }
    maskPainter.drawPolygon(translatedPolygon);

    // Apply mask to selected area
    selectedArea.setMask(mask.createHeuristicMask());

    // Store in internal clipboard
    m_clipboard = selectedArea;

    // Also copy to system clipboard
    QApplication::clipboard()->setPixmap(selectedArea);
}

void Canvas::pasteSelection()
{
    // Use default position for menu-based paste
    pasteSelectionAt(QPoint(50, 50));
}

void Canvas::pasteSelectionAt(const QPoint &position)
{
    if (m_clipboard.isNull()) {
        // Try to get from system clipboard
        m_clipboard = QApplication::clipboard()->pixmap();
        if (m_clipboard.isNull()) {
            return;
        }
    }

    // Center the pasted content around the mouse position
    QPoint pastePos = position - QPoint(m_clipboard.width() / 2, m_clipboard.height() / 2);

    // Ensure the paste position doesn't go outside canvas bounds
    pastePos.setX(qMax(0, qMin(pastePos.x(), m_document->width() - m_clipboard.width())));
    pastePos.setY(qMax(0, qMin(pastePos.y(), m_document->height() - m_clipboard.height())));

    QPainter painter(&currentLayer().pixmap());
    painter.drawPixmap(pastePos, m_clipboard);

    compositeAllLayers();
    update();
}

void Canvas::insertImageAt(const QPixmap &image, const QPoint &position)
{
    if (image.isNull()) {
        return;
    }

    // Save canvas state for undo
    QPixmap canvasBeforeInsert = m_canvas;

    // Scale image if it's larger than the canvas
    QPixmap scaledImage = image;
    int maxWidth = m_document->width();
    int maxHeight = m_document->height();

    if (image.width() > maxWidth || image.height() > maxHeight) {
        scaledImage = image.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // Center the image around the given position
    QPoint insertPos = position - QPoint(scaledImage.width() / 2, scaledImage.height() / 2);

    // Ensure the image stays within canvas bounds
    insertPos.setX(qMax(0, qMin(insertPos.x(), m_document->width() - scaledImage.width())));
    insertPos.setY(qMax(0, qMin(insertPos.y(), m_document->height() - scaledImage.height())));

    // Draw the image onto the current layer
    QPainter painter(&currentLayer().pixmap());
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(insertPos, scaledImage);

    compositeAllLayers();

    // Create undo command for image insertion
    m_undoStack->push(new CanvasUndoCommand(this, canvasBeforeInsert, m_canvas, "Insert Image"));

    update();
}

void Canvas::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu contextMenu(this);

    // Add Cut action (only if there's a selection)
    QAction *cutAction = nullptr;
    if (m_hasSelection && !m_lassoPolygon.isEmpty()) {
        cutAction = contextMenu.addAction("Cut");
        connect(cutAction, &QAction::triggered, this, &Canvas::cutSelection);
    }

    // Add Copy action (only if there's a selection)
    QAction *copyAction = nullptr;
    if (m_hasSelection && !m_lassoPolygon.isEmpty()) {
        copyAction = contextMenu.addAction("Copy");
        connect(copyAction, &QAction::triggered, this, &Canvas::copySelection);
    }

    // Add separator if we have cut/copy actions
    if (cutAction || copyAction) {
        contextMenu.addSeparator();
    }

    // Add Paste action (if clipboard has content)
    QAction *pasteAction = nullptr;
    if (!m_clipboard.isNull() || !QApplication::clipboard()->pixmap().isNull()) {
        pasteAction = contextMenu.addAction("Paste Here");
        // Capture the position now, before the lambda is executed
        QPoint clickPos = mapToCanvas(event->pos());
        connect(pasteAction, &QAction::triggered, this, [this, clickPos]() {
            pasteSelectionAt(clickPos);
        });
    }

    // Only show menu if there are actions
    if (cutAction || copyAction || pasteAction) {
        contextMenu.exec(event->globalPos());
    }
}

void Canvas::drawStraightLine(const QPoint &startPoint, const QPoint &endPoint)
{
    QPainter painter(&currentLayer().pixmap());

    if (m_currentPattern == PatternBar::Solid) {
        // For solid pattern, use regular line drawing
        painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(startPoint, endPoint);
    } else {
        // For patterns, draw a thicker line with pattern fill
        QBrush patternBrush;
        if (isCustomPattern(m_currentPattern)) {
            patternBrush = createCustomPatternBrush(m_currentPattern);
        } else {
            Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
            patternBrush = QBrush(m_currentColor, brushStyle);
        }
        painter.setBrush(patternBrush);

        // Draw a path that can be filled with pattern
        QPainterPath path;
        path.moveTo(startPoint);
        path.lineTo(endPoint);

        QPainterPathStroker stroker;
        stroker.setWidth(4);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);

        QPainterPath strokedPath = stroker.createStroke(path);
        painter.setPen(Qt::NoPen);
        painter.drawPath(strokedPath);
    }

    compositeAllLayers();

    // Calculate update rectangle that encompasses the entire line
    QRect updateRect = QRect(startPoint, endPoint).normalized();
    int margin = (m_currentPattern == PatternBar::Solid) ? 2 : 3; // Add margin for line thickness
    updateRect = updateRect.adjusted(-margin, -margin, +margin, +margin);

    // Scale the update rectangle for the widget coordinates
    updateRect = QRect(updateRect.topLeft() * m_scaleFactor,
                      updateRect.bottomRight() * m_scaleFactor);
    update(updateRect);
}

void Canvas::drawBezierCurve(const QPoint &p0, const QPoint &p1, const QPoint &p2, const QPoint &p3)
{
    QPainter painter(&currentLayer().pixmap());

    // Create bezier curve path
    QPainterPath path;
    path.moveTo(p0);
    path.cubicTo(p1, p2, p3);

    if (m_currentPattern == PatternBar::Solid) {
        // For solid pattern, use regular curve drawing
        painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(path);
    } else {
        // For patterns, draw a thicker curve with pattern fill
        QBrush patternBrush;
        if (isCustomPattern(m_currentPattern)) {
            patternBrush = createCustomPatternBrush(m_currentPattern);
        } else {
            Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
            patternBrush = QBrush(m_currentColor, brushStyle);
        }
        painter.setBrush(patternBrush);

        // Create stroked path for pattern filling
        QPainterPathStroker stroker;
        stroker.setWidth(4);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);

        QPainterPath strokedPath = stroker.createStroke(path);
        painter.setPen(Qt::NoPen);
        painter.drawPath(strokedPath);
    }

    compositeAllLayers();

    // Calculate bounding rectangle for updates
    QRect boundingRect = path.boundingRect().toRect();
    int margin = (m_currentPattern == PatternBar::Solid) ? 2 : 3;
    boundingRect = boundingRect.adjusted(-margin, -margin, +margin, +margin);

    // Scale the update rectangle for widget coordinates
    QRect updateRect = QRect(boundingRect.topLeft() * m_scaleFactor,
                            boundingRect.bottomRight() * m_scaleFactor);
    update(updateRect);
}

void Canvas::drawSquare(const QPoint &startPoint, const QPoint &endPoint, bool filled)
{
    QPainter painter(&currentLayer().pixmap());
    painter.setPen(QPen(m_currentColor, 2, Qt::SolidLine));

    if (filled) {
        QBrush patternBrush;
        if (isCustomPattern(m_currentPattern)) {
            patternBrush = createCustomPatternBrush(m_currentPattern);
        } else {
            Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
            patternBrush = QBrush(m_currentColor, brushStyle);
        }
        painter.setBrush(patternBrush);
    } else {
        painter.setBrush(Qt::NoBrush);
    }

    // Create rectangle from start and end points
    QRect squareRect(startPoint, endPoint);
    painter.drawRect(squareRect.normalized());

    compositeAllLayers();

    // Calculate update rectangle
    QRect updateRect = squareRect.normalized();
    int margin = 2; // Add margin for line thickness
    updateRect = updateRect.adjusted(-margin, -margin, +margin, +margin);

    // Scale the update rectangle for the widget coordinates
    updateRect = QRect(updateRect.topLeft() * m_scaleFactor,
                      updateRect.bottomRight() * m_scaleFactor);
    update(updateRect);
}

void Canvas::drawRoundedSquare(const QPoint &startPoint, const QPoint &endPoint, bool filled)
{
    QPainter painter(&currentLayer().pixmap());
    painter.setPen(QPen(m_currentColor, 2, Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing);

    if (filled) {
        QBrush patternBrush;
        if (isCustomPattern(m_currentPattern)) {
            patternBrush = createCustomPatternBrush(m_currentPattern);
        } else {
            Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
            patternBrush = QBrush(m_currentColor, brushStyle);
        }
        painter.setBrush(patternBrush);
    } else {
        painter.setBrush(Qt::NoBrush);
    }

    // Create rectangle from start and end points
    QRect squareRect(startPoint, endPoint);
    QRect normalizedRect = squareRect.normalized();

    // Use corner radius that's proportional to the smaller dimension
    int cornerRadius = qMin(normalizedRect.width(), normalizedRect.height()) / 8;
    cornerRadius = qMax(cornerRadius, 3); // Minimum radius of 3 pixels
    cornerRadius = qMin(cornerRadius, 12); // Maximum radius of 12 pixels

    painter.drawRoundedRect(normalizedRect, cornerRadius, cornerRadius);

    compositeAllLayers();

    // Calculate update rectangle
    QRect updateRect = normalizedRect;
    int margin = 2; // Add margin for line thickness
    updateRect = updateRect.adjusted(-margin, -margin, +margin, +margin);

    // Scale the update rectangle for the widget coordinates
    updateRect = QRect(updateRect.topLeft() * m_scaleFactor,
                      updateRect.bottomRight() * m_scaleFactor);
    update(updateRect);
}

void Canvas::drawOval(const QPoint &startPoint, const QPoint &endPoint, bool filled)
{
    QPainter painter(&currentLayer().pixmap());
    painter.setPen(QPen(m_currentColor, 2, Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing);

    if (filled) {
        QBrush patternBrush;
        if (isCustomPattern(m_currentPattern)) {
            patternBrush = createCustomPatternBrush(m_currentPattern);
        } else {
            Qt::BrushStyle brushStyle = patternTypeToBrushStyle(m_currentPattern);
            patternBrush = QBrush(m_currentColor, brushStyle);
        }
        painter.setBrush(patternBrush);
    } else {
        painter.setBrush(Qt::NoBrush);
    }

    // Create rectangle from start and end points
    QRect ovalRect(startPoint, endPoint);
    QRect normalizedRect = ovalRect.normalized();

    painter.drawEllipse(normalizedRect);

    compositeAllLayers();

    // Calculate update rectangle
    QRect updateRect = normalizedRect;
    int margin = 2; // Add margin for line thickness
    updateRect = updateRect.adjusted(-margin, -margin, +margin, +margin);

    // Scale the update rectangle for the widget coordinates
    updateRect = QRect(updateRect.topLeft() * m_scaleFactor,
                      updateRect.bottomRight() * m_scaleFactor);
    update(updateRect);
}

void Canvas::setCurrentPattern(PatternBar::PatternType pattern)
{
    m_currentPattern = pattern;
}

void Canvas::setCurrentColor(const QColor &color)
{
    m_currentColor = color;
}

Qt::BrushStyle Canvas::patternTypeToBrushStyle(PatternBar::PatternType pattern)
{
    switch (pattern) {
        case PatternBar::Solid: return Qt::SolidPattern;
        case PatternBar::Dense1: return Qt::Dense1Pattern;
        case PatternBar::Dense2: return Qt::Dense2Pattern;
        case PatternBar::Dense3: return Qt::Dense3Pattern;
        case PatternBar::Dense4: return Qt::Dense4Pattern;
        case PatternBar::Dense5: return Qt::Dense5Pattern;
        case PatternBar::Dense6: return Qt::Dense6Pattern;
        case PatternBar::Dense7: return Qt::Dense7Pattern;
        case PatternBar::Horizontal: return Qt::HorPattern;
        case PatternBar::Vertical: return Qt::VerPattern;
        case PatternBar::Cross: return Qt::CrossPattern;
        case PatternBar::BDiag: return Qt::BDiagPattern;
        case PatternBar::FDiag: return Qt::FDiagPattern;
        case PatternBar::DiagCross: return Qt::DiagCrossPattern;
        // Custom patterns return solid and are handled separately
        case PatternBar::Dots:
        case PatternBar::Grid:
        case PatternBar::Circles:
        case PatternBar::Waves:
        case PatternBar::Stars:
        case PatternBar::Brick:
        case PatternBar::Hexagons:
        case PatternBar::Scales:
        case PatternBar::Zigzag:
        case PatternBar::Checkerboard:
        case PatternBar::Triangles:
        case PatternBar::Noise:
            return Qt::SolidPattern;
        default: return Qt::SolidPattern;
    }
}

bool Canvas::isCustomPattern(PatternBar::PatternType pattern)
{
    return pattern >= PatternBar::Dots;
}

QBrush Canvas::createCustomPatternBrush(PatternBar::PatternType pattern)
{
    // Create a pattern pixmap
    QPixmap patternPixmap(16, 16);
    patternPixmap.fill(Qt::white);

    QPainter painter(&patternPixmap);
    painter.setPen(QPen(m_currentColor, 1));
    drawCustomPattern(painter, pattern, 0, 0, 16, 16);

    return QBrush(patternPixmap);
}

void Canvas::drawCustomPattern(QPainter &painter, PatternBar::PatternType type, int x, int y, int width, int height)
{
    painter.setBrush(Qt::NoBrush);
    // Don't reset pen color here - use whatever was set by caller

    switch (type) {
        case PatternBar::Dots:
        {
            // Draw small dots in a grid pattern
            for (int i = x + 2; i < x + width - 1; i += 4) {
                for (int j = y + 2; j < y + height - 1; j += 4) {
                    painter.setBrush(painter.pen().color());
                    painter.setPen(Qt::NoPen);
                    painter.drawEllipse(i, j, 1, 1);
                }
            }
            break;
        }

        case PatternBar::Grid:
        {
            // Draw grid lines
            // Use pen color set by caller;
            for (int i = x; i <= x + width; i += 4) {
                painter.drawLine(i, y, i, y + height);
            }
            for (int j = y; j <= y + height; j += 4) {
                painter.drawLine(x, j, x + width, j);
            }
            break;
        }

        case PatternBar::Circles:
        {
            // Draw small circles in a grid pattern
            painter.setBrush(Qt::NoBrush);
            // Use pen color set by caller;
            for (int i = x + 2; i < x + width - 2; i += 6) {
                for (int j = y + 2; j < y + height - 2; j += 6) {
                    painter.drawEllipse(i, j, 4, 4);
                }
            }
            break;
        }

        case PatternBar::Waves:
        {
            // Draw wavy lines
            // Use pen color set by caller;
            for (int j = y + 2; j < y + height; j += 6) {
                QPainterPath wave;
                wave.moveTo(x, j);
                for (int i = x; i < x + width; i += 2) {
                    wave.quadTo(i + 1, j + ((i / 2) % 2 == 0 ? -2 : 2), i + 2, j);
                }
                painter.drawPath(wave);
            }
            break;
        }

        case PatternBar::Stars:
        {
            // Draw small star shapes
            painter.setBrush(painter.pen().color());
            painter.setPen(Qt::NoPen);
            for (int i = x + 4; i < x + width - 4; i += 8) {
                for (int j = y + 4; j < y + height - 4; j += 8) {
                    // Draw a simple 4-pointed star
                    painter.drawRect(i - 1, j - 2, 2, 4);
                    painter.drawRect(i - 2, j - 1, 4, 2);
                }
            }
            break;
        }

        case PatternBar::Brick:
        {
            // Draw brick pattern
            // Use pen color set by caller;
            bool offset = false;
            for (int j = y; j < y + height; j += 6) {
                int startX = offset ? x - 4 : x;
                for (int i = startX; i < x + width; i += 8) {
                    painter.drawRect(i, j, 8, 6);
                }
                offset = !offset;
            }
            break;
        }

        case PatternBar::Hexagons:
        {
            // Draw hexagon pattern
            // Use pen color set by caller;
            painter.setBrush(Qt::NoBrush);
            for (int j = y + 2; j < y + height - 2; j += 8) {
                for (int i = x + 4; i < x + width - 4; i += 10) {
                    int offsetY = ((i - x) / 10) % 2 == 0 ? 0 : 4;
                    QPolygon hexagon;
                    hexagon << QPoint(i, j + offsetY + 2)
                            << QPoint(i + 2, j + offsetY)
                            << QPoint(i + 6, j + offsetY)
                            << QPoint(i + 8, j + offsetY + 2)
                            << QPoint(i + 6, j + offsetY + 4)
                            << QPoint(i + 2, j + offsetY + 4);
                    painter.drawPolygon(hexagon);
                }
            }
            break;
        }

        case PatternBar::Scales:
        {
            // Draw fish scale pattern
            // Use pen color set by caller;
            painter.setBrush(Qt::NoBrush);
            for (int j = y; j < y + height; j += 4) {
                for (int i = x; i < x + width; i += 6) {
                    int offsetX = (j / 4) % 2 == 0 ? 0 : 3;
                    painter.drawArc(i + offsetX - 3, j - 2, 6, 4, 0, 180 * 16);
                }
            }
            break;
        }

        case PatternBar::Zigzag:
        {
            // Draw zigzag pattern
            // Use pen color set by caller;
            for (int j = y + 4; j < y + height; j += 8) {
                QPainterPath zigzag;
                zigzag.moveTo(x, j);
                for (int i = x; i < x + width; i += 4) {
                    zigzag.lineTo(i + 2, j + ((i / 4) % 2 == 0 ? -2 : 2));
                    zigzag.lineTo(i + 4, j);
                }
                painter.drawPath(zigzag);
            }
            break;
        }

        case PatternBar::Checkerboard:
        {
            // Draw checkerboard pattern
            painter.setPen(Qt::NoPen);
            painter.setBrush(painter.pen().color());
            for (int j = y; j < y + height; j += 4) {
                for (int i = x; i < x + width; i += 4) {
                    if ((i / 4 + j / 4) % 2 == 0) {
                        painter.drawRect(i, j, 4, 4);
                    }
                }
            }
            break;
        }

        case PatternBar::Triangles:
        {
            // Draw triangle pattern
            // Use pen color set by caller;
            painter.setBrush(Qt::NoBrush);
            for (int j = y + 6; j < y + height; j += 8) {
                for (int i = x + 4; i < x + width - 4; i += 8) {
                    QPolygon triangle;
                    triangle << QPoint(i, j - 4)
                             << QPoint(i - 3, j + 1)
                             << QPoint(i + 3, j + 1);
                    painter.drawPolygon(triangle);
                }
            }
            break;
        }

        case PatternBar::Noise:
        {
            // Draw random noise pattern
            painter.setPen(Qt::NoPen);
            painter.setBrush(painter.pen().color());
            // Simple pseudo-random pattern based on position
            for (int j = y; j < y + height; j += 1) {
                for (int i = x; i < x + width; i += 1) {
                    // Simple hash-like function for pseudo-random
                    int hash = ((i * 73) + (j * 37)) % 100;
                    if (hash < 20) {
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

// Undo/Redo system implementation
void Canvas::saveCanvasState()
{
    // This is called before an operation to save the current state
    // The actual undo command is created in the drawing operations
}

void Canvas::setCanvasPixmap(const QPixmap &pixmap)
{
    m_canvas = pixmap;
    update();
}

void Canvas::undo()
{
    m_undoStack->undo();
}

void Canvas::redo()
{
    m_undoStack->redo();
}

bool Canvas::canUndo() const
{
    return m_undoStack->canUndo();
}

bool Canvas::canRedo() const
{
    return m_undoStack->canRedo();
}

QUndoStack* Canvas::undoStack() const
{
    return m_undoStack;
}

bool Canvas::isPointInSelection(const QPoint &point) const
{
    if (!m_hasSelection || m_lassoPolygon.isEmpty()) {
        return false;
    }
    return m_lassoPolygon.containsPoint(point, Qt::OddEvenFill);
}

void Canvas::setPixelZoomMode(bool enabled)
{
    m_pixelZoomMode = enabled;
    if (enabled) {
        setFocusPolicy(Qt::StrongFocus);
        setFocus();
    }
    update();
}

void Canvas::movePixelCursor(int dx, int dy)
{
    if (!m_pixelZoomMode) return;

    QPoint newCenter = QPoint(qBound(0, m_zoomCenter.x() + dx, m_document->width() - 1),
                             qBound(0, m_zoomCenter.y() + dy, m_document->height() - 1));

    // Use setZoomCenter to automatically reposition magnifier
    setZoomCenter(newCenter);
}

void Canvas::setZoomCenter(const QPoint &center)
{
    m_zoomCenter = center;
    m_pixelCursor = center;

    // Automatically position magnifier near the editing area
    // Convert canvas coordinates to screen coordinates
    QPoint screenCenter = QPoint(center.x() * m_scaleFactor, center.y() * m_scaleFactor);

    // Calculate optimal magnifier position to avoid obscuring the editing area
    int magnifierOffset = m_magnifierRadius + 50; // Distance from editing area
    QPoint newMagnifierPos;

    // Try different positions in order of preference
    // 1. To the right of the editing area
    newMagnifierPos = QPoint(screenCenter.x() + magnifierOffset, screenCenter.y());
    if (newMagnifierPos.x() + m_magnifierRadius < width() - 20) {
        m_magnifierPosition = newMagnifierPos;
    }
    // 2. To the left of the editing area
    else if (screenCenter.x() - magnifierOffset - m_magnifierRadius > 20) {
        m_magnifierPosition = QPoint(screenCenter.x() - magnifierOffset, screenCenter.y());
    }
    // 3. Below the editing area
    else if (screenCenter.y() + magnifierOffset + m_magnifierRadius < height() - 20) {
        m_magnifierPosition = QPoint(screenCenter.x(), screenCenter.y() + magnifierOffset);
    }
    // 4. Above the editing area
    else if (screenCenter.y() - magnifierOffset - m_magnifierRadius > 20) {
        m_magnifierPosition = QPoint(screenCenter.x(), screenCenter.y() - magnifierOffset);
    }
    // 5. Default to top-right corner if no good position found
    else {
        m_magnifierPosition = QPoint(width() - m_magnifierRadius - 20, m_magnifierRadius + 20);
    }

    // Ensure magnifier stays within widget bounds
    m_magnifierPosition.setX(qBound(m_magnifierRadius + 10, m_magnifierPosition.x(), width() - m_magnifierRadius - 10));
    m_magnifierPosition.setY(qBound(m_magnifierRadius + 10, m_magnifierPosition.y(), height() - m_magnifierRadius - 10));

    update();
}

void Canvas::setShowCoordinates(bool show)
{
    m_showCoordinates = show;
    if (show) {
        setMouseTracking(true);  // Enable mouse tracking to show coordinates
    } else {
        setMouseTracking(false);
    }
    update();
}

// Layer management implementation (delegates to document)
void Canvas::setCurrentLayerIndex(int index)
{
    m_document->setCurrentLayerIndex(index);
    emit currentLayerChanged(index);
}

void Canvas::addLayer(const QString &name)
{
    m_document->addLayer(name);
    compositeAllLayers();
    emit layersChanged();
    emit currentLayerChanged(m_document->currentLayerIndex());
    update();
}

void Canvas::deleteLayer(int index)
{
    m_document->deleteLayer(index);
    compositeAllLayers();
    emit layersChanged();
    emit currentLayerChanged(m_document->currentLayerIndex());
    update();
}

void Canvas::duplicateLayer(int index)
{
    m_document->duplicateLayer(index);
    compositeAllLayers();
    emit layersChanged();
    emit currentLayerChanged(m_document->currentLayerIndex());
    update();
}

void Canvas::moveLayer(int fromIndex, int toIndex)
{
    m_document->moveLayer(fromIndex, toIndex);
    compositeAllLayers();
    emit layersChanged();
    emit currentLayerChanged(m_document->currentLayerIndex());
    update();
}

void Canvas::setLayerVisibility(int index, bool visible)
{
    m_document->setLayerVisibility(index, visible);
    compositeAllLayers();
    emit layersChanged();
    update();
}

void Canvas::setLayerOpacity(int index, qreal opacity)
{
    m_document->setLayerOpacity(index, opacity);
    compositeAllLayers();
    emit layersChanged();
    update();
}

void Canvas::setLayerBlendMode(int index, Layer::BlendMode mode)
{
    m_document->setLayerBlendMode(index, mode);
    compositeAllLayers();
    emit layersChanged();
    update();
}

void Canvas::setLayerName(int index, const QString &name)
{
    m_document->setLayerName(index, name);
    emit layersChanged();
}

void Canvas::compositeAllLayers()
{
    m_canvas = m_document->composite();
}

void Canvas::updateCanvasSize()
{
    // Show only one page at a time
    setFixedSize(m_document->width() * m_scaleFactor, m_document->height() * m_scaleFactor);
}

void Canvas::performScissorsCut(const QPolygon &cutLine)
{
    if (cutLine.size() < 2) {
        return;
    }

    // Get the current layer pixmap
    QPixmap layerPixmap = currentLayer().pixmap();
    QImage layerImage = layerPixmap.toImage();

    // Create a thick cutting line path (scissors blade width)
    QPainterPath cutPath;
    QPainterPathStroker stroker;
    stroker.setWidth(3); // Width of scissors cut

    cutPath.moveTo(cutLine.first());
    for (int i = 1; i < cutLine.size(); ++i) {
        cutPath.lineTo(cutLine.at(i));
    }

    QPainterPath thickCutPath = stroker.createStroke(cutPath);

    // Determine which side is "left" and which is "right" of the cut
    // We'll use a flood fill approach from opposite corners
    QRect bounds = layerImage.rect();

    // Create masks for both regions
    QImage mask1(bounds.size(), QImage::Format_ARGB32);
    QImage mask2(bounds.size(), QImage::Format_ARGB32);
    mask1.fill(Qt::transparent);
    mask2.fill(Qt::transparent);

    // Paint the cut path onto a mask
    QImage cutMask(bounds.size(), QImage::Format_ARGB32);
    cutMask.fill(Qt::transparent);
    QPainter cutPainter(&cutMask);
    cutPainter.fillPath(thickCutPath, Qt::white);

    // Flood fill from top-left for region 1
    QStack<QPoint> stack1;
    stack1.push(QPoint(0, 0));
    while (!stack1.isEmpty()) {
        QPoint p = stack1.pop();
        if (p.x() < 0 || p.x() >= bounds.width() || p.y() < 0 || p.y() >= bounds.height()) continue;
        if (mask1.pixelColor(p) != Qt::transparent) continue;
        if (cutMask.pixelColor(p).alpha() > 0) continue;

        mask1.setPixelColor(p, Qt::white);
        stack1.push(QPoint(p.x() + 1, p.y()));
        stack1.push(QPoint(p.x() - 1, p.y()));
        stack1.push(QPoint(p.x(), p.y() + 1));
        stack1.push(QPoint(p.x(), p.y() - 1));
    }

    // Flood fill from bottom-right for region 2
    QStack<QPoint> stack2;
    stack2.push(QPoint(bounds.width() - 1, bounds.height() - 1));
    while (!stack2.isEmpty()) {
        QPoint p = stack2.pop();
        if (p.x() < 0 || p.x() >= bounds.width() || p.y() < 0 || p.y() >= bounds.height()) continue;
        if (mask2.pixelColor(p) != Qt::transparent) continue;
        if (cutMask.pixelColor(p).alpha() > 0) continue;
        if (mask1.pixelColor(p) != Qt::transparent) continue;

        mask2.setPixelColor(p, Qt::white);
        stack2.push(QPoint(p.x() + 1, p.y()));
        stack2.push(QPoint(p.x() - 1, p.y()));
        stack2.push(QPoint(p.x(), p.y() + 1));
        stack2.push(QPoint(p.x(), p.y() - 1));
    }

    // Create pixmaps for both pieces
    m_scissorsPiece1 = QPixmap(bounds.size());
    m_scissorsPiece2 = QPixmap(bounds.size());
    m_scissorsPiece1.fill(Qt::transparent);
    m_scissorsPiece2.fill(Qt::transparent);

    // Extract pixels for each piece
    QImage piece1 = m_scissorsPiece1.toImage();
    QImage piece2 = m_scissorsPiece2.toImage();

    for (int y = 0; y < bounds.height(); ++y) {
        for (int x = 0; x < bounds.width(); ++x) {
            if (mask1.pixelColor(x, y).alpha() > 0) {
                piece1.setPixelColor(x, y, layerImage.pixelColor(x, y));
            }
            if (mask2.pixelColor(x, y).alpha() > 0) {
                piece2.setPixelColor(x, y, layerImage.pixelColor(x, y));
            }
        }
    }

    m_scissorsPiece1 = QPixmap::fromImage(piece1);
    m_scissorsPiece2 = QPixmap::fromImage(piece2);

    // Create regions from masks
    m_scissorsRegion1 = QRegion();
    m_scissorsRegion2 = QRegion();

    for (int y = 0; y < bounds.height(); ++y) {
        for (int x = 0; x < bounds.width(); ++x) {
            if (mask1.pixelColor(x, y).alpha() > 0) {
                m_scissorsRegion1 += QRect(x, y, 1, 1);
            }
            if (mask2.pixelColor(x, y).alpha() > 0) {
                m_scissorsRegion2 += QRect(x, y, 1, 1);
            }
        }
    }

    // Clear the current layer
    currentLayer().pixmap().fill(Qt::transparent);

    // Initialize piece offsets
    m_piece1Offset = QPoint(0, 0);
    m_piece2Offset = QPoint(0, 0);

    m_hasScissorsPieces = true;
    compositeAllLayers();
    update();
}