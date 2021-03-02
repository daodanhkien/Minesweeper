#ifndef HUMAN_H
#define HUMAN_H

#include <GLFW/glfw3.h>
#include <player.h>
#include <iostream>

using namespace std;

class Human : public Player
{
private:
    GLFWwindow* window;

    int width, height;
public:
    Human(/* args */);
    ~Human();
    virtual int update() {
        static double lastActive = glfwGetTime();
        if (glfwGetTime() - lastActive >= 0.25) {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                this->x = y*mapWidth/width;
                this->y = x*mapHeight/height;
                lastActive = glfwGetTime();
                return 1;
            }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                this->x = y*mapWidth/width;
                this->y = x*mapHeight/height;
                lastActive = glfwGetTime();
                return 2;
            }
        }
        

        return 0;
    }
};

Human::Human(/* args */) : Player()
{
    this->window = glfwGetCurrentContext();
    glfwGetWindowSize(window, &width, &height);
}

Human::~Human()
{
}


#endif
