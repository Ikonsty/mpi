#include "../includes/grid.hpp"

grid::grid(int n_col, int n_row)
    : n_col_(n_col),
      n_row_(n_row)
{
    grid_.resize(n_col_ * n_row_);
}

double grid::get(size_t row, size_t col) {
    return grid_[n_col_ * row + col];
}

int grid::put(size_t row, size_t col, double value) {
    try {
        grid_[n_col_ * row + col] = value;
        return 0;
    } catch (...) {
        std::cerr << "Error while putting value to grid" << std::endl;
        return 1;
    }
}

void grid::resize(int n_col, int n_row)
{
    n_col_ = n_col;
    n_row_ = n_row;
    grid_.resize(n_col_ * n_row_);
}

void grid::autofill(const std::vector<double>& vc) {
    try {
        for (size_t i = 0; i < grid_.size(); ++i) {
            grid_[i] = vc[i];
        }
    } catch (...) {
        std::cerr << "Can`t convert input type to double" << std::endl;
    }
}

size_t grid::get_n_col() {
    return n_col_;
}

size_t grid::get_n_row() {
    return n_row_;
}

void grid::print() {
    size_t counter = 0;
    for(size_t i = 0; i < grid_.size(); ++i) {
        std::cout << grid_[i] << " ";
        ++counter;
        if (counter == n_col_) {
            std::cout << std::endl;
            counter = 0;
        }
    }
}
