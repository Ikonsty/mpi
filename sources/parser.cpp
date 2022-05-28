#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <iterator>
#include "../includes/errors.h"
#include "../includes/parser.hpp"

namespace po = boost::program_options;
namespace sys = std::filesystem;

config_options_t::config_options_t() {
    config.add_options()
            ("density", po::value<double>()->default_value(1.0),
             "density")
            ("heat_capacity", po::value<double>()->default_value(1.0),
             "heat_capacity")
            ("therm_conduct", po::value<double>()->default_value(1.0),
             "therm_conduct")
            ("x_start", po::value<double>()->default_value(0.0),
             "x_start")
            ("x_end", po::value<double>()->default_value(0.0),
             "x_end")
            ("y_start", po::value<double>()->default_value(100.0),
             "y_start")
            ("y_end", po::value<double>()->default_value(100.0),
             "y_end")
            ("delta_x", po::value<double>()->default_value(1.0),
             "delta_x")
            ("delta_y", po::value<double>()->default_value(1.0),
             "delta_y")
            ("delta_t", po::value<double>()->default_value(0.01),
             "delta_t")
            ("interval_t", po::value<size_t>()->default_value(10),
             "interval_t")
            ("total_time", po::value<size_t>()->default_value(1000),
             "total_time")
            ("out_dir", po::value<sys::path>()->default_value(""),
             "out_dir")
            ("init_dir", po::value<sys::path>()->default_value(""),
             "init_dir");
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

        density = vm["density"].as<double>();
        heat_capacity = vm["heat_capacity"].as<double>();
        therm_conduct = vm["therm_conduct"].as<double>();
        x_start = vm["x_start"].as<double>();
        x_end = vm["x_end"].as<double>();
        y_start = vm["y_start"].as<double>();
        y_end = vm["y_end"].as<double>();
        delta_x = vm["delta_x"].as<double>();
        delta_y = vm["delta_y"].as<double>();
        delta_t = vm["delta_t"].as<double>();
        interval_t = vm["interval_t"].as<size_t>();
        total_time = vm["total_time"].as<size_t>();
        out_dir = vm["out_dir"].as<sys::path>();
        init_dir = vm["init_dir"].as<sys::path>();

    }
    catch (std::exception &e) {
        {
            std::cerr << e.what() << std::endl;
            exit(CFG_FILE_READ_ERROR);
        }
    }
}
