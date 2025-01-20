//
// Created by Tahira on 14.01.25.
//

#ifndef NONGUIMUSICPLAYER_H
#define NONGUIMUSICPLAYER_H

#include <SFML/Audio.hpp>
#include <QObject>
#include "Node.h"
#include <QTimer>
#include <QEventLoop>

class NonGUIMusicPlayer: public QObject {
    Q_OBJECT

    sf::Music music;
    bool loopS = false;
    bool LoopAll = false;
    bool play = false;

public slots:
    void loopPlaylist( Node *head, Node *current);
    void loopSong( Node *head, Node *current);
    void playSong(Node *n);
    void updatePlay(bool p);
    void updateLoopSong(bool l);
    void updateLoopAll(bool l);
};

#endif //NONGUIMUSICPLAYER_H
