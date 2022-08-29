#include "cppli/cppli.hpp"

namespace cli = cppli;

bool is_debug = false;
int new_project_callback(cli::context&);

int main(int argc, char** argv) {
    auto context
        = cli::context{}
            .set_arg(argc, argv)
        | cli::default_help{}
        | cli::default_error{};

    auto parameters
        = cli::command{}
            .set_name("new")
            .set_description("Create new project.")
            .set_callback(&new_project_callback)
        | cli::command{}
            .set_name("open")
            .set_description("Open project.")
            .set_callback([](cli::context&) { /* Code here... */ return 0; })
        | cli::command{}
            .set_names({ "build", "yolo" })
            .set_description("Build project.")
            .set_callback([](cli::context&) { /* Code here... */ return 0; })
        | cli::option_flag<bool>{ is_debug }
            .set_names({ "-d", "--debug" })
            .set_description("Run program in debug mode.");

    return parameters.parse(context);
}

int new_project_callback(cli::context& context) {
    auto name = std::string{};
    auto template_name = std::optional<std::string>{};

    auto parameters
        = cli::option<std::string>{ name }
            .set_name("name")
            .set_description("Name of project.")
        | cli::option<std::optional<std::string>>{ template_name }
            .set_name("--template")
            .set_description("Optional template filename.");

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