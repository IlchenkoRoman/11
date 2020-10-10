#include "Widget.h"
#include "ui_Widget.h"

#include <QFileDialog>
#include <QStandardPaths>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    // Подключаемся к слотам
    connect(ui->downloadPushButton, &QPushButton::clicked, this, &Widget::onDownloadButtonClicked);
    connect(ui->selectTargetFolderPushButton, &QPushButton::clicked, this, &Widget::onSelectTargetFolderButtonClicked);
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &Widget::onCancelButtonClicked);
    connect(&m_downloader, &Downloader::updateDownloadProgress, this, &Widget::onUpdateProgress);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onDownloadButtonClicked()
{
    // Запускаем скачивание файла передавая в качестве аргументов
    // путь к каталогу, куда будем закачивать файлы
    // url, где находится файл
    m_downloader.get(ui->targetFolderLineEdit->text(), ui->urlLineEdit->text());
}

void Widget::onSelectTargetFolderButtonClicked()
{
    // Выбор целевого каталога для скачивания
    QString targetFolder = QFileDialog::getExistingDirectory(this,
                                                             tr("Select folder"),
                                                             QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
                                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->targetFolderLineEdit->setText(targetFolder);
}

void Widget::onCancelButtonClicked()
{
    // Отмена загрузки
    m_downloader.cancelDownload();
    ui->downloadProgressBar->setMaximum(100);
    ui->downloadProgressBar->setValue(0);
}

void Widget::onUpdateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    // Обновляем прогресс загрузки
    ui->downloadProgressBar->setMaximum(bytesTotal);
    ui->downloadProgressBar->setValue(bytesReceived);
}
