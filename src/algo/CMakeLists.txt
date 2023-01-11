add_library(algo CNF.cpp)
target_include_directories(algo PUBLIC include)
target_link_libraries(algo PRIVATE lang)