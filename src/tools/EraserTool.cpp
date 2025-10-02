#include "EraserTool.h"
#include "../core/Layer.h"
#include <QPainter>

namespace Unimalen {

EraserTool::EraserTool(int diameter)
    : m_erasing(false)
    , m_diameter(diameter)
{
}

void EraserTool::onPress(const QPoint &pos, ToolContext &ctx)
{
    m_erasing = true;
    m_beforeErasing = ctx.layer()->pixmap().copy();
    eraseAt(pos, ctx);
}

void EraserTool::onMove(const QPoint &pos, ToolContext &ctx)
{
    if (m_erasing) {
        eraseAt(pos, ctx);
    }
}

void EraserTool::onRelease(const QPoint &pos, ToolContext &ctx)
{
    if (m_erasing) {
        eraseAt(pos, ctx);
        commitChanges(ctx);
        m_erasing = false;
    }
}

void EraserTool::commitChanges(ToolContext &ctx)
{
    // This is where we'd push an undo command
    m_beforeErasing = QPixmap();
}

void EraserTool::eraseAt(const QPoint &position, ToolContext &ctx)
{
    QPainter painter(&ctx.layer()->pixmap());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));

    int radius = m_diameter / 2;
    QRect eraserRect(position.x() - radius, position.y() - radius,
                     m_diameter, m_diameter);

    painter.drawEllipse(eraserRect);
}

} // namespace Unimalen
