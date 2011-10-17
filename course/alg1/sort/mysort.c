#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"mysort.h"

#define DEFAULT_QSP 7

#ifndef QSORT_STOP_POINT
# define QSORT_STOP_POINT DEFAULT_QSP
#elif QSORT_STOP_POINT < 3
# undef QSORT_STOP_POINT
# define QSORT_STOP_POINT DEFAULT_QSP
#endif

void __swap(void *a, void *b, void *tmp, size_t size)
{
	if(a == b)
		return;
	memmove(tmp, a, size);
	memmove(a, b, size);
	memmove(b, tmp, size);
}

void sel_sort(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *))
{
	void *minp;
	int i;
	void *tmp=malloc(size);

	while(nmemb > 0){
		minp = base;
		for(i=1; i<nmemb; i++){
			if(compar(base+size*i, minp) < 0)
				minp = base+size*i;
		}
		__swap(base, minp, tmp, size);
		base += size;
		nmemb--;
	}
}

void insertion_sort(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *))
{
	void *itr1, *itr2;
	void *val=malloc(size);

	for(itr1=base; itr1<base+size*nmemb; itr1+=size){
		memmove(val, itr1, size);
		for(itr2=itr1-size; itr2>=base && compar(itr2, val)>0; itr2-=size){
			memmove(itr2+size, itr2, size);
		}
		memmove(itr2+size, val, size);
	}
}

void *__mid(const void *a, const void *b, const void *c, int(*compar)(const void *, const void *))
{
	if(compar(a, b) < 0){
		if(compar(b, c) < 0)
			return (void *)b;
		if(compar(a, c) < 0)
			return (void *)c;
		return (void *)a;
	}else{
		if(compar(a, c) < 0)
			return (void *)a;
		if(compar(b, c) < 0)
			return (void *)c;
		return (void *)b;
	}
}

void myqsort1(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *))
{
	size_t pos;
	size_t radix;
	size_t i;
	void *tmp=malloc(size);

	if(nmemb < QSORT_STOP_POINT){
		insertion_sort(base, nmemb, size, compar);
		return;
	}
	do{
		__swap(base+size*(nmemb-1), __mid(base, base+size*(nmemb/2), base+size*(nmemb-1), compar), tmp, size);
		radix = nmemb-1;
		pos = 0;
		for(i=0; i<radix; i++){
			int res=compar(base+size*i, base+size*radix);
			if(res == 0){
				__swap(base+size*i, base+size*(radix-1), tmp, size);
				radix--;
				i--;
			}else if(res < 0){
				__swap(base+size*i, base+size*pos, tmp, size);
				pos++;
			}
		}
		if(pos >= 2)
			myqsort1(base, pos, size, compar);
		for(i=radix; i<nmemb; i++, pos++)
			__swap(base+size*i, base+size*pos, tmp, size);
		base += size*pos;
		nmemb -= pos;
	}while(nmemb > QSORT_STOP_POINT);
	insertion_sort(base, nmemb, size, compar);
	free(tmp);
}

void myqsort2(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *))
{
	void *head, *tail;
	void *radix=malloc(size);
	void *tmp=malloc(size);
	if(nmemb <= QSORT_STOP_POINT){
		insertion_sort(base, nmemb, size, compar);
		return;
	}
	do{
		memmove(radix, __mid(base, base+size*(nmemb/2), base+size*(nmemb-1), compar), size);
		head = base-size;
		tail = base+size*nmemb;
		while(1){
			for(head+=size; compar(head, radix)<0; head+=size);
			for(tail-=size; compar(tail, radix)>0; tail-=size);
			if(head >= tail)
				break;
			__swap(head, tail, tmp, size);
		}
		myqsort2(tail+size, nmemb-(tail-base)/size-1, size, compar);
		nmemb = (tail-base)/size+1;
	}while(nmemb > QSORT_STOP_POINT);
	insertion_sort(base, nmemb, size, compar);
}

void __merge_sort(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *), void *buf)
{
	size_t mid;
	void *p;
	void *ap, *bp;

	if(nmemb == 1)
		return;
	mid = nmemb/2;
	__merge_sort(buf, mid, size, compar, base);
	__merge_sort(buf+size*mid, nmemb-mid, size, compar, base+size*mid);
	for(p=base,ap=buf,bp=buf+size*mid; p<base+size*nmemb; p+=size){
		if(ap >= buf+size*mid || bp >= buf+size*nmemb)
			break;
		if(compar(ap, bp) < 0){
			memmove(p, ap, size);
			ap += size;
		}else{
			memmove(p, bp, size);
			bp += size;
		}
	}
	while(ap < buf+size*mid){
		memmove(p, ap, size);
		ap += size;
		p += size;
	}
	while(bp < buf+size*nmemb){
		memmove(p, bp, size);
		bp += size;
		p += size;
	}
}

void merge_sort(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *))
{
	void *buf=malloc(nmemb*size);
	memcpy(buf, base, size*nmemb);
	__merge_sort(base, nmemb, size, compar, buf);
	free(buf);
}

void __build_heap(void *heap, void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *))
{
	size_t i, j;
	size_t pos;

	memcpy(heap, base, size);
	base += size;
	for(i=1; i<nmemb; i++){
		pos = i;
		for(j=pos/2;; j/=2){
			if(compar(base, heap+size*j) > 0){
				memcpy(heap+size*pos, heap+size*j, size);
				pos = j;
			}else{
				break;
			}
			if(j == 0)
				break;
		}
		memcpy(heap+size*pos, base, size);
		base += size;
	}
}

void heap_sort(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *))
{
	void *heap=malloc(size*nmemb);
	int i;
	int tail;
	int left, right;
	int comp_l, comp_r;
	void *tmp=malloc(size);

	__build_heap(heap, base, nmemb, size, compar);
	for(tail=nmemb-1; tail>0; tail--){
		__swap(heap, heap+size*tail, tmp, size);
		for(i=0; i<tail;){
			left = i*2;
			right = i*2+1;
			if(left < tail){
				comp_l = compar(heap+size*i, heap+size*left);
			}else{
				comp_l = 1;
			}
			if(right < tail){
				comp_r = compar(heap+size*i, heap+size*right);
			}else{
				comp_r = 1;
			}
			// judge
			if(comp_l < 0 && comp_r < 0){
				if(compar(heap+size*left, heap+size*right) > 0){
					__swap(heap+size*i, heap+size*left, tmp, size);
					i = left;
				}else{
					__swap(heap+size*i, heap+size*right, tmp, size);
					i = right;
				}
			}else if(comp_l < 0){
				__swap(heap+size*i, heap+size*left, tmp, size);
				i = left;
			}else if(comp_r < 0){
				__swap(heap+size*i, heap+size*right, tmp, size);
				i = right;
			}else{
				break;
			}
		}
	}
	memcpy(base, heap, size*nmemb);
}
