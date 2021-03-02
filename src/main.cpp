#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <utils.h>
#include <dataMap.h>
#include <GamePlayer.h>
#include <ctime>

using namespace std;

void processInput(GLFWwindow* window);

int main(int argc, char* argv[])
{
    init();
    srand(time(NULL));

    GLFWwindow* window = glfwGetCurrentContext();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glfwSwapInterval(1);
    GamePlayer game;
    game.setSizeMap(10, 10);

    while ( !glfwWindowShouldClose(window) ) {
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        game.update();
        game.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
