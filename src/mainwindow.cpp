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
#include <QSettings>
#include <QDockWidget>
#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>

// Define static const
const int MainWindow::MaxRecentFiles;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_currentFile("")
{
    m_tabWidget = new TabWidget(this);
    m_toolBar = new ToolBar(this);
    m_patternBar = new PatternBar(this);
    m_thicknessBar = new ThicknessBar(this);
    m_layerPanel = new LayerPanel(this);
    m_colorBar = new ColorBar(this, ColorBar::Pastels);

    setCentralWidget(m_tabWidget);

    // Create dock widgets for all toolbars BEFORE createMenus() so toggleViewAction() is available
    // Left side: Tools at top, Colors below
    m_toolBarDock = new QDockWidget(tr("Tools"), this);
    m_toolBarDock->setWidget(m_toolBar);
    m_toolBarDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_toolBarDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::LeftDockWidgetArea, m_toolBarDock);

    m_colorBarDock = new QDockWidget(tr("Colors"), this);
    m_colorBarDock->setWidget(m_colorBar);
    m_colorBarDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_colorBarDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::LeftDockWidgetArea, m_colorBarDock);

    // Split the left area vertically so both can show
    splitDockWidget(m_toolBarDock, m_colorBarDock, Qt::Vertical);

    // Right side: Thickness at top, Layers below
    m_thicknessBarDock = new QDockWidget(tr("Line Thickness"), this);
    m_thicknessBarDock->setWidget(m_thicknessBar);
    m_thicknessBarDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_thicknessBarDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::RightDockWidgetArea, m_thicknessBarDock);

    m_layerPanelDock = new QDockWidget(tr("Layers"), this);
    m_layerPanelDock->setWidget(m_layerPanel);
    m_layerPanelDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_layerPanelDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::RightDockWidgetArea, m_layerPanelDock);

    // Split the right area vertically
    splitDockWidget(m_thicknessBarDock, m_layerPanelDock, Qt::Vertical);

    // Bottom: Patterns
    m_patternBarDock = new QDockWidget(tr("Patterns"), this);
    m_patternBarDock->setWidget(m_patternBar);
    m_patternBarDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    m_patternBarDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::BottomDockWidgetArea, m_patternBarDock);

    // Use dock widget's built-in toggle view actions for proper visibility management
    m_showToolBarAction = m_toolBarDock->toggleViewAction();
    m_showToolBarAction->setText(tr("Show &Tools"));

    m_showColorBarAction = m_colorBarDock->toggleViewAction();
    m_showColorBarAction->setText(tr("Show &Colors"));

    m_showThicknessBarAction = m_thicknessBarDock->toggleViewAction();
    m_showThicknessBarAction->setText(tr("Show &Line Thickness"));

    m_showLayerPanelAction = m_layerPanelDock->toggleViewAction();
    m_showLayerPanelAction->setText(tr("Show &Layers"));

    m_showPatternBarAction = m_patternBarDock->toggleViewAction();
    m_showPatternBarAction->setText(tr("Show &Patterns"));

    // Now create actions and menus (after dock widgets so toggleViewActions are available)
    createActions();
    createMenus();

    // Create page indicator in status bar
    m_pageIndicator = new QLabel(this);
    m_pageIndicator->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_pageIndicator->setMinimumWidth(100);
    statusBar()->addPermanentWidget(m_pageIndicator);
    updatePageIndicator();

    // Create additional status bar labels
    m_statusCursorLabel = new QLabel("Cursor: (0, 0)", this);
    m_statusCursorLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_statusCursorLabel->setMinimumWidth(120);
    statusBar()->addWidget(m_statusCursorLabel);

    m_statusZoomLabel = new QLabel("Zoom: 100%", this);
    m_statusZoomLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_statusZoomLabel->setMinimumWidth(100);
    statusBar()->addWidget(m_statusZoomLabel);

    m_statusCanvasSizeLabel = new QLabel("Canvas: 576x720", this);
    m_statusCanvasSizeLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_statusCanvasSizeLabel->setMinimumWidth(150);
    statusBar()->addWidget(m_statusCanvasSizeLabel);

    m_statusMemoryLabel = new QLabel("Memory: ~50MB", this);
    m_statusMemoryLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_statusMemoryLabel->setMinimumWidth(120);
    statusBar()->addWidget(m_statusMemoryLabel);

    // Connect toolbar signals
    connect(m_toolBar, &ToolBar::pencilToolSelected, this, &MainWindow::onPencilSelected);
    connect(m_toolBar, &ToolBar::textToolSelected, this, &MainWindow::onTextSelected);
    connect(m_toolBar, &ToolBar::sprayToolSelected, this, &MainWindow::onSpraySelected);
    connect(m_toolBar, &ToolBar::brushToolSelected, this, &MainWindow::onBrushSelected);
    connect(m_toolBar, &ToolBar::markerToolSelected, this, &MainWindow::onMarkerSelected);
    connect(m_toolBar, &ToolBar::eraserToolSelected, this, &MainWindow::onEraserSelected);
    connect(m_toolBar, &ToolBar::lineToolSelected, this, &MainWindow::onLineSelected);
    connect(m_toolBar, &ToolBar::bezierToolSelected, this, &MainWindow::onBezierSelected);
    connect(m_toolBar, &ToolBar::scissorsToolSelected, this, &MainWindow::onScissorsSelected);
    connect(m_toolBar, &ToolBar::fillToolSelected, this, &MainWindow::onFillSelected);
    connect(m_toolBar, &ToolBar::lassoToolSelected, this, &MainWindow::onLassoSelected);
    connect(m_toolBar, &ToolBar::rectSelectToolSelected, this, &MainWindow::onRectSelectSelected);
    connect(m_toolBar, &ToolBar::eyedropperToolSelected, this, &MainWindow::onEyedropperSelected);
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

    connect(m_tabWidget, &TabWidget::currentChanged, this, &MainWindow::onTabChanged);

    // Connect signals for the initial canvas
    Canvas *initialCanvas = getCurrentCanvas();
    if (initialCanvas) {
        connectCanvasSignals(initialCanvas);
    }

    // Initialize recent files
    loadRecentFiles();
    updateRecentFilesMenu();

    // Initialize preferences and auto-save
    loadPreferences();
    m_autoSaveTimer = new QTimer(this);
    connect(m_autoSaveTimer, &QTimer::timeout, this, [this]() {
        if (m_autoSaveEnabled) {
            Canvas *canvas = getCurrentCanvas();
            if (canvas && canvas->isModified() && !m_currentFile.isEmpty()) {
                canvas->saveCanvas(m_currentFile + ".autosave");
            }
        }
    });
    applyAutoSaveSettings();

    setWindowTitle(tr("erstelle - Untitled"));
    resize(800, 600);
}

