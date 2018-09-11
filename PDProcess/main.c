#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int B1[10],B2[10];
char BTYPE[10][100];
int count = 0,countr=0,curr=2,line=0,a=0,b=0,wordNum=0;
FILE *fpr,*fpw,*fprr,*fpt;
char str[100],strr[100];
char WORD[100][100],POS[100][100],temp[1000];
int WLEN[100];
char delims[] = "/";
char *result=NULL;

int main()
{

    resetB();//B1,B2数组初始化
    fprr = fopen("result.txt", "r");//result在其他程序里面进行了预处理，除去双数行
    fpr = fopen("test.txt", "r");
    fpw = fopen("data888.xls","w") ;

    //单词WORD[0~1],词性POS[-3~3],单词长度WLEN[-1~1],前面的边界类型BTYPE[-1],当前的边界类型
    fprintf(fpw,"WORD[0]\tWORD[1]\tPOS[-3]\tPOS[-2]\tPOS[-1]\tPOS[0]\tPOS[1]\tPOS[2]\tPOS[3]\tWLEN[-1]\tWLEN[0]\tWLEN[1]\tWORD_NUM\tSENTENCE_LENGTH\tBTYPE[-1]\tBTYPE[0]\n" );
    while (fscanf(fpr, "%s", str) != EOF)
    {

        if(line==1)
        {
            WriteExcel();
            count=0;
            countr=0;
            line=0;
            curr=2;
            a=0;//每换一行，a,b都需要重置为0
            b=0;
            wordNum=0;
            resetB();
        }
         count++;
         //读result文件
         ReadResult();

        if(str[0]=='0'||str[0]==','||str[0]=='.'||(strcmp(str,"\'"))==0||str[0]=='?'||str[0]=='!')//引号需要考虑
        {
            strcpy(WORD[count],"NULL!");
            WLEN[count]=0;
            if(str[0]!='0')
            {
             strcpy(WORD[count],str);
             strcpy(POS[count],str);
             if(str[0]=='.'||str[0]=='?'||str[0]=='!')
                line++;
            }
            else
            {
                strcpy(POS[count],"NULL!");//表示句首
                 printf("%s\n",str);
            }

        }
        else
        {
            wordNum++;
            result = strtok(str, delims );
            strcpy(WORD[count],result);
            WLEN[count]=strlen(WORD[count]);
            result = strtok( NULL, delims );
            strcpy(POS[count],result);
        }
    }

    WriteExcel();
    fclose(fpr);
    fclose(fpw);
    fclose(fprr);

    return 0;
}


void setBtype(int curr)
{
    int x;
    if(B1[0]==0&&B2[0]==0)
    {

        strcpy(BTYPE[1],"B0");
        strcpy(BTYPE[0],"NULL!");
    }
    else
    {
      int i;
      for(i=0;i<10;i++)
            {

                 if(B1[0]>curr)
                {
                    strcpy(BTYPE[1],"B0");
                    strcpy(BTYPE[0],"NULL!");
                    x=0;
                    break;
                }
                else if(B1[i]<curr&&curr<B1[i+1])
                {
                    strcpy(BTYPE[1],"B0");
                    strcpy(BTYPE[0],"B1");
                    x=B1[i];
                    break;
                }
                else if(B1[i]==curr)
                {
                    strcpy(BTYPE[1],"B1");
                    if(i==0)
                    {
                        strcpy(BTYPE[0],"NULL!");
                        x=0;
                    }
                    else
                    {
                         strcpy(BTYPE[0],"B1");
                         x=B1[i-1];
                    }
                    break;
                }


            }
    for(i=0;i<10;i++)
            {
                if(B2[i]<curr&&curr<B2[i+1])
                {
                    if(x<B2[i])
                        strcpy(BTYPE[0],"B2");
                    break;
                }
                else if(B2[i]==curr)
                {
                    strcpy(BTYPE[1],"B2");
                    if(i!=0)
                    {
                        if(x<B2[i-1])
                            strcpy(BTYPE[0],"B2");
                    }
                    break;
                }

            }
    }
}

void resetB()
{
     int i;
      for(i=0;i<10;i++)
            {
                B1[i]=0;
                B2[i]=0;
            }
}

void WriteExcel()
{
     for(curr=2;curr<=count;curr++)
            {
                if((strcmp(WORD[curr],"NULL!"))==0);//句中出现标点，虽然不打印，可是curr要+1
                else
                {
                  setBtype(curr);
                if(curr<=3)
                fprintf(fpw,"%s\t%s\tNULL!\tNULL!\t%s\t%s\t%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%s\t%s\n",
                     WORD[curr],WORD[curr+1],POS[curr-1],POS[curr],POS[curr+1],POS[curr+2],POS[curr+3],
                     WLEN[curr-1],WLEN[curr],WLEN[curr+1],wordNum,count-1,BTYPE[0],BTYPE[1]) ;
                else if(curr<=count-3)
                fprintf(fpw,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%s\t%s\n",
                     WORD[curr],WORD[curr+1],POS[curr-3],POS[curr-2],POS[curr-1],POS[curr],POS[curr+1],POS[curr+2],POS[curr+3],
                     WLEN[curr-1],WLEN[curr],WLEN[curr+1],wordNum,count-1,BTYPE[0],BTYPE[1]) ;
                else if(curr==count-2)
                fprintf(fpw,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\tNULL!\t%d\t%d\t%d\t%d\t%d\t%s\t%s\n",
                WORD[curr],WORD[curr+1],POS[curr-3],POS[curr-2],POS[curr-1],POS[curr],POS[curr+1],POS[curr+2],
                WLEN[curr-1],WLEN[curr],WLEN[curr+1],wordNum,count-1,BTYPE[0],BTYPE[1]) ;
                else if(curr==count-1)
                fprintf(fpw,"%s\t%s\t%s\t%s\t%s\t%s\t%s\tNULL!\tNULL!\t%d\t%d\t%d\t%d\t%d\t%s\t%s\n",
                WORD[curr],WORD[curr+1],POS[curr-3],POS[curr-2],POS[curr-1],POS[curr],POS[curr+1],
                WLEN[curr-1],WLEN[curr],WLEN[curr+1],wordNum,count-1,BTYPE[0],BTYPE[1]) ;
                 }
            }
}
void ReadResult()
{
     if(countr==count-1)
        {

            fscanf(fprr, "%s", strr);
            countr++;
            int i;
            for(i=0;i<strlen(strr);i++)
            {
                if(strr[i]=='/')
                {
                  B1[a]=countr;
                  B1[a+1]=1000;//标注为结束
                  a++;
                  countr++;
                }
                else if(strr[i]=='%')
                {
                   B2[b]=countr;
                   B2[b+1]=1000;
                   b++;
                   countr++;
                }
                else if(strr[i]=='\'')//she's这种词分成两个词看待
                {
                    countr++;
                }
            }
        }
}
