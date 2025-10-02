#include "ToolManager.h"

namespace Unimalen {

ToolManager::ToolManager(QObject *parent)
    : QObject(parent)
    , m_activeTool(nullptr)
{
}

ToolManager::~ToolManager() = default;

void ToolManager::registerTool(const QString &id, Tool *tool)
{
    m_tools[id] = tool;
}

Tool* ToolManager::tool(const QString &id) const
{
    return m_tools.value(id, nullptr);
}

void ToolManager::setActiveTool(const QString &id)
{
    if (m_activeToolId == id) {
        return;
    }

    Tool *newTool = tool(id);
    if (!newTool) {
        return;
    }

    // Deactivate current tool
    if (m_activeTool) {
        m_activeTool->onDeactivate();
    }

    // Activate new tool
    m_activeTool = newTool;
    m_activeToolId = id;
    m_activeTool->onActivate();

    emit activeToolChanged(id);
}

} // namespace Unimalen
