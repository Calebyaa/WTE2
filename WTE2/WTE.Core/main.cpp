#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef WTE_MAIN

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