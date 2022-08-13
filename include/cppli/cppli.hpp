/*
* MIT License
*
* Copyright (c) 2022 Jimmie Bergmann
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#ifndef CPPLI_HPP
#define CPPLI_HPP

#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <algorithm>
#include <functional>
#include <utility>
#include <iostream>

namespace cppli {

    struct error;
    struct default_error;
    struct help;
    struct default_help;
    struct context;
    struct command;
    struct command_group;

    using error_callback = std::function<void(context&, std::string)>;
    using help_callback = std::function<int(context&)>;
    using command_callback = std::function<int(context&)>;;

    context& operator | (context& lhs, const error& rhs);
    context& operator | (context& lhs, error&& rhs);
    context operator | (context&& lhs, const error& rhs);
    context operator | (context&& lhs, error&& rhs);

    context& operator | (context& lhs, const help& rhs);
    context& operator | (context& lhs, help&& rhs);
    context operator | (context&& lhs, const help& rhs);
    context operator | (context&& lhs, help&& rhs);

    command_group operator | (command& lhs, const command& rhs);
    command_group operator | (command& lhs, command&& rhs);
    command_group operator | (command&& lhs, const command& rhs);
    command_group operator | (command&& lhs, command&& rhs);

    command_group operator | (command& lhs, const error& rhs);
    command_group operator | (command& lhs, error&& rhs);
    command_group operator | (command&& lhs, const error& rhs);
    command_group operator | (command&& lhs, error&& rhs);

    command_group operator | (command& lhs, const help& rhs);
    command_group operator | (command& lhs, help&& rhs);
    command_group operator | (command&& lhs, const help& rhs);
    command_group operator | (command&& lhs, help&& rhs);

    command_group& operator | (command_group& lhs, const command& rhs);
    command_group& operator | (command_group& lhs, command&& rhs);
    command_group operator | (command_group&& lhs, const command& rhs);
    command_group operator | (command_group&& lhs, command&& rhs);

    command_group& operator | (command_group& lhs, const command_group& rhs);
    command_group& operator | (command_group& lhs, command_group&& rhs);
    command_group operator | (command_group&& lhs, const command_group& rhs);
    command_group operator | (command_group&& lhs, command_group&& rhs);

    command_group& operator | (command_group& lhs, const error& rhs);
    command_group& operator | (command_group& lhs, error&& rhs);
    command_group operator | (command_group&& lhs, const error& rhs);
    command_group operator | (command_group&& lhs, error&& rhs);

    command_group& operator | (command_group& lhs, const help& rhs);
    command_group& operator | (command_group& lhs, help&& rhs);
    command_group operator | (command_group&& lhs, const help& rhs);
    command_group operator | (command_group&& lhs, help&& rhs);

    [[nodiscard]] std::string default_help_string(
        const context& p_context, 
        const command_group& p_command_group);

}

namespace cppli::impl {

    std::string_view parse_program_name(const char* path);

    template<typename T>
    const error* get_error(const context& p_context, const T& p_other);

    error_callback get_error_callback(const error* p_error);

    template<typename T>
    const help* get_help(const context& p_context, const T& p_other);

}

namespace cppli {

    struct error {

        error_callback callback;

        error& set_callback(const error_callback& p_callback);
        error& set_callback(error_callback&& p_callback);

    };


    struct default_error : error {

        default_error();

    };


    struct help {

        std::vector<std::string> names = { "--help", "-h", "-?" };
        std::string description = "Show this help.";
        help_callback callback = {};

        bool has_names() const;

        help& set_name(const std::string& p_name);
        help& set_name(std::string&& p_name);

        help& set_names(std::initializer_list<std::string> p_names);
        help& set_names(const std::vector<std::string>& p_names);
        help& set_names(std::vector<std::string>&& p_names);

        help& set_description(const std::string& p_description);
        help& set_description(std::string&& p_description);

        help& set_callback(const help_callback& p_callback);
        help& set_callback(help_callback&& p_callback);

    };


    struct default_help : help {

        explicit default_help(std::vector<std::string> names = { "--help", "-h", "-?" });

    };


    struct context {

        int argc = 0;
        char** argv = nullptr;
        bool first_arg_is_path = true;
        std::optional<error> error = {};
        std::optional<help> help = {};
        std::vector<std::string> current_path = {};
        std::optional<std::reference_wrapper<const command_group>> current_command_group = {};

        context& set_arg(int p_argc, char** p_argv);
        context& set_argc(int p_argc);
        context& set_argv(char** p_argv);

        context& move_to_next_arg();

    };


    struct command {
        
        std::vector<std::string> names = {};
        std::string description = "";
        command_callback callback = {};

        bool has_name(std::string_view p_name) const;

        command& set_name(const std::string& p_name);
        command& set_name(std::string&& p_name);

        command& set_names(std::initializer_list<std::string> p_names);
        command& set_names(const std::vector<std::string>& p_names);
        command& set_names(std::vector<std::string>&& p_names);

        command& set_description(const std::string& p_description);
        command& set_description(std::string&& p_description);

        command& set_callback(const command_callback& p_callback);
        command& set_callback(command_callback&& p_callback);

    };


    struct command_group {

        std::vector<command> commands = {};
        std::optional<error> error;
        std::optional<help> help;

        int parse(context& p_context) const;

        command_group& add_command(const command& p_command);
        command_group& add_command(command&& p_command);

        command_group& add_commands(command_group&& p_command_group);
        command_group& add_commands(const command_group& p_command_group);

        command_group& set_error(const cppli::error& p_error);
        command_group& set_error(cppli::error&& p_error);

        command_group& set_help(const cppli::help& p_help);
        command_group& set_help(cppli::help&& p_help);

    };

}


// Implementations.
namespace cppli::impl {

    inline std::string_view parse_program_name(const char* path)
    {
        const auto path_view = std::string_view{ path, strlen(path) };

        const auto last_dir_pos = path_view.find_last_of("/\\");
        const auto program_offset = last_dir_pos != std::string_view::npos ? last_dir_pos + 1 : 0;
        const auto program_exe_len = path_view.size() - program_offset;

        const auto program_exe_view = std::string_view{ path_view.data() + program_offset, program_exe_len };

        const auto last_ext_pos = program_exe_view.find_last_of('.');
        const auto program_len = last_ext_pos != std::string_view::npos ? last_ext_pos : program_exe_view.size();

        return std::string_view{ program_exe_view.data(), program_len };
    }

    template<typename T>
    inline const error* get_error(const context& p_context, const T& p_other) {
        return p_other.error.has_value() ?
            &p_other.error.value() :
            p_context.error.has_value() ?
                &p_context.error.value() :
                nullptr;
    }
 
    inline error_callback get_error_callback(const error* p_error) {
        return p_error ? p_error->callback : [](context&, std::string) {};
    }

    template<typename T>
    inline const help* get_help(const context& p_context, const T& p_other) {
        return p_other.help.has_value() ?
            &p_other.help.value() :
            p_context.help.has_value() ?
            &p_context.help.value() :
            nullptr;
    }

}

namespace cppli {

    // Error.
    inline error& error::set_callback(const error_callback& p_callback) {
        callback = p_callback;
        return *this;
    }
    inline error& error::set_callback(error_callback&& p_callback) {
        callback = p_callback;
        return *this;
    }


    // Default error.
    inline default_error::default_error() :
        error{ [](context&, std::string message) { std::cerr << message << "\n"; } }
    {}


    // Help
    inline bool help::has_names() const {
        auto it = std::find_if(names.begin(), names.end(), 
            [](const auto& name) { return !name.empty(); });

        return it != names.end();
    }

    inline help& help::set_name(const std::string& p_name) {
        names = { p_name };
        return *this;
    }
    inline help& help::set_name(std::string&& p_name) {
        names = { std::move(p_name) };
        return *this;
    }

    inline help& help::set_names(std::initializer_list<std::string> p_names) {
        names = p_names;
        return *this;
    }
    inline help& help::set_names(const std::vector<std::string>& p_names) {
        names = p_names;
        return *this;
    }
    inline help& help::set_names(std::vector<std::string>&& p_names) {
        names = std::move(p_names);
        return *this;
    }

    inline help& help::set_description(const std::string& p_description) {
        description = p_description;
        return *this;
    }
    inline help& help::set_description(std::string&& p_description) {
        description = std::move(p_description);
        return *this;
    }

    inline help& help::set_callback(const help_callback& p_callback) {
        callback = p_callback;
        return *this;
    }
    inline help& help::set_callback(help_callback&& p_callback) {
        callback = std::move(p_callback);
        return *this;
    }


    // Default help.
    inline default_help::default_help(std::vector<std::string> names) :
        help{ std::move(names), "Show this help.", [](context& p_context) {
            
            if (p_context.current_command_group.has_value()) {
                std::cout << default_help_string(
                    p_context,
                    p_context.current_command_group.value()) << "\n";
            }
           
            return 0; 
        } }
    {}


    // Context.
    inline context& context::set_arg(int p_argc, char** p_argv) {
        argc = p_argc;
        argv = p_argv;
        return *this;
    }
    inline context& context::set_argc(int p_argc) {
        argc = p_argc;
        return *this;
    }
    inline context& context::set_argv(char** p_argv) {
        argv = p_argv;
        return *this;
    }

    inline context& context::move_to_next_arg() {
        --argc;
        ++argv;
        first_arg_is_path = false;
        return *this;
    }


    // Command.
    inline bool command::has_name(std::string_view p_name) const {
        auto it = std::find_if(names.begin(), names.end(), [p_name](const auto& name) {
            return name == p_name;
        });

        return it != names.end();
    }

    inline command& command::set_name(const std::string& p_name) {
        names = { p_name };
        return *this;
    }
    inline command& command::set_name(std::string&& p_name) {
        names = { std::move(p_name) };
        return *this;
    }

    inline command& command::set_names(std::initializer_list<std::string> p_names) {
        names = p_names;
        return *this;
    }
    inline command& command::set_names(const std::vector<std::string>& p_names) {
        names = p_names;
        return *this;
    }
    inline command& command::set_names(std::vector<std::string>&& p_names) {
        names = std::move(p_names);
        return *this;
    }

    inline command& command::set_description(const std::string& p_description) {
        description = p_description;
        return *this;
    }
    inline command& command::set_description(std::string&& p_description) {
        description = std::move(p_description);
        return *this;
    }

    inline command& command::set_callback(const command_callback& p_callback) {
        callback = p_callback;
        return *this;
    }
    inline command& command::set_callback(command_callback&& p_callback) {
        callback = std::move(p_callback);
        return *this;
    }


    // Command group.
    inline int command_group::parse(context& p_context) const {
        auto* help = impl::get_help(p_context, *this);

        if (commands.empty() && (help == nullptr || !help->has_names()) ) {
            return 0;
        }

        p_context.current_command_group = *this;

        auto* error = impl::get_error(p_context, *this);
        auto error_callback = impl::get_error_callback(error);

        if (p_context.argc < 1) {
            error_callback(p_context, "Missing command.");
            return 1;
        }

        if (p_context.first_arg_is_path) {
            auto program_name = impl::parse_program_name(p_context.argv[0]);
            if (program_name.empty()) {
                error_callback(p_context, "Expecting program path as first parameter.");
                return 1;
            }

            p_context.current_path.emplace_back(program_name);
            p_context.move_to_next_arg();
        }

        if (p_context.argc < 1) {
            error_callback(p_context, "Missing command.");
            return 1;
        }

        auto find_command = [&](std::string_view name) -> const command* {
            for (auto& command : commands) {
                if (command.has_name(name)) {
                    return &command;
                }
            }

            return nullptr;
        };

        const auto command_name = std::string_view{ p_context.argv[0] };

        auto command = find_command(command_name);
        if (command == nullptr)
        {
            if (help == nullptr || !help->has_names()) {
                error_callback(p_context, std::string{ "Unknown command '" } + std::string{ command_name } + "'.");
                return 1;
            }

            auto help_it = std::find_if(help->names.begin(), help->names.end(),
                [&command_name](const auto& name) { return name == command_name; });
            
            if (help_it == help->names.end()) {
                error_callback(p_context, std::string{ "Unknown command '" } + std::string{ command_name } + "'.");
                return 1;
            }

            return help->callback ? help->callback(p_context) : 0;
        }

        p_context.current_path.emplace_back(command_name);
        p_context.move_to_next_arg();

        return command->callback ? command->callback(p_context) : 0;
    }

    inline command_group& command_group::add_command(const command& p_command) {
        commands.emplace_back(p_command);
        return *this;
    }
    inline command_group& command_group::add_command(command&& p_command) {
        commands.emplace_back(std::move(p_command));
        return *this;
    }

    inline command_group& command_group::add_commands(command_group&& p_command_group) {
        commands.insert(
            commands.end(),
            std::make_move_iterator(p_command_group.commands.begin()),
            std::make_move_iterator(p_command_group.commands.end()));
        p_command_group.commands.clear();

        if (!error.has_value() && p_command_group.error.has_value()) {
            error = std::move(p_command_group.error);
        }

        if (!help.has_value() && p_command_group.help.has_value()) {
            help = std::move(p_command_group.help);
        }

        return *this;
    }
    inline command_group& command_group::add_commands(const command_group& p_command_group) {
        commands.insert(
            commands.end(),
            p_command_group.commands.begin(),
            p_command_group.commands.end());

        if (!error.has_value() && p_command_group.error.has_value()) {
            error = p_command_group.error;
        }

        if (!help.has_value() && p_command_group.help.has_value()) {
            help = p_command_group.help;
        }

        return *this;
    }

    inline command_group& command_group::set_error(const cppli::error& p_error) {
        error = p_error;
        return *this;
    }
    inline command_group& command_group::set_error(cppli::error&& p_error) {
        error = std::move(p_error);
        return *this;
    }

    inline command_group& command_group::set_help(const cppli::help& p_help) {
        help = p_help;
        return *this;
    }
    inline command_group& command_group::set_help(cppli::help&& p_help) {
        help = std::move(p_help);
        return *this;
    }


    // Context operators.
    inline context& operator | (context& lhs, const error& rhs) {
        lhs.error = rhs;
        return lhs;
    }
    inline context& operator | (context& lhs, error&& rhs) {
        lhs.error = std::move(rhs);
        return lhs;
    }
    inline context operator | (context&& lhs, const error& rhs) {
        lhs.error = rhs;
        return lhs;
    }
    inline context operator | (context&& lhs, error&& rhs) {
        lhs.error = std::move(rhs);
        return lhs;
    }

    inline context& operator | (context& lhs, const help& rhs) {
        lhs.help = rhs;
        return lhs;
    }
    inline context& operator | (context& lhs, help&& rhs) {
        lhs.help = std::move(rhs);
        return lhs;
    }
    inline context operator | (context&& lhs, const help& rhs) {
        lhs.help = rhs;
        return lhs;
    }
    inline context operator | (context&& lhs, help&& rhs) {
        lhs.help = std::move(rhs);
        return lhs;
    }


    // Command operators.
    inline command_group operator | (command& lhs, const command& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.add_command(rhs);
        return group;
    }
    inline command_group operator | (command& lhs, command&& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.add_command(std::move(rhs));
        return group;
    }
    inline command_group operator | (command&& lhs, const command& rhs) {
        auto group = command_group{};
        group.add_command(std::move(lhs));
        group.add_command(rhs);
        return group;
    }
    inline command_group operator | (command&& lhs, command&& rhs) {
        auto group = command_group{};
        group.add_command(std::move(lhs));
        group.add_command(std::move(rhs));
        return group;
    }

    inline command_group operator | (command& lhs, const error& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.set_error(rhs);
        return group;
    }
    inline command_group operator | (command& lhs, error&& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.set_error(std::move(rhs));
        return group;
    }
    inline command_group operator | (command&& lhs, const error& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.set_error(rhs);
        return group;
    }
    inline command_group operator | (command&& lhs, error&& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.set_error(std::move(rhs));
        return group;
    }

    inline command_group operator | (command& lhs, const help& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.set_help(rhs);
        return group;
    }
    inline command_group operator | (command& lhs, help&& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.set_help(std::move(rhs));
        return group;
    }
    inline command_group operator | (command&& lhs, const help& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.set_help(rhs);
        return group;
    }
    inline command_group operator | (command&& lhs, help&& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.set_help(std::move(rhs));
        return group;
    }

    inline command_group& operator | (command_group& lhs, const command& rhs) {
        lhs.add_command(rhs);
        return lhs;
    }
    inline command_group& operator | (command_group& lhs, command&& rhs) {
        lhs.add_command(std::move(rhs));
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, const command& rhs) {
        lhs.add_command(rhs);
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, command&& rhs) {
        lhs.add_command(std::move(rhs));
        return lhs;
    }

    inline command_group& operator | (command_group& lhs, const command_group& rhs) {
        lhs.add_commands(rhs);
        return lhs;
    }
    inline command_group& operator | (command_group& lhs, command_group&& rhs) {
        lhs.add_commands(std::move(rhs));
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, const command_group& rhs) {
        lhs.add_commands(rhs);
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, command_group&& rhs) {
        lhs.add_commands(std::move(rhs));
        return lhs;
    }

    inline command_group& operator | (command_group& lhs, const error& rhs) {
        lhs.set_error(rhs);
        return lhs;
    }
    inline command_group& operator | (command_group& lhs, error&& rhs) {
        lhs.set_error(std::move(rhs));
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, const error& rhs) {
        lhs.set_error(rhs);
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, error&& rhs) {
        lhs.set_error(std::move(rhs));
        return lhs;
    }

    inline command_group& operator | (command_group& lhs, const help& rhs) {
        lhs.set_help(rhs);
        return lhs;
    }
    inline command_group& operator | (command_group& lhs, help&& rhs) {
        lhs.set_help(std::move(rhs));
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, const help& rhs) {
        lhs.set_help(rhs);
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, help&& rhs) {
        lhs.set_help(std::move(rhs));
        return lhs;
    }

    inline std::string default_help_string(
        const context& p_context,
        const command_group& p_command_group) 
    {
        auto result = std::string{ "Usage: " };
        result += !p_context.current_path.empty() ? p_context.current_path.back() + " " : "";

        auto* help = impl::get_help(p_context, p_command_group);

        const size_t pre_command_count = 
            p_command_group.commands.size() + 
            (help && help->has_names()) ? size_t{ 1 } : size_t{ 0 };

        if (pre_command_count == 0) {
            return result;
        }

        size_t min_command_column = 0;
        std::vector<std::pair<std::string, std::string>> command_rows = {};
        command_rows.reserve(pre_command_count);

        auto add_command_rows = [&min_command_column, &command_rows](const std::vector<std::string>& names, const std::string& description) {
            const std::string name = !names.empty() ? names.front() : ""; // FIX!
            if (name.empty()) {
                return;
            }
            command_rows.emplace_back(name, description);
            min_command_column = std::max(min_command_column, name.size());
        };
        
        for (const auto& command : p_command_group.commands) {
            add_command_rows(command.names, command.description);
        }
        if(help) {
            add_command_rows(help->names, help->description);
        }

        if (command_rows.empty()) {
            return result;
        }

        result += "[command] [command-options]\n\ncommands:\n";

        for (const auto& command_row : command_rows) {
            const auto& [name, description] = command_row;
            result += "  " + name;
            const size_t tab_width = min_command_column - name.size() + size_t{ 6 };
            result += std::string(tab_width, ' ');
            result += description + "\n";
        }

        return result;
    }

}

#endif