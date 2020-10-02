#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GLFW/glfw3.h>
#include "../include/batch2d.h"

// this file will have a placeholder window and stuff to test the batch renderer
void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

float randf() 
{
	return (rand()%100000) / 100000.f;
}

int main() 
{
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

	double sum = 0;
	unsigned int loops = 0;

	int total_quads = 10000;
	int quads_per_drawcall = 100;

	Batch_system_init(1);
	BatchID id = Batch_create(quads_per_drawcall);

	while(!glfwWindowShouldClose(window))
	{
		//srand(0);
		prev_time = glfwGetTime();

		glfwSwapInterval(0);
		glfwSwapBuffers(window);

		float start = glfwGetTime();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for(int i = 0; i < total_quads; i++) 
		{
			Batch_drawRect(-1.f + 2*randf(), -1.f + 2*randf(), .01f, .01f);
		}

		Batch_flush();
		glfwPollEvents();

		float render_time = (glfwGetTime() - start);
		delta = (float) (glfwGetTime() - prev_time);

		sum += render_time;
		loops++;

		printf("render taken: %f\n", render_time);
		printf("frame time: %f\n\n", delta);
	}

//	printf("Total time: %03f, Average: %03f in %u loops\n", sum, sum / (double) loops, loops);
//	FILE* output = fopen("results.txt", "a");
//
//	fprintf(output, "%u, %u, %03f, %03f, %u\n", total_quads, quads_per_drawcall, sum, sum / (double) loops, loops);
//
//	fclose(output);

	Batch_destroy(id);
	Batch_system_free();
	return 0;
}
