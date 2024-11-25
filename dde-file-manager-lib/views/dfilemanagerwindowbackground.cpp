#include "dfilemanagerwindowbackground.h"
#include <QFile>
#include <DThemeManager>
#include <QDebug>

DWIDGET_USE_NAMESPACE

DFileManagerWindowBackground::DFileManagerWindowBackground(DMainWindow *window)
{
    setMainWindow(window);
    refresh();
}

void DFileManagerWindowBackground::drawInWidget(QPainter *painter)
{
    QList<BackgroundPlace> list = {
        BackgroundPlace::FullWindow,
        BackgroundPlace::Center,
        BackgroundPlace::TopCenter,
        BackgroundPlace::BottomCenter,
        BackgroundPlace::CenterLeft,
        BackgroundPlace::CenterRight,
        BackgroundPlace::TopLeft,
        BackgroundPlace::TopRight,
        BackgroundPlace::BottomLeft,
        BackgroundPlace::BottomRight
    };
    for (BackgroundPlace i: list) {
        QList<int> xy = getImageXY(DFileManagerWindowBackground::BackgroundPlace(i));
        int x = xy[0];
        int y = xy[1];
        qDebug() << i << x << y;
        painter->drawImage(x, y, getImage(DFileManagerWindowBackground::BackgroundPlace(i)));
    }
}

void DFileManagerWindowBackground::setMainWindow(DMainWindow *window)
{
    m_dmainWindow = window;
}

QImage DFileManagerWindowBackground::getImage(BackgroundPlace place)
{
    if (place <= 8) {
        return m_imageVar[place];
    }
    return m_imageVar[place].scaled(m_dmainWindow->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
}

QList<int> DFileManagerWindowBackground::getImageSize(BackgroundPlace place)
{
    QList<int> size;
    QImage image = getImage(place);
    size << image.size().width() << image.height();
    return size;
    /*if (place <= 8) {
        // 如果为不需要拉伸的，则直接返回原尺寸
        size << image.size().width() << image.height();
        return size;
    }
    // 如果需要拉伸，则需要计算尺寸
    int windowWidth = m_dmainWindow->width();
    int windowHeight = m_dmainWindow->height();
    int oldImageWidth = image.width();
    int oldImageHeight = image.height();
    image = image.scaled(m_dmainWindow->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    size << image.size().width() << image.height();
    return size;*/
}

QList<int> DFileManagerWindowBackground::getImageXY(BackgroundPlace place)
{
    QList<int> xy;
    QList<int> image = getImageSize(place);
    int imageWidth = image[0];
    int imageHeight = image[1];
    int windowWidth = 0;
    int windowHeight = 0;
    if (m_dmainWindow) {
        windowWidth = m_dmainWindow->width();
        windowHeight = m_dmainWindow->height();
    }
    int x = 0, y = 0;

    //// 确定 X 轴
    // 如果为 Left（0,3,6）,则可以直接确定 X = 0
    if (place % 3 == 0) {
        x = 0;
    }
    // 如果为 Center（1,4,7）,则可以直接确定 X = (width - image.width) / 2
    if (place % 3 == 1 || place == 9) {
        x = (windowWidth - imageWidth) / 2;
    }
    // 如果为 Bottom（2,5,8）,则可以直接确定 X = width - image.width
    if (place % 3 == 2) {
        x = windowWidth - imageWidth;
    }
    //// 确定 Y 轴
    // 如果为 Top（0<=place<=2）,则可以直接确定 Y = 0
    if (place >= 0 && place <= 2) {
        y = 0;
    }
    // 如果为 Center（3<=place<=5）,则可以直接确定 Y = (height - image.height) / 2
    if ((place >= 3 && place <= 5) || place == 9) {
        y = (windowHeight - imageHeight) / 2;
    }
    // 如果为 Bottom（6<=place<=8）,则可以直接确定 Y = height - image.width
    if (place >= 6 && place <= 8) {
        y = windowHeight - imageHeight;
    }
    xy << x << y;
    return xy;
}

void DFileManagerWindowBackground::refresh()
{
    QString theme = DThemeManager::instance()->theme(m_dmainWindow);

    QStringList imageName = {"TopLeft", "TopCenter", "TopRight",
                            "CenterLeft", "Center", "CenterRight",
                            "BottomLeft", "BottomCenter", "BottomRight",
                             // 特殊的一类
                            "FullWindow"};
    QStringList imagePath = {};
    // 清空列表
    m_imageVar.clear();
    // 获取图片路径
    QString basePath = QDir::homePath() + "/.config/GXDE/dde-file-manager/background-" + theme;
    for (QString i: imageName) {
        imagePath.append(basePath + "-" + i + ".png");
    }
    // 加载 QImage 对象
    for (int i = 0; i < imageName.count(); ++i) {
        QImage image;
        if (QFile::exists(imagePath[i])) {
            image.load(imagePath[i]);
        }
        m_imageVar.append(image);
    }
}
