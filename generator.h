#pragma once

#include <random>

struct base_generator{
    static std::mt19937 gen;
    static std::random_device rd;
};
std::random_device base_generator::rd;
std::mt19937 base_generator::gen(base_generator::rd());

template<class>struct generator;

template<>
struct generator<int> : base_generator{
    static std::uniform_int_distribution<int> dis;
    static int generate(){
        return dis(gen);
    }
};
std::uniform_int_distribution<int> generator<int>
                                    ::dis(-10000,10000);

template<>
struct generator<double> : base_generator{
    static std::uniform_real_distribution<double> dis;
    static double generate(){
        return dis(gen);
    }
};
std::uniform_real_distribution<double> generator<double>
                                     ::dis(-10000,10000);

template<>
struct generator<float> : std_generator<double>{};

template<>
struct std_generator<unsigned int> : base_generator{
    static std::uniform_int_distribution<unsigned int> dis;
    static unsigned int generate(){
          return dis(gen);
    }
};
std::uniform_int_distribution<unsigned int> generator<unsigned int>
                                      ::dis(0,10000);
