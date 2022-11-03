add_rules("mode.debug", "mode.release")

add_requires("chipmunk2d", "entt", "fmt", "libsdl", "libsdl_image", "lz4", "nlohmann_json")
add_requires("imgui", { configs = { sdl2 = true }})

set_allowedarchs("windows|x64")
set_warnings("allextra")

set_rundir("bin") -- Le dossier courant lors de l'exécution des binaires (depuis VS) - c'est depuis ce dossier que les chemins commencent
set_targetdir("bin/$(plat)_$(arch)_$(mode)") -- Le dossier de sortie des binaires, les $(X) sont remplacés par les valeurs existantes (plat = windows, arch = x64 et mode = debug)

set_languages("c++17")

-- Désactivation de quelques warnings pas utiles dans notre cas avec VS
if is_plat("windows") then
    set_runtimes("MD")
    add_cxflags("/wd4251") -- Disable warning: class needs to have dll-interface to be used by clients of class blah blah blah
    add_cxflags("/wd4275") -- Disable warning: DLL-interface class 'class_1' used as base for DLL-interface blah
end

target("A4Engine")
    set_kind("shared")
    add_defines("A4ENGINE_BUILD")
    add_headerfiles("include/A4Engine/*.h", "include/A4Engine/*.hpp", "include/A4Engine/*.inl")
    add_includedirs("include", { public = true })
    add_files("src/A4Engine/**.cpp")
    add_packages("libsdl", "libsdl_image", "nlohmann_json", "fmt", "entt", "imgui", "chipmunk2d", { public = true })
    add_packages("lz4")

target("A4Game")
    set_kind("binary")
    add_deps("A4Engine")
    add_headerfiles("include/A4Game/*.h", "include/A4Game/*.hpp")
    add_files("src/A4Game/**.cpp")

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

