/* Copyright (c) 2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "xaboutwidget.h"
#include "ui_xaboutwidget.h"

XAboutWidget::XAboutWidget(QWidget *pParent) :
    QWidget(pParent),
    ui(new Ui::XAboutWidget)
{
    ui->setupUi(this);

    ui->tabWidgetAbout->setCurrentIndex(0);
}

XAboutWidget::~XAboutWidget()
{
    delete ui;
}

void XAboutWidget::setData(DATA data)
{
    g_data=data;

    ui->labelInfo->setText(data.sInfo);
    ui->labelLibraries->setText(data.sLibraries);

    QPixmap pixMap=QPixmap(data.sLogoPath);

    pixMap=pixMap.scaledToHeight(height());

    ui->labelLogo->setPixmap(pixMap);

    QList<QFileInfo> listFileInfos=QDir(data.sThanksPath).entryInfoList(QStringList()<<"*.json",QDir::Files);

    qint32 nNumberOfFiles=listFileInfos.count();

    {
    #if QT_VERSION >= 0x050300
        const QSignalBlocker blocker1(ui->listWidgetThanks);
    #else
        const bool bBlocked1=ui->listWidgetThanks->blockSignals(true);
    #endif
        for(qint32 i=0;i<nNumberOfFiles;i++)
        {
            QString sFileName=listFileInfos.at(i).absoluteFilePath();

            THANKS_RECORD thanksRecord=getThanksRecord(sFileName);

            QListWidgetItem *pItem=new QListWidgetItem;
            pItem->setText(thanksRecord.sName);
            pItem->setData(Qt::UserRole,sFileName);

            ui->listWidgetThanks->insertItem(i,pItem);
        }
    #if QT_VERSION < 0x050300
        ui->listWidgetThanks->blockSignals(bBlocked1);
    #endif
    }

    random();
}

void XAboutWidget::random()
{
    qint32 nNumberOfFiles=ui->listWidgetThanks->count();

    if(nNumberOfFiles)
    {
        quint16 nRandom=0;

    #if QT_VERSION >=QT_VERSION_CHECK(5,10,0)
        nRandom=(quint16)(QRandomGenerator::global()->generate());
    #elif (QT_VERSION_MAJOR>=6)
        nRandom=(quint16)(QRandomGenerator::global()->generate());
    #else
        static quint32 nSeed=0;

        if(!nSeed)
        {
            quint32 nRValue=QDateTime::currentMSecsSinceEpoch()&0xFFFFFFFF;

            nSeed^=nRValue;
            qsrand(nSeed);
        }
        nRandom=(quint16)qrand();
    #endif

        ui->listWidgetThanks->setCurrentRow(nRandom%nNumberOfFiles);
    }
}

void XAboutWidget::on_pushButtonCheckUpdates_clicked()
{
    QDesktopServices::openUrl(QUrl(g_data.sUpdatesLink));
}

void XAboutWidget::on_pushButtonAvatar_clicked()
{
    random();
}

void XAboutWidget::on_pushButtonWebsite_clicked()
{
    QDesktopServices::openUrl(QUrl(g_thanksRecordCurrent.sWebsite));
}

void XAboutWidget::on_pushButtonGithub_clicked()
{
    QString sLink=QString("https://github.com/%1").arg(g_thanksRecordCurrent.sGithub);

    QDesktopServices::openUrl(QUrl(sLink));
}

void XAboutWidget::on_pushButtonTwitter_clicked()
{
    QString sLink=QString("https://twitter.com/%1").arg(g_thanksRecordCurrent.sTwitter);

    QDesktopServices::openUrl(QUrl(sLink));
}

XAboutWidget::THANKS_RECORD XAboutWidget::getThanksRecord(QString sFileName)
{
    THANKS_RECORD result={};

    QFile file;
    file.setFileName(sFileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QJsonDocument jsDoc=QJsonDocument::fromJson(QString(file.readAll()).toUtf8());

        if(jsDoc.isObject())
        {
            result.sName=jsDoc.object()["data"].toObject()["name"].toString();
            result.sWebsite=jsDoc.object()["data"].toObject()["website"].toString();
            result.sTwitter=jsDoc.object()["data"].toObject()["twitter"].toString();
            result.sGithub=jsDoc.object()["data"].toObject()["github"].toString();

            result.sAvatar=jsDoc.object()["data"].toObject()["avatar"].toString();

            if(result.sAvatar!="")
            {
                result.sAvatar=QFileInfo(sFileName).absolutePath()+QDir::separator()+result.sAvatar;
            }

            ui->pushButtonWebsite->setEnabled(result.sWebsite!="");
            ui->pushButtonGithub->setEnabled(result.sGithub!="");
            ui->pushButtonTwitter->setEnabled(result.sTwitter!="");
        }

        file.close();
    }

    return result;
}

void XAboutWidget::on_listWidgetThanks_currentItemChanged(QListWidgetItem *pCurrent,QListWidgetItem *pPrevious)
{
    Q_UNUSED(pPrevious)

    if(pCurrent)
    {
        QString sFilePath=pCurrent->data(Qt::UserRole).toString();

        g_thanksRecordCurrent=getThanksRecord(sFilePath);

        QPixmap pixmap(g_thanksRecordCurrent.sAvatar);
        QIcon buttonIcon(pixmap);
        ui->pushButtonAvatar->setIcon(buttonIcon);
        ui->pushButtonAvatar->setIconSize(pixmap.rect().size());
    }
}
