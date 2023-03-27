/* Copyright (c) 2022-2023 hors<horsicq@gmail.com>
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
#ifndef XABOUTWIDGET_H
#define XABOUTWIDGET_H

#include <QClipboard>
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QUrl>
#include <QWidget>
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QRandomGenerator>
#elif (QT_VERSION_MAJOR >= 6)
#include <QRandomGenerator>
#endif
#ifdef QT_NETWORK_LIB
#include <QNetworkAccessManager>
#include <QNetworkReply>
#endif
// TODO XGitHub API

namespace Ui {
class XAboutWidget;
}

class XAboutWidget : public QWidget {
    Q_OBJECT

public:
    struct THANKS_RECORD {
        QString sName;
        QString sAvatar;
        QString sWebsite;
        QString sGithub;
        QString sTwitter;
        // TODO mb more
    };

    struct DATA {
        QString sInfo;
        QString sUpdatesLink;
        QString sServerVersionLink;
        QString sLibraries;
        QString sLogoPath;
        QString sThanksPath;
    };

    explicit XAboutWidget(QWidget *pParent = nullptr);
    ~XAboutWidget();

    void setData(DATA data);

private:
    void randomImage();

private slots:
    void on_pushButtonCheckUpdates_clicked();
    void on_toolButtonAvatar_clicked();
    void on_pushButtonWebsite_clicked();
    void on_pushButtonGithub_clicked();
    void on_pushButtonTwitter_clicked();
    void on_listWidgetThanks_currentItemChanged(QListWidgetItem *pItemCurrent, QListWidgetItem *pItemPrevious);
    void on_labelInfo_linkActivated(const QString &sLink);
    THANKS_RECORD getThanksRecord(QString sFileName);

private:
    Ui::XAboutWidget *ui;
    DATA g_data;
    THANKS_RECORD g_thanksRecordCurrent;
};

#endif  // XABOUTWIDGET_H
