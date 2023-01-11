add_library(lang object.cpp parser.cpp tokenizer.cpp interpretation.cpp)
target_include_directories(lang PUBLIC include)