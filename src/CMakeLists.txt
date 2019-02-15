file(GLOB SRC MonsterWorld/* *)
#set(SRC
#        MonsterWorld/Monster.cpp MonsterWorld/Monster.h
#        MonsterWorld/Zombie.cpp MonsterWorld/Zombie.h
#        MonsterWorld/Orge.cpp MonsterWorld/Orge.h
#
#        test_builder_parttern.cpp
#        test_cpp17_feature.cpp
#        test_cpp11_feature.cpp
#        test_MonsterWorld.cpp
#        test_cpp14_monad.cpp
#)

add_executable(test_cpp11_app ${SRC} test_cpp11_unique_ptr.cpp test_VariadicTemplate.cpp test_KeyStoreTagsUtils.cpp test_fseek.cpp)

target_link_libraries(test_cpp11_app gtest gtest_main)