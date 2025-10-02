#include "mainwindow.h"
#include "canvas.h"
#include "tabwidget.h"
#include "toolbar.h"
#include "patternbar.h"
#include "thicknessbar.h"
#include "layerpanel.h"
#include "colorbar.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QActionGroup>
#include <QStandardPaths>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_currentFile("")
{
    m_tabWidget = new TabWidget(this);
    m_toolBar = new ToolBar(this);
    m_patternBar = new PatternBar(this);
    m_thicknessBar = new ThicknessBar(this);
    m_layerPanel = new LayerPanel(this);
    m_colorBar = new ColorBar(this, ColorBar::CopicPastels);

    setCentralWidget(m_tabWidget);

    createActions();
    createMenus();

    // Create page indicator in status bar
    m_pageIndicator = new QLabel(this);
    m_pageIndicator->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_pageIndicator->setMinimumWidth(100);
    statusBar()->addPermanentWidget(m_pageIndicator);
    updatePageIndicator();

    // Show toolbars as separate floating windows after main window is shown
    QTimer::singleShot(200, this, [this]() {
        positionToolbarsAroundCanvas();
    });

    connect(m_toolBar, &ToolBar::pencilToolSelected, this, &MainWindow::onPencilSelected);
    connect(m_toolBar, &ToolBar::textToolSelected, this, &MainWindow::onTextSelected);
    connect(m_toolBar, &ToolBar::sprayToolSelected, this, &MainWindow::onSpraySelected);
    connect(m_toolBar, &ToolBar::brushToolSelected, this, &MainWindow::onBrushSelected);
    connect(m_toolBar, &ToolBar::eraserToolSelected, this, &MainWindow::onEraserSelected);
    connect(m_toolBar, &ToolBar::lineToolSelected, this, &MainWindow::onLineSelected);
    connect(m_toolBar, &ToolBar::bezierToolSelected, this, &MainWindow::onBezierSelected);
    connect(m_toolBar, &ToolBar::scissorsToolSelected, this, &MainWindow::onScissorsSelected);
    connect(m_toolBar, &ToolBar::fillToolSelected, this, &MainWindow::onFillSelected);
    connect(m_toolBar, &ToolBar::lassoToolSelected, this, &MainWindow::onLassoSelected);
    connect(m_toolBar, &ToolBar::squareToolSelected, this, &MainWindow::onSquareSelected);
    connect(m_toolBar, &ToolBar::filledSquareToolSelected, this, &MainWindow::onFilledSquareSelected);
    connect(m_toolBar, &ToolBar::roundedSquareToolSelected, this, &MainWindow::onRoundedSquareSelected);
    connect(m_toolBar, &ToolBar::filledRoundedSquareToolSelected, this, &MainWindow::onFilledRoundedSquareSelected);
    connect(m_toolBar, &ToolBar::ovalToolSelected, this, &MainWindow::onOvalSelected);
    connect(m_toolBar, &ToolBar::filledOvalToolSelected, this, &MainWindow::onFilledOvalSelected);

    connect(m_patternBar, QOverload<PatternBar::PatternType>::of(&PatternBar::patternSelected),
            this, [this](PatternBar::PatternType pattern) {
                Canvas *canvas = getCurrentCanvas();
                if (canvas) canvas->setCurrentPattern(pattern);
            });

    connect(m_thicknessBar, QOverload<ThicknessBar::ThicknessType>::of(&ThicknessBar::thicknessSelected),
            this, [this](ThicknessBar::ThicknessType thickness) { onThicknessSelected(static_cast<int>(thickness)); });

    connect(m_colorBar, &ColorBar::colorSelected,
            this, [this](const QColor &color) {
                Canvas *canvas = getCurrentCanvas();
                if (canvas) canvas->setCurrentColor(color);
            });

    // Connect toolbar close events to uncheck menu items
    connect(m_toolBar, &ToolBar::toolBarClosed, this, [this]() {
        m_showToolBarAction->setChecked(false);
    });
    connect(m_patternBar, &PatternBar::patternBarClosed, this, [this]() {
        m_showPatternBarAction->setChecked(false);
    });
    connect(m_thicknessBar, &ThicknessBar::thicknessBarClosed, this, [this]() {
        m_showThicknessBarAction->setChecked(false);
    });
    connect(m_layerPanel, &LayerPanel::layerPanelClosed, this, [this]() {
        m_showLayerPanelAction->setChecked(false);
    });

    connect(m_tabWidget, &TabWidget::currentChanged, this, &MainWindow::onTabChanged);

    // Connect signals for the initial canvas
    Canvas *initialCanvas = getCurrentCanvas();
    if (initialCanvas) {
        connectCanvasSignals(initialCanvas);
    }

    setWindowTitle(tr("unimalen - Untitled"));
    resize(800, 600);
}

