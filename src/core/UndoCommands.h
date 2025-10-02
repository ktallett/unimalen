#pragma once

#include <QUndoCommand>
#include <QPixmap>

namespace Unimalen {

class Layer;

// Command for painting operations on a layer
class PaintCommand : public QUndoCommand
{
public:
    PaintCommand(Layer *layer, const QPixmap &oldPixmap, const QPixmap &newPixmap,
                 const QString &text = "Paint");

    void undo() override;
    void redo() override;

private:
    Layer *m_layer;
    QPixmap m_oldPixmap;
    QPixmap m_newPixmap;
};

} // namespace Unimalen
