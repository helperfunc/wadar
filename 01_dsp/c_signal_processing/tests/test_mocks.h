#ifndef TEST_MOCKS_H
#define TEST_MOCKS_H

#include <complex.h>
#include <unistd.h>  // For useconds_t and usleep
#include "../proc.h"
#include "../salsa.h"
#include "../wadar.h"

// Function pointer types
typedef RadarData* (*SalsaLoadFunc)(const char*);
typedef int* (*FindPeaksFunc)(double*, int, int*, double);
typedef double (*CalculateSNRFunc)(double complex*, int, int, int);
typedef CaptureData* (*ProcRadarFramesFunc)(const char*, const char*, double);
typedef double (*ProcTagTestFunc)(const char*, const char*, double);
typedef CaptureData* (*ProcTwoTagFunc)(const char*, const char*, double, double);
typedef int (*SystemFunc)(const char*);
typedef int (*UsleepFunc)(useconds_t usec);
typedef void (*WadarSaveDataFunc)(char*, char*, double, double, int);
// Function pointer types for wadar functions
typedef double (*WadarFunc)(char*, char*, char*, double, int, int, double);
typedef void (*WadarAirCaptureFunc)(char*, char*, double, int, int);
typedef double (*WadarTagTestFunc)(char*, char*, double, int, int);
typedef double (*WadarTwoTagFunc)(char*, char*, double, double, int, int, double);
// Function pointer type for soil moisture calculation
typedef double (*ProcSoilMoistureFunc)(double, double, const char*, double);

// Global function pointers
extern WadarFunc g_wadar;
extern WadarAirCaptureFunc g_wadarAirCapture;
extern WadarTagTestFunc g_wadarTagTest;
extern WadarTwoTagFunc g_wadarTwoTag;

// Global function pointers
extern SalsaLoadFunc g_salsaLoad;
extern FindPeaksFunc g_findPeaks;
extern CalculateSNRFunc g_calculateSNR;
extern ProcRadarFramesFunc g_procRadarFrames;
extern ProcTagTestFunc g_procTagTest;
extern ProcTwoTagFunc g_procTwoTag;
extern SystemFunc g_system;
extern UsleepFunc g_usleep;
extern WadarSaveDataFunc g_wadarSaveData;
// Add to global function pointers
extern ProcSoilMoistureFunc g_procSoilMoisture;


// extern ProcRadarFramesFunc g_originalProcRadarFrames;
// extern ProcTagTestFunc g_originalProcTagTest;
// extern ProcTwoTagFunc g_originalProcTwoTag;
// extern SystemFunc g_originalSystem;
// extern UsleepFunc g_originalUsleep;
// extern WadarSaveDataFunc g_originalWadarSaveData;

// Mock function declarations
extern CaptureData* mockProcRadarFrames(const char* path, const char* name, double tagHz);
extern double mockProcTagTest(const char* fullDataPath, const char* captureName, double tagHz);
extern double mockProcTagTestFail(const char* fullDataPath, const char* captureName, double tagHz);
extern CaptureData* mockProcTwoTag(const char* fullDataPath, const char* captureName, double tag1Hz, double tag2Hz);
extern int mockSystem(const char* command);
extern int mockUsleep(useconds_t usec);
extern void mockWadarSaveData(char* fullDataPath, char* name, double vwc, double snr, int peakBin);
extern CaptureData* mockProcRadarFramesFail(const char* path, const char* name, double tagHz);
extern RadarData* mockSalsaLoad(const char* fileName);
extern RadarData* mockSalsaLoadFail(const char* fileName);
extern int* mockFindPeaks(double* arr, int size, int* numPeaks, double minPeakHeight);
extern double mockCalculateSNR(double complex* captureFT, int numOfSamplers, int freqTag, int peakBin);
extern CaptureData* mockProcTwoTagFail(const char* fullDataPath, const char* captureName, 
    double tag1Hz, double tag2Hz);

// Mock function declarations
extern double mockWadar(char* fullDataPath, char* airFramesName, char* trialName, 
    double tagHz, int frameCount, int captureCount, double tagDepth);
extern void mockWadarAirCapture(char* fullDataPath, char* airFramesName, 
            double tagHz, int frameCount, int captureCount);
extern double mockWadarTagTest(char* fullDataPath, char* trialName, 
           double tagHz, int frameCount, int captureCount);
extern double mockWadarTwoTag(char* fullDataPath, char* trialName, 
          double tag1Hz, double tag2Hz, int frameCount, 
          int captureCount, double tagDiff);
// Add mock function declaration
extern double mockProcSoilMoisture(double freq1, double freq2, const char* soilType, double temp);

// Function declarations
void initProcTestFunctionPointers(void);
void initWadarTestFunctionPointers(void);
// Function declarations
void setupMocks(void);
void restoreMocks(void);

#endif // TEST_MOCKS_H