void MainWindow::createActions()
{
    m_newAction = new QAction(tr("&New"), this);
    m_newAction->setShortcut(QKeySequence::New);
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newFile);

    m_openAction = new QAction(tr("&Open..."), this);
    m_openAction->setShortcut(QKeySequence::Open);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);

    m_saveAction = new QAction(tr("&Save"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    m_saveAsAction = new QAction(tr("Save &As..."), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);

    m_insertImageAction = new QAction(tr("&Insert Image..."), this);
    m_insertImageAction->setShortcut(QKeySequence("Ctrl+I"));
    connect(m_insertImageAction, &QAction::triggered, this, &MainWindow::insertImage);

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);

    m_newTabAction = new QAction(tr("New &Tab"), this);
    m_newTabAction->setShortcut(QKeySequence("Ctrl+T"));
    connect(m_newTabAction, &QAction::triggered, this, &MainWindow::newTab);

    m_closeTabAction = new QAction(tr("&Close Tab"), this);
    m_closeTabAction->setShortcut(QKeySequence("Ctrl+W"));
    connect(m_closeTabAction, &QAction::triggered, this, &MainWindow::closeTab);

    // Edit menu actions
    m_undoAction = new QAction(tr("&Undo"), this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    connect(m_undoAction, &QAction::triggered, this, [this]() {
        Canvas *canvas = getCurrentCanvas();
        if (canvas) canvas->undo();
    });

    m_redoAction = new QAction(tr("&Redo"), this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    connect(m_redoAction, &QAction::triggered, this, [this]() {
        Canvas *canvas = getCurrentCanvas();
        if (canvas) canvas->redo();
    });

    m_cutAction = new QAction(tr("Cu&t"), this);
    m_cutAction->setShortcut(QKeySequence::Cut);
    connect(m_cutAction, &QAction::triggered, this, &MainWindow::onCut);

    m_copyAction = new QAction(tr("&Copy"), this);
    m_copyAction->setShortcut(QKeySequence::Copy);
    connect(m_copyAction, &QAction::triggered, this, &MainWindow::onCopy);

    m_pasteAction = new QAction(tr("&Paste"), this);
    m_pasteAction->setShortcut(QKeySequence::Paste);
    connect(m_pasteAction, &QAction::triggered, this, &MainWindow::onPaste);

    m_scale1xAction = new QAction("100%", this);
    m_scale1xAction->setCheckable(true);
    m_scale1xAction->setChecked(true);
    connect(m_scale1xAction, &QAction::triggered, this, &MainWindow::setScale1x);

    m_scale2xAction = new QAction("200%", this);
    m_scale2xAction->setCheckable(true);
    connect(m_scale2xAction, &QAction::triggered, this, &MainWindow::setScale2x);

    m_scale4xAction = new QAction("400%", this);
    m_scale4xAction->setCheckable(true);
    connect(m_scale4xAction, &QAction::triggered, this, &MainWindow::setScale4x);

    m_pixelZoomAction = new QAction(tr("Pixel Zoom"), this);
    m_pixelZoomAction->setCheckable(true);
    connect(m_pixelZoomAction, &QAction::toggled, this, &MainWindow::togglePixelZoom);

    m_showCoordinatesAction = new QAction(tr("Show Coordinates"), this);
    m_showCoordinatesAction->setCheckable(true);
    connect(m_showCoordinatesAction, &QAction::toggled, this, &MainWindow::toggleCoordinates);

    m_scaleGroup = new QActionGroup(this);
    m_scaleGroup->addAction(m_scale1xAction);
    m_scaleGroup->addAction(m_scale2xAction);
    m_scaleGroup->addAction(m_scale4xAction);

    // Toolbar visibility actions
    m_showToolBarAction = new QAction(tr("Show &Tools"), this);
    m_showToolBarAction->setCheckable(true);
    m_showToolBarAction->setChecked(true); // Default visible
    connect(m_showToolBarAction, &QAction::toggled, this, &MainWindow::toggleToolBar);

    m_showPatternBarAction = new QAction(tr("Show &Patterns"), this);
    m_showPatternBarAction->setCheckable(true);
    m_showPatternBarAction->setChecked(true); // Default visible
    connect(m_showPatternBarAction, &QAction::toggled, this, &MainWindow::togglePatternBar);

    m_showThicknessBarAction = new QAction(tr("Show &Line Thickness"), this);
    m_showThicknessBarAction->setCheckable(true);
    m_showThicknessBarAction->setChecked(true); // Default visible
    connect(m_showThicknessBarAction, &QAction::toggled, this, &MainWindow::toggleThicknessBar);

    m_showLayerPanelAction = new QAction(tr("Show &Layers"), this);
    m_showLayerPanelAction->setCheckable(true);
    m_showLayerPanelAction->setChecked(true); // Default visible
    connect(m_showLayerPanelAction, &QAction::toggled, this, &MainWindow::toggleLayerPanel);

    m_showColorBarAction = new QAction(tr("Show &Colors"), this);
    m_showColorBarAction->setCheckable(true);
    m_showColorBarAction->setChecked(true); // Default visible
    connect(m_showColorBarAction, &QAction::toggled, this, &MainWindow::toggleColorBar);

    // Font selection actions
    m_courierFontAction = new QAction("Courier New", this);
    m_courierFontAction->setCheckable(true);
    connect(m_courierFontAction, &QAction::triggered, this, [this]() { onFontChanged("Courier New"); });

    m_openSansFontAction = new QAction("Open Sans", this);
    m_openSansFontAction->setCheckable(true);
    m_openSansFontAction->setChecked(true); // Default
    connect(m_openSansFontAction, &QAction::triggered, this, [this]() { onFontChanged("Open Sans"); });

    m_serifFontAction = new QAction("Liberation Serif", this);
    m_serifFontAction->setCheckable(true);
    connect(m_serifFontAction, &QAction::triggered, this, [this]() { onFontChanged("Liberation Serif"); });

    m_notoSansFontAction = new QAction("Noto Sans (International)", this);
    m_notoSansFontAction->setCheckable(true);
    connect(m_notoSansFontAction, &QAction::triggered, this, [this]() { onFontChanged("Noto Sans"); });

    m_notoSansCJKFontAction = new QAction("Noto Sans CJK (Chinese/Japanese/Korean)", this);
    m_notoSansCJKFontAction->setCheckable(true);
    connect(m_notoSansCJKFontAction, &QAction::triggered, this, [this]() { onFontChanged("Noto Sans CJK"); });

    m_dejavuSansFontAction = new QAction("DejaVu Sans (Cyrillic)", this);
    m_dejavuSansFontAction->setCheckable(true);
    connect(m_dejavuSansFontAction, &QAction::triggered, this, [this]() { onFontChanged("DejaVu Sans"); });

    m_fontGroup = new QActionGroup(this);
    m_fontGroup->addAction(m_courierFontAction);
    m_fontGroup->addAction(m_openSansFontAction);
    m_fontGroup->addAction(m_serifFontAction);
    m_fontGroup->addAction(m_notoSansFontAction);
    m_fontGroup->addAction(m_notoSansCJKFontAction);
    m_fontGroup->addAction(m_dejavuSansFontAction);

    // Font size actions
    m_fontSize8Action = new QAction("8", this);
    m_fontSize8Action->setCheckable(true);
    connect(m_fontSize8Action, &QAction::triggered, this, [this]() { onFontSizeChanged(8); });

    m_fontSize12Action = new QAction("12", this);
    m_fontSize12Action->setCheckable(true);
    m_fontSize12Action->setChecked(true); // Default
    connect(m_fontSize12Action, &QAction::triggered, this, [this]() { onFontSizeChanged(12); });

    m_fontSize18Action = new QAction("18", this);
    m_fontSize18Action->setCheckable(true);
    connect(m_fontSize18Action, &QAction::triggered, this, [this]() { onFontSizeChanged(18); });

    m_fontSize24Action = new QAction("24", this);
    m_fontSize24Action->setCheckable(true);
    connect(m_fontSize24Action, &QAction::triggered, this, [this]() { onFontSizeChanged(24); });

    m_fontSize32Action = new QAction("32", this);
    m_fontSize32Action->setCheckable(true);
    connect(m_fontSize32Action, &QAction::triggered, this, [this]() { onFontSizeChanged(32); });

    m_fontSize64Action = new QAction("64", this);
    m_fontSize64Action->setCheckable(true);
    connect(m_fontSize64Action, &QAction::triggered, this, [this]() { onFontSizeChanged(64); });

    m_fontSizeGroup = new QActionGroup(this);
    m_fontSizeGroup->addAction(m_fontSize8Action);
    m_fontSizeGroup->addAction(m_fontSize12Action);
    m_fontSizeGroup->addAction(m_fontSize18Action);
    m_fontSizeGroup->addAction(m_fontSize24Action);
    m_fontSizeGroup->addAction(m_fontSize32Action);
    m_fontSizeGroup->addAction(m_fontSize64Action);

    // Layer actions
    m_addLayerAction = new QAction(tr("&Add Layer"), this);
    m_addLayerAction->setShortcut(QKeySequence("Ctrl+Shift+N"));
    connect(m_addLayerAction, &QAction::triggered, this, &MainWindow::onLayerAdded);

    m_deleteLayerAction = new QAction(tr("&Delete Layer"), this);
    m_deleteLayerAction->setShortcut(QKeySequence("Del"));
    connect(m_deleteLayerAction, &QAction::triggered, this, [this]() {
        Canvas *canvas = getCurrentCanvas();
        if (canvas && canvas->layers().size() > 1) {
            onLayerDeleted(canvas->currentLayerIndex());
        }
    });

    m_duplicateLayerAction = new QAction(tr("D&uplicate Layer"), this);
    m_duplicateLayerAction->setShortcut(QKeySequence("Ctrl+J"));
    connect(m_duplicateLayerAction, &QAction::triggered, this, [this]() {
        Canvas *canvas = getCurrentCanvas();
        if (canvas) {
            onLayerDuplicated(canvas->currentLayerIndex());
        }
    });

    // Page actions
    m_addPageAction = new QAction(tr("Add &Page"), this);
    m_addPageAction->setShortcut(QKeySequence("Ctrl+Shift+P"));
    connect(m_addPageAction, &QAction::triggered, this, &MainWindow::onPageAdded);

    m_deletePageAction = new QAction(tr("Delete P&age"), this);
    connect(m_deletePageAction, &QAction::triggered, this, &MainWindow::onPageDeleted);

    m_duplicatePageAction = new QAction(tr("Du&plicate Page"), this);
    connect(m_duplicatePageAction, &QAction::triggered, this, &MainWindow::onPageDuplicated);

    m_nextPageAction = new QAction(tr("&Next Page"), this);
    m_nextPageAction->setShortcut(QKeySequence("Ctrl+Right"));
    connect(m_nextPageAction, &QAction::triggered, this, &MainWindow::nextPage);

    m_previousPageAction = new QAction(tr("&Previous Page"), this);
    m_previousPageAction->setShortcut(QKeySequence("Ctrl+Left"));
    connect(m_previousPageAction, &QAction::triggered, this, &MainWindow::previousPage);

    // Page size actions
    m_pageSizeGroup = new QActionGroup(this);
    using Unimalen::PageSize;
    using Unimalen::PAGE_SIZES;

    for (const auto& info : PAGE_SIZES) {
        QAction *action = new QAction(info.name, this);
        action->setCheckable(true);
        action->setActionGroup(m_pageSizeGroup);
        connect(action, &QAction::triggered, this, [this, info]() {
            setPageSize(info.size);
        });
        m_pageSizeActions.append(action);

        // Default to Custom size
        if (info.size == PageSize::Custom) {
            action->setChecked(true);
        }
    }

    // Create tab shortcuts for numbers 1-9
    for (int i = 1; i <= 9; ++i) {
        QAction *tabAction = new QAction(this);
        tabAction->setShortcut(QKeySequence(QString("Ctrl+%1").arg(i)));
        connect(tabAction, &QAction::triggered, this, [this, i]() { switchToTab(i); });
        addAction(tabAction);
        m_tabShortcuts.append(tabAction);
    }
}

void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_newAction);
    fileMenu->addAction(m_newTabAction);
    fileMenu->addSeparator();

    // Add page size submenu
    QMenu *pageSizeMenu = fileMenu->addMenu(tr("Page &Size"));
    for (QAction *action : m_pageSizeActions) {
        pageSizeMenu->addAction(action);
    }

    fileMenu->addSeparator();
    fileMenu->addAction(m_openAction);
    fileMenu->addAction(m_insertImageAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_closeTabAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exitAction);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(m_undoAction);
    editMenu->addAction(m_redoAction);
    editMenu->addSeparator();
    editMenu->addAction(m_cutAction);
    editMenu->addAction(m_copyAction);
    editMenu->addAction(m_pasteAction);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(m_scale1xAction);
    viewMenu->addAction(m_scale2xAction);
    viewMenu->addAction(m_scale4xAction);
    viewMenu->addSeparator();
    viewMenu->addAction(m_showToolBarAction);
    viewMenu->addAction(m_showPatternBarAction);
    viewMenu->addAction(m_showThicknessBarAction);
    viewMenu->addAction(m_showLayerPanelAction);
    viewMenu->addAction(m_showColorBarAction);

    QMenu *goodiesMenu = menuBar()->addMenu(tr("&Goodies"));
    goodiesMenu->addAction(m_pixelZoomAction);
    goodiesMenu->addAction(m_showCoordinatesAction);

    QMenu *layerMenu = menuBar()->addMenu(tr("&Layer"));
    layerMenu->addAction(m_addLayerAction);
    layerMenu->addAction(m_duplicateLayerAction);
    layerMenu->addAction(m_deleteLayerAction);

    QMenu *pageMenu = menuBar()->addMenu(tr("&Page"));
    pageMenu->addAction(m_addPageAction);
    pageMenu->addAction(m_duplicatePageAction);
    pageMenu->addAction(m_deletePageAction);
    pageMenu->addSeparator();
    pageMenu->addAction(m_previousPageAction);
    pageMenu->addAction(m_nextPageAction);

    QMenu *fontMenu = menuBar()->addMenu(tr("F&ont"));
    fontMenu->addAction(m_courierFontAction);
    fontMenu->addAction(m_openSansFontAction);
    fontMenu->addAction(m_serifFontAction);
    fontMenu->addSeparator();
    fontMenu->addAction(m_notoSansFontAction);
    fontMenu->addAction(m_notoSansCJKFontAction);
    fontMenu->addAction(m_dejavuSansFontAction);

    QMenu *fontSizeMenu = menuBar()->addMenu(tr("Font &Size"));
    fontSizeMenu->addAction(m_fontSize8Action);
    fontSizeMenu->addAction(m_fontSize12Action);
    fontSizeMenu->addAction(m_fontSize18Action);
    fontSizeMenu->addAction(m_fontSize24Action);
    fontSizeMenu->addAction(m_fontSize32Action);
    fontSizeMenu->addAction(m_fontSize64Action);
}

