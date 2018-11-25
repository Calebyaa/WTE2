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
        "#version 450 core                                                   \n"
        "                                                                    \n"
        "void main() {                                                       \n"
        "    const vec4 vertices[3] = vec4[3](vec4(-0.25, -0.25, 0.5, 1.0),  \n"
        "                                     vec4( 0.25, -0.25, 0.5, 1.0),  \n"
        "                                     vec4( 0.25,  0.25, 0.5, 1.0)); \n"
        "    gl_Position = vertices[gl_VertexID];                            \n"
        "}                                                                   \n"
    };

    static const GLchar* fragment_shader_source[]{
        "#version 450 core                                                   \n"
        "                                                                    \n"
        "out vec4 color;                                                     \n"
        "                                                                    \n"
        "void main() {                                                       \n"
        "    color = vec4(1.0, 1.0, 1.0, 1.0);                               \n"
        "}"
    };

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    auto error = glGetError();

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
error = glGetError();


    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
error = glGetError();
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUseProgram(rendering_program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() {
    GLFWwindow* window;
    
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1600, 900, "Working Title Engine 2", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    startup();

    do {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window));

    shutdown();
}

#endif // !WTE_MAIN