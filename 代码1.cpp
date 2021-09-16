#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;
//词法分析程序
//首先定义种别码
/*
关键字(32)
auto       break    case     char        const      continue
default    do       double   else        enum       extern
float      for      goto     if          int        long
register   return   short    signed      sizeof     static
struct     switch   typedef  union       unsigned   void
volatile    while


/****************************************************************************************/
//全局变量，关键字表
static char reserveWord[32][20] = {
    "auto", "break", "case", "char", "const", "continue","default", "do", 
    "double", "else", "enum", "extern","float", "for","goto", "if",
    "int", "long","register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void","volatile", "while"
};
//界符运算符表,根据需要可以自行增加
//static char operatorOrDelimiter[36][10] = {
//    "+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
//    "!=", ";", "(", ")", "^", ",", "\"", "\'", "#", "&",
//    "&&", "|", "||", "%", "~", "<<", ">>", "[", "]", "{",
//    "}", "\\", ".", "\?", ":", "!"
//};


/****************************************************************************************/

/********查找关键字*****************/
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
/********查找保留字*****************/

/*********************判断是否为字母********************/
bool IsLetter(char letter)
{//注意C语言允许下划线也为标识符的一部分可以放在首部或其他地方
    if (letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z' || letter == '_')
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*********************判断是否为字母********************/

/*********************判断是否为"{"********************/
bool IsLeft(char letter)
{//注意C语言允许下划线也为标识符的一部分可以放在首部或其他地方
    if (letter == '{' )
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*********************判断是否为"{"********************/

/*********************判断是否为"}"********************/
bool IsRight(char letter)
{//注意C语言允许下划线也为标识符的一部分可以放在首部或其他地方
    if (letter == '}')
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*********************判断是否为"}"********************/

/*****************判断是否为数字************************/
//bool IsDigit(char digit)
//{
//    if (digit >= '0' && digit <= '9')
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
/*****************判断是否为数字************************/


/********************编译预处理，取出无用的字符和注释**********************/
//ps：空格要留下
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
                if (r[i] == EOF)
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
/********************编译预处理，取出无用的字符和注释**********************/


/****************************分析子程序，算法核心***********************/
void Scanner(int& syn, char resourceProject[], char token[], int& pProject)
{//根据DFA的状态转换图设计
    int i, count = 0;//count用来做token[]的指示器，收集有用字符
    char ch;//当前字符
    ch = resourceProject[pProject];
    while (ch == ' ')//过滤空格
    {
        pProject++;
        ch = resourceProject[pProject];
    }
    for (i = 0; i < 20; i++)//清零
    {
        token[i] = '\0';
    }
    //判断开始
    if (IsLetter(resourceProject[pProject]))
    {//开头为字母
        token[count++] = resourceProject[pProject];//收集
        pProject++;//下移
        while (IsLetter(resourceProject[pProject]))
        {//后跟字母或数字
            token[count++] = resourceProject[pProject];//收集
            pProject++;//下移
        }//多读了一个字符既是下次将要开始的指针位置
        token[count] = '\0';//结束符
        syn = searchReserve(reserveWord, token);//查表找到种别码

        if (syn == -1)
        {//若不是保留字则是其他
            syn = 100;//特殊码，无意义
        }
        return;
    }
    else if (IsLeft(resourceProject[pProject]))
    {
        syn = 62;
        pProject++;
        return;
    }
    else if (IsRight(resourceProject[pProject]))
    {
        syn = 63;
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == EOF)//结束
    {
        syn = 0;//种别码为0
        return;
    }
    else//忽略其他符号
    {
        syn = 100;
        pProject++;//指针下移，为下一扫描做准备
        return;
    }
}

int countcase(string words[])
{
    static int temp = 0;
    int num = 0;
    while (1)
    {
        if (words[temp] == "switch")
        {
            temp++;
            break;
        }
        else
        {
            temp++;
        }
    }
    
    while (1)
    {
        if (words[temp] == "case")
        {
            num++;
            temp++;
        }
        else if (words[temp] == "switch"|| words[temp] == "end" )
        {
            break;
        }
        else
        {
            temp++;
        }
        
    }
    return num;
}

int countifelse(string words[],int x)
{
    string s;
    int flag = 0;
    while (words[flag] != "end")
    {
        if (words[flag] == "if"|| words[flag] == "else" || words[flag] == "{")
        {
            s += words[flag];
            
        }
        else if (words[flag] == "}")
        {
            if (s[s.length() - 1] == '}')
            {
                s += words[flag];
            }
            else 
            {
                if (s[s.length() - 2] == '{')
                {
                    s.pop_back();
                }
                else
                {
                    s += words[flag];
                }
            }

        }
        flag++;
    }

    //cout << endl << s << endl;

    int index = 0;//下标
    int count1 = 0;//ifelse计数
    int count2 = 0;//ifelse计数
    string sub1 = "if{}else{}";//子串
    string sub2 = "elseif{}";//子串

    while (s.length() != 2)
    {
        index = 0;
        while ((index = s.find(sub1, index)) < s.length())//找不到自动跳出
        {
            if (s[index - 1] != 'e')
            {
                count1++;
                s.erase(index, 10);//删掉子串
            }
            else
            {
                index++;
            }
        }
        index = 0;
        while ((index = s.find(sub2, index)) < s.length())//找不到自动跳出
        {
            int temp = index;
            index++;
            while (s.find(sub2, index) == index + 7)
            {
                index += 8;
            }
            count2++;
            index += 8;
            if (s[index] == 'l')
            {
                s.erase(temp - 4, index - temp + 9);
            }
            else
            {
                s.erase(temp - 4, index - temp + 3);
            }
        }
        //cout << endl << s << endl;
    }
    //cout << count2 << endl;
    if (x == 3)
    {
        return count1;
    }
    if (x == 4)
    {
        return count2;
    }
}

int main()
{
    //打开一个文件，读取其中的源程序
    int level=4;
    //string path;
    int totalnum = 0;
    int switchnum = 0;
    int casenum[1000] = {0};
    int ifelsenum = 0;
    int ifelseifelsenum = 0;
    string words[10000];
    int numforwords = 0;
    
    char resourceProject[10000];
    char token[20] = { 0 };
    int syn = -1, i;//初始化
    int pProject = 0;//源程序指针
    int keynum[32] = { 0 };
    FILE* fp, * fp1;
    string path= "D:\\lizi.txt";
    //cin >> path>>level;
    const char* path1 = path.c_str();
    
    if ((fp = fopen( path1 , "r")) == NULL)
    {//打开源程序
        cout << "can't open this file";
        exit(0);
    }
    resourceProject[pProject] = fgetc(fp);
    while (resourceProject[pProject] != EOF)
    {//将源程序读入resourceProject[]数组
        pProject++;
        resourceProject[pProject] = fgetc(fp);
    }
    resourceProject[++pProject] = '\0';
    //关闭文件
    fclose(fp);
    //输出代码
    //cout << endl << "源程序为:" << endl;
    //cout << resourceProject << endl;
    //对源程序进行过滤
    filterResource(resourceProject, pProject);
    //cout << endl << "过滤之后的程序:" << endl;
    //cout << resourceProject << endl;
    pProject = 0;//从头开始读

    while (syn != 0)
    {
        //启动扫描
        Scanner(syn, resourceProject, token, pProject);
        if (syn >= 1 && syn <= 32)
        {//保留字
            //printf("%s  \n", reserveWord[syn - 1]);
            words[numforwords] = reserveWord[syn - 1];
            numforwords++;
            totalnum++;
            keynum[syn - 1]++;
        }
        
        if (syn == 26)//是switch
        {
            switchnum++;

        }
        else if (syn == 62)
        {
            //printf("{  \n");
            words[numforwords] = "{";
            numforwords++;
        }
        else if (syn == 63)
        {
            //printf("}  \n");
            words[numforwords] = "}";
            numforwords++;
        }
        else if (syn == 0)
        {
            words[numforwords] = "end";
            numforwords++;
        }

    }
    
    for (int i = 0; i < 32; i++)
    {
        if (keynum[i] != 0)
        {
            printf("%s : %d  \n", reserveWord[i], keynum[i]);
        }
        
    }
    switch (level)
    {
        case 1:
            cout << "total num : " << totalnum << endl;
            break;
        case 2:
            cout << "total num : " << totalnum << endl;
            cout << "switch num : " << switchnum << endl;
            cout << "case num : " ;
            for (int i = 0; i < switchnum; i++)
            {
                cout << countcase(words) << ' ';
            }
            cout << endl;
            break;
        case 3:
            cout << "total num : " << totalnum << endl;
            cout << "switch num : " << switchnum << endl;
            cout << "case num : ";
            for (int i = 0; i < switchnum; i++)
            {
                cout << countcase(words) << ' ';
            }
            cout << endl;
            ifelsenum = countifelse(words,3);
            cout << "if-else num : " << ifelsenum << endl;
            break;
        case 4:
            cout << "total num : " << totalnum << endl;
            cout << "switch num : " << switchnum << endl;
            cout << "case num : ";
            for (int i = 0; i < switchnum; i++)
            {
                cout << countcase(words) << ' ';
            }
            cout << endl;
            ifelsenum = countifelse(words,3);
            ifelseifelsenum = countifelse(words, 4);
            cout << "if-else num : " << ifelsenum << endl;
            cout << "if-elseif-else num : " << ifelseifelsenum << endl;
            break;
    }
    //cout <<"total num : " << totalnum << endl;

    /*for (int i = 0; i < numforwords; i++)
    {
        cout<< words[i] <<endl;
    }*/

    return 0;
}