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

/*********************�ж��Ƿ�Ϊ"{"********************/
bool IsLeft(char letter)
{//ע��C���������»���ҲΪ��ʶ����һ���ֿ��Է����ײ��������ط�
    if (letter == '{' )
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*********************�ж��Ƿ�Ϊ"{"********************/

/*********************�ж��Ƿ�Ϊ"}"********************/
bool IsRight(char letter)
{//ע��C���������»���ҲΪ��ʶ����һ���ֿ��Է����ײ��������ط�
    if (letter == '}')
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*********************�ж��Ƿ�Ϊ"}"********************/


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
                if (r[i] == EOF)
                    break;
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
        if (r[i] == '\"' )
        {//��Ϊ�ַ�����ɾȥ
            i += 1;
            while (r[i] != '\"' )
            {
                
                if (r[i] == '\\')
                {
                    i+=2;
                }
                else if (r[i] == EOF)
                {
                    printf("�ַ�������û���ҵ�\"���������������\n");
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
        {//��Ϊ�ַ���ɾȥ
            i += 1;
            while (r[i] != '\'')
            {
                
                if (r[i] == '\\')
                {
                    i+=2;
                }
                else if (r[i] == EOF)
                {
                    printf("�ַ�����û���ҵ�\'���������������\n");
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
        token[count] = '\0';//������
        syn = searchReserve(reserveWord, token);//����ҵ��ֱ���

        if (syn == -1)
        {//�����Ǳ�������������
            syn = 100;//�����룬������
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
    else  if (resourceProject[pProject] == EOF)//����
    {
        syn = 0;//�ֱ���Ϊ0
        return;
    }
    else//������������
    {
        syn = 100;
        pProject++;//ָ�����ƣ�Ϊ��һɨ����׼��
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
    //������ֻ����ifelse{}��Щ�ַ��������һ�����ַ���������ֱ��ʹ��string�Ŀ⺯��
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

    int index = 0;//�±�
    string sub1 = "if{}else{}";//�Ӵ�
    string sub2 = "elseif{}";//�Ӵ�
    //���ڴ��ڸ���Ƕ�ף����Դ���һ��whileѭ����ÿ��ѭ�����ҵ�Ƕ�׵������ṹ��������ɾ����Ȼ���ٴ�ѭ����
    while (s.find("else", 0) < s.length())//����ַ����в�����else�ˣ�֤���Ѿ���ɼ���
    {
        //Ѱ��if-else
        index = 0;//Ѱ��ǰ����
        while ((index = s.find(sub1, index)) < s.length())//�Ҳ����Զ�����
        {
            if (s[index - 1] != 'e')
            {
                count1++;
                s.erase(index, 10);//ɾ��if{}else{}
            }
            else
            {
                index++;
            }
        }
        
        //Ѱ��if-elseif-else
        index = 0;//Ѱ��ǰ����
        while ((index = s.find(sub2, index)) < s.length())//�Ҳ����Զ�����
        {
            int temp = index;
            index++;
            while (s.find(sub2, index) == index + 7)//�����������elseif{}��������¼
            {
                index += 8;
            }
            index += 8;
            if (s[index] == 'l')//���������else()��if-elseif-else����+1��Ȼ��ɾ��
            {
                s.erase(temp - 4, index - temp + 9);
                count2++;
            }
            else//û�еĻ���ɾ��
            {
                s.erase(temp - 4, index - temp + 3);
            }
        }
        //cout << endl << s << endl;
    }


}

int main()
{
    //��һ���ļ�����ȡ���е�Դ����
    int level;//Ҫ��ȼ�
    string path;//�ļ�·��
    int totalnum = 0;
    int switchnum = 0;
    int casenum[1000] = {0};
    int ifelse_num = 0;
    int ifelseifelse_num = 0;
    string words[10000];
    int numforwords = 0;
    
    char resourceProject[100000];
    char token[30] = { 0 };
    int syn = -1, i;//��ʼ��
    int pProject = 0;//Դ����ָ��
    int keynum[32] = { 0 };
    FILE* fp, * fp1;
    //string path= "D:\\lizi.txt";
    cout << "Please enter path :" ;
    cin >> path;
    cout << endl <<"Please enter level :";
    cin >> level;
    cout << endl;
    const char* path1 = path.c_str();//��Ҫת����������Ϊconst char*
    
    if ((fp = fopen( path1 , "r")) == NULL)
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
            words[numforwords] = reserveWord[syn - 1];
            numforwords++;
            totalnum++;
            keynum[syn - 1]++;
        }
        
        if (syn == 26)//��switch
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
    
    //���ÿ�ֹؼ��ʵĸ���
    /*for (int i = 0; i < 32; i++)
    {
        if (keynum[i] != 0)
        {
            printf("%s : %d  \n", reserveWord[i], keynum[i]);
        }
        
    }*/
    
    //����ؼ����б�
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