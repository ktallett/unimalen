#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QBrush>
#include <QCloseEvent>

class PatternBar : public QWidget
{
    Q_OBJECT

public:
    explicit PatternBar(QWidget *parent = nullptr);
    void showAsFloatingWindow();

    enum PatternType {
        Solid = 0,
        Dense1,
        Dense2,
        Dense3,
        Dense4,
        Dense5,
        Dense6,
        Dense7,
        Horizontal,
        Vertical,
        Cross,
        BDiag,
        FDiag,
        DiagCross,
        // Custom textured patterns
        Dots,
        Grid,
        Circles,
        Waves,
        Stars,
        Brick,
        Hexagons,
        Scales,
        Zigzag,
        Checkerboard,
        Triangles,
        Noise
    };

signals:
    void patternSelected(PatternType pattern);
    void patternBarClosed();

private slots:
    void onSolidClicked();
    void onDense1Clicked();
    void onDense2Clicked();
    void onDense3Clicked();
    void onDense4Clicked();
    void onDense5Clicked();
    void onDense6Clicked();
    void onDense7Clicked();
    void onHorizontalClicked();
    void onVerticalClicked();
    void onCrossClicked();
    void onBDiagClicked();
    void onFDiagClicked();
    void onDiagCrossClicked();
    void onDotsClicked();
    void onGridClicked();
    void onCirclesClicked();
    void onWavesClicked();
    void onStarsClicked();
    void onBrickClicked();
    void onHexagonsClicked();
    void onScalesClicked();
    void onZigzagClicked();
    void onCheckerboardClicked();
    void onTrianglesClicked();
    void onNoiseClicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void createPatternButton(PatternType type, Qt::BrushStyle style, const QString &tooltip, int row, int col);
    void drawCustomPattern(QPainter &painter, PatternType type, int x, int y, int width, int height);

    QGridLayout *m_layout;
    QToolButton *m_solidButton;
    QToolButton *m_dense1Button;
    QToolButton *m_dense2Button;
    QToolButton *m_dense3Button;
    QToolButton *m_dense4Button;
    QToolButton *m_dense5Button;
    QToolButton *m_dense6Button;
    QToolButton *m_dense7Button;
    QToolButton *m_horizontalButton;
    QToolButton *m_verticalButton;
    QToolButton *m_crossButton;
    QToolButton *m_bdiagButton;
    QToolButton *m_fdiagButton;
    QToolButton *m_diagCrossButton;
    QToolButton *m_dotsButton;
    QToolButton *m_gridButton;
    QToolButton *m_circlesButton;
    QToolButton *m_wavesButton;
    QToolButton *m_starsButton;
    QToolButton *m_brickButton;
    QToolButton *m_hexagonsButton;
    QToolButton *m_scalesButton;
    QToolButton *m_zigzagButton;
    QToolButton *m_checkerboardButton;
    QToolButton *m_trianglesButton;
    QToolButton *m_noiseButton;

    PatternType m_currentPattern;
};