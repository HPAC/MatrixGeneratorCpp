set(tests generator)

function(create_test test source)

    add_executable(${test} ${source}.cpp)
    if(GTEST_DEPENDENCY)
        add_dependencies(${test} ${GTEST_DEPENDENCY})
    endif(GTEST_DEPENDENCY)

    target_link_libraries(${test} ${LINK_LIBRARIES})
    target_link_libraries(${test} ${GTEST_LINK_FLAGS})

    add_test(NAME ${test} COMMAND ${test})

endfunction(create_test)

foreach(test_dir ${tests})
    add_subdirectory(${test_dir})
endforeach()
