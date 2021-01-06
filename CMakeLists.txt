cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code Basic/Basic.cpp
        Basic/exp.cpp
        Basic/program.cpp
        Basic/parser.cpp
        Basic/statement.cpp
        Basic/evalstate.cpp
        StanfordCPPLib/tokenscanner.cpp
        StanfordCPPLib/simpio.cpp
        StanfordCPPLib/strlib.cpp
        StanfordCPPLib/error.cpp
        #[[Test/score.cc]]
        )
