#include "BrushTool.h"
#include "../core/Layer.h"
#include <QPainter>

namespace Unimalen {

BrushTool::BrushTool(int diameter)
    : m_painting(false)
    , m_diameter(diameter)
{
}

void BrushTool::onPress(const QPoint &pos, ToolContext &ctx)
{
    m_painting = true;
    m_beforePainting = ctx.layer()->pixmap().copy();
    paintAt(pos, ctx);
}

void BrushTool::onMove(const QPoint &pos, ToolContext &ctx)
{
    if (m_painting) {
        paintAt(pos, ctx);
    }
}

void BrushTool::onRelease(const QPoint &pos, ToolContext &ctx)
{
    if (m_painting) {
        paintAt(pos, ctx);
        commitChanges(ctx);
        m_painting = false;
    }
}

void BrushTool::commitChanges(ToolContext &ctx)
{
    // This is where we'd push an undo command
    m_beforePainting = QPixmap();
}

void BrushTool::paintAt(const QPoint &position, ToolContext &ctx)
{
    QPainter painter(&ctx.layer()->pixmap());
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(ctx.foregroundColor()));

    int radius = m_diameter / 2;
    QRect brushRect(position.x() - radius, position.y() - radius,
                    m_diameter, m_diameter);

    painter.drawEllipse(brushRect);
}

} // namespace Unimalen
