#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/algorithm/string.hpp>
#include <Magick++.h>

#include "includes/grid.hpp"
#include "includes/helper.hpp"
#include "includes/errors.h"
#include "includes/parser.hpp"
#include "includes/diff_scheme.hpp"



int main(int argc, char * argv[]) {
    if (argc != 2) {
        std::cerr << "Невірна кількість аргументів!" << std::endl;
        exit(ARGUMENTS_ERROR);
    }
    Magick::InitializeMagick(*argv);
    config_options_t obj(argv);
    size_t n_proc = 4;

    const double A = obj.therm_conduct / (obj.density * obj.heat_capacity);

    // Check criteria fon Neyman
    if (!criteria_fon_Neyman(obj, A)) {
        std::cerr << "Delta t is too high." << std::endl;
        exit(CRITERIA_ERROR);
    }




    boost::mpi::environment env{argc, argv};
    boost::mpi::communicator world;

    if (world.rank() == 0) {

        auto init = read_initial_t(obj.init_dir, n_proc, world.rank());
        grid grid_(init.first.first, init.first.second);
        grid_.autofill(init.second);
    #ifdef DEBUG
        grid_.print();
    #endif
        size_t total_it = obj.total_time/obj.delta_t;
        for (size_t k=0; k<total_it; ++k) {
            for (size_t l=0; l<obj.delta_t; ++l) {
                size_t curr_row = 1;
                size_t curr_col = 1;
                std::vector<boost::mpi::request> requests(grid_.get_n_row());
                std::vector<double> outside_val(grid_.get_n_row());

                for (size_t i = 0; i < requests.size(); ++i) {
                    world.isend(1, i, grid_.get(i, grid_.get_n_col() - 1));
                }

                for (size_t i = 0; i < requests.size(); ++i) {
                    requests[i] = world.irecv(1, i, outside_val[i]);
                }

                while (curr_row < (grid_.get_n_row() - 1)) {
                    while (curr_col < (grid_.get_n_col())) {
                        near_points_t T;
                        T.i_j = grid_.get(curr_row, curr_col); // current element in whole grid
                        T.ip_j = grid_.get(curr_row - 1, curr_col);
                        T.in_j = grid_.get(curr_row + 1, curr_col);
                        T.i_jp = grid_.get(curr_row, curr_col - 1);

                        if (curr_col == grid_.get_n_col() - 1) {
                            boost::mpi::wait_any(requests.begin() + curr_row, requests.begin() + curr_row + 1);
                            T.i_jn = outside_val[curr_row];
                        } else {
                            T.i_jn = grid_.get(curr_row, curr_col + 1);
                        }

                        double t_res = diff_func(obj, T, A);
                        // change process grid
                        grid_.put(curr_row, curr_col, t_res);

                        ++curr_col;
                    }
                    curr_col = 1; // index of fist elem in row
                    ++curr_row;
                }
            }
            world.isend(4, world.rank(), grid_);
        }

//        grid_.print();
    }

    else if (world.rank() == 1) {

        auto init = read_initial_t(obj.init_dir, n_proc, world.rank());
        grid grid_(init.first.first, init.first.second);
        grid_.autofill(init.second);
    #ifdef DEBUG
        grid_.print();
    #endif
        size_t total_it = obj.total_time/obj.delta_t;
        for (size_t k=0; k<total_it; ++k) {
            for (size_t l=0; l<obj.delta_t; ++l) {
                size_t curr_row = 1;
                size_t curr_col = 0;
                std::vector<boost::mpi::request> l_requests(grid_.get_n_row());
                std::vector<boost::mpi::request> r_requests(grid_.get_n_row());
                std::vector<double> l_outside_val(grid_.get_n_row());
                std::vector<double> r_outside_val(grid_.get_n_row());

                for (size_t i = 0; i < l_requests.size(); ++i) {
                    world.isend(0, i, grid_.get(i, grid_.get_n_col() - 1));
                }

                for (size_t i = 0; i < l_requests.size(); ++i) {
                    l_requests[i] = world.irecv(0, i, l_outside_val[i]);
                }

                for (size_t i = 0; i < r_requests.size(); ++i) {
                    world.isend(2, i, grid_.get(i, grid_.get_n_col() - 1));
                }

                for (size_t i = 0; i < r_requests.size(); ++i) {
                    r_requests[i] = world.irecv(2, i, r_outside_val[i]);
                }

                while (curr_row < (grid_.get_n_row() - 1)) {
                    while (curr_col < grid_.get_n_col()) {
                        near_points_t T;
                        T.i_j = grid_.get(curr_row, curr_col); // current element in whole grid
                        T.ip_j = grid_.get(curr_row - 1, curr_col);
                        T.in_j = grid_.get(curr_row + 1, curr_col);

                        if (curr_col == 0){
                            boost::mpi::wait_any(l_requests.begin() + curr_row, l_requests.begin() + curr_row + 1);
                            T.i_jp = l_outside_val[curr_row];
                            T.i_jn = grid_.get(curr_row, curr_col + 1);
                        } else if (curr_col == grid_.get_n_col() - 1) {
                            boost::mpi::wait_any(r_requests.begin() + curr_row, r_requests.begin() + curr_row + 1);
                            T.i_jn = r_outside_val[curr_row];
                            T.i_jp = grid_.get(curr_row, curr_col - 1);
                        } else {
                            T.i_jn = grid_.get(curr_row, curr_col + 1);
                            T.i_jp = grid_.get(curr_row, curr_col - 1);
                        }

                        double t_res = diff_func(obj, T, A);
                        // chenge process grid
                        grid_.put(curr_row, curr_col, t_res);

                        ++curr_col;
                    }
                    curr_col = 1; // index of fist elem in row
                    ++curr_row;
                }
            }
            world.isend(4, world.rank(), grid_);
        }

//        grid_.print();
    }

    else if (world.rank() == 2) {

        auto init = read_initial_t(obj.init_dir, n_proc, world.rank());
        grid grid_(init.first.first, init.first.second);
        grid_.autofill(init.second);
    #ifdef DEBUG
        grid_.print();
    #endif
        size_t total_it = obj.total_time/obj.delta_t;
        for (size_t k=0; k<total_it; ++k) {
            for (size_t l=0; l<obj.delta_t; ++l) {
                size_t curr_row = 1;
                size_t curr_col = 0;
                std::vector<boost::mpi::request> l_requests(grid_.get_n_row());
                std::vector<boost::mpi::request> r_requests(grid_.get_n_row());
                std::vector<double> l_outside_val(grid_.get_n_row());
                std::vector<double> r_outside_val(grid_.get_n_row());

                for (size_t i = 0; i < l_requests.size(); ++i) {
                    world.isend(1, i, grid_.get(i, grid_.get_n_col() - 1));
                }

                for (size_t i = 0; i < l_requests.size(); ++i) {
                    l_requests[i] = world.irecv(1, i, l_outside_val[i]);
                }

                for (size_t i = 0; i < r_requests.size(); ++i) {
                    world.isend(3, i, grid_.get(i, grid_.get_n_col() - 1));
                }

                for (size_t i = 0; i < r_requests.size(); ++i) {
                    r_requests[i] = world.irecv(3, i, r_outside_val[i]);
                }

                while (curr_row < (grid_.get_n_row() - 1)) {
                    while (curr_col < grid_.get_n_col()) {
                        near_points_t T;
                        T.i_j = grid_.get(curr_row, curr_col); // current element in whole grid
                        T.ip_j = grid_.get(curr_row - 1, curr_col);
                        T.in_j = grid_.get(curr_row + 1, curr_col);

                        if (curr_col == 0){
                            boost::mpi::wait_any(l_requests.begin() + curr_row, l_requests.begin() + curr_row + 1);
                            T.i_jp = l_outside_val[curr_row];
                            T.i_jn = grid_.get(curr_row, curr_col + 1);
                        } else if (curr_col == grid_.get_n_col() - 1) {
                            boost::mpi::wait_any(r_requests.begin() + curr_row, r_requests.begin() + curr_row + 1);
                            T.i_jn = r_outside_val[curr_row];
                            T.i_jp = grid_.get(curr_row, curr_col - 1);
                        } else {
                            T.i_jn = grid_.get(curr_row, curr_col + 1);
                            T.i_jp = grid_.get(curr_row, curr_col - 1);
                        }

                        double t_res = diff_func(obj, T, A);
                        // chenge process grid
                        grid_.put(curr_row, curr_col, t_res);

                        ++curr_col;
                    }
                    curr_col = 1; // index of fist elem in row
                    ++curr_row;
                }
            }
            world.isend(4, world.rank(), grid_);
        }

//        grid_.print();
    }

    else if (world.rank() == 3) {

        auto init = read_initial_t(obj.init_dir, n_proc, world.rank());
        grid grid_(init.first.first, init.first.second);
        grid_.autofill(init.second);
    #ifdef DEBUG
        grid_.print();
    #endif
        size_t total_it = obj.total_time/obj.delta_t;
        for (size_t k=0; k<total_it; ++k) {
            for (size_t l=0; l<obj.delta_t; ++l) {
                size_t curr_row = 1;
                size_t curr_col = 0;
                std::vector<boost::mpi::request> requests(grid_.get_n_row());
                std::vector<double> outside_val(grid_.get_n_row());

                for (size_t i = 0; i < requests.size(); ++i) {
                    world.isend(2, i, grid_.get(i, grid_.get_n_col() - 1));
                }

                for (size_t i = 0; i < requests.size(); ++i) {
                    requests[i] = world.irecv(2, i, outside_val[i]);
                }

                while (curr_row < (grid_.get_n_row() - 1)) {
                    while (curr_col < (grid_.get_n_col()) - 1) {
                        near_points_t T;
                        T.i_j = grid_.get(curr_row, curr_col); // current element in whole grid
                        T.ip_j = grid_.get(curr_row - 1, curr_col);
                        T.in_j = grid_.get(curr_row + 1, curr_col);
                        T.i_jn = grid_.get(curr_row, curr_col + 1);

                        if (curr_col == 0) {
                            boost::mpi::wait_any(requests.begin() + curr_row, requests.begin() + curr_row + 1);
                            T.i_jp = outside_val[curr_row];
                        } else {
                            T.i_jp = grid_.get(curr_row, curr_col - 1);
                        }

                        double t_res = diff_func(obj, T, A);
                        // chenge process grid
                        grid_.put(curr_row, curr_col, t_res);

                        ++curr_col;
                    }
                    curr_col = 1; // index of fist elem in row
                    ++curr_row;
                }
            }
            world.isend(4, world.rank(), grid_);
        }

//        grid_.print();
    }
    else if (world.rank() == 4) {
        std::vector<boost::mpi::request> requests(4);
        std::vector<grid> outside_val;
        for (size_t i=0; i<obj.total_time/obj.delta_t; ++i) {
            for (size_t j = 0; j < 4; j++) {
                requests[i] = world.irecv(j, j, outside_val[i]);
            }

            boost::mpi::wait_all(requests.begin(), requests.end());

            int height = outside_val[0].get_n_row();
            int width = 0;
            for (int i = 0; i < outside_val.size(); ++i) {
                width += outside_val[i].get_n_col();
            }
            Magick::Image img;
            std::vector<size_t> pixels;
            for (int t = 0; t < height; ++t) {
                for (int j = 0; j < outside_val[0].get_n_col(); j++) {
                    std::vector<size_t> px = convert_to_rgb(outside_val[0].get(t, j));
                    pixels.push_back(px[0]);
                    pixels.push_back(px[1]);
                    pixels.push_back(px[2]);
                }
                for (int j = 0; j < outside_val[1].get_n_col(); j++) {
                    std::vector<size_t> px = convert_to_rgb(outside_val[1].get(t, j));
                    pixels.push_back(px[0]);
                    pixels.push_back(px[1]);
                    pixels.push_back(px[2]);
                }
                for (int j = 0; j < outside_val[2].get_n_col(); j++) {
                    std::vector<size_t> px = convert_to_rgb(outside_val[2].get(t, j));
                    pixels.push_back(px[0]);
                    pixels.push_back(px[1]);
                    pixels.push_back(px[2]);
                }
                for (int j = 0; j < outside_val[3].get_n_col(); j++) {
                    std::vector<size_t> px = convert_to_rgb(outside_val[3].get(t, j));
                    pixels.push_back(px[0]);
                    pixels.push_back(px[1]);
                    pixels.push_back(px[2]);
                }
            }
            img.read(height, width, "RGB", Magick::CharPixel, &pixels);
            std::string path = "../data/heatmap";
            path += std::to_string(i) + ".png";
            img.write(path);
        }
    }
    return 0;
}
