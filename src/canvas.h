#pragma once

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QContextMenuEvent>
#include <QLineEdit>
#include <QFont>
#include <QPolygon>
#include <QClipboard>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QUndoStack>
#include <QUndoCommand>
#include "patternbar.h"
#include "core/Layer.h"
#include "core/Document.h"

using Unimalen::Layer;
using Unimalen::Document;

class Canvas : public QWidget
{
    Q_OBJECT

signals:
    void layersChanged();
    void currentLayerChanged(int index);
    void documentModified();
    void mousePositionChanged(int x, int y); // New: emit mouse position for status bar
    void colorPicked(const QColor &color); // New: emit when eyedropper picks a color

public:
    explicit Canvas(QWidget *parent = nullptr);

    void setScaleFactor(int factor);
    void setZoomLevel(double zoomPercent); // New: support arbitrary zoom levels
    int getScaleFactor() const { return m_scaleFactor; }
    double getZoomLevel() const { return m_zoomLevel; } // New: get current zoom percentage
    void setPixelZoomMode(bool enabled);
    bool isPixelZoomMode() const { return m_pixelZoomMode; }
    void movePixelCursor(int dx, int dy);
    void setZoomCenter(const QPoint &center);
    void setShowCoordinates(bool show);
    bool isShowCoordinates() const { return m_showCoordinates; }

    void newCanvas();
    bool loadCanvas(const QString &fileName);
    bool saveCanvas(const QString &fileName);

