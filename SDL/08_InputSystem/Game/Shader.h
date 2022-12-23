#ifndef __SHADER_H__
#define __SHADER_H__

#include "MyMath.h"
#include <GL/glew.h>
#include <string>

class Shader {
  public:
    Shader();
    ~Shader();
    // Load the vertex/fragment shaders with the given names
    bool Load(const std::string &vertName, const std::string &fragName);
    void Unload();
    // Set this as the active shader program
    void SetActive();
    // Sets a Matrix uniform
    void SetMatrixUniform(const char *name, const Matrix4 &matrix);

  private:
    // Tries to compile the specified shader
    bool CompileShader(const std::string &fileName,
                       GLenum shaderType,
                       GLuint &outShader);

    // Tests whether shader compiled successfully
    bool IsCompiled(GLuint shader);
    // Tests whether vertex/fragment programs link
    bool IsValidProgram();

  private:
    // Store the shader object IDs
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;
};

#endif // __SHADER_H__