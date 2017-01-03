set(tests random)

foreach(test ${tests})
    create_test(test.generator.${test} ${test})
endforeach()
