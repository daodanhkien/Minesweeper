#ifndef DATA_MAP_H
#define DATA_MAP_H

class DataMap
{
private:
    unsigned char** map;
    unsigned char** mark;
    unsigned int VAO, VBO;
    unsigned int shaderID;
    int width, height;

    void update(int x, int y);
    int sizeBoom;
    int unmark;
    int status;

public:
    DataMap();
    ~DataMap();

    void choose(int x, int y);
    void flag(int x, int y);
    void render();
    void createMap(int w, int h);
    void free();
    int getStatus();
    void reCreate();
};

#endif
