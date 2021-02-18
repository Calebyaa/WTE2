#include "wte/graphics/graphics.h"    

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

wte::graphics::Renderer::Renderer() {

}

void wte::graphics::Renderer::Start() {
    // ********************
    // ** Start GL setup **
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // TODO: should be parameterized. Perhaps by some sort of struct?
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // TODO: Also parameterized, but the window handle is the return. Maybe.
    //       Alternate options include:
    //           -> only supporting one window
    //           -> generating a handle to the window and obfuscating the pointer
    window_ = glfwCreateWindow(1920, 1080, "Working Title Engine 2", NULL, NULL);
    if (!window_)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window_);
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
    //GLuint vertex_array_object;
    //GLuint vbo_vert, vbo_color;
    
    CompileShaders_();

    // TODO: Allow for easy creation of these per shad prog?
    //       Or maybe it's the responsibility of the programmer to set it up?
    //       For sure requires research.
    // TODO: More research on what other engines do, baby.
    position_attribute_location_ = glGetAttribLocation(program_, "position");
    color_attribute_location_ = glGetAttribLocation(program_, "color");

    // TODO: this should be 1 VAO per geometry.
    //       each VAO should define the requisite VBOs.
    glCreateVertexArrays(1, &(vao_));
    glBindVertexArray(vao_);

    glCreateBuffers(1, &(vbo_vertices_));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(position_attribute_location_, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_attribute_location_);

    glCreateBuffers(1, &(vbo_colors_));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors_);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(color_attribute_location_, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(color_attribute_location_);

    glUseProgram(program_);

    // TODO: should be added to the GL configurable stuff,
    //       but should be set-able per-buffer (once FBOs get
    //       into the mix)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

bool wte::graphics::Renderer::Render() {

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window_);
    glfwPollEvents();

    return(!glfwWindowShouldClose(window_));
}

void wte::graphics::Renderer::Shutdown() {
    glUseProgram(0);
    glDisableVertexAttribArray(position_attribute_location_);
    glDisableVertexAttribArray(color_attribute_location_);
    glDeleteProgram(program_);
    glDeleteBuffers(1, &vbo_vertices_);
    glDeleteBuffers(1, &vbo_colors_);
    glDeleteVertexArrays(1, &vao_);
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void wte::graphics::Renderer::CompileShaders_() {
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

    glDetachShader(program, vertex_shader);
    glDeleteShader(vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(fragment_shader);

    program_ = program;
}