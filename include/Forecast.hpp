#ifndef _FORECAST_HPP_
#define _FORECAST_HPP_

#include <numeric>

class Forecast{
public:
    template<typename Container>
    static double make_forecast(Container&&);

private:
    template<typename Container>
    double get_mean(Container&&) const;

    template<typename Container>
    double get_variance(Container&&) const;

    template<typename Mean>
    double probability_geometric(Mean&&) const;

    template<typename Mean, typename N>
    double probability_poisson(Mean&&, N&&) const;
};

template<typename Container>
double Forecast::get_mean(Container&& sample) const
{
    if(sample.size() == 0)
        return 0;
    double result = 0;
    result = std::accumulate(std::begin(std::forward<Container>(sample)), std::end(std::forward<Container>(sample)), result);
    return result / sample.size();
}

template<typename Container>
double Forecast::get_variance(Container&& sample) const
{
    double mean_square = 0;
    auto iter_end = std::end(std::forward<Container>(sample));
    for(auto iter = std::begin(std::forward<Container>(sample)); iter != iter_end; iter++){
        mean_square += (*iter)*(*iter);
    }
    if(sample.size() != 0)
        mean_square /= sample.size();

    double mean = get_mean(std::forward<Container>(sample));
    return mean_square - mean*mean;
}

template<typename Mean>
double Forecast::probability_geometric(Mean&& mean) const
{
    return 1.0 / mean;
}

template<typename Mean, typename N>
double Forecast::probability_poisson(Mean&& mean, N&& n) const
{
    return mean / n;
}

template<typename Container>
double Forecast::make_forecast(Container&& sample)
{
    return probability_geometric(get_mean(std::forward<Container>(sample)));
}

#endif //_FORECAST_HPP_