void MainWindow::newFile()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->newCanvas();
        setCurrentFile("");
    }
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open Image", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "OpenRaster Files (*.ora);;PNG Files (*.png);;All Files (*)");

    if (!fileName.isEmpty()) {
        if (m_tabWidget->loadDocument(fileName)) {
            setCurrentFile(fileName);
        } else {
            QMessageBox::warning(this, "unimalen", "Cannot open file " + fileName);
        }
    }
}

void MainWindow::saveFile()
{
    if (m_currentFile.isEmpty()) {
        saveAsFile();
    } else {
        if (!m_tabWidget->saveCurrentDocument(m_currentFile)) {
            QMessageBox::warning(this, "unimalen", "Cannot save file " + m_currentFile);
        }
    }
}

void MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save Image", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "OpenRaster Files (*.ora);;PNG Files (*.png)");

    if (!fileName.isEmpty()) {
        if (m_tabWidget->saveCurrentDocument(fileName)) {
            setCurrentFile(fileName);
        } else {
            QMessageBox::warning(this, "unimalen", "Cannot save file " + fileName);
        }
    }
}

void MainWindow::setScale1x()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setScaleFactor(1);
        canvas->setPixelZoomMode(false);
        m_pixelZoomAction->setChecked(false);
    }
}

void MainWindow::setScale2x()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setScaleFactor(2);
        canvas->setPixelZoomMode(false);
        m_pixelZoomAction->setChecked(false);
    }
}

