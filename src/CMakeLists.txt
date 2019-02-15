file(GLOB SRC MonsterWorld/* *)

add_executable(${PROJECT_NAME}
        ${SRC}
        test_cpp11_unique_ptr.cpp
        test_VariadicTemplate.cpp
        test_KeyStoreTagsUtils.cpp
        test_fseek.cpp)

target_link_libraries(${PROJECT_NAME} gtest gtest_main)