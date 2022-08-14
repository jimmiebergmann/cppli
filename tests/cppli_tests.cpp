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

#include "gtest/gtest.h"
#include "cppli/cppli.hpp"
#include <array>

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
        auto args = std::array{ "path/to/progrma", "test" };
        auto context = cli::context{}.set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_command);
    }
    {
        auto args = std::array{ "path/to/progrma", "", "test" };
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
        auto args = std::array{ "path/to/progrma", "test" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_command);
        EXPECT_FALSE(triggered_callback);
    }
    reset_triggered_callback();
    {
        auto args = std::array{ "path/to/progrma", "" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::missing_command);
        EXPECT_FALSE(triggered_callback);
    }

    // OK
    reset_triggered_callback();
    {
        auto args = std::array{ "path/to/progrma", "yolo" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 123);
        EXPECT_TRUE(triggered_callback);
    }
    reset_triggered_callback();
    {
        auto args = std::array{ "path/to/progrma", "swag" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 123);
        EXPECT_TRUE(triggered_callback);
    }
    reset_triggered_callback();
    {
        auto args = std::array{ "path/to/progrma", "", "swag" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 3);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 123);
        EXPECT_TRUE(triggered_callback);
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
        auto args = std::array{ "path/to/progrma", "test" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::unknown_command);
        EXPECT_TRUE(check_triggered_callbacks(false, false, false));
    }
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/progrma", "" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), cli::parse_codes::missing_command);
        EXPECT_TRUE(check_triggered_callbacks(false, false, false));
    }

    // OK
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/progrma", "yolo" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 100);
        EXPECT_TRUE(check_triggered_callbacks(true, false, false));
    }
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/progrma", "swag" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 100);
        EXPECT_TRUE(check_triggered_callbacks(true, false, false));
    }
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/progrma", "hello" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 200);
        EXPECT_TRUE(check_triggered_callbacks(false, true, false));
    }
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/progrma", "foo" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 2);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 300);
        EXPECT_TRUE(check_triggered_callbacks(false, false, true));
    }
    reset_triggered_callbacks();
    {
        auto args = std::array{ "path/to/progrma", "", "bar" };
        auto context = cli::context{}
            .set_arg(static_cast<int>(args.size()), const_cast<char**>(args.data()));

        EXPECT_EQ(context.argc, 3);
        EXPECT_EQ(context.argv, args.data());
        EXPECT_EQ(group.parse(context), 300);
        EXPECT_TRUE(check_triggered_callbacks(false, false, true));
    }
}