#ifndef COMPUTE_SHADER_H
#define COMPUTE_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    
    public:
        unsigned int ID;
        
        // vert + frag mode
        Shader(const char* vertPath, const char* fragPath)
        {
            std::string vertString;
            std::ifstream vertFile;
            std::string fragString;
            std::ifstream fragFile;

            vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try
            {
                vertFile.open(vertPath);
                std::stringstream vertStream;
                vertStream << vertFile.rdbuf();
                vertFile.close();
                vertString = vertStream.str();

                fragFile.open(fragPath);
                std::stringstream fragStream;
                fragStream << fragFile.rdbuf();
                fragFile.close();
                fragString = fragStream.str();
            }
            catch (std::ifstream::failure &e)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << "\n";
            }
            const char *vertCode = vertString.c_str();
            const char *fragCode = fragString.c_str();

            GLuint vert;
            vert = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vert, 1, &vertCode, NULL);
            glCompileShader(vert);
            checkCompileErrors(vert,"VERTEX");

            GLuint frag;
            frag = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(frag, 1, &fragCode, NULL);
            glCompileShader(frag);
            checkCompileErrors(vert, "FRAGMENT");

            ID = glCreateProgram();
            glAttachShader(ID,vert);
            glAttachShader(ID, frag);
            glLinkProgram(ID);
            checkCompileErrors(vert, "PROGRAM");

            glDeleteShader(vert);
            glDeleteShader(frag);
        }

        // compile shader
        Shader(const char* computePath)
        {
            std::string shaderString;
            std::ifstream shaderFile;

            shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                shaderFile.open(computePath);

                std::stringstream shaderStream;
                shaderStream << shaderFile.rdbuf();

                shaderFile.close();

                shaderString = shaderStream.str();
            }
            catch(std::ifstream::failure& e)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << "\n";
            }

            const char *shaderCode = shaderString.c_str();

            GLuint compute;
            compute = glCreateShader(GL_COMPUTE_SHADER);
            glShaderSource(compute,1,&shaderCode,NULL);
            glCompileShader(compute);
            checkCompileErrors(compute, "COMPUTE");

            ID = glCreateProgram();
            glAttachShader(ID,compute);
            glLinkProgram(ID);
            checkCompileErrors(ID,"PROGRAM");
            glDeleteShader(compute);
        }

        void use()
        {
            glUseProgram(ID);
        }
        // utility uniform functions
        // ------------------------------------------------------------------------
        void setBool(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        // ------------------------------------------------------------------------
        void setInt(const std::string &name, int value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setFloat(const std::string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setVec2(const std::string &name, const glm::vec2 &value) const
        {
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string &name, float x, float y) const
        {
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string &name, const glm::vec3 &value) const
        {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string &name, float x, float y, float z) const
        {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string &name, const glm::vec4 &value) const
        {
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string &name, float x, float y, float z, float w)
        {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string &name, const glm::mat2 &mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string &name, const glm::mat3 &mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(const std::string &name, const glm::mat4 &mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

    private:
        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        void checkCompileErrors(GLuint shader, std::string type)
        {
            GLint success;
            GLchar infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                              << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                              << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
};

#endif