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
    auto name = std::string{};
    auto template_name = std::optional<std::string>{};

    auto options 
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

    if (auto result = options.parse(context); result != cli::parse_codes::successful) {
        return result;
    }

    /* Create new project with name and template here... */

    return 0;
}