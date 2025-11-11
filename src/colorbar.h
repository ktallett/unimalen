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
    // Curated color palettes
    enum PaletteType {
        Pastels,           // Pastel colors for soft illustration
        Markers            // Professional marker colors
    };

    explicit ColorBar(QWidget *parent = nullptr, PaletteType palette = Pastels);

    QColor currentColor() const { return m_currentColor; }
    void setCurrentColor(const QColor &color);

signals:
    void colorSelected(const QColor &color);

private slots:
    void onColorButtonClicked();

private:
    void createPalette(PaletteType palette);
    void addColorButton(const QColor &color, const QString &name);
    QList<QPair<QColor, QString>> getPastelColors();
    QList<QPair<QColor, QString>> getMarkerColors();

    QGridLayout *m_layout;
    QColor m_currentColor;
    QPushButton *m_selectedButton;
    QList<QPushButton*> m_colorButtons;
};
