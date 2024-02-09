/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *               2016 ~ 2018 dragondjf
 *
 * Author:     dragondjf<dingjiangfeng@deepin.com>
 *
 * Maintainer: dragondjf<dingjiangfeng@deepin.com>
 *             zccrs<zhangjide@deepin.com>
 *             Tangtong<tangtong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <tuple>
#include <atomic>

#include <QPair>
#include <QLabel>
#include <QWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QValidator>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedLayout>

#include "ddesktoprenamedialog.h"


class DDesktopRenameDialogPrivate
{
public:
    explicit DDesktopRenameDialogPrivate(DDesktopRenameDialog* const qPtr);
    ~DDesktopRenameDialogPrivate()=default;
    DDesktopRenameDialogPrivate(const DDesktopRenameDialogPrivate& other)=delete;
    DDesktopRenameDialogPrivate& operator=(const DDesktopRenameDialogPrivate& other)=delete;


    void initUi();
    void initUiParameters();
    void initUiLayout();

    DDesktopRenameDialog* q_ptr{ nullptr };

    QLabel* m_titleLabel{ nullptr };

    std::tuple<QLabel*, QComboBox*, QHBoxLayout*, QFrame*> m_itemsForSelecting{};
    QStackedLayout* m_stackedLayout{ nullptr };

    std::tuple<QLabel*, QLineEdit*, QHBoxLayout*> m_modeOneItemsForFinding{};
    std::tuple<QLabel*, QLineEdit*, QHBoxLayout*> m_modeOneItemsForReplacing{};
    QPair<QVBoxLayout*, QFrame*> m_modeOneLayout{};


    std::tuple<QLabel*, QLineEdit*, QHBoxLayout*> m_modeTwoItemsForAdding{};
    std::tuple<QLabel*, QComboBox*, QHBoxLayout*> m_modeTwoItemsForLocating{};
    QPair<QVBoxLayout*, QFrame*> m_modeTwoLayout{};


    std::tuple<QLabel*, QLineEdit*, QHBoxLayout*> m_modeThreeItemsForFileName{};
    std::tuple<QLabel*, QLineEdit*, QHBoxLayout*> m_modeThreeItemsForSNNumber{};
    QPair<QVBoxLayout*, QFrame*> m_modeThreeLayout{};
    QRegExpValidator* m_validator{ nullptr };

    QVBoxLayout* m_mainLayout{ nullptr };
    QFrame* m_mainFrame{ nullptr };
    std::size_t m_currentmode{ 0 };      //###: reocord current mode.
    std::array<bool, 3>  m_currentEnabled{{ false }}; //###: this array record current status of renamebutton at current mode.
    DFileService::AddTextFlags m_flagForAdding{ DFileService::AddTextFlags::Before }; //###: this flag is useful in mode 2. It record what to append str to name.
};


DDesktopRenameDialogPrivate::DDesktopRenameDialogPrivate(DDesktopRenameDialog * const qPtr)
                        :q_ptr{ qPtr}
{
    this->initUi();
    this->initUiParameters();
    this->initUiLayout();

}

void DDesktopRenameDialogPrivate::initUi()
{

    m_titleLabel = new QLabel{};

    m_itemsForSelecting = std::make_tuple(new QLabel{}, new QComboBox{}, new QHBoxLayout{}, new QFrame{});
    m_stackedLayout = new QStackedLayout{};

    m_modeOneItemsForFinding = std::make_tuple(new QLabel{}, new QLineEdit{}, new QHBoxLayout{});
    m_modeOneItemsForReplacing = std::make_tuple(new QLabel{}, new QLineEdit{}, new QHBoxLayout{});
    m_modeOneLayout = QPair<QVBoxLayout*, QFrame*>{new QVBoxLayout{}, new QFrame{}};


    m_modeTwoItemsForAdding = std::make_tuple(new QLabel{}, new QLineEdit{}, new QHBoxLayout);
    m_modeTwoItemsForLocating = std::make_tuple(new QLabel{}, new QComboBox{}, new QHBoxLayout{});
    m_modeTwoLayout = QPair<QVBoxLayout*, QFrame*>{new QVBoxLayout{}, new QFrame{}};


    m_modeThreeItemsForFileName = std::make_tuple(new QLabel{}, new QLineEdit{}, new QHBoxLayout{});
    m_modeThreeItemsForSNNumber = std::make_tuple(new QLabel{}, new QLineEdit{}, new QHBoxLayout{});
    m_modeThreeLayout = QPair<QVBoxLayout*, QFrame*>{new QVBoxLayout{}, new QFrame{}};

    QRegExp regStr{ QString{"[0-9]+"} };
    m_validator = new QRegExpValidator{ regStr };

    m_mainLayout = new QVBoxLayout{};
    m_mainFrame = new QFrame{};
}


