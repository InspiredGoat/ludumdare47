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


static const char* vertex_shader_source =
"#version 330\n"
"\n"
"layout (location = 0) in vec2 aPos;\n"
"\n"
"void main() {\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"}\n";
static const char* frag_shader_source = 
"#version 330\n"
"\n"
"out vec4 FragColor"
"\n"
"void main() {\n"
"	FragColor = vec4(1.0, 1.0, 0, 1.0);\n"
"}\n";

static unsigned int shader_program;

static unsigned short max_batch_count = 0;
static unsigned short active_batch_count = 0;
static Batch* batches;


//----------------------------------------------------------------------------------------------------


void Batch_system_init(unsigned short max_batches) {
	// allocate batch memory
	max_batch_count = max_batches;
	batches = (Batch*) malloc(sizeof(Batch) * max_batch_count);

	// compile shader programs
	
	int success;
	char info_log[512];

	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		printf("ERROR SHADER DIDN'T COMPILE %i: %s\n", __LINE__, info_log);
	}

	unsigned int frag_shader;
	
	frag_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
	glCompileShader(frag_shader);

	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
		printf("ERROR SHADER DIDN'T COMPILE %i: %s\n", __LINE__, info_log);
	}

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, frag_shader);
	glLinkProgram(shader_program);

	// NOTE: Currently no error checking on the link status

	glDeleteShader(vertex_shader);
	glDeleteShader(frag_shader);
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

// REWORK
void Batch_remove(BatchID id) {
}


//----------------------------------------------------------------------------------------------------


void Batch_drawRect(BatchID id, int x, int y, int width, int height) {
	Vertex verts[2];

	verts[0].x = x;
	verts[0].y = y;

	verts[1].x = x + width;
	verts[1].y = y + height;



	Batch* batch = &batches[id];

	// if buffers are full, flush before proceeding
	if(batch->current_index >= batch->max_pool_size) {
		Batch_flush(id);
	}

	glBindVertexArray(batch->vao);

	// allocate 2 vertices per rect
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, batch->current_index * 2 * sizeof(Vertex), sizeof(Vertex) * 2, verts);

	// allocate indeces

	batch->current_index++;
}


//----------------------------------------------------------------------------------------------------


void Batch_flush(BatchID id) {
	Batch* batch = &batches[id];

	// draw instanced elements

	// reset batch parameters
	batch->current_index = 0;
}


//----------------------------------------------------------------------------------------------------
