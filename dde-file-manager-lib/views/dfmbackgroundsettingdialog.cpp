/*#include "dfmbackgroundsettingdialog.h"
#include <QGridLayout>
#include <QFileDialog>

DFMBackgroundSettingDialog::DFMBackgroundSettingDialog(QWidget *parent) : DDialog(parent)
{
    for (QString i: m_backgroundManager.m_imageTranslateList) {
        QPushButton *button = new QPushButton(this);
        button->setText(i);
        button->setToolTip(tr("Set background: ") + i);
        m_settingButtonList.append(button);
        connect(button, &QPushButton::clicked, this, &DFMBackgroundSettingDialog::setBackgroundButtonClick);
    }
    for (int i = 0; i < 9; ++i) {
        m_mainLayout.addWidget(m_settingButtonList[i], i / 3, i % 3);
    }
    m_mainWidget.setLayout(&m_mainLayout);

    // 添加 QWidget 至 DDialog
    addContent(&m_mainWidget, Qt::AlignmentFlag::AlignCenter);
}

void DFMBackgroundSettingDialog::setBackgroundButtonClick()
{
    QPushButton *clickedButton = static_cast<QPushButton *>(sender());
    int buttonIndex = -1;
    // 寻找点击按钮对应的编号
    for (int i = 0; i < m_settingButtonList.count(); ++i) {
        if (m_settingButtonList[i] == clickedButton) {
            buttonIndex = i;
            break;
        }
    }
    if (buttonIndex == -1) {
        return;
    }
    // 获取位置
    QString place = m_backgroundManager.m_imageList.at(buttonIndex);
    QString filePath = QFileDialog::getOpenFileName(this, "", QDir::homePath(), "图像文件 (*.jpg);;所有文件 (*.*)");
    if (filePath == NULL || filePath == "") {
        return;
    }
    setBackground(filePath, "light", place);
    setBackground(filePath, "dark", place);

}

void DFMBackgroundSettingDialog::setBackground(QString filePath, QString theme, QString place)
{

}

// 释放内存
DFMBackgroundSettingDialog::~DFMBackgroundSettingDialog()
{
    for (QPushButton *i: m_settingButtonList) {
        delete i;
    }
}*/

