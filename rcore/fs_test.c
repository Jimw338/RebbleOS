/* fs_test.c
 * Tests for RebbleOS filesystem
 * RebbleOS
 */

#include "fs.h"
#include "test.h"
#include "debug.h"
#include <string.h>

TEST(fs_creat_basic) {
    struct fd fd, *fdp;
    struct file file;
    int rv;
    
    fdp = fs_creat(&fd, "testfile", 16384);
    if (!fdp) {
        *artifact = 1;
        return TEST_FAIL;
    }
    
    fs_mark_written(fdp);
    
    rv = fs_find_file(&file, "testfile");
    if (rv < 0) {
        *artifact = 2;
        return TEST_FAIL;
    }
    
    *artifact = 0;
    return TEST_PASS;
}

TEST(fs_find_noent) {
    struct file file;
    int rv = fs_find_file(&file, "noent");
    *artifact = rv;
    return (rv < 0) ? TEST_PASS : TEST_FAIL;
}

TEST(fs_two_files) {
    struct fd fd1, fd2, *fdp;
    struct file file1, file2;
    int rv = TEST_PASS;
    
    *artifact = 0;
    
    fdp = fs_creat(&fd1, "hello", 5);
    if (!fdp) { *artifact = 1; return TEST_FAIL; }
    fs_write(&fd1, "Hello", 5);
    
    fdp = fs_creat(&fd2, "world", 5);
    if (!fdp) { *artifact = 2; return TEST_FAIL; }
    fs_write(&fd2, "World", 5);
    
    char buf[5];

    rv = fs_find_file(&file1, "world");
    if (rv < 0) { *artifact = 3; return TEST_FAIL; }
    fs_open(&fd1, &file1);
    rv = fs_read(&fd1, buf, 5);
    if (rv < 5) { *artifact = 4; return TEST_FAIL; }
    if (memcmp(buf, "World", 5)) { *artifact = 5; return TEST_FAIL; }

    rv = fs_find_file(&file1, "hello");
    if (rv < 0) { *artifact = 6; return TEST_FAIL; }
    fs_open(&fd1, &file1);
    rv = fs_read(&fd1, buf, 5);
    if (rv < 5) { *artifact = 7; return TEST_FAIL; }
    if (memcmp(buf, "Hello", 5)) { *artifact = 8; return TEST_FAIL; }
    
    *artifact = 0;
    return TEST_PASS;
}