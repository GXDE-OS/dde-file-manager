
#include "dtagedit.h"
#include "../app/define.h"
#include "../tag/tagmanager.h"
#include "../utils/singleton.h"
#include "../app/filesignalmanager.h"
#include "../controllers/appcontroller.h"
#include "../interfaces/dfmeventdispatcher.h"

#include "dfileservices.h"

#include <QObject>
#include <QKeyEvent>
#include <QApplication>

DTagEdit::DTagEdit(QWidget * const parent)
    : DArrowRectangle{ DArrowRectangle::ArrowTop,  parent }
{
    this->initializeWidgets();
    this->initializeParameters();
    this->initializeLayout();
    this->initializeConnect();

    this->installEventFilter(this);

    setWindowFlags(Qt::Tool);
}

void DTagEdit::setFocusOutSelfClosing(bool value)noexcept
{
    ///###: CAS!
    bool excepted{ !value };
    m_flagForShown.compare_exchange_strong(excepted, value, std::memory_order_release);
}

void DTagEdit::setFilesForTagging(const QList<DUrl>& files)
{
    m_files = files;
}

void DTagEdit::setDefaultCrumbs(const QStringList &list)
{
    for (const QString &crumb : list)
        m_crumbEdit->appendCrumb(crumb);
}

void DTagEdit::onFocusOut()
{
    if(m_flagForShown.load(std::memory_order_acquire)){
        this->processTags();
        this->close();
    }
}

void DTagEdit::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
    {
        this->processTags();
        event->accept();
        this->close();
        break;
    }
    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
        QObject::disconnect(this, &DTagEdit::windowDeactivate, this, &DTagEdit::onFocusOut);
        this->processTags();
        event->accept();
        this->close();
        break;
    }
    default:
        break;
    }

    DArrowRectangle::keyPressEvent(event);
}

void DTagEdit::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void DTagEdit::initializeWidgets()
{
    m_crumbEdit = new DCrumbEdit;
    m_promptLabel = new QLabel{
                                QObject::tr("Input tag info, such as work, family. A comma is used between two tags.")
                              };
    m_totalLayout = new QVBoxLayout;
    m_BGFrame = new QFrame;
}

void DTagEdit::initializeParameters()
{
    m_crumbEdit->setFixedSize(140, 40);
    m_crumbEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_crumbEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_crumbEdit->setCrumbReadOnly(true);
    m_crumbEdit->setCrumbRadius(2);

    m_promptLabel->setFixedWidth(140);
    m_promptLabel->setWordWrap(true);
    m_BGFrame->setContentsMargins(QMargins{0, 0, 0, 0});

    this->setMargin(0);
    this->setFixedWidth(140);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setBorderColor(QColor{"#ffffff"});
    this->setBackgroundColor(QColor{"#ffffff"});
    this->setWindowFlags(Qt::FramelessWindowHint);
}

void DTagEdit::initializeLayout()
{
    m_totalLayout->addStretch(1);
    m_totalLayout->addWidget(m_crumbEdit);
    m_totalLayout->addSpacing(8);
    m_totalLayout->addWidget(m_promptLabel);
    m_totalLayout->addStretch(1);

    m_BGFrame->setLayout(m_totalLayout);
    this->setContent(m_BGFrame);
}

void DTagEdit::initializeConnect()
{
    QObject::connect(this, &DTagEdit::windowDeactivate, this, &DTagEdit::onFocusOut);
}

void DTagEdit::processTags()
{
    QList<QString> tagList{ m_crumbEdit->crumbList() };

    DFileService::instance()->makeTagsOfFiles(this, m_files, tagList);
}
