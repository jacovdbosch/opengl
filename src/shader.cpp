#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "glad/glad.h"

static unsigned int createGLShader(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        
        std::cout << "[ERROR]: Error compiling shader" << std::endl << infoLog << std::endl;
    }
    
    return shader;
}

Shader* createShader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        
        std::stringstream vShaderStream, fShaderStream;
        
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "[ERROR]: Failed to read shader files." << std::endl;
    }
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    unsigned int vertexShader = createGLShader(vShaderCode, GL_VERTEX_SHADER);
    unsigned int fragmentShader = createGLShader(fShaderCode, GL_FRAGMENT_SHADER);
    
    unsigned int shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        
        std::cout << "[ERROR]: Error linking program " << std::endl << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return new Shader { shaderProgram };
}

void deleteShader(Shader* shader) {
    glDeleteProgram(shader->id);
    delete shader;
}

void useShader(Shader* shader) {
    glUseProgram(shader->id);
}

void setShaderBool(Shader* shader, const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(shader->id, name.c_str()), (int)value);
};

void setShaderInt(Shader* shader, const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(shader->id, name.c_str()), value);
};

void setShaderFloat(Shader* shader, const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(shader->id, name.c_str()), value);
};

void setShaderVec2(Shader* shader, const std::string& name, float x, float y) {
    glUniform2f(glGetUniformLocation(shader->id, name.c_str()), x, y);
}

void setShaderVec3(Shader* shader, const std::string& name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(shader->id, name.c_str()), x, y, z);
}

void setShaderVec4(Shader* shader, const std::string& name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(shader->id, name.c_str()), x, y, z, w);
};