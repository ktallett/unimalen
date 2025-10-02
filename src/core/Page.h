#pragma once

#include "Layer.h"
#include <QPixmap>
#include <QList>
#include <QString>

namespace Unimalen {

constexpr int MAX_LAYERS_PER_PAGE = 3;

class Page
{
public:
    explicit Page(int width, int height);
    ~Page() = default;

    // Layer management
    QList<Layer>& layers() { return m_layers; }
    const QList<Layer>& layers() const { return m_layers; }

    int currentLayerIndex() const { return m_currentLayerIndex; }
    void setCurrentLayerIndex(int index);

    Layer& currentLayer();
    const Layer& currentLayer() const;

    void addLayer(const QString &name = QString());
    void deleteLayer(int index);
    void duplicateLayer(int index);
    void moveLayer(int fromIndex, int toIndex);
    void setLayerVisibility(int index, bool visible);
    void setLayerOpacity(int index, qreal opacity);
    void setLayerBlendMode(int index, Layer::BlendMode mode);
    void setLayerName(int index, const QString &name);

    // Compositing
    QPixmap composite() const;
    void compositeToPixmap(QPixmap &target) const;

    // Page state
    void clear();
    void resize(int width, int height);

    int width() const { return m_width; }
    int height() const { return m_height; }

private:
    int m_width;
    int m_height;
    QList<Layer> m_layers;
    int m_currentLayerIndex;
};

} // namespace Unimalen
