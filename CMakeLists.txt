cmake_minimum_required(VERSION 3.31)
project(miniShell)

set(CMAKE_CXX_STANDARD 20)

# Use pkg-config to locate Readline
find_package(PkgConfig REQUIRED)
pkg_check_modules(READLINE REQUIRED readline)

# Define the executable and its sources
add_executable(miniShell
    main.cpp
    ScopedFdRedirect.cpp
    ScopedFdRedirect.h
        Globbing.cpp
        Tokenize.cpp
        Run.cpp
)

# Include Readline headers
target_include_directories(miniShell PRIVATE ${READLINE_INCLUDE_DIRS})

# Link against Readline and its dependencies
target_link_libraries(miniShell PRIVATE ${READLINE_LIBRARIES})

# If linking fails, you may need to link against ncurses as well:
# target_link_libraries(miniShell PRIVATE ${READLINE_LIBRARIES} ncurses)
