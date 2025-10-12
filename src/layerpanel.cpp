#include "layerpanel.h"
#include <QHBoxLayout>
#include <QPixmap>
#include <QPainter>

LayerPanel::LayerPanel(QWidget *parent)
    : QWidget(parent)
    , m_currentLayerIndex(0)
    , m_updatingUI(false)
{
    setMinimumWidth(240);  // Increased to account for scrollbar
    setMinimumHeight(300);

    createUI();
}

void LayerPanel::createUI()
{
    // Create main layout for the widget
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Create content widget
    QWidget *contentWidget = new QWidget();
    m_mainLayout = new QVBoxLayout(contentWidget);
    m_mainLayout->setContentsMargins(12, 12, 12, 12);
    m_mainLayout->setSpacing(12);

    // Layer list
    m_layerList = new QListWidget();
    m_layerList->setDragDropMode(QAbstractItemView::InternalMove);
    m_layerList->setDefaultDropAction(Qt::MoveAction);
    m_layerList->setMinimumHeight(150);  // Ensure list has some minimum height
    m_mainLayout->addWidget(m_layerList);

    connect(m_layerList, &QListWidget::currentItemChanged, this, &LayerPanel::onCurrentItemChanged);
    connect(m_layerList, &QListWidget::itemChanged, this, &LayerPanel::onItemChanged);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Create Add button with icon
    m_addButton = new QPushButton();
    m_addButton->setFixedSize(30, 30);
    m_addButton->setToolTip("Add Layer");
    m_addButton->setStyleSheet(
        "QPushButton { background-color: transparent; border: 2px solid transparent; border-radius: 4px; padding: 2px; }"
        "QPushButton:hover { border: 2px solid #4482B4; }"
        "QPushButton:pressed { background-color: rgba(68, 130, 180, 0.1); }"
    );
    QPixmap addIcon(24, 24);
    addIcon.fill(Qt::transparent);
    QPainter addPainter(&addIcon);
    addPainter.setPen(QPen(Qt::black, 2));
    addPainter.drawLine(12, 6, 12, 18);  // Vertical line
    addPainter.drawLine(6, 12, 18, 12);  // Horizontal line
    m_addButton->setIcon(QIcon(addIcon));
    m_addButton->setIconSize(QSize(24, 24));
    connect(m_addButton, &QPushButton::clicked, this, &LayerPanel::onAddLayerClicked);
    buttonLayout->addWidget(m_addButton);

    // Create Duplicate button with icon
    m_duplicateButton = new QPushButton();
    m_duplicateButton->setFixedSize(30, 30);
    m_duplicateButton->setToolTip("Duplicate Layer");
    m_duplicateButton->setStyleSheet(
        "QPushButton { background-color: transparent; border: 2px solid transparent; border-radius: 4px; padding: 2px; }"
        "QPushButton:hover { border: 2px solid #4482B4; }"
        "QPushButton:pressed { background-color: rgba(68, 130, 180, 0.1); }"
    );
    QPixmap dupIcon(24, 24);
    dupIcon.fill(Qt::transparent);
    QPainter dupPainter(&dupIcon);
    dupPainter.setPen(QPen(Qt::black, 2));
    dupPainter.setBrush(Qt::NoBrush);
    dupPainter.drawRect(4, 4, 12, 12);   // Back rectangle
    dupPainter.drawRect(8, 8, 12, 12);   // Front rectangle
    m_duplicateButton->setIcon(QIcon(dupIcon));
    m_duplicateButton->setIconSize(QSize(24, 24));
    connect(m_duplicateButton, &QPushButton::clicked, this, &LayerPanel::onDuplicateLayerClicked);
    buttonLayout->addWidget(m_duplicateButton);

    // Create Delete button with icon
    m_deleteButton = new QPushButton();
    m_deleteButton->setFixedSize(30, 30);
    m_deleteButton->setToolTip("Delete Layer");
    m_deleteButton->setStyleSheet(
        "QPushButton { background-color: transparent; border: 2px solid transparent; border-radius: 4px; padding: 2px; }"
        "QPushButton:hover { border: 2px solid #4482B4; }"
        "QPushButton:pressed { background-color: rgba(68, 130, 180, 0.1); }"
    );
    QPixmap delIcon(24, 24);
    delIcon.fill(Qt::transparent);
    QPainter delPainter(&delIcon);
    delPainter.setPen(QPen(Qt::black, 2));
    delPainter.drawLine(6, 6, 18, 18);   // Diagonal \
    delPainter.drawLine(18, 6, 6, 18);   // Diagonal /
    m_deleteButton->setIcon(QIcon(delIcon));
    m_deleteButton->setIconSize(QSize(24, 24));
    connect(m_deleteButton, &QPushButton::clicked, this, &LayerPanel::onDeleteLayerClicked);
    buttonLayout->addWidget(m_deleteButton);

    buttonLayout->addStretch();
    m_mainLayout->addLayout(buttonLayout);

    // Opacity controls
    m_opacityLabel = new QLabel("Opacity: 100%");
    m_mainLayout->addWidget(m_opacityLabel);

    m_opacitySlider = new QSlider(Qt::Horizontal);
    m_opacitySlider->setRange(0, 100);
    m_opacitySlider->setValue(100);
    connect(m_opacitySlider, &QSlider::valueChanged, this, &LayerPanel::onOpacityChanged);
    m_mainLayout->addWidget(m_opacitySlider);

    // Blend mode controls
    m_blendModeLabel = new QLabel("Blend Mode:");
    m_mainLayout->addWidget(m_blendModeLabel);

    m_blendModeCombo = new QComboBox();
    m_blendModeCombo->addItem("Normal", static_cast<int>(Layer::Normal));
    m_blendModeCombo->addItem("Multiply", static_cast<int>(Layer::Multiply));
    m_blendModeCombo->addItem("Screen", static_cast<int>(Layer::Screen));
    m_blendModeCombo->addItem("Overlay", static_cast<int>(Layer::Overlay));
    connect(m_blendModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LayerPanel::onBlendModeChanged);
    m_mainLayout->addWidget(m_blendModeCombo);

    // Wire up scroll area
    scrollArea->setWidget(contentWidget);
    outerLayout->addWidget(scrollArea);
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

