#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>  // Add this for usleep
#include "../proc.h"
#include "../utils.h"
#include "../salsa.h"
#include "../wadar.h"
#include "test_mocks.h"

// --- TEST FUNCTIONS ---

// Test wadar function with mock functions
void testWadar() {
    printf("Running testWadar...\n");
    setupMocks();

    double vwc = g_wadar("user@ip:/path", "air.frames", "test", 80.0, 100, 2, 0.1);
    assert(vwc >= 0.0 && vwc <= 1.0);

    restoreMocks();
    printf("testWadar passed!\n");
}

// Test wadarAirCapture function
void testWadarAirCapture() {
    printf("Running testWadarAirCapture...\n");
    // Swap out real functions with mocks
    setupMocks();

    g_wadarAirCapture("user@ip:/path", "air.frames", 80.0, 100, 2);

    // Restore original functions
    restoreMocks();
    printf("testWadarAirCapture passed!\n");
}

// Test wadarTagTest function
void testWadarTagTest() {
    printf("Running testWadarTagTest...\n");
    // Swap out real functions with mocks
    setupMocks();

    double snr = g_wadarTagTest("user@ip:/path", "test", 80.0, 100, 2);

    // Assertions
    assert(snr == 20.0);

    // Restore original functions
    restoreMocks();

    printf("testWadarTagTest passed!\n");
}

// Test wadarTwoTag function
void testWadarTwoTag() {
    printf("Running testWadarTwoTag...\n");
    // Swap out real functions with mocks
    setupMocks();

    double vwc = g_wadarTwoTag("user@ip:/path", "test", 79.0, 80.0, 100, 2, 0.1);

    // Assertions
    assert(vwc >= 0.0 && vwc <= 1.0);

    // Restore original functions
    restoreMocks();
    printf("testWadarTwoTag passed!\n");
}

// Test wadarSaveData
void testWadarSaveData() {
    printf("Running testWadarSaveData...\n");
    
    // Store original function pointers
    WadarSaveDataFunc originalWadarSaveData = g_wadarSaveData;

    // Setup mock functions
    setupMocks();

    // Test data
    char fullDataPath[] = "test/path";
    char name[] = "test_data";
    double vwc = 0.5;
    double snr = 15.0;
    int peakBin = 256;

    // Call function through function pointer
    g_wadarSaveData(fullDataPath, name, vwc, snr, peakBin);

    // No need for file assertions since we're using mocks
    // The mock function will handle validation internally

    // Restore original functions
    restoreMocks();

    printf("testWadarSaveData passed!\n");
}

// Test main with different arguments
void testMain() {
    printf("Running testMain...\n");
    // Swap out real functions with mocks
    setupMocks();

    // Test "wadar"
    char *wadar_argv[] = {"wadar", "wadar", "-s", "user@ip:/path", "-b", "air.frames", "-t", "test", "-f", "80", "-c", "100", "-n", "2", "-d", "0.1"};
    int wadar_argc = sizeof(wadar_argv) / sizeof(wadar_argv[0]);
    assert(main(wadar_argc, wadar_argv) == 0);

    // Test "wadarAirCapture"
    char *wadarAirCapture_argv[] = {"wadar", "wadarAirCapture", "-s", "user@ip:/path", "-b", "air.frames", "-f", "80", "-c", "100", "-n", "2"};
    int wadarAirCapture_argc = sizeof(wadarAirCapture_argv) / sizeof(wadarAirCapture_argv[0]);
    assert(main(wadarAirCapture_argc, wadarAirCapture_argv) == 0);

    // Test "wadarTagTest"
    char *wadarTagTest_argv[] = {"wadar", "wadarTagTest", "-s", "user@ip:/path", "-t", "test", "-f", "80", "-c", "100", "-n", "2"};
    int wadarTagTest_argc = sizeof(wadarTagTest_argv) / sizeof(wadarTagTest_argv[0]);
    assert(main(wadarTagTest_argc, wadarTagTest_argv) == 0);

    // Test "wadarTwoTag"
    char *wadarTwoTag_argv[] = {"wadar", "wadarTwoTag", "-s", "user@ip:/path", "-t", "test", "-f", "79", "-g", "80", "-c", "100", "-n", "2", "-d", "0.1"};
    int wadarTwoTag_argc = sizeof(wadarTwoTag_argv) / sizeof(wadarTwoTag_argv[0]);
    assert(main(wadarTwoTag_argc, wadarTwoTag_argv) == 0);

    // Restore original functions
    restoreMocks();

    printf("testMain passed!\n");
}
//Add test for when tag test fail
void testMainTagTestFail() {
        printf("Running testMainTagTestFail...\n");
    // Swap out real functions with mocks
    setupMocks();

        // Test "wadarTagTest"
    char *wadarTagTest_argv[] = {"wadar", "wadarTagTest", "-s", "user@ip:/path", "-t", "test", "-f", "80", "-c", "100", "-n", "2"};
    int wadarTagTest_argc = sizeof(wadarTagTest_argv) / sizeof(wadarTagTest_argv[0]);
    assert(main(wadarTagTest_argc, wadarTagTest_argv) == 0);

    // Restore original functions
    restoreMocks();

    printf("testMainTagTestFail passed!\n");
}
