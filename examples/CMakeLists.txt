
set(examples benchmarker generator)

foreach(example ${examples})
    add_executable(${example} ${example}.cpp)
    target_link_libraries(${example} PRIVATE libtests)
endforeach()