void MainWindow::createActions()
{
    m_newAction = new QAction(tr("&New"), this);
    m_newAction->setShortcut(QKeySequence::New);
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newFile);

    m_newFromClipboardAction = new QAction(tr("New from &Clipboard"), this);
    m_newFromClipboardAction->setShortcut(QKeySequence("Ctrl+Shift+V"));
    connect(m_newFromClipboardAction, &QAction::triggered, this, &MainWindow::newFromClipboard);

    m_openAction = new QAction(tr("&Open..."), this);
    m_openAction->setShortcut(QKeySequence::Open);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);

    m_saveAction = new QAction(tr("&Save"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    m_saveAsAction = new QAction(tr("Save &As..."), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);

    m_exportAction = new QAction(tr("&Export As..."), this);
    m_exportAction->setShortcut(QKeySequence("Ctrl+Shift+E"));
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::exportFile);

    m_preferencesAction = new QAction(tr("&Preferences..."), this);
    m_preferencesAction->setShortcut(QKeySequence::Preferences);
    connect(m_preferencesAction, &QAction::triggered, this, &MainWindow::showPreferences);

    m_insertImageAction = new QAction(tr("&Insert Image..."), this);
    m_insertImageAction->setShortcut(QKeySequence("Ctrl+Shift+I"));
    connect(m_insertImageAction, &QAction::triggered, this, &MainWindow::insertImage);

    m_convertToBlackAndWhiteAction = new QAction(tr("Convert to &Black and White"), this);
    m_convertToBlackAndWhiteAction->setShortcut(QKeySequence("Ctrl+B"));
    connect(m_convertToBlackAndWhiteAction, &QAction::triggered, this, &MainWindow::convertToBlackAndWhite);

    m_adjustBrightnessContrastAction = new QAction(tr("Adjust &Brightness/Contrast..."), this);
    connect(m_adjustBrightnessContrastAction, &QAction::triggered, this, &MainWindow::adjustBrightnessContrast);

    m_posterizeAction = new QAction(tr("&Posterize..."), this);
    connect(m_posterizeAction, &QAction::triggered, this, &MainWindow::posterize);

    m_halftoneAction = new QAction(tr("&Halftone/Dither..."), this);
    connect(m_halftoneAction, &QAction::triggered, this, &MainWindow::halftone);

    m_edgeDetectAction = new QAction(tr("&Edge Detect"), this);
    connect(m_edgeDetectAction, &QAction::triggered, this, &MainWindow::edgeDetect);

    m_invertColorsAction = new QAction(tr("&Invert Colors"), this);
    m_invertColorsAction->setShortcut(QKeySequence("Ctrl+I"));
    connect(m_invertColorsAction, &QAction::triggered, this, &MainWindow::invertColors);

    m_despeckleAction = new QAction(tr("&Despeckle/Cleanup..."), this);
    connect(m_despeckleAction, &QAction::triggered, this, &MainWindow::despeckle);

    m_autoLevelsAction = new QAction(tr("Auto &Levels"), this);
    connect(m_autoLevelsAction, &QAction::triggered, this, &MainWindow::autoLevels);

    m_rotateClockwiseAction = new QAction(tr("Rotate &Clockwise"), this);
    m_rotateClockwiseAction->setShortcut(QKeySequence("Ctrl+R"));
    connect(m_rotateClockwiseAction, &QAction::triggered, this, &MainWindow::rotateClockwise);

    m_rotateCounterClockwiseAction = new QAction(tr("Rotate &Counter-Clockwise"), this);
    m_rotateCounterClockwiseAction->setShortcut(QKeySequence("Ctrl+Shift+R"));
    connect(m_rotateCounterClockwiseAction, &QAction::triggered, this, &MainWindow::rotateCounterClockwise);

    m_rotate180Action = new QAction(tr("Rotate 1&80°"), this);
    connect(m_rotate180Action, &QAction::triggered, this, &MainWindow::rotate180);

    m_flipHorizontalAction = new QAction(tr("Flip &Horizontal"), this);
    m_flipHorizontalAction->setShortcut(QKeySequence("Ctrl+H"));
    connect(m_flipHorizontalAction, &QAction::triggered, this, &MainWindow::flipHorizontal);

    m_flipVerticalAction = new QAction(tr("Flip &Vertical"), this);
    m_flipVerticalAction->setShortcut(QKeySequence("Ctrl+Shift+H"));
    connect(m_flipVerticalAction, &QAction::triggered, this, &MainWindow::flipVertical);

    m_scaleImageAction = new QAction(tr("&Scale/Resize..."), this);
    m_scaleImageAction->setShortcut(QKeySequence("Ctrl+E"));
    connect(m_scaleImageAction, &QAction::triggered, this, &MainWindow::scaleImage);

    m_cropImageAction = new QAction(tr("C&rop..."), this);
    m_cropImageAction->setShortcut(QKeySequence("Ctrl+Shift+X"));
    connect(m_cropImageAction, &QAction::triggered, this, &MainWindow::cropImage);

    m_addDropShadowAction = new QAction(tr("Add &Drop Shadow..."), this);
    connect(m_addDropShadowAction, &QAction::triggered, this, &MainWindow::addDropShadow);

    m_addBorderAction = new QAction(tr("Add B&order..."), this);
    connect(m_addBorderAction, &QAction::triggered, this, &MainWindow::addBorder);

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);

    m_newTabAction = new QAction(tr("New &Tab"), this);
    m_newTabAction->setShortcut(QKeySequence("Ctrl+T"));
    connect(m_newTabAction, &QAction::triggered, this, &MainWindow::newTab);

    m_closeTabAction = new QAction(tr("&Close Tab"), this);
    m_closeTabAction->setShortcut(QKeySequence("Ctrl+W"));
    connect(m_closeTabAction, &QAction::triggered, this, &MainWindow::closeTab);

    // Recent files actions
    for (int i = 0; i < MaxRecentFiles; ++i) {
        m_recentFileActions[i] = new QAction(this);
        m_recentFileActions[i]->setVisible(false);
        connect(m_recentFileActions[i], &QAction::triggered, this, &MainWindow::openRecentFile);
    }

    m_clearRecentFilesAction = new QAction(tr("Clear Recent Files"), this);
    connect(m_clearRecentFilesAction, &QAction::triggered, this, &MainWindow::clearRecentFiles);

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

    // Transform selection actions
    m_rotateSelection90Action = new QAction(tr("Rotate Selection 90° CW"), this);
    m_rotateSelection90Action->setShortcut(QKeySequence("Ctrl+Alt+R"));
    connect(m_rotateSelection90Action, &QAction::triggered, this, &MainWindow::onRotateSelection90);

    m_rotateSelection180Action = new QAction(tr("Rotate Selection 180°"), this);
    m_rotateSelection180Action->setShortcut(QKeySequence("Ctrl+Alt+Shift+R"));
    connect(m_rotateSelection180Action, &QAction::triggered, this, &MainWindow::onRotateSelection180);

    m_rotateSelection270Action = new QAction(tr("Rotate Selection 270° CW"), this);
    m_rotateSelection270Action->setShortcut(QKeySequence("Ctrl+Alt+L"));
    connect(m_rotateSelection270Action, &QAction::triggered, this, &MainWindow::onRotateSelection270);

    m_flipSelectionHorizontalAction = new QAction(tr("Flip Selection Horizontal"), this);
    m_flipSelectionHorizontalAction->setShortcut(QKeySequence("Ctrl+Alt+H"));
    connect(m_flipSelectionHorizontalAction, &QAction::triggered, this, &MainWindow::onFlipSelectionHorizontal);

    m_flipSelectionVerticalAction = new QAction(tr("Flip Selection Vertical"), this);
    m_flipSelectionVerticalAction->setShortcut(QKeySequence("Ctrl+Alt+V"));
    connect(m_flipSelectionVerticalAction, &QAction::triggered, this, &MainWindow::onFlipSelectionVertical);

    // Zoom actions
    m_zoom25Action = new QAction("25%", this);
    m_zoom25Action->setCheckable(true);
    connect(m_zoom25Action, &QAction::triggered, this, &MainWindow::setZoom25);

    m_zoom50Action = new QAction("50%", this);
    m_zoom50Action->setCheckable(true);
    connect(m_zoom50Action, &QAction::triggered, this, &MainWindow::setZoom50);

    m_scale1xAction = new QAction("100%", this);
    m_scale1xAction->setCheckable(true);
    m_scale1xAction->setChecked(true);
    m_scale1xAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0)); // Ctrl+0 for 100%
    connect(m_scale1xAction, &QAction::triggered, this, &MainWindow::setScale1x);

    m_zoom200Action = new QAction("200%", this);
    m_zoom200Action->setCheckable(true);
    connect(m_zoom200Action, &QAction::triggered, this, &MainWindow::setZoom200);

    m_scale2xAction = new QAction("Unused 200%", this); // Keep for compatibility
    m_scale2xAction->setCheckable(true);
    m_scale2xAction->setVisible(false);
    connect(m_scale2xAction, &QAction::triggered, this, &MainWindow::setScale2x);

    m_scale4xAction = new QAction("400%", this);
    m_scale4xAction->setCheckable(true);
    connect(m_scale4xAction, &QAction::triggered, this, &MainWindow::setScale4x);

    m_zoomFitAction = new QAction("Fit to Window", this);
    m_zoomFitAction->setCheckable(true);
    m_zoomFitAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F)); // Ctrl+F for fit
    connect(m_zoomFitAction, &QAction::triggered, this, &MainWindow::setZoomFit);

    m_zoomInAction = new QAction("Zoom In", this);
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn); // Ctrl+Plus
    connect(m_zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);

    m_zoomOutAction = new QAction("Zoom Out", this);
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut); // Ctrl+Minus
    connect(m_zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);

    m_pixelZoomAction = new QAction(tr("Pixel Zoom"), this);
    m_pixelZoomAction->setCheckable(true);
    connect(m_pixelZoomAction, &QAction::toggled, this, &MainWindow::togglePixelZoom);

    m_showCoordinatesAction = new QAction(tr("Show Coordinates"), this);
    m_showCoordinatesAction->setCheckable(true);
    connect(m_showCoordinatesAction, &QAction::toggled, this, &MainWindow::toggleCoordinates);

    m_scaleGroup = new QActionGroup(this);
    m_scaleGroup->addAction(m_zoom25Action);
    m_scaleGroup->addAction(m_zoom50Action);
    m_scaleGroup->addAction(m_scale1xAction);
    m_scaleGroup->addAction(m_zoom200Action);
    m_scaleGroup->addAction(m_scale4xAction);
    m_scaleGroup->addAction(m_zoomFitAction);

    // Toolbar visibility actions - these will be set up in constructor after dock widgets are created

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

    // Zine font actions
    m_specialEliteFontAction = new QAction("Special Elite (Typewriter)", this);
    m_specialEliteFontAction->setCheckable(true);
    connect(m_specialEliteFontAction, &QAction::triggered, this, [this]() { onFontChanged("Special Elite"); });

    m_permanentMarkerFontAction = new QAction("Permanent Marker", this);
    m_permanentMarkerFontAction->setCheckable(true);
    connect(m_permanentMarkerFontAction, &QAction::triggered, this, [this]() { onFontChanged("Permanent Marker"); });

    m_bangersFontAction = new QAction("Bangers (Comic)", this);
    m_bangersFontAction->setCheckable(true);
    connect(m_bangersFontAction, &QAction::triggered, this, [this]() { onFontChanged("Bangers"); });

    m_creepsterFontAction = new QAction("Creepster (Horror)", this);
    m_creepsterFontAction->setCheckable(true);
    connect(m_creepsterFontAction, &QAction::triggered, this, [this]() { onFontChanged("Creepster"); });

    m_pressStart2PFontAction = new QAction("Press Start 2P (Pixel)", this);
    m_pressStart2PFontAction->setCheckable(true);
    connect(m_pressStart2PFontAction, &QAction::triggered, this, [this]() { onFontChanged("Press Start 2P"); });

    m_caveatFontAction = new QAction("Caveat (Handwriting)", this);
    m_caveatFontAction->setCheckable(true);
    connect(m_caveatFontAction, &QAction::triggered, this, [this]() { onFontChanged("Caveat"); });

    m_indieFlowerFontAction = new QAction("Indie Flower (Handwriting)", this);
    m_indieFlowerFontAction->setCheckable(true);
    connect(m_indieFlowerFontAction, &QAction::triggered, this, [this]() { onFontChanged("Indie Flower"); });

    m_vt323FontAction = new QAction("VT323 (Terminal)", this);
    m_vt323FontAction->setCheckable(true);
    connect(m_vt323FontAction, &QAction::triggered, this, [this]() { onFontChanged("VT323"); });

    m_robotoMonoFontAction = new QAction("Roboto Mono", this);
    m_robotoMonoFontAction->setCheckable(true);
    connect(m_robotoMonoFontAction, &QAction::triggered, this, [this]() { onFontChanged("Roboto Mono"); });

    m_amaticFontAction = new QAction("Amatic SC (Casual)", this);
    m_amaticFontAction->setCheckable(true);
    connect(m_amaticFontAction, &QAction::triggered, this, [this]() { onFontChanged("Amatic SC"); });

    m_satisfyFontAction = new QAction("Satisfy (Script)", this);
    m_satisfyFontAction->setCheckable(true);
    connect(m_satisfyFontAction, &QAction::triggered, this, [this]() { onFontChanged("Satisfy"); });

    // Letraset-style fonts
    m_bebasNeueFontAction = new QAction("Bebas Neue (Letraset)", this);
    m_bebasNeueFontAction->setCheckable(true);
    connect(m_bebasNeueFontAction, &QAction::triggered, this, [this]() { onFontChanged("Bebas Neue"); });

    m_orbitronFontAction = new QAction("Orbitron (Geometric)", this);
    m_orbitronFontAction->setCheckable(true);
    connect(m_orbitronFontAction, &QAction::triggered, this, [this]() { onFontChanged("Orbitron"); });

    m_audiowideFontAction = new QAction("Audiowide (Retro Tech)", this);
    m_audiowideFontAction->setCheckable(true);
    connect(m_audiowideFontAction, &QAction::triggered, this, [this]() { onFontChanged("Audiowide"); });

    m_russoOneFontAction = new QAction("Russo One (Bold)", this);
    m_russoOneFontAction->setCheckable(true);
    connect(m_russoOneFontAction, &QAction::triggered, this, [this]() { onFontChanged("Russo One"); });

    m_righteousFontAction = new QAction("Righteous (1970s)", this);
    m_righteousFontAction->setCheckable(true);
    connect(m_righteousFontAction, &QAction::triggered, this, [this]() { onFontChanged("Righteous"); });

    m_michromaFontAction = new QAction("Michroma (Geometric)", this);
    m_michromaFontAction->setCheckable(true);
    connect(m_michromaFontAction, &QAction::triggered, this, [this]() { onFontChanged("Michroma"); });

    m_fontGroup = new QActionGroup(this);
    m_fontGroup->addAction(m_courierFontAction);
    m_fontGroup->addAction(m_openSansFontAction);
    m_fontGroup->addAction(m_serifFontAction);
    m_fontGroup->addAction(m_notoSansFontAction);
    m_fontGroup->addAction(m_notoSansCJKFontAction);
    m_fontGroup->addAction(m_dejavuSansFontAction);
    m_fontGroup->addAction(m_specialEliteFontAction);
    m_fontGroup->addAction(m_permanentMarkerFontAction);
    m_fontGroup->addAction(m_bangersFontAction);
    m_fontGroup->addAction(m_creepsterFontAction);
    m_fontGroup->addAction(m_pressStart2PFontAction);
    m_fontGroup->addAction(m_caveatFontAction);
    m_fontGroup->addAction(m_indieFlowerFontAction);
    m_fontGroup->addAction(m_vt323FontAction);
    m_fontGroup->addAction(m_robotoMonoFontAction);
    m_fontGroup->addAction(m_amaticFontAction);
    m_fontGroup->addAction(m_satisfyFontAction);
    m_fontGroup->addAction(m_bebasNeueFontAction);
    m_fontGroup->addAction(m_orbitronFontAction);
    m_fontGroup->addAction(m_audiowideFontAction);
    m_fontGroup->addAction(m_russoOneFontAction);
    m_fontGroup->addAction(m_righteousFontAction);
    m_fontGroup->addAction(m_michromaFontAction);

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

    // Paper color actions
    m_paperColorGroup = new QActionGroup(this);
    using Unimalen::PaperColor;

    QAction *whiteAction = new QAction(tr("White"), this);
    whiteAction->setCheckable(true);
    whiteAction->setActionGroup(m_paperColorGroup);
    whiteAction->setChecked(true); // Default to white
    connect(whiteAction, &QAction::triggered, this, [this]() {
        setPaperColor(PaperColor::White);
    });
    m_paperColorActions.append(whiteAction);

    QAction *greyAction = new QAction(tr("Grey"), this);
    greyAction->setCheckable(true);
    greyAction->setActionGroup(m_paperColorGroup);
    connect(greyAction, &QAction::triggered, this, [this]() {
        setPaperColor(PaperColor::Grey);
    });
    m_paperColorActions.append(greyAction);

    QAction *blackAction = new QAction(tr("Black"), this);
    blackAction->setCheckable(true);
    blackAction->setActionGroup(m_paperColorGroup);
    connect(blackAction, &QAction::triggered, this, [this]() {
        setPaperColor(PaperColor::Black);
    });
    m_paperColorActions.append(blackAction);

    // Color palette actions
    m_colorPaletteGroup = new QActionGroup(this);

    QAction *pastelsAction = new QAction(tr("Pastels"), this);
    pastelsAction->setCheckable(true);
    pastelsAction->setActionGroup(m_colorPaletteGroup);
    pastelsAction->setChecked(true); // Default
    connect(pastelsAction, &QAction::triggered, this, [this]() {
        setColorPalette(ColorBar::Pastels);
    });
    m_colorPaletteActions.append(pastelsAction);

    QAction *markersAction = new QAction(tr("Markers"), this);
    markersAction->setCheckable(true);
    markersAction->setActionGroup(m_colorPaletteGroup);
    connect(markersAction, &QAction::triggered, this, [this]() {
        setColorPalette(ColorBar::Markers);
    });
    m_colorPaletteActions.append(markersAction);

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
    fileMenu->addAction(m_newFromClipboardAction);
    fileMenu->addAction(m_newTabAction);
    fileMenu->addSeparator();

    // Add page size submenu
    QMenu *pageSizeMenu = fileMenu->addMenu(tr("Page &Size"));
    for (QAction *action : m_pageSizeActions) {
        pageSizeMenu->addAction(action);
    }

    // Add paper color submenu
    QMenu *paperColorMenu = fileMenu->addMenu(tr("Paper &Color"));
    for (QAction *action : m_paperColorActions) {
        paperColorMenu->addAction(action);
    }

    fileMenu->addSeparator();
    fileMenu->addAction(m_openAction);

    // Add recent files submenu
    QMenu *recentFilesMenu = fileMenu->addMenu(tr("Open &Recent"));
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFilesMenu->addAction(m_recentFileActions[i]);
    }
    recentFilesMenu->addSeparator();
    recentFilesMenu->addAction(m_clearRecentFilesAction);

    fileMenu->addSeparator();
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addAction(m_exportAction);
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
    editMenu->addSeparator();
    QMenu *transformMenu = editMenu->addMenu(tr("&Transform Selection"));
    transformMenu->addAction(m_rotateSelection90Action);
    transformMenu->addAction(m_rotateSelection180Action);
    transformMenu->addAction(m_rotateSelection270Action);
    transformMenu->addSeparator();
    transformMenu->addAction(m_flipSelectionHorizontalAction);
    transformMenu->addAction(m_flipSelectionVerticalAction);
    editMenu->addSeparator();
    editMenu->addAction(m_preferencesAction);

    QMenu *imageMenu = menuBar()->addMenu(tr("&Image"));
    imageMenu->addAction(m_insertImageAction);
    imageMenu->addSeparator();
    imageMenu->addAction(m_convertToBlackAndWhiteAction);
    imageMenu->addAction(m_adjustBrightnessContrastAction);
    imageMenu->addAction(m_posterizeAction);
    imageMenu->addAction(m_halftoneAction);
    imageMenu->addAction(m_edgeDetectAction);
    imageMenu->addAction(m_invertColorsAction);
    imageMenu->addAction(m_despeckleAction);
    imageMenu->addAction(m_autoLevelsAction);
    imageMenu->addSeparator();
    imageMenu->addAction(m_rotateClockwiseAction);
    imageMenu->addAction(m_rotateCounterClockwiseAction);
    imageMenu->addAction(m_rotate180Action);
    imageMenu->addSeparator();
    imageMenu->addAction(m_flipHorizontalAction);
    imageMenu->addAction(m_flipVerticalAction);
    imageMenu->addSeparator();
    imageMenu->addAction(m_scaleImageAction);
    imageMenu->addAction(m_cropImageAction);
    imageMenu->addSeparator();
    imageMenu->addAction(m_addDropShadowAction);
    imageMenu->addAction(m_addBorderAction);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(m_zoomInAction);
    viewMenu->addAction(m_zoomOutAction);
    viewMenu->addSeparator();
    viewMenu->addAction(m_zoom25Action);
    viewMenu->addAction(m_zoom50Action);
    viewMenu->addAction(m_scale1xAction);
    viewMenu->addAction(m_zoom200Action);
    viewMenu->addAction(m_scale4xAction);
    viewMenu->addAction(m_zoomFitAction);
    viewMenu->addSeparator();
    viewMenu->addAction(m_showToolBarAction);
    viewMenu->addAction(m_showPatternBarAction);
    viewMenu->addAction(m_showThicknessBarAction);
    viewMenu->addAction(m_showLayerPanelAction);
    viewMenu->addAction(m_showColorBarAction);
    viewMenu->addSeparator();

    // Add color palette submenu
    QMenu *colorPaletteMenu = viewMenu->addMenu(tr("Color &Palette"));
    for (QAction *action : m_colorPaletteActions) {
        colorPaletteMenu->addAction(action);
    }

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
    fontMenu->addSeparator();
    fontMenu->addAction(m_specialEliteFontAction);
    fontMenu->addAction(m_permanentMarkerFontAction);
    fontMenu->addAction(m_bangersFontAction);
    fontMenu->addAction(m_creepsterFontAction);
    fontMenu->addAction(m_pressStart2PFontAction);
    fontMenu->addAction(m_caveatFontAction);
    fontMenu->addAction(m_indieFlowerFontAction);
    fontMenu->addAction(m_satisfyFontAction);
    fontMenu->addAction(m_amaticFontAction);
    fontMenu->addAction(m_vt323FontAction);
    fontMenu->addAction(m_robotoMonoFontAction);
    fontMenu->addSeparator();
    fontMenu->addAction(m_bebasNeueFontAction);
    fontMenu->addAction(m_orbitronFontAction);
    fontMenu->addAction(m_audiowideFontAction);
    fontMenu->addAction(m_russoOneFontAction);
    fontMenu->addAction(m_righteousFontAction);
    fontMenu->addAction(m_michromaFontAction);

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

