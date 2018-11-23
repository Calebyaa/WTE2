#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef WTE_MAIN

GLuint compile_shaders() {
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    static const GLchar* vertex_shader_source[]{
        "#version 450 core                              \n"
        "                                               \n"
        "void main() {                                  \n"
        "    gl_position = vec4(0.0, 0.0, 0.5, 1.0);    \n"
        "}                                              \n"
    };

    static const GLchar* fragment_shader_source[]{
        "#version 450 core                              \n"
        "                                               \n"
        "out vec4 color;                                \n"
        "                                               \n"
        "void main() {                                  \n"
        "    color = vec4(1.0, 1.0, 1.0, 1.0);          \n"
        "}"
    };

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);



    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    auto error = glGetError();
    return program;
}

static GLuint rendering_program;
static GLuint vertex_array_object;

void startup() {
    rendering_program = compile_shaders();
    glCreateVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);
}

void shutdown() {
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteProgram(rendering_program);
}

void render() {

    glClearColor(1.0f, 0.5f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(rendering_program);
    glDrawArrays(GL_POINTS, 0, 1);
}

int main() {
    GLFWwindow* window;
    
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }


    window = glfwCreateWindow(1600, 900, "Working Title Engine 2", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    startup();

    glPointSize(40.0f);

    

    do {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window));

    shutdown();
}

#endif // !WTE_MAIN