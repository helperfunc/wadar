#include <stdio.h>
#include "test_mocks.h"
#include "proc_tests.h"
#include "salsa_tests.h"
#include "utils_tests.h"
#include "wadar_tests.h"

int main() {
    printf("Running testMain...\n");
    
    // Initialize function pointers
    initProcTestFunctionPointers();
    initWadarTestFunctionPointers();

    // Run tests only once
    // Proc tests
    testProcRadarFrames();
    testProcRadarFramesFail();
    testProcLargestPeak();
    testProcCaptureCWT();
    testProcSoilMoisture();
    testProcTagTest();
    testProcTagTestFail();
    testProcTwoTag();
    testProcTwoTagFail();

    // Salsa tests
    testSalsaLoadValidFile();
    testSalsaLoadInvalidFile();
    testSalsaLoadIncorrectMagic();
    testSalsaLoadIncompleteHeader();
    testSalsaLoadIncompleteData();
    testFreeRadarDataNull();

    // Utils tests
    testNoveldaDDC();
    testHamming();
    testSmoothData();
    testComputeFFT();
    testFindPeaks();
    testCalculateSNR();
    testCompare();
    testMedian();

    // Wadar tests
    testWadar();
    testWadarAirCapture();
    testWadarTagTest();
    testWadarTwoTag();
    testWadarSaveData();

    return 0;
}