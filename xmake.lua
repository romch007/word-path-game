set_project("word-path-game")

set_rules("mode.debug", "mode.release", "mode.valgrind")

set_allowedmodes("debug", "release", "valgrind")
set_defaultmode("debug")

add_languages("cxx17")

add_requires("cxxopts")

set_warnings("allextra")

target("word-path-game")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs("include")
    add_headerfiles("include/*.hpp")
    add_packages("cxxopts")