void DDesktopRenameDialogPrivate::initUiParameters()
{
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setObjectName(QString{"DRenameDialogTitleLabel"});
    m_titleLabel->setStyleSheet(QString{"QLabel#DRenameDialogTitleLabel"
                                        "{"
                                            "color: #000000;"
                                            "font-family: SourceHanSansSC;"
                                            "font-size: 14px;"
                                        "}"
                                        });

    QLabel* tagLabel{ nullptr };
    QComboBox* modeChoseBox{ nullptr };
    QLineEdit* contentLineEdit{ nullptr };

    tagLabel = std::get<0>(m_itemsForSelecting);
    tagLabel->setText(QObject::tr("Mode:"));
    modeChoseBox = std::get<1>(m_itemsForSelecting);
    modeChoseBox->addItems(QList<QString>{ QObject::tr("Replace Text"), QObject::tr("Add Text"), QObject::tr("Custom Text") });
    modeChoseBox->setFixedSize(QSize{275, 25});


    ///###: mode 1
    tagLabel = std::get<0>(m_modeOneItemsForFinding);
    tagLabel->setText(QObject::tr("Find:"));
    contentLineEdit = std::get<1>(m_modeOneItemsForFinding);
    contentLineEdit->setFocus();
    contentLineEdit->setPlaceholderText(QObject::tr("Required"));
    contentLineEdit->setFixedSize(QSize{275, 25});
    contentLineEdit->setObjectName( QString{"DRenameDialogLineEditForFinding"} );
//    contentLineEdit->setFocusPolicy(Qt::StrongFocus);
    contentLineEdit->setStyleSheet("QLineEdit#DRenameDialogLineEditForFinding:focus"
                                                         "{"
                                                                "border: 1px solid #2ca7f8;"
                                                                "border-radius: 2px;"
                                                         "}"
                                                         "QLineEdit#DRenameDialogLineEditForFinding"
                                                         "{"
                                                                "border: 1px solid rgba(0, 0, 0, 0.08);"
                                                                "border-radius: 2px;"
                                                         "}"

                                                        );

    tagLabel = std::get<0>(m_modeOneItemsForReplacing);
    tagLabel->setText(QObject::tr("Replace:"));
    contentLineEdit = std::get<1>(m_modeOneItemsForReplacing);
    contentLineEdit->setPlaceholderText(QObject::tr("Optional"));
    contentLineEdit->setFixedSize(QSize{275, 25});
    contentLineEdit->setObjectName( QString{ "DRenameDialogLineEditForReplacing" } );
    contentLineEdit->setStyleSheet("QLineEdit#DRenameDialogLineEditForReplacing:focus"
                                                           "{"
                                                                 "border: 1px solid #2ca7f8;"
                                                                 "border-radius: 2px;"
                                                           "}"
                                                           "QLineEdit#DRenameDialogLineEditForReplacing"
                                                           "{"
                                                                  "border: 1px solid rgba(0, 0, 0, 0.08);"
                                                                  "border-radius: 2px;"
                                                           "}"
                                                           );

    ///###: mode 2
    tagLabel = std::get<0>(m_modeTwoItemsForAdding);
    tagLabel->setText(QObject::tr("Add:"));
    contentLineEdit = std::get<1>(m_modeTwoItemsForAdding);
    contentLineEdit->setPlaceholderText(QObject::tr("Required"));
    contentLineEdit->setFixedSize(QSize{275, 25});
    contentLineEdit->setMaxLength(300);
    contentLineEdit->setObjectName(QString{"DRenameDialogLineEditForAdding"});
    contentLineEdit->setStyleSheet("QLineEdit#DRenameDialogLineEditForAdding:focus"
                                                        "{"
                                                              "border: 1px solid #2ca7f8;"
                                                              "border-radius: 2px;"
                                                         "}"
                                                        "QLineEdit#DRenameDialogLineEditForAdding"
                                                        "{"
                                                               "border: 1px solid rgba(0, 0, 0, 0.08);"
                                                               "border-radius: 2px;"
                                                        "}"
                                                        );

    tagLabel = std::get<0>(m_modeTwoItemsForLocating);
    tagLabel->setText(QObject::tr("Location:"));
    modeChoseBox = std::get<1>(m_modeTwoItemsForLocating);
    modeChoseBox->addItems(QList<QString>{ QObject::tr("Before file name"), QObject::tr("After file name") });
    modeChoseBox->setFixedSize( QSize{275, 25} );

    ///###: mode3
    tagLabel = std::get<0>(m_modeThreeItemsForFileName);
    tagLabel->setText(QObject::tr("File name:"));
    contentLineEdit = std::get<1>(m_modeThreeItemsForFileName);
    contentLineEdit->setPlaceholderText(QObject::tr("Required"));
    contentLineEdit->setFixedSize(QSize{275, 25});
    contentLineEdit->setObjectName( QString{ "DRenameDialogLineEditForCustomizing" } );
    contentLineEdit->setStyleSheet("QLineEdit#DRenameDialogLineEditForCustomizing:focus"
                                                            "{"
                                                                    "border: 1px solid rgba(0, 132, 255, 0.4);"
                                                                    "border-radius: 2px;"
                                                             "}"
                                                            "QLineEdit#DRenameDialogLineEditForCustomizing"
                                                            "{"
                                                                   "border: 1px solid rgba(0, 0, 0, 0.08);"
                                                                   "border-radius: 2px;"
                                                            "}"
                                                            );
    tagLabel = std::get<0>(m_modeThreeItemsForSNNumber);
    tagLabel->setText(QObject::tr("+SN:"));
    contentLineEdit = std::get<1>(m_modeThreeItemsForSNNumber);
    contentLineEdit->setFixedSize(QSize{275, 25});
    contentLineEdit->setValidator(m_validator);
    contentLineEdit->setText(QString{"1"});

    tagLabel = nullptr;
    modeChoseBox = nullptr;
    contentLineEdit = nullptr;
}

