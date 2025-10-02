#pragma once

#include "Tool.h"
#include <QPoint>
#include <QPixmap>

namespace Unimalen {

class LineTool : public Tool
{
public:
    LineTool();
    ~LineTool() override = default;

    QString name() const override { return "Line"; }

    void onPress(const QPoint &pos, ToolContext &ctx) override;
    void onMove(const QPoint &pos, ToolContext &ctx) override;
    void onRelease(const QPoint &pos, ToolContext &ctx) override;

    void renderOverlay(QPainter &painter) const override;

    bool hasUncommittedChanges() const override { return m_drawing; }
    void commitChanges(ToolContext &ctx) override;

private:
    void drawStraightLine(const QPoint &startPoint, const QPoint &endPoint, ToolContext &ctx);

    bool m_drawing;
    QPoint m_startPoint;
    QPoint m_currentPoint;
    QPixmap m_beforeDrawing;
};

} // namespace Unimalen
