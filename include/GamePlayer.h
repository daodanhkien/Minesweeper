#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <dataMap.h>
#include <player.h>
#include <Human.h>

class GamePlayer
{
private:
    Player *player;
    DataMap map;
public:
    GamePlayer(/* args */);
    ~GamePlayer();
    void update() {
        int status = player->update();
        if (status != 0) {
            if (status == 1) map.choose(player->getX(), player->getY());
            else if (status == 2) map.flag(player->getX(), player->getY());
        }
        if (map.getStatus() != 0) {
            if (map.getStatus() == -1) cout << "lose" << endl;
            else if (map.getStatus() == 1) cout << "win" << endl;
            
            map.reCreate();
        }
    }
    void render() {
        map.render();
    }
    void setSizeMap(int w, int h) {
        map.createMap(w, h);
        player->setWH(w, h);
    }
};

GamePlayer::GamePlayer(/* args */)
{
    player = new Human();
}

GamePlayer::~GamePlayer()
{
}


#endif
