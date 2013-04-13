#include<cstdio>
#include<cstdlib>

struct Event{
	int startTime;
	int endTime;
};

int comp(const void *p, const void *q)
{
	return ((Event*)p)->startTime - ((Event*)q)->startTime;
}

int main()
{
	int n;
	int times=1;
	while(scanf("%d\n", &n) == 1){
		Event event[105];
		int elen=0;
		for(int i=0; i<n; i++){
			char buf[300];
			fgets(buf, 300, stdin);
			int sh, sm, eh, em;
			sscanf(buf, "%d:%d %d:%d", &sh, &sm, &eh, &em);
			event[elen].startTime = (sh-10)*60+sm;
			event[elen].endTime = (eh-10)*60+em;
			elen++;
		}
		event[elen].startTime = (18-10)*60+0; event[elen].endTime = (18-10)*60+1; elen++;
		qsort(event, n+1, sizeof(Event), comp);
		int from=0;
		int maxTime=0;
		int fromTime=0;
		for(int i=0; i<n+1; i++){
			if(event[i].startTime-from > maxTime){
				maxTime = event[i].startTime-from;
				fromTime = from;
			}
			from = event[i].endTime;
		}
		if(maxTime >= 60)
			printf("Day #%d: the longest nap starts at %d:%02d and will last for %d hours and %d minutes.\n", times, fromTime/60+10, fromTime%60, maxTime/60, maxTime%60);
		else
			printf("Day #%d: the longest nap starts at %d:%02d and will last for %d minutes.\n", times, fromTime/60+10, fromTime%60, maxTime);
		times++;
	}
	return 0;
}
