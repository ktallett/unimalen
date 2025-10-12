#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QColor>
#include <QList>
#include <QScrollArea>

class ColorBar : public QWidget
{
    Q_OBJECT

public:
    // Curated pastel color palettes
    enum PaletteType {
        CopicPastels,      // Copic marker pastel colors
        PantonePastels,    // Pantone pastel colors
        CustomPastels      // Custom pastel selection
    };

    explicit ColorBar(QWidget *parent = nullptr, PaletteType palette = CopicPastels);

    QColor currentColor() const { return m_currentColor; }
    void setCurrentColor(const QColor &color);

signals:
    void colorSelected(const QColor &color);

private slots:
    void onColorButtonClicked();

private:
    void createPalette(PaletteType palette);
    void addColorButton(const QColor &color, const QString &name);
    QList<QPair<QColor, QString>> getCopicPastelColors();
    QList<QPair<QColor, QString>> getPantonePastelColors();
    QList<QPair<QColor, QString>> getCustomPastelColors();

    QGridLayout *m_layout;
    QColor m_currentColor;
    QPushButton *m_selectedButton;
    QList<QPushButton*> m_colorButtons;
};
