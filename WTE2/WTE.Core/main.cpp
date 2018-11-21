#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef WTE_MAIN
static const struct
{
    float x, y, z, w;
    float r, g, b, a;
} vertices[3] =
{
    {-0.6f, -0.4f, 0.0f, 1.0f,
      1.0f,  0.0f, 0.0f, 1.0f },
    { 0.6f, -0.4f, 0.0f, 1.0f,
      0.0f,  1.0f, 0.0f, 1.0f },
    { 0.0f,  0.6f, 0.0f, 1.0f,
      0.0f,  0.0f, 1.0f, 1.0f }
};

static const char* vertexShaderSource =
"#version 430 core\n"
"uniform mat4 mvp;\n"
"in vec4 vertexPosition;\n"
"in vec4 vertexColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"    gl_Position = mvp * vertexPosition;\n"
"    color = vertexColor;\n"
"}\n";

static const char* fragmentShaderSource =
"#version 430 core\n"
"in vec4 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = color;\n"
"}\n";


int main() {

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(1600, 900, "Working Title Engine 2", NULL, NULL);

    if (nullptr == window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    glClearColor(0.96f, 0.36f, 0.15f, 1.0f);

    // create your buffers
    GLuint vertexBuffer, vert, frag, program;
    GLint mvpLocation, vertexPositionLocation, vertexColorLocation;

    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // this should be spread out in to two buffers.
    // though I'm not actually likely to keep using it at all.
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);   
    glEnableVertexAttribArray(vertexPositionLocation);
    glVertexAttribPointer(vertexPositionLocation, 4, GL_FLOAT, GL_FALSE,
        sizeof(float) * 8, (void*)0);    
    glEnableVertexAttribArray(vertexColorLocation);
    glVertexAttribPointer(vertexColorLocation, 4, GL_FLOAT, GL_FALSE,
        sizeof(float) * 8, (void*)(sizeof(float) * 4));


    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertexShaderSource, NULL);
    glCompileShader(vert);
    
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragmentShaderSource, NULL);
    glCompileShader(frag);

    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    mvpLocation = glGetUniformLocation(program, "mvp");
    vertexPositionLocation = glGetAttribLocation(program, "vertexPosition");
    vertexColorLocation = glGetAttribLocation(program, "vertexColor");

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    auto ratio = width / (float)height;

    glViewport(0, 0, width, height);

    glm::mat4x4 m(1.0f);
    glm::mat4x4 p = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    glm::mat4x4 mvp = p * m;
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(mvp));

    glUseProgram(program);

    do {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

#endif // !WTE_MAIN