void DDesktopRenameDialogPrivate::initUiLayout()
{
    QLabel* tagLabel{ nullptr };
    QComboBox* modeChoseBox{ nullptr };
    QLineEdit* contentLineEdit{ nullptr };
    QHBoxLayout* hLayout{ nullptr };


    tagLabel = std::get<0>(m_itemsForSelecting);
    modeChoseBox = std::get<1>(m_itemsForSelecting);
    tagLabel->setBuddy(modeChoseBox);
    hLayout = std::get<2>(m_itemsForSelecting);
    hLayout->addWidget(tagLabel);
    hLayout->addSpacing(30);
    hLayout->addWidget(modeChoseBox);
    hLayout->setMargin(0);


    ///###: mode 1
    tagLabel = std::get<0>(m_modeOneItemsForFinding);
    contentLineEdit = std::get<1>(m_modeOneItemsForFinding);
    tagLabel->setBuddy(contentLineEdit);
    hLayout = std::get<2>(m_modeOneItemsForFinding);
    hLayout->addWidget(tagLabel);
    hLayout->addSpacing(30);
    hLayout->addWidget(contentLineEdit);
    hLayout->setMargin(0);


    tagLabel = std::get<0>(m_modeOneItemsForReplacing);
    contentLineEdit = std::get<1>(m_modeOneItemsForReplacing);
    tagLabel->setBuddy(contentLineEdit);
    hLayout = std::get<2>(m_modeOneItemsForReplacing);
    hLayout->addWidget(tagLabel);
    hLayout->addSpacing(30);
    hLayout->addWidget(contentLineEdit);


    m_modeOneLayout.first->addLayout(std::get<2>(m_modeOneItemsForFinding));
    m_modeOneLayout.first->addSpacing(10);
    m_modeOneLayout.first->addLayout(std::get<2>(m_modeOneItemsForReplacing));
    m_modeOneLayout.first->setSpacing(0);
    m_modeOneLayout.first->setMargin(0);
    m_modeOneLayout.second->setLayout(m_modeOneLayout.first);




    ///###: mode 2
    hLayout = std::get<2>(m_modeTwoItemsForAdding);
    hLayout->setSpacing(0);
    hLayout->setMargin(0);
    tagLabel = std::get<0>(m_modeTwoItemsForAdding);
    contentLineEdit = std::get<1>(m_modeTwoItemsForAdding);
    tagLabel->setBuddy(contentLineEdit);
    hLayout->addWidget(tagLabel);
    hLayout->addSpacing(30);
    hLayout->addWidget(contentLineEdit);
    hLayout->setMargin(0);


    tagLabel = std::get<0>(m_modeTwoItemsForLocating);
    modeChoseBox = std::get<1>(m_modeTwoItemsForLocating);
    tagLabel->setBuddy(modeChoseBox);
    hLayout = std::get<2>(m_modeTwoItemsForLocating);
    hLayout->addWidget(tagLabel);
    hLayout->addWidget(modeChoseBox);


    m_modeTwoLayout.first->addLayout(std::get<2>(m_modeTwoItemsForAdding));
    m_modeTwoLayout.first->addSpacing(10);
    m_modeTwoLayout.first->addLayout(std::get<2>(m_modeTwoItemsForLocating));
    m_modeTwoLayout.first->setSpacing(0);
    m_modeTwoLayout.first->setMargin(0);
    m_modeTwoLayout.second->setLayout(m_modeTwoLayout.first);


    ///###: mode 3
    tagLabel = std::get<0>(m_modeThreeItemsForFileName);
    contentLineEdit = std::get<1>(m_modeThreeItemsForFileName);
    tagLabel->setBuddy(contentLineEdit);
    hLayout = std::get<2>(m_modeThreeItemsForFileName);
    hLayout->addWidget(tagLabel);
    hLayout->addWidget(contentLineEdit);

    tagLabel = std::get<0>(m_modeThreeItemsForSNNumber);
    contentLineEdit = std::get<1>(m_modeThreeItemsForSNNumber);
    tagLabel->setBuddy(contentLineEdit);
    hLayout = std::get<2>(m_modeThreeItemsForSNNumber);
    hLayout->addWidget(tagLabel);
    hLayout->addWidget(contentLineEdit);


    m_modeThreeLayout.first->addLayout(std::get<2>(m_modeThreeItemsForFileName));
    m_modeThreeLayout.first->addSpacing(20);
    m_modeThreeLayout.first->addLayout(std::get<2>(m_modeThreeItemsForSNNumber));
    m_modeThreeLayout.first->setSpacing(0);
    m_modeThreeLayout.first->setMargin(0);
    m_modeThreeLayout.second->setLayout(m_modeThreeLayout.first);

    ///###: total layout.
    m_stackedLayout->addWidget(m_modeOneLayout.second);
    m_stackedLayout->addWidget(m_modeTwoLayout.second);
    m_stackedLayout->addWidget(m_modeThreeLayout.second);
    m_stackedLayout->setCurrentIndex(0);

    m_mainLayout->setSpacing(0);
    m_mainLayout->setMargin(0);
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addSpacing(30);
    m_mainLayout->addLayout(std::get<2>(m_itemsForSelecting));
    m_mainLayout->addSpacing(10);
    m_mainLayout->addLayout(m_stackedLayout);
    m_mainLayout->addSpacing(15);

    m_mainFrame->setLayout(m_mainLayout);
}



