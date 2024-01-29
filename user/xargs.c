/*
Write a simple version of the UNIX xargs program: read lines from the standard
input and run a command for each line, supplying the line as arguments to the
command.
*/
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char *args[MAXARG];  // 存放 xargs 命令的参数
    char buf[512], temp[512];
    int n, tempIndex = 0, argIndex = argc - 1;
    if (argc < 2) {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }
    // 忽略掉 xargs 本身
    for (int i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }
    args[argc - 1] = temp;
    args[argc] = 0; // 设置 NULL 作为结尾
    // 从标准输入读取一行，作为 xargs 命令的参数
    while ((n = read(0, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < strlen(buf); i++) {
            // 碰到换行符，就 fork 一个子进程执行 xargs 命令
            if (buf[i] == '\n') {
                // 碰到换行符了，结束当前行
                temp[tempIndex] = '\0';
                // 检查参数是否过多
                if (argIndex < MAXARG - 1) {
                    args[argIndex++] = temp;
                } else {
                    fprintf(2, "Xargs: Too many arguments\n");
                    exit(1);
                }

                if (fork() == 0) {
                    if (exec(argv[1], args) == -1) {
                        fprintf(2, "exec failed\n");
                        exit(1);
                    }
                } else {
                    wait(0);
                }

                // 重置 temp 和 argIndex 为下一行准备
                tempIndex = 0;
                argIndex = argc - 1;
                args[argIndex] = temp;

            } else {
                temp[tempIndex++] = buf[i];
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