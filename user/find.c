/*
Write a simple version of the UNIX find program: find all the files in
a directory tree with a specific name.
*/
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // find 的第一个参数必须是目录
    if (st.type != T_DIR) {
        fprintf(2, "find: %s is not a directory\n", path);
        return;
    }

    // 路径过长
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        printf("find: path too long\n");
        return;
    }

    // 把路径拷贝到 buf 中
    strcpy(buf, path);
    p = buf + strlen(buf);  // p 指向 buf 的末尾
    *p++ = '/';             // 在 buf 的末尾添加一个 '/'

    // 循环从 fd 中读取目录项
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) {
            continue;
        }
        // 不要进入 . 和 .. 目录递归
        if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) {
            continue;
        }
        // 把目录项的名字拷贝到 buf 的末尾(也就是 p)
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;  // 末尾添加一个 '\0' 作为结束符

        if (stat(buf, &st) < 0) {
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }
        // 如果是目录类型的文件，进去进行递归查找
        if (st.type == T_DIR) {
            find(buf, filename);
        } else if (st.type == T_FILE && !strcmp(de.name, filename)) {
            // 查找到了同名文件
            printf("%s\n", buf);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find <directory> <filename>\n");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}