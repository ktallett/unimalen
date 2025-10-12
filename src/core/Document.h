#pragma once

#include "Layer.h"
#include "Page.h"
#include "Types.h"
#include <QPixmap>
#include <QList>
#include <QString>

namespace Unimalen {

constexpr int MAX_PAGES = 24;

class Document
{
public:
    explicit Document(int width = DEFAULT_CANVAS_WIDTH,
                     int height = DEFAULT_CANVAS_HEIGHT);
    ~Document() = default;

    // Document properties
    int width() const { return m_width; }
    int height() const { return m_height; }
    QSize size() const { return QSize(m_width, m_height); }
    PageSize pageSize() const { return m_pageSize; }
    void setPageSize(PageSize size);
    PaperColor paperColor() const { return m_paperColor; }
    void setPaperColor(PaperColor color);

    // Page management
    int pageCount() const { return m_pages.size(); }
    int currentPageIndex() const { return m_currentPageIndex; }
    void setCurrentPageIndex(int index);
    Page& currentPage();
    const Page& currentPage() const;
    Page& pageAt(int index);
    const Page& pageAt(int index) const;

    void addPage();
    void deletePage(int index);
    void duplicatePage(int index);
    void movePage(int fromIndex, int toIndex);

    // Layer management (delegates to current page)
    QList<Layer>& layers() { return currentPage().layers(); }
    const QList<Layer>& layers() const { return currentPage().layers(); }

    int currentLayerIndex() const { return currentPage().currentLayerIndex(); }
    void setCurrentLayerIndex(int index);

    Layer& currentLayer();
    const Layer& currentLayer() const;

    void addLayer(const QString &name = QString());
    void deleteLayer(int index);
    void duplicateLayer(int index);
    void moveLayer(int fromIndex, int toIndex);
    void setLayerVisibility(int index, bool visible);
    void setLayerOpacity(int index, qreal opacity);
    void setLayerBlendMode(int index, Layer::BlendMode mode);
    void setLayerName(int index, const QString &name);

    // Compositing
    QPixmap composite() const; // Composite current page
    void compositeToPixmap(QPixmap &target) const;
    QPixmap compositePage(int pageIndex) const;

    // File I/O
    bool saveAsORA(const QString &fileName) const;
    bool loadFromORA(const QString &fileName);

    bool saveAsPNG(const QString &fileName) const;
    bool loadFromPNG(const QString &fileName);

    // Multi-page file I/O
    bool saveAsZine(const QString &folderPath) const;
    bool loadFromZine(const QString &folderPath);

    // Document state
    void clear();
    void resize(int width, int height);

private:
    int m_width;
    int m_height;
    PageSize m_pageSize;
    PaperColor m_paperColor;
    QList<Page> m_pages;
    int m_currentPageIndex;
};

} // namespace Unimalen
