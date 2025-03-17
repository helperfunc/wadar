#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <assert.h>
#include <fftw3.h>
#include "../utils.h"
#include "../salsa.h" // Include salsa.h for freeRadarData

// Helper function to print complex numbers for debugging
void printComplex(const char *label, double complex num) {
    printf("%s: %f + %fi\n", label, creal(num), cimag(num));
}

// --- Test NoveldaDDC ---
void testNoveldaDDC() {
    printf("Running testNoveldaDDC...\n");
    int frameSize = 512;
    double *rfSignal = (double *)malloc(frameSize * sizeof(double));
    double complex *basebandSignal = (double complex *)malloc(frameSize * sizeof(double complex));

    // Initialize rfSignal with a simple sine wave
    for (int i = 0; i < frameSize; i++) {
        rfSignal[i] = sin(2 * M_PI * 0.1 * i); // 0.1 cycles per sample
    }

    NoveldaDDC(rfSignal, basebandSignal);

    // Basic checks to ensure the function runs without crashing
    assert(basebandSignal != NULL);
    // Further checks can be made here based on the expected output
    // For example, we can check if the downconverted signal has a non-zero magnitude

    bool nonZeroFound = false;
    for(int i = 0; i < frameSize; ++i) {
        if(cabs(basebandSignal[i]) > 0.0001) {
            nonZeroFound = true;
            break;
        }
    }
    assert(nonZeroFound);

    free(rfSignal);
    free(basebandSignal);
    printf("testNoveldaDDC passed!\n");
}

// --- Test Hamming ---
void testHamming() {
    printf("Running testHamming...\n");
    int M = 10;
    double window[M + 1];

    hamming(window, M);

    // Check if the window has the correct length
    assert(sizeof(window) / sizeof(window[0]) == M + 1);

    // Check if the window is symmetric
    for (int i = 0; i <= M / 2; i++) {
        assert(fabs(window[i] - window[M - i]) < 1e-10);
    }

    // Check a few specific values
    assert(fabs(window[0] - 0.08) < 1e-10);
    assert(fabs(window[M/2] - 1.0) < 1e-10);
    assert(fabs(window[M] - 0.08) < 1e-10);

    printf("testHamming passed!\n");
}

// --- Test SmoothData ---
void testSmoothData() {
    printf("Running testSmoothData...\n");
    int length = 100;
    int windowSize = 5;
    double *data = (double *)malloc(length * sizeof(double));
    assert(data != NULL);

    // Initialize data with a sine wave plus noise for better smoothing test
    for (int i = 0; i < length; i++) {
        // Create a noisy sine wave
        data[i] = 10.0 * sin(2.0 * M_PI * i / 20.0) + ((double)rand() / RAND_MAX - 0.5) * 2.0;
    }

    // Store original values
    double *originalData = (double *)malloc(length * sizeof(double));
    assert(originalData != NULL);
    memcpy(originalData, data, length * sizeof(double));

    // Apply smoothing
    smoothData(data, length, windowSize);
    
    // Print some debug values
    printf("First few original values: %.2f, %.2f, %.2f\n", 
           originalData[0], originalData[1], originalData[2]);
    printf("First few smoothed values: %.2f, %.2f, %.2f\n", 
           data[0], data[1], data[2]);

    // Test if the data has been smoothed (variation should be less)
    double originalVariation = 0.0;
    double smoothedVariation = 0.0;
    for (int i = 1; i < length; i++) {
        originalVariation += fabs(originalData[i] - originalData[i-1]);
        smoothedVariation += fabs(data[i] - data[i-1]);
    }
    
    printf("Original variation: %.2f\n", originalVariation);
    printf("Smoothed variation: %.2f\n", smoothedVariation);
    
    // Smoothed data should have less variation
    assert(smoothedVariation < originalVariation);

    free(originalData);
    free(data);
    printf("testSmoothData passed!\n");
}

// --- Test ComputeFFT ---
void testComputeFFT() {
    printf("Running testComputeFFT...\n");
    int numFrames = 64;
    int numOfSamplers = 512;

    double complex *framesBB = (double complex *)malloc(numFrames * numOfSamplers * sizeof(double complex));
    double complex *captureFT = (double complex *)malloc(numFrames * numOfSamplers * sizeof(double complex));

    // Initialize framesBB with a simple sine wave
    for (int i = 0; i < numFrames; i++) {
        for (int j = 0; j < numOfSamplers; j++) {
            framesBB[j + i * numOfSamplers] = sin(2 * M_PI * 0.1 * i) + I * 0.0;
        }
    }
    
    computeFFT(framesBB, captureFT, numFrames, numOfSamplers);

    // Basic check: Ensure the function runs without crashing
    assert(captureFT != NULL);

    //Check that the results are not all zeros
    bool nonZeroFound = false;
    for(int i = 0; i < numFrames * numOfSamplers; ++i) {
        if(cabs(captureFT[i]) > 0.0001) {
            nonZeroFound = true;
            break;
        }
    }
    assert(nonZeroFound);

    free(framesBB);
    free(captureFT);
    printf("testComputeFFT passed!\n");
}

