#include "LineTool.h"
#include "../core/Layer.h"
#include <QPainter>

namespace Unimalen {

LineTool::LineTool()
    : m_drawing(false)
{
}

void LineTool::onPress(const QPoint &pos, ToolContext &ctx)
{
    m_drawing = true;
    m_startPoint = pos;
    m_currentPoint = pos;
    m_beforeDrawing = ctx.layer()->pixmap().copy();
}

void LineTool::onMove(const QPoint &pos, ToolContext &ctx)
{
    if (m_drawing) {
        m_currentPoint = pos;
    }
}

void LineTool::onRelease(const QPoint &pos, ToolContext &ctx)
{
    if (m_drawing) {
        m_currentPoint = pos;
        drawStraightLine(m_startPoint, m_currentPoint, ctx);
        commitChanges(ctx);
        m_drawing = false;
    }
}

void LineTool::renderOverlay(QPainter &painter) const
{
    if (m_drawing) {
        painter.setPen(QPen(Qt::black, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(m_startPoint, m_currentPoint);
    }
}

void LineTool::commitChanges(ToolContext &ctx)
{
    // This is where we'd push an undo command
    m_beforeDrawing = QPixmap();
}

void LineTool::drawStraightLine(const QPoint &startPoint, const QPoint &endPoint, ToolContext &ctx)
{
    QPainter painter(&ctx.layer()->pixmap());
    painter.setPen(QPen(ctx.foregroundColor(), ctx.lineThickness(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(startPoint, endPoint);
}

} // namespace Unimalen
