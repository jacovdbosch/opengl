#ifndef SHADER_H
#define SHADER_H

#include <string>

struct Shader {
    unsigned int id;
};

Shader* createShader(const char* vertexPath, const char* fragmentPath);

void deleteShader(Shader* shader);
void useShader(Shader* shader);
void setShaderBool(Shader* shader, const std::string& name, bool value);
void setShaderInt(Shader* shader, const std::string& name, int value);
void setShaderFloat(Shader* shader, const std::string& name, float value);
void setShaderVec2(Shader* shader, const std::string& name, float x, float y);
void setShaderVec3(Shader* shader, const std::string& name, float x, float y, float z);
void setShaderVec4(Shader* shader, const std::string& name, float x, float y, float z, float w);

#endif
