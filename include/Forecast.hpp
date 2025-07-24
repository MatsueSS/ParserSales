#ifndef _FORECAST_HPP_
#define _FORECAST_HPP_

#include <numeric>
#include <string>
#include <iostream>

class FCexception : public std::exception{
protected:
    std::string msg;

public:
    FCexception(std::string str) : msg(std::move(str)) {}
    FCexception(const FCexception& obj) : msg(obj.msg) {}

    const char * what() const noexcept override { return msg.c_str(); };
};

class BadTypeValueFCexception : public FCexception{
public:
    BadTypeValueFCexception(std::string str) : FCexception(std::move(str)) {}
};

class ZeroDivisionFCexception : public FCexception{
public:
    ZeroDivisionFCexception(std::string str) : FCexception(std::move(str)) {}
};

class Forecast{
public:
    template<typename Container>
    double make_forecast(Container&&) const;

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
    if(mean == 0){
        throw ZeroDivisionFCexception("Zero Division\n");
    }

    return 1.0 / mean;
}

template<typename Mean, typename N>
double Forecast::probability_poisson(Mean&& mean, N&& n) const
{
    if constexpr (!std::is_same<std::decay_t<N>, int>::value){
        throw BadTypeValueFCexception("Value must be int\n");
    }

    if(n == 0){
        throw ZeroDivisionFCexception("Zero division\n");
    }

    return mean / n;
}

template<typename Container>
double Forecast::make_forecast(Container&& sample) const
{
    double result;
    try{
        result = probability_geometric(get_mean(std::forward<Container>(sample)));
    } catch(ZeroDivisionFCexception& e){
        throw ZeroDivisionFCexception("Zero Division\n");
    }
    return result;
}

#endif //_FORECAST_HPP_