    void setPencilMode(bool enabled) { m_pencilMode = enabled; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isPencilMode() const { return m_pencilMode; }

    void setTextMode(bool enabled) { m_textMode = enabled; m_pencilMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isTextMode() const { return m_textMode; }

    void setSprayMode(bool enabled, int diameter) { m_sprayMode = enabled; m_sprayDiameter = diameter; m_pencilMode = false; m_textMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isSprayMode() const { return m_sprayMode; }

    void setBrushMode(bool enabled, int diameter) { m_brushMode = enabled; m_brushDiameter = diameter; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isBrushMode() const { return m_brushMode; }

    void setMarkerMode(bool enabled) { m_markerMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isMarkerMode() const { return m_markerMode; }

    void setEraserMode(bool enabled, int diameter) { m_eraserMode = enabled; m_eraserDiameter = diameter; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isEraserMode() const { return m_eraserMode; }

    void setLineMode(bool enabled) { m_lineMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isLineMode() const { return m_lineMode; }

    void setFillMode(bool enabled) { m_fillMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isFillMode() const { return m_fillMode; }

    void setLassoMode(bool enabled) { m_lassoMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isLassoMode() const { return m_lassoMode; }

    void setSquareMode(bool enabled) { m_squareMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isSquareMode() const { return m_squareMode; }

    void setFilledSquareMode(bool enabled) { m_filledSquareMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isFilledSquareMode() const { return m_filledSquareMode; }

    void setRoundedSquareMode(bool enabled) { m_roundedSquareMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isRoundedSquareMode() const { return m_roundedSquareMode; }

    void setFilledRoundedSquareMode(bool enabled) { m_filledRoundedSquareMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isFilledRoundedSquareMode() const { return m_filledRoundedSquareMode; }

    void setOvalMode(bool enabled) { m_ovalMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isOvalMode() const { return m_ovalMode; }

    void setFilledOvalMode(bool enabled) { m_filledOvalMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isFilledOvalMode() const { return m_filledOvalMode; }

    void setBezierMode(bool enabled) { m_bezierMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_showLinePreview = false; clearSelection(); }
    bool isBezierMode() const { return m_bezierMode; }

    void setScissorsMode(bool enabled) { m_scissorsMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_rectSelectMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isScissorsMode() const { return m_scissorsMode; }

    void setRectSelectMode(bool enabled) { m_rectSelectMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_scissorsMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isRectSelectMode() const { return m_rectSelectMode; }

    void setEyedropperMode(bool enabled) { m_eyedropperMode = enabled; m_pencilMode = false; m_textMode = false; m_sprayMode = false; m_brushMode = false; m_markerMode = false; m_eraserMode = false; m_lineMode = false; m_fillMode = false; m_lassoMode = false; m_rectSelectMode = false; m_scissorsMode = false; m_squareMode = false; m_filledSquareMode = false; m_roundedSquareMode = false; m_filledRoundedSquareMode = false; m_ovalMode = false; m_filledOvalMode = false; m_bezierMode = false; m_showLinePreview = false; clearSelection(); }
    bool isEyedropperMode() const { return m_eyedropperMode; }

    // Selection operations
    void cutSelection();
    void copySelection();
    void pasteSelection();
    void pasteSelectionAt(const QPoint &position);

    // Transform operations
    void rotateSelection(int degrees);  // 90, 180, 270, or custom angle
    void flipSelectionHorizontal();
    void flipSelectionVertical();

    // Image insertion
    void insertImageAt(const QPixmap &image, const QPoint &position);

    void setTextFont(const QString &fontFamily);
    void setTextFontSize(int fontSize);
    void setCurrentPattern(PatternBar::PatternType pattern);
    void setCurrentColor(const QColor &color);
    QColor currentColor() const { return m_currentColor; }

    void setPanMode(bool enabled) { m_panMode = enabled; }
    bool isPanMode() const { return m_panMode; }

    // Document access
    Document* document() { return m_document; }
    const Document* document() const { return m_document; }

    // Layer management (delegates to document)
    QList<Layer>& layers() { return m_document->layers(); }
    const QList<Layer>& layers() const { return m_document->layers(); }
    int currentLayerIndex() const { return m_document->currentLayerIndex(); }
    void setCurrentLayerIndex(int index);
    Layer& currentLayer() { return m_document->currentLayer(); }
    const Layer& currentLayer() const { return m_document->currentLayer(); }

    void addLayer(const QString &name = QString());
    void deleteLayer(int index);
    void duplicateLayer(int index);
    void moveLayer(int fromIndex, int toIndex);
    void setLayerVisibility(int index, bool visible);
    void setLayerOpacity(int index, qreal opacity);
    void setLayerBlendMode(int index, Layer::BlendMode mode);
    void setLayerName(int index, const QString &name);

    // Undo/Redo system
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;
    QUndoStack* undoStack() const;
    void setCanvasPixmap(const QPixmap &pixmap);

    // Compositing (public for MainWindow access)
    void compositeAllLayers();

    // Multi-page support
    void updateCanvasSize();

    // Document state
    bool isModified() const { return m_isModified; }
    void setModified(bool modified);
    QString filePath() const { return m_filePath; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void finishTextInput();
    void commitText();

private:
    QPoint mapToCanvas(const QPoint &point);
    void drawLineTo(const QPoint &endPoint);
    void drawStraightLine(const QPoint &startPoint, const QPoint &endPoint);
    void drawBezierCurve(const QPoint &p0, const QPoint &p1, const QPoint &p2, const QPoint &p3);
    void drawSquare(const QPoint &startPoint, const QPoint &endPoint, bool filled);
    void drawRoundedSquare(const QPoint &startPoint, const QPoint &endPoint, bool filled);
    void drawOval(const QPoint &startPoint, const QPoint &endPoint, bool filled);
    void startTextInput(const QPoint &position);
    void sprayPaint(const QPoint &position);
    void performScissorsCut(const QPolygon &cutLine);
    void brushPaint(const QPoint &position);
    void markerPaint(const QPoint &position);
    void eraserPaint(const QPoint &position);
    void floodFill(const QPoint &position, const QColor &fillColor);
    void clearSelection();
    Qt::BrushStyle patternTypeToBrushStyle(PatternBar::PatternType pattern);
    QBrush createCustomPatternBrush(PatternBar::PatternType pattern);
    void drawCustomPattern(QPainter &painter, PatternBar::PatternType type, int x, int y, int width, int height);
    bool isCustomPattern(PatternBar::PatternType pattern);
    bool saveAsORA(const QString &fileName);
    bool loadFromORA(const QString &fileName);

    static constexpr int CANVAS_WIDTH = 576;
    static constexpr int CANVAS_HEIGHT = 720;
    static constexpr int DPI = 72;

    QPixmap m_canvas;
    Document* m_document;
    int m_scaleFactor;
    double m_zoomLevel; // New: arbitrary zoom level (percentage, 100.0 = 100%)
    bool m_drawing;
    bool m_pixelZoomMode;
    QPoint m_pixelCursor;
    int m_pixelZoomFactor;
    QPoint m_zoomCenter;
    int m_magnifierRadius;
    QPoint m_magnifierPosition;
    bool m_drawingInMagnifier;
    bool m_showCoordinates;
    QPoint m_mousePosition;
    bool m_panMode;
    bool m_panning;
    QPoint m_panStartPos;
    bool m_pencilMode;
    bool m_textMode;
    bool m_sprayMode;
    bool m_brushMode;
    bool m_markerMode;
    bool m_eraserMode;
    bool m_lineMode;
    bool m_fillMode;
    bool m_lassoMode;
    bool m_rectSelectMode;
    bool m_eyedropperMode;
    bool m_squareMode;
    bool m_filledSquareMode;
    bool m_roundedSquareMode;
    bool m_filledRoundedSquareMode;
    bool m_ovalMode;
    bool m_filledOvalMode;
    bool m_bezierMode;
    bool m_scissorsMode;
    int m_sprayDiameter;
    int m_brushDiameter;
    int m_eraserDiameter;
    QPoint m_lastPoint;
    QPoint m_lineStartPoint;
    QPoint m_lineCurrentPoint;
    QPoint m_bezierPoints[4];  // Start, control1, control2, end
    int m_bezierClickCount;
    bool m_showLinePreview;
    QPoint m_squareStartPoint;
    QPoint m_squareCurrentPoint;
    bool m_showSquarePreview;
    QPoint m_roundedSquareStartPoint;
    QPoint m_roundedSquareCurrentPoint;
    bool m_showRoundedSquarePreview;
    QPoint m_ovalStartPoint;
    QPoint m_ovalCurrentPoint;
    bool m_showOvalPreview;

    // Scissors tool
    QPolygon m_scissorsCutLine;
    bool m_drawingScissors;
    bool m_hasScissorsPieces;
    QPixmap m_scissorsPiece1;
    QPixmap m_scissorsPiece2;
    QRegion m_scissorsRegion1;
    QRegion m_scissorsRegion2;
    QPoint m_piece1Offset;
    QPoint m_piece2Offset;
    bool m_draggingPiece1;
    bool m_draggingPiece2;
    QPoint m_pieceDragStart;

    // Lasso selection
    QPolygon m_lassoPolygon;
    bool m_hasSelection;
    bool m_drawingLasso;

    // Rectangle selection
    QRect m_rectSelection;
    QPoint m_rectSelectStart;
    QPoint m_rectSelectCurrent;
    bool m_drawingRectSelect;
    QPixmap m_clipboard;

    // Selection dragging
    bool m_draggingSelection;
    QPoint m_dragStartPoint;
    QPixmap m_selectedPixmap;
    QPoint m_selectionOffset;
    bool isPointInSelection(const QPoint &point) const;

    QLineEdit *m_textEdit;
    QPoint m_textPosition;
    QFont m_textFont;
    bool m_textMovable;
    bool m_movingText;
    QPoint m_textDragStart;
    PatternBar::PatternType m_currentPattern;
    QColor m_currentColor;

    // Undo/Redo system
    QUndoStack *m_undoStack;
    QPixmap m_canvasBeforeEdit;
    void saveCanvasState();

    // Document state
    bool m_isModified;
    QString m_filePath;
};