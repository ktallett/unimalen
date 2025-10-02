#include "layerpanel.h"
#include <QApplication>
#include <QScreen>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPixmap>
#include <QPainter>

LayerPanel::LayerPanel(QWidget *parent)
    : QWidget(parent)
    , m_currentLayerIndex(0)
    , m_updatingUI(false)
{
    setFixedSize(250, 400);
    setStyleSheet("QWidget { background-color: #f0f0f0; border: 1px solid #ccc; }");

    createUI();
}

void LayerPanel::createUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(5, 5, 5, 5);
    m_mainLayout->setSpacing(5);

    // Layer list
    m_layerList = new QListWidget(this);
    m_layerList->setDragDropMode(QAbstractItemView::InternalMove);
    m_layerList->setDefaultDropAction(Qt::MoveAction);
    m_mainLayout->addWidget(m_layerList);

    connect(m_layerList, &QListWidget::currentItemChanged, this, &LayerPanel::onCurrentItemChanged);
    connect(m_layerList, &QListWidget::itemChanged, this, &LayerPanel::onItemChanged);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    m_addButton = new QPushButton("+", this);
    m_addButton->setFixedSize(30, 30);
    m_addButton->setToolTip("Add Layer");
    connect(m_addButton, &QPushButton::clicked, this, &LayerPanel::onAddLayerClicked);
    buttonLayout->addWidget(m_addButton);

    m_duplicateButton = new QPushButton("⧉", this);
    m_duplicateButton->setFixedSize(30, 30);
    m_duplicateButton->setToolTip("Duplicate Layer");
    connect(m_duplicateButton, &QPushButton::clicked, this, &LayerPanel::onDuplicateLayerClicked);
    buttonLayout->addWidget(m_duplicateButton);

    m_deleteButton = new QPushButton("×", this);
    m_deleteButton->setFixedSize(30, 30);
    m_deleteButton->setToolTip("Delete Layer");
    connect(m_deleteButton, &QPushButton::clicked, this, &LayerPanel::onDeleteLayerClicked);
    buttonLayout->addWidget(m_deleteButton);

    buttonLayout->addStretch();
    m_mainLayout->addLayout(buttonLayout);

    // Opacity controls
    m_opacityLabel = new QLabel("Opacity: 100%", this);
    m_mainLayout->addWidget(m_opacityLabel);

    m_opacitySlider = new QSlider(Qt::Horizontal, this);
    m_opacitySlider->setRange(0, 100);
    m_opacitySlider->setValue(100);
    connect(m_opacitySlider, &QSlider::valueChanged, this, &LayerPanel::onOpacityChanged);
    m_mainLayout->addWidget(m_opacitySlider);

    // Blend mode controls
    m_blendModeLabel = new QLabel("Blend Mode:", this);
    m_mainLayout->addWidget(m_blendModeLabel);

    m_blendModeCombo = new QComboBox(this);
    m_blendModeCombo->addItem("Normal", static_cast<int>(Layer::Normal));
    m_blendModeCombo->addItem("Multiply", static_cast<int>(Layer::Multiply));
    m_blendModeCombo->addItem("Screen", static_cast<int>(Layer::Screen));
    m_blendModeCombo->addItem("Overlay", static_cast<int>(Layer::Overlay));
    connect(m_blendModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LayerPanel::onBlendModeChanged);
    m_mainLayout->addWidget(m_blendModeCombo);
}

void LayerPanel::showAsFloatingWindow()
{
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("Layers"));
    show();

    // Position relative to parent window if it exists
    if (parentWidget()) {
        QRect parentRect = parentWidget()->geometry();
        int spacing = 10;
        int x = parentRect.right() + spacing;
        int y = parentRect.top() + 200;
        move(x, y);
    } else {
        // Fallback positioning
        QScreen *screen = QApplication::primaryScreen();
        if (screen) {
            QRect screenGeometry = screen->availableGeometry();
            int x = screenGeometry.width() - width() - 60;
            int y = 200;
            move(x, y);
        } else {
            move(800, 200);
        }
    }
    raise();
    activateWindow();
}

void LayerPanel::setLayers(const QList<Layer> &layers)
{
    m_layers = layers;
    updateLayerList();
}

void LayerPanel::setCurrentLayerIndex(int index)
{
    if (index >= 0 && index < m_layers.size() && index != m_currentLayerIndex) {
        m_currentLayerIndex = index;
        m_layerList->setCurrentRow(m_layers.size() - 1 - index); // Invert for display
        updateOpacitySlider();
        updateBlendModeCombo();
    }
}

int LayerPanel::currentLayerIndex() const
{
    return m_currentLayerIndex;
}

