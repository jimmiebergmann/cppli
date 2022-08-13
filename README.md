# c[pp]li
![version](https://img.shields.io/badge/Version-v0.1.0-blue) [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT)  
Single header C++17 CLI parser library.

# Example
Following example is from examples/readme_example_cpp20. See other examples for C++17 syntax.
```cpp
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

int new_project_callback(cli::context&) {
    /* Code here... */
    return 0;
}
```
## Execution
```
> readme_example_cpp20 --help
```
```
Usage: readme_example_cpp20 [command] [command-options]

Commands:
  -h|--help       Show command line help.
  new             Create new project.
  open            Open project.
  build|yolo      Build project.
```