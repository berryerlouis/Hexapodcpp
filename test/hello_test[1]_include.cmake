if(EXISTS "/mnt/c/Users/SESA741930/Documents/perso/Hexapodcpp/test/hello_test[1]_tests.cmake")
  include("/mnt/c/Users/SESA741930/Documents/perso/Hexapodcpp/test/hello_test[1]_tests.cmake")
else()
  add_test(hello_test_NOT_BUILT hello_test_NOT_BUILT)
endif()