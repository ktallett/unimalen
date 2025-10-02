#pragma once

#include "Tool.h"
#include <QPoint>
#include <QPixmap>

namespace Unimalen {

class EraserTool : public Tool
{
public:
    explicit EraserTool(int diameter = 10);
    ~EraserTool() override = default;

    QString name() const override { return "Eraser"; }

    void onPress(const QPoint &pos, ToolContext &ctx) override;
    void onMove(const QPoint &pos, ToolContext &ctx) override;
    void onRelease(const QPoint &pos, ToolContext &ctx) override;

    bool hasUncommittedChanges() const override { return m_erasing; }
    void commitChanges(ToolContext &ctx) override;

    void setDiameter(int diameter) { m_diameter = diameter; }
    int diameter() const { return m_diameter; }

private:
    void eraseAt(const QPoint &position, ToolContext &ctx);

    bool m_erasing;
    int m_diameter;
    QPixmap m_beforeErasing;
};

} // namespace Unimalen
