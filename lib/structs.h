//
// Created by simon on 12/5/22.
//

#ifndef LIBVGF_STRUCTS_H
#define LIBVGF_STRUCTS_H
#include <bits/types/FILE.h>

// DEFINES
#define VGF_SIG "VGF"
#define VGF_SIZEOF_SIG 4

#define VGF_VERSION 1
#define VGF_WATERMARK "Made with libVGF version 1.0a"
#define VGF_WATERMARK_LEN (strlen(VGF_WATERMARK) * sizeof(char))

// Signature
struct VGF_header{
    char sig[4]; // VGF\0 (Very Good File Null Terminator)
    int version;
    int offsetToInfo; // Offset to info struct
};

// Information on all the chunks (must be read manually, cannot be cased to struct)
struct VGF_chunkInfo{
    int numChunks;
    struct VGF_chunk *chunks;
};

struct VGF_chunk{
    int chunkNameLen;
    char *chunkName;
    int chunkOffset;
    int chunkSize;
    int numSubChunks;

    void *data;
};



typedef struct{
    struct VGF_header fHeader;
    struct VGF_chunkInfo fChunks;
    FILE *fVGF;
} VGF;


#endif //LIBVGF_STRUCTS_H
