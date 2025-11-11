#include "MarkerTool.h"
#include "../core/Layer.h"
#include <QPainter>

namespace Unimalen {

MarkerTool::MarkerTool()
    : m_drawing(false)
{
}

void MarkerTool::onPress(const QPoint &pos, ToolContext &ctx)
{
    m_drawing = true;
    m_lastPoint = pos;

    // Save state for undo
    m_beforeDrawing = ctx.layer()->pixmap().copy();

    // Draw single marker stroke at start point
    QPainter painter(&ctx.layer()->pixmap());
    painter.setRenderHint(QPainter::Antialiasing);

    // Make marker semi-transparent for realistic marker effect
    QColor markerColor = ctx.foregroundColor();
    markerColor.setAlpha(180);  // 70% opacity for marker transparency

    painter.setPen(QPen(markerColor, ctx.lineThickness() * 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPoint(pos);
}

void MarkerTool::onMove(const QPoint &pos, ToolContext &ctx)
{
    if (m_drawing) {
        drawLineTo(pos, ctx);
        m_lastPoint = pos;
    }
}

void MarkerTool::onRelease(const QPoint &pos, ToolContext &ctx)
{
    if (m_drawing) {
        drawLineTo(pos, ctx);
        commitChanges(ctx);
        m_drawing = false;
    }
}

void MarkerTool::commitChanges(ToolContext &ctx)
{
    // This is where we'd push an undo command
    // For now, just clear the before state
    m_beforeDrawing = QPixmap();
}

void MarkerTool::drawLineTo(const QPoint &endPoint, ToolContext &ctx)
{
    QPainter painter(&ctx.layer()->pixmap());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    // Make marker semi-transparent for realistic marker effect
    QColor markerColor = ctx.foregroundColor();
    markerColor.setAlpha(180);  // 70% opacity for marker transparency

    // Marker pen with medium width and smooth edges
    painter.setPen(QPen(markerColor, ctx.lineThickness() * 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(m_lastPoint, endPoint);
}

} // namespace Unimalen
