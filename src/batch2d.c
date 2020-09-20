#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <time.h>

#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include "../include/batch2d.h"


//----------------------------------------------------------------------------------------------------


typedef struct {
	float x;
	float y;
} Vertex;

typedef struct {
	unsigned char is_active;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int max_pool_size;
	unsigned int current_index;
} Batch;


//----------------------------------------------------------------------------------------------------


static const char* vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
static const char* frag_shader_source = "#version 330 core\n"
	"out vec4 FragColor;"
	"void main() {\n"
	"	FragColor = vec4(1.0f, 0, 0, 1.0f);\n"
	"}\n\0";

static unsigned int shader_program;

static unsigned short max_batch_count = 0;
static unsigned short active_batch_count = 0;
static unsigned short active_batch_index = 0;
static Batch* batches;


//----------------------------------------------------------------------------------------------------


void Batch_system_init(unsigned short max_batches) {
	// allocate batch memory
	max_batch_count = max_batches;
	batches = (Batch*) malloc(sizeof(Batch) * max_batch_count);

	// compile shader programs
	
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	int success;
	char info_log[512];

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		printf("Vertex shader didn't compile: %s\n", info_log);
	}

	unsigned int frag_shader;
	
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
	glCompileShader(frag_shader);

	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
		printf("Frag shader didn't compile: %s\n", info_log);
	}

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, frag_shader);
	glLinkProgram(shader_program);

	// NOTE: Currently no error checking on the link status

	glDeleteShader(vertex_shader);
	glDeleteShader(frag_shader);
}


void Batch_system_free() {
	free(batches);
}


//----------------------------------------------------------------------------------------------------


BatchID Batch_create(unsigned int max_pool_size) {
	// check if system is initialized
	assert(max_batch_count > 0);

	Batch* batch = &batches[active_batch_count];

	batch->is_active = 1;
	batch->current_index = 0;
	batch->max_pool_size = max_pool_size;

	glGenVertexArrays(1, &batch->vao);
	glBindVertexArray(batch->vao);

	glGenBuffers(1, &batch->vbo);
	glGenBuffers(1, &batch->ebo);

	// allocate memory on gpu
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4 * max_pool_size, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6 * max_pool_size, NULL, GL_DYNAMIC_DRAW);

	// indices can be generated in advance
	for(unsigned int i = 0; i < max_pool_size; i++) {
		unsigned int indices[6];

		indices[0] = i * 4;
		indices[1] = i * 4 + 1;
		indices[2] = i * 4 + 3;
		indices[3] = i * 4 + 1;
		indices[4] = i * 4 + 2;
		indices[5] = i * 4 + 3;

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, i * 6 * sizeof(unsigned int), sizeof(unsigned int) * 6, indices);
	}


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

	// unbind data
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	active_batch_count++;
	active_batch_index = active_batch_count - 1;

	return active_batch_count - 1;
}

// REWORK
void Batch_destroy(BatchID id) {
}


//----------------------------------------------------------------------------------------------------

void Batch_drawRect(float x, float y, float width, float height) {
	Batch* batch = &batches[active_batch_index];

	// if buffers are full, flush before proceeding
	if(batch->current_index >= batch->max_pool_size) {
		Batch_flush(active_batch_index);
	}

	// 4 points per quad
	Vertex verts[4];

	// top right
	verts[0].x = x + width;
	verts[0].y = y;

	// bottom right
	verts[1].x = x + width;
	verts[1].y = y + height;

	// bottom left
	verts[2].x = x;
	verts[2].y = y + height;

	// top left
	verts[3].x = x;
	verts[3].y = y;

	glBindVertexArray(batch->vao);

	// allocate 4 vertices per rect
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, batch->current_index * 4 * sizeof(Vertex), sizeof(Vertex) * 4, verts);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	batch->current_index++;
}


//----------------------------------------------------------------------------------------------------


void Batch_flush() {
	Batch* batch = &batches[active_batch_index];
	glUseProgram(shader_program);

	glBindVertexArray(batch->vao);

	printf("Current index: %i\n", batch->current_index);
	// draw instanced elements
	glDrawElementsInstanced(GL_TRIANGLES, batch->current_index * 6, GL_UNSIGNED_INT, 0, batch->current_index);
	glBindVertexArray(0);

	// reset batch parameters
	batch->current_index = 0;
}


//----------------------------------------------------------------------------------------------------
