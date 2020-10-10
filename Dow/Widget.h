#ifndef WIDGET_H
#define WIDGET_H

#include "Downloader.h"

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    // Слот для старта загрузки
    void onDownloadButtonClicked();

    // Слот для выбора каталога для скачивания
    void onSelectTargetFolderButtonClicked();

    // Слот для отмены загрузки
    void onCancelButtonClicked();

    // Слот для обновления прогресса загрузки
    void onUpdateProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    Ui::Widget *ui;
    Downloader m_downloader; // Класс для скачивания
};

#endif // WIDGET_H
