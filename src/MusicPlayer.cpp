//
// Created by Tahira on 09.01.25.
//

#include "MusicPlayer.h"
#include "MusicPlayer.h"
#include "./ui_musicplayer.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    tail=nullptr;
    head=nullptr;
    current=nullptr;
    LoopAll = false;
    LoopSong = false;
    play = false;

    thread = new QThread();
    mp = new NonGUIMusicPlayer();
    mp->moveToThread(thread);

    connect(this, &MusicPlayer::playPause, mp, &NonGUIMusicPlayer::playSong);
    connect(this, &MusicPlayer::loopAll, mp, &NonGUIMusicPlayer::loopPlaylist);
    connect(this, &MusicPlayer::loop, mp, &NonGUIMusicPlayer::loopSong);
    connect(this, &MusicPlayer::playStatus, mp, &NonGUIMusicPlayer::updatePlay, Qt::QueuedConnection);

    connect(ui->add, &QPushButton::clicked, this, [this]() {
        string title = ui->Title->toPlainText().toStdString();
        string path = ui->filePath->toPlainText().toStdString();
        int pos = ui->Position->toPlainText().toInt();
        addSong(title, path, pos);
    });

    connect(ui->remove, &QPushButton::clicked, this, [this]() {
        string title = ui->Title->toPlainText().toStdString();
        deleteSong(title);
    });

    connect(ui->play, &QPushButton::clicked, this, [this]() {
        play = !play;
        emit playStatus(play);
        string title = ui->Title->toPlainText().toStdString();
        if (play) {
            ui->play->setStyleSheet("background-color: blue; color: white");
            playPauseSong(title);
        }
        else {
            ui->play->setStyleSheet("background-color: none; color: white");
            playPauseSong(title);
        }
    });

    connect(ui->prev, &QPushButton::clicked, this, &MusicPlayer::PrevSong);
    connect(ui->next, &QPushButton::clicked, this, &MusicPlayer::NextSong);

    connect(this, &MusicPlayer::loopAllStatus, mp, &NonGUIMusicPlayer::updateLoopAll, Qt::QueuedConnection);
    connect(ui->loopAll, &QPushButton::clicked, this, [this]() {
        LoopAll = !LoopAll;
        emit loopAllStatus(LoopAll);
        if (!LoopAll) {
            ui->loopAll->setStyleSheet("background-color: none; color: white");
        }
        else {
            ui->loopAll->setStyleSheet("background-color: blue;color: white");
        }
        loopPlaylist();
    });

    connect(this, &MusicPlayer::loopSongStatus, mp, &NonGUIMusicPlayer::updateLoopSong, Qt::QueuedConnection);
    connect(ui->loop, &QPushButton::clicked, this, [this]() {
        LoopSong = !LoopSong;
        emit loopSongStatus(LoopSong);
        if (!LoopSong) {
            ui->loop->setStyleSheet("background-color: none; color: white");
        }
        else {
            ui->loop->setStyleSheet("background-color: blue;color: white");
        }
        loopSong();
    });

    connect(ui->display, &QPushButton::clicked, this, &MusicPlayer::displayPlaylist);

    thread->start();
}

MusicPlayer::~MusicPlayer()
{
    thread->quit();
    thread->wait();
    delete thread;
    thread=nullptr;
    delete ui;
    Node *temp = head;
    head->prev = nullptr;
    tail->next = nullptr;
    while (temp != nullptr) {
        head = temp->next;
        temp->next = nullptr;
        temp->prev = nullptr;
        delete temp;
        temp=head;
    }
    tail=nullptr;
}

void MusicPlayer::addSong(const string& title, const string& path, const int pos) {
    if (title.empty() || path.empty())
        return;
    if(map_songs.find(title) != map_songs.end())
        return;
    Node *n = new Node;
    n->songTitle = title;
    n->filePath = path;
    // pos not given->end
    if(pos <= 0 ) {
        insertAtEnd(n);
    }
    // pos given
    //->start
    else if (pos == 1 ) {
        insertAtStart(n);
    }
    //->middle/end
    else {
        insertAtPos(n, pos);
    }
    map_songs[title] = n;
}

