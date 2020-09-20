#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include "../include/batch2d.h"

// this file will have a placeholder window and stuff to test the batch renderer

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "testapp", NULL, NULL);
	glViewport(0, 0, 800, 800);

	Batch_system_init(1);
	BatchID id = Batch_create(10);

	while(!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		Batch_drawRect(-1, -1, 2, 2);
		Batch_flush();
		glfwPollEvents();
	}

	Batch_destroy(id);
	Batch_system_free();
	return 0;
}
