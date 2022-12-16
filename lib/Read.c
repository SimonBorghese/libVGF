//
// Created by simon on 12/12/22.
//

#include "Read.h"

static inline struct VGF_chunk* _VGF_getChunk(VGF *file, const char *name){
    for (int c = 0; c < file->fChunks.numChunks; c++){
        if (!strcmp(file->fChunks.chunks[c].chunkName, name)){
            return &file->fChunks.chunks[c];
        }
    }
    return NULL;
}

VGF* VGF_R_InitFile(const char *filename){
    // Allocate a new VGF Object
    VGF *_New = (VGF*) malloc(sizeof(VGF));
    // Open the file
    _New->fVGF = fopen(filename, "r");
    // Read the header
    fread(&_New->fHeader, sizeof(struct VGF_header), 1, _New->fVGF);
    // Ensure it is a VGF file
    assert(!strncmp(_New->fHeader.sig, VGF_SIG, VGF_SIZEOF_SIG));
    // Ensure it is a matching version
    assert(_New->fHeader.version == VGF_VERSION);

    // Move into chunk Info
    fseek(_New->fVGF, _New->fHeader.offsetToInfo, SEEK_SET);
    // Read number of chunks
    fread(&_New->fChunks.numChunks, sizeof(int), 1, _New->fVGF);
    printf("Num chunks: %d\n", _New->fChunks.numChunks);
    // Allocate enough VGF_Chunks for the amount in the file
    _New->fChunks.chunks = (struct VGF_chunk*) (malloc(sizeof(struct VGF_chunk) * _New->fChunks.numChunks));
    // Read Each Chunk
    for (int c = 0; c < _New->fChunks.numChunks; c++){
        struct VGF_chunk* targetChunk = &_New->fChunks.chunks[c];

        // Read the length of the name
        fread(&targetChunk->chunkNameLen, sizeof(int), 1, _New->fVGF);
        // Allocate enough storage for the chunk name
        targetChunk->chunkName = (char*) (malloc(sizeof(char) * targetChunk->chunkNameLen));
        // Read Chunk Name
        fread(&targetChunk->chunkName[0], sizeof(char), targetChunk->chunkNameLen, _New->fVGF);
        // Read Chunk offset, size, and number of sub chunks
        fread(&targetChunk->chunkOffset, sizeof(int), 1, _New->fVGF);
        fread(&targetChunk->chunkSize, sizeof(int), 1, _New->fVGF);
        fread(&targetChunk->numSubChunks, sizeof(int), 1, _New->fVGF);
    }
    // Now, with all chunk metadata read, return the object
    return _New;
}

int VGF_R_getNumChunks(VGF *file){ return file->fChunks.numChunks; }

int VGF_R_getNumSubChunks(VGF *file, const char *chunkName) { return _VGF_getChunk(file, chunkName)->numSubChunks; }

void* VGF_R_readChunk(VGF *file, const char *chunkName, int *numChunks){
    struct VGF_chunk *_chunk = _VGF_getChunk(file, chunkName);
    fseek(file->fVGF, _chunk->chunkOffset, SEEK_SET);

    void *_chunkData = malloc(_chunk->chunkSize * _chunk->numSubChunks);
    fread(_chunkData, _chunk->chunkSize, _chunk->numSubChunks, file->fVGF);

    assert(numChunks != NULL);
    *numChunks = _chunk->numSubChunks;
    return _chunkData;
}

int VGF_R_closeFile(VGF *file){
    fclose(file->fVGF);

    for (int c = 0; c < file->fChunks.numChunks; c++){
        free(file->fChunks.chunks[c].chunkName);
    }
    free(file->fChunks.chunks);

    free(file);
}