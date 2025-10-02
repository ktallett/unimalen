#pragma once

#include <QPixmap>
#include <QString>
#include <QPainter>

class Layer
{
public:
    enum BlendMode {
        Normal,
        Multiply,
        Screen,
        Overlay
    };

    explicit Layer(const QString &name = "Layer", int width = 576, int height = 720);
    Layer(const Layer &other);
    Layer& operator=(const Layer &other);

    // Layer properties
    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

    qreal opacity() const { return m_opacity; }
    void setOpacity(qreal opacity) { m_opacity = qBound(0.0, opacity, 1.0); }

    BlendMode blendMode() const { return m_blendMode; }
    void setBlendMode(BlendMode mode) { m_blendMode = mode; }

    // Layer content
    QPixmap& pixmap() { return m_pixmap; }
    const QPixmap& pixmap() const { return m_pixmap; }

    // Layer operations
    void clear();
    void resize(int width, int height);
    Layer duplicate() const;

    // Composite this layer onto target with current settings
    void compositeTo(QPainter &painter) const;

private:
    QString m_name;
    QPixmap m_pixmap;
    bool m_visible;
    qreal m_opacity;
    BlendMode m_blendMode;
};