#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "../includes/diff_scheme.hpp"


// Count T(i,j, k+1)
double diff_func(const config_options_t& obj,
                 const near_points_t& T, const double& A) {

    double Tn_i_j;
    Tn_i_j = T.i_j + obj.delta_t * A * ( ( (T.ip_j - 2*T.i_j + T.in_j) / (pow(obj.delta_x, 2)) )
                                     + ( (T.i_jp - 2*T.i_j + T.i_jn) / (pow(obj.delta_y, 2)) ) );
    return Tn_i_j;
}

bool criteria_fon_Neyman(const config_options_t& obj, const double& A) {
    return ( obj.delta_t <= ( pow(std::max(obj.delta_x, obj.delta_y), 2) / (4*A)) );
}
