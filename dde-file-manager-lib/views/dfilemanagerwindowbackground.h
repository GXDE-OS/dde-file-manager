#ifndef DFILEMANAGERWINDOWBACKGROUND_H
#define DFILEMANAGERWINDOWBACKGROUND_H

#include <QImage>
#include <QList>
#include <QPainter>
#include <dmainwindow.h>

DWIDGET_USE_NAMESPACE

class DFileManagerWindowBackground
{
public:
    enum BackgroundPlace {
        TopLeft = 0,
        TopCenter = 1,
        TopRight = 2,
        CenterLeft = 3,
        Center = 4,
        CenterRight = 5,
        BottomLeft = 6,
        BottomCenter = 7,
        BottomRight = 8,
        // 特殊的一类
        FullWindow = 9
    };
    QStringList m_imageList = {"TopLeft", "TopCenter", "TopRight",
                            "CenterLeft", "Center", "CenterRight",
                            "BottomLeft", "BottomCenter", "BottomRight",
                             // 特殊的一类
                            "FullWindow"};
    QStringList m_imageTranslateList = {"TopLeft", "TopCenter", "TopRight",
                                        "CenterLeft", "Center", "CenterRight",
                                        "BottomLeft", "BottomCenter", "BottomRight",
                                         // 特殊的一类
                                        "FullWindow"};

    DFileManagerWindowBackground(DMainWindow *window);
    void refresh();
    void resizeImage();
    void setMainWindow(DMainWindow *window);

    QImage getImage(BackgroundPlace place);
    QList<int> getImageXY(BackgroundPlace place);
    QList<int> getImageSize(BackgroundPlace place);

    void drawInWidget(QPainter *painter);

private:
    QList<QImage> m_imageVar;
    DMainWindow *m_dmainWindow;
    bool m_showFMLogo = true;
    QImage m_fmLogo;
    QImage m_backgroundResized;
    /*QImage m_topLeft_image;
    QImage m_topCenter_image;
    QImage m_topRight_image;
    QImage m_centerLeft_image;
    QImage m_center_image;
    QImage m_centerRight_image;
    QImage m_bottomLeft_image;
    QImage m_bottomCenter_image;
    QImage m_bottomRight_image;*/
};

#endif // DFILEMANAGERWINDOWBACKGROUND_H
