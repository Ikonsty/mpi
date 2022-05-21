#ifndef PARSER_HPP
#define PARSER_HPP
#include <boost/program_options.hpp>
#include <filesystem>

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

    std::filesystem::path out_dir;
    size_t n_threads;
    float density;
    float heat_capacity;
    float x_start;
    float x_end;
    float y_start;
    float y_end;
    float delta_x;
    float delta_y;
    float delta_t;
    size_t interval_t;

};


#endif // PARSER_HPP
