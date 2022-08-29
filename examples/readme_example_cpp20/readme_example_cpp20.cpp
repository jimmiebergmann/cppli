#include "cppli/cppli.hpp"

namespace cli = cppli;

bool is_debug = false;
int new_project_callback(cli::context&);

int main(int argc, char** argv) {
    auto context
        = cli::context{ .argc = argc, .argv = argv }
        | cli::default_help{}
        | cli::default_error{};

    auto parameters
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
          }
        | cli::option_flag<bool>{
            .value = is_debug,
            .names = { "-d", "--debug" },
            .description = "Run program in debug mode.",
          };

    return parameters.parse(context);
}

int new_project_callback(cli::context& context) {
    auto name = std::string{};
    auto template_name = std::optional<std::string>{};

    auto parameters
        = cli::option<std::string>{ 
            .value = name,
            .names = { "name" },
            .description = "Name of project."
          }
        | cli::option<std::optional<std::string>>{
            .value = template_name,
            .names = { "--template" },
            .description = "Optional template filename."
          };

    if (auto result = parameters.parse(context); result == cli::parse_codes::successful_help) {
        return 0;
    }
    else if (result != cli::parse_codes::successful) {
        return result;
    }

    /* Create new project with name and template here... */
    std::cout << "New project '" << name
              << "', from template '" << template_name.value_or("none")
              << (is_debug ? "' in debug mode.\n" : "'.\n");

    return 0;
}