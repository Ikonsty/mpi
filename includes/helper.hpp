#ifndef HELPER_HPP
#define HELPER_HPP

#include <filesystem>
#include "../includes/grid.hpp"

namespace sys = std::filesystem;

std::pair<std::pair<int, int>, std::vector<double>> read_initial_t(sys::path filepath, size_t pr_n, size_t rank);

#endif // HELPER_HPP
