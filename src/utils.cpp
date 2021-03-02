#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fstream>
#include <sstream>
#include <shareData.h>

using namespace std;

void init()
{
    const int screenWidth = 800;
    const int screenHeight = 800;

    if (glfwInit() != GLFW_TRUE) {
        cerr << "Error to init GLFW" << endl;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Minesweeper", NULL, NULL);
    if (window == NULL) {
        cerr << "Error to create GLFW window" << endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != true) {
        cerr << "Error to load OpenGL context" << endl;
        glfwTerminate();
    }

    ShareData::init();
    stbi_set_flip_vertically_on_load(true);
}

unsigned int LoadTexture(const char* name, unsigned int toType)
{
    unsigned int id = 0;
    int w, h, chanel;
    unsigned char* data = stbi_load(name, &w, &h, &chanel, 0);
    if (data != NULL) {
        if (chanel == 4) chanel = GL_RGBA;
        else if (chanel == 3) chanel = GL_RGB;
        else if (chanel == 2) chanel = GL_RG;
        else chanel = GL_RED;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, toType, w, h, 0, chanel, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        float color[] = {0.3f, 0.3f, 0.3f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    stbi_image_free(data);

    return id;
}

void ShaderLog(unsigned int id)
{
    int maxLength;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
    char *log = new char[maxLength];
    int logLength;
    glGetShaderInfoLog(id, maxLength, &logLength, log);
    if (logLength > 0) {
        glGetShaderiv(id, GL_SHADER_TYPE, &logLength);
        switch (logLength) {
            case GL_VERTEX_SHADER:
                cerr << "Vertex shader error" << endl;
                break;
            case GL_GEOMETRY_SHADER:
                cerr << "Geometry shader error" << endl;
                break;
            case GL_FRAGMENT_SHADER:
                cerr << "Fragment shader error" << endl;
                break;
            default:
                cerr << "Not detect type shader" << endl;
        }
        cerr << log << endl;
    }

    delete[] log;
}

void ProgramLog(unsigned int id)
{
    int maxLength;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
    char* log = new char[maxLength];
    int logLength;
    glGetProgramInfoLog(id, maxLength, &logLength, log);
    if (logLength > 0) {
        cerr << log << endl;
    }

    delete[] log;
}

unsigned int CreateShaderFromString(const char* string, GLenum type)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &string, NULL);
    glCompileShader(id);
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success != true) {
        ShaderLog(id);
        glDeleteShader(id);
        id = 0;
    }

    return id;
}

unsigned int CreateShaderFromFile(const char* path, GLenum type)
{
    ifstream shaderFile;
    string shaderData;
    shaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        shaderFile.open(path);
        stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderData = shaderStream.str();
    }
    catch (ifstream::failure& e) {
        cerr << "Error to load shader from file " << path << endl;
        return 0;
    }

    const char* string = shaderData.c_str();

    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &string, NULL);
    glCompileShader(id);
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success != true) {
        cerr << "Error to compile shader " << path << endl;
        ShaderLog(id);
        glDeleteShader(id);
        id = 0;
    }

    return id;
}

unsigned int CreateProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    glDetachShader(id, vertexShader);
    glDetachShader(id, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (success != true) {
        cerr << "Error to link program" << endl;
        ProgramLog(id);
        glDeleteProgram(id);
        id = 0;
    }

    return id;
}

unsigned int CreateProgram(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader)
{
    unsigned int id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glAttachShader(id, geometryShader);
    glLinkProgram(id);
    glDetachShader(id, vertexShader);
    glDetachShader(id, fragmentShader);
    glDetachShader(id, geometryShader);
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (success != true) {
        cerr << "Error to link program" << endl;
        ProgramLog(id);
        glDeleteProgram(id);
        id = 0;
    }

    return id;
}

unsigned int LoadShaderFromFile(const char* vertexPath, const char* fragmentPath)
{
    unsigned int vertexShader = CreateShaderFromFile(vertexPath, GL_VERTEX_SHADER);
    unsigned int fragmentShader = CreateShaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);

    return CreateProgram(vertexShader, fragmentShader);
}

unsigned int LoadShaderFromFile(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{   
    unsigned int vertexShader = CreateShaderFromFile(vertexPath, GL_VERTEX_SHADER);
    unsigned int geometryShader = CreateShaderFromFile(geometryPath, GL_GEOMETRY_SHADER);
    unsigned int fragmentShader = CreateShaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);

    return CreateProgram(vertexShader, geometryShader, fragmentShader);
}

unsigned int LoadShaderFromString(const char* vertexString, const char* fragmentString)
{
    unsigned int vertexShader = CreateShaderFromString(vertexString, GL_VERTEX_SHADER);
    unsigned int fragmentShader = CreateShaderFromString(fragmentString, GL_FRAGMENT_SHADER);

    return CreateProgram(vertexShader, fragmentShader);
}

unsigned int LoadShaderFromString(const char* vertexString, const char* geometryString, const char* fragmentString)
{
    unsigned int vertexShader = CreateShaderFromString(vertexString, GL_VERTEX_SHADER);
    unsigned int geometryShader = CreateShaderFromString(geometryString, GL_GEOMETRY_SHADER);
    unsigned int fragmentShader = CreateShaderFromString(fragmentString, GL_FRAGMENT_SHADER);
    
    return CreateProgram(vertexShader, geometryShader, fragmentShader);
}
