set(GLOB_SOURCE_FILES *.cpp
                      *.c
                      *.cu
    CACHE INTERNAL "File endings to glob for source files"
)

set(GLOB_HEADER_FILES *.h
                      *.hpp
                      *.cpph
    CACHE INTERNAL "File endings to glob for header files"
)

file(GLOB_RECURSE allFiles CONFIGURE_DEPENDS "*") # Find all files
file(GLOB_RECURSE sourceFiles CONFIGURE_DEPENDS ${GLOB_SOURCE_FILES}) # Find all source files
file(GLOB_RECURSE headerFiles CONFIGURE_DEPENDS ${GLOB_HEADER_FILES}) # Find all header files

list_minus(otherFiles LIST1 ${allFiles} LIST2 ${sourceFiles})
list_minus(otherFiles LIST1 ${otherFiles} LIST2 ${headerFiles})
set_source_files_properties(${headerFiles} PROPERTIES HEADER_FILE_ONLY ON)
set_source_files_properties(${otherFiles} PROPERTIES HEADER_FILE_ONLY ON)

group_files("Source Files" FILES ${sourceFiles})
group_files("Header Files" FILES ${headerFiles})
group_files("Other Files" FILES ${otherFiles})

list(FILTER sourceFiles EXCLUDE REGEX ".*Main\\.cpp$")

add_library(nsObj OBJECT ${sourceFiles} ${headerFiles} ${otherFiles})

set(MSVC_COMPILER_FLAGS
    $<$<COMPILE_LANGUAGE:CXX>:/MP> # Use as many cores as available for compilation.
    $<$<COMPILE_LANGUAGE:CXX>:/W4> #/Wall # Baseline reasonable warnings.
    $<$<COMPILE_LANGUAGE:CXX>:/permissive-> # Standards conformance mode for MSVC compiler.
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:Release>:/Oi>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:Release>:/O2>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:Release>:/Ot>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:Release>:/GL>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:Release>:/GF>>
)

set(CLANG_COMPILER_FLAGS
    -g
    -W
    -Wall
    -Wextra # Reasonable and standard.
    -Wshadow # Warn the user if a variable declaration shadows one from a parent context.
    -Wnon-virtual-dtor # Warn the user if a class with virtual functions has a non-virtual destructor. This helps catch hard to track down memory errors.
    -Wcast-align # Warn for potential performance problem casts.
    -Wunused # Warn on anything being unused.
    -Wunused-function
    -Wunused-variable
    -Wunused-parameter
    -Woverloaded-virtual # Warn if you overload (not override) a virtual function.
    $<$<CONFIG:Debug>:-O0;-ggdb;-fno-elide-constructors>
)

set(GCC_COMPILER_FLAGS
    ${CLANG_COMPILER_FLAGS}
    -Wmisleading-indentation # Warn if indentation implies blocks where blocks do not exist.
    -Wduplicated-cond # Warn if if/else chain has duplicated conditions.
    -Wduplicated-branches # Warn if if/else branches have duplicated code.
    -Wlogical-op # Warn about logical operations being used where bitwise were probably wanted.
)

if(MSVC)
    set(COMPILER_FLAGS ${MSVC_COMPILER_FLAGS})
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(COMPILER_FLAGS ${CLANG_COMPILER_FLAGS})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_FLAGS ${GCC_COMPILER_FLAGS})
else()
    message(AUTHOR_WARNING "No compiler flags set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif()

target_compile_options(nsObj PUBLIC ${COMPILER_FLAGS})

target_link_libraries(nsObj PUBLIC
    tinyxml2
    MPI::MPI_CXX
    ${PETSc_LIBRARIES}
)

target_include_directories(nsObj PUBLIC
    ${CMAKE_SOURCE_DIR}/Source
    ${PETSc_INCLUDE_DIRS}
)

set_target_properties(nsObj PROPERTIES
    DEBUG_POSTFIX "d"
)

add_executable(ns Main.cpp)

target_link_libraries(ns PRIVATE
    nsObj
)

set_target_properties(ns PROPERTIES
    DEBUG_POSTFIX "d"
)
