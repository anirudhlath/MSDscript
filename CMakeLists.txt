cmake_minimum_required(VERSION 3.21)
project(msdscript)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(
        catch.hpp
        Expr.cpp
        Expr.h
        Parser.cpp
        Parser.h
        Val.cpp
        Val.h
        main.cpp pointer.h)
