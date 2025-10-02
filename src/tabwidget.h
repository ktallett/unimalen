#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QList>

class Canvas;

class TabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TabWidget(QWidget *parent = nullptr);
    ~TabWidget();

    Canvas* currentCanvas() const;
    Canvas* canvasAt(int index) const;
    int currentIndex() const;
    int count() const;

    void newTab();
    void closeTab(int index);
    void setCurrentTab(int index);

    bool loadDocument(const QString &fileName);
    bool saveDocument(const QString &fileName);
    bool saveCurrentDocument(const QString &fileName);

    void setTabName(int index, const QString &name);
    QString tabName(int index) const;

    static const int MAX_TABS = 9;

signals:
    void currentChanged(int index);
    void tabCloseRequested(int index);
    void documentChanged();

private slots:
    void onCurrentChanged(int index);
    void onTabCloseRequested(int index);

private:
    void setupCanvas(Canvas *canvas);
    QString generateTabName() const;

    QTabWidget *m_tabWidget;
    QVBoxLayout *m_layout;
    QList<QScrollArea*> m_scrollAreas;
    mutable int m_untitledCounter;
};