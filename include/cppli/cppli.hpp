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
#include <charconv>
#include <iostream>
#include <cstring>
#include <cctype>

namespace cppli {

    struct error;
    struct default_error;
    struct help;
    struct default_help;
    struct context;

    struct command; 
    struct command_group;
     
    template<typename T> struct option;
    template<typename T> struct option_flag;
    struct option_group;

    class option_proxy;
    class option_flag_proxy;

    using error_callback = std::function<void(context&, std::string)>;
    using help_callback = std::function<int(context&)>;
    using command_callback = std::function<int(context&)>;;

    namespace parse_codes {
        static constexpr int successful = 0;
        static constexpr int successful_help = 1;
        static constexpr int missing_path = 2;
        static constexpr int missing_command = 3;
        static constexpr int unknown_command = 4;
        static constexpr int missing_option = 5;
        static constexpr int invalid_option = 6;
        static constexpr int unknown_option = 7;
    }

    context operator | (const context& lhs, const error& rhs);
    context operator | (const context& lhs, error&& rhs);
    context operator | (context&& lhs, const error& rhs);
    context operator | (context&& lhs, error&& rhs);

    context operator | (const context& lhs, const help& rhs);
    context operator | (const context& lhs, help&& rhs);
    context operator | (context&& lhs, const help& rhs);
    context operator | (context&& lhs, help&& rhs);

    command_group operator | (const command& lhs, const command& rhs);
    command_group operator | (const command& lhs, command&& rhs);
    command_group operator | (command&& lhs, const command& rhs);
    command_group operator | (command&& lhs, command&& rhs);

    command_group operator | (const command& lhs, const error& rhs);
    command_group operator | (const command& lhs, error&& rhs);
    command_group operator | (command&& lhs, const error& rhs);
    command_group operator | (command&& lhs, error&& rhs);

    command_group operator | (const command& lhs, const help& rhs);
    command_group operator | (const command& lhs, help&& rhs);
    command_group operator | (command&& lhs, const help& rhs);
    command_group operator | (command&& lhs, help&& rhs);

    command_group operator | (const command_group& lhs, const command& rhs);
    command_group operator | (const command_group& lhs, command&& rhs);
    command_group operator | (command_group&& lhs, const command& rhs);
    command_group operator | (command_group&& lhs, command&& rhs);

    command_group operator | (const command_group& lhs, const command_group& rhs);
    command_group operator | (const command_group& lhs, command_group&& rhs);
    command_group operator | (command_group&& lhs, const command_group& rhs);
    command_group operator | (command_group&& lhs, command_group&& rhs);

    command_group operator | (const command_group& lhs, const error& rhs);
    command_group operator | (const command_group& lhs, error&& rhs);
    command_group operator | (command_group&& lhs, const error& rhs);
    command_group operator | (command_group&& lhs, error&& rhs);

    command_group operator | (const command_group& lhs, const help& rhs);
    command_group operator | (const command_group& lhs, help&& rhs);
    command_group operator | (command_group&& lhs, const help& rhs);
    command_group operator | (command_group&& lhs, help&& rhs);

    template<typename T, typename U>
    option_group operator | (const option<T>& lhs, const option<U>& rhs);
    template<typename T, typename U>
    option_group operator | (const option<T>& lhs, option<U>&& rhs);
    template<typename T, typename U>
    option_group operator | (option<T>&& lhs, const option<U>& rhs);
    template<typename T, typename U>
    option_group operator | (option<T>&& lhs, option<U>&& rhs);

    template<typename T, typename U>
    option_group operator | (const option<T>& lhs, const option_flag<U>& rhs);
    template<typename T, typename U>
    option_group operator | (const option<T>& lhs, option_flag<U>&& rhs);
    template<typename T, typename U>
    option_group operator | (option<T>&& lhs, const option_flag<U>& rhs);
    template<typename T, typename U>
    option_group operator | (option<T>&& lhs, option_flag<U>&& rhs);

    template<typename T>
    option_group operator | (const option_group& lhs, const option<T>& rhs);
    template<typename T>
    option_group operator | (const option_group& lhs, option<T>&& rhs);
    template<typename T>
    option_group operator | (option_group&& lhs, const option<T>& rhs);
    template<typename T>
    option_group operator | (option_group&& lhs, option<T>&& rhs);

    template<typename T>
    option_group operator | (const option_group& lhs, const option_flag<T>& rhs);
    template<typename T>
    option_group operator | (const option_group& lhs, option_flag<T>&& rhs);
    template<typename T>
    option_group operator | (option_group&& lhs, const option_flag<T>& rhs);
    template<typename T>
    option_group operator | (option_group&& lhs, option_flag<T>&& rhs);

    template<typename T, typename U>
    option_group operator | (const option_flag<T>& lhs, const option_flag<U>& rhs);
    template<typename T, typename U>
    option_group operator | (const option_flag<T>& lhs, option_flag<U>&& rhs);
    template<typename T, typename U>
    option_group operator | (option_flag<T>&& lhs, const option_flag<U>& rhs);
    template<typename T, typename U>
    option_group operator | (option_flag<T>&& lhs, option_flag<U>&& rhs);

