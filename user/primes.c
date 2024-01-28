/*
Write a concurrent version of prime sieve using pipes.
*/
#include "user/user.h"

#define RD 0
#define WR 1

void Primes() {
    int new_fd[2];       // 定义管道描述符
    int prev, next;  // 前一个素数和下一个素数
    // 从管道中读取一个素数
    if (read(0, &prev, sizeof(int))) {
        printf("prime %d\n", prev);
        pipe(new_fd);  // 创建管道
        // 创建子进程
        if (fork() == 0) {
            // 将标准输入重定向到管道的读端
            close(1);
            dup(new_fd[WR]);
            close(new_fd[RD]);
            close(new_fd[WR]);
            // 循环读入数据
            while (read(0, &next, sizeof(int))) {
                // 如果读入的数据不是前一个管道的倍数，就写入管道
                if (next % prev != 0) {
                    write(1, &next, sizeof(int));
                }
            }
        } else {
            wait(0);
            close(0);
            dup(new_fd[RD]);
            close(new_fd[RD]);
            close(new_fd[WR]);
            Primes();
        }
    }
}

int main(int argc, char *argv[]) {
    int fd[2];  // 定义管道描述符
    pipe(fd);   // 创建管道

    // 创建子进程
    if (fork() == 0) {
        close(1);
        dup(fd[WR]);
        close(fd[RD]);
        close(fd[WR]);
        for (int i = 2; i < 36; i++) {
            write(1, &i, sizeof(int));
        }
    } else {
        wait(0);
        close(0);
        dup(fd[RD]);
        close(fd[RD]);
        close(fd[WR]);
        Primes();
    }

    exit(0);
}