void MusicPlayer::deleteSong(const string& title) {
    if (title.empty())
        return;
    Node *d = nullptr;
    if (map_songs.find(title) != map_songs.end()) {
        d = map_songs[title];
    }
    else {
        return;
    }
    map_songs.erase(title);
    if (head == d) {
        deleteAtStart(d);
    }
    else if (tail == d) {
        deleteAtEnd(d);
    }
    else {
        deleteAtPos(d);
    }
}

void MusicPlayer::NextSong() {
    if (current == nullptr) {
        current = head;
    }
    else {
        current = current->next;
    }
}

void MusicPlayer::PrevSong() {
    if (current == nullptr) {
        current = head;
    }
    else {
        current = current->prev;
    }
}

void MusicPlayer::playPauseSong(const string& title) {
    if (title.empty()) {
        return;
    }
    Node *n = nullptr;
    if (map_songs.find(title) != map_songs.end()) {
        n = map_songs[title];
        emit playPause(n);
    }
}

void MusicPlayer::loopPlaylist() {
    if (current == nullptr) {
        current = head;
    }
    emit loopAll(head, current);
}

void MusicPlayer::loopSong() {
    if (current == nullptr) {
        current = head;
    }
    emit loop( head, current);
}

void MusicPlayer::displayPlaylist() {
    Node *temp = head;
    string songs;
    string bullet = "\u2022";
    if((head == tail) & (head != nullptr)) {
        songs += bullet + temp->songTitle + "\n";
    }
    while (temp != tail) {
        songs += bullet + temp->songTitle + "\n";
        temp = temp->next;
    }
    if ((temp == tail) && (head != tail)) {
        songs += bullet + temp->songTitle + "\n";
    }
    ui->screen->setText(QString::fromStdString(songs));
}

void MusicPlayer::insertAtStart(Node *n) {
    // empty before
    if(head == nullptr) {
        head = n;
        tail = n;
        n->next =n;
        n->prev =n;
    }
    // more nodes already
    else {
        n->next = head;
        head->prev = n;
        head = n;
        tail->next = head;
        head->prev = tail;
    }
}

void MusicPlayer::insertAtEnd(Node *n) {
    //empty
    if(head == nullptr) {
        insertAtStart(n);
    }
    //list of nodes already
    else {
        n->prev =tail;
        tail->next = n;
        tail = n;
        tail->next = head;
        head->prev = tail;
    }
}

void MusicPlayer::insertAtPos(Node *n, int pos) {
    int count = 1;
    Node *temp =head;
    while ((count != pos) && (temp != tail)) {
        temp = temp->next;
        count++;
    }
    if (count != pos) {
        insertAtEnd(n);
        return;
    }
    n->prev = temp->prev;
    temp->prev->next = n;
    temp->prev = n;
    n->next = temp;
}

void MusicPlayer::deleteAtStart(Node *d) {
    if (head == nullptr) {
        return;
    }
    else if (head == tail) {
        head = nullptr;
        tail = nullptr;
    }
    else {
        head = head->next;
        head->prev = tail;
        tail->prev = head;
    }
    d->next = nullptr;
    d->prev = nullptr;
    delete d;
    d = nullptr;
}

void MusicPlayer::deleteAtEnd(Node *d) {
    if (tail == nullptr) {
        return;
    }
    else if (head == tail) {
        deleteAtStart(d);
    }
    else {
        tail = tail->prev;
        tail->next = head;
        head->prev = tail;
        d->prev = nullptr;
        d->next = nullptr;
        delete d;
        d = nullptr;
    }
}

void MusicPlayer::deleteAtPos(Node *d){
    d->prev->next = d->next;
    d->next->prev = d->prev;
    d->prev = nullptr;
    d->next = nullptr;
    delete d;
    d = nullptr;
}

