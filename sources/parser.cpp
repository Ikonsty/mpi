#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <iterator>
#include "../includes/parser.hpp"
#include "../includes/errors.h"

namespace po = boost::program_options;
namespace sys = std::filesystem;

config_options_t::config_options_t() {
    config.add_options()
            ("out_dir", po::value<sys::path>()->default_value(""),
             "out_dir")
            ("n_threads", po::value<size_t>()->default_value(2),
             "n_threads")
            ("density", po::value<float>()->default_value(1.0),
             "density")
            ("heat_capacity", po::value<float>()->default_value(1.0),
             "heat_capacity")
            ("x_start", po::value<float>()->default_value(0.0),
             "x_start")
            ("x_end", po::value<float>()->default_value(0.0),
             "x_end")
            ("y_start", po::value<float>()->default_value(100.0),
             "y_start")
            ("y_end", po::value<float>()->default_value(100.0),
             "y_end")
            ("delta_x", po::value<float>()->default_value(1.0),
             "delta_x")
            ("delta_y", po::value<float>()->default_value(1.0),
             "delta_y")
            ("delta_t", po::value<float>()->default_value(0.01),
             "delta_t")
            ("interval_t", po::value<size_t>()->default_value(10),
             "interval_t");
}

config_options_t::config_options_t(char *argv[]):
        config_options_t()
{
    parse(argv);
}

void config_options_t::parse(char *argv[]) {
    std::string config_file = argv[1];
    std::ifstream file(config_file.c_str());
    if (!file)
    {
        std::cerr << "Не вдалося відкрити конфігураційний файл!" << std::endl;
        exit(CFG_FILE_OPEN_ERROR);
    }
    try {
        po::options_description config_file_options;
        config_file_options.add(config);
        store(parse_config_file(file, config_file_options), vm);
        notify(vm);

        out_dir = vm["out_dir"].as<sys::path>();
        n_threads = vm["n_threads"].as<size_t>();
        density = vm["density"].as<float>();
        heat_capacity = vm["heat_capacity"].as<float>();
        x_start = vm["x_start"].as<float>();
        x_end = vm["x_end"].as<float>();
        y_start = vm["y_start"].as<float>();
        y_end = vm["y_end"].as<float>();
        delta_x = vm["delta_x"].as<float>();
        delta_y = vm["delta_y"].as<float>();
        delta_t = vm["delta_t"].as<float>();
        interval_t = vm["interval_t"].as<size_t>();

    }
    catch (std::exception &e) {
        {
            std::cerr << e.what() << std::endl;
            exit(CFG_FILE_READ_ERROR);
        }
    }
}
