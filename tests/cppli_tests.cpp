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

#include "cppli_tests.hpp"

int main(int argc, char** argv)
{
    // Command line arguments for running a single test:
    // --gtest_filter=Test.Case*

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

namespace cli = cppli;

TEST(command_group, empty_group)
{
    auto group = cli::command_group{};

    EXPECT_TRUE(group.commands.empty());
    EXPECT_FALSE(group.error_handler.has_value());
    EXPECT_FALSE(group.help_handler.has_value());

    // FAIL
    {
        auto context = cli::context{}.set_arg(0, nullptr);

        EXPECT_EQ(context.argc, 0);
        EXPECT_EQ(context.argv, nullptr);
        EXPECT_EQ(group.parse(context), cli::parse_codes::missing_path);
    }
    {
        auto args = std::array{ "path/to/program" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 1);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::missing_command);
    }
    {
        auto args = std::array{ "path/to/program", "test" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_command);
    }
    {
        auto args = std::array{ "path/to/program", "", "test" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 3);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_command);
    }
}

TEST(command_group, single_command)
{
    bool triggered_callback = false;

    const auto reset_triggered_callback = [&triggered_callback]() {
        triggered_callback = false;
    };

    auto group
        = cli::command_group{}
        | cli::command{}
            .set_names({ "yolo", "swag" })
            .set_description("Testning")
            .set_callback([&](auto&) { triggered_callback = true; return 123; });
    
    static_assert(std::is_same_v<decltype(group), cli::command_group>, 
        "Expecting type to be cli::command_group");

    EXPECT_FALSE(triggered_callback);
    EXPECT_EQ(group.commands.size(), size_t{ 1 });
    EXPECT_FALSE(group.error_handler.has_value());
    EXPECT_FALSE(group.help_handler.has_value());

    // FAIL
    reset_triggered_callback();
    {
        auto args = std::array{ "path/to/program", "test" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_command);
        EXPECT_FALSE(triggered_callback);
    }
    reset_triggered_callback();
    {
        auto args = std::array{ "path/to/program", "" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_command);
        EXPECT_FALSE(triggered_callback);
    }

    // OK
    reset_triggered_callback();
    {
        auto args = std::array{ "path/to/program", "yolo" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 123);
        EXPECT_TRUE(triggered_callback);
    }
    reset_triggered_callback();
    {
        auto args = std::array{ "path/to/program", "swag" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 123);
        EXPECT_TRUE(triggered_callback);
    }
    reset_triggered_callback();
    {
        auto args = std::array{ "path/to/program", "", "swag" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 3);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_command);
        EXPECT_FALSE(triggered_callback);
    }
}

TEST(command_group, multiple_commands)
{
    auto triggered_callbacks = std::array{ false, false, false };

    const auto reset_triggered_callbacks = [&triggered_callbacks]() {
        triggered_callbacks = { false, false, false };
    };

    const auto check_triggered_callbacks = [&triggered_callbacks](bool a, bool b, bool c) {
        return triggered_callbacks[0] == a && triggered_callbacks[1] == b && triggered_callbacks[2] == c;
    };

    auto group
        = cli::command{}
            .set_names({ "yolo", "swag" })
            .set_description("Testning 1")
            .set_callback([&](auto&) { triggered_callbacks[0] = true; return 100; })
        | cli::command{}
            .set_names({ "hello" })
            .set_description("Testning 2")
            .set_callback([&](auto&) { triggered_callbacks[1] = true; return 200; })
        | cli::command{}
            .set_names({ "foo", "bar" })
            .set_description("Testning 3")
            .set_callback([&](auto&) { triggered_callbacks[2] = true; return 300; });

    static_assert(std::is_same_v<decltype(group), cli::command_group>,
        "Expecting type to be cli::command_group");

    EXPECT_TRUE(check_triggered_callbacks(false, false, false));
    EXPECT_EQ(group.commands.size(), size_t{ 3 });
    EXPECT_FALSE(group.error_handler.has_value());
    EXPECT_FALSE(group.help_handler.has_value());

    // FAIL
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/program", "test" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_command);
        EXPECT_TRUE(check_triggered_callbacks(false, false, false));
    }
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/program", "" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_command);
        EXPECT_TRUE(check_triggered_callbacks(false, false, false));
    }

    // OK
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/program", "yolo" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 100);
        EXPECT_TRUE(check_triggered_callbacks(true, false, false));
    }
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/program", "swag" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 100);
        EXPECT_TRUE(check_triggered_callbacks(true, false, false));
    }
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/program", "hello" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 200);
        EXPECT_TRUE(check_triggered_callbacks(false, true, false));
    }
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/program", "foo" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 300);
        EXPECT_TRUE(check_triggered_callbacks(false, false, true));
    }
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/program", "", "bar" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 3);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_command);
        EXPECT_TRUE(check_triggered_callbacks(false, false, false));
    }
}

