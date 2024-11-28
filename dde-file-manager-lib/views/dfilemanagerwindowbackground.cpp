#include "dfilemanagerwindowbackground.h"
#include <QFile>
#include <DThemeManager>
#include <QDebug>
#include <QDir>

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
    if (place == BackgroundPlace::FullWindow) {
        // 拉伸图片
        return m_imageVar[place].scaled(m_dmainWindow->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    }
    return m_imageVar[place];
}

QList<int> DFileManagerWindowBackground::getImageSize(BackgroundPlace place)
{
    QList<int> size;
    QImage image = getImage(place);
    size << image.size().width() << image.height();
    return size;
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
    if (place % 3 == 1 || place == BackgroundPlace::FullWindow) {
        x = (windowWidth - imageWidth) / 2;
    }
    // 如果为 Bottom（2,5,8）,则可以直接确定 X = width - image.width
    if (place % 3 == 2) {
        x = windowWidth - imageWidth;
    }
    //// 确定 Y 轴
    // 如果为 Top（0<=place<=2）,则可以直接确定 Y = 0
    if (place >= BackgroundPlace::TopLeft && place <= BackgroundPlace::TopRight) {
        y = 0;
    }
    // 如果为 Center（3<=place<=5）,则可以直接确定 Y = (height - image.height) / 2
    if ((place >= BackgroundPlace::CenterLeft && place <= BackgroundPlace::CenterRight) || place == 9) {
        y = (windowHeight - imageHeight) / 2;
    }
    // 如果为 Bottom（6<=place<=8）,则可以直接确定 Y = height - image.width
    if (place >= BackgroundPlace::BottomLeft && place <= BackgroundPlace::BottomRight) {
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
