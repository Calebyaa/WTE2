#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace wte {
namespace graphics {

class Renderer {
public:
    Renderer();
    void Start();
    bool Render();
    void Shutdown();

private:
    GLFWwindow* window_;
    GLuint vao_, vbo_vertices_, vbo_colors_, program_;
    GLint position_attribute_location_, color_attribute_location_;

    void CompileShaders_();
};

}
}