void MainWindow::newFromClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    QPixmap pixmap = clipboard->pixmap();

    if (pixmap.isNull()) {
        QMessageBox::information(this, "New from Clipboard", "No image data in clipboard.");
        return;
    }

    // Create a new tab with canvas sized to clipboard image
    m_tabWidget->newTab();
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        // Create a new canvas with the clipboard image size
        canvas->newCanvas();

        // Insert the clipboard image at position (0,0)
        canvas->insertImageAt(pixmap, QPoint(0, 0));

        setCurrentFile("");
        setWindowTitle(tr("erstelle - New from Clipboard"));
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
            QMessageBox::warning(this, "erstelle", "Cannot open file " + fileName);
        }
    }
}

void MainWindow::saveFile()
{
    if (m_currentFile.isEmpty()) {
        saveAsFile();
    } else {
        if (!m_tabWidget->saveCurrentDocument(m_currentFile)) {
            QMessageBox::warning(this, "erstelle", "Cannot save file " + m_currentFile);
        }
    }
}

void MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save Image", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "OpenRaster Files (*.ora);;PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp);;GIF Files (*.gif)");

    if (!fileName.isEmpty()) {
        if (m_tabWidget->saveCurrentDocument(fileName)) {
            setCurrentFile(fileName);
        } else {
            QMessageBox::warning(this, "erstelle", "Cannot save file " + fileName);
        }
    }
}

