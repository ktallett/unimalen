#include "Layer.h"
#include <QPainter>

namespace Unimalen {

Layer::Layer(const QString &name, int width, int height)
    : m_name(name)
    , m_pixmap(width, height)
    , m_visible(true)
    , m_opacity(1.0)
    , m_blendMode(Normal)
{
    m_pixmap.fill(Qt::transparent);
}

Layer::Layer(const Layer &other)
    : m_name(other.m_name)
    , m_pixmap(other.m_pixmap)
    , m_visible(other.m_visible)
    , m_opacity(other.m_opacity)
    , m_blendMode(other.m_blendMode)
{
}

Layer& Layer::operator=(const Layer &other)
{
    if (this != &other) {
        m_name = other.m_name;
        m_pixmap = other.m_pixmap;
        m_visible = other.m_visible;
        m_opacity = other.m_opacity;
        m_blendMode = other.m_blendMode;
    }
    return *this;
}

void Layer::clear()
{
    m_pixmap.fill(Qt::transparent);
}

void Layer::resize(int width, int height)
{
    if (m_pixmap.size() != QSize(width, height)) {
        QPixmap newPixmap(width, height);
        newPixmap.fill(Qt::transparent);

        QPainter painter(&newPixmap);
        painter.drawPixmap(0, 0, m_pixmap);

        m_pixmap = newPixmap;
    }
}

Layer Layer::duplicate() const
{
    Layer copy(*this);
    copy.setName(m_name + " copy");
    return copy;
}

void Layer::compositeTo(QPainter &painter) const
{
    if (!m_visible || m_opacity <= 0.0) {
        return;
    }

    qreal oldOpacity = painter.opacity();
    painter.setOpacity(oldOpacity * m_opacity);

    // Set composition mode based on blend mode
    QPainter::CompositionMode compositionMode;
    switch (m_blendMode) {
        case Normal:
            compositionMode = QPainter::CompositionMode_SourceOver;
            break;
        case Multiply:
            compositionMode = QPainter::CompositionMode_Multiply;
            break;
        case Screen:
            compositionMode = QPainter::CompositionMode_Screen;
            break;
        case Overlay:
            compositionMode = QPainter::CompositionMode_Overlay;
            break;
        default:
            compositionMode = QPainter::CompositionMode_SourceOver;
            break;
    }

    QPainter::CompositionMode oldMode = painter.compositionMode();
    painter.setCompositionMode(compositionMode);

    painter.drawPixmap(0, 0, m_pixmap);

    painter.setCompositionMode(oldMode);
    painter.setOpacity(oldOpacity);
}

} // namespace Unimalen