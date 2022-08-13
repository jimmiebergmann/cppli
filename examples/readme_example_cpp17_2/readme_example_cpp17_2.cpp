#include "cppli/cppli.hpp"

namespace cli = cppli;

int new_project_callback(cli::context&);

int main(int argc, char** argv) {
    auto context
        = cli::context{}
            .set_arg(argc, argv)
        | cli::default_help{}
        | cli::default_error{};

    auto commands
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
            .set_callback([](cli::context&) { /* Code here... */ return 0; });

    return commands.parse(context);
}

int new_project_callback(cli::context&) {
    /* Code here... */
    return 0;
}