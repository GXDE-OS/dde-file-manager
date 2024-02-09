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

#ifndef MOUNTASKPASSWORDDIALOG_H
#define MOUNTASKPASSWORDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QJsonObject>
#include <ddialog.h>
#include <dpasswordedit.h>

DWIDGET_USE_NAMESPACE




class MountAskPasswordDialog : public DDialog
{
    Q_OBJECT
public:
    explicit MountAskPasswordDialog(QWidget *parent = 0);
    ~MountAskPasswordDialog();

    void initUI();
    void initConnect();
    QJsonObject getLoginData();

    bool getDomainLineVisible() const;
    void setDomainLineVisible(bool domainLineVisible);

signals:

public slots:
    void setLoginData(const QJsonObject& obj);
    void handleConnect();
    void togglePasswordFrame(int id);
    void handleButtonClicked(int index, QString text);

private:
    QLabel* m_messageLabel;
    QLabel* m_domainLabel;

    QFrame* m_passwordFrame;

    QButtonGroup* m_anonymousButtonGroup;

    QLineEdit* m_usernameLineEdit;
    QLineEdit* m_domainLineEdit;
    DPasswordEdit* m_passwordLineEdit;

    QCheckBox* m_passwordCheckBox;

    QButtonGroup* m_passwordButtonGroup;


    QJsonObject m_loginObj;

    bool m_domainLineVisible = true;
};

#endif // MOUNTASKPASSWORDDIALOG_H
