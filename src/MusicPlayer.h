//
// Created by Tahira on 05.01.25.
//

#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
#include <unordered_map>
#include <SFML/Audio.hpp>
#include <QThread>
#include "Node.h"
#include "NonGUIMusicPlayer.h"
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MusicPlayer;
}
QT_END_NAMESPACE

class MusicPlayer : public QMainWindow
{
    Q_OBJECT

signals:
    void playPause(Node *n);
    void loopAll(Node *head, Node *current);
    void loop( Node *head, Node *current);
    void playStatus(bool p);
    void loopAllStatus(bool l);
    void loopSongStatus(bool l);

public:

    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();

private slots:
    void addSong(const std::string& title, const std::string& path, int pos = -1);
    void deleteSong(const std::string& title);
    void playPauseSong(const std::string& title);
    void displayPlaylist();
    void NextSong();
    void PrevSong();
    void loopPlaylist();
    void loopSong();

private:
    NonGUIMusicPlayer *mp;
    Ui::MusicPlayer *ui;
    unordered_map<string, Node *> map_songs;
    Node *head;
    Node *tail;
    Node *current;
    bool LoopAll;
    bool LoopSong;
    bool play;
    QThread *thread;

    void insertAtStart(Node *n);
    void insertAtEnd(Node *n);
    void insertAtPos(Node *n, int pos);

    void deleteAtStart(Node *d);

    void deleteAtEnd(Node *d);

    void deleteAtPos(Node *d);

    void playSong(Node *n);
};

#endif //MUSICPLAYER_H
