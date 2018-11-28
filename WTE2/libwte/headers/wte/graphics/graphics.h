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
    GLuint vao_, vbo_pos_, vbo_col_, prog_;
    GLint loc_pos_, loc_col_;

    void CompileShaders();
};

}
}
