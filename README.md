# c[pp]li
Single header C++17 CLI parser library.

# Example
```cpp
#include "cppli/cppli.hpp"

namespace cli = cppli;

int main(int argc, char** argv) {
    auto context
        = cli::context{ .argc = argc, .argv = argv }
        | cli::default_help{}
        | cli::default_error{};
    
    auto commands
        = cli::command{ 
            .names = { "new" },
            .description = "Create new project.",
            .callback = &new_project
          }
        | cli::command{ 
            .names = { "open" },
            .description = "Open project.",
            .callback = &open_project
          }
        | cli::command{ 
            .names = { "build" },
            .description = "Build project.",
            .callback = &build_project
          };

    return commands.parse(context);
}
```
## Execution
```
> my_program --help
```
```
Usage: my_program [command] [command-options]

commands:
  new         Create new project.
  open        Open project.
  build       Build project.
  --help      Show this help.
```