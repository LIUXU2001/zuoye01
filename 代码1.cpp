#include <stack>
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
                if (r[i] == EOF)
                    break;
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
        if (r[i] == '\"' )
        {//若为字符串，删去
            i += 1;
            while (r[i] != '\"' )
            {
                
                if (r[i] == '\\')
                {
                    i+=2;
                }
                else if (r[i] == EOF)
                {
                    printf("字符串出错，没有找到\"，程序结束！！！\n");
                    exit(0);
                }
                else
                {
                    i++;
                }
            }
            i += 1;
        }
        if (r[i] == '\'')
        {//若为字符，删去
            i += 1;
            while (r[i] != '\'')
            {
                
                if (r[i] == '\\')
                {
                    i+=2;
                }
                else if (r[i] == EOF)
                {
                    printf("字符出错，没有找到\'，程序结束！！！\n");
                    exit(0);
                }
                else
                {
                    i++;
                }
            }
            i += 1;

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

void count_ifelse(string words[],int &count1,int &count2)
{
    string s;
    int flag = 0;
    //这里是只留下ifelse{}这些字符串，组成一个长字符串，方便直接使用string的库函数
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
                if (s.length() ==1)
                {
                    s.pop_back();
                }
                else if (s[s.length() - 2] == '{')
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
    string sub1 = "if{}else{}";//子串
    string sub2 = "elseif{}";//子串
    //由于存在各种嵌套，所以创建一个while循环，每次循环先找到嵌套的最里层结构，计数并删除，然后再次循环。
    while (s.find("else", 0) < s.length())//如果字符串中不包含else了，证明已经完成计数
    {
        //寻找if-else
        index = 0;//寻找前归零
        while ((index = s.find(sub1, index)) < s.length())//找不到自动跳出
        {
            if (s[index - 1] != 'e')
            {
                count1++;
                s.erase(index, 10);//删掉if{}else{}
            }
            else
            {
                index++;
            }
        }
        
        //寻找if-elseif-else
        index = 0;//寻找前归零
        while ((index = s.find(sub2, index)) < s.length())//找不到自动跳出
        {
            int temp = index;
            index++;
            while (s.find(sub2, index) == index + 7)//如果有连续的elseif{}，继续记录
            {
                index += 8;
            }
            index += 8;
            if (s[index] == 'l')//如果后面有else()，if-elseif-else计数+1，然后删掉
            {
                s.erase(temp - 4, index - temp + 9);
                count2++;
            }
            else//没有的话就删掉
            {
                s.erase(temp - 4, index - temp + 3);
            }
        }
        //cout << endl << s << endl;
    }


}

int main()
{
    //打开一个文件，读取其中的源程序
    int level;//要求等级
    string path;//文件路径
    int totalnum = 0;
    int switchnum = 0;
    int casenum[1000] = {0};
    int ifelse_num = 0;
    int ifelseifelse_num = 0;
    string words[10000];
    int numforwords = 0;
    
    char resourceProject[100000];
    char token[30] = { 0 };
    int syn = -1, i;//初始化
    int pProject = 0;//源程序指针
    int keynum[32] = { 0 };
    FILE* fp, * fp1;
    //string path= "D:\\lizi.txt";
    cout << "Please enter path :" ;
    cin >> path;
    cout << endl <<"Please enter level :";
    cin >> level;
    cout << endl;
    const char* path1 = path.c_str();//需要转换数据类型为const char*
    
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
    
    //输出每种关键词的个数
    /*for (int i = 0; i < 32; i++)
    {
        if (keynum[i] != 0)
        {
            printf("%s : %d  \n", reserveWord[i], keynum[i]);
        }
        
    }*/
    
    //输出关键词列表
    /*for (int i = 0; i < numforwords; i++)
    {
        cout << words[i] << endl;
    }*/

    switch (4)
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
            if (switchnum == 0)
            {
                cout << 0;
            }
            else 
            {
                for (int i = 0; i < switchnum; i++)
                {
                    cout << countcase(words) << ' ';
                }
            }
            cout << endl;
            count_ifelse(words, ifelse_num, ifelseifelse_num);
            cout << "if-else num : " << ifelse_num << endl;
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
            count_ifelse(words, ifelse_num, ifelseifelse_num);
            cout << "if-else num : " << ifelse_num << endl;
            cout << "if-elseif-else num : " << ifelseifelse_num << endl;
            break;
    }
    //cout <<"total num : " << totalnum << endl;

    /*for (int i = 0; i < numforwords; i++)
    {
        cout<< words[i] <<endl;
    }*/
    
    return 0;
}