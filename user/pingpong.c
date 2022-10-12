#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[]){
    int p1[2],p2[2];
    pipe(p1);       //pipe direction:parent--->child
    pipe(p2);       //pipe direction:child--->parent
    char temp;
    /*
    if(fork()==0){      //current process is child
        int child_pid = getpid();
        //write a bite to the pipe2
        write(p2[1],"y",1);
        //read a bite from the pipe1
        int flag;
        flag = read(p1[0],&temp,1);
        if(flag <=0){
            fprintf(2,"child read from pipe error");
            exit(1);
        }
        else
            fprintf(1,"%d:received ping\n",child_pid);
    }
    else{               //current process is parent
        int parent_pid = getpid();
        //write a bite to the pipe1
        write(p1[1],"y",1);
        //read a bite from the pipe1
        int flag;
        flag = read(p2[0],&temp,1);
        if(flag <=0){
            fprintf(2,"parent read from pipe error");
            exit(1);
        }
        else
            fprintf(1,"%d:received pong\n",parent_pid);
    }*/
    //hard... pipe in a process should be single direction?
    if(fork()==0){      //current process is child
        int child_pid = getpid();
        //close
        close(p2[0]);
        close(p1[1]);
        //read a bite from the pipe1
        int flag;
        flag = read(p1[0],&temp,1);
        close(p1[0]);
        if(flag <=0){
            fprintf(2,"child read from pipe error");
            exit(1);
        }
        else
            fprintf(1,"%d: received ping\n",child_pid);

        //write a bite to the pipe2
        write(p2[1],"y",1);
        close(p2[1]);
    }
    else{               //current process is parent
        int parent_pid = getpid();
        //close
        close(p2[1]);
        close(p1[0]);
        //write a bite to the pipe1
        write(p1[1],"y",1);
        close(p1[1]);
        //read a bite from the pipe1
        int flag;
        flag = read(p2[0],&temp,1);
        close(p2[0]);
        if(flag <=0){
            fprintf(2,"parent read from pipe error");
            exit(1);
        }
        else
            fprintf(1,"%d: received pong\n",parent_pid);
    }
    exit(0);
}