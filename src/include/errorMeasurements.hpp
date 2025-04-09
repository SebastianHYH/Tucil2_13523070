#ifndef ERRORMEASUREMENTS_H
#define ERRORMEASUREMENTS_H

#include <vector>

class ErrorMeasurementMethod {
public:
    static double computeVariance(const std::vector<std::vector<std::vector<int>>>& block);
    static double computeMAD(const std::vector<std::vector<std::vector<int>>>& block);
    static double computeMaxPixelDiff(const std::vector<std::vector<std::vector<int>>>& block);
    static double computeEntropy(const std::vector<std::vector<std::vector<int>>>& block);
};

#endif
