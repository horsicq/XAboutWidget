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
}

void XAboutWidget::on_pushButtonCheckUpdates_clicked()
{
    QDesktopServices::openUrl(QUrl(g_data.sUpdatesLink));
}
