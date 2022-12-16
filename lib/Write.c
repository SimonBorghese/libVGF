//
// Created by simon on 12/13/22.
//

#include "Write.h"

VGF* VGF_W_InitFile(const char *filename){
    VGF *_New = (VGF*) malloc(sizeof(VGF));
    // Setup the main header to be written
    strncpy(_New->fHeader.sig, VGF_SIG, VGF_SIZEOF_SIG);
    _New->fHeader.version = VGF_VERSION;
    _New->fHeader.offsetToInfo = sizeof(struct VGF_header) + (VGF_WATERMARK_LEN);

    // Create the file
    _New->fVGF = fopen(filename, "w");

    _New->fChunks.chunks = NULL;
    _New->fChunks.numChunks = 0;
    return _New;
}

// Memory allocated for data must be sizePerChunk * chunkCount, otherwise will lead to segfault and that's no godo
int VGF_W_AddChunk(VGF *file, const char *chunkName, void *data, int sizePerChunk, int chunkCount){
    // Add another chunk
    file->fChunks.numChunks++;

    // Reallocate the array to include another chunk
    file->fChunks.chunks = reallocarray(file->fChunks.chunks, file->fChunks.numChunks, sizeof(struct VGF_chunk));
    assert(file->fChunks.chunks);

    int _targetChunk = file->fChunks.numChunks - 1;
    struct VGF_chunk *_currentChunk = &file->fChunks.chunks[_targetChunk];

    _currentChunk->chunkName = (char*) malloc((sizeof(char) * strlen(chunkName)) + 1);
    strcpy(_currentChunk->chunkName, chunkName);

    _currentChunk->numSubChunks = chunkCount;
    _currentChunk->chunkSize = sizePerChunk;
    _currentChunk->chunkNameLen = (strlen(chunkName) * sizeof(char)) + 1;
    _currentChunk->data = data;
    //_currentChunk->chunkOffset: // Set offset later/don't need/can't use
    return 1;
}

int VGF_W_WriteToFile(VGF* file){
    // First, write header
    fwrite(&file->fHeader, sizeof(struct VGF_header), 1, file->fVGF);
    fwrite(&VGF_WATERMARK[0], VGF_WATERMARK_LEN, 1, file->fVGF);
    fwrite(&file->fChunks.numChunks, sizeof(int), 1, file->fVGF);

    int currentOffset = ftell(file->fVGF);

    for (int c = 0; c < file->fChunks.numChunks; c++){
        currentOffset += (sizeof(int) * 4) + (sizeof(char) * file->fChunks.chunks[c].chunkNameLen);
    }

    for (int c = 0; c < file->fChunks.numChunks; c++) {
        struct VGF_chunk *_currentChunk = &file->fChunks.chunks[c];
        _currentChunk->chunkOffset = currentOffset;
        currentOffset += _currentChunk->chunkSize * _currentChunk->numSubChunks;

        fwrite(&_currentChunk->chunkNameLen, sizeof(int), 1, file->fVGF);
        fwrite(&_currentChunk->chunkName[0], sizeof(char), _currentChunk->chunkNameLen, file->fVGF);
        fwrite(&_currentChunk->chunkOffset, sizeof(int), 1, file->fVGF);
        fwrite(&_currentChunk->chunkSize, sizeof(int), 1, file->fVGF);
        fwrite(&_currentChunk->numSubChunks, sizeof(int), 1, file->fVGF);
    }

    for (int c = 0; c < file->fChunks.numChunks; c++){
        fwrite(file->fChunks.chunks[c].data, file->fChunks.chunks[c].chunkSize, file->fChunks.chunks[c].numSubChunks, file->fVGF);
    }

    fflush(file->fVGF);
    fclose(file->fVGF);
    return 1;
}
int VGF_W_Close(VGF* file){
    for (int c = 0; c < file->fChunks.numChunks; c++) {
        free(file->fChunks.chunks[c].chunkName);
    }
    free(file->fChunks.chunks);
    free(file);
    return 1;
}