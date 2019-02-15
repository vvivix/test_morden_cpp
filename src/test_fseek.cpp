//
// Created by vvivi on 2018/12/11.
//

#include "gtest/gtest.h"

#include <stdio.h>
#include <stdlib.h>//fseek函数调用
TEST(FileSeekTest, Case1) {

    FILE * fp = fopen("a.txt", "wb");
    if (fp == NULL) {
        printf("open file failed\n");
        exit(1);
    }
    fseek(fp, 2, SEEK_SET);//光标移到文件开始起第二个字节处。
    fwrite("yun", 1, 3, fp); //文件内写入内容yun
    fclose(fp);
}