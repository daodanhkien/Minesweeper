#include <glad/glad.h>
#include <dataMap.h>
#include <utils.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
#include <shareData.h>
#include <iostream>

using namespace std;

DataMap::DataMap()
{
    this->map = NULL;
    this->mark = NULL;
    this->height = 0;
    this->width = 0;
    glGenVertexArrays(1, &VAO);
    this->VBO = 0;
    this->shaderID = ShareData::shaderID;
}

void DataMap::update(int x, int y)
{
    if (x < 0 || x >= height || y < 0 || y >= width || this->map[x][y] == 9 || this->mark[x][y] == 0xFF) return;
    this->mark[x][y] = 0xFF;
    --unmark;
    glBufferSubData(GL_ARRAY_BUFFER, (width*height + x*width + y)*6*sizeof(glm::vec2), 6*sizeof(glm::vec2), &ShareData::index[map[x][y]][0]);
    if (this->map[x][y] == 0) {
        update(x-1, y);
        update(x+1, y);
        update(x, y+1);
        update(x, y-1);
    }
}

void DataMap::choose(int x, int y)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (this->map[x][y] == 9) {
        this->status = -1;
        return;
    }
    update(x, y);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DataMap::createMap(int w, int h)
{
    this->status = 0;
    if (glIsBuffer(VBO)) {
        glDeleteBuffers(1, &VBO);
    }

    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glm::vec2 *data = new glm::vec2[w*h*6*2];

    int count = 0;
    this->map = new unsigned char*[h];
    this->mark = new unsigned char*[h];
    for (int i = 0; i < h; ++i) {
        this->map[i] = new unsigned char[w];
        this->mark[i] = new unsigned char[w];
        for (int y = 0; y < w; ++y) {
            glm::vec2 temp = 20.0f * glm::vec2(y+0.5f, (h-1-i)+0.5f);
            for (int i = 0; i < 6; ++i) {
                data[count++] = temp + ShareData::position[i] * glm::vec2(9.0f);
            }
        }
        memset(this->map[i], 0x00, w);
        memset(this->mark[i], 0x00, w);
    }
    this->sizeBoom = w*h/5;
    this->unmark = w*h;
    for (int t = 0; t < sizeBoom; ++t) {
        int x = rand()%(h*w);
        int y = x%w;
        x /= w;
        if (map[x][y] == 9) {
            --t;
            continue;
        }

        map[x][y] = 9;
        if (x == 0) {
            if (map[x+1][y] != 9) ++map[x+1][y];
            if (y != 0) {
                if (map[x+1][y-1] != 9) ++map[x+1][y-1];
                if (map[x][y-1] != 9) ++map[x][y-1];
            }
            if (y != w-1) {
                if (map[x+1][y+1] != 9) ++map[x+1][y+1];
                if (map[x][y+1] != 9) ++map[x][y+1];
            }
        } else if (x == h-1) {
            if (map[x-1][y] != 9) ++map[x-1][y];
            if (y != 0) {
                if (map[x-1][y-1] != 9) ++map[x-1][y-1];
                if (map[x][y-1] != 9) ++map[x][y-1];
            }
            if (y != w-1) {
                if (map[x-1][y+1] != 9) ++map[x-1][y+1];
                if (map[x][y+1] != 9) ++map[x][y+1];
            }
        } else {
            if (map[x+1][y] != 9) ++map[x+1][y];
            if (map[x-1][y] != 9) ++map[x-1][y];

            if (y != 0) {
                if (map[x+1][y-1] != 9) ++map[x+1][y-1];
                if (map[x-1][y-1] != 9) ++map[x-1][y-1];
                if (map[x][y-1] != 9) ++map[x][y-1];
            }
            if (y != w-1) {
                if (map[x+1][y+1] != 9) ++map[x+1][y+1];
                if (map[x-1][y+1] != 9) ++map[x-1][y+1];
                if (map[x][y+1] != 9) ++map[x][y+1];
            }
        }
    }

    for (int x = 0; x < h; ++x) {
        for (int y = 0; y < w; ++y) {
            for (int i = 0; i < 6; ++i) {
                data[count++] = glm::vec2(-1.0f);
            }
        }
    }

    glm::mat4 ortho = glm::ortho(0.0f, w*20.0f, 0.0f, h*20.0f, -10.0f, 10.0f);
    glUseProgram(shaderID);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "matrix"), 1, GL_FALSE, glm::value_ptr(ortho));

    glBufferData(GL_ARRAY_BUFFER, 2*w*h*6*sizeof(glm::vec2), data, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(w*h*6*sizeof(glm::vec2)));

    glBindVertexArray(0);

    delete[] data;

    this->width = w;
    this->height = h;
}

DataMap::~DataMap()
{
    if (this->map != NULL) delete[] this->map;
    if (this->mark != NULL) delete[] this->mark;
}

void DataMap::free()
{
    if (this->map != NULL) delete[] this->map;
    if (this->mark != NULL) delete[] this->mark;
    if (glIsVertexArray(VAO)) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (glIsBuffer(VBO)) {
        glDeleteBuffers(1, &VBO);
    }
    if (glIsProgram(shaderID)) {
        glDeleteProgram(shaderID);
    }

    this->map = NULL;
    this->mark = NULL;
    this->VAO = 0;
    this->VBO = 0;
    this->shaderID = 0;
}

void DataMap::render()
{
    glUseProgram(shaderID);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, ShareData::texImage);
    glDrawArrays(GL_TRIANGLES, 0, 6*width*height);
    glBindVertexArray(0);
}

void DataMap::flag(int x, int y)
{
    if (this->mark[x][y] != 0xFF) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, (width*height + x*width + y)*6*sizeof(glm::vec2), 6*sizeof(glm::vec2), &ShareData::index[10][0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

int DataMap::getStatus()
{
    if (status == -1) return -1;
    else return (sizeBoom == unmark);
}

void DataMap::reCreate()
{
    this->createMap(width, height);
}
