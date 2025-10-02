#include "Page.h"
#include <QPainter>

namespace Unimalen {

Page::Page(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_currentLayerIndex(0)
{
    // Create initial layer
    addLayer("Layer 1");
}

void Page::setCurrentLayerIndex(int index)
{
    if (index >= 0 && index < m_layers.size()) {
        m_currentLayerIndex = index;
    }
}

Layer& Page::currentLayer()
{
    if (m_layers.isEmpty()) {
        addLayer("Layer 1");
    }
    if (m_currentLayerIndex < 0 || m_currentLayerIndex >= m_layers.size()) {
        m_currentLayerIndex = 0;
    }
    return m_layers[m_currentLayerIndex];
}

const Layer& Page::currentLayer() const
{
    return const_cast<Page*>(this)->currentLayer();
}

void Page::addLayer(const QString &name)
{
    // Limit to MAX_LAYERS_PER_PAGE for zine mode
    if (m_layers.size() >= MAX_LAYERS_PER_PAGE) {
        return;
    }

    QString layerName = name.isEmpty() ? QString("Layer %1").arg(m_layers.size() + 1) : name;
    m_layers.append(Layer(layerName, m_width, m_height));
    m_currentLayerIndex = m_layers.size() - 1;
}

void Page::deleteLayer(int index)
{
    if (index >= 0 && index < m_layers.size() && m_layers.size() > 1) {
        m_layers.removeAt(index);
        if (m_currentLayerIndex >= m_layers.size()) {
            m_currentLayerIndex = m_layers.size() - 1;
        }
    }
}

void Page::duplicateLayer(int index)
{
    // Check layer limit
    if (m_layers.size() >= MAX_LAYERS_PER_PAGE) {
        return;
    }

    if (index >= 0 && index < m_layers.size()) {
        Layer duplicate = m_layers[index].duplicate();
        duplicate.setName(m_layers[index].name() + " copy");
        m_layers.insert(index + 1, duplicate);
        m_currentLayerIndex = index + 1;
    }
}

void Page::moveLayer(int fromIndex, int toIndex)
{
    if (fromIndex >= 0 && fromIndex < m_layers.size() &&
        toIndex >= 0 && toIndex < m_layers.size() && fromIndex != toIndex) {
        m_layers.move(fromIndex, toIndex);
        if (m_currentLayerIndex == fromIndex) {
            m_currentLayerIndex = toIndex;
        }
    }
}

void Page::setLayerVisibility(int index, bool visible)
{
    if (index >= 0 && index < m_layers.size()) {
        m_layers[index].setVisible(visible);
    }
}

void Page::setLayerOpacity(int index, qreal opacity)
{
    if (index >= 0 && index < m_layers.size()) {
        m_layers[index].setOpacity(opacity);
    }
}

void Page::setLayerBlendMode(int index, Layer::BlendMode mode)
{
    if (index >= 0 && index < m_layers.size()) {
        m_layers[index].setBlendMode(mode);
    }
}

void Page::setLayerName(int index, const QString &name)
{
    if (index >= 0 && index < m_layers.size()) {
        m_layers[index].setName(name);
    }
}

QPixmap Page::composite() const
{
    QPixmap result(m_width, m_height);
    compositeToPixmap(result);
    return result;
}

void Page::compositeToPixmap(QPixmap &target) const
{
    target = QPixmap(m_width, m_height);
    target.fill(Qt::transparent);

    QPainter painter(&target);
    painter.setRenderHint(QPainter::Antialiasing, false);

    // Composite layers from bottom to top
    for (const Layer &layer : m_layers) {
        if (layer.isVisible()) {
            layer.compositeTo(painter);
        }
    }
}

void Page::clear()
{
    m_layers.clear();
    addLayer("Layer 1");
}

void Page::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    for (Layer &layer : m_layers) {
        layer.resize(width, height);
    }
}

} // namespace Unimalen
