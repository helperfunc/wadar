#include <stdlib.h>
#include <math.h>
#include <assert.h>  // Add this for assert()
#include <string.h>  // For strstr()
#include <unistd.h>  // For usleep
#include "test_mocks.h"
#include "../utils.h"
#include "../wadar.h"
#include "../salsa.h"
#include "../proc.h"
#include <unistd.h>  // Add this for usleep


// Global function pointers
SalsaLoadFunc g_salsaLoad = NULL;
FindPeaksFunc g_findPeaks = NULL;
CalculateSNRFunc g_calculateSNR = NULL;
ProcRadarFramesFunc g_procRadarFrames = NULL;
ProcTagTestFunc g_procTagTest = NULL;
ProcTwoTagFunc g_procTwoTag = NULL;
SystemFunc g_system = NULL;
UsleepFunc g_usleep = NULL;
WadarSaveDataFunc g_wadarSaveData = NULL;
// Add global function pointer
ProcSoilMoistureFunc g_procSoilMoisture = NULL;
WadarFunc g_wadar = NULL;
WadarAirCaptureFunc g_wadarAirCapture = NULL;
WadarTagTestFunc g_wadarTagTest = NULL;
WadarTwoTagFunc g_wadarTwoTag = NULL;


// Original function pointers (internal to this file)
SalsaLoadFunc originalSalsaLoad = NULL;
ProcRadarFramesFunc originalProcRadarFrames = NULL;
ProcTagTestFunc originalProcTagTest = NULL;
ProcTwoTagFunc originalProcTwoTag = NULL;
SystemFunc originalSystem = NULL;
UsleepFunc originalUsleep = NULL;
WadarSaveDataFunc originalWadarSaveData = NULL;
ProcSoilMoistureFunc originalProcSoilMoisture = NULL;
WadarFunc originalWadar = NULL;
WadarAirCaptureFunc originalWadarAirCapture = NULL;
WadarTagTestFunc originalWadarTagTest = NULL;
WadarTwoTagFunc originalWadarTwoTag = NULL;

double mockWadar(char* fullDataPath, char* airFramesName, char* trialName, 
    double tagHz, int frameCount, int captureCount, double tagDepth) {
return 0.3; // Return a realistic VWC value
}

void mockWadarAirCapture(char* fullDataPath, char* airFramesName, 
            double tagHz, int frameCount, int captureCount) {
// Mock implementation - does nothing but simulates success
}

double mockWadarTagTest(char* fullDataPath, char* trialName, 
           double tagHz, int frameCount, int captureCount) {
return 20.0; // Return a realistic SNR value
}

double mockWadarTwoTag(char* fullDataPath, char* trialName, 
          double tag1Hz, double tag2Hz, int frameCount, 
          int captureCount, double tagDiff) {
return 0.4; // Return a realistic VWC value
}


void setupMocks(void) {
    // Store original functions
    originalProcRadarFrames = g_procRadarFrames;
    originalProcTagTest = g_procTagTest;
    originalProcTwoTag = g_procTwoTag;
    originalSystem = g_system;
    originalUsleep = g_usleep;
    originalWadarSaveData = g_wadarSaveData;
    originalWadar = g_wadar;
    originalWadarAirCapture = g_wadarAirCapture;
    originalWadarTagTest = g_wadarTagTest;
    originalWadarTwoTag = g_wadarTwoTag;

    // Set mock functions
    g_procRadarFrames = mockProcRadarFrames;
    g_procTagTest = mockProcTagTest;
    g_procTwoTag = mockProcTwoTag;
    g_system = mockSystem;
    g_usleep = mockUsleep;
    g_wadarSaveData = mockWadarSaveData;
    g_wadar = mockWadar;
    g_wadarAirCapture = mockWadarAirCapture;
    g_wadarTagTest = mockWadarTagTest;
    g_wadarTwoTag = mockWadarTwoTag;

    originalProcSoilMoisture = g_procSoilMoisture;
    g_procSoilMoisture = mockProcSoilMoisture;
}

void restoreMocks(void) {
    // Restore original functions
    g_salsaLoad = originalSalsaLoad;
    g_procRadarFrames = originalProcRadarFrames;
    g_procTagTest = originalProcTagTest;
    g_procTwoTag = originalProcTwoTag;
    g_system = originalSystem;
    g_usleep = originalUsleep;
    g_wadarSaveData = originalWadarSaveData;
    g_procSoilMoisture = originalProcSoilMoisture;
    g_wadar = originalWadar;
    g_wadarAirCapture = originalWadarAirCapture;
    g_wadarTagTest = originalWadarTagTest;
    g_wadarTwoTag = originalWadarTwoTag;
}


double mockProcSoilMoisture(double freq1, double freq2, const char* soilType, double temp) {
    if (strcmp(soilType, "farm") == 0) {
        return 0.5; // Return a valid VWC value between 0 and 1
    }
    return -1.0; // Return error for invalid soil type
}

void initProcTestFunctionPointers() {
    g_salsaLoad = salsaLoad;
    g_findPeaks = findPeaks;
    g_calculateSNR = calculateSNR;
}

