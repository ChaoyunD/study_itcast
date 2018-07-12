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

#include <time.h>

/**
 * 本代码用来获取，文件内容修改时间
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

        
        printf("%s 的内容修改时间：%s\n", argv[1], ctime(&st.st_mtime));

        /** 注意,在man 2 stat中， mtime还有一种形式，st_mtim */

        /* 建议使用： */
        printf("%s 的内容修改时间：%s\n", argv[1], ctime(&st.st_mtim.tv_sec));

        /* 注意： 在ctime产生的字符串， 会自动携带\n, 详细见man 3 time */
        return 0;
}
