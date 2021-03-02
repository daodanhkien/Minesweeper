#include <glad/glad.h>
#include <utils.h>
#include <shareData.h>

unsigned int ShareData::texImage = 0;
unsigned int ShareData::shaderID = 0;
std::unordered_map<int, glm::vec2[6]> ShareData::index;

glm::vec2 ShareData::position[] = 
{
    glm::vec2(-1.0f, 1.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, -1.0f),

    glm::vec2(-1.0f, 1.0f),
    glm::vec2(1.0f, -1.0f),
    glm::vec2(-1.0f, -1.0f)
};

void ShareData::init()
{
    ShareData::shaderID = LoadShaderFromFile("shader/shader.vs", "shader/shader.fs");
    ShareData::texImage = LoadTexture("D:/file/data.png");
    int count = 0;
    glm::vec2 index[] = 
    {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),

        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
    };
    glm::vec2 temp;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            temp = glm::vec2(j*0.2f, (1-i)*0.25f + 0.5f);
            for (int t = 0; t < 6; ++t) {
                ShareData::index[count][t] = temp + index[t]*glm::vec2(0.2f, 0.25f);
            }

            ++count;
        }
    }
    temp = glm::vec2(0.0f);
    for (int t = 0; t < 6; ++t) {
        ShareData::index[count][t] = temp + index[t]*glm::vec2(1.0f, 0.5f);
    }
}
