cmake_minimum_required(VERSION 3.29)
project(Database)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.0.2
)

FetchContent_MakeAvailable(fmt)

# Add executable for the project
add_executable(Database
        main.cpp
        Database.cpp

        # Command files
        commands/CreateTableCommand.cpp
        commands/DropTableCommand.cpp
        commands/AlterTableCommand.cpp
        commands/ShowTablesCommand.cpp
        commands/InsertCommand.cpp
        commands/SaveToFileCommand.cpp
        commands/LoadFromFileCommand.cpp
        commands/SelectCommand.cpp
)

target_link_libraries(Database fmt)