void MainWindow::setScale4x()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setScaleFactor(4);
        canvas->setPixelZoomMode(false);
        m_pixelZoomAction->setChecked(false);
    }
}

void MainWindow::togglePixelZoom(bool enabled)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setPixelZoomMode(enabled);
        if (enabled) {
            // Disable regular scaling when in pixel zoom mode
            QAction *checkedAction = m_scaleGroup->checkedAction();
            if (checkedAction) {
                checkedAction->setChecked(false);
            }
        }
    }
}

void MainWindow::toggleCoordinates(bool enabled)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setShowCoordinates(enabled);
    }
}

void MainWindow::onPencilSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setPencilMode(true);
}

void MainWindow::onTextSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setTextMode(true);
}

void MainWindow::onSpraySelected(int diameter)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setSprayMode(true, diameter);
}

void MainWindow::onBrushSelected(int diameter)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setBrushMode(true, diameter);
}

void MainWindow::onEraserSelected(int diameter)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setEraserMode(true, diameter);
}

void MainWindow::onLineSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setLineMode(true);
}

void MainWindow::onBezierSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setBezierMode(true);
}

void MainWindow::onScissorsSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setScissorsMode(true);
}

void MainWindow::onFillSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setFillMode(true);
}

void MainWindow::onLassoSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setLassoMode(true);
}

