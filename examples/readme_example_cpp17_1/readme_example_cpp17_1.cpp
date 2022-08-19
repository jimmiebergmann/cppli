#include "cppli/cppli.hpp"

namespace cli = cppli;

int new_project_callback(cli::context&);

int main(int argc, char** argv) {
    auto context
        = cli::context{ argc, argv }
        | cli::default_help{}
        | cli::default_error{};

    auto commands
        = cli::command{
            { "new" },
            "Create new project.",
            &new_project_callback
          }
        | cli::command{
            { "open" },
            "Open project.",
            [](cli::context&) { /* Code here... */ return 0; }
          }
        | cli::command{
            { "build", "yolo" },
            "Build project.",
            [](cli::context&) { /* Code here... */ return 0; }
          };

    return commands.parse(context);
}

int new_project_callback(cli::context& context) {
    auto name = std::string{};
    auto template_name = std::optional<std::string>{};

    auto options
        = cli::option<std::string>{ name, { "name" }, "Name of project." }
        | cli::option<std::optional<std::string>>{ template_name, { "--template" }, "Optional template filename." };

    if (auto result = options.parse(context); result == cli::parse_codes::successful_help) {
        return 0;
    }
    else if (result != cli::parse_codes::successful) {
        return result;
    }

    /* Create new project with name and template here... */
    std::cout << "New project '" << name
              << "', from template '" << template_name.value_or("none") << "'\n";

    return 0;
}