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

#ifndef CPPLI_TESTS_HPP
#define CPPLI_TESTS_HPP

#include "gtest/gtest.h"
#include "cppli/cppli.hpp"
#include <array>
#include <streambuf>
#include <sstream>

namespace test {

    

    class cout_redirect {
    private:

        struct null_streambuf : public std::streambuf
        {
            using int_type = std::streambuf::int_type;
            using traits = std::streambuf::traits_type;

            virtual int_type overflow(int_type value) override {
                return value;
            }
        };

        std::streambuf* m_original_cout_streambuf;
        bool m_delete_rdbuf;

    public:
        [[nodiscard]] cout_redirect() :
            m_original_cout_streambuf(std::cout.rdbuf(new null_streambuf())),
            m_delete_rdbuf(true)
        {}

        [[nodiscard]] explicit cout_redirect(std::stringstream& p_output) :
            m_original_cout_streambuf(std::cout.rdbuf(p_output.rdbuf())),
            m_delete_rdbuf(false)
        {}
        

        ~cout_redirect() {
            auto* ret = std::cout.rdbuf(m_original_cout_streambuf);
            if (m_delete_rdbuf) {
                delete ret;
            }
        }

        cout_redirect(const cout_redirect&) = delete;
        cout_redirect(cout_redirect&&) = delete;
        cout_redirect& operator = (const cout_redirect&) = delete;
        cout_redirect& operator = (cout_redirect&&) = delete;

    };

}

#endif