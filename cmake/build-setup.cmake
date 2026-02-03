set(CMAKE_CXX_STANDARD "20" CACHE STRING "")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "")
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)

if(MSVC)
    # MSVC specific flags
    set(COMPILER_FLAGS
        /W4          # Serious warnings
        /WX          # Warnings as errors
        /permissive- # Standards conformance
        /sdl         # Security Development Lifecycle checks
    )
else()
    # General Clang & GCC flags
    set(COMPILER_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wunused
        -Wcast-align
        -Wconversion
        -Wsign-conversion
        -Wdouble-promotion
        -Wold-style-cast
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Wimplicit-fallthrough
        -Wformat=2
        -Wmissing-variable-declarations
    )

    # Clang specific refinements
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        list(APPEND COMPILER_FLAGS 
            -Wshadow-all 
            -Wreserved-identifier
        )
    # GCC specific refinements
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        list(APPEND COMPILER_FLAGS 
            -Wshadow 
            -Wlogical-op 
            -Wduplicated-cond 
            -Wduplicated-branches
            -Wmissing-declarations # GCC equivalent for missing-variable-declarations
        )
    endif()
endif()

# Convert list to string for CMAKE_CXX_FLAGS
string(REPLACE ";" " " COMPILER_FLAGS_STR "${COMPILER_FLAGS}")

# Apply globally to the cache
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS_STR}" CACHE STRING "" FORCE)