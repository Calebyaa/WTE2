#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <wte/graphics/graphics.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#ifndef WTE_MAIN

GLuint compile_shaders() {
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;

    static const GLchar* vertex_shader_source[]{
        "#version 450 core                                                   \n"
        "in vec4 position;                                                   \n"
        "in vec4 color;                                                      \n"
        "out vec4 vert_out_color;                                            \n"
        "                                                                    \n"
        "void main() {                                                       \n"
        "    gl_Position = position;                                         \n"
        "    vert_out_color = color;                                         \n"
        "}                                                                   \n"
    };

    static const GLchar* fragment_shader_source[]{
        "#version 450 core                                                   \n"
        "in vec4 vert_out_color;                                             \n"
        "out vec4 frag_out_color;                                            \n"
        "                                                                    \n"
        "void main() {                                                       \n"
        "    frag_out_color = vert_out_color;                                \n"
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

    return program;
}

int main() {

    GLFWwindow* window;
    GLuint rendering_program;
    GLuint vertex_array_object;
    GLuint vbo_vert, vbo_color;
    GLint vert_loc, color_loc;
    

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

    rendering_program = compile_shaders();


    vert_loc = glGetAttribLocation(rendering_program, "position");
    color_loc = glGetAttribLocation(rendering_program, "color");

    std::vector<glm::vec4> verts{
        {-0.25f, -0.25f, 0.5f, 1.0f},
        { 0.25f, -0.25f, 0.5f, 1.0f},
        {  0.0f,  0.25f, 0.5f, 1.0f}
    };

    // 180, 12, 252 - purp
    // 255, 66, 220 - pink
    // 97, 144, 230 - DX Blue
    std::vector<glm::vec4> colors{
        { 0.71f, 0.05f, 0.99f, 1.0f },
        {  1.0f, 0.26f, 0.86f, 1.0f },
        { 0.38f, 0.56f, 0.90f, 1.0f }
    };


    glCreateVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    glCreateBuffers(1, &vbo_vert);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(vert_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vert_loc);

    glCreateBuffers(1, &vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(color_loc);

    glUseProgram(rendering_program);

    do {
        glClearColor(1.0f, 0.5f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window));

    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteProgram(rendering_program);
}

#endif // !WTE_MAIN