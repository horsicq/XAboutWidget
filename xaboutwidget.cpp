/* Copyright (c) 2022-2025 hors<horsicq@gmail.com>
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

#include "qjsonarray.h"
#include "qradiobutton.h"
#include "ui_xaboutwidget.h"
#include "../XUpdate/xupdate.h"

XAboutWidget::XAboutWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XAboutWidget)
{
    ui->setupUi(this);

    m_data = {};

    ui->labelDate->setText(__DATE__);
    ui->tabWidgetAbout->setCurrentIndex(0);
}

XAboutWidget::~XAboutWidget()
{
    delete ui;
}

void XAboutWidget::adjustView()
{
}

void XAboutWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
}

void XAboutWidget::setData(const DATA &data)
{
    m_data = data;

    ui->labelInfo->setText(data.sInfo);
    ui->labelLibraries->setText(data.sLibraries);

    QPixmap pixMap = QPixmap(data.sLogoPath);

    pixMap = pixMap.scaledToHeight(height());

    ui->labelLogo->setPixmap(pixMap);
}

void XAboutWidget::on_pushButtonCheckUpdates_clicked()
{
#ifdef QT_NETWORK_LIB
    QString apiUrl = "https://api.github.com/repos/horsicq/DIE-engine/releases";

    QNetworkAccessManager manager(this);
    QNetworkRequest request((QUrl(apiUrl)));
    QNetworkReply *pReply = manager.get(request);

    QEventLoop loop;
    QObject::connect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (pReply->error() == QNetworkReply::NoError) {
        QByteArray baData = pReply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(baData);
        QJsonArray releasesArray = jsonDoc.array();

        QString sLatestStableVersion;
        QString sLatestBetaVersion;

        for (const QJsonValue &val : releasesArray) {
            QJsonObject releaseObj = val.toObject();

            QString tagName = releaseObj["tag_name"].toString();
            bool isPrerelease = releaseObj["prerelease"].toBool();

            if (!isPrerelease && sLatestStableVersion.isEmpty())
                sLatestStableVersion = tagName;

            if (isPrerelease && sLatestBetaVersion.isEmpty())
                sLatestBetaVersion = tagName;

            if (!sLatestStableVersion.isEmpty() && !sLatestBetaVersion.isEmpty())
                break;
    if (m_data.sServerVersionLink != "") {
        QNetworkAccessManager manager(this);
        QNetworkRequest request(QUrl(m_data.sServerVersionLink));
        QNetworkReply *pReply = manager.get(request);
        QEventLoop loop;
        QObject::connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        if (pReply->error() == QNetworkReply::NoError) {
            if (pReply->bytesAvailable()) {
                QByteArray baData = pReply->readAll();
                QString sVersion = QString(baData.data());

                if (QCoreApplication::applicationVersion().toDouble() < sVersion.toDouble()) {
                    if (QMessageBox::information(this, tr("Update information"),
                                                 QString("%1\r\n\r\n%2\r\n\r\n%3").arg(tr("New version available"), sVersion, tr("Go to download page?")),
                                                 QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
                        QDesktopServices::openUrl(QUrl(m_data.sUpdatesLink));
                    }
                } else {
                    QMessageBox::information(this, tr("Update information"), tr("No update available"));
                }
            }
        } else {
            QMessageBox::critical(this, tr("Network error"), pReply->errorString());
        }

        if (sLatestStableVersion.isEmpty() && sLatestBetaVersion.isEmpty()) {
            QMessageBox::information(this, tr("Update Info"), tr("No updates found."));
            return;
        }

#ifdef Q_OS_WIN
        XUpdate *updater = new XUpdate(this);
        updater->showVersionSelectionDialog(sLatestStableVersion, sLatestBetaVersion);
#endif

    } else {
        QMessageBox::critical(this, tr("Network Error"), pReply->errorString());
        QDesktopServices::openUrl(QUrl(m_data.sUpdatesLink));
    }

    pReply->deleteLater();
#else
    QDesktopServices::openUrl(QUrl("https://github.com/horsicq/DIE-engine/releases"));
    QDesktopServices::openUrl(QUrl(m_data.sUpdatesLink));
#endif
}




void XAboutWidget::on_labelInfo_linkActivated(const QString &sLink)
{
    if (sLink.startsWith("http", Qt::CaseInsensitive)) {
        QDesktopServices::openUrl(sLink);
    } else {
        QApplication::clipboard()->setText(sLink);

        QMessageBox::information(this, tr("Information"), tr("The value copied to clipboard"));
    }
}

void XAboutWidget::on_pushButtonFollowGithub_clicked()
{
    QDesktopServices::openUrl(QUrl(QString("https://github.com/horsicq")));
}

void XAboutWidget::on_pushButtonFollowTwitter_clicked()
{
    QDesktopServices::openUrl(QUrl(QString("https://twitter.com/horsicq")));
}

void XAboutWidget::on_pushButtonFollowYoutube_clicked()
{
    QDesktopServices::openUrl(QUrl(QString("https://www.youtube.com/@funreverseengineering")));
}

void XAboutWidget::on_pushButtonThanks_clicked()
{
    QDesktopServices::openUrl(QUrl(m_data.sThanksLink));
}

void XAboutWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