TEST(option_group, empty_group)
{
    auto group = cli::option_group{};

    EXPECT_TRUE(group.required_options.empty());
    EXPECT_TRUE(group.optional_options.empty());
    EXPECT_TRUE(group.flag_options.empty());
    EXPECT_FALSE(group.error_handler.has_value());
    EXPECT_FALSE(group.help_handler.has_value());

    // FAIL
    {
        auto context = cli::context{}.set_arg(0, nullptr);

        EXPECT_EQ(context.argc, 0);
        EXPECT_EQ(context.argv, nullptr);
        EXPECT_EQ(group.parse(context), cli::parse_codes::missing_path);
    }
    {
        auto args = std::array{ "path/to/program", "test" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_option);
    }
    {
        auto args = std::array{ "path/to/program", "", "test" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 3);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_option);
    }

    // OK
    {
        auto args = std::array{ "path/to/program" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 1);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::successful);
    }
}

TEST(option_group, single_option)
{
    int value = 0;

    auto group 
        = cli::option_group{}
        | cli::option<int>{ value, "value"};

    EXPECT_EQ(group.required_options.size(), size_t{ 1 });
    EXPECT_TRUE(group.optional_options.empty());
    EXPECT_TRUE(group.flag_options.empty());
    EXPECT_FALSE(group.error_handler.has_value());
    EXPECT_FALSE(group.help_handler.has_value());

    // FAIL
    value = 0;
    {
        auto context = cli::context{}.set_arg(0, nullptr);

        EXPECT_EQ(context.argc, 0);
        EXPECT_EQ(context.argv, nullptr);
        EXPECT_EQ(group.parse(context), cli::parse_codes::missing_path);
        EXPECT_EQ(value, 0);
    }
    value = 0;
    {
        auto args = std::array{ "path/to/program" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 1);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::missing_option);
        EXPECT_EQ(value, 0);
    }
    value = 0;
    {
        auto args = std::array{ "path/to/program", "", };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::invalid_option);
        EXPECT_EQ(value, 0);
    }
    value = 0;
    {
        auto args = std::array{ "path/to/program", "foo bar", };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::invalid_option);
        EXPECT_EQ(value, 0);
    }
    value = 0;
    {
        auto args = std::array{ "path/to/program", "123", "234"};
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 3);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_option);
        EXPECT_EQ(value, 123);
    }

    // OK
    value = 0;
    {
        auto args = std::array{ "path/to/program", "123" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::successful);
        EXPECT_EQ(value, 123);
    }
}