    template<typename T, typename U>
    option_group operator | (const option_flag<T>& lhs, const option<U>& rhs);
    template<typename T, typename U>
    option_group operator | (const option_flag<T>& lhs, option<U>&& rhs);
    template<typename T, typename U>
    option_group operator | (option_flag<T>&& lhs, const option<U>& rhs);
    template<typename T, typename U>
    option_group operator | (option_flag<T>&& lhs, option<U>&& rhs);
 
}

namespace cppli::impl {

    std::string_view parse_program_name(const char* path);

    bool handle_first_arg_is_path(context& p_context);

    template<typename T>
    const error* get_error_handler(const context& p_context, const T& p_other);

    error_callback get_error_callback(const error* p_error);

    template<typename T>
    const help* get_help_handler(const context& p_context, const T& p_other);

    using set_value_callback = std::function<bool(std::string_view)>;
    using set_flag_callback = std::function<void()>;

    template<typename T>
    set_value_callback create_set_value_callback(T& p_value);

    template<typename T>
    set_flag_callback create_set_flag_callback(T& p_value);

    template <typename, template <typename, typename...> typename>
    struct is_template_instance : public std::false_type {};

    template <typename...Ts, template <typename, typename...> typename U>
    struct is_template_instance<U<Ts...>, U> : public std::true_type {};

    template <typename T, template <typename, typename...> typename U>
    constexpr bool is_template_instance_v = is_template_instance<T, U>::value;

    template<typename T>
    struct raw_option_type {
        using type = T;
    };

    template<typename T>
    struct raw_option_type<std::optional<T>> {
        using type = T;
    };

    template<typename T>
    using raw_option_type_t = typename raw_option_type<T>::type;

}

namespace cppli {

    struct error {

        error_callback callback;

        error& set_callback(const error_callback& p_callback);
        error& set_callback(error_callback&& p_callback);

    };


    struct default_error : error {

        default_error();

        static void default_callback(context& p_context, std::string p_message);

    };


    struct help {

        std::vector<std::string> names = { "-h", "--help" };
        std::string description = "Show command line help.";
        help_callback callback = {};

        bool has_names() const;
        bool has_name(std::string_view p_name) const;

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

        explicit default_help(std::vector<std::string> names = { "-h", "--help" });

        static int default_callback(context& p_context);

        static std::string default_string(
            const context& p_context,
            const command_group& p_command_group);

        static std::string default_string(
            const context& p_context,
            const option_group& p_option_group);

        static std::string join_names(const std::vector<std::string>& names);

    };


    struct context {

        int argc = 0;
        char** argv = nullptr;
        bool first_arg_is_path = true;
        std::optional<error> error_handler = {};
        std::optional<help> help_handler = {};
        std::vector<std::string> current_path = {};
        std::optional<std::reference_wrapper<const command_group>> current_command_group = {};
        std::optional<std::reference_wrapper<const option_group>> current_option_group = {};

        context& set_arg(int p_argc, char** p_argv);
        context& set_argc(int p_argc);
        context& set_argv(char** p_argv);

        context& set_error(const error& p_error);
        context& set_error(error&& p_error);

        context& set_help(const help& p_help);
        context& set_help(help&& p_help);

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
        std::optional<error> error_handler = {};
        std::optional<help> help_handler = {};

        int parse(context& p_context) const;

        command_group& add_command(const command& p_command);
        command_group& add_command(command&& p_command);

        command_group& add_commands(command_group&& p_command_group);
        command_group& add_commands(const command_group& p_command_group);

        command_group& set_error(const error& p_error);
        command_group& set_error(error&& p_error);

        command_group& set_help(const help& p_help);
        command_group& set_help(help&& p_help);

    };


    template<typename T>
    struct option {
        T& value;
        std::vector<std::string> names = {};
        std::string description = "";

        option& set_name(const std::string& p_name);
        option& set_name(std::string&& p_name);

        option& set_names(std::initializer_list<std::string> p_names);
        option& set_names(const std::vector<std::string>& p_names);
        option& set_names(std::vector<std::string>&& p_names);

        option& set_description(const std::string& p_description);
        option& set_description(std::string&& p_description);

    };


    class option_proxy {

    public:

        std::vector<std::string> names = {};
        std::string description = "";

        template<typename T>
        option_proxy(const option<T>& p_option);
        template<typename T>
        option_proxy(option<T>&& p_option);

        option_proxy(const option_proxy&) = default;
        option_proxy(option_proxy&&) = default;
        option_proxy& operator = (const option_proxy&) = default;
        option_proxy& operator = (option_proxy&&) = default;

        bool has_names() const;

        bool set_value(std::string_view p_value) const;

    private:

        impl::set_value_callback m_set_value_callback;

    };


    template<typename T = bool>
    struct option_flag {
        T& value;
        std::vector<std::string> names = {};
        std::string description = "";

        option_flag& set_name(const std::string& p_name);
        option_flag& set_name(std::string&& p_name);