void MainWindow::onSquareSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setSquareMode(true);
}

void MainWindow::onFilledSquareSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setFilledSquareMode(true);
}

void MainWindow::onRoundedSquareSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setRoundedSquareMode(true);
}

void MainWindow::onFilledRoundedSquareSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setFilledRoundedSquareMode(true);
}

void MainWindow::onOvalSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setOvalMode(true);
}

void MainWindow::onFilledOvalSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setFilledOvalMode(true);
}

void MainWindow::onCut()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->cutSelection();
}

void MainWindow::onCopy()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->copySelection();
}

void MainWindow::onPaste()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->pasteSelection();
}

void MainWindow::onFontChanged(const QString &fontFamily)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setTextFont(fontFamily);
}

void MainWindow::onFontSizeChanged(int fontSize)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setTextFontSize(fontSize);
}

void MainWindow::onThicknessSelected(int thickness)
{
    // For now, just store the selected thickness
    // This will be connected to the canvas when line thickness functionality is implemented
    Q_UNUSED(thickness);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    m_currentFile = fileName;
    QString title = "unimalen - ";
    if (fileName.isEmpty()) {
        title += "Untitled";
    } else {
        title += QFileInfo(fileName).baseName();
    }
    setWindowTitle(title);
}

void MainWindow::toggleToolBar(bool visible)
{
    if (visible) {
        // Position and show the toolbar
        QRect mainRect = geometry();
        int spacing = 10;
        m_toolBar->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
        m_toolBar->setWindowTitle(tr("Tools"));
        m_toolBar->show();
        m_toolBar->move(mainRect.left() - m_toolBar->width() - spacing, mainRect.top() + 50);
        m_toolBar->raise();
    } else {
        m_toolBar->hide();
    }
}

