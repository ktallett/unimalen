#pragma once

#include "Tool.h"
#include <QPoint>
#include <QPixmap>

namespace Unimalen {

class PencilTool : public Tool
{
public:
    PencilTool();
    ~PencilTool() override = default;

    QString name() const override { return "Pencil"; }

    void onPress(const QPoint &pos, ToolContext &ctx) override;
    void onMove(const QPoint &pos, ToolContext &ctx) override;
    void onRelease(const QPoint &pos, ToolContext &ctx) override;

    bool hasUncommittedChanges() const override { return m_drawing; }
    void commitChanges(ToolContext &ctx) override;

private:
    void drawLineTo(const QPoint &endPoint, ToolContext &ctx);

    bool m_drawing;
    QPoint m_lastPoint;
    QPixmap m_beforeDrawing;  // For undo
};

} // namespace Unimalen
