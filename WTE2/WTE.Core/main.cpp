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

    // TODO: Allow for loading of external shader files
    //       by file name/path
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

    // TODO: Allow for user specification of shader types.
    // Q: Should users be able to set up the whole pipeline?
    // A: There should be some fuckin kinda fuckin object that 
    //    the fuckin users can fuckin pass that fuckin holds 
    //    the fuckin shader types, fuck.
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
    

    // ********************
    // ** Start GL setup **
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    
    // TODO: should be parameterized. Perhaps by some sort of struct?
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // TODO: Also parameterized, but the window handle is the return. Maybe.
    //       Alternate options include:
    //           -> only supporting one window
    //           -> generating a handle to the window and obfuscating the pointer
    window = glfwCreateWindow(1600, 900, "Working Title Engine 2", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    // ** End GL Setup **
    // ******************


    // ***********************
    // ** Start buffer data **
    // TODO: These should be not-hard-coded
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
    // ** End buffer data **
    // *********************

    // Q: is this shader specific?
    //    as in, do I need more than one VAO/shad?
    // A: This has nothing to do with a shader.
    //    A single VAO should have multiple VBOs bound TO IT.
    //    This allows for single binding of a complete set of data.
    //    This is also how you would handle batching.
    GLuint vertex_array_object;
    GLuint vbo_vert, vbo_color;

    GLuint rendering_program = compile_shaders();

    // TODO: Allow for easy creation of these per shad prog?
    //       Or maybe it's the responsibility of the programmer to set it up?
    //       For sure requires research.
    // TODO: More research on what other engines do, baby.
    GLint vert_loc, color_loc;
    vert_loc = glGetAttribLocation(rendering_program, "position");
    color_loc = glGetAttribLocation(rendering_program, "color");

    // TODO: this should be 1 VAO per geometry.
    //       each VAO should define the requisite VBOs.
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

    // TODO: should be added to the GL configurable stuff,
    //       but should be set-able per-buffer (once FBOs get
    //       into the mix)
    glClearColor(1.0f, 0.5f, 0.25f, 1.0f);

    do {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window));

    // TODO: should probably be cleaning up my buffers here, too.
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteProgram(rendering_program);
}

#endif // !WTE_MAIN