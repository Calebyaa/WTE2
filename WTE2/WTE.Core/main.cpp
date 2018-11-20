#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef WTE_MAIN
static const struct
{
    float x, y, z;
    float r, g, b;
} vertices[3] =
{
    {-0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f},
    { 0.6f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f},
    { 0.0f,  0.6f, 0.0f, 0.0f, 0.0f, 1.0f}
};

static const char* vertex_shader_text =
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

int main() {

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    auto window = glfwCreateWindow(1600, 900, "Working Title Engine 2", NULL, NULL);

    if (nullptr == window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(0);

    glClearColor(0.96f, 0.36f, 0.15f, 1.0f);

    // simplest draw loop, maybe?
    do {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);

}

#endif // !WTE_MAIN