DDesktopRenameDialog::DDesktopRenameDialog(QWidget* const parent)
                  :DDialog{ parent },
                   d_ptr{ new DDesktopRenameDialogPrivate{ this } }
{
    this->initUi();
    this->initUiParameters();
    this->initConnect();

    this->installEventFilter(this);
}


///###: I initialize buttons here.
///   and other widgets in DDesktopRenameDialog is initialized when new DDesktopRenameDialogPrivate.
void DDesktopRenameDialog::initUi()
{
    DDesktopRenameDialogPrivate* const d{ d_func() };
    this->addContent(d->m_mainFrame, Qt::AlignCenter);
    this->addButton(QObject::tr("Cancel"));
    this->addButton(QObject::tr("Rename"), true);
}

void DDesktopRenameDialog::initUiParameters()
{
    if(QPushButton* renameButton = dynamic_cast<QPushButton*>(this->getButton(1))){
        renameButton->setStyleSheet(
                    "QPushButton:enabled"
                    "{"
                        "font-family: SourceHanSansSC;"
                        "font-size: 13px;"
                        "color: #0087ff;"
                    "}"

                    "QPushButton:hover"
                    "{"
                         "background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                         "stop: 0 #8CCFFF,"
                         "stop: 1  #4BB8FF);"
                         "color: white;"
                        "border-radius: 0px;"
                     "}"
                     "QPushButton:pressed"
                     "{"
                              "background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                               "stop: 0 #0b8cff,"
                                               "stop: 1 #0aa1ff);"
                             "border: 1px solid rgba(29, 129, 255, 0.3);"
                             "color: white;"
                     "}"

                    );
    }
}


