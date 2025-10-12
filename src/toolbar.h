#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QComboBox>
#include <QLabel>
#include <QCloseEvent>
#include <QScrollArea>

class ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);
    void showAsFloatingWindow();

signals:
    void pencilToolSelected();
    void textToolSelected();
    void sprayToolSelected(int diameter);
    void brushToolSelected(int diameter);
    void eraserToolSelected(int diameter);
    void lineToolSelected();
    void bezierToolSelected();
    void scissorsToolSelected();
    void fillToolSelected();
    void lassoToolSelected();
    void squareToolSelected();
    void filledSquareToolSelected();
    void roundedSquareToolSelected();
    void filledRoundedSquareToolSelected();
    void ovalToolSelected();
    void filledOvalToolSelected();
    void toolBarClosed();

private slots:
    void onPencilClicked();
    void onTextClicked();
    void onSprayClicked();
    void onBrushClicked();
    void onEraserClicked();
    void onLineClicked();
    void onBezierClicked();
    void onScissorsClicked();
    void onFillClicked();
    void onLassoClicked();
    void onSquareClicked();
    void onFilledSquareClicked();
    void onRoundedSquareClicked();
    void onFilledRoundedSquareClicked();
    void onOvalClicked();
    void onFilledOvalClicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QGridLayout *m_layout;
    QToolButton *m_pencilButton;
    QToolButton *m_textButton;
    QToolButton *m_sprayButton;
    QToolButton *m_brushButton;
    QToolButton *m_eraserButton;
    QToolButton *m_lineButton;
    QToolButton *m_bezierButton;
    QToolButton *m_scissorsButton;
    QToolButton *m_fillButton;
    QToolButton *m_lassoButton;
    QToolButton *m_squareButton;
    QToolButton *m_filledSquareButton;
    QToolButton *m_roundedSquareButton;
    QToolButton *m_filledRoundedSquareButton;
    QToolButton *m_ovalButton;
    QToolButton *m_filledOvalButton;
    int m_currentSpraySize;
    int m_currentBrushSize;
    int m_currentEraserSize;
};