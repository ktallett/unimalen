#pragma once

#include <QPoint>
#include <QString>
#include <QPixmap>
#include <QColor>

class QPainter;
class QMouseEvent;
class QKeyEvent;

namespace Unimalen {

class Document;
class Layer;

// Context provided to tools for drawing operations
class ToolContext
{
public:
    ToolContext(Document *document, Layer *layer, const QColor &foregroundColor, const QColor &backgroundColor)
        : m_document(document)
        , m_layer(layer)
        , m_foregroundColor(foregroundColor)
        , m_backgroundColor(backgroundColor)
        , m_lineThickness(1)
    {}

    Document* document() const { return m_document; }
    Layer* layer() const { return m_layer; }
    QColor foregroundColor() const { return m_foregroundColor; }
    QColor backgroundColor() const { return m_backgroundColor; }
    int lineThickness() const { return m_lineThickness; }
    void setLineThickness(int thickness) { m_lineThickness = thickness; }

private:
    Document *m_document;
    Layer *m_layer;
    QColor m_foregroundColor;
    QColor m_backgroundColor;
    int m_lineThickness;
};

// Abstract base class for all drawing tools
class Tool
{
public:
    virtual ~Tool() = default;

    // Tool identification
    virtual QString name() const = 0;

    // Mouse event handlers
    virtual void onPress(const QPoint &pos, ToolContext &ctx) = 0;
    virtual void onMove(const QPoint &pos, ToolContext &ctx) = 0;
    virtual void onRelease(const QPoint &pos, ToolContext &ctx) = 0;

    // Keyboard event handler (optional)
    virtual void onKeyPress(QKeyEvent *event, ToolContext &ctx) { Q_UNUSED(event); Q_UNUSED(ctx); }

    // Render any tool-specific overlay/preview
    virtual void renderOverlay(QPainter &painter) const {}

    // Called when tool becomes active/inactive
    virtual void onActivate() {}
    virtual void onDeactivate() {}

    // Check if tool has unsaved changes (for tool switching)
    virtual bool hasUncommittedChanges() const { return false; }
    virtual void commitChanges(ToolContext &ctx) { Q_UNUSED(ctx); }

protected:
    Tool() = default;
};

} // namespace Unimalen
