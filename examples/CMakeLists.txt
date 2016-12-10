
set(examples benchmarker)

foreach(example ${examples})
    add_executable(${example} ${example}.cpp)
    target_link_libraries(${example} ${LINK_LIBRARIES})
endforeach()