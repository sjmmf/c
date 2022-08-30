#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<errno.h>

#define MSGSIZE 50
#define POSSIBLE '1'  //可能为素数
#define PRECISE '0'   //准确判断

/*此函数用于判断是否是素数*/
void prime(int pipe_read, int pipe_write) {
    char nums[MSGSIZE];
    read(pipe_read, nums, MSGSIZE);
    int value = 0;

    for (int i = 0; i < MSGSIZE; i++) {
        if (nums[i] == POSSIBLE) {
            value = i;
            break;
        }
    }
    if (value == 0) {
        exit(0);
    }

    printf("Prime %d", value);
    nums[value] = PRECISE;

    for (int i = 0; i < MSGSIZE; i++) {
        if (i % value == 0) {
            nums[i] = PRECISE;
        }
    }

    int pid = fork();
    /*子进程*/
    if (pid > 0) {
        write(pipe_read, nums, MSGSIZE);
    }
    /*子进程派生出来的子进程*/
    if (pid == 0) {
        prime(pipe_read, pipe_write);
    }
}

int main(int argc, char* argv[]) {
    int fd[2]; //声明一个管道
    pipe(fd);  //初始化管道

    char nums[MSGSIZE];
    for (int i = 0; i < MSGSIZE; i++) {
        nums[i] = POSSIBLE;    //记为1含义是可能为素数
    }

    int pid = fork();   //调用fork函数分出子进程
    /*父进程*/
    if (pid > 0) {
        nums[0] = PRECISE;
        nums[1] = PRECISE;
        write(fd[1], nums, MSGSIZE);
        wait(0);
    }
    /*子进程*/
    if (pid == 0) {
        prime(fd[0], fd[1]);
        wait(0);
    }
    exit(0);
}