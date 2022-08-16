#include "cppli/cppli.hpp"

namespace cli = cppli;

int new_project_callback(cli::context&);

int main(int argc, char** argv) {
    auto context
        = cli::context{ .argc = argc, .argv = argv }
        | cli::default_help{}
        | cli::default_error{};

    auto commands
        = cli::command{
            .names = { "new" },
            .description = "Create new project.",
            .callback = &new_project_callback
          }
        | cli::command{
            .names = { "open" },
            .description = "Open project.",
            .callback = [](cli::context&) { /* Code here... */ return 0; }
          }
        | cli::command{
            .names = { "build", "yolo" },
            .description = "Build project.",
            .callback = [](cli::context&) { /* Code here... */ return 0; }
          };

    return commands.parse(context);
}

int new_project_callback(cli::context& context) {
    int value_int = 0;   
    std::string value_str = "";
    bool value_bool = false;
    std::optional<int> value_int_opt;
    std::optional<std::string> value_str_opt;
    std::optional<bool> value_bool_opt;

    auto options 
        = cli::option<int>{ 
            .value = value_int,
            .name = "int"
          }
        | cli::option<std::string>{
            .value = value_str,
            .name = "str"
          }
        | cli::option<bool>{
            .value = value_bool,
            .name = "bool"
          }
        | cli::option<std::optional<int>>{
            .value = value_int_opt,
            .name = "--int_opt"
          }
        | cli::option<std::optional<std::string>>{
            .value = value_str_opt,
            .name = "--str_opt"
          } 
        | cli::option<std::optional<bool>>{
            .value = value_bool_opt,
            .name = "--bool_opt"
        };

    auto result = options.parse(context);

    return result;
}