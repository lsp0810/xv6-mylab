#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int find(char *dirname,char *filename);

int find(char *dirname,char *filename){
    //find函数需要做到的几件事情：
    //读取当前目录的stat信息（送入的dirname必须是目录，不然报错并返回）
    //对当前目录的所有file文件strcmpy，符合的输出信息
    //对于目录文件进行递归迭代
    int fd;
    struct dirent de;
    struct stat   st;
    char buf[512],*p;
    
    if((fd = open(dirname,0))<0){
        fprintf(2,"no access to open %s\n",dirname);
        return 0;
    }

    if(fstat(fd,&st)<0){
        fprintf(2,"no access to stat %s\n",dirname);
        close(fd);
        return 0;
    }

    if(st.type != T_DIR){
        fprintf(2,"the first augment SHOULD BE a directory");
        close(fd);
        return 0;
    }
    //接下来需要便利此目录
    if(strlen(dirname) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("path too long\n");
        close(fd);
        return 0;
    }
    strcpy(buf, dirname);
    p = buf+strlen(buf);
    *p++ = '/';
    //迷惑的几点：
    //xv6中文件名长度固定是DIRSIZ么？
    //de.num的实际含义
    //为什么要对文件名的最后一位赋值为0？
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || strcmp(de.name,".")==0 || strcmp(de.name,"..")==0)
        continue;
      memmove(p, de.name, strlen(de.name));
      p[strlen(de.name)] = '\0';
      if(stat(buf, &st) < 0){
        printf("cannot stat %s\n", buf);
        continue;
      }
      switch(st.type){
        case T_DIR:
            find(buf,filename);
            break;
        default:
            if(strcmp(filename,de.name)==0)
                fprintf(1,"%s\n",buf);
            break;
      }
    }
    return 0;
}

int main(int argc,char *argv[]){
    if(argc<3){
        fprintf(2,"lack of argument,[usage]:find <directory> <filename>\n");
        exit(0);
    }
    //fprintf(1,"%s   %s\n",argv[1],argv[2]);
    find(argv[1],argv[2]);
    exit(0);
    return 0;
}