#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QCryptographicHash>
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QResizeEvent>
#include <QSettings>
#include <QCloseEvent>
QT_BEGIN_NAMESPACE

namespace Ui { class player; }
QT_END_NAMESPACE

class player : public QMainWindow
{
    Q_OBJECT

public:
    player(QWidget *parent = nullptr);
    ~player();

private:
    QVideoWidget *videoWidget = new QVideoWidget(this);
    QMediaPlayer *mediaPLayer = new QMediaPlayer(videoWidget);
    QMediaPlaylist *playlist = new QMediaPlaylist(mediaPLayer);
    QStandardItemModel *playlistModel =new QStandardItemModel(this);
    void closeEvent(QCloseEvent* event);
    void saveTimings(const QMediaContent&);
    void removeTimings(const QMediaContent&);
    void setSettings(const QString&);
    void addToPlaylist(const QString&);
    void addMediaToPLaylist(const QUrl&);
    int getPosition(const QMediaContent&);
    Ui::player *ui;
};
#endif // PLAYER_H
