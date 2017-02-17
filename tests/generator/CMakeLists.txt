set(tests random zeros ones eye)

foreach(test ${tests})
    create_test(test.generator.${test} ${test})
endforeach()
