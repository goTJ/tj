/*   @JUDGE_ID:   10326EH   140   C */
#include<stdio.h>
#include<string.h>

enum { MAX = 30 };
char tab[MAX][MAX];
int ans[10];
int len,min;

int count(int from,int to,int temp[])
{
        int i,j;

        for(i=0;(temp[i]!=('A'+from))&&(temp[i]!=('A'+to));i++);
        for(j=1,i++;(temp[i]!=('A'+from))&&(temp[i]!=('A'+to));i++,j++);
        return j;
}

void check(int temp[])
{
        int sum,i,j,k,max=0;

        for(i=0;i<len;i++)
                for(j=0;j<MAX;j++){
                        k = temp[i] - 'A';
                        if(k==j)
                                sum = 0;
                        else if(tab[k][j])
                                sum = count(k,j,temp);
                        if(sum>max)
                                max = sum;
                }
        if(max<min){
/*              for(i=0;i<len;i++)
                        printf("%c ",temp[i]);
                printf("-> %d\n",min);
                */
                min = max;
                for(i=0;i<len;i++)
                        ans[i] = temp[i];
        }
}

void swap(int i,int n,int temp[])
{
        int t;

        t = temp[i];
        temp[i] = temp[n];
        temp[n] = t;
}

void perm(int deep,int temp[])
{
        int i;

        if(deep == (len-1)){
                /*
                for(i=0;i<len;i++)
                        printf("%c ",temp[i]);
                printf("\n");
                */
                check(temp);
        }
        else {
                for(i=deep;i<len;i++){
                        swap(deep,i,temp);
                        perm(deep+1,temp);
                        swap(i,deep,temp);
                }
        }
}


int input(int temp[])
{
        char str[100];
        int i,j,k,a[MAX];

        gets(str);
        for(i=0;i<MAX;i++)
                a[i] = 0;
        if(str[0] == '#')
                return 0;
        else {
                for(i=0;i<strlen(str);i++){
                        k = str[i] - 'A';
                        a[k] = 1;
                        for(i+=2;i<strlen(str)&&str[i]!=';';i++){
                                tab[k][str[i]-'A'] = 1;
                                a[str[i]-'A'] = 1;
                        }
                }
                for(j=0,i=0;i<MAX;i++)
                        if(a[i])
                                temp[j++] = i + 'A';
                len = j;
                return 1;
        }
}

int main(void)
{
        int i;
        int temp[10];

        while(input(temp)){
        //      bug();
                min = 8;
                perm(0,temp);
                if(len == 1){
                        printf("%c -> 0\n",temp[0]);
                        continue;
                }
                else {
                        for(i=0;i<len;i++)
                                printf("%c ",ans[i]);
                        printf("-> %d\n",min);
                }
                memset(tab,0,sizeof(tab));
                memset(temp,0,sizeof(temp));
        }
        return 0;
}
/* @END_OF_SOURCE_CODE */
