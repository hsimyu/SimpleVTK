#ifndef TEST_SIMPLE_VTK_HPP_INCLUDED
#define TEST_SIMPLE_VTK_HPP_INCLUDED

#include <simple_vtk.hpp>
#include <gtest/gtest.h>

namespace TEST_SIMPLE_VTK {
    class Test1 : public ::testing::Test {
        protected:
            Test1();
            ~Test1();

            SimpleVTK gen;
            const std::string header = R"(<?xml version="1.0" ?>
)";
            const std::string indent = "    ";

            void compareCurrentContentAndTarget(std::string target) {
                std::cout << "----- current content -----" << std::endl;
                std::cout << gen.getRawString() << std::endl;
                std::cout << "----- target content -----" << std::endl;
                std::cout << target << std::endl;
            }
    };
}

#endif