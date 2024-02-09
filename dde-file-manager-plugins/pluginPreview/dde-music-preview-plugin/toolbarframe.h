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

#ifndef TOOLBARFRAME_H
#define TOOLBARFRAME_H

#include <QFrame>
#include <QMediaPlayer>


class QPushButton;
class QSlider;
class QLabel;
class QMediaPlayer;
class QTimer;
class ToolBarFrame : public QFrame
{
    Q_OBJECT
public:
    explicit ToolBarFrame(const QString& uri, QWidget *parent = 0);

    void initUI();
    void initConnections();

signals:

public slots:
    void onPlayStateChanged(const QMediaPlayer::State& state);
    void onPlayStatusChanged(const QMediaPlayer::MediaStatus& status);
    void onPlayControlButtonClicked();
    void updateProgress();
    void seekPosition(const int& pos);
    void play();
    void pause();
    void stop();

private:
    QMediaPlayer* m_player;
    QPushButton* m_playControlButton;
    QSlider* m_progressSlider;
    QLabel* m_durationLabel;
    QTimer* m_updateProgressTimer;
};

#endif // TOOLBARFRAME_H