void DDesktopRenameDialog::initConnect()
{
    DDesktopRenameDialogPrivate* const d{ d_func() };
    using funcType = void (QComboBox::*)(int index);


    QObject::connect(std::get<1>(d->m_itemsForSelecting), static_cast<funcType>(&QComboBox::currentIndexChanged), this, &DDesktopRenameDialog::onCurrentModeChanged);
    QObject::connect(std::get<1>(d->m_modeTwoItemsForLocating), static_cast<funcType>(&QComboBox::currentIndexChanged), this, &DDesktopRenameDialog::onCurrentAddModeChanged);
    QObject::connect(std::get<1>(d->m_modeThreeItemsForSNNumber), &QLineEdit::textChanged, this, &DDesktopRenameDialog::onContentChangedForCustomzedSN);
    QObject::connect(this, &DDesktopRenameDialog::visibleChanged, this, &DDesktopRenameDialog::onVisibleChanged);


    try{

        if(QPushButton* cancelButton = dynamic_cast<QPushButton*>(this->getButton(0))){
            QObject::connect(this, &DDesktopRenameDialog::clickCancelButton, cancelButton, &QPushButton::click);
        }

        if(QPushButton* renameButton = dynamic_cast<QPushButton*>(this->getButton(1))){
            QObject::connect(this, &DDesktopRenameDialog::clickRenameButton, renameButton, &QPushButton::click);
        }

    }catch(const std::bad_cast& error){
        (void)error;

        throw std::runtime_error{ "badly dynamic cast in DDesktopRenameDialog" };
    }

}


void DDesktopRenameDialog::onCurrentModeChanged(const std::size_t &index)noexcept
{
    DDesktopRenameDialogPrivate* const d{ d_func() };

    d->m_currentmode = index;
    d->m_stackedLayout->setCurrentIndex(index);

    ///###: here, make lineEdit has focus immediately, after change mode.
    this->onVisibleChanged(true);
}

void DDesktopRenameDialog::onCurrentAddModeChanged(const std::size_t &index)noexcept
{
    DDesktopRenameDialogPrivate* d{ d_func() };

    if(index == 0){
        d->m_flagForAdding = DFileService::AddTextFlags::Before;

    }else{
        d->m_flagForAdding = DFileService::AddTextFlags::After;
    }

    ///###: here, make lineEdit has focus immediately, after change mode.
    this->onVisibleChanged(true);
}

std::size_t DDesktopRenameDialog::getCurrentModeIndex()const noexcept
{
    const DDesktopRenameDialogPrivate* const d{ d_func() };

    return d->m_currentmode;
}

DFileService::AddTextFlags DDesktopRenameDialog::getAddMode()const noexcept
{
    const DDesktopRenameDialogPrivate* const d{ d_func() };

    return d->m_flagForAdding;
}

