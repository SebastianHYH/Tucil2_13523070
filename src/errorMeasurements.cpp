#include "include\errorMeasurements.hpp"
#include <cmath>
#include <algorithm>
#include <unordered_map>
using namespace std;

double ErrorMeasurementMethod::computeVariance(const vector<vector<vector<int>>>& block) {
    double varianceR = 0.0, varianceG = 0.0, varianceB = 0.0;
    int totalPixels = block.size() * block[0].size();

    double meanR = 0.0, meanG = 0.0, meanB = 0.0;
    for (const auto& row : block) {
        for (const auto& pixel : row) {
            meanR += pixel[0];
            meanG += pixel[1];
            meanB += pixel[2];
        }
    }
    meanR /= totalPixels;
    meanG /= totalPixels;
    meanB /= totalPixels;

    for (const auto& row : block) {
        for (const auto& pixel : row) {
            varianceR += (pixel[0] - meanR) * (pixel[0] - meanR);
            varianceG += (pixel[1] - meanG) * (pixel[1] - meanG);
            varianceB += (pixel[2] - meanB) * (pixel[2] - meanB);
        }
    }
    varianceR /= totalPixels;
    varianceG /= totalPixels;
    varianceB /= totalPixels;

    return (varianceR + varianceG + varianceB) / 3.0;
}


double ErrorMeasurementMethod::computeMAD(const vector<vector<vector<int>>>& block) {
    double MADR = 0.0, MADG = 0.0, MADB = 0.0;
    int totalPixels = block.size() * block[0].size();

    double meanR = 0, meanG = 0, meanB = 0;
    for (const auto& row : block) {
        for (const auto& pixel : row) {
            meanR += pixel[0];
            meanG += pixel[1];
            meanB += pixel[2];
        }
    }
    meanR /= totalPixels;
    meanG /= totalPixels;
    meanB /= totalPixels;

    for (const auto& row : block) {
        for (const auto& pixel : row) {
            MADR += abs(pixel[0] - meanR);
            MADG += abs(pixel[1] - meanG);
            MADB += abs(pixel[2] - meanB);
        }
    }
    MADR /= totalPixels;
    MADG /= totalPixels;
    MADB /= totalPixels;

    return (MADR + MADG + MADB) / 3.0;
}

double ErrorMeasurementMethod::computeMaxPixelDiff(const vector<vector<vector<int>>>& block) {
    double maxR = 0.0, maxG = 0.0, maxB = 0.0, minR = 255.0, minG = 255.0, minB = 255.0;
    int totalPixels = block.size() * block[0].size();

    for (const auto& row : block) {
        for (const auto& pixel : row) {
            if (pixel[0] > maxR) maxR = pixel[0];
            if (pixel[0] < minR) minR = pixel[0];
            if (pixel[1] > maxG) maxG = pixel[1];
            if (pixel[1] < minG) minG = pixel[1];
            if (pixel[2] > maxB) maxB = pixel[2];
            if (pixel[2] < minB) minB = pixel[2];
        }
    }

    return ((maxR - minR) + (maxG - minG) + (maxB - minB)) / 3.0;
}

double ErrorMeasurementMethod::computeEntropy(const vector<vector<vector<int>>>& block) {
    unordered_map<int, int> entR, entG, entB;
    int totalPixels = block.size() * block[0].size();

    for (const auto& row : block) {
        for (const auto& pixel : row) {
            entR[pixel[0]]++; 
            entG[pixel[1]]++; 
            entB[pixel[2]]++; 
        }
    }

    auto calculateEntropy = [&](const unordered_map<int, int>& freqMap) -> double {
        double entropy = 0.0;
        for (const auto& [value, count] : freqMap) {
            double probability = static_cast<double>(count) / totalPixels;
            if (probability > 0) {
                entropy -= probability * log2(probability);
            }
        }
        return entropy;
    };

    double entropyR = calculateEntropy(entR);
    double entropyG = calculateEntropy(entG);
    double entropyB = calculateEntropy(entB);

    return (entropyR + entropyG + entropyB) / 3.0;
}