#include <stdio.h>
#include <string.h>
#include <assert.h>

#define IMPLEMENT_STRING
#include "../cstring.h"

// Test case 1
void cstring_testcase1() {
    char *test_string1 = "This is a test.";
    char *desired_split1[] = {"This", "is", "a", "test."};
    char *delimiter_join1 = " ";
    char *desired_joined1 = "This is a test.";
    size_t len = sizeof(desired_split1) / sizeof(desired_split1[0]);
    char *delimiter1 = " ";
    String **splited1 = str_split(test_string1, delimiter1);
    char *joined1 = str_join((const char **)splited1, delimiter_join1, len);

    assert(vector_length(splited1) == len);
    for (size_t i = 0; i < len; i++) {
        assert(strcmp(splited1[i], desired_split1[i]) == 0);
        free_string(splited1[i]);
    }
    free_string(joined1);
    free_string(splited1);
}

// Test case 2
void cstring_testcase2() {
    char *test_string2 = "Split,me,by,comma";
    char *desired_split2[] = {"Split", "me", "by", "comma"};
    char *delimiter_join2 = ",";
    char *desired_joined2 = "Split,me,by,comma";
    size_t len = sizeof(desired_split2) / sizeof(desired_split2[0]);
    char *delimiter2 = ",";
    String **splited2 = str_split(test_string2, delimiter2);
    char *joined2 = str_join((const char **)splited2, delimiter_join2, len);

    assert(vector_length(splited2) == len);
    for (size_t i = 0; i < len; i++) {
        assert(strcmp(splited2[i], desired_split2[i]) == 0);
        free_string(splited2[i]);
    }
    free_string(joined2);
    free_string(splited2);
}

// Test case 3
void cstring_testcase3() {
    char *test_string3 = "One-word";
    char *desired_split3[] = {"One","word"};
    char *delimiter_join3 = "-";
    char *desired_joined3 = "One-word";
    size_t len = sizeof(desired_split3) / sizeof(desired_split3[0]);
    char *delimiter3 = "-";
    String **splited3 = str_split(test_string3, delimiter3);
    char *joined3 = str_join((const char **)splited3, delimiter_join3, len);

    assert(vector_length(splited3) == len);
    for (size_t i = 0; i < len; i++) {
        assert(strcmp(splited3[i], desired_split3[i]) == 0);
        free_string(splited3[i]);
    }
    free_string(joined3);
    free_string(splited3);
}

// Test case 4
void cstring_testcase4() {
    char *test_string4 = "Multiple      spaces";
    char *desired_split4[] = {"Multiple", "", "", "", "", "", "spaces"};
    char *delimiter_join4 = " ";
    char *desired_joined4 = "Multiple      spaces";
    size_t len = sizeof(desired_split4) / sizeof(desired_split4[0]);
    char *delimiter4 = " ";
    String **splited4 = str_split(test_string4, delimiter4);
    char *joined4 = str_join((const char **)splited4, delimiter_join4, len);

    assert(vector_length(splited4) == len);
    for (size_t i = 0; i < len; i++) {
        assert(strcmp(splited4[i], desired_split4[i]) == 0);
        free_string(splited4[i]);
    }
    free_string(joined4);
    free_string(splited4);
}

// Test case 5
void cstring_testcase5() {
    char *test_string5 = "";
    char *desired_split5[] = {""};
    char *delimiter_join5 = " ";
    char *desired_joined5 = "";
    size_t len = sizeof(desired_split5) / sizeof(desired_split5[0]);
    char *delimiter5 = " ";
    String **splited5 = str_split(test_string5, delimiter5);
    char *joined5 = str_join((const char **)splited5, delimiter_join5, len);

    assert(vector_length(splited5) == len);
    for (size_t i = 0; i < len; i++) {
        assert(strcmp(splited5[i], desired_split5[i]) == 0);
        free_string(splited5[i]);
    }
    free_string(joined5);
    free_string(splited5);
}
void test_cstring(){
    cstring_testcase1();
    cstring_testcase2();
    cstring_testcase3();
    cstring_testcase4();
    cstring_testcase5();

}


int main(){
    test_cstring();
    return 0;
}
