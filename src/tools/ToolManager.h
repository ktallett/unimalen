#pragma once

#include "Tool.h"
#include <QObject>
#include <QHash>
#include <memory>

namespace Unimalen {

// Manages the collection of available tools and the currently active tool
class ToolManager : public QObject
{
    Q_OBJECT

public:
    explicit ToolManager(QObject *parent = nullptr);
    ~ToolManager() override;

    // Tool registration
    void registerTool(const QString &id, Tool *tool);
    Tool* tool(const QString &id) const;

    // Active tool management
    void setActiveTool(const QString &id);
    Tool* activeTool() const { return m_activeTool; }
    QString activeToolId() const { return m_activeToolId; }

signals:
    void activeToolChanged(const QString &toolId);

private:
    QHash<QString, Tool*> m_tools;
    Tool *m_activeTool;
    QString m_activeToolId;
};

} // namespace Unimalen
