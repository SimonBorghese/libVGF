//
// Created by simon on 12/12/22.
//

#ifndef LIBVGF_READ_H
#define LIBVGF_READ_H
#include "Common.h"

VGF* VGF_R_InitFile(const char *filename);


int VGF_R_getNumChunks(VGF *file);

int VGF_R_getNumSubChunks(VGF *file, const char *chunkName);

void* VGF_R_readChunk(VGF *file, const char *chunkName, int *numChunks);

int VGF_R_closeFile(VGF *file);


#endif //LIBVGF_READ_H
