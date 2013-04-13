#include<cstdio>

long long count(int data[], int num, int buf[])
{
	int mid=num/2;
	long long sum=0;

	if(mid >= 2)
		sum += count(buf, mid, data);
	if(num-mid >= 2)
		sum += count(buf+mid, num-mid, data+mid);
	register int pa=0, pb=mid;
	for(int i=0; i<num; i++){
		if(pa < mid && pb < num){
			if(data[pa] <= data[pb]){
				buf[i] = data[pa++];
			}else{
				buf[i] = data[pb++];
				sum += mid-pa;
			}
		}else if(pa < mid){
			buf[i] = data[pa++];
		}else{
			buf[i] = data[pb++];
		}
	}
	return sum;
}

int main()
{
	int num;
	int *data, *buf;
	freopen("test_data.txt", "r", stdin);

	scanf("%d", &num);
	data = new int[num];
	buf = new int[num];
	for(int i=0; i<num; i++){
		scanf("%d", &data[i]);
		buf[i] = data[i];
	}
	printf("%lld\n", count(data, num, buf));
	delete data;
	delete buf;
	return 0;
}
