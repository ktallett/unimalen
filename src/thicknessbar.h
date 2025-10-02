#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QToolButton>
#include <QCloseEvent>

class ThicknessBar : public QWidget
{
    Q_OBJECT

public:
    enum ThicknessType {
        Dotted = 0,
        Thin = 1,
        Medium = 2,
        Thick = 3,
        ExtraThick = 4
    };

    explicit ThicknessBar(QWidget *parent = nullptr);
    void showAsFloatingWindow();

signals:
    void thicknessSelected(ThicknessBar::ThicknessType thickness);
    void thicknessBarClosed();

private slots:
    void onDottedClicked();
    void onThinClicked();
    void onMediumClicked();
    void onThickClicked();
    void onExtraThickClicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QHBoxLayout *m_layout;
    QToolButton *m_dottedButton;
    QToolButton *m_thinButton;
    QToolButton *m_mediumButton;
    QToolButton *m_thickButton;
    QToolButton *m_extraThickButton;
    ThicknessType m_currentThickness;

    void createThicknessButton(QToolButton **button, const QString &tooltip, ThicknessType type);
    QPixmap createThicknessIcon(ThicknessType type);
};