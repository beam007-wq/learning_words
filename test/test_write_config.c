#include "../src/common.h"
#include <stdio.h>

bool change(const char *paramater_name, const void *value, const char type) {
    // value 传入时就已经是指向数据的内存地址了，直接传给 config_write
    if (!config_write(paramater_name, value, type)) {
        return false;
    }
    return true;
}

int main() {
    const char *paramater_names[4] = {"background_color", "language", "learn_words_number", "review_words_number"};
    const char types[4] = {'s', 's', 'n', 'n'};
    const char *content_value[2] = {"red", "zh_CN"};
    int content_value_index = 0;

    const int number[2] = {15, 20};
    int number_index = 0;

    for (int i = 0; i < 4; i++) {
        if (types[i] == 's') {
            if (!change(paramater_names[i], content_value[content_value_index], types[i])) {
                puts("error");
                return 1;
            }
            content_value_index++;
        } else {
            // 重点修正：给 number[number_index] 加上 & 取地址符
            if (!change(paramater_names[i], &number[number_index], types[i])) {
                puts("error");
                return 1;
            }
            number_index++;
        }
    }

    return 0;
}
