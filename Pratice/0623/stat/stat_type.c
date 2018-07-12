/************************************************************
 * Copyright:  (C) 2017 Beijing itcast Technologies Co.,Ltd.
 * All rights reserved.
 * 
 * FileName: XXX.c
 * Author: Guo Tongbin (Tony)
 * 
 * Description:
 *  The template file format.
 *
 * Change Log:      
 *	<who>		<when>		<what>
 *	GuoTongbin	2017-0-0 	Created
 *       
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * 本代码用来获取，文件类型
 */

int main(int argc, char *argv[])
{

        if (argc < 2){
                printf("Please input: %s <file>\n", argv[0]);
                return 0;
        }

        /* int stat(const char *pathname, struct stat *buf); */
        int ret;
        struct stat st;
        
        ret = stat(argv[1],  &st);
        if (ret <0){
                perror("Error for stat");
                return 1;
        }

        char type ;
        switch (st.st_mode & S_IFMT){
        case S_IFREG:   /* 普通文件 */
                type = '-' ;
                break;
        case S_IFDIR:
                type = 'd';
                break;
                /* 。。。。 */
        default:
                type = '-'  ; /*  未知类型暂按普通文件处理 */
        }
        printf("%s 的类型：%c\n", argv[1], type);
        
        return 0;
}
