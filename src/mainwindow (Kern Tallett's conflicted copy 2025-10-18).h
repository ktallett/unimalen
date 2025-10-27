#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QActionGroup>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QLabel>
#include <QDockWidget>
#include "core/Types.h"

class Canvas;
class TabWidget;
class ToolBar;
class PatternBar;
class ThicknessBar;
class LayerPanel;
class ColorBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void insertImage();
    void convertToBlackAndWhite();
    void adjustBrightnessContrast();
    void posterize();
    void halftone();
    void edgeDetect();
    void invertColors();
    void despeckle();
    void autoLevels();
    void rotateClockwise();
    void rotateCounterClockwise();
    void rotate180();
    void flipHorizontal();
    void flipVertical();
    void scaleImage();
    void cropImage();
    void addDropShadow();
    void addBorder();
    void newTab();
    void closeTab();
    void setPageSize(Unimalen::PageSize size);
    void setScale1x();
    void setScale2x();
    void setScale4x();
    void togglePixelZoom(bool enabled);
    void toggleCoordinates(bool enabled);
    void onPencilSelected();
    void onTextSelected();
    void onSpraySelected(int diameter);
    void onBrushSelected(int diameter);
    void onEraserSelected(int diameter);
    void onLineSelected();
    void onBezierSelected();
    void onScissorsSelected();
    void onFillSelected();
    void onLassoSelected();
    void onSquareSelected();
    void onFilledSquareSelected();
    void onRoundedSquareSelected();
    void onFilledRoundedSquareSelected();
    void onOvalSelected();
    void onFilledOvalSelected();
    void onCut();
    void onCopy();
    void onPaste();
    void onFontChanged(const QString &fontFamily);
    void onFontSizeChanged(int fontSize);
    void onThicknessSelected(int thickness);
    void toggleToolBar(bool visible);
    void togglePatternBar(bool visible);
    void toggleThicknessBar(bool visible);
    void toggleLayerPanel(bool visible);
    void toggleColorBar(bool visible);
    void onTabChanged(int index);
    void switchToTab(int tabNumber);
    void checkForUpdates();
    void showAbout();

    // Layer management slots
    void onLayerAdded();
    void onLayerDeleted(int index);
    void onLayerDuplicated(int index);
    void onLayerSelected(int index);
    void onLayerVisibilityChanged(int index, bool visible);
    void onLayerOpacityChanged(int index, qreal opacity);
    void onLayerBlendModeChanged(int index, int mode);
    void onLayerRenamed(int index, const QString &name);
    void onLayersChanged();
    void onCurrentLayerChanged(int index);

    // Page management slots
    void onPageAdded();
    void onPageDeleted();
    void onPageDuplicated();
    void nextPage();
    void previousPage();
    void updatePageIndicator();

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

private:
    void createMenus();
    void createActions();
    void setCurrentFile(const QString &fileName);
    void connectCanvasSignals(Canvas *canvas);
    Canvas* getCurrentCanvas();

    TabWidget *m_tabWidget;
    ToolBar *m_toolBar;
    PatternBar *m_patternBar;
    ThicknessBar *m_thicknessBar;
    LayerPanel *m_layerPanel;
    ColorBar *m_colorBar;

    // Dock widgets
    QDockWidget *m_toolBarDock;
    QDockWidget *m_patternBarDock;
    QDockWidget *m_thicknessBarDock;
    QDockWidget *m_colorBarDock;
    QDockWidget *m_layerPanelDock;

    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_insertImageAction;
    QAction *m_convertToBlackAndWhiteAction;
    QAction *m_adjustBrightnessContrastAction;
    QAction *m_posterizeAction;
    QAction *m_halftoneAction;
    QAction *m_edgeDetectAction;
    QAction *m_invertColorsAction;
    QAction *m_despeckleAction;
    QAction *m_autoLevelsAction;
    QAction *m_rotateClockwiseAction;
    QAction *m_rotateCounterClockwiseAction;
    QAction *m_rotate180Action;
    QAction *m_flipHorizontalAction;
    QAction *m_flipVerticalAction;
    QAction *m_scaleImageAction;
    QAction *m_cropImageAction;
    QAction *m_addDropShadowAction;
    QAction *m_addBorderAction;
    QAction *m_exitAction;
    QAction *m_newTabAction;
    QAction *m_closeTabAction;

    // Edit menu actions
    QAction *m_undoAction;
    QAction *m_redoAction;
    QAction *m_cutAction;
    QAction *m_copyAction;
    QAction *m_pasteAction;

    QAction *m_scale1xAction;
    QAction *m_scale2xAction;
    QAction *m_scale4xAction;
    QAction *m_pixelZoomAction;
    QAction *m_showCoordinatesAction;
    QActionGroup *m_scaleGroup;

    // Toolbar visibility actions
    QAction *m_showToolBarAction;
    QAction *m_showPatternBarAction;
    QAction *m_showThicknessBarAction;
    QAction *m_showLayerPanelAction;
    QAction *m_showColorBarAction;

    // Font menu actions
    QAction *m_courierFontAction;
    QAction *m_openSansFontAction;
    QAction *m_serifFontAction;
    QAction *m_notoSansFontAction;
    QAction *m_notoSansCJKFontAction;
    QAction *m_dejavuSansFontAction;

    // Zine font actions
    QAction *m_specialEliteFontAction;      // Typewriter
    QAction *m_permanentMarkerFontAction;   // Hand marker
    QAction *m_bangersFontAction;           // Comic
    QAction *m_creepsterFontAction;         // Horror
    QAction *m_pressStart2PFontAction;      // Pixel/retro
    QAction *m_caveatFontAction;            // Handwriting
    QAction *m_indieFlowerFontAction;       // Handwriting
    QAction *m_vt323FontAction;             // Terminal/VT
    QAction *m_robotoMonoFontAction;        // Modern mono
    QAction *m_amaticFontAction;            // Casual handwriting
    QAction *m_satisfyFontAction;           // Script/cursive
    QActionGroup *m_fontGroup;

    // Font size menu actions
    QAction *m_fontSize8Action;
    QAction *m_fontSize12Action;
    QAction *m_fontSize18Action;
    QAction *m_fontSize24Action;
    QAction *m_fontSize32Action;
    QAction *m_fontSize64Action;
    QActionGroup *m_fontSizeGroup;

    // Layer menu actions
    QAction *m_addLayerAction;
    QAction *m_deleteLayerAction;
    QAction *m_duplicateLayerAction;

    // Page menu actions
    QAction *m_addPageAction;
    QAction *m_deletePageAction;
    QAction *m_duplicatePageAction;
    QAction *m_nextPageAction;
    QAction *m_previousPageAction;

    // Page size menu actions
    QActionGroup *m_pageSizeGroup;
    QList<QAction*> m_pageSizeActions;

    // Help menu actions
    QAction *m_checkUpdatesAction;
    QAction *m_aboutAction;

    // Page indicator label
    QLabel *m_pageIndicator;

    QString m_currentFile;
    QList<QAction*> m_tabShortcuts;
};