        option_flag& set_names(std::initializer_list<std::string> p_names);
        option_flag& set_names(const std::vector<std::string>& p_names);
        option_flag& set_names(std::vector<std::string>&& p_names);

        option_flag& set_description(const std::string& p_description);
        option_flag& set_description(std::string&& p_description);
    };


    class option_flag_proxy {

    public:

        std::vector<std::string> names = {};
        std::string description = "";

        template<typename T>
        option_flag_proxy(const option_flag<T>& p_option_flag);
        template<typename T>
        option_flag_proxy(option_flag<T>&& p_option_flag);

        option_flag_proxy(const option_flag_proxy&) = default;
        option_flag_proxy(option_flag_proxy&&) = default;
        option_flag_proxy& operator = (const option_flag_proxy&) = default;
        option_flag_proxy& operator = (option_flag_proxy&&) = default;

        bool has_names() const;

        bool set_flag() const;

    private:

        impl::set_flag_callback m_set_flag_callback;
    };


    struct option_group {

        std::vector<option_proxy> required_options = {};
        std::vector<option_proxy> optional_options = {};
        std::vector<option_flag_proxy> flag_options = {};
        std::optional<error> error_handler = {};
        std::optional<help> help_handler = {};

        int parse(context& p_context) const;

        template<typename T>
        option_group& add_option(const option<T>& p_option);
        template<typename T>
        option_group& add_option(option<T>&& p_option);

        template<typename T>
        option_group& add_option(const option_flag<T>& p_option_flag);
        template<typename T>
        option_group& add_option(option_flag<T>&& p_option_flag);

        option_group& set_error(const error& p_error);
        option_group& set_error(error&& p_error);

        option_group& set_help(const help& p_help);
        option_group& set_help(help&& p_help);

    };

}


// Implementations.
namespace cppli::impl {

    inline std::string_view parse_program_name(const char* path) {
        const auto path_view = std::string_view{ path, strlen(path) };

        const auto last_dir_pos = path_view.find_last_of("/\\");
        const auto program_offset = last_dir_pos != std::string_view::npos ? last_dir_pos + 1 : 0;
        const auto program_exe_len = path_view.size() - program_offset;

        const auto program_exe_view = std::string_view{ path_view.data() + program_offset, program_exe_len };

        const auto last_ext_pos = program_exe_view.find_last_of('.');
        const auto program_len = last_ext_pos != std::string_view::npos ? last_ext_pos : program_exe_view.size();

        return std::string_view{ program_exe_view.data(), program_len };
    }

    inline bool handle_first_arg_is_path(context& p_context) {
        if (p_context.first_arg_is_path) {
            if (p_context.argc <= 0) {
                return false;
            }
                
            auto program_name = impl::parse_program_name(p_context.argv[0]);
            if (!program_name.empty()) {
                p_context.current_path.emplace_back(program_name);
            }
            p_context.move_to_next_arg();
        }
        return true;
    }

    template<typename T>
    inline const error* get_error_handler(const context& p_context, const T& p_other) {
        return p_other.error_handler.has_value() ?
            &p_other.error_handler.value() :
            p_context.error_handler.has_value() ?
                &p_context.error_handler.value() :
                nullptr;
    }

    inline error_callback get_error_callback(const error* p_error) {
        return p_error ? p_error->callback : [](context&, std::string) {};
    }

    template<typename T>
    inline const help* get_help_handler(const context& p_context, const T& p_other) {
        return p_other.help_handler.has_value() ?
            &p_other.help_handler.value() :
            p_context.help_handler.has_value() ?
                &p_context.help_handler.value() :
                nullptr;
    }

    template<typename T>
    inline set_value_callback create_set_value_callback(T& p_value) {
        return [&p_value](std::string_view string) mutable {
            static_assert(!std::is_const_v<T>, "Option value cannot be const.");
            static_assert(std::is_reference_v<decltype(p_value)>, "Option value must be a reference.");

            using type = typename impl::raw_option_type_t<std::decay_t<decltype(p_value)>>;

            if constexpr (std::is_same_v<type, bool> == true) {
                auto string_copy = std::string{ string };
                std::transform(string_copy.begin(), string_copy.end(), string_copy.begin(), [](const auto c) {
                    return static_cast<decltype(c)>(std::tolower(static_cast<int>(c)));
                    });

                if (string_copy == "true" || string_copy == "1") {
                    p_value = true;
                    return true;
                }
                if (string_copy == "false" || string_copy == "0") {
                    p_value = false;
                    return true;
                }

                return false;
            }
            else if constexpr (std::is_integral_v<type> == true || std::is_floating_point_v<type> == true) {
                type value{};
                const auto result = std::from_chars(string.data(), string.data() + string.size(), value).ec == std::errc();
                if (result) {
                    p_value = value;
                }
                return result;
            }
            else {
                p_value = string;
                return true;
            }
        };
    }

