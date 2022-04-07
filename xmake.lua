set_project("word-path-game")

set_rules("mode.debug", "mode.release", "mode.valgrind")

set_allowedmodes("debug", "release", "valgrind")
set_defaultmode("debug")

add_requires("cxxopts")

add_includedirs("include")

add_languages("cxx17")
set_warnings("allextra")

target("word-path-game")
    set_group("word-path-game")
    set_kind("binary")
    add_files("src/*.cpp")
    add_headerfiles("include/*.hpp")
    add_packages("cxxopts")

option("tests")
  set_default(false)
  set_showmenu(true)
  set_description("Build unit tests")
option_end()

if has_config("tests") then
    add_requires("catch2")

    target("word-path-game-tests")
        add_deps("word-path-game")
        add_files("tests/*.cpp")
        add_packages("catch2")
        set_group("Tests")
        set_kind("binary")
        add_headerfiles("include/*.hpp")
end
