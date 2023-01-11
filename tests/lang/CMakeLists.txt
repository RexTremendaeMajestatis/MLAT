add_executable(lang_test test.cpp)

target_link_libraries(lang_test
PRIVATE
  GTest::GTest
  lang)