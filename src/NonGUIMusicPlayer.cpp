//
// Created by Tahira on 14.01.25.
//

#include "NonGUIMusicPlayer.h"

#include "MusicPlayer.h"

void NonGUIMusicPlayer::playSong(Node *n) {
    string path = n->filePath;
    if (!music.openFromFile(path)) { //to do: dialog box error
        return;
    }
    if (play) {
        music.play();
    }
    else {
        music.stop(); //To Do:later fix pause play mechanism
    }
}

void NonGUIMusicPlayer::loopPlaylist(Node *head, Node *current){
    QEventLoop loop;
    while (LoopAll && (head != nullptr)) {
        if (music.getStatus() != sf::Music::Status::Playing) {
            playSong(current);
            current = current->next;
        }
        QTimer::singleShot(1000, &loop, &QEventLoop::quit);
        loop.exec();
    }
}

void NonGUIMusicPlayer::loopSong(Node *head, Node *current){
    QEventLoop loop;
    while (loopS && (head != nullptr)) {
        if (music.getStatus() != sf::Music::Status::Playing) {
            playSong(current);
        }
        QTimer::singleShot(1000, &loop, &QEventLoop::quit);
        loop.exec();
    }
}

void NonGUIMusicPlayer::updatePlay(bool p) {
    play = p;
}

void NonGUIMusicPlayer::updateLoopSong(bool l) {
    loopS = l;
}

void NonGUIMusicPlayer::updateLoopAll(bool l) {
    LoopAll = l;
}
