#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//xarg作用：将标准输出作为额外的参数输入到xarg命令中
//例如：command1的输出为：output1
//那么：command1 | xargs command input1 input2等价于：
//command input1 input2 output1

int main(int argc,char* argv[]){
    char *execbuffer[32];       //送入exec指令的buffer指针


    //先将xargs的入参送入execbuffer：
    for(int i=0;i<argc-1;i++)
        execbuffer[i] = argv[i+1];
    
    char buffer[320];       //存放标准输出的字符串
    char tempbuffer[320];   //实际处理的char数组
    char *buffer_pointer = tempbuffer;

    int buffer_size = 0;
    int tempbuffer_index = 0;
    int execbuffer_index = argc - 1;
    while((buffer_size = read(0,buffer,sizeof(buffer)))>0){     //一次读一行的输出
        for(int i=0;i<buffer_size;i++){
            char temp = buffer[i];
            //读到空格，即一个参数输入完毕，需要：
            //将操作的char数组空格变为\0表示
            if(temp == ' '){            
                tempbuffer[tempbuffer_index++] = '\0';
                execbuffer[execbuffer_index++] = buffer_pointer;
                buffer_pointer = tempbuffer + tempbuffer_index;
            }
            //读到这一行的末尾，需要：
            else if(temp == '\n'){
                tempbuffer[tempbuffer_index++] = '\0';
                execbuffer[execbuffer_index++] = buffer_pointer;
                execbuffer[execbuffer_index] = 0;
                if(fork()==0){  //创建子进程运行exec
                    exec(argv[1],execbuffer);
                }
                else{
                    wait(0);
                }
                //先要把execbuffer数组的额外参数几个地址赋值为0
                for(int i = argc;i<=execbuffer_index;i++)
                    execbuffer[i] = 0;
                //后续需要将额外参数个数以及指针重置
                execbuffer_index = argc - 1;
                tempbuffer_index = 0;
                buffer_pointer = tempbuffer;
                
            }
            else{
                tempbuffer[tempbuffer_index++] = temp;
            }
        }
    }
    exit(0);
}