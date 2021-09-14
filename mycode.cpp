#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

static char reserveWord[32][20] = {
    "auto", "break", "case", "char", "const", "continue","default", "do", 
    "double", "else", "enum", "extern","float", "for","goto", "if", 
    "int", "long","register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void","volatile", "while"
};

static char operatorOrDelimiter[36][10] = {
    "+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
    "!=", ";", "(", ")", "^", ",", "\"", "\'", "#", "&",
    "&&", "|", "||", "%", "~", "<<", ">>", "[", "]", "{",
    "}", "\\", ".", "\?", ":", "!"
};

int searchReserve(char reserveWord[][20], char s[])
{
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(reserveWord[i], s) == 0)
        {//若成功查找，则返回种别码
            return i + 1;//返回种别码
        }
    }
    return -1;//否则返回-1，代表查找不成功，即为标识符
}

void filterResource(char r[], int pProject)
{
    char tempString[10000];
    int count = 0;
    for (int i = 0; i <= pProject; i++)
    {
        if (r[i] == '/' && r[i + 1] == '/')
        {//若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
            while (r[i] != '\n')
            {
                i++;//向后扫描
            }
        }
        if (r[i] == '/' && r[i + 1] == '*')
        {//若为多行注释“/* 。。。*/”则去除该内容
            i += 2;
            while (r[i] != '*' || r[i + 1] != '/')
            {
                i++;//继续扫描
                if (r[i] == '$')
                {
                    printf("注释出错，没有找到 */，程序结束！！！\n");
                    exit(0);
                }
            }
            i += 2;//跨过“*/”
        }
        if (r[i] != '\n' && r[i] != '\t' && r[i] != '\v' && r[i] != '\r')
        {//若出现无用字符，则过滤；否则加载
            tempString[count++] = r[i];
        }
    }
    tempString[count] = '\0';
    strcpy(r, tempString);//产生净化之后的源程序
}