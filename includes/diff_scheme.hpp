#ifndef DIFF_SCHEME_HPP
#define DIFF_SCHEME_HPP

#include "../includes/parser.hpp"

struct near_points_t{
    double i_j;
    double ip_j;
    double in_j;
    double i_jp;
    double i_jn;
};

double diff_func(const config_options_t& obj,
                 const near_points_t& T, const double& A);

bool criteria_fon_Neyman(const config_options_t& obj, const double& A);

#endif // DIFF_SCHEME_HPP
