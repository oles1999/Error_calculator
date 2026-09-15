#include "errorsEngine.h"
#include <cmath>
#include <boost/math/statistics/univariate_statistics.hpp>
#include <boost/math/statistics/t_test.hpp>
#include <numeric>

double ErrCalcEngine::T_test(InputData data)
{
    double res;
    boost::math::students_t dist(static_cast<double>(data.data.size() - 1));
    double p = 1.0 - (1.0 - data.confidence) / 2;
    res = boost::math::quantile(dist, p);
    return res;
}

ErrCalcEngine::StatResults ErrCalcEngine::CalculateStatResults(InputData data)
{
    StatResults res;
    res.count = data.data.size();
    if (res.count < 2){
        return res;
    }

    res.sum = std::accumulate(data.data.begin(), data.data.end(), 0.0);
    res.count = data.data.size();
    res.mean = res.sum / res.count;
    res.dispersion = boost::math::statistics::sample_variance(data.data);
    res.standartError = std::sqrt(res.dispersion / res.count);
    res.t_test = T_test(data);
    res.absoluteError = res.t_test * res.standartError;
    if (data.instErr != 0)
        res.absoluteError = std::hypot(res.absoluteError, data.instErr);
    return res;

}
