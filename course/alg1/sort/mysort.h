#ifndef MYSORT_H
#define MYSORT_H

extern void sel_sort(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *));
extern void insertion_sort(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *));
extern void myqsort1(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *));
extern void myqsort2(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *));
extern void merge_sort(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *));
extern void heap_sort(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *));
extern void qsort1(void *base, size_t nmemb, size_t size, int(*compar)(const void *, const void *));

#endif
