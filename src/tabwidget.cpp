#include "tabwidget.h"
#include "canvas.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFileInfo>

TabWidget::TabWidget(QWidget *parent)
    : QWidget(parent)
    , m_tabWidget(new QTabWidget(this))
    , m_layout(new QVBoxLayout(this))
    , m_untitledCounter(1)
{
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_tabWidget);

    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setMovable(true);

    connect(m_tabWidget, &QTabWidget::currentChanged, this, &TabWidget::onCurrentChanged);
    connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, &TabWidget::onTabCloseRequested);

    newTab();
}

TabWidget::~TabWidget()
{
}

Canvas* TabWidget::currentCanvas() const
{
    QScrollArea *scrollArea = qobject_cast<QScrollArea*>(m_tabWidget->currentWidget());
    if (scrollArea) {
        return qobject_cast<Canvas*>(scrollArea->widget());
    }
    return nullptr;
}

Canvas* TabWidget::canvasAt(int index) const
{
    if (index < 0 || index >= m_tabWidget->count()) {
        return nullptr;
    }

    QScrollArea *scrollArea = qobject_cast<QScrollArea*>(m_tabWidget->widget(index));
    if (scrollArea) {
        return qobject_cast<Canvas*>(scrollArea->widget());
    }
    return nullptr;
}

int TabWidget::currentIndex() const
{
    return m_tabWidget->currentIndex();
}

int TabWidget::count() const
{
    return m_tabWidget->count();
}

void TabWidget::newTab()
{
    if (m_tabWidget->count() >= MAX_TABS) {
        return;
    }

    Canvas *canvas = new Canvas(this);
    QScrollArea *scrollArea = new QScrollArea(this);

    scrollArea->setWidget(canvas);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setStyleSheet("QScrollArea { background-color: #e0e0e0; }");

    setupCanvas(canvas);

    QString tabName = generateTabName();
    int index = m_tabWidget->addTab(scrollArea, tabName);
    m_scrollAreas.append(scrollArea);

    m_tabWidget->setCurrentIndex(index);

    emit documentChanged();
}

void TabWidget::closeTab(int index)
{
    if (index < 0 || index >= m_tabWidget->count() || m_tabWidget->count() <= 1) {
        return;
    }

    QScrollArea *scrollArea = qobject_cast<QScrollArea*>(m_tabWidget->widget(index));
    if (scrollArea) {
        Canvas *canvas = qobject_cast<Canvas*>(scrollArea->widget());
        if (canvas) {
            delete canvas;
        }
        m_scrollAreas.removeAll(scrollArea);
        delete scrollArea;
    }

    m_tabWidget->removeTab(index);

    emit documentChanged();
}

void TabWidget::setCurrentTab(int index)
{
    if (index >= 0 && index < m_tabWidget->count()) {
        m_tabWidget->setCurrentIndex(index);
    }
}

bool TabWidget::loadDocument(const QString &fileName)
{
    if (m_tabWidget->count() >= MAX_TABS) {
        return false;
    }

    Canvas *canvas = new Canvas(this);
    if (!canvas->loadCanvas(fileName)) {
        delete canvas;
        return false;
    }

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(canvas);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setStyleSheet("QScrollArea { background-color: #e0e0e0; }");

    setupCanvas(canvas);

    QString tabName = QFileInfo(fileName).baseName();
    int index = m_tabWidget->addTab(scrollArea, tabName);
    m_scrollAreas.append(scrollArea);

    m_tabWidget->setCurrentIndex(index);

    emit documentChanged();
    return true;
}

bool TabWidget::saveDocument(const QString &fileName)
{
    return saveCurrentDocument(fileName);
}

bool TabWidget::saveCurrentDocument(const QString &fileName)
{
    Canvas *canvas = currentCanvas();
    if (!canvas) {
        return false;
    }

    bool success = canvas->saveCanvas(fileName);
    if (success) {
        QString tabName = QFileInfo(fileName).baseName();
        m_tabWidget->setTabText(currentIndex(), tabName);
    }

    return success;
}

void TabWidget::setTabName(int index, const QString &name)
{
    if (index >= 0 && index < m_tabWidget->count()) {
        m_tabWidget->setTabText(index, name);
    }
}

QString TabWidget::tabName(int index) const
{
    if (index >= 0 && index < m_tabWidget->count()) {
        return m_tabWidget->tabText(index);
    }
    return QString();
}

void TabWidget::onCurrentChanged(int index)
{
    emit currentChanged(index);
}

void TabWidget::onTabCloseRequested(int index)
{
    closeTab(index);
    emit tabCloseRequested(index);
}

void TabWidget::setupCanvas(Canvas *canvas)
{
    // This method can be used to set up common canvas properties
    // Currently, the canvas is initialized with defaults in its constructor
}

QString TabWidget::generateTabName() const
{
    return QString("Untitled %1").arg(m_untitledCounter++);
}