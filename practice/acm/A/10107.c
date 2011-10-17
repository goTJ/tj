/* @JUDGE_ID: 10319NX 10107 C ""*/
#include<stdio.h>
#include<stdlib.h>

struct node {
	int num;
	struct node *next;
};

struct node *add(struct node *root ,int n)
{
	struct node *temp;
	
	if(root&&n> root->num)
		root->next = add(root->next,n);
	else if(root && n <= root->num){
		temp = malloc(sizeof(struct node));
		temp->num = n;
		temp->next = root;
		return temp;
	}
	else {
		root = malloc(sizeof(struct node));
		root->num = n;
		root->next = NULL;
	}
	return root;
}
void find(int len,struct node *p)
{
	int i,sum=0;

	if(len&1){
		len/=2;
		for(i=1;i<=len;i++,p=p->next);
		printf("%d\n",p->num);
	}
	else {
		len/=2;
		for(i=1;i<len;i++,p=p->next);
		sum=p->num;
		p=p->next;
		sum+=p->num;
		printf("%d\n",sum/2);
	}
}

int main()
{
	struct node *root,*temp;
	int len,n;
	
	len = 1;
	root = NULL;
	while(scanf("%d",&n)!=EOF){
		root = add(root,n);
		temp = root;
		if(len==1)
			printf("%d\n",root->num);
		else find(len,temp);
		len++;
	}
	return 0;
}
/*END_OF_SOURCE_CODE*/
