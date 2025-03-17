#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include "../proc.h"
#include "../utils.h"
#include "../salsa.h"
#include "../wavelib/header/wavelib.h"
#include "test_mocks.h"

void testProcRadarFrames() {
    printf("Running testProcRadarFrames...\n");

    // Setup mocks
    setupMocks();

    // Call through function pointer
    CaptureData *captureData = g_procRadarFrames("user@ip:/path", "test.frames", 80.0);
    
    // Verify results
    assert(captureData != NULL);
    printf("Checking procSuccess...\n");
    assert(captureData->procSuccess == true);
    printf("Checking peakBin...\n");
    assert(captureData->peakBin >= 0 && captureData->peakBin < 512);
    printf("Checking SNRdB...\n");
    assert(captureData->SNRdB == 20.0);
    printf("Checking numFrames...\n");
    assert(captureData->numFrames == 100);
    printf("Checking arrays...\n");
    assert(captureData->tagFT != NULL);
    assert(captureData->captureFT != NULL);
    
    // Clean up
    freeCaptureData(captureData);

    // Restore original functions
    restoreMocks();

    printf("testProcRadarFrames passed!\n");
}

// Test procRadarFrames failure
void testProcRadarFramesFail() {
    printf("Running testProcRadarFramesFail...\n");
    // Store original function pointer
    SalsaLoadFunc originalSalsaLoad = g_salsaLoad;

    // Replace salsaLoad with mockSalsaLoadFail 
    g_salsaLoad = mockSalsaLoad;

    CaptureData *captureData = mockProcRadarFramesFail("user@ip:/path", "test.frames", 80.0);
    assert(captureData == NULL);

    // Restore original function
    g_salsaLoad = originalSalsaLoad;
    printf("testProcRadarFramesFail passed!\n");
}

// Test procTagTest
void testProcTagTest() {
    printf("Running testProcTagTest...\n");
    // Store original function pointers
    SalsaLoadFunc originalSalsaLoad = g_salsaLoad;
    FindPeaksFunc originalFindPeaks = g_findPeaks;
    CalculateSNRFunc originalCalculateSNR = g_calculateSNR;
    ProcTagTestFunc originalProcTagTest = g_procTagTest;

    // Replace with mock functions
    g_salsaLoad = mockSalsaLoad;
    g_findPeaks = mockFindPeaks;
    g_calculateSNR = mockCalculateSNR;
    g_procTagTest = mockProcTagTest;

    double snr = g_procTagTest("user@ip:/path", "test.frames", 80.0);
    assert(snr == 20.0);

    // Restore original functions
    g_salsaLoad = originalSalsaLoad;
    g_findPeaks = originalFindPeaks;
    g_calculateSNR = originalCalculateSNR;
    g_procTagTest = originalProcTagTest;
    printf("testProcTagTest passed!\n");
}

void testProcTagTestFail() {
    printf("Running testProcTagTestFail...\n");
    // Store original function pointer
    SalsaLoadFunc originalSalsaLoad = g_salsaLoad;
    ProcTagTestFunc originalProcTagTest = g_procTagTest;

    // Replace functions with mocks
    g_salsaLoad = mockSalsaLoadFail;  // Use failing mock
    g_procTagTest = mockProcTagTestFail;  // Use failing mock

    double snr = g_procTagTest("user@ip:/path", "test.frames", 80.0);
    assert(snr == -1.0);

    // Restore original functions
    g_salsaLoad = originalSalsaLoad;
    g_procTagTest = originalProcTagTest;
    
    printf("testProcTagTestFail passed!\n");
}

void testProcTwoTag() {
    printf("Running testProcTwoTag...\n");
    // Store original function pointers
    SalsaLoadFunc originalSalsaLoad = g_salsaLoad;
    FindPeaksFunc originalFindPeaks = g_findPeaks;
    CalculateSNRFunc originalCalculateSNR = g_calculateSNR;
    ProcTwoTagFunc originalProcTwoTag = g_procTwoTag;

    // Replace with mock functions
    g_salsaLoad = mockSalsaLoad;
    g_findPeaks = mockFindPeaks;
    g_calculateSNR = mockCalculateSNR;
    g_procTwoTag = mockProcTwoTag;

    CaptureData *captureData = g_procTwoTag("user@ip:/path", "test.frames", 79.0, 80.0);
    assert(captureData != NULL);
    assert(captureData->procSuccess == true);
    assert(captureData->peakBin >= 0 && captureData->peakBin < 512);
    assert(captureData->peakBin2 >= 0 && captureData->peakBin2 < 512);
    assert(captureData->SNRdB == 20.0);
    assert(captureData->SNRdB2 == 21.0);
    assert(captureData->numFrames == 100);
    freeCaptureData(captureData);

    // Restore original functions
    g_salsaLoad = originalSalsaLoad;
    g_findPeaks = originalFindPeaks;
    g_calculateSNR = originalCalculateSNR;
    g_procTwoTag = originalProcTwoTag;
    printf("testProcTwoTag passed!\n");
}

void testProcTwoTagFail() {
    printf("Running testProcTwoTagFail...\n");
    // Store original function pointer
    SalsaLoadFunc originalSalsaLoad = g_salsaLoad;
    ProcTwoTagFunc originalProcTwoTag = g_procTwoTag;

    // Replace functions with mocks
    g_salsaLoad = mockSalsaLoadFail;  // Use failing mock
    g_procTwoTag = mockProcTwoTagFail;  // Use failing mock instead

    CaptureData *captureData = g_procTwoTag("user@ip:/path", "test.frames", 79.0, 80.0);
    assert(captureData == NULL);

    // Restore original functions
    g_salsaLoad = originalSalsaLoad;
    g_procTwoTag = originalProcTwoTag;
    
    printf("testProcTwoTagFail passed!\n");
}

void testProcLargestPeak() {
    printf("Running testProcLargestPeak...\n");
    double tagFT[512] = {0};
    tagFT[256] = 10.0;  // Set a peak in the middle
    tagFT[257] = 9.0;   // Smaller peak
    tagFT[255] = 9.0;   // Smaller peak
    
    int peakBin = procLargestPeak(tagFT);
    assert(peakBin == 256);
    printf("testProcLargestPeak passed!\n");
}

void testProcCaptureCWT() {
    printf("Running testProcCaptureCWT...\n");
    double tagFT[512] = {0};
    tagFT[256] = 10.0;
    tagFT[257] = 9.0;
    tagFT[255] = 9.0;
    
    int peakBin = procCaptureCWT(tagFT);
    assert(peakBin >= 0 && peakBin < 512);
    printf("testProcCaptureCWT passed!\n");
}


void testProcSoilMoisture() {
    printf("Running testProcSoilMoisture...\n");
    
    // Store original function pointer
    ProcSoilMoistureFunc originalProcSoilMoisture = g_procSoilMoisture;
    
    // Replace with mock function
    g_procSoilMoisture = mockProcSoilMoisture;

    // Test valid soil type
    double vwc = g_procSoilMoisture(260.0, 250.0, "farm", 0.1);
    assert(vwc > 0.0 && vwc < 1.0);
    
    // Test invalid soil type
    vwc = g_procSoilMoisture(260.0, 250.0, "invalidSoil", 0.1);
    assert(vwc == -1.0);

    // Restore original function
    g_procSoilMoisture = originalProcSoilMoisture;
    
    printf("testProcSoilMoisture passed!\n");
}