void MainWindow::exportFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Export Image", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "PNG Files (*.png);;JPEG Files (*.jpg *.jpeg);;BMP Files (*.bmp);;GIF Files (*.gif);;All Files (*)");

    if (!fileName.isEmpty()) {
        Canvas *canvas = getCurrentCanvas();
        if (canvas && canvas->saveCanvas(fileName)) {
            QMessageBox::information(this, "Export Success", "Image exported successfully!");
        } else {
            QMessageBox::warning(this, "erstelle", "Cannot export file " + fileName);
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
        updateStatusBar();
    }
}

void MainWindow::setScale2x()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setScaleFactor(2);
        canvas->setPixelZoomMode(false);
        m_pixelZoomAction->setChecked(false);
        updateStatusBar();
    }
}

void MainWindow::setScale4x()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setScaleFactor(4);
        canvas->setPixelZoomMode(false);
        m_pixelZoomAction->setChecked(false);
        updateStatusBar();
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

void MainWindow::setZoom25()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setZoomLevel(25.0);
        canvas->setPixelZoomMode(false);
        m_pixelZoomAction->setChecked(false);
        updateStatusBar();
    }
}

void MainWindow::setZoom50()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setZoomLevel(50.0);
        canvas->setPixelZoomMode(false);
        m_pixelZoomAction->setChecked(false);
        updateStatusBar();
    }
}

void MainWindow::setZoom200()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        canvas->setZoomLevel(200.0);
        canvas->setPixelZoomMode(false);
        m_pixelZoomAction->setChecked(false);
        updateStatusBar();
    }
}

void MainWindow::setZoomFit()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        // Find the scroll area by traversing up from canvas
        QWidget *parent = canvas->parentWidget();
        QScrollArea *scrollArea = nullptr;
        while (parent && !scrollArea) {
            scrollArea = qobject_cast<QScrollArea*>(parent);
            parent = parent->parentWidget();
        }

        if (scrollArea) {
            QSize viewportSize = scrollArea->viewport()->size();
            double scaleX = (double)viewportSize.width() / 576.0; // CANVAS_WIDTH
            double scaleY = (double)viewportSize.height() / 720.0; // CANVAS_HEIGHT
            double fitScale = qMin(scaleX, scaleY) * 100.0;
            canvas->setZoomLevel(fitScale);
            canvas->setPixelZoomMode(false);
            m_pixelZoomAction->setChecked(false);
            updateStatusBar();
        } else {
            // Fallback to 100% if we can't find the scroll area
            canvas->setZoomLevel(100.0);
            updateStatusBar();
        }
    }
}

void MainWindow::zoomIn()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        double currentZoom = canvas->getZoomLevel();
        // Increase by 25% increments
        double newZoom = currentZoom + 25.0;
        canvas->setZoomLevel(newZoom);
        updateStatusBar();
    }
}

void MainWindow::zoomOut()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) {
        double currentZoom = canvas->getZoomLevel();
        // Decrease by 25% increments
        double newZoom = currentZoom - 25.0;
        canvas->setZoomLevel(newZoom);
        updateStatusBar();
    }
}

void MainWindow::updateStatusBar()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && statusBar()) {
        // Update zoom level
        if (m_statusZoomLabel) {
            m_statusZoomLabel->setText(QString("Zoom: %1%").arg((int)canvas->getZoomLevel()));
        }

        // Update canvas size
        if (m_statusCanvasSizeLabel) {
            m_statusCanvasSizeLabel->setText(QString("Canvas: 576x720"));
        }

        // Update memory usage (simplified)
        if (m_statusMemoryLabel) {
            // In a real implementation, you'd calculate actual memory usage
            m_statusMemoryLabel->setText(QString("Memory: ~50MB"));
        }
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

void MainWindow::onMarkerSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setMarkerMode(true);
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

void MainWindow::onRectSelectSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setRectSelectMode(true);
}

void MainWindow::onEyedropperSelected()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->setEyedropperMode(true);
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

void MainWindow::onRotateSelection90()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->rotateSelection(90);
}

void MainWindow::onRotateSelection180()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->rotateSelection(180);
}

void MainWindow::onRotateSelection270()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->rotateSelection(270);
}

void MainWindow::onFlipSelectionHorizontal()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->flipSelectionHorizontal();
}

void MainWindow::onFlipSelectionVertical()
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas) canvas->flipSelectionVertical();
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
    QString title = "erstelle - ";
    if (fileName.isEmpty()) {
        title += "Untitled";
    } else {
        title += QFileInfo(fileName).baseName();
    }
    setWindowTitle(title);

    // Update recent files
    if (!fileName.isEmpty()) {
        updateRecentFiles(fileName);
    }
}

void MainWindow::updateRecentFiles(const QString &fileName)
{
    // Remove any existing occurrence of this file
    m_recentFiles.removeAll(fileName);

    // Add to the front
    m_recentFiles.prepend(fileName);

    // Keep only MaxRecentFiles
    while (m_recentFiles.size() > MaxRecentFiles) {
        m_recentFiles.removeLast();
    }

    // Update menu and save to settings
    updateRecentFilesMenu();
    saveRecentFiles();
}

void MainWindow::updateRecentFilesMenu()
{
    int numRecentFiles = qMin(m_recentFiles.size(), MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = QString("&%1 %2").arg(i + 1).arg(QFileInfo(m_recentFiles[i]).fileName());
        m_recentFileActions[i]->setText(text);
        m_recentFileActions[i]->setData(m_recentFiles[i]);
        m_recentFileActions[i]->setVisible(true);
    }

    for (int i = numRecentFiles; i < MaxRecentFiles; ++i) {
        m_recentFileActions[i]->setVisible(false);
    }
}

void MainWindow::loadRecentFiles()
{
    QSettings settings("erstelle", "erstelle");
    m_recentFiles = settings.value("recentFiles").toStringList();
}

