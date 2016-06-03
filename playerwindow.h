/******************************************************************************
    Simple Player:  this file is part of QtAV examples
    Copyright (C) 2014-2015 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QWidget>
#include <QtAV>
#include <QtAV/AVPlayer.h>
#include <QtAV/AVTranscoder.h>
#include <QStandardPaths>
#include <QDateTime>

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
class QLabel;
class QCheckBox;
QT_END_NAMESPACE
class PlayerWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerWindow(QWidget *parent = 0);
public Q_SLOTS:
    void openMedia();
    void seek(int);
    void capture();
    void startRecord();
    void stopRecord();
private Q_SLOTS:
    void updateSlider();
    void updatePreview(const QImage& image);
    void onCaptureSaved(const QString& path);
    void onCaptureError();
private:
    QtAV::VideoOutput *m_vo;
    QtAV::AVPlayer *m_player;
    QtAV::AVTranscoder *recorder;
    QSlider *m_slider;
    QPushButton *m_openBtn;
    QPushButton *m_captureBtn;
    QPushButton *m_sRecordBtn;
    QPushButton *m_rRecordBtn;
    QLabel *m_preview;
};

#endif // PLAYERWINDOW_H