QPair<QString, QString> DDesktopRenameDialog::getModeOneContent()const noexcept
{
    const DDesktopRenameDialogPrivate* const d{ d_func() };

    QString findStr{ std::get<1>(d->m_modeOneItemsForFinding)->text() };
    QString replaceStr{ std::get<1>(d->m_modeOneItemsForReplacing)->text() };

    return QPair<QString, QString>{ findStr, replaceStr };
}


QPair<QString, DFileService::AddTextFlags> DDesktopRenameDialog::getModeTwoContent()const noexcept
{
    const DDesktopRenameDialogPrivate* const d{ d_func() };

    QString addStr{ std::get<1>(d->m_modeTwoItemsForAdding)->text() };
    DFileService::AddTextFlags flag{ d->m_flagForAdding };

    return QPair<QString, DFileService::AddTextFlags>{ addStr, flag };
}


QPair<QString, QString> DDesktopRenameDialog::getModeThreeContent()const noexcept
{
    const DDesktopRenameDialogPrivate* const d{ d_func() };

    QString fileName{ std::get<1>(d->m_modeThreeItemsForFileName)->text() };
    QString numberStr{ std::get<1>(d->m_modeThreeItemsForSNNumber)->text() };

    return QPair<QString, QString>{ fileName, numberStr};
}


///###: This function will change enable of property of the button and
///     style sheet.
void DDesktopRenameDialog::setRenameButtonStatus(const bool &enabled)
{
    if(QPushButton* renameButton = dynamic_cast<QPushButton*>(this->getButton(1))){

        renameButton->setEnabled(enabled);
    }
}


void DDesktopRenameDialog::onContentChangedForFinding(const QString &content)
{
    DDesktopRenameDialogPrivate* const d{ d_func() };

    if(content.isEmpty() == false){
        d->m_currentEnabled[0] = true;
        this->setRenameButtonStatus(true);

    }else{
        d->m_currentEnabled[0] = false;
        this->setRenameButtonStatus(false);
    }
}


void DDesktopRenameDialog::onContentChangedForAdding(const QString &content)
{
    DDesktopRenameDialogPrivate* const d{ d_func() };

    if(content.isEmpty() == false){
        d->m_currentEnabled[1] = true;
        this->setRenameButtonStatus(true);

    }else{
        d->m_currentEnabled[1] = false;
        this->setRenameButtonStatus(false);
    }
}

void DDesktopRenameDialog::onContentChangedForCustomzedSN(const QString &content)
{
    DDesktopRenameDialogPrivate* d{ d_func() };

    QLineEdit* lineEditForSNNumber{ std::get<1>(d->m_modeThreeItemsForSNNumber) };

    std::string numberStr{ content.toStdString() };
    try{
        std::stoull(numberStr);

    }catch(const std::out_of_range& err){
        (void)err;
        lineEditForSNNumber->setText(QString{"1"});

    }catch(...){
        lineEditForSNNumber->setText(QString{"1"});
    }
}

void DDesktopRenameDialog::setDialogTitle(const QString &tile)noexcept
{
    DDesktopRenameDialogPrivate* const d{ d_func() };
    d->m_titleLabel->setText(tile);
}


void DDesktopRenameDialog::setVisible(bool visible)
{
    this->DDialog::setVisible(visible);
    emit visibleChanged(visible);
}


void DDesktopRenameDialog::onVisibleChanged(bool visible)noexcept
{
    DDesktopRenameDialogPrivate* const d{ d_func() };

    if(visible == true){

        switch(d->m_currentmode)
        {
             case 0:
             {
                QLineEdit* lineEdit{ std::get<1>(d->m_modeOneItemsForFinding) };
                lineEdit->setFocus();
                break;
             }
             case 1:
            {
                QLineEdit* lineEdit{ std::get<1>(d->m_modeTwoItemsForAdding) };
                lineEdit->setFocus();
                break;
            }
            case 2:
            {
                QLineEdit* lineEdit{ std::get<1>(d->m_modeThreeItemsForFileName) };
                lineEdit->setFocus();
                break;
            }
            default:
                break;

        }
    }
}


