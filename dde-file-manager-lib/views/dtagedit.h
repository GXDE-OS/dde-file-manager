#ifndef DTAGEDIT_H
#define DTAGEDIT_H

#include <set>

#include "durl.h"
#include "dcrumbedit.h"
#include "darrowrectangle.h"


#include <QFrame>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFocusEvent>

using namespace Dtk::Widget;


class DTagEdit final : public DArrowRectangle
{
    Q_OBJECT

public:
    explicit DTagEdit(QWidget* const parent = nullptr);
    virtual ~DTagEdit() = default;
    DTagEdit(const DTagEdit& other) = delete;
    DTagEdit& operator=(const DTagEdit& other) = delete;

    void setFocusOutSelfClosing(bool value) noexcept;
    void setFilesForTagging(const QList<DUrl>& files);
    void setDefaultCrumbs(const QStringList &list);

public slots:
    void onFocusOut();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void initializeWidgets();
    void initializeParameters();
    void initializeLayout();
    void initializeConnect();

    void processTags();

    void onPressESC() noexcept;

    DCrumbEdit* m_crumbEdit{ nullptr };
    QLabel* m_promptLabel{ nullptr };
    QVBoxLayout* m_totalLayout{ nullptr };
    QFrame* m_BGFrame{ nullptr };

    QList<DUrl> m_files{};

    std::atomic<bool> m_flagForShown{ false };
};



#endif // DTAGEDIT_H