    template<typename T>
    inline set_flag_callback create_set_flag_callback(T& p_value) {
        return [&p_value]() mutable {
            static_assert(!std::is_const_v<T>, "Option value cannot be const.");
            static_assert(std::is_reference_v<decltype(p_value)>, "Option value must be a reference.");

            using type = typename impl::raw_option_type_t<std::decay_t<decltype(p_value)>>;

            p_value = static_cast<type>(true);
        };
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
        error{ &default_callback }
    {}

    inline void default_error::default_callback(context&, std::string p_message) {
        std::cerr << p_message << "\n";
    }



    // Help
    inline bool help::has_names() const {
        auto it = std::find_if(names.begin(), names.end(),
            [](const auto& name) { return !name.empty(); });

        return it != names.end();
    }

    inline bool help::has_name(std::string_view p_name) const {
        auto it = std::find_if(names.begin(), names.end(), [p_name](const auto& name) {
            return name == p_name;
        });

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
        help{ std::move(names), "Show command line help.", &default_callback }
    {}

    inline int default_help::default_callback(context& p_context) {
        if (p_context.current_command_group.has_value()) {
            std::cout << default_string(
                p_context,
                p_context.current_command_group.value());
        }
        else if (p_context.current_option_group.has_value()) {
            std::cout << default_string(
                p_context,
                p_context.current_option_group.value());
        }

        return parse_codes::successful_help;
    }

    inline std::string default_help::default_string(
        const context& p_context,
        const command_group& p_command_group)
    {
        auto result = std::string{ "Usage: " };
        result += !p_context.current_path.empty() ? p_context.current_path.back() + " " : "";

        auto* help_handler = impl::get_help_handler(p_context, p_command_group);

        const size_t pre_command_count =
            p_command_group.commands.size() +
            ((help_handler && help_handler->has_names()) ? size_t{ 1 } : size_t{ 0 });

        if (pre_command_count == 0) {
            return result;
        }

        size_t min_command_column = 0;
        std::vector<std::pair<std::string, std::string>> command_rows = {};
        command_rows.reserve(pre_command_count);

        auto add_command_row = [&](const std::vector<std::string>& p_names, const std::string& p_description) {
            const std::string name = join_names(p_names);
            if (name.empty()) {
                return;
            }
            command_rows.emplace_back(name, p_description);
            min_command_column = std::max(min_command_column, name.size());
        };

        if (help_handler) {
            add_command_row(help_handler->names, help_handler->description);
        }
        for (const auto& command : p_command_group.commands) {
            add_command_row(command.names, command.description);
        }

        if (command_rows.empty()) {
            return result;
        }

        result += "[command] [command-options]\n\nCommands:\n";

        for (const auto& command_row : command_rows) {
            const auto& [name, description] = command_row;
            result += "  " + name;
            const size_t tab_width = min_command_column - name.size() + size_t{ 6 };
            result += std::string(tab_width, ' ');
            result += description + "\n";
        }

        return result;
    }

    inline std::string default_help::default_string(
        const context& p_context,
        const option_group& p_option_group)
    {
        auto result = std::string{ "Usage: " };
        result += !p_context.current_path.empty() ? p_context.current_path.back() + " " : "";

        auto* help_handler = impl::get_help_handler(p_context, p_option_group);

        const size_t pre_command_count =
            p_option_group.required_options.size() +
            p_option_group.optional_options.size() +
            p_option_group.flag_options.size() +
            ((help_handler && help_handler->has_names()) ? size_t{ 1 } : size_t{ 0 });

        if (pre_command_count == 0) {
            return result;
        }

        size_t min_command_column = 0;
        std::vector<std::pair<std::string, std::string>> option_rows = {};
        option_rows.reserve(pre_command_count);

        auto add_option_row = [&](const std::vector<std::string>& p_names, const std::string& p_description) {
            const std::string name = join_names(p_names);
            if (name.empty()) {
                return;
            }
            option_rows.emplace_back(name, p_description);
            min_command_column = std::max(min_command_column, name.size());
        };

        if (help_handler) {
            add_option_row(help_handler->names, help_handler->description);
        }
        for (const auto& option : p_option_group.required_options) {
            add_option_row(option.names, option.description);

            if (option.has_names()) {
                result += "<" + option.names.front() + "> ";
            }        
        }
        for (const auto& option : p_option_group.optional_options) {
            add_option_row(option.names, option.description);
        }
        for (const auto& option : p_option_group.flag_options) {
            add_option_row(option.names, option.description);
        }

        if (option_rows.empty()) {
            return result;
        }

        result += "[options]\n\nOptions:\n";

        for (const auto& option_row : option_rows) {
            const auto& [name, description] = option_row;
            result += "  " + name;
            const size_t tab_width = min_command_column - name.size() + size_t{ 6 };
            result += std::string(tab_width, ' ');
            result += description + "\n";
        }

        return result;
    }

    inline std::string default_help::join_names(const std::vector<std::string>& names) {
        if (names.empty()) {
            return "";
        }

        std::string result = names.front();
        for (auto it = std::next(names.begin()); it != names.end(); ++it) {
            const auto& name = *it;
            if (name.empty()) {
                continue;
            }
            result += "|" + name;
        }

        return result;
    }

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

    inline context& context::set_error(const error& p_error) {
        error_handler = p_error;
        return *this;
    }
    inline context& context::set_error(error&& p_error) {
        error_handler = std::move(p_error);
        return *this;
    }

    inline context& context::set_help(const help& p_help) {
        help_handler = p_help;
        return *this;
    }
    inline context& context::set_help(help&& p_help) {
        help_handler = std::move(p_help);
        return *this;
    }

    inline context& context::move_to_next_arg() {
        if (argc <= 0) {
            return *this;
        }
        first_arg_is_path = false;
        --argc;
        ++argv;

        while (argc > 0) {
            if (argv[0] != nullptr) {
                break;
            }
            --argc;
            ++argv;
        }

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
        p_context.current_command_group = *this;
        p_context.current_option_group.reset();

        auto* current_help_handler = impl::get_help_handler(p_context, *this);
        auto* current_error_handler = impl::get_error_handler(p_context, *this);
        auto error_callback = impl::get_error_callback(current_error_handler);

        if (!impl::handle_first_arg_is_path(p_context)) {
            error_callback(p_context, "Missing path.");
            return parse_codes::missing_path;
        }

        if (p_context.argc < 1) {
            error_callback(p_context, "Missing command.");
            return parse_codes::missing_command;
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
        if (command == nullptr) {
            // Help
            if (current_help_handler != nullptr && current_help_handler->callback &&
                current_help_handler->has_name(command_name))
            {
                return current_help_handler->callback(p_context);
            }

            error_callback(p_context, "Unknown command '" + std::string{ command_name } + "'.");
            return parse_codes::unknown_command;
        }

        p_context.current_path.emplace_back(command_name);
        p_context.move_to_next_arg();

        return command->callback ? command->callback(p_context) : parse_codes::successful;
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

        if (!error_handler.has_value() && p_command_group.error_handler.has_value()) {
            error_handler = std::move(p_command_group.error_handler);
        }

        if (!help_handler.has_value() && p_command_group.help_handler.has_value()) {
            help_handler = std::move(p_command_group.help_handler);
        }

        return *this;
    }
    inline command_group& command_group::add_commands(const command_group& p_command_group) {
        commands.insert(
            commands.end(),
            p_command_group.commands.begin(),
            p_command_group.commands.end());

        if (!error_handler.has_value() && p_command_group.error_handler.has_value()) {
            error_handler = p_command_group.error_handler;
        }

        if (!help_handler.has_value() && p_command_group.help_handler.has_value()) {
            help_handler = p_command_group.help_handler;
        }

        return *this;
    }

    inline command_group& command_group::set_error(const error& p_error) {
        error_handler = p_error;
        return *this;
    }
    inline command_group& command_group::set_error(error&& p_error) {
        error_handler = std::move(p_error);
        return *this;
    }

    inline command_group& command_group::set_help(const help& p_help) {
        help_handler = p_help;
        return *this;
    }
    inline command_group& command_group::set_help(help&& p_help) {
        help_handler = std::move(p_help);
        return *this;
    }


    // Option.
    template<typename T>
    inline option<T>& option<T>::set_name(const std::string& p_name) {
        names = { p_name };
        return *this;
    }
    template<typename T>
    inline option<T>& option<T>::set_name(std::string&& p_name) {
        names = { std::move(p_name) };
        return *this;
    }

    template<typename T>
    inline option<T>& option<T>::set_names(std::initializer_list<std::string> p_names) {
        names = p_names;
        return *this;
    }
    template<typename T>
    inline option<T>& option<T>::set_names(const std::vector<std::string>& p_names) {
        names = p_names;
        return *this;
    }
    template<typename T>
    inline option<T>& option<T>::set_names(std::vector<std::string>&& p_names) {
        names = std::move(p_names);
        return *this;
    }

    template<typename T>
    inline option<T>& option<T>::set_description(const std::string& p_description) {
        description = p_description;
        return *this;
    }
    template<typename T>
    inline option<T>& option<T>::set_description(std::string&& p_description) {
        description = std::move(p_description);
        return *this;
    }


    // Option proxy.
    template<typename T>
    inline option_proxy::option_proxy(const option<T>& p_option) :
        names(p_option.names),
        description(p_option.description),
        m_set_value_callback(impl::create_set_value_callback<T>(p_option.value))
    {}

    template<typename T>
    inline option_proxy::option_proxy(option<T>&& p_option) :
        names(std::move(p_option.names)),
        description(std::move(p_option.description)),
        m_set_value_callback(impl::create_set_value_callback<T>(p_option.value))
    {}

    inline bool option_proxy::has_names() const {
        auto it = std::find_if(names.begin(), names.end(), [](const auto& name) { 
            return !name.empty(); 
         });

        return it != names.end();
    }

    inline bool option_proxy::set_value(std::string_view p_value) const {
        if (!m_set_value_callback) {
            return false;
        }
        return m_set_value_callback(p_value);
    }

    // Option flag.
    template<typename T>
    inline option_flag<T>& option_flag<T>::set_name(const std::string& p_name) {
        names = { p_name };
        return *this;
    }
    template<typename T>
    inline option_flag<T>& option_flag<T>::set_name(std::string&& p_name) {
        names = { std::move(p_name) };
        return *this;
    }

    template<typename T>
    inline option_flag<T>& option_flag<T>::set_names(std::initializer_list<std::string> p_names) {
        names = p_names;
        return *this;
    }
    template<typename T>
    inline option_flag<T>& option_flag<T>::set_names(const std::vector<std::string>& p_names) {
        names = p_names;
        return *this;
    }
    template<typename T>
    inline option_flag<T>& option_flag<T>::set_names(std::vector<std::string>&& p_names) {
        names = std::move(p_names);
        return *this;
    }

    template<typename T>
    inline option_flag<T>& option_flag<T>::set_description(const std::string& p_description) {
        description = p_description;
        return *this;
    }
    template<typename T>
    inline option_flag<T>& option_flag<T>::set_description(std::string&& p_description) {
        description = std::move(p_description);
        return *this;
    }


    // Option flag proxy.
    template<typename T>
    inline option_flag_proxy::option_flag_proxy(const option_flag<T>& p_option_flag) :
        names(p_option_flag.names),
        description(p_option_flag.description),
        m_set_flag_callback(impl::create_set_flag_callback<T>(p_option_flag.value))
    {}

    template<typename T>
    inline option_flag_proxy::option_flag_proxy(option_flag<T>&& p_option_flag) :
        names(std::move(p_option_flag.names)),
        description(std::move(p_option_flag.description)),
        m_set_flag_callback(impl::create_set_flag_callback<T>(p_option_flag.value))
    {}

    
    inline bool option_flag_proxy::has_names() const {
        auto it = std::find_if(names.begin(), names.end(), [](const auto& name) {
            return !name.empty();
        });

        return it != names.end();
    }

    inline bool option_flag_proxy::set_flag() const {
        if (!m_set_flag_callback) {
            return false;
        }
        m_set_flag_callback();
        return true;
    }

    
    // Option group.
    inline int option_group::parse(context& p_context) const {
        p_context.current_command_group.reset();
        p_context.current_option_group = *this;
        
        auto* current_help_handler = impl::get_help_handler(p_context, *this);
        auto* current_error_handler = impl::get_error_handler(p_context, *this);
        auto error_callback = impl::get_error_callback(current_error_handler); 

        if (!impl::handle_first_arg_is_path(p_context)) {
            error_callback(p_context, "Missing path.");
            return parse_codes::missing_path;
        }

        // Help
        if (current_help_handler != nullptr && current_help_handler->callback &&
            p_context.argc > 0 && current_help_handler->has_name(p_context.argv[0]))
        {
            return current_help_handler->callback(p_context);
        }

        // Required options.
        for (auto& option : required_options) {
            auto first_opt_name = !option.names.empty() ? option.names.front() : "opt";

            if (p_context.argc <= 0) {
                error_callback(p_context, "Missing option '" + first_opt_name + "'.");
                return parse_codes::missing_option;
            }

            const auto opt_value = std::string_view{ p_context.argv[0] };

            if (!option.set_value(opt_value)) {
                error_callback(p_context, "Invalid value '" + std::string{ opt_value } + "' of option '" + first_opt_name + "'.");
                return parse_codes::invalid_option;
            }

            p_context.move_to_next_arg();
        } 
        
        while (p_context.argc > 0) {
            const auto opt_name = std::string_view{ p_context.argv[0] };

            // Flag options.
            auto flag_it = std::find_if(flag_options.begin(), flag_options.end(), [&opt_name](const auto& opt) {
                for (const auto& name : opt.names) {
                    if (name == opt_name) {
                        return true;
                    }
                }
                return false;
            });

            if (flag_it != flag_options.end()) {
                auto& option = *flag_it;

                if (!option.set_flag()) {
                    error_callback(p_context, "Failed to set flag of option '" + std::string{ opt_name } + "'.");
                    return parse_codes::invalid_option;
                }

                p_context.move_to_next_arg();
                continue;
            }

            // Optional options.
            auto optional_it = std::find_if(optional_options.begin(), optional_options.end(), [&opt_name](const auto& opt) {
                for (const auto& name : opt.names) {
                    if (name == opt_name) {
                        return true;
                    }
                }
                return false;
            });

            if (optional_it != optional_options.end()) {
                auto& option = *optional_it;
                p_context.move_to_next_arg();

                auto first_opt_name = !option.names.empty() ? option.names.front() : "opt";

                if (p_context.argc <= 0) {
                    error_callback(p_context, "Missing value of option '" + first_opt_name + "'.");
                    return parse_codes::missing_option;
                }

                const auto opt_value = std::string_view{ p_context.argv[0] };
                
                if (!option.set_value(opt_value)) {
                    error_callback(p_context, "Invalid value '" + std::string{ opt_value } + "' of option '" + first_opt_name + "'.");
                    return parse_codes::invalid_option;
                }

                p_context.move_to_next_arg();
                continue;
            }
     
            error_callback(p_context, "Unknown option '" + std::string{ opt_name } + "'.");
            return parse_codes::unknown_option;
        }

        return parse_codes::successful;
    }

    template<typename T>
    inline option_group& option_group::add_option(const option<T>& p_option) {
        if constexpr (impl::is_template_instance_v<T, std::optional> == true) {
            optional_options.emplace_back(p_option);
        }
        else {
            required_options.emplace_back(p_option);
        }
        
        return *this;
    }
    template<typename T>
    inline option_group& option_group::add_option(option<T>&& p_option) {
        if constexpr (impl::is_template_instance_v<T, std::optional> == true) {
            optional_options.emplace_back(p_option);
        }
        else {
            required_options.emplace_back(p_option);
        }
        return *this;
    }

    template<typename T>
    inline option_group& option_group::add_option(const option_flag<T>& p_option_flag) {
        flag_options.emplace_back(p_option_flag);
        return *this;
    }
    template<typename T>
    inline option_group& option_group::add_option(option_flag<T>&& p_option_flag) {
        flag_options.emplace_back(p_option_flag);
        return *this;
    }

    inline option_group& option_group::set_error(const error& p_error) {
        error_handler = p_error;
        return *this;
    }
    inline option_group& option_group::set_error(error&& p_error) {
        error_handler = std::move(p_error);
        return *this;
    }

    inline option_group& option_group::set_help(const help& p_help) {
        help_handler = p_help;
        return *this;
    }
    inline option_group& option_group::set_help(help&& p_help) {
        help_handler = std::move(p_help);
        return *this;
    }


    // Context operators.
    inline context operator | (const context& lhs, const error& rhs) {
        auto new_context = context{ lhs };
        new_context.set_error(rhs);
        return new_context;
    }
    inline context operator | (const context& lhs, error&& rhs) {
        auto new_context = context{ lhs };
        new_context.set_error(std::move(rhs));
        return new_context;
    }
    inline context operator | (context&& lhs, const error& rhs) {
        lhs.set_error(rhs);
        return lhs;
    }
    inline context operator | (context&& lhs, error&& rhs) {
        lhs.set_error(std::move(rhs));
        return lhs;
    }

    inline context operator | (const context& lhs, const help& rhs) {
        auto new_context = context{ lhs };
        new_context.set_help(rhs);
        return new_context;
    }
    inline context operator | (const context& lhs, help&& rhs) {
        auto new_context = context{ lhs };
        new_context.set_help(std::move(rhs));
        return new_context;
    }
    inline context operator | (context&& lhs, const help& rhs) {
        lhs.set_help(rhs);
        return lhs;
    }
    inline context operator | (context&& lhs, help&& rhs) {
        lhs.set_help(std::move(rhs));
        return lhs;
    }


    // Command operators.
    inline command_group operator | (const command& lhs, const command& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.add_command(rhs);
        return group;
    }
    inline command_group operator | (const command& lhs, command&& rhs) {
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

    inline command_group operator | (const command& lhs, const error& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.set_error(rhs);
        return group;
    }
    inline command_group operator | (const command& lhs, error&& rhs) {
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

    inline command_group operator | (const command& lhs, const help& rhs) {
        auto group = command_group{};
        group.add_command(lhs);
        group.set_help(rhs);
        return group;
    }
    inline command_group operator | (const command& lhs, help&& rhs) {
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

    inline command_group operator | (const command_group& lhs, const command& rhs) {
        auto group = command_group{ lhs };
        group.add_command(rhs);
        return group;
    }
    inline command_group operator | (const command_group& lhs, command&& rhs) {
        auto group = command_group{ lhs };
        group.add_command(std::move(rhs));
        return group;
    }
    inline command_group operator | (command_group&& lhs, const command& rhs) {
        lhs.add_command(rhs);
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, command&& rhs) {
        lhs.add_command(std::move(rhs));
        return lhs;
    }

    inline command_group operator | (const command_group& lhs, const command_group& rhs) {
        auto group = command_group{ lhs };
        group.add_commands(rhs);
        return group;
    }
    inline command_group operator | (const command_group& lhs, command_group&& rhs) {
        auto group = command_group{ lhs };
        group.add_commands(std::move(rhs));
        return group;
    }
    inline command_group operator | (command_group&& lhs, const command_group& rhs) {
        lhs.add_commands(rhs);
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, command_group&& rhs) {
        lhs.add_commands(std::move(rhs));
        return lhs;
    }

    inline command_group operator | (const command_group& lhs, const error& rhs) {
        auto group = command_group{ lhs };
        group.set_error(rhs);
        return group;
    }
    inline command_group operator | (const command_group& lhs, error&& rhs) {
        auto group = command_group{ lhs };
        group.set_error(std::move(rhs));
        return group;
    }
    inline command_group operator | (command_group&& lhs, const error& rhs) {
        lhs.set_error(rhs);
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, error&& rhs) {
        lhs.set_error(std::move(rhs));
        return lhs;
    }

    inline command_group operator | (const command_group& lhs, const help& rhs) {
        auto group = command_group{ lhs };
        group.set_help(rhs);
        return group;
    }
    inline command_group operator | (const command_group& lhs, help&& rhs) {
        auto group = command_group{ lhs };
        group.set_help(std::move(rhs));
        return group;
    }
    inline command_group operator | (command_group&& lhs, const help& rhs) {
        lhs.set_help(rhs);
        return lhs;
    }
    inline command_group operator | (command_group&& lhs, help&& rhs) {
        lhs.set_help(std::move(rhs));
        return lhs;
    }

    template<typename T, typename U>
    inline option_group operator | (const option<T>& lhs, const option<U>& rhs) {
        auto group = option_group{};
        group.add_option(lhs);
        group.add_option(rhs);
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (const option<T>& lhs, option<U>&& rhs) {
        auto group = option_group{};
        group.add_option(lhs);
        group.add_option(std::move(rhs));
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (option<T>&& lhs, const option<U>& rhs) {
        auto group = option_group{};
        group.add_option(std::move(lhs));
        group.add_option(rhs);
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (option<T>&& lhs, option<U>&& rhs) {
        auto group = option_group{};
        group.add_option(std::move(lhs));
        group.add_option(std::move(rhs));
        return group;
    }

    template<typename T, typename U>
    inline option_group operator | (const option<T>& lhs, const option_flag<U>& rhs) {
        auto group = option_group{};
        group.add_option(lhs);
        group.add_option(rhs);
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (const option<T>& lhs, option_flag<U>&& rhs) {
        auto group = option_group{};
        group.add_option(lhs);
        group.add_option(std::move(rhs));
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (option<T>&& lhs, const option_flag<U>& rhs) {
        auto group = option_group{};
        group.add_option(std::move(lhs));
        group.add_option(rhs);
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (option<T>&& lhs, option_flag<U>&& rhs) {
        auto group = option_group{};
        group.add_option(std::move(lhs));
        group.add_option(std::move(rhs));
        return group;
    }

    template<typename T>
    inline option_group operator | (const option_group& lhs, const option<T>& rhs) {
        auto group = option_group{ lhs };
        group.add_option(rhs);
        return group;
    }
    template<typename T>
    inline option_group operator | (const option_group& lhs, option<T>&& rhs) {
        auto group = option_group{ lhs };
        group.add_option(std::move(rhs));
        return group;
    }
    template<typename T>
    inline option_group operator | (option_group&& lhs, const option<T>& rhs) {
        lhs.add_option(rhs);
        return lhs;
    }
    template<typename T>
    inline option_group operator | (option_group&& lhs, option<T>&& rhs) {
        lhs.add_option(std::move(rhs));
        return lhs;
    }

    template<typename T>
    inline option_group operator | (const option_group& lhs, const option_flag<T>& rhs) {
        auto group = option_group{ lhs };
        group.add_option(rhs);
        return group;
    }
    template<typename T>
    inline option_group operator | (const option_group& lhs, option_flag<T>&& rhs) {
        auto group = option_group{ lhs };
        group.add_option(std::move(rhs));
        return group;
    }
    template<typename T>
    inline option_group operator | (option_group&& lhs, const option_flag<T>& rhs) {
        lhs.add_option(rhs);
        return lhs;
    }
    template<typename T>
    inline option_group operator | (option_group&& lhs, option_flag<T>&& rhs) {
        lhs.add_option(std::move(rhs));
        return lhs;
    }

    template<typename T, typename U>
    inline option_group operator | (const option_flag<T>& lhs, const option_flag<U>& rhs) {
        auto group = option_group{};
        group.add_option(lhs);
        group.add_option(rhs);
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (const option_flag<T>& lhs, option_flag<U>&& rhs) {
        auto group = option_group{};
        group.add_option(lhs);
        group.add_option(std::move(rhs));
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (option_flag<T>&& lhs, const option_flag<U>& rhs) {
        auto group = option_group{};
        group.add_option(std::move(lhs));
        group.add_option(rhs);
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (option_flag<T>&& lhs, option_flag<U>&& rhs) {
        auto group = option_group{};
        group.add_option(std::move(lhs));
        group.add_option(std::move(rhs));
        return group;
    }

    template<typename T, typename U>
    inline option_group operator | (const option_flag<T>& lhs, const option<U>& rhs) {
        auto group = option_group{};
        group.add_option(lhs);
        group.add_option(rhs);
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (const option_flag<T>& lhs, option<U>&& rhs) {
        auto group = option_group{};
        group.add_option(lhs);
        group.add_option(std::move(rhs));
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (option_flag<T>&& lhs, const option<U>& rhs) {
        auto group = option_group{};
        group.add_option(std::move(lhs));
        group.add_option(rhs);
        return group;
    }
    template<typename T, typename U>
    inline option_group operator | (option_flag<T>&& lhs, option<U>&& rhs) {
        auto group = option_group{};
        group.add_option(std::move(lhs));
        group.add_option(std::move(rhs));
        return group;
    }


}

#endif