// --- Test findPeaks ---
void testFindPeaks() {
    printf("Running testFindPeaks...\n");
    int size = 100;
    double arr[size];
    int numPeaks;
    double minPeakHeight = 5.0;

    // Create an array with 3 clear peaks
    for (int i = 0; i < size; i++) {
        arr[i] = 0.0;
    }
    arr[20] = 10.0;
    arr[50] = 8.0;
    arr[80] = 12.0;

    int *peaks = findPeaks(arr, size, &numPeaks, minPeakHeight);

    // Check the number of peaks found
    assert(numPeaks == 3);

    // Check the location of the peaks
    assert(peaks[0] == 20);
    assert(peaks[1] == 50);
    assert(peaks[2] == 80);

    free(peaks);
    printf("testFindPeaks passed!\n");
}

// --- Test calculateSNR ---
void testCalculateSNR() {
    printf("Running testCalculateSNR...\n");
    int numOfSamplers = 512;
    int numFrames = 64;
    int freqTag = 10;
    int peakBin = 256;

    double complex *captureFT = (double complex *)malloc(numFrames * numOfSamplers * sizeof(double complex));
    assert(captureFT != NULL);

    // Initialize array to zero
    memset(captureFT, 0, numFrames * numOfSamplers * sizeof(double complex));

    // Add noise in the frequency range used for SNR calculation (0.945 to 0.955 of freqTag)
    int noiseFreqLowBound = (int)(freqTag * 0.945);  // = 9
    int noiseFreqHighBound = (int)(freqTag * 0.955); // = 9
    
    // Add noise to multiple points for better averaging
    for (int j = noiseFreqLowBound - 1; j <= noiseFreqHighBound + 1; j++) {
        if (j == freqTag) continue;  // Skip the signal frequency
        int noiseIndex = peakBin + numOfSamplers * (j - 1);
        if (noiseIndex >= 0 && noiseIndex < numFrames * numOfSamplers) {
            captureFT[noiseIndex] = 0.01 + 0.01 * I;  // Reduced noise level
        }
    }

    // Create signal peak at the tag frequency
    int peakIndex = peakBin + numOfSamplers * (freqTag - 1);
    captureFT[peakIndex] = 10.0 + 0.0 * I;  // Strong signal

    // Print debug info
    printf("Signal frequency: %d\n", freqTag);
    printf("Peak magnitude at index %d: %f\n", peakIndex, cabs(captureFT[peakIndex]));
    printf("Noise frequency range: %d to %d\n", noiseFreqLowBound - 1, noiseFreqHighBound + 1);
    printf("Sample noise magnitude: %f\n", cabs(0.01 + 0.01 * I));

    double snr = calculateSNR(captureFT, numOfSamplers, freqTag, peakBin);
    printf("Calculated SNR: %f dB\n", snr);

    // With signal = 10.0 and noise = 0.01√2, SNR should be around 10 * log10(10/(0.01√2)) ≈ 28 dB
    assert(snr > 20.0);  // Lower bound
    assert(snr < 30.0);  // Upper bound

    free(captureFT);
    printf("testCalculateSNR passed!\n");
}

// --- Test compare ---
void testCompare() {
    printf("Running testCompare...\n");
    double a = 5.0;
    double b = 10.0;
    double c = 5.0;

    // Test a < b
    assert(compare(&a, &b) < 0);

    // Test a > b
    assert(compare(&b, &a) > 0);

    // Test a == c
    assert(compare(&a, &c) == 0);
    
    printf("testCompare passed!\n");
}

// --- Test median ---
void testMedian() {
    printf("Running testMedian...\n");
    double arr1[] = {1.0, 2.0, 3.0, 4.0};
    double arr2[] = {1.0, 2.0, 3.0};
    double arr3[] = {4.0, 1.0, 2.0, 3.0}; // Unsorted

    // Test even length
    assert(fabs(median(arr1, sizeof(arr1)/sizeof(arr1[0])) - 2.5) < 1e-10);

    // Test odd length
    assert(fabs(median(arr2, sizeof(arr2)/sizeof(arr2[0])) - 2.0) < 1e-10);
    
    //Test unsorted
    assert(fabs(median(arr3, sizeof(arr3)/sizeof(arr3[0])) - 2.5) < 1e-10);

    printf("testMedian passed!\n");
}
