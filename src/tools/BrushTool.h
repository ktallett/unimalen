#pragma once

#include "Tool.h"
#include <QPoint>
#include <QPixmap>

namespace Unimalen {

class BrushTool : public Tool
{
public:
    explicit BrushTool(int diameter = 10);
    ~BrushTool() override = default;

    QString name() const override { return "Brush"; }

    void onPress(const QPoint &pos, ToolContext &ctx) override;
    void onMove(const QPoint &pos, ToolContext &ctx) override;
    void onRelease(const QPoint &pos, ToolContext &ctx) override;

    bool hasUncommittedChanges() const override { return m_painting; }
    void commitChanges(ToolContext &ctx) override;

    void setDiameter(int diameter) { m_diameter = diameter; }
    int diameter() const { return m_diameter; }

private:
    void paintAt(const QPoint &position, ToolContext &ctx);

    bool m_painting;
    int m_diameter;
    QPixmap m_beforePainting;
};

} // namespace Unimalen
