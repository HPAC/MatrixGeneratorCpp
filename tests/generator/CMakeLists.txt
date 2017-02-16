set(tests random zeros ones)

foreach(test ${tests})
    create_test(test.generator.${test} ${test})
endforeach()
