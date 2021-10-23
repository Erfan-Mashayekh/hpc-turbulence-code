include(CMakeParseArguments)

function(list_minus resultOut)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs LIST1 LIST2)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(result ${ARG_LIST1})
    foreach(secondEntry ${ARG_LIST2})
        list(REMOVE_ITEM result ${secondEntry})
    endforeach()

    set(${resultOut} ${result} PARENT_SCOPE)
endfunction()

function(group_files group)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs FILES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    foreach(filename ${ARG_FILES})
       file(RELATIVE_PATH rel ${CMAKE_CURRENT_SOURCE_DIR} ${filename})
       if(rel MATCHES ".*/.*")
          # Subdirectory found -> add to subdirectory
          get_filename_component(dir ${rel} PATH)
          # Replace slashes by some underscores to use path as variable name
          # -> you are allowed to use at max two underscores in your folder name
          string(REGEX REPLACE "/" "___" subname ${dir})
          list(APPEND subfiles${subname} ${filename})
          list(APPEND subnames ${subname})
       else()
          # No subdirectory found -> add to normal group
          list(APPEND files ${filename})
       endif()
    endforeach()

    if(subnames)
       list(REMOVE_DUPLICATES subnames)
    endif()

    foreach(subname ${subnames})
       string(REGEX REPLACE  "___" "\\\\" subgroup ${subname})
       #message(STATUS "Group: ${group}\\${subgroup} and files: ${subfiles${subname}}")
       source_group("${group}\\${subgroup}" FILES ${subfiles${subname}})
    endforeach()
    #message(STATUS " Group: ${group} and files: ${files}")
    source_group("${group}" FILES ${files})
endfunction()
