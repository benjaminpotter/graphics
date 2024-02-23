
#include <fstream>
#include <sstream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "linalg.h"

class ShaderProgram {

public:

    ShaderProgram() {}
    ShaderProgram(const char *vertexSource, const char *fragmentSource) {
        GLuint vertexShader, fragmentShader;

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        GLint success;
        char log[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, log);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
        }

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);
        
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);  
    }

    ~ShaderProgram() {
        glDeleteProgram(shaderProgram);
    }

    static ShaderProgram fromFiles(const char *vertexPath, const char *fragmentPath) {

        std::ifstream vertex, fragment;
        std::stringstream vertexBuffer, fragmentBuffer;

        std::cout << vertexPath << std::endl;
        
        vertex.open(vertexPath);
        if(!vertex)
            throw;

        fragment.open(fragmentPath);
        if(!fragment)
            throw;

        vertexBuffer << vertex.rdbuf();
        fragmentBuffer << fragment.rdbuf();


        return ShaderProgram(vertexBuffer.str().c_str(), fragmentBuffer.str().c_str());
    }

    void use() {
        glUseProgram(shaderProgram);
    }

    void setMat4(const char *name, mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_TRUE, &value[0][0]);
    }

private:

    GLuint shaderProgram;

};