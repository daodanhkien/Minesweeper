#ifndef PLAYER_H
#define PLAYER_H

class Player
{
protected:
    /* data */
    int x;
    int y;

    int mapWidth;
    int mapHeight;
public:
    Player();
    ~Player();
    int getX() {
        return this->x;
    }
    int getY() {
        return this->y;
    }
    virtual int update() = 0;

    void setWH(int width, int height) {
        this->mapWidth = width;
        this->mapHeight = height;
    }
};

Player::Player()
{
    this->x = 0;
    this->y = 0;
    this->mapWidth = 0;
    this->mapHeight = 0;
}

Player::~Player()
{
}

#endif