void MainWindow::saveRecentFiles()
{
    QSettings settings("erstelle", "erstelle");
    settings.setValue("recentFiles", m_recentFiles);
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action) {
        QString fileName = action->data().toString();
        if (QFile::exists(fileName)) {
            if (m_tabWidget->loadDocument(fileName)) {
                setCurrentFile(fileName);
            } else {
                QMessageBox::warning(this, "erstelle", "Cannot open file " + fileName);
            }
        } else {
            QMessageBox::warning(this, "File Not Found", "The file " + fileName + " no longer exists.");
            m_recentFiles.removeAll(fileName);
            updateRecentFilesMenu();
            saveRecentFiles();
        }
    }
}

void MainWindow::clearRecentFiles()
{
    m_recentFiles.clear();
    updateRecentFilesMenu();
    saveRecentFiles();
}

void MainWindow::loadPreferences()
{
    QSettings settings("erstelle", "erstelle");
    m_autoSaveEnabled = settings.value("autoSave/enabled", true).toBool();
    m_autoSaveInterval = settings.value("autoSave/interval", 5).toInt(); // Default 5 minutes
}

void MainWindow::savePreferences()
{
    QSettings settings("erstelle", "erstelle");
    settings.setValue("autoSave/enabled", m_autoSaveEnabled);
    settings.setValue("autoSave/interval", m_autoSaveInterval);
}

void MainWindow::applyAutoSaveSettings()
{
    if (m_autoSaveEnabled && m_autoSaveInterval > 0) {
        m_autoSaveTimer->start(m_autoSaveInterval * 60 * 1000); // Convert minutes to milliseconds
    } else {
        m_autoSaveTimer->stop();
    }
}

void MainWindow::showPreferences()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Preferences"));
    dialog.setMinimumWidth(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

    // Auto-save group
    QGroupBox *autoSaveGroup = new QGroupBox(tr("Auto-Save"), &dialog);
    QVBoxLayout *autoSaveLayout = new QVBoxLayout(autoSaveGroup);

    QCheckBox *autoSaveCheckBox = new QCheckBox(tr("Enable auto-save"), autoSaveGroup);
    autoSaveCheckBox->setChecked(m_autoSaveEnabled);
    autoSaveLayout->addWidget(autoSaveCheckBox);

    QHBoxLayout *intervalLayout = new QHBoxLayout();
    QLabel *intervalLabel = new QLabel(tr("Save interval:"), autoSaveGroup);
    QSpinBox *intervalSpinBox = new QSpinBox(autoSaveGroup);
    intervalSpinBox->setRange(1, 60);
    intervalSpinBox->setValue(m_autoSaveInterval);
    intervalSpinBox->setSuffix(tr(" minutes"));
    intervalLayout->addWidget(intervalLabel);
    intervalLayout->addWidget(intervalSpinBox);
    intervalLayout->addStretch();
    autoSaveLayout->addLayout(intervalLayout);

    QLabel *infoLabel = new QLabel(tr("Auto-save files are saved with .autosave extension"), autoSaveGroup);
    infoLabel->setStyleSheet("color: gray; font-size: 10px;");
    autoSaveLayout->addWidget(infoLabel);

    mainLayout->addWidget(autoSaveGroup);

    // Dialog buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    if (dialog.exec() == QDialog::Accepted) {
        // Save preferences
        m_autoSaveEnabled = autoSaveCheckBox->isChecked();
        m_autoSaveInterval = intervalSpinBox->value();
        savePreferences();
        applyAutoSaveSettings();

        QMessageBox::information(this, tr("Preferences"),
            tr("Preferences saved successfully!"));
    }
}


void MainWindow::newTab()
{
    m_tabWidget->newTab();
}

void MainWindow::closeTab()
{
    int currentIndex = m_tabWidget->currentIndex();
    Canvas *canvas = m_tabWidget->canvasAt(currentIndex);

    // Check if the tab has unsaved changes
    if (canvas && canvas->isModified()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Unsaved Changes"));
        msgBox.setText(tr("This document has unsaved changes."));
        msgBox.setInformativeText(tr("Do you want to save your changes before closing?"));
        msgBox.setIcon(QMessageBox::Warning);

        QPushButton *saveButton = msgBox.addButton(tr("Save"), QMessageBox::AcceptRole);
        QPushButton *discardButton = msgBox.addButton(tr("Don't Save"), QMessageBox::DestructiveRole);
        QPushButton *cancelButton = msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
        msgBox.setDefaultButton(saveButton);

        msgBox.exec();

        if (msgBox.clickedButton() == saveButton) {
            saveFile();
            // If still modified, user cancelled the save dialog
            if (canvas->isModified()) {
                return;  // Don't close the tab
            }
        } else if (msgBox.clickedButton() == cancelButton) {
            return;  // Don't close the tab
        }
        // If discard button clicked, continue with closing
    }

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
        setWindowTitle(QString("erstelle - %1").arg(currentTabName));
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

    // Connect document modified signal to update window title
    connect(canvas, &Canvas::documentModified, this, [this]() {
        Canvas *canvas = getCurrentCanvas();
        if (canvas) {
            QString currentTabName = m_tabWidget->tabName(m_tabWidget->currentIndex());
            QString title = QString("erstelle - %1%2").arg(currentTabName).arg(canvas->isModified() ? " *" : "");
            setWindowTitle(title);
        }
    });

    // Connect layer panel signals to canvas operations
    connect(m_layerPanel, &LayerPanel::layerSelected, this, &MainWindow::onLayerSelected);
    connect(m_layerPanel, &LayerPanel::layerAdded, this, &MainWindow::onLayerAdded);
    connect(m_layerPanel, &LayerPanel::layerDeleted, this, &MainWindow::onLayerDeleted);
    connect(m_layerPanel, &LayerPanel::layerDuplicated, this, &MainWindow::onLayerDuplicated);
    connect(m_layerPanel, &LayerPanel::layerVisibilityChanged, this, &MainWindow::onLayerVisibilityChanged);
    connect(m_layerPanel, &LayerPanel::layerOpacityChanged, this, &MainWindow::onLayerOpacityChanged);
    connect(m_layerPanel, &LayerPanel::layerBlendModeChanged, this, &MainWindow::onLayerBlendModeChanged);
    connect(m_layerPanel, &LayerPanel::layerRenamed, this, &MainWindow::onLayerRenamed);

    // Connect mouse position signal for status bar
    connect(canvas, &Canvas::mousePositionChanged, this, [this](int x, int y) {
        if (m_statusCursorLabel) {
            m_statusCursorLabel->setText(QString("Cursor: (%1, %2)").arg(x).arg(y));
        }
    });

    // Connect color picked signal from eyedropper
    connect(canvas, &Canvas::colorPicked, this, [this](const QColor &color) {
        if (m_colorBar) {
            m_colorBar->setCurrentColor(color);
        }
    });

    // Initialize layer panel with current canvas layers
    if (m_layerPanel->isVisible()) {
        m_layerPanel->setLayers(canvas->layers());
        m_layerPanel->setCurrentLayerIndex(canvas->currentLayerIndex());
    }
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Check for unsaved changes in all tabs
    bool hasUnsavedChanges = false;
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        Canvas *canvas = m_tabWidget->canvasAt(i);
        if (canvas && canvas->isModified()) {
            hasUnsavedChanges = true;
            break;
        }
    }

    if (hasUnsavedChanges) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Unsaved Changes"));
        msgBox.setText(tr("You have unsaved changes."));
        msgBox.setInformativeText(tr("Do you want to save your changes before closing?"));
        msgBox.setIcon(QMessageBox::Warning);

        QPushButton *saveButton = msgBox.addButton(tr("Save"), QMessageBox::AcceptRole);
        QPushButton *discardButton = msgBox.addButton(tr("Don't Save"), QMessageBox::DestructiveRole);
        QPushButton *cancelButton = msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
        msgBox.setDefaultButton(saveButton);

        msgBox.exec();

        if (msgBox.clickedButton() == saveButton) {
            // Save all modified documents
            bool allSaved = true;
            for (int i = 0; i < m_tabWidget->count(); ++i) {
                Canvas *canvas = m_tabWidget->canvasAt(i);
                if (canvas && canvas->isModified()) {
                    m_tabWidget->setCurrentTab(i);
                    saveFile();
                    // Check if still modified (user may have cancelled save dialog)
                    if (canvas->isModified()) {
                        allSaved = false;
                        break;
                    }
                }
            }
            if (!allSaved) {
                event->ignore();
                return;
            }
        } else if (msgBox.clickedButton() == cancelButton) {
            event->ignore();
            return;
        }
        // If discard button clicked, continue with closing
    }

    // Close all floating toolbars before quitting
    m_toolBar->close();
    m_patternBar->close();
    m_thicknessBar->close();
    event->accept();
}

void MainWindow::setPageSize(Unimalen::PageSize size)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && canvas->document()) {
        canvas->document()->setPageSize(size);
        canvas->updateCanvasSize();
        canvas->compositeAllLayers();
        canvas->update();
    }
}

void MainWindow::setPaperColor(Unimalen::PaperColor color)
{
    Canvas *canvas = getCurrentCanvas();
    if (canvas && canvas->document()) {
        canvas->document()->setPaperColor(color);
        canvas->compositeAllLayers();
        canvas->update();
    }
}