void initWadarTestFunctionPointers() {
    // Initialize all wadar-related function pointers
    g_procRadarFrames = procRadarFrames;
    g_procTagTest = procTagTest;
    g_procTwoTag = procTwoTag;
    g_system = system;
    g_usleep = usleep;
    g_wadarSaveData = wadarSaveData;
}

// Mock implementations
CaptureData* mockProcRadarFrames(const char* fullDataPath, const char* captureName, double tagHz) {
    // Allocate and initialize CaptureData with test values
    CaptureData* captureData = (CaptureData*)malloc(sizeof(CaptureData));
    if (!captureData) return NULL;

    // Initialize with known test values
    captureData->procSuccess = true;
    captureData->peakBin = 256;
    captureData->SNRdB = 20.0;
    captureData->numFrames = 100;
    
    // Allocate arrays with proper size
    captureData->tagFT = (double*)malloc(512 * sizeof(double));
    captureData->captureFT = (double complex*)malloc(100 * 512 * sizeof(double complex));
    
    if (!captureData->tagFT || !captureData->captureFT) {
        freeCaptureData(captureData);
        return NULL;
    }

    // Initialize arrays with test data
    for (int i = 0; i < 512; i++) {
        captureData->tagFT[i] = 0.0;
    }
    captureData->tagFT[256] = 10.0;  // Set peak

    for (int i = 0; i < 100 * 512; i++) {
        captureData->captureFT[i] = 0.0 + 0.0 * I;
    }

    return captureData;
}

double mockProcTagTest(const char* fullDataPath, const char* captureName, double tagHz) {
    return 20.0;
}

double mockProcTagTestFail(const char* fullDataPath, const char* captureName, double tagHz) {
    return -1.0;
}

CaptureData* mockProcTwoTag(const char* fullDataPath, const char* captureName, double tag1Hz, double tag2Hz) {
    CaptureData* captureData = (CaptureData*)malloc(sizeof(CaptureData));
    if (!captureData) return NULL;

    captureData->procSuccess = true;
    captureData->peakBin = 256;
    captureData->peakBin2 = 260;
    captureData->SNRdB = 20.0;
    captureData->SNRdB2 = 21.0;
    captureData->numFrames = 100;
    captureData->tagFT = (double*)malloc(512 * sizeof(double));
    captureData->tagFT2 = (double*)malloc(512 * sizeof(double));
    
    if (!captureData->tagFT || !captureData->tagFT2) {
        free(captureData->tagFT);
        free(captureData->tagFT2);
        free(captureData);
        return NULL;
    }

    for (int i = 0; i < 512; i++) {
        captureData->tagFT[i] = 1.0;
        captureData->tagFT2[i] = 1.0;
    }
    return captureData;
}

int mockSystem(const char* command) {
    return (strstr(command, "frameLogger") != NULL) ? 0 : -1;
}

int mockUsleep(useconds_t usec) {
    return 0;
}

void mockWadarSaveData(char* fullDataPath, char* name, double vwc, double snr, int peakBin) {
    // Validate input parameters
    assert(fullDataPath != NULL);
    assert(name != NULL);
    assert(vwc >= 0.0 && vwc <= 1.0);
    assert(snr >= 0.0);
    assert(peakBin >= 0 && peakBin < 512);
    
    // Mock implementation doesn't actually write to file
    // Just validates the parameters are correct
}

CaptureData* mockProcRadarFramesFail(const char* path, const char* name, double tagHz) {
    return NULL;
}

CaptureData* mockProcTwoTagFail(const char* fullDataPath, const char* captureName, 
    double tag1Hz, double tag2Hz) {
return NULL;  // Simulate failure case
}

RadarData* mockSalsaLoad(const char* fileName) {
    RadarData* radarData = (RadarData*)malloc(sizeof(RadarData));
    if (!radarData) return NULL;

    radarData->numFrames = 100;
    radarData->frameRate = 200;
    radarData->times = (double*)malloc(radarData->numFrames * sizeof(double));
    radarData->frameTot = (double*)malloc(radarData->numFrames * 512 * sizeof(double));

    if (!radarData->times || !radarData->frameTot) {
        free(radarData->times);
        free(radarData->frameTot);
        free(radarData);
        return NULL;
    }

    for (int i = 0; i < radarData->numFrames; i++) {
        radarData->times[i] = (double)i;
        for (int j = 0; j < 512; j++) {
            radarData->frameTot[j + i * 512] = sin(2 * M_PI * 0.1 * i) + (double)rand()/RAND_MAX * 0.1;
        }
    }

    return radarData;
}

RadarData* mockSalsaLoadFail(const char* fileName) {
    return NULL;
}

int* mockFindPeaks(double* arr, int size, int* numPeaks, double minPeakHeight) {
    int* peaks = (int*)malloc(size * sizeof(int));
    *numPeaks = 0;
    if (size > 0) {
        peaks[0] = size / 2;
        *numPeaks = 1;
    }
    return peaks;
}

double mockCalculateSNR(double complex* captureFT, int numOfSamplers, int freqTag, int peakBin) {
    return 20.0;
}