void MainWindow::togglePatternBar(bool visible)
{
    if (visible) {
        // Position and show the toolbar
        QRect mainRect = geometry();
        int spacing = 10;
        m_patternBar->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
        m_patternBar->setWindowTitle(tr("Patterns"));
        m_patternBar->show();
        int patternX = mainRect.left() + (mainRect.width() - m_patternBar->width()) / 2;
        int patternY = mainRect.bottom() + spacing;
        m_patternBar->move(patternX, patternY);
        m_patternBar->raise();
    } else {
        m_patternBar->hide();
    }
}

void MainWindow::toggleThicknessBar(bool visible)
{
    if (visible) {
        // Position and show the toolbar
        QRect mainRect = geometry();
        int spacing = 10;
        m_thicknessBar->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
        m_thicknessBar->setWindowTitle(tr("Line Thickness"));
        m_thicknessBar->show();
        m_thicknessBar->move(mainRect.right() + spacing, mainRect.top() + 50);
        m_thicknessBar->raise();
    } else {
        m_thicknessBar->hide();
    }
}

void MainWindow::toggleLayerPanel(bool visible)
{
    if (visible) {
        // Position and show the layer panel
        QRect mainRect = geometry();
        int spacing = 10;
        m_layerPanel->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
        m_layerPanel->setWindowTitle(tr("Layers"));
        m_layerPanel->show();
        m_layerPanel->move(mainRect.right() + spacing, mainRect.top() + 200);
        m_layerPanel->raise();
    } else {
        m_layerPanel->hide();
    }
}

void MainWindow::toggleColorBar(bool visible)
{
    if (visible) {
        m_colorBar->show();
        positionToolbarsAroundCanvas();
    } else {
        m_colorBar->hide();
    }
}

void MainWindow::newTab()
{
    m_tabWidget->newTab();
}

void MainWindow::closeTab()
{
    int currentIndex = m_tabWidget->currentIndex();
    m_tabWidget->closeTab(currentIndex);
}

void MainWindow::onTabChanged(int index)
{
    Q_UNUSED(index)

    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        connectCanvasSignals(canvas);
    }

    // Update window title based on current tab
    QString currentTabName = m_tabWidget->tabName(m_tabWidget->currentIndex());
    if (!currentTabName.isEmpty()) {
        setWindowTitle(QString("unimalen - %1").arg(currentTabName));
    }
}

void MainWindow::switchToTab(int tabNumber)
{
    // tabNumber is 1-based from keyboard shortcuts
    int index = tabNumber - 1;
    if (index >= 0 && index < m_tabWidget->count()) {
        m_tabWidget->setCurrentTab(index);
    }
}

// Layer management slots
void MainWindow::onLayerAdded()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->addLayer();
    }
}

void MainWindow::onLayerDeleted(int index)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && canvas->layers().size() > 1) {
        canvas->deleteLayer(index);
    }
}

void MainWindow::onLayerDuplicated(int index)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->duplicateLayer(index);
    }
}

void MainWindow::onLayerSelected(int index)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setCurrentLayerIndex(index);
    }
}

void MainWindow::onLayerVisibilityChanged(int index, bool visible)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setLayerVisibility(index, visible);
    }
}

void MainWindow::onLayerOpacityChanged(int index, qreal opacity)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setLayerOpacity(index, opacity);
    }
}

void MainWindow::onLayerBlendModeChanged(int index, int mode)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setLayerBlendMode(index, static_cast<Layer::BlendMode>(mode));
    }
}

void MainWindow::onLayerRenamed(int index, const QString &name)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setLayerName(index, name);
    }
}

void MainWindow::onLayersChanged()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && m_layerPanel->isVisible()) {
        m_layerPanel->setLayers(canvas->layers());
    }
}

