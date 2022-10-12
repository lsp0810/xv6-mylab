#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


//主要一点，数据并非并行处理的（难以实现）
//而是一级一级对数据进行筛选
int echo_prime(int lastpipe[]);

int main(int argc,char *argv[]){
    
    int the_pipe[2];
    int num[34];        //store number 2-35

    pipe(the_pipe);
    for(int i=2;i<=35;i++)
        num[i-2]=i;
    
    if(fork()!=0){      //main process
        close(the_pipe[0]); //main process only write number
        write(the_pipe[1],num,34*sizeof(int));
        close(the_pipe[1]);
        wait(0);
        exit(0);
    }
    else{       //child process
        echo_prime(the_pipe);   //反复迭代的函数
        //exit(0);
    }
    return(0);

}


int echo_prime(int lastpipe[]){
    //每一次迭代要完成：
    //接受来自上一级process通过pipe传输来的数字
    //找到传输来的基准数字（第一个数字）并进行输出
    //完成本轮数字的质数筛选
    //建立管道与子进程，将筛选的数字传递给子进程
    //final：函数迭代
    int base_num = 0;
    int flag;
    int dd_num[34];
    
    close(lastpipe[1]);
    flag = read(lastpipe[0],&base_num,sizeof(int));

    if(base_num==0 || flag ==0)
        return 0;
    fprintf(1,"prime %d\n",base_num);

    int index=0;
    while(1){
        int temp;
        int new_flag = read(lastpipe[0],&temp,sizeof(int));
        if(new_flag>0){
            if(temp%base_num!=0)
                dd_num[index++]=temp;
        }
        else{
            break;
        }
    }
    close(lastpipe[0]);
    //现在将筛选的数字传输到下一进程
    int new_pipe[2];
    pipe(new_pipe);

    if(fork()!=0){
        close(new_pipe[0]);
        write(new_pipe[1],dd_num,index*sizeof(int));
        close(new_pipe[1]);
        wait(0);
        exit(0);
    }
    else{
        // exit(0);
        echo_prime(new_pipe);
        exit(0);
    }
    return(0);

}
