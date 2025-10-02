#include "UndoCommands.h"
#include "Layer.h"
#include <QPainter>

namespace Unimalen {

PaintCommand::PaintCommand(Layer *layer, const QPixmap &oldPixmap, const QPixmap &newPixmap,
                           const QString &text)
    : m_layer(layer)
    , m_oldPixmap(oldPixmap)
    , m_newPixmap(newPixmap)
{
    setText(text);
}

void PaintCommand::undo()
{
    if (m_layer) {
        m_layer->pixmap() = m_oldPixmap;
    }
}

void PaintCommand::redo()
{
    if (m_layer) {
        m_layer->pixmap() = m_newPixmap;
    }
}

} // namespace Unimalen