void MainWindow::onCurrentLayerChanged(int index)
{
    if (m_layerPanel->isVisible()) {
        m_layerPanel->setCurrentLayerIndex(index);
    }
}

Canvas* MainWindow::getCurrentCanvas()
{
    return m_tabWidget->currentCanvas();
}

void MainWindow::connectCanvasSignals(Canvas *canvas)
{
    if (!canvas) {
        m_undoAction->setEnabled(false);
        m_redoAction->setEnabled(false);
        return;
    }

    // Disconnect from previous canvas
    disconnect(this, SLOT(m_undoAction));
    disconnect(this, SLOT(m_redoAction));

    // Connect undo stack to actions
    connect(canvas->undoStack(), &QUndoStack::canUndoChanged, m_undoAction, &QAction::setEnabled);
    connect(canvas->undoStack(), &QUndoStack::canRedoChanged, m_redoAction, &QAction::setEnabled);

    // Update initial state
    m_undoAction->setEnabled(canvas->undoStack()->canUndo());
    m_redoAction->setEnabled(canvas->undoStack()->canRedo());

    // Connect layer signals
    connect(canvas, &Canvas::layersChanged, this, &MainWindow::onLayersChanged);
    connect(canvas, &Canvas::currentLayerChanged, this, &MainWindow::onCurrentLayerChanged);

    // Connect layer panel signals to canvas operations
    connect(m_layerPanel, &LayerPanel::layerSelected, this, &MainWindow::onLayerSelected);
    connect(m_layerPanel, &LayerPanel::layerAdded, this, &MainWindow::onLayerAdded);
    connect(m_layerPanel, &LayerPanel::layerDeleted, this, &MainWindow::onLayerDeleted);
    connect(m_layerPanel, &LayerPanel::layerDuplicated, this, &MainWindow::onLayerDuplicated);
    connect(m_layerPanel, &LayerPanel::layerVisibilityChanged, this, &MainWindow::onLayerVisibilityChanged);
    connect(m_layerPanel, &LayerPanel::layerOpacityChanged, this, &MainWindow::onLayerOpacityChanged);
    connect(m_layerPanel, &LayerPanel::layerBlendModeChanged, this, &MainWindow::onLayerBlendModeChanged);
    connect(m_layerPanel, &LayerPanel::layerRenamed, this, &MainWindow::onLayerRenamed);

    // Initialize layer panel with current canvas layers
    if (m_layerPanel->isVisible()) {
        m_layerPanel->setLayers(canvas->layers());
        m_layerPanel->setCurrentLayerIndex(canvas->currentLayerIndex());
    }
}

