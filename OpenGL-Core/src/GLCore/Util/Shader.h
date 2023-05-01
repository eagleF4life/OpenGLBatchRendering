#pragma once

#include <string>

#include <glad/glad.h>
#include "glm/fwd.hpp"

namespace GLCore::Utils {

    class Shader
    {
    public:
        ~Shader();

        GLuint GetRendererID() { return m_RendererID; }

        static Shader* FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

        void SetUniformMat4(uint32_t shader, const std::string& name, const glm::mat4& matrix) const;
        void SetUniform4fv(uint32_t shader, const std::string& name, const glm::vec4& vector) const;

    private:
        Shader() = default;
        int GetUniformLocation(const std::string& name) const;

        void LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        GLuint CompileShader(GLenum type, const std::string& source);
        GLuint m_RendererID;
        mutable std::unordered_map<std::string, int> m_UniformLocationCache;
    };

}
