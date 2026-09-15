#ifndef ERRORSENGINE_H
#define ERRORSENGINE_H

#include <vector>

namespace ErrCalcEngine {

struct InputData
{
    std::vector<double> data;
    double confidence;
    double instErr;
};

struct StatResults
{
    double sum = std::numeric_limits<double>::quiet_NaN();
    double mean = std::numeric_limits<double>::quiet_NaN();;
    double dispersion = std::numeric_limits<double>::quiet_NaN();;
    double standartError = std::numeric_limits<double>::quiet_NaN();;
    double t_test = std::numeric_limits<double>::quiet_NaN();;
    double absoluteError = std::numeric_limits<double>::quiet_NaN();;
    size_t count = 0;
};

double T_test(InputData data);
StatResults CalculateStatResults(InputData data);
}

#endif // ERRORSENGINE_H