void MainWindow::setColorPalette(ColorBar::PaletteType palette)
{
    // Save current color
    QColor currentColor = m_colorBar->currentColor();

    // Remove old color bar from dock
    m_colorBarDock->setWidget(nullptr);
    delete m_colorBar;

    // Create new color bar with selected palette
    m_colorBar = new ColorBar(this, palette);
    m_colorBarDock->setWidget(m_colorBar);

    // Restore current color
    m_colorBar->setCurrentColor(currentColor);

    // Reconnect signals
    connect(m_colorBar, &ColorBar::colorSelected,
            this, [this](const QColor &color) {
        Canvas *canvas = getCurrentCanvas();
        if (canvas) {
            canvas->setCurrentColor(color);
        }
    });
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

void MainWindow::convertToBlackAndWhite()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    // Get the current layer
    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    // Convert to QImage for pixel manipulation
    QImage image = layerPixmap.toImage();
    if (image.isNull()) return;

    // Create dialog for threshold adjustment
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Convert to Black and White"));

    QFormLayout *formLayout = new QFormLayout;

    // Threshold slider
    QSpinBox *thresholdSpinBox = new QSpinBox(&dialog);
    thresholdSpinBox->setRange(0, 255);
    thresholdSpinBox->setValue(128);
    formLayout->addRow(tr("Threshold:"), thresholdSpinBox);

    QLabel *helpLabel = new QLabel(tr("Lower = more black, Higher = more white"), &dialog);
    helpLabel->setWordWrap(true);
    formLayout->addRow(helpLabel);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    dialog.setLayout(mainLayout);

    // Show dialog and process
    if (dialog.exec() == QDialog::Accepted) {
        int threshold = thresholdSpinBox->value();

        // Convert to grayscale first
        image = image.convertToFormat(QImage::Format_Grayscale8);

        // Apply threshold to create pure black and white (no grays)
        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                QColor pixel = image.pixelColor(x, y);
                int gray = pixel.red();  // In grayscale, R=G=B
                if (gray < threshold) {
                    image.setPixelColor(x, y, Qt::black);
                } else {
                    image.setPixelColor(x, y, Qt::white);
                }
            }
        }

        // Apply the converted image back to the layer
        currentLayer.pixmap() = QPixmap::fromImage(image);

        // Update the canvas
        canvas->compositeAllLayers();
        canvas->update();
        canvas->setModified(true);
    }
}

void MainWindow::rotateClockwise()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    // Get the current layer
    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    // Rotate the pixmap 90 degrees clockwise
    QTransform transform;
    transform.rotate(90);
    currentLayer.pixmap() = layerPixmap.transformed(transform);

    // Update the canvas
    canvas->compositeAllLayers();
    canvas->update();
    canvas->setModified(true);
}

void MainWindow::rotateCounterClockwise()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    // Get the current layer
    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    // Rotate the pixmap 90 degrees counter-clockwise
    QTransform transform;
    transform.rotate(-90);
    currentLayer.pixmap() = layerPixmap.transformed(transform);

    // Update the canvas
    canvas->compositeAllLayers();
    canvas->update();
    canvas->setModified(true);
}

void MainWindow::rotate180()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    // Get the current layer
    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    // Rotate the pixmap 180 degrees
    QTransform transform;
    transform.rotate(180);
    currentLayer.pixmap() = layerPixmap.transformed(transform);

    // Update the canvas
    canvas->compositeAllLayers();
    canvas->update();
    canvas->setModified(true);
}

void MainWindow::flipHorizontal()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    // Get the current layer
    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    // Flip the pixmap horizontally
    QTransform transform;
    transform.scale(-1, 1);
    currentLayer.pixmap() = layerPixmap.transformed(transform);

    // Update the canvas
    canvas->compositeAllLayers();
    canvas->update();
    canvas->setModified(true);
}

void MainWindow::flipVertical()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    // Get the current layer
    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    // Flip the pixmap vertically
    QTransform transform;
    transform.scale(1, -1);
    currentLayer.pixmap() = layerPixmap.transformed(transform);

    // Update the canvas
    canvas->compositeAllLayers();
    canvas->update();
    canvas->setModified(true);
}

void MainWindow::scaleImage()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    if (layerPixmap.isNull()) {
        QMessageBox::warning(this, tr("Scale Image"), tr("No image to scale on current layer."));
        return;
    }

    // Create dialog
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Scale/Resize Image"));

    QFormLayout *formLayout = new QFormLayout;

    // Width spinner
    QSpinBox *widthSpinBox = new QSpinBox(&dialog);
    widthSpinBox->setRange(1, 10000);
    widthSpinBox->setValue(layerPixmap.width());
    widthSpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("Width:"), widthSpinBox);

    // Height spinner
    QSpinBox *heightSpinBox = new QSpinBox(&dialog);
    heightSpinBox->setRange(1, 10000);
    heightSpinBox->setValue(layerPixmap.height());
    heightSpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("Height:"), heightSpinBox);

    // Maintain aspect ratio checkbox
    QCheckBox *aspectRatioCheckBox = new QCheckBox(tr("Maintain aspect ratio"), &dialog);
    aspectRatioCheckBox->setChecked(true);
    formLayout->addRow(aspectRatioCheckBox);

    // Smooth scaling checkbox
    QCheckBox *smoothCheckBox = new QCheckBox(tr("Smooth scaling"), &dialog);
    smoothCheckBox->setChecked(true);
    formLayout->addRow(smoothCheckBox);

    // Current size label
    QLabel *currentSizeLabel = new QLabel(tr("Current size: %1 x %2 px").arg(layerPixmap.width()).arg(layerPixmap.height()), &dialog);
    formLayout->addRow(currentSizeLabel);

    // Store original aspect ratio
    double aspectRatio = static_cast<double>(layerPixmap.width()) / layerPixmap.height();

    // Connect width change to height when aspect ratio is locked
    connect(widthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [&](int value) {
        if (aspectRatioCheckBox->isChecked()) {
            heightSpinBox->blockSignals(true);
            heightSpinBox->setValue(static_cast<int>(value / aspectRatio));
            heightSpinBox->blockSignals(false);
        }
    });

    // Connect height change to width when aspect ratio is locked
    connect(heightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [&](int value) {
        if (aspectRatioCheckBox->isChecked()) {
            widthSpinBox->blockSignals(true);
            widthSpinBox->setValue(static_cast<int>(value * aspectRatio));
            widthSpinBox->blockSignals(false);
        }
    });

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    dialog.setLayout(mainLayout);

    // Show dialog and process
    if (dialog.exec() == QDialog::Accepted) {
        int newWidth = widthSpinBox->value();
        int newHeight = heightSpinBox->value();

        Qt::TransformationMode mode = smoothCheckBox->isChecked() ?
            Qt::SmoothTransformation : Qt::FastTransformation;

        currentLayer.pixmap() = layerPixmap.scaled(newWidth, newHeight, Qt::IgnoreAspectRatio, mode);

        // Update the canvas
        canvas->compositeAllLayers();
        canvas->update();
        canvas->setModified(true);
    }
}

void MainWindow::cropImage()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    if (layerPixmap.isNull()) {
        QMessageBox::warning(this, tr("Crop Image"), tr("No image to crop on current layer."));
        return;
    }

    // Create dialog
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Crop Image"));

    QFormLayout *formLayout = new QFormLayout;

    // X position spinner
    QSpinBox *xSpinBox = new QSpinBox(&dialog);
    xSpinBox->setRange(0, layerPixmap.width() - 1);
    xSpinBox->setValue(0);
    xSpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("X:"), xSpinBox);

    // Y position spinner
    QSpinBox *ySpinBox = new QSpinBox(&dialog);
    ySpinBox->setRange(0, layerPixmap.height() - 1);
    ySpinBox->setValue(0);
    ySpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("Y:"), ySpinBox);

    // Width spinner
    QSpinBox *widthSpinBox = new QSpinBox(&dialog);
    widthSpinBox->setRange(1, layerPixmap.width());
    widthSpinBox->setValue(layerPixmap.width());
    widthSpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("Width:"), widthSpinBox);

    // Height spinner
    QSpinBox *heightSpinBox = new QSpinBox(&dialog);
    heightSpinBox->setRange(1, layerPixmap.height());
    heightSpinBox->setValue(layerPixmap.height());
    heightSpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("Height:"), heightSpinBox);

    // Current size label
    QLabel *currentSizeLabel = new QLabel(tr("Original size: %1 x %2 px").arg(layerPixmap.width()).arg(layerPixmap.height()), &dialog);
    formLayout->addRow(currentSizeLabel);

    // Update max values when x/y change
    connect(xSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [&](int value) {
        widthSpinBox->setMaximum(layerPixmap.width() - value);
        if (widthSpinBox->value() > widthSpinBox->maximum()) {
            widthSpinBox->setValue(widthSpinBox->maximum());
        }
    });

    connect(ySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [&](int value) {
        heightSpinBox->setMaximum(layerPixmap.height() - value);
        if (heightSpinBox->value() > heightSpinBox->maximum()) {
            heightSpinBox->setValue(heightSpinBox->maximum());
        }
    });

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    dialog.setLayout(mainLayout);

    // Show dialog and process
    if (dialog.exec() == QDialog::Accepted) {
        int x = xSpinBox->value();
        int y = ySpinBox->value();
        int width = widthSpinBox->value();
        int height = heightSpinBox->value();

        // Validate crop region
        if (width > 0 && height > 0) {
            QRect cropRect(x, y, width, height);
            currentLayer.pixmap() = layerPixmap.copy(cropRect);

            // Update the canvas
            canvas->compositeAllLayers();
            canvas->update();
            canvas->setModified(true);
        }
    }
}

