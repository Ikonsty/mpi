#ifndef PARSER_HPP
#define PARSER_HPP
#include <boost/program_options.hpp>
#include <filesystem>

namespace sys = std::filesystem;

class config_options_t
{
public:
    config_options_t();
    config_options_t(char *argv[]);

    config_options_t(const config_options_t &) = default;
    config_options_t &operator=(const config_options_t &) = delete;
    config_options_t(config_options_t &&) = default;
    config_options_t &operator=(config_options_t &&) = delete;
    ~config_options_t() = default;

    void parse(char *argv[]);



    boost::program_options::variables_map vm;
    boost::program_options::options_description config;

    double density;
    double heat_capacity;
    double therm_conduct;
    double x_start;
    double x_end;
    double y_start;
    double y_end;
    double delta_x;
    double delta_y;
    double delta_t;
    size_t interval_t;
    size_t total_time;
    sys::path out_dir;
    sys::path init_dir;
};


#endif // PARSER_HPP
