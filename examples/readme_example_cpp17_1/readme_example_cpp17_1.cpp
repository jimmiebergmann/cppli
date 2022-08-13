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

int new_project_callback(cli::context&) {
    /* Code here... */
    return 0;
}