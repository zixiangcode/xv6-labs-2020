/*
Write a simple version of the UNIX xargs program: read lines from the standard
input and run a command for each line, supplying the line as arguments to the
command.
*/
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char *args[MAXARG];  // 存放 xargs 命令的参数
    char buf[512];
    int n;
    if (argc < 2) {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }
    // 忽略掉 xargs 本身
    for (int i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }
    // 从标准输入读取一行，作为 xargs 命令的参数
    while ((n = read(0, buf, sizeof(buf))) > 0) {
        char temp[512] = {"\0"};
        args[argc - 1] = temp;
        for (int i = 0; i < strlen(buf); i++) {
            // 碰到换行符，就 fork 一个子进程执行 xargs 命令
            if (buf[i] == '\n') {
                if (fork() == 0) {
                    if (exec(argv[1], args) == -1) {
                        fprintf(2, "exec failed\n");
                        exit(1);
                    }
                } else {
                    wait(0);
                }
            } else {
                temp[i] = buf[i];
            }
        }
    }
    // read 函数失败 
    if (n == -1) {
        fprintf(2, "read failed\n");
        exit(1);
    }

    exit(0);
}