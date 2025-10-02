#include "PencilTool.h"
#include "../core/Layer.h"
#include <QPainter>

namespace Unimalen {

PencilTool::PencilTool()
    : m_drawing(false)
{
}

void PencilTool::onPress(const QPoint &pos, ToolContext &ctx)
{
    m_drawing = true;
    m_lastPoint = pos;

    // Save state for undo
    m_beforeDrawing = ctx.layer()->pixmap().copy();

    // Draw single pixel at start point
    QPainter painter(&ctx.layer()->pixmap());
    painter.setPen(QPen(ctx.foregroundColor(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPoint(pos);
}

void PencilTool::onMove(const QPoint &pos, ToolContext &ctx)
{
    if (m_drawing) {
        drawLineTo(pos, ctx);
        m_lastPoint = pos;
    }
}

void PencilTool::onRelease(const QPoint &pos, ToolContext &ctx)
{
    if (m_drawing) {
        drawLineTo(pos, ctx);
        commitChanges(ctx);
        m_drawing = false;
    }
}

void PencilTool::commitChanges(ToolContext &ctx)
{
    // This is where we'd push an undo command
    // For now, just clear the before state
    m_beforeDrawing = QPixmap();
}

void PencilTool::drawLineTo(const QPoint &endPoint, ToolContext &ctx)
{
    QPainter painter(&ctx.layer()->pixmap());
    painter.setPen(QPen(ctx.foregroundColor(), ctx.lineThickness(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(m_lastPoint, endPoint);
}

} // namespace Unimalen
