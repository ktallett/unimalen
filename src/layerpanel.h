#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QListWidgetItem>
#include <QCloseEvent>
#include "core/Layer.h"

using Unimalen::Layer;

class LayerItem;

class LayerPanel : public QWidget
{
    Q_OBJECT

public:
    explicit LayerPanel(QWidget *parent = nullptr);
    void showAsFloatingWindow();

    // Layer management
    void setLayers(const QList<Layer> &layers);
    void setCurrentLayerIndex(int index);
    int currentLayerIndex() const;

    // Update UI from external changes
    void updateLayerList();
    void updateLayerProperties(int index, const Layer &layer);

signals:
    void layerSelected(int index);
    void layerAdded();
    void layerDeleted(int index);
    void layerDuplicated(int index);
    void layerMoved(int fromIndex, int toIndex);
    void layerVisibilityChanged(int index, bool visible);
    void layerOpacityChanged(int index, qreal opacity);
    void layerBlendModeChanged(int index, Layer::BlendMode mode);
    void layerRenamed(int index, const QString &name);
    void layerPanelClosed();

private slots:
    void onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void onAddLayerClicked();
    void onDeleteLayerClicked();
    void onDuplicateLayerClicked();
    void onOpacityChanged(int value);
    void onBlendModeChanged(int index);
    void onItemChanged(QListWidgetItem *item);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void createUI();
    void updateOpacitySlider();
    void updateBlendModeCombo();
    LayerItem* createLayerItem(const Layer &layer, int index);

    QVBoxLayout *m_mainLayout;
    QListWidget *m_layerList;
    QPushButton *m_addButton;
    QPushButton *m_deleteButton;
    QPushButton *m_duplicateButton;
    QLabel *m_opacityLabel;
    QSlider *m_opacitySlider;
    QLabel *m_blendModeLabel;
    QComboBox *m_blendModeCombo;

    QList<Layer> m_layers;
    int m_currentLayerIndex;
    bool m_updatingUI;
};

// Custom list widget item for layers
class LayerItem : public QListWidgetItem
{
public:
    LayerItem(const Layer &layer, int index);
    void updateFromLayer(const Layer &layer);
    void setLayerIndex(int index) { m_layerIndex = index; }
    int layerIndex() const { return m_layerIndex; }

private:
    int m_layerIndex;
};