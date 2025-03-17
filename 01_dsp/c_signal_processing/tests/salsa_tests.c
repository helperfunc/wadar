#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "../salsa.h"
#include "../proc.h"
#include "../utils.h"
#include "../wavelib/header/wavelib.h"
#include "test_mocks.h"

// Helper function to create a dummy .frames file for testing
void createDummyFramesFile(const char *filename, int numFrames, int numSamplers, int frameRate) {
    FILE *fid = fopen(filename, "wb");
    if (!fid) {
        perror("Error creating dummy .frames file");
        return;
    }

    // Write magic number
    uint32_t magic = 0xFEFE00A2;
    fwrite(&magic, sizeof(uint32_t), 1, fid);

    // Write dummy header data
    int iterations = 10;
    int pps = 1;
    int dacMin = 0;
    int dacMax = 8191;
    int dacStep = 1;
    fwrite(&iterations, sizeof(int), 1, fid);
    fwrite(&pps, sizeof(int), 1, fid);
    fwrite(&dacMin, sizeof(int), 1, fid);
    fwrite(&dacMax, sizeof(int), 1, fid);
    fwrite(&dacStep, sizeof(int), 1, fid);

    // Write radar specifier
    int radarSpecifier = 2;
    fwrite(&radarSpecifier, sizeof(int), 1, fid);

    // Write X2 specific params
    float samplesPerSecond = 3.9E10;
    int pgen = 0;
    float offsetDistance = 0.0;
    float sampleDelayToReference = 0.0;
    fwrite(&samplesPerSecond, sizeof(float), 1, fid);
    fwrite(&pgen, sizeof(int), 1, fid);
    fwrite(&offsetDistance, sizeof(float), 1, fid);
    fwrite(&sampleDelayToReference, sizeof(float), 1, fid);

    // Write number of samplers and frames
    fwrite(&numSamplers, sizeof(int), 1, fid);
    fwrite(&numFrames, sizeof(int), 1, fid);

    //Write numRuns
    int numRuns = 1;
    fwrite(&numRuns, sizeof(int), 1, fid);

    //Write frame rate
    fwrite(&frameRate, sizeof(int), 1, fid);

    // Write dummy times data
    double time = 0.0;
    for (int i = 0; i < numFrames; i++) {
        fwrite(&time, sizeof(double), 1, fid);
        time += 1.0 / frameRate;
    }
    // Write dummy frame data
    uint32_t dataPoint;
        for (int i = 0; i < numFrames * numSamplers; ++i) {
            dataPoint = rand() % (dacMax - dacMin + 1) + dacMin;
            fwrite(&dataPoint, sizeof(uint32_t), 1, fid);
    }

    // Write dummy fpsEst value
    float fpsEst = (float)frameRate;
    fwrite(&fpsEst, sizeof(float), 1, fid);

    fclose(fid);
}

// Test salsaLoad with a valid file
void testSalsaLoadValidFile() {
    printf("Running testSalsaLoadValidFile...\n");
    const char *filename = "test.frames";
    int numFrames = 100;
    int numSamplers = 512;
    int frameRate = 200;
    createDummyFramesFile(filename, numFrames, numSamplers, frameRate);

    RadarData *radarData = salsaLoad(filename);
    assert(radarData != NULL);
    assert(radarData->numFrames == numFrames);
    assert(radarData->frameRate == frameRate);
    assert(radarData->times != NULL);
    assert(radarData->frameTot != NULL);

    // Check if data has been read into frameTot
    assert(radarData->frameTot[0] >= 0.0 && radarData->frameTot[0] <= 8191.0); 
    
    freeRadarData(radarData);
    remove(filename);
    printf("testSalsaLoadValidFile passed!\n");
}

// Test salsaLoad with an invalid file
void testSalsaLoadInvalidFile() {
    printf("Running testSalsaLoadInvalidFile...\n");
    RadarData *radarData = salsaLoad("invalid.frames");
    assert(radarData == NULL);
    printf("testSalsaLoadInvalidFile passed!\n");
}

// Test salsaLoad with an incorrect magic number
void testSalsaLoadIncorrectMagic() {
    printf("Running testSalsaLoadIncorrectMagic...\n");
    const char *filename = "test_incorrect_magic.frames";
    int numFrames = 100;
    int numSamplers = 512;
    int frameRate = 200;

    createDummyFramesFile(filename, numFrames, numSamplers, frameRate);
    FILE *fid = fopen(filename, "r+b");
    if (!fid) {
        perror("Error opening test file");
        return;
    }

    uint32_t incorrectMagic = 0x00000000;
    fseek(fid, 0, SEEK_SET);
    fwrite(&incorrectMagic, sizeof(uint32_t), 1, fid);
    fclose(fid);

    RadarData *radarData = salsaLoad(filename);
    assert(radarData == NULL);

    remove(filename);
    printf("testSalsaLoadIncorrectMagic passed!\n");
}

// Test salsaLoad with an incomplete header
void testSalsaLoadIncompleteHeader() {
    printf("Running testSalsaLoadIncompleteHeader...\n");

    // Store original function pointer
    SalsaLoadFunc originalSalsaLoad = g_salsaLoad;

    // Replace with failing mock
    g_salsaLoad = mockSalsaLoadFail;

    RadarData *radarData = g_salsaLoad("test_incomplete_header.frames");
    assert(radarData == NULL);

    // Restore original function
    g_salsaLoad = originalSalsaLoad;

    printf("testSalsaLoadIncompleteHeader passed!\n");
}

//Test salsaLoad with incomplete data
void testSalsaLoadIncompleteData(){
    printf("Running testSalsaLoadIncompleteData...\n");
    const char *filename = "test_incomplete_data.frames";
    int numFrames = 100;
    int numSamplers = 512;
    int frameRate = 200;

    createDummyFramesFile(filename, numFrames, numSamplers, frameRate);
    FILE *fid = fopen(filename, "r+b");
    if (!fid) {
        perror("Error opening test file");
        return;
    }

    fseek(fid, 0, SEEK_END);
    long fileSize = ftell(fid);

    fseek(fid, 0, SEEK_SET);
    ftruncate(fileno(fid), fileSize - (50 * sizeof(uint32_t)));
    fclose(fid);

    RadarData *radarData = salsaLoad(filename);
    assert(radarData == NULL);

    remove(filename);
    printf("testSalsaLoadIncompleteData passed!\n");
}

// Test freeRadarData with NULL
void testFreeRadarDataNull() {
    printf("Running testFreeRadarDataNull...\n");
    freeRadarData(NULL); // Should not crash
    printf("testFreeRadarDataNull passed!\n");
}
