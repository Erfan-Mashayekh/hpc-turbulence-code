file(GLOB_RECURSE sourceFiles CONFIGURE_DEPENDS ${GLOB_SOURCE_FILES}) # Find all source files

foreach(file ${sourceFiles})
    get_filename_component(filename ${file} NAME_WLE)
    add_executable(${filename} ${file})
    add_test(NAME ${filename} COMMAND ${filename})

    target_link_libraries(${filename} PRIVATE
        nsObj
    )

    set_target_properties(${filename} PROPERTIES
        DEBUG_POSTFIX "d"
        FOLDER "Tests"
    )
endforeach()

add_test(NAME Cavity2DTest
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 1 $<TARGET_FILE:ns> ${CMAKE_SOURCE_DIR}/ExampleCases/Cavity2D.xml
)

add_test(NAME Cavity3DTest
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 1 $<TARGET_FILE:ns> ${CMAKE_SOURCE_DIR}/ExampleCases/Cavity3D.xml
)

add_test(NAME Channel2DTest
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 1 $<TARGET_FILE:ns> ${CMAKE_SOURCE_DIR}/ExampleCases/Channel2D.xml
)

add_test(NAME Channel3DTest
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 1 $<TARGET_FILE:ns> ${CMAKE_SOURCE_DIR}/ExampleCases/Channel3D.xml
)

add_test(NAME ChannelBackwardFacingStep2DTest
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 1 $<TARGET_FILE:ns> ${CMAKE_SOURCE_DIR}/ExampleCases/ChannelBackwardFacingStep2D.xml
)

add_test(NAME ChannelBackwardFacingStep3DTest
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 1 $<TARGET_FILE:ns> ${CMAKE_SOURCE_DIR}/ExampleCases/ChannelBackwardFacingStep3D.xml
)
