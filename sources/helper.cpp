#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>


#include "../includes/helper.hpp"
#include "../includes/errors.h"

namespace sys = std::filesystem;

/// Read initial grid and cut a part that corresponds a given rank
//col, row, chunk
std::pair<std::pair<int, int>, std::vector<double>> read_initial_t(sys::path filepath, size_t pr_n, size_t rank) {

        std::ifstream file(filepath);
        std::vector<std::string> splited;
        std::vector<double> chunk;
        if(!file) {
            std::cerr << "Error while reading input file." << std::endl;
            exit(INIT_FILE_READ_ERROR);
        }
        std::ostringstream ss;
        ss << file.rdbuf();
        boost::split(splited, ss.str(), boost::is_any_of("\n "));

        int n_col = std::stoi(splited[0]);
        int n_row = std::stoi(splited[1]);
        int chunk_size = (n_col - 2) / pr_n;
        int my_chunk_size = chunk_size;
        if (rank == 0) {
            my_chunk_size += 1;
        }
        else if (rank == 3) {
            my_chunk_size += ((n_col - 2) % pr_n) + 1;
        }

        int curr_row = 0;
        int init_col = 1 + rank * chunk_size;
        if(rank == 0) {
            --init_col;
        }
        int curr_col = init_col;

        while(curr_row < n_row) {
            while ( (curr_col - init_col) < my_chunk_size) {
                chunk.push_back(stod(splited[n_col*curr_row + curr_col + 2]));
                ++curr_col;
            }
            curr_col = init_col;
            ++curr_row;
        }

        return std::make_pair(std::make_pair(my_chunk_size, n_row), chunk);

}

std::vector<size_t> convert_to_rgb(double temp) {
    double range = 100;
    double alpha = (range - (temp + 50))/range;
    std::vector<size_t> rgb(3);
    rgb[0] = alpha*127 + (1 - alpha)*255; //red
    rgb[1] = alpha*127 + (1 - alpha)*127; //green
    rgb[2] = alpha*255 + (1 - alpha)*127; //blue
    return rgb;
}