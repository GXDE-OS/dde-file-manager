/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     Gary Wang <wzc782970009@gmail.com>
 *
 * Maintainer: Gary Wang <wangzichong@deepin.com>
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

#ifndef DFMADVANCESEARCHBAR_H
#define DFMADVANCESEARCHBAR_H

#include <QVBoxLayout>

#include <QComboBox>
#include <QLabel>
#include <QWidget>
#include <dfmglobal.h>
#include <dboxwidget.h>
#include <dlinkbutton.h>

DWIDGET_USE_NAMESPACE

DFM_BEGIN_NAMESPACE

class DFMAdvanceSearchBar : public DBoxWidget
{
    Q_OBJECT
public:
    DFMAdvanceSearchBar(QWidget* parent = nullptr);

    void resetForm();

signals:
    void optionChanged(QMap<int, QVariant> formData);

private slots:
    void onOptionChanged();
    void onResetButtonPressed();

protected slots:
    void hideEvent(QHideEvent *) override;

private:
    void initUI();
    void initConnection();

    enum LabelIndex {
        SEARCH_RANGE, FILE_TYPE, SIZE_RANGE, DATE_RANGE, LABEL_COUNT,
        TRIGGER_SEARCH = 114514
    };

    QBoxLayout *mainLayout;
    QLabel *asbLabels[LABEL_COUNT];
    QComboBox *asbCombos[LABEL_COUNT];
    DLinkButton *resetBtn;
    bool needSearchAgain = true;
};

DFM_END_NAMESPACE

#endif // DFMADVANCESEARCHBAR_H
