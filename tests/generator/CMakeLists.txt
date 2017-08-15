set(tests random constant eye)

foreach(test ${tests})
    create_test(test.generator.${test} ${test})
endforeach()
