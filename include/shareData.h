#ifndef SHARE_H
#define SHARE_H

#include <unordered_map>
#include <glm/glm.hpp>

class ShareData
{
public:
    static unsigned int texImage;
    static unsigned int shaderID;
    static std::unordered_map<int, glm::vec2[6]> index;
    static void init();
    static glm::vec2 position[];
};

#endif
