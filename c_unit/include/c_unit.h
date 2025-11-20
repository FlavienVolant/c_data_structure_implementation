#ifndef C_UNITS_H
#define C_UNITS_H

#define ADD_TEST(list, func) append_test(list, func, #func)

#define ASSERT_TRUE(boolean) if (!(boolean)) {return -1;}
#define ASSERT_NOT_NULL(ptr) if ((ptr) == NULL) {return -1;}
#define ASSERT_EQUALS(a, b) if ((a) != (b)) {return -1;}

typedef void*(_beforeEach()); // return the struct of params as void*
typedef int(testFunction(void *params)); // params are given by _beforeEach, return 0 == success; other == fail
typedef void(_afterEach(void *params)); // used to free the params allocated in _beforeEach

struct TestList {
    struct TestNode *head;
    int length;
};

struct TestNode {
    testFunction *f;
    char *testName;
    struct TestNode *next;
};

struct TestList* create_test_list();

void append_test(struct TestList *list, testFunction f, char *testName);

int run(testFunction f, _beforeEach before, _afterEach after);
void run_tests(struct TestList *list, _beforeEach before, _afterEach after);

void clear_tests(struct TestList *list);
void free_test_list(struct TestList *list);

#endif // C_UNITS_H