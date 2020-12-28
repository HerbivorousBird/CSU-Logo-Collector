#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:
    unsigned int ID; 
    Shader() { }
    Shader  &use();
    void    Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr); // note: geometry source code is optional 
    // utility functions
    void    setFloat    (const char *name, float value, bool useShader = false);
    void    setInt  (const char *name, int value, bool useShader = false);
    void    setVec2f (const char *name, float x, float y, bool useShader = false);
    void    setVec2f (const char *name, const glm::vec2 &value, bool useShader = false);
    void    setVec3f (const char *name, float x, float y, float z, bool useShader = false);
    void    setVec3f (const char *name, const glm::vec3 &value, bool useShader = false);
    void    setVec4f (const char *name, float x, float y, float z, float w, bool useShader = false);
    void    setVec4f (const char *name, const glm::vec4 &value, bool useShader = false);
    void    setMat4  (const char *name, const glm::mat4 &matrix, bool useShader = false);
private:
    void    checkCompileErrors(unsigned int object, std::string type); 
};

#endif