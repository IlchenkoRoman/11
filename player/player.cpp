#include "player.h"
#include "ui_player.h"

player::player(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::player)
{
    ui->setupUi(this);
    this->setFixedSize(955, 490);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    mediaPLayer->setVolume(ui->Volume->value());
    ui->playlsittable->setModel(playlistModel);
    ui->playlsittable->verticalHeader()->setVisible(false);
    ui->playlsittable->horizontalHeader()->setVisible(false);
    ui->playlsittable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->playlsittable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->playlsittable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->playlsittable->horizontalHeader()->setStretchLastSection(true);


    mediaPLayer->setPlaylist(playlist);
    mediaPLayer->setVideoOutput(videoWidget);
    ui->videolayer->insertWidget(50,videoWidget);

    setSettings("timings");
    connect(ui->SelectVideo,&QPushButton::pressed,[&](){
        QString filename = QFileDialog::getOpenFileName(this,"File");
        if(filename.isEmpty()){
            return;
        }
        addMediaToPLaylist(QUrl(filename));
        addToPlaylist(filename);
        ui->Play->click();
        videoWidget->show();
        mediaPLayer->setVolume(10);

    });
    connect(ui->Play,&QPushButton::pressed,[&](){
        if(ui->Play->text() == "Play"){
        mediaPLayer->play();
        ui->Play->setText("Pause");
        } else {
            mediaPLayer->pause();
            ui->Play->setText("Play");
        }

    });
    connect(ui->Volume,&QSlider::valueChanged,[&](const int& value){
        mediaPLayer->setVolume(value);
    });
    connect(mediaPLayer,&QMediaPlayer::positionChanged,[&](const qint64& position){
        ui->Video->setValue(position);
    });
    connect(ui->Video,&QSlider::sliderMoved,[&](const int& value){
        mediaPLayer->setPosition(value);
    });
    connect(mediaPLayer,&QMediaPlayer::durationChanged,[&](const qint64& duration){
        ui->Video->setMaximum(duration);
    });
    connect(ui->prev, &QPushButton::pressed, [&](){
        saveTimings(playlist->currentMedia());
        playlist->previous();
        mediaPLayer->setPosition(getPosition(playlist->currentMedia()));
    });
    connect(ui->next, &QPushButton::pressed, [&](){
        saveTimings(playlist->currentMedia());
        playlist->next();
        mediaPLayer->setPosition(getPosition(playlist->currentMedia()));
    });
    connect(ui->Remove,&QPushButton::pressed,[&](){
        removeTimings(playlist->currentMedia());
        playlistModel->removeRow(playlist->currentIndex());
        playlist->removeMedia(playlist->currentIndex());

    });
    connect(ui->playlsittable, &QTableView::doubleClicked, [&](const QModelIndex &index){
        saveTimings(playlist->currentMedia());
        playlist->setCurrentIndex(index.row());
        mediaPLayer->setPosition(getPosition(playlist->currentMedia()));
        ui->Play->click();
    });
    connect(mediaPLayer,&QMediaPlayer::currentMediaChanged,[&](const QMediaContent &content){
        mediaPLayer->setPosition(getPosition(content));
    });
}

player::~player()
{
    delete ui;
}

void player::saveTimings(const QMediaContent& content){
    QSettings timings("Org","App");
    timings.beginGroup("timings");
    timings.setValue(content.request().url().toString(),ui->Video->value());
    timings.endGroup();
}
void player::removeTimings(const QMediaContent& content){
    QSettings timings("Org","App");
    timings.beginGroup("timings");
    timings.remove(content.request().url().toString());
    timings.endGroup();
}

void player::setSettings(const QString& group){
    QSettings timings("Org","App");
    if(group != "timings"){return;}
    timings.beginGroup(group);
    for (const auto& item : timings.allKeys()){
        addMediaToPLaylist(QUrl(item));
        addToPlaylist(item);
    }
    timings.endGroup();
}
void player::closeEvent(QCloseEvent *event){
    saveTimings(playlist->currentMedia());
}

void player::addToPlaylist(const QString& filename){
    playlistModel->appendRow(new QStandardItem(QDir(filename).dirName()));
}
void player::addMediaToPLaylist(const QUrl& filename){
    playlist->addMedia(filename);
}

int player::getPosition(const QMediaContent& content){
    QSettings timings("Org","App");
    timings.beginGroup("timings");
    auto smth = timings.value(content.request().url().toString()).toInt();
    timings.endGroup();
    return smth;
}
