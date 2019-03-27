file(GLOB SRC MonsterWorld/* *)
file(GLOB LKS linux_kernel_snippet/* *)

add_executable(${PROJECT_NAME}
        ${SRC}
        ${LKS}
        test_cpp11_unique_ptr.cpp
        test_VariadicTemplate.cpp
        test_KeyStoreTagsUtils.cpp
        test_fseek.cpp linux_kernel_snippet/test_bootmem.cpp test_sth_in_struct.cpp test_delete_obj_array.cpp linux_kernel_snippet/test_mm_build_zonelists.cpp linux_kernel_snippet/test_free_page.cpp test_condition_variable.cpp test_cpp11_codecvt.cpp design_pattern/Observer.cpp)

target_link_libraries(${PROJECT_NAME} gtest gtest_main)