void MainWindow::positionToolbarsAroundCanvas()
{
    // Get main window position and size
    QRect mainRect = geometry();
    int spacing = 10;

    // Position ToolBar to the left of the main window (only if it should be visible)
    if (m_showToolBarAction->isChecked()) {
        m_toolBar->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
        m_toolBar->setWindowTitle(tr("Tools"));
        m_toolBar->show();
        m_toolBar->move(mainRect.left() - m_toolBar->width() - spacing, mainRect.top() + 50);
        m_toolBar->raise();
    }

    // Position PatternBar below the main window (only if it should be visible)
    if (m_showPatternBarAction->isChecked()) {
        m_patternBar->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
        m_patternBar->setWindowTitle(tr("Patterns"));
        m_patternBar->show();
        int patternX = mainRect.left() + (mainRect.width() - m_patternBar->width()) / 2;
        int patternY = mainRect.bottom() + spacing;
        m_patternBar->move(patternX, patternY);
        m_patternBar->raise();
    }

    // Position ThicknessBar to the right of the main window (only if it should be visible)
    if (m_showThicknessBarAction->isChecked()) {
        m_thicknessBar->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
        m_thicknessBar->setWindowTitle(tr("Line Thickness"));
        m_thicknessBar->show();
        m_thicknessBar->move(mainRect.right() + spacing, mainRect.top() + 50);
        m_thicknessBar->raise();
    }

    // Position ColorBar to the left of the main window, below the toolbar
    m_colorBar->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    m_colorBar->setWindowTitle(tr("Pastel Colors"));
    m_colorBar->show();
    int colorY = mainRect.top() + 50;
    if (m_showToolBarAction->isChecked()) {
        colorY = mainRect.top() + 350; // Below toolbar
    }
    m_colorBar->move(mainRect.left() - m_colorBar->width() - spacing, colorY);
    m_colorBar->raise();

    // Position LayerPanel to the right of the main window, below the thickness bar (only if it should be visible)
    if (m_showLayerPanelAction->isChecked()) {
        m_layerPanel->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
        m_layerPanel->setWindowTitle(tr("Layers"));
        m_layerPanel->show();
        int layerY = mainRect.top() + 200; // Below thickness bar
        if (m_showThicknessBarAction->isChecked()) {
            layerY = mainRect.top() + 100; // Adjust if thickness bar is visible
        }
        m_layerPanel->move(mainRect.right() + spacing, layerY);
        m_layerPanel->raise();

        // Initialize with current canvas layers if available
        Canvas *currentCanvas = getCurrentCanvas();
        if (currentCanvas) {
            m_layerPanel->setLayers(currentCanvas->layers());
            m_layerPanel->setCurrentLayerIndex(currentCanvas->currentLayerIndex());
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // Delay repositioning slightly to allow window to finish resizing
    QTimer::singleShot(50, this, &MainWindow::positionToolbarsAroundCanvas);
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
    // Delay repositioning slightly to allow window to finish moving
    QTimer::singleShot(50, this, &MainWindow::positionToolbarsAroundCanvas);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("Quit unimalen"),
        tr("Are you sure you want to quit?"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No  // Default to No
    );

    if (reply == QMessageBox::Yes) {
        // Close all floating toolbars before quitting
        m_toolBar->close();
        m_patternBar->close();
        m_thicknessBar->close();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::setPageSize(Unimalen::PageSize size)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && canvas->document()) {
        canvas->document()->setPageSize(size);
        canvas->setFixedSize(canvas->document()->width() * canvas->getScaleFactor(),
                            canvas->document()->height() * canvas->getScaleFactor());
        canvas->compositeAllLayers();
        canvas->update();
        positionToolbarsAroundCanvas();
    }
}

void MainWindow::insertImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Insert Image"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        tr("Image Files (*.png *.jpg *.jpeg *.bmp *.gif);;All Files (*)"));

    if (!fileName.isEmpty()) {
        Canvas *canvas = getCurrentCanvas();
        if (canvas) {
            QPixmap image(fileName);
            if (image.isNull()) {
                QMessageBox::warning(this, tr("Insert Image"), tr("Cannot load image file."));
                return;
            }

            // Insert image at center of canvas
            QPoint center(canvas->document()->width() / 2, canvas->document()->height() / 2);
            canvas->insertImageAt(image, center);
        }
    }
}

// Page management
void MainWindow::onPageAdded()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && canvas->document()) {
        canvas->document()->addPage();
        canvas->document()->setCurrentPageIndex(canvas->document()->pageCount() - 1);
        canvas->updateCanvasSize();
        canvas->compositeAllLayers();
        canvas->update();
        emit canvas->layersChanged();
        updatePageIndicator();
        statusBar()->showMessage(tr("Page added"), 2000);
    }
}

void MainWindow::onPageDeleted()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && canvas->document()) {
        if (canvas->document()->pageCount() <= 1) {
            statusBar()->showMessage(tr("Cannot delete the only page"), 2000);
            return;
        }

        int currentPage = canvas->document()->currentPageIndex();
        canvas->document()->deletePage(currentPage);
        canvas->updateCanvasSize();
        canvas->compositeAllLayers();
        canvas->update();
        emit canvas->layersChanged();
        updatePageIndicator();
        statusBar()->showMessage(tr("Page deleted"), 2000);
    }
}

void MainWindow::onPageDuplicated()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && canvas->document()) {
        int currentPage = canvas->document()->currentPageIndex();
        canvas->document()->duplicatePage(currentPage);
        canvas->document()->setCurrentPageIndex(currentPage + 1);
        canvas->updateCanvasSize();
        canvas->compositeAllLayers();
        canvas->update();
        emit canvas->layersChanged();
        updatePageIndicator();
        statusBar()->showMessage(tr("Page duplicated"), 2000);
    }
}

void MainWindow::nextPage()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && canvas->document()) {
        int current = canvas->document()->currentPageIndex();
        int total = canvas->document()->pageCount();
        if (current < total - 1) {
            canvas->document()->setCurrentPageIndex(current + 1);
            canvas->compositeAllLayers();
            canvas->update();
            emit canvas->layersChanged(); // Update layer panel
            updatePageIndicator();
        }
    }
}

void MainWindow::previousPage()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && canvas->document()) {
        int current = canvas->document()->currentPageIndex();
        if (current > 0) {
            canvas->document()->setCurrentPageIndex(current - 1);
            canvas->compositeAllLayers();
            canvas->update();
            emit canvas->layersChanged(); // Update layer panel
            updatePageIndicator();
        }
    }
}

void MainWindow::updatePageIndicator()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && canvas->document()) {
        int current = canvas->document()->currentPageIndex() + 1;
        int total = canvas->document()->pageCount();
        m_pageIndicator->setText(tr("Page %1 of %2").arg(current).arg(total));
    } else {
        m_pageIndicator->setText(tr("Page 1 of 1"));
    }
}