void MainWindow::addDropShadow()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    if (layerPixmap.isNull()) {
        QMessageBox::warning(this, tr("Add Drop Shadow"), tr("No image on current layer."));
        return;
    }

    // Create dialog
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Add Drop Shadow"));

    QFormLayout *formLayout = new QFormLayout;

    // Offset X spinner
    QSpinBox *offsetXSpinBox = new QSpinBox(&dialog);
    offsetXSpinBox->setRange(-100, 100);
    offsetXSpinBox->setValue(5);
    offsetXSpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("Offset X:"), offsetXSpinBox);

    // Offset Y spinner
    QSpinBox *offsetYSpinBox = new QSpinBox(&dialog);
    offsetYSpinBox->setRange(-100, 100);
    offsetYSpinBox->setValue(5);
    offsetYSpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("Offset Y:"), offsetYSpinBox);

    // Blur radius spinner
    QSpinBox *blurSpinBox = new QSpinBox(&dialog);
    blurSpinBox->setRange(0, 50);
    blurSpinBox->setValue(8);
    blurSpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("Blur:"), blurSpinBox);

    // Opacity spinner
    QSpinBox *opacitySpinBox = new QSpinBox(&dialog);
    opacitySpinBox->setRange(0, 100);
    opacitySpinBox->setValue(70);
    opacitySpinBox->setSuffix(tr(" %"));
    formLayout->addRow(tr("Opacity:"), opacitySpinBox);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    dialog.setLayout(mainLayout);

    // Show dialog and process
    if (dialog.exec() == QDialog::Accepted) {
        int offsetX = offsetXSpinBox->value();
        int offsetY = offsetYSpinBox->value();
        int blur = blurSpinBox->value();
        qreal opacity = opacitySpinBox->value() / 100.0;

        // Calculate new size to accommodate shadow
        int newWidth = layerPixmap.width() + qAbs(offsetX) + blur * 2;
        int newHeight = layerPixmap.height() + qAbs(offsetY) + blur * 2;

        // Create new pixmap with space for shadow
        QPixmap resultPixmap(newWidth, newHeight);
        resultPixmap.fill(Qt::transparent);

        QPainter painter(&resultPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        // Calculate shadow position
        int shadowX = qMax(0, offsetX) + blur;
        int shadowY = qMax(0, offsetY) + blur;

        // Draw shadow (simple version - just a semi-transparent black copy)
        painter.setOpacity(opacity);
        painter.drawPixmap(shadowX, shadowY, layerPixmap);

        // Draw original image on top
        int imageX = qMax(0, -offsetX) + blur;
        int imageY = qMax(0, -offsetY) + blur;
        painter.setOpacity(1.0);
        painter.drawPixmap(imageX, imageY, layerPixmap);

        painter.end();

        currentLayer.pixmap() = resultPixmap;

        // Update the canvas
        canvas->compositeAllLayers();
        canvas->update();
        canvas->setModified(true);
    }
}

void MainWindow::addBorder()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    if (layerPixmap.isNull()) {
        QMessageBox::warning(this, tr("Add Border"), tr("No image on current layer."));
        return;
    }

    // Create dialog
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Add Border"));

    QFormLayout *formLayout = new QFormLayout;

    // Width spinner
    QSpinBox *widthSpinBox = new QSpinBox(&dialog);
    widthSpinBox->setRange(1, 100);
    widthSpinBox->setValue(5);
    widthSpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("Width:"), widthSpinBox);

    // Color selection (simplified - black or white)
    QComboBox *colorCombo = new QComboBox(&dialog);
    colorCombo->addItem(tr("Black"), QColor(Qt::black));
    colorCombo->addItem(tr("White"), QColor(Qt::white));
    colorCombo->addItem(tr("Gray"), QColor(Qt::gray));
    formLayout->addRow(tr("Color:"), colorCombo);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    dialog.setLayout(mainLayout);

    // Show dialog and process
    if (dialog.exec() == QDialog::Accepted) {
        int borderWidth = widthSpinBox->value();
        QColor borderColor = colorCombo->currentData().value<QColor>();

        // Calculate new size
        int newWidth = layerPixmap.width() + borderWidth * 2;
        int newHeight = layerPixmap.height() + borderWidth * 2;

        // Create new pixmap with border
        QPixmap resultPixmap(newWidth, newHeight);
        resultPixmap.fill(Qt::transparent);

        QPainter painter(&resultPixmap);
        painter.setRenderHint(QPainter::Antialiasing);

        // Draw border
        painter.setPen(Qt::NoPen);
        painter.setBrush(borderColor);
        painter.drawRect(0, 0, newWidth, newHeight);

        // Draw original image centered
        painter.drawPixmap(borderWidth, borderWidth, layerPixmap);

        painter.end();

        currentLayer.pixmap() = resultPixmap;

        // Update the canvas
        canvas->compositeAllLayers();
        canvas->update();
        canvas->setModified(true);
    }
}

void MainWindow::adjustBrightnessContrast()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    if (layerPixmap.isNull()) {
        QMessageBox::warning(this, tr("Adjust Brightness/Contrast"), tr("No image on current layer."));
        return;
    }

    // Create dialog
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Adjust Brightness/Contrast"));

    QFormLayout *formLayout = new QFormLayout;

    // Brightness slider
    QSpinBox *brightnessSpinBox = new QSpinBox(&dialog);
    brightnessSpinBox->setRange(-100, 100);
    brightnessSpinBox->setValue(0);
    formLayout->addRow(tr("Brightness:"), brightnessSpinBox);

    // Contrast slider
    QSpinBox *contrastSpinBox = new QSpinBox(&dialog);
    contrastSpinBox->setRange(-100, 100);
    contrastSpinBox->setValue(0);
    formLayout->addRow(tr("Contrast:"), contrastSpinBox);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    dialog.setLayout(mainLayout);

    // Show dialog and process
    if (dialog.exec() == QDialog::Accepted) {
        int brightness = brightnessSpinBox->value();
        int contrast = contrastSpinBox->value();

        QImage image = layerPixmap.toImage();
        if (image.isNull()) return;

        // Apply brightness and contrast adjustments
        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                QColor pixel = image.pixelColor(x, y);

                // Adjust brightness
                int r = pixel.red() + brightness;
                int g = pixel.green() + brightness;
                int b = pixel.blue() + brightness;

                // Apply contrast
                double factor = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
                r = qBound(0, (int)(factor * (r - 128) + 128), 255);
                g = qBound(0, (int)(factor * (g - 128) + 128), 255);
                b = qBound(0, (int)(factor * (b - 128) + 128), 255);

                image.setPixelColor(x, y, QColor(r, g, b, pixel.alpha()));
            }
        }

        currentLayer.pixmap() = QPixmap::fromImage(image);

        // Update the canvas
        canvas->compositeAllLayers();
        canvas->update();
        canvas->setModified(true);
    }
}

void MainWindow::posterize()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    if (layerPixmap.isNull()) {
        QMessageBox::warning(this, tr("Posterize"), tr("No image on current layer."));
        return;
    }

    // Create dialog
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Posterize"));

    QFormLayout *formLayout = new QFormLayout;

    // Levels spinner
    QSpinBox *levelsSpinBox = new QSpinBox(&dialog);
    levelsSpinBox->setRange(2, 16);
    levelsSpinBox->setValue(4);
    formLayout->addRow(tr("Levels:"), levelsSpinBox);

    QLabel *helpLabel = new QLabel(tr("Reduces colors to create a bold graphic look"), &dialog);
    helpLabel->setWordWrap(true);
    formLayout->addRow(helpLabel);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    dialog.setLayout(mainLayout);

    // Show dialog and process
    if (dialog.exec() == QDialog::Accepted) {
        int levels = levelsSpinBox->value();

        QImage image = layerPixmap.toImage();
        if (image.isNull()) return;

        // Posterize the image
        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                QColor pixel = image.pixelColor(x, y);

                // Reduce each channel to the specified number of levels
                int r = (pixel.red() * levels / 256) * (255 / (levels - 1));
                int g = (pixel.green() * levels / 256) * (255 / (levels - 1));
                int b = (pixel.blue() * levels / 256) * (255 / (levels - 1));

                r = qBound(0, r, 255);
                g = qBound(0, g, 255);
                b = qBound(0, b, 255);

                image.setPixelColor(x, y, QColor(r, g, b, pixel.alpha()));
            }
        }

        currentLayer.pixmap() = QPixmap::fromImage(image);

        // Update the canvas
        canvas->compositeAllLayers();
        canvas->update();
        canvas->setModified(true);
    }
}

