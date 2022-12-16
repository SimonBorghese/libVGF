//
// Created by simon on 12/5/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <structs.h>
#include <string.h>
#include <inttypes.h>
#include "Read.h"
#include "Write.h"

struct dataType1{
    int uwu;
    float owo;
    uint64_t owo2;
    char aChar;
};

int main(void){

    printf("Hello VGF\n");

    struct dataType1 mainData[2];
    mainData[0].uwu = 42069;
    mainData[0].owo = 494.2443;
    mainData[0].owo2 = 69069069;
    mainData[0].aChar = 'o';

    mainData[1].uwu = 20243;
    mainData[1].owo = 69.420;
    mainData[1].owo2 = 4200000;
    mainData[1].aChar = 'Q';

    VGF *writeFile = VGF_W_InitFile("test.vgf");
    VGF_W_AddChunk(writeFile, "vec1", &mainData[0], sizeof(struct dataType1), 2);
    VGF_W_WriteToFile(writeFile);


    VGF *file = VGF_R_InitFile("test.vgf");

    int numberChunks = 0;
    struct dataType1 *foundType = VGF_R_readChunk(file, "vec1", &numberChunks);
    printf("Got: %d %f %d %c\n", foundType[0].uwu, foundType[0].owo, foundType[0].owo2, foundType[0].aChar);
    printf("Got: %d %f %d %c\n", foundType[1].uwu, foundType[1].owo, foundType[1].owo2, foundType[1].aChar);
    free(foundType);

    VGF_R_closeFile(file);
    printf("Closed!\n");



    return 0;
}