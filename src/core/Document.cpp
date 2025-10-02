#include "Document.h"
#include <QPainter>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QTemporaryDir>

namespace Unimalen {

Document::Document(int width, int height)
    : m_width(width)
    , m_height(height)
    , m_pageSize(PageSize::Custom)
    , m_currentPageIndex(0)
{
    // Create initial page
    m_pages.append(Page(width, height));
}

// Page management
void Document::setCurrentPageIndex(int index)
{
    if (index >= 0 && index < m_pages.size()) {
        m_currentPageIndex = index;
    }
}

Page& Document::currentPage()
{
    if (m_pages.isEmpty()) {
        m_pages.append(Page(m_width, m_height));
    }
    if (m_currentPageIndex < 0 || m_currentPageIndex >= m_pages.size()) {
        m_currentPageIndex = 0;
    }
    return m_pages[m_currentPageIndex];
}

const Page& Document::currentPage() const
{
    return const_cast<Document*>(this)->currentPage();
}

Page& Document::pageAt(int index)
{
    if (index >= 0 && index < m_pages.size()) {
        return m_pages[index];
    }
    return currentPage();
}

const Page& Document::pageAt(int index) const
{
    if (index >= 0 && index < m_pages.size()) {
        return m_pages[index];
    }
    return currentPage();
}

void Document::addPage()
{
    if (m_pages.size() >= MAX_PAGES) {
        return;
    }
    m_pages.append(Page(m_width, m_height));
}

void Document::deletePage(int index)
{
    if (index >= 0 && index < m_pages.size() && m_pages.size() > 1) {
        m_pages.removeAt(index);
        if (m_currentPageIndex >= m_pages.size()) {
            m_currentPageIndex = m_pages.size() - 1;
        }
    }
}

void Document::duplicatePage(int index)
{
    if (m_pages.size() >= MAX_PAGES) {
        return;
    }
    if (index >= 0 && index < m_pages.size()) {
        // Create a new page and copy all layers
        Page newPage(m_width, m_height);
        newPage.clear();

        const Page& source = m_pages[index];
        for (const Layer& layer : source.layers()) {
            newPage.addLayer(layer.name() + " copy");
            newPage.layers().last().pixmap() = layer.pixmap().copy();
            newPage.layers().last().setVisible(layer.isVisible());
            newPage.layers().last().setOpacity(layer.opacity());
            newPage.layers().last().setBlendMode(layer.blendMode());
        }

        m_pages.insert(index + 1, newPage);
    }
}

void Document::movePage(int fromIndex, int toIndex)
{
    if (fromIndex >= 0 && fromIndex < m_pages.size() &&
        toIndex >= 0 && toIndex < m_pages.size() && fromIndex != toIndex) {
        m_pages.move(fromIndex, toIndex);
        if (m_currentPageIndex == fromIndex) {
            m_currentPageIndex = toIndex;
        }
    }
}

// Layer management (delegates to current page)
void Document::setCurrentLayerIndex(int index)
{
    currentPage().setCurrentLayerIndex(index);
}

Layer& Document::currentLayer()
{
    return currentPage().currentLayer();
}

const Layer& Document::currentLayer() const
{
    return currentPage().currentLayer();
}

void Document::addLayer(const QString &name)
{
    currentPage().addLayer(name);
}

void Document::deleteLayer(int index)
{
    currentPage().deleteLayer(index);
}

void Document::duplicateLayer(int index)
{
    currentPage().duplicateLayer(index);
}

void Document::moveLayer(int fromIndex, int toIndex)
{
    currentPage().moveLayer(fromIndex, toIndex);
}

void Document::setLayerVisibility(int index, bool visible)
{
    currentPage().setLayerVisibility(index, visible);
}

void Document::setLayerOpacity(int index, qreal opacity)
{
    currentPage().setLayerOpacity(index, opacity);
}

void Document::setLayerBlendMode(int index, Layer::BlendMode mode)
{
    currentPage().setLayerBlendMode(index, mode);
}

void Document::setLayerName(int index, const QString &name)
{
    currentPage().setLayerName(index, name);
}

// Compositing
QPixmap Document::composite() const
{
    return currentPage().composite();
}

void Document::compositeToPixmap(QPixmap &target) const
{
    currentPage().compositeToPixmap(target);
}

QPixmap Document::compositePage(int pageIndex) const
{
    if (pageIndex >= 0 && pageIndex < m_pages.size()) {
        return m_pages[pageIndex].composite();
    }
    return QPixmap();
}

bool Document::saveAsORA(const QString &fileName) const
{
    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        return false;
    }