void LayerPanel::updateLayerList()
{
    m_updatingUI = true;

    m_layerList->clear();

    // Add layers in reverse order (top layer first in UI)
    for (int i = m_layers.size() - 1; i >= 0; --i) {
        LayerItem *item = createLayerItem(m_layers[i], i);
        m_layerList->addItem(item);
    }

    if (m_currentLayerIndex < m_layers.size()) {
        m_layerList->setCurrentRow(m_layers.size() - 1 - m_currentLayerIndex);
    }

    m_updatingUI = false;
}

void LayerPanel::updateLayerProperties(int index, const Layer &layer)
{
    if (index >= 0 && index < m_layers.size()) {
        m_layers[index] = layer;

        // Update the corresponding list item
        int listIndex = m_layers.size() - 1 - index;
        if (listIndex < m_layerList->count()) {
            LayerItem *item = static_cast<LayerItem*>(m_layerList->item(listIndex));
            if (item) {
                item->updateFromLayer(layer);
            }
        }

        // Update controls if this is the current layer
        if (index == m_currentLayerIndex) {
            updateOpacitySlider();
            updateBlendModeCombo();
        }
    }
}

LayerItem* LayerPanel::createLayerItem(const Layer &layer, int index)
{
    LayerItem *item = new LayerItem(layer, index);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    return item;
}

void LayerPanel::onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)

    if (!current || m_updatingUI) return;

    LayerItem *layerItem = static_cast<LayerItem*>(current);
    int newIndex = layerItem->layerIndex();

    if (newIndex != m_currentLayerIndex) {
        m_currentLayerIndex = newIndex;
        updateOpacitySlider();
        updateBlendModeCombo();
        emit layerSelected(newIndex);
    }
}

void LayerPanel::onAddLayerClicked()
{
    emit layerAdded();
}

void LayerPanel::onDeleteLayerClicked()
{
    if (m_layers.size() > 1 && m_currentLayerIndex >= 0) {
        emit layerDeleted(m_currentLayerIndex);
    }
}

void LayerPanel::onDuplicateLayerClicked()
{
    if (m_currentLayerIndex >= 0) {
        emit layerDuplicated(m_currentLayerIndex);
    }
}

void LayerPanel::onOpacityChanged(int value)
{
    if (!m_updatingUI && m_currentLayerIndex >= 0) {
        qreal opacity = value / 100.0;
        m_opacityLabel->setText(QString("Opacity: %1%").arg(value));
        emit layerOpacityChanged(m_currentLayerIndex, opacity);
    }
}

void LayerPanel::onBlendModeChanged(int index)
{
    if (!m_updatingUI && m_currentLayerIndex >= 0) {
        Layer::BlendMode mode = static_cast<Layer::BlendMode>(m_blendModeCombo->itemData(index).toInt());
        emit layerBlendModeChanged(m_currentLayerIndex, mode);
    }
}

void LayerPanel::onItemChanged(QListWidgetItem *item)
{
    if (!m_updatingUI) {
        LayerItem *layerItem = static_cast<LayerItem*>(item);
        emit layerRenamed(layerItem->layerIndex(), item->text());
    }
}

void LayerPanel::updateOpacitySlider()
{
    if (m_currentLayerIndex >= 0 && m_currentLayerIndex < m_layers.size()) {
        m_updatingUI = true;
        int opacity = qRound(m_layers[m_currentLayerIndex].opacity() * 100);
        m_opacitySlider->setValue(opacity);
        m_opacityLabel->setText(QString("Opacity: %1%").arg(opacity));
        m_updatingUI = false;
    }
}

void LayerPanel::updateBlendModeCombo()
{
    if (m_currentLayerIndex >= 0 && m_currentLayerIndex < m_layers.size()) {
        m_updatingUI = true;
        Layer::BlendMode mode = m_layers[m_currentLayerIndex].blendMode();
        int index = m_blendModeCombo->findData(static_cast<int>(mode));
        m_blendModeCombo->setCurrentIndex(index);
        m_updatingUI = false;
    }
}

// LayerItem implementation
LayerItem::LayerItem(const Layer &layer, int index)
    : QListWidgetItem()
    , m_layerIndex(index)
{
    updateFromLayer(layer);
}

void LayerItem::updateFromLayer(const Layer &layer)
{
    setText(layer.name());

    // Create a thumbnail icon
    QPixmap thumbnail = layer.pixmap().scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setIcon(QIcon(thumbnail));

    // Set visual state based on visibility
    if (!layer.isVisible()) {
        QFont font = data(Qt::FontRole).value<QFont>();
        font.setItalic(true);
        setData(Qt::FontRole, font);
        setForeground(QColor(128, 128, 128));
    } else {
        QFont font = data(Qt::FontRole).value<QFont>();
        font.setItalic(false);
        setData(Qt::FontRole, font);
        setForeground(QColor(0, 0, 0));
    }
}

void LayerPanel::closeEvent(QCloseEvent *event)
{
    emit layerPanelClosed();
    QWidget::closeEvent(event);
}