TEST(option_group, single_option_optional)
{
    std::optional<int> value;

    auto group
        = cli::option_group{}
        | cli::option<std::optional<int>>{ value, "--value"};
    
    EXPECT_TRUE(group.required_options.empty());
    EXPECT_EQ(group.optional_options.size(), size_t{ 1 });
    EXPECT_TRUE(group.flag_options.empty());
    EXPECT_FALSE(group.error_handler.has_value());
    EXPECT_FALSE(group.help_handler.has_value());

    // FAIL
    value.reset();
    {
        auto context = cli::context{}.set_arg(0, nullptr);

        EXPECT_EQ(context.argc, 0);
        EXPECT_EQ(context.argv, nullptr);
        EXPECT_EQ(group.parse(context), cli::parse_codes::missing_path);
        EXPECT_FALSE(value.has_value());
    }

    value.reset();
    {
        auto args = std::array{ "path/to/program", "", };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_option);
        EXPECT_FALSE(value.has_value());
    }
    value.reset();
    {
        auto args = std::array{ "path/to/program", "foo bar", };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_option);
        EXPECT_FALSE(value.has_value());
    }
    value.reset();
    {
        auto args = std::array{ "path/to/program", "123", "234" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 3);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_option);
        EXPECT_FALSE(value.has_value());
    }
    value.reset();
    {
        auto args = std::array{ "path/to/program", "--value", "" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 3);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::invalid_option);
        EXPECT_FALSE(value.has_value());
    }
    value.reset();
    {
        auto args = std::array{ "path/to/program", "--value", "foo bar" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 3);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::invalid_option);
        EXPECT_FALSE(value.has_value());
    }

    // OK
    value.reset();
    {
        auto args = std::array{ "path/to/program" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 1);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::successful);
        EXPECT_FALSE(value.has_value());
    }
    value.reset();
    {
        auto args = std::array{ "path/to/program", "--value", "123"};
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 3);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::successful);
        ASSERT_TRUE(value.has_value());
        EXPECT_EQ(value.value(), 123);
    }
}

TEST(default_error, context)
{
    auto group
        = cli::command_group{}
        | cli::command{}
            .set_names({ "yolo" })
            .set_description("Testning 1");

    static_assert(std::is_same_v<decltype(group), cli::command_group>,
        "Expecting type to be cli::command_group");

    EXPECT_EQ(group.commands.size(), size_t{ 1 });
    EXPECT_FALSE(group.error_handler.has_value());
    EXPECT_FALSE(group.help_handler.has_value());

    // FAIL
    {
        auto args = std::array{ "path/to/program", "test" };
        auto context
            = cli::context{}
                .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()))
            | cli::default_error{};

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());

        std::stringstream cerr_stream;
        {
            auto cout_redirect = test::output_redirect{ std::cerr, cerr_stream };
            ASSERT_EQ(group.parse(context), cli::parse_codes::unknown_command);
        }

        std::string expected_error_string = "Unknown command 'test'.\n";

        auto error_string = cerr_stream.str();
        EXPECT_STREQ(error_string.c_str(), expected_error_string.c_str());
    }
    {
        auto args = std::array{ "path/to/program"};
        auto context
            = cli::context{}
             .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()))
            | cli::default_error{};

        EXPECT_EQ(context.argc, 1);
        EXPECT_EQ(context.argv, args.data());

        std::stringstream cerr_stream;
        {
            auto cout_redirect = test::output_redirect{ std::cerr, cerr_stream };
            ASSERT_EQ(group.parse(context), cli::parse_codes::missing_command);
        }

        std::string expected_error_string = "Missing command.\n";

        auto error_string = cerr_stream.str();
        EXPECT_STREQ(error_string.c_str(), expected_error_string.c_str());
    }
}

TEST(default_help, context)
{
    auto group
        = cli::command{}
            .set_names({ "yolo", "swag" })
            .set_description("Testning 1")
        | cli::command{}
            .set_names({ "hello" })
            .set_description("Testning 2")
        | cli::command{}
            .set_names({ "foo", "bar" })
            .set_description("Testning 3");

    static_assert(std::is_same_v<decltype(group), cli::command_group>,
        "Expecting type to be cli::command_group");

    EXPECT_EQ(group.commands.size(), size_t{ 3 });
    EXPECT_FALSE(group.error_handler.has_value());
    EXPECT_FALSE(group.help_handler.has_value());

    auto args = std::array{ "path/to/program", "--help" };
    auto context
        = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()))
        | cli::default_help{};

    EXPECT_EQ(context.argc, 2);
    EXPECT_EQ(context.argv, args.data());

    std::stringstream cout_stream;
    {
        auto cout_redirect = test::output_redirect{ std::cout, cout_stream };
        ASSERT_EQ(group.parse(context), 0);
    }
    
    std::string expected_help_string =
        "Usage: program [command] [command-options]\n\n"
        "Commands:\n"
        "  -h|--help      Show command line help.\n"
        "  yolo|swag      Testning 1\n"
        "  hello          Testning 2\n"
        "  foo|bar        Testning 3\n";

    auto help_string = cout_stream.str();
    EXPECT_STREQ(help_string.c_str(), expected_help_string.c_str());

}