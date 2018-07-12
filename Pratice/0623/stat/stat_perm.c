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
 * 本代码用来获取，文件权限
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

        /* 三组，每组三位，加\0 */
        char perm_user[4] = { 0 };
        char perm_group[4] = { '-','-','-' };
        char perm_other[4] = "---";

        /* 以文件属主user为例 */
        perm_user[0]=(st.st_mode & S_IRUSR) ? 'r': '-'; /* 文件属主读权限 */
        perm_user[1]=(st.st_mode & S_IWUSR) ? 'w' : '-'; /* 文件属主写权限 */
        perm_user[2]=(st.st_mode & S_IXUSR) ? 'x' : '-'; /* 文件属主执行权限 */

        /* 补充其他两组 */
        
        printf("%s 的类型：%s%s%s\n", argv[1], perm_user, perm_group, perm_other);

        /* 考虑： 把三个数组，合并成一个数组，如何处理？ */
        return 0;
}
