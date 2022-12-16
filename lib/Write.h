//
// Created by simon on 12/13/22.
//

#ifndef LIBVGF_WRITE_H
#define LIBVGF_WRITE_H
#include "Common.h"

VGF* VGF_W_InitFile(const char *filename);

// Memory allocated for data must be sizePerChunk * chunkCount, otherwise will lead to segfault and that's no godo
int VGF_W_AddChunk(VGF *file, const char *chunkName, void *data, int sizePerChunk, int chunkCount);

int VGF_W_WriteToFile(VGF* file);

#endif //LIBVGF_WRITE_H
