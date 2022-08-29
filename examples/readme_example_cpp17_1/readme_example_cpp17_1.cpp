#include "cppli/cppli.hpp"

namespace cli = cppli;

bool is_debug = false;
int new_project_callback(cli::context&);

int main(int argc, char** argv) {
    auto context
        = cli::context{ argc, argv }
        | cli::default_help{}
        | cli::default_error{};

    auto parameters
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
          }
        | cli::option_flag<bool>{ 
            is_debug,
            { "-d", "--debug" },
            "Run program in debug mode."
          };

    return parameters.parse(context);
}

int new_project_callback(cli::context& context) {
    auto name = std::string{};
    auto template_name = std::optional<std::string>{};

    auto parameters
        = cli::option<std::string>{ name, { "name" }, "Name of project." }
        | cli::option<std::optional<std::string>>{ template_name, { "--template" }, "Optional template filename." };

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