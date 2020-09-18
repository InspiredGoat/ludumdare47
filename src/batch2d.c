#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include "../include/batch2d.h"


//----------------------------------------------------------------------------------------------------


typedef struct {
	float x;
	float y;
} Vertex;

typedef struct {
	unsigned int vao;
	unsigned int vbo;
	unsigned int max_pool_size;
	unsigned int current_index;
} Batch;


//----------------------------------------------------------------------------------------------------


static unsigned short max_batch_count = 0;
static unsigned short active_batch_count = 0;
static Batch* batches;


//----------------------------------------------------------------------------------------------------


void Batch_system_init(unsigned short max_batches) {
	max_batch_count = max_batches;
	batches = (Batch*) malloc(sizeof(Batch) * max_batch_count);
}


//----------------------------------------------------------------------------------------------------


BatchID Batch_create(unsigned int max_pool_size) {
	// check if system is initialized
	assert(max_batch_count > 0);

	Batch* batch = &batches[active_batch_count];

	batch->max_pool_size = max_pool_size;

	glGenVertexArrays(1, &batch->vao);
	glBindVertexArray(batch->vao);

	// allocate memory on gpu
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 2 * max_pool_size, NULL, GL_DYNAMIC_DRAW);

	// unbind data
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	active_batch_count++;
	return active_batch_count;
}

void Batch_destroy(Batch* batch) {
	
	free(batch);
}


//----------------------------------------------------------------------------------------------------


void Batch_drawRect(BatchID id, int x, int y, int width, int height) {
	Vertex verts[2];

	verts[0].x = x;
	verts[0].y = y;

	verts[1].x = x + width;
	verts[1].y = y + height;

	Batch* batch = &batches[active_batch_count];

	glBindVertexArray(batch->vao);

	// Allocate 2 verticies per draw
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, batch->current_index * 2 * sizeof(Vertex), sizeof(Vertex) * 2, verts);
}


//----------------------------------------------------------------------------------------------------


void Batch_display(BatchID id) {
}


//----------------------------------------------------------------------------------------------------