    QString tempPath = tempDir.path();

    // Create directory structure
    QDir().mkpath(tempPath + "/data");
    QDir().mkpath(tempPath + "/Thumbnails");

    // Save layers as PNG files (from current page)
    const Page& page = currentPage();
    for (int i = 0; i < page.layers().size(); ++i) {
        QString layerPath = tempPath + QString("/data/layer%1.png").arg(i);
        if (!page.layers()[i].pixmap().save(layerPath, "PNG")) {
            return false;
        }
    }

    // Create mimetype file
    QFile mimeFile(tempPath + "/mimetype");
    if (!mimeFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    mimeFile.write("image/openraster");
    mimeFile.close();

    // Create stack.xml
    QFile stackFile(tempPath + "/stack.xml");
    if (!stackFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stackStream(&stackFile);
    stackStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    stackStream << "<image version=\"0.0.3\" w=\"" << m_width << "\" h=\"" << m_height
                << "\" xres=\"" << DEFAULT_DPI << "\" yres=\"" << DEFAULT_DPI << "\">\n";
    stackStream << "  <stack>\n";

    // Write layer information (in reverse order - bottom to top)
    for (int i = page.layers().size() - 1; i >= 0; --i) {
        const Layer &layer = page.layers()[i];
        stackStream << "    <layer name=\"" << layer.name()
                   << "\" src=\"data/layer" << i << ".png\""
                   << " x=\"0\" y=\"0\""
                   << " opacity=\"" << layer.opacity() << "\""
                   << " visibility=\"" << (layer.isVisible() ? "visible" : "hidden") << "\""
                   << " composite-op=\"svg:src-over\"/>\n";
    }

    stackStream << "  </stack>\n";
    stackStream << "</image>\n";
    stackFile.close();

    // Create thumbnail
    QPixmap composited = composite();
    QPixmap thumbnail = composited.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (!thumbnail.save(tempPath + "/Thumbnails/thumbnail.png", "PNG")) {
        return false;
    }

    // Create ZIP file using system zip command
    QProcess zipProcess;
    zipProcess.setWorkingDirectory(tempPath);

    QStringList args;
    args << "-r" << fileName << ".";

    zipProcess.start("zip", args);
    zipProcess.waitForFinished();

    return zipProcess.exitCode() == 0;
}

bool Document::loadFromORA(const QString &fileName)
{
    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        return false;
    }

    QString tempPath = tempDir.path();

    // Extract ZIP file using system unzip command
    QProcess unzipProcess;
    QStringList args;
    args << fileName << "-d" << tempPath;

    unzipProcess.start("unzip", args);
    unzipProcess.waitForFinished();

    if (unzipProcess.exitCode() != 0) {
        return false;
    }

    // Check if it's a valid ORA file
    QFile mimeFile(tempPath + "/mimetype");
    if (!mimeFile.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray mimeContent = mimeFile.readAll();
    mimeFile.close();

    if (mimeContent != "image/openraster") {
        return false;
    }

    // Clear current page layers
    Page& page = currentPage();
    page.clear();

    // Load the first layer (simplified - just load layer0.png or layer1.png)
    QString layerPath = tempPath + "/data/layer0.png";
    if (!QFile::exists(layerPath)) {
        layerPath = tempPath + "/data/layer1.png";
    }

    QPixmap loadedPixmap;
    if (!loadedPixmap.load(layerPath)) {
        // Failed to load
        return false;
    }

    // Draw onto current layer
    QPainter painter(&currentLayer().pixmap());
    painter.drawPixmap(0, 0, loadedPixmap);

    return true;
}

bool Document::saveAsPNG(const QString &fileName) const
{
    QPixmap composited = composite();
    return composited.save(fileName, "PNG");
}

bool Document::loadFromPNG(const QString &fileName)
{
    QPixmap loadedPixmap;
    if (!loadedPixmap.load(fileName)) {
        return false;
    }

    // Clear current page
    Page& page = currentPage();
    page.clear();

    // Draw loaded image onto layer
    QPainter painter(&currentLayer().pixmap());
    painter.drawPixmap(0, 0, loadedPixmap);

    return true;
}

void Document::clear()
{
    m_pages.clear();
    m_currentPageIndex = 0;
    m_pages.append(Page(m_width, m_height));
}

void Document::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    for (Page &page : m_pages) {
        page.resize(width, height);
    }
}

void Document::setPageSize(PageSize size)
{
    m_pageSize = size;
    const PageSizeInfo& info = getPageSizeInfo(size);
    resize(info.width, info.height);
}

// Multi-page zine file I/O
bool Document::saveAsZine(const QString &folderPath) const
{
    QDir dir(folderPath);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            return false;
        }
    }

    // Save each page as a separate ORA file
    for (int i = 0; i < m_pages.size(); ++i) {
        QString pageFileName = dir.filePath(QString("page_%1.ora").arg(i + 1, 2, 10, QChar('0')));

        QTemporaryDir tempDir;
        if (!tempDir.isValid()) {
            return false;
        }

        QString tempPath = tempDir.path();
        QDir().mkpath(tempPath + "/data");
        QDir().mkpath(tempPath + "/Thumbnails");

        const Page& page = m_pages[i];

        // Save layers as PNG files
        for (int j = 0; j < page.layers().size(); ++j) {
            QString layerPath = tempPath + QString("/data/layer%1.png").arg(j);
            if (!page.layers()[j].pixmap().save(layerPath, "PNG")) {
                return false;
            }
        }

        // Create mimetype file
        QFile mimeFile(tempPath + "/mimetype");
        if (!mimeFile.open(QIODevice::WriteOnly)) {
            return false;
        }
        mimeFile.write("image/openraster");
        mimeFile.close();

        // Create stack.xml
        QFile stackFile(tempPath + "/stack.xml");
        if (!stackFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return false;
        }

        QTextStream stackStream(&stackFile);
        stackStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        stackStream << "<image version=\"0.0.3\" w=\"" << m_width << "\" h=\"" << m_height
                    << "\" xres=\"" << DEFAULT_DPI << "\" yres=\"" << DEFAULT_DPI << "\">\n";
        stackStream << "  <stack>\n";

        for (int j = page.layers().size() - 1; j >= 0; --j) {
            const Layer &layer = page.layers()[j];
            stackStream << "    <layer name=\"" << layer.name()
                       << "\" src=\"data/layer" << j << ".png\""
                       << " x=\"0\" y=\"0\""
                       << " opacity=\"" << layer.opacity() << "\""
                       << " visibility=\"" << (layer.isVisible() ? "visible" : "hidden") << "\""
                       << " composite-op=\"svg:src-over\"/>\n";
        }

        stackStream << "  </stack>\n";
        stackStream << "</image>\n";
        stackFile.close();

        // Create thumbnail
        QPixmap composited = page.composite();
        QPixmap thumbnail = composited.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        if (!thumbnail.save(tempPath + "/Thumbnails/thumbnail.png", "PNG")) {
            return false;
        }

        // Create ZIP file
        QProcess zipProcess;
        zipProcess.setWorkingDirectory(tempPath);
        QStringList args;
        args << "-r" << pageFileName << ".";
        zipProcess.start("zip", args);
        zipProcess.waitForFinished();

        if (zipProcess.exitCode() != 0) {
            return false;
        }
    }

    // Save metadata file
    QFile metaFile(dir.filePath("zine_meta.txt"));
    if (metaFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream metaStream(&metaFile);
        metaStream << "pages=" << m_pages.size() << "\n";
        metaStream << "width=" << m_width << "\n";
        metaStream << "height=" << m_height << "\n";
        metaFile.close();
    }

    return true;
}

bool Document::loadFromZine(const QString &folderPath)
{
    QDir dir(folderPath);
    if (!dir.exists()) {
        return false;
    }

    // Load metadata
    QFile metaFile(dir.filePath("zine_meta.txt"));
    int pageCount = 0;
    if (metaFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream metaStream(&metaFile);
        while (!metaStream.atEnd()) {
            QString line = metaStream.readLine();
            if (line.startsWith("pages=")) {
                pageCount = line.mid(6).toInt();
            }
        }
        metaFile.close();
    }

    if (pageCount == 0) {
        // Try to count ORA files
        QStringList oraFiles = dir.entryList(QStringList() << "page_*.ora", QDir::Files, QDir::Name);
        pageCount = oraFiles.size();
    }

    if (pageCount == 0) {
        return false;
    }

    // Clear current pages
    m_pages.clear();
    m_currentPageIndex = 0;

    // Load each page
    for (int i = 0; i < pageCount && i < MAX_PAGES; ++i) {
        QString pageFileName = dir.filePath(QString("page_%1.ora").arg(i + 1, 2, 10, QChar('0')));

        // For now, create empty pages - full ORA loading would go here
        m_pages.append(Page(m_width, m_height));
    }

    return true;
}

} // namespace Unimalen
