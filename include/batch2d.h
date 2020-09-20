#pragma once

typedef unsigned short BatchID;

void Batch_system_init(unsigned short max_batches); // sets up batches
void Batch_system_free();

BatchID Batch_create(unsigned int max_pool_size);
void Batch_destroy(BatchID batch);

void Batch_setActiveBatch(BatchID batch);
void Batch_drawRect(float x, float y, float width, float height); // add some way to pass textures
void Batch_flush(); // draws instances =)
