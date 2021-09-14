
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;
//�ʷ���������
//���ȶ����ֱ���
/*
�ؼ���(32)
auto       break    case     char        const      continue
default    do       double   else        enum       extern
float      for      goto     if          int        long
register   return   short    signed      sizeof     static
struct     switch   typedef  union       unsigned   void
volatile    while


/****************************************************************************************/
//ȫ�ֱ������ؼ��ֱ�
static char reserveWord[32][20] = {
    "auto", "break", "case", "char", "const", "continue","default", "do", 
    "double", "else", "enum", "extern","float", "for","goto", "if",
    "int", "long","register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void","volatile", "while"
};
//����������,������Ҫ������������
//static char operatorOrDelimiter[36][10] = {
//    "+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
//    "!=", ";", "(", ")", "^", ",", "\"", "\'", "#", "&",
//    "&&", "|", "||", "%", "~", "<<", ">>", "[", "]", "{",
//    "}", "\\", ".", "\?", ":", "!"
//};


/****************************************************************************************/

/********���ҹؼ���*****************/
int searchReserve(char reserveWord[][20], char s[])
{
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(reserveWord[i], s) == 0)
        {//���ɹ����ң��򷵻��ֱ���
            return i + 1;//�����ֱ���
        }
    }
    return -1;//���򷵻�-1��������Ҳ��ɹ�����Ϊ��ʶ��
}
/********���ұ�����*****************/

/*********************�ж��Ƿ�Ϊ��ĸ********************/
bool IsLetter(char letter)
{//ע��C���������»���ҲΪ��ʶ����һ���ֿ��Է����ײ��������ط�
    if (letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z' || letter == '_')
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*********************�ж��Ƿ�Ϊ��ĸ********************/


/*****************�ж��Ƿ�Ϊ����************************/
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
/*****************�ж��Ƿ�Ϊ����************************/


/********************����Ԥ����ȡ�����õ��ַ���ע��**********************/
//ps���ո�Ҫ����
void filterResource(char r[], int pProject)
{
    char tempString[10000];
    int count = 0;
    for (int i = 0; i <= pProject; i++)
    {
        if (r[i] == '/' && r[i + 1] == '/')
        {//��Ϊ����ע�͡�//��,��ȥ��ע�ͺ���Ķ�����ֱ�������س�����
            while (r[i] != '\n')
            {
                i++;//���ɨ��
            }
        }
        if (r[i] == '/' && r[i + 1] == '*')
        {//��Ϊ����ע�͡�/* ������*/����ȥ��������
            i += 2;
            while (r[i] != '*' || r[i + 1] != '/')
            {
                i++;//����ɨ��
                if (r[i] == EOF)
                {
                    printf("ע�ͳ���û���ҵ� */���������������\n");
                    exit(0);
                }
            }
            i += 2;//�����*/��
        }
        if (r[i] != '\n' && r[i] != '\t' && r[i] != '\v' && r[i] != '\r')
        {//�����������ַ�������ˣ��������
            tempString[count++] = r[i];
        }
    }
    tempString[count] = '\0';
    strcpy(r, tempString);//��������֮���Դ����
}
/********************����Ԥ����ȡ�����õ��ַ���ע��**********************/


/****************************�����ӳ����㷨����***********************/
void Scanner(int& syn, char resourceProject[], char token[], int& pProject)
{//����DFA��״̬ת��ͼ���
    int i, count = 0;//count������token[]��ָʾ�����ռ������ַ�
    char ch;//��ǰ�ַ�
    ch = resourceProject[pProject];
    while (ch == ' ')//���˿ո�
    {
        pProject++;
        ch = resourceProject[pProject];
    }
    for (i = 0; i < 20; i++)//����
    {
        token[i] = '\0';
    }
    //�жϿ�ʼ
    if (IsLetter(resourceProject[pProject]))
    {//��ͷΪ��ĸ
        token[count++] = resourceProject[pProject];//�ռ�
        pProject++;//����
        while (IsLetter(resourceProject[pProject]))
        {//�����ĸ������
            token[count++] = resourceProject[pProject];//�ռ�
            pProject++;//����
        }//�����һ���ַ������´ν�Ҫ��ʼ��ָ��λ��
        token[count] = '\0';
        syn = searchReserve(reserveWord, token);//����ҵ��ֱ���
        if (syn == -1)
        {//�����Ǳ�������������
            syn = 100;//�����룬������
        }
        return;
    }
    else  if (resourceProject[pProject] == EOF)//����
    {
        syn = 0;//�ֱ���Ϊ0
    }
    else
    {
        syn = 100;
        pProject++;//ָ�����ƣ�Ϊ��һɨ����׼��
        return;
    }
}


int main()
{
    //��һ���ļ�����ȡ���е�Դ����
    char resourceProject[10000];
    char token[20] = { 0 };
    int syn = -1, i;//��ʼ��
    int pProject = 0;//Դ����ָ��
    int keynum[32] = { 0 };
    FILE* fp, * fp1;
    int num1=0;
    if ((fp = fopen("D:\\lizi.txt", "r")) == NULL)
    {//��Դ����
        cout << "can't open this file";
        exit(0);
    }
    resourceProject[pProject] = fgetc(fp);
    while (resourceProject[pProject] != EOF)
    {//��Դ�������resourceProject[]����
        pProject++;
        resourceProject[pProject] = fgetc(fp);
    }
    resourceProject[++pProject] = '\0';
    //�ر��ļ�
    fclose(fp);
    //�������
    //cout << endl << "Դ����Ϊ:" << endl;
    //cout << resourceProject << endl;
    //��Դ������й���
    filterResource(resourceProject, pProject);
    //cout << endl << "����֮��ĳ���:" << endl;
    //cout << resourceProject << endl;
    pProject = 0;//��ͷ��ʼ��

    while (syn != 0)
    {
        //����ɨ��
        Scanner(syn, resourceProject, token, pProject);
        if (syn >= 1 && syn <= 32)
        {//������
            //printf("%s  \n", reserveWord[syn - 1]);
            num1++;
            keynum[syn - 1]++;
        }

    }
    for (int i = 0; i < 32; i++)
    {
        if (keynum[i] != 0)
        {
            printf("%s : %d  \n", reserveWord[i], keynum[i]);
        }
        
    }
    cout << num1 << endl;
    return 0;
}