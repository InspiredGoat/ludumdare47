#pragma once

typedef unsigned short BatchID;

void Batch_system_init(unsigned short max_batches); // sets up batches

BatchID Batch_add(unsigned int max_pool_size);
void Batch_drawRect(BatchID batch, int x, int y, int width, int height); // add some way to pass textures
void Batch_display(BatchID batch);
void Batch_remove(BatchID batch);
