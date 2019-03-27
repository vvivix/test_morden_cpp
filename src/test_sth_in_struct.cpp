//
// Created by vvivi on 2019/2/21.
//


#include <stdio.h>
#include <gtest/gtest.h>

struct str{
    int len;
    char s[0];
};

struct foo {
    struct str *a;
};

TEST(STH_STRANGE, CASE_1) {
    struct foo f={0};
    if (f.a->s) {
        printf( f.a->s);
    }
}