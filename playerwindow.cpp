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

#include "playerwindow.h"
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QtAVWidgets/QtAVWidgets>
#include <QtAV>
#include <QtAVWidgets/QtAVWidgets>

using namespace QtAV;

PlayerWindow::PlayerWindow(QWidget *parent) : QWidget(parent)
{
    QtAV::Widgets::registerRenderers();
    setWindowTitle(QString::fromLatin1("QtAV simple player example"));
    m_player = new AVPlayer(this);
    QVBoxLayout *vl = new QVBoxLayout();
    setLayout(vl);
    m_vo = new VideoOutput(this);
    if (!m_vo->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), QString::fromLatin1("Can not create video renderer"));
        return;
    }
    m_player->setRenderer(m_vo);
    vl->addWidget(m_vo->widget());
    m_slider = new QSlider();
    m_slider->setOrientation(Qt::Horizontal);
    connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(seek(int)));
    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(updateSlider()));
    connect(m_player, SIGNAL(started()), SLOT(updateSlider()));

    vl->addWidget(m_slider);
    QHBoxLayout *hb = new QHBoxLayout();
    vl->addLayout(hb);
    m_openBtn = new QPushButton(tr("Open"));
    m_captureBtn = new QPushButton(tr("Capture video frame"));
    m_sRecordBtn = new QPushButton(tr("StartRecord"));
    m_rRecordBtn = new QPushButton(tr("StopRecord"));
    hb->addWidget(m_openBtn);
    hb->addWidget(m_captureBtn);
    hb->addWidget(m_sRecordBtn);
    hb->addWidget(m_rRecordBtn);

    m_preview = new QLabel(tr("Capture preview"));
    m_preview->setFixedSize(200, 150);
    hb->addWidget(m_preview);
    connect(m_openBtn, SIGNAL(clicked()), SLOT(openMedia()));
    connect(m_captureBtn, SIGNAL(clicked()), SLOT(capture()));
    connect(m_sRecordBtn, SIGNAL(clicked()), SLOT(startRecord()));
    connect(m_rRecordBtn, SIGNAL(clicked()), SLOT(stopRecord()));
    connect(m_player->videoCapture(), SIGNAL(imageCaptured(QImage)), SLOT(updatePreview(QImage)));
    connect(m_player->videoCapture(), SIGNAL(saved(QString)), SLOT(onCaptureSaved(QString)));
    connect(m_player->videoCapture(), SIGNAL(failed()), SLOT(onCaptureError()));

    QDateTime now = QDateTime::currentDateTime();
    QString name = "video_"+now.toString("ddMMyyhhmmss");
    QString dir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    QString path(dir + QStringLiteral("/") + name + QStringLiteral(".mp4"));
    QVariantHash muxopt, avfopt;
    avfopt[QString::fromLatin1("segment_time")] = 4;
    avfopt[QString::fromLatin1("segment_list_size")] = 0;
    avfopt[QString::fromLatin1("segment_format")] = QString::fromLatin1("mpegts");
    muxopt[QString::fromLatin1("avformat")] = avfopt;
    recorder = new AVTranscoder;
    recorder->setMediaSource(m_player);
    recorder->setOutputOptions(muxopt);
    recorder->setAsync(true);
    if (!recorder->createVideoEncoder()) {
        qWarning("Failed to create video encoder");
    }
    //QString cv = QString::fromLatin1("libx264");
    //recorder->setOutputFormat(QLatin1String("mjpeg"));
    recorder->videoEncoder()->setCodecName(QLatin1String("mpeg4"));
    recorder->videoEncoder()->setBitRate(1920*1080);
    recorder->videoEncoder()->setHeight(1080);
    recorder->videoEncoder()->setWidth(1920);
    QStringList liste = recorder->videoEncoder()->supportedCodecs();
    if (liste.isEmpty()){
            qDebug() << "Y a un probleme la WALABOOK----------------------------------------------------";
    }
    else{
             qDebug() << liste;
    }
    recorder->setOutputMedia(path);

}

void PlayerWindow::openMedia()
{
//    QString file = QFileDialog::getOpenFileName(0, tr( "Open a video"));
//    if (file.isEmpty())
//        return;

    QVariantHash opt;
    opt["probesize"] = 32;
    m_player->setOptionsForFormat(opt);

    QVariantHash opt2;
    opt2["fflags"]="nobuffer";
    m_player->setOptionsForFormat(opt2);

    QVariantHash opt3;
    opt3["analyzeduration"] = 1;
    m_player->setOptionsForFormat(opt3);

    QVariantHash opt4;
    opt4["max_delay"] = 1;
    m_player->setOptionsForFormat(opt4);


    m_player->setFrameRate(120);
    m_player->setBufferValue(1);
//    m_player->setNotifyInterval(1);
    m_player->play("rtsp://admin:123456@192.168.1.200:554/mpeg4");
}

void PlayerWindow::seek(int pos)
{
    if (!m_player->isPlaying())
        return;
    m_player->seek(pos*1000LL); // to msecs
}

void PlayerWindow::updateSlider()
{
    m_slider->setRange(0, int(m_player->duration()/1000LL));
    m_slider->setValue(int(m_player->position()/1000LL));
}

void PlayerWindow::updatePreview(const QImage &image)
{
    m_preview->setPixmap(QPixmap::fromImage(image).scaled(m_preview->size()));
}

void PlayerWindow::capture()
{
    //m_player->captureVideo();
    m_player->videoCapture()->capture();
}

void PlayerWindow::startRecord(){
//    recorder->moveToThread(&transcoderThread);
//    transcoderThread.start();
    recorder->start();
}

void PlayerWindow::stopRecord(){
    recorder->stop();
//    transcoderThread.quit();
//    transcoderThread.wait();
}

void PlayerWindow::onCaptureSaved(const QString &path)
{
    setWindowTitle(tr("saved to: ") + path);
}

void PlayerWindow::onCaptureError()
{
    QMessageBox::warning(0, QString::fromLatin1("QtAV video capture"), tr("Failed to capture video frame"));
}
