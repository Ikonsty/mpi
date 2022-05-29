#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <vector>
#include <boost/serialization/access.hpp>

class grid
{
public:
    ~grid() = default;
    grid(const grid&) = delete;
    grid& operator=(const grid&) = delete;

    grid(int n_col, int n_row);

    /// get double from the array
    double get(size_t row, size_t col);

    /// put double to the array. Return 0 if there were no exceptions
    int put(size_t row, size_t col, double value);

    void resize(int n_col, int n_row);

    void autofill(const std::vector<double>& vc);

    size_t get_n_col();

    size_t get_n_row();

    void print();

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & grid_;
        ar & n_col_;
        ar & n_row_;
    }

private:
    friend class boost::serialization::access;
    std::vector<double> grid_;
    size_t n_col_;
    size_t n_row_;
};

#endif // GRID_HPP
