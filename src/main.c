#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GLFW/glfw3.h>
#include "../include/batch2d.h"

// this file will have a placeholder window and stuff to test the batch renderer
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

float randf() {
	return (rand()%100000) / 100000.f;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "testapp", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glViewport(0, 0, 800, 800);


	clock_t time;
	float prev_time;
	float delta;


	Batch_system_init(1);
	BatchID id = Batch_create(100);

	while(!glfwWindowShouldClose(window)) {
		//srand(0);
		prev_time = glfwGetTime();

		//glfwSwapInterval(0);
		glfwSwapBuffers(window);

		float start = glfwGetTime();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for(int i = 0; i < 4000; i++) {
			Batch_drawRect(-1.f + 2*randf(), -1.f + 2*randf(), .01f, .01f);
		}


		//start = glfwGetTime();
		Batch_flush();
		
		printf("time taken: %f\n", (glfwGetTime() - start));
		//printf("flush time taken: %f\n", (glfwGetTime() - start));

		glfwPollEvents();

		delta = (float) (glfwGetTime() - prev_time);

		printf("frame time: %f\n\n", delta);
	}

	Batch_destroy(id);
	Batch_system_free();
	return 0;
}