void MainWindow::halftone()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    if (layerPixmap.isNull()) {
        QMessageBox::warning(this, tr("Halftone"), tr("No image on current layer."));
        return;
    }

    // Create dialog
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Halftone/Dither"));

    QFormLayout *formLayout = new QFormLayout;

    // Pattern type
    QComboBox *patternCombo = new QComboBox(&dialog);
    patternCombo->addItem(tr("Dots (Halftone)"), 0);
    patternCombo->addItem(tr("Dither (Floyd-Steinberg)"), 1);
    formLayout->addRow(tr("Pattern:"), patternCombo);

    // Dot size (for halftone)
    QSpinBox *dotSizeSpinBox = new QSpinBox(&dialog);
    dotSizeSpinBox->setRange(2, 20);
    dotSizeSpinBox->setValue(4);
    dotSizeSpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("Dot Size:"), dotSizeSpinBox);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    dialog.setLayout(mainLayout);

    // Show dialog and process
    if (dialog.exec() == QDialog::Accepted) {
        int patternType = patternCombo->currentData().toInt();
        int dotSize = dotSizeSpinBox->value();

        QImage image = layerPixmap.toImage();
        if (image.isNull()) return;

        if (patternType == 0) {
            // Halftone dots effect
            QImage result(image.width(), image.height(), QImage::Format_ARGB32);
            result.fill(Qt::white);

            for (int y = 0; y < image.height(); y += dotSize) {
                for (int x = 0; x < image.width(); x += dotSize) {
                    // Sample the center pixel of this cell
                    int sampleX = qMin(x + dotSize / 2, image.width() - 1);
                    int sampleY = qMin(y + dotSize / 2, image.height() - 1);
                    QColor pixel = image.pixelColor(sampleX, sampleY);

                    // Convert to grayscale
                    int gray = qGray(pixel.red(), pixel.green(), pixel.blue());

                    // Calculate dot radius based on darkness
                    double radius = (dotSize / 2.0) * (1.0 - gray / 255.0);

                    // Draw the dot
                    QPainter painter(&result);
                    painter.setRenderHint(QPainter::Antialiasing);
                    painter.setBrush(Qt::black);
                    painter.setPen(Qt::NoPen);
                    painter.drawEllipse(QPointF(x + dotSize / 2.0, y + dotSize / 2.0), radius, radius);
                }
            }

            currentLayer.pixmap() = QPixmap::fromImage(result);
        } else {
            // Floyd-Steinberg dithering
            image = image.convertToFormat(QImage::Format_RGB32);

            for (int y = 0; y < image.height(); ++y) {
                for (int x = 0; x < image.width(); ++x) {
                    QColor oldPixel = image.pixelColor(x, y);
                    int oldGray = qGray(oldPixel.red(), oldPixel.green(), oldPixel.blue());
                    int newGray = oldGray < 128 ? 0 : 255;

                    image.setPixelColor(x, y, QColor(newGray, newGray, newGray));

                    int error = oldGray - newGray;

                    // Distribute error to neighboring pixels
                    if (x + 1 < image.width()) {
                        QColor pixel = image.pixelColor(x + 1, y);
                        int gray = qGray(pixel.red(), pixel.green(), pixel.blue());
                        gray = qBound(0, gray + error * 7 / 16, 255);
                        image.setPixelColor(x + 1, y, QColor(gray, gray, gray));
                    }
                    if (y + 1 < image.height()) {
                        if (x > 0) {
                            QColor pixel = image.pixelColor(x - 1, y + 1);
                            int gray = qGray(pixel.red(), pixel.green(), pixel.blue());
                            gray = qBound(0, gray + error * 3 / 16, 255);
                            image.setPixelColor(x - 1, y + 1, QColor(gray, gray, gray));
                        }
                        QColor pixel = image.pixelColor(x, y + 1);
                        int gray = qGray(pixel.red(), pixel.green(), pixel.blue());
                        gray = qBound(0, gray + error * 5 / 16, 255);
                        image.setPixelColor(x, y + 1, QColor(gray, gray, gray));

                        if (x + 1 < image.width()) {
                            QColor pixel = image.pixelColor(x + 1, y + 1);
                            int gray = qGray(pixel.red(), pixel.green(), pixel.blue());
                            gray = qBound(0, gray + error * 1 / 16, 255);
                            image.setPixelColor(x + 1, y + 1, QColor(gray, gray, gray));
                        }
                    }
                }
            }

            currentLayer.pixmap() = QPixmap::fromImage(image);
        }

        // Update the canvas
        canvas->compositeAllLayers();
        canvas->update();
        canvas->setModified(true);
    }
}

void MainWindow::edgeDetect()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    if (layerPixmap.isNull()) {
        QMessageBox::warning(this, tr("Edge Detect"), tr("No image on current layer."));
        return;
    }

    QImage image = layerPixmap.toImage();
    if (image.isNull()) return;

    // Convert to grayscale first
    image = image.convertToFormat(QImage::Format_Grayscale8);

    QImage result(image.width(), image.height(), QImage::Format_Grayscale8);
    result.fill(Qt::white);

    // Sobel edge detection
    int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int y = 1; y < image.height() - 1; ++y) {
        for (int x = 1; x < image.width() - 1; ++x) {
            int gx = 0;
            int gy = 0;

            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int pixel = qGray(image.pixelColor(x + kx, y + ky).rgb());
                    gx += pixel * sobelX[ky + 1][kx + 1];
                    gy += pixel * sobelY[ky + 1][kx + 1];
                }
            }

            int magnitude = qMin(255, (int)sqrt(gx * gx + gy * gy));
            result.setPixelColor(x, y, QColor(255 - magnitude, 255 - magnitude, 255 - magnitude));
        }
    }

    currentLayer.pixmap() = QPixmap::fromImage(result);

    // Update the canvas
    canvas->compositeAllLayers();
    canvas->update();
    canvas->setModified(true);
}

void MainWindow::invertColors()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    if (layerPixmap.isNull()) {
        QMessageBox::warning(this, tr("Invert Colors"), tr("No image on current layer."));
        return;
    }

    QImage image = layerPixmap.toImage();
    if (image.isNull()) return;

    // Invert all colors
    image.invertPixels();

    currentLayer.pixmap() = QPixmap::fromImage(image);

    // Update the canvas
    canvas->compositeAllLayers();
    canvas->update();
    canvas->setModified(true);
}

void MainWindow::despeckle()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    if (layerPixmap.isNull()) {
        QMessageBox::warning(this, tr("Despeckle"), tr("No image on current layer."));
        return;
    }

    // Create dialog
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Despeckle/Cleanup"));

    QFormLayout *formLayout = new QFormLayout;

    // Radius spinner
    QSpinBox *radiusSpinBox = new QSpinBox(&dialog);
    radiusSpinBox->setRange(1, 5);
    radiusSpinBox->setValue(1);
    radiusSpinBox->setSuffix(tr(" px"));
    formLayout->addRow(tr("Radius:"), radiusSpinBox);

    QLabel *helpLabel = new QLabel(tr("Removes small specks and artifacts from scanned images"), &dialog);
    helpLabel->setWordWrap(true);
    formLayout->addRow(helpLabel);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    dialog.setLayout(mainLayout);

    // Show dialog and process
    if (dialog.exec() == QDialog::Accepted) {
        int radius = radiusSpinBox->value();

        QImage image = layerPixmap.toImage();
        if (image.isNull()) return;

        QImage result = image.copy();

        // Apply median filter to remove noise
        for (int y = radius; y < image.height() - radius; ++y) {
            for (int x = radius; x < image.width() - radius; ++x) {
                QVector<int> reds, greens, blues;

                // Collect neighboring pixels
                for (int ky = -radius; ky <= radius; ++ky) {
                    for (int kx = -radius; kx <= radius; ++kx) {
                        QColor pixel = image.pixelColor(x + kx, y + ky);
                        reds.append(pixel.red());
                        greens.append(pixel.green());
                        blues.append(pixel.blue());
                    }
                }

                // Sort and take median
                std::sort(reds.begin(), reds.end());
                std::sort(greens.begin(), greens.end());
                std::sort(blues.begin(), blues.end());

                int medianIndex = reds.size() / 2;
                QColor originalPixel = image.pixelColor(x, y);
                result.setPixelColor(x, y, QColor(reds[medianIndex], greens[medianIndex], blues[medianIndex], originalPixel.alpha()));
            }
        }

        currentLayer.pixmap() = QPixmap::fromImage(result);

        // Update the canvas
        canvas->compositeAllLayers();
        canvas->update();
        canvas->setModified(true);
    }
}

void MainWindow::autoLevels()
{
    Canvas *canvas = getCurrentCanvas();
    if (!canvas) return;

    Layer &currentLayer = canvas->currentLayer();
    QPixmap layerPixmap = currentLayer.pixmap();

    if (layerPixmap.isNull()) {
        QMessageBox::warning(this, tr("Auto Levels"), tr("No image on current layer."));
        return;
    }

    QImage image = layerPixmap.toImage();
    if (image.isNull()) return;

    // Find min and max values for each channel
    int minR = 255, maxR = 0;
    int minG = 255, maxG = 0;
    int minB = 255, maxB = 0;

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor pixel = image.pixelColor(x, y);
            minR = qMin(minR, pixel.red());
            maxR = qMax(maxR, pixel.red());
            minG = qMin(minG, pixel.green());
            maxG = qMax(maxG, pixel.green());
            minB = qMin(minB, pixel.blue());
            maxB = qMax(maxB, pixel.blue());
        }
    }

    // Stretch levels to full range
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor pixel = image.pixelColor(x, y);

            int r = maxR > minR ? (pixel.red() - minR) * 255 / (maxR - minR) : pixel.red();
            int g = maxG > minG ? (pixel.green() - minG) * 255 / (maxG - minG) : pixel.green();
            int b = maxB > minB ? (pixel.blue() - minB) * 255 / (maxB - minB) : pixel.blue();

            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);

            image.setPixelColor(x, y, QColor(r, g, b, pixel.alpha()));
        }
    }

    currentLayer.pixmap() = QPixmap::fromImage(image);

    // Update the canvas
    canvas->compositeAllLayers();
    canvas->update();
    canvas->setModified(true);
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