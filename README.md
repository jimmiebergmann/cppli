# c[pp]li
![version](https://img.shields.io/badge/Version-v0.1.0-blue) [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://github.com/jimmiebergmann/cppli/blob/master/LICENSE) [![GitHub Workflow Status (branch)](https://img.shields.io/github/workflow/status/jimmiebergmann/cppli/Build/master?label=Github&logo=Github)](https://github.com/jimmiebergmann/cppli/actions) [![Codecov code coverage](https://img.shields.io/codecov/c/github/jimmiebergmann/cppli/master?label=Codecov&logo=Codecov)](https://codecov.io/gh/jimmiebergmann/cppli)  
Single header C++17 CLI parser library.

# Example
Following example is from examples/readme_example_cpp20. See other [examples](https://github.com/jimmiebergmann/cppli/blob/master/examples) for C++17 syntax.
```cpp
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
        | cli::option_flag<bool>{ /* Global debug flag. */
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
```
### Execution
```
> readme_example_cpp20 --help
```
```
Usage: readme_example_cpp20_d [options] [command] [command-options]

Options:
  -d|--debug      Run program in debug mode.

Commands:
  -h|--help       Show command line help.
  new             Create new project.
  open            Open project.
  build|yolo      Build project.
```
```
> readme_example_cpp20 new --help
```
```
Usage: new <name> [options]

Options:
  -h|--help       Show command line help.
  name            Name of project.
  --template      Optional template filename.
```
```
> readme_example_cpp20 --debug new example --template test
```
```
New project 'example', from template 'test' in debug mode.
```

# Usage
Add [include](https://github.com/jimmiebergmann/cppli/blob/master/include) to your project's include directories and `#include "cppli/cppli.hpp"`.  
All files are licensed under `MIT license`, see the [LICENSE](https://github.com/jimmiebergmann/cppli/blob/master/LICENSE) for more information.

# Build examples and tests
```
git clone --recurse-submodules https://github.com/jimmiebergmann/cppli.git
mkdir cppli/build
cd cppli/build
cmake ..
cmake --build . --config Release
```