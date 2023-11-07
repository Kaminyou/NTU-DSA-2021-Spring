// Reference
// https://stackoverflow.com/questions/21060934/modify-merge-sort-for-storing-original-indexes
// https://www.geeksforgeeks.org/binary-indexed-tree-or-fenwick-tree-2/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"
#define ELEMENTRANGE 2097200
#define ELEMENTSHIFT 1048600
#define ELEMENTNUMMAX 3000001

typedef struct ft {
    int *sum_array;
    int n;
    int sum_all;
} FT_t;

void merge(int *a, int *b, int*c, int mid, int len)
{
    int tmp_a[len];
    int tmp_b[len];
    int tmp_c[len];
    int i = 0;
    int j = mid;
    int k = 0;

    memcpy(tmp_a, a, len*sizeof(*a));
    memcpy(tmp_b, b, len*sizeof(*b));
    memcpy(tmp_c, c, len*sizeof(*c));

    while (i < mid && j < len) {
        if (tmp_a[i] < tmp_a[j]) {
            a[k] = tmp_a[i];
            b[k] = tmp_b[i];
            c[k] = tmp_c[i];
            i++;
        }
        else {  
            a[k] = tmp_a[j];
            b[k] = tmp_b[j];
            c[k] = tmp_c[j];
            j++;
        }
        ++k; 
    }

    while (i < mid) {
        a[k] = tmp_a[i];
        b[k] = tmp_b[i];
        c[k] = tmp_c[i];
        k++;
        i++;
    }

    while (j < len) {
        a[k] = tmp_a[j];
        b[k] = tmp_b[j];
        c[k] = tmp_c[j];
        k++;
        j++;
    }
}

void mergeSort(int *a, int *b, int *c, int len)
{
    if (len > 1)
    {
        int mid = len / 2;
        mergeSort(a, b, c, mid);
        mergeSort(a+mid, b+mid, c+mid, len-mid); // note: pointer math for right-segment
        merge(a, b, c, mid, len);
    }
}

void resetSumArray(int *sum_array_pointer){
    for (int i =0;i<ELEMENTRANGE; i ++){
        sum_array_pointer[i] = 0;
    }
}

FT_t* createFT(int *sum_array_pointer){
    FT_t *ft = (FT_t*)malloc(sizeof(FT_t));
    ft->sum_array = sum_array_pointer;
    ft->n = ELEMENTRANGE;
    ft->sum_all = 0;
    return ft; 
}

void deleteFT(FT_t* ft){
    free(ft);
}

int lowbit(int x){
    return x & (-x);
}


void addFenwickTree(FT_t *ft, int x, int value){
    x += ELEMENTSHIFT;
    while (x <= ft->n){
        ft->sum_array[x] += value;
        x += lowbit(x);
    }
    ft->sum_all += value;
}

// equal and less than
int sumFenwickTree(FT_t *ft, int x){
    x += ELEMENTSHIFT;
    int s = 0;
    while (x > 0) {
        s += ft->sum_array[x];
        x -= lowbit(x);
    }
    return s;
}

// equal and larger than
int equalandlargerFenwickTree(FT_t *ft, int x){
    int lessthan;
    lessthan = sumFenwickTree(ft, x-1);
    return ft->sum_all - lessthan;
}

void triangle_get_left_right(int *q, int *r, int index, int *left, int *right){
    if (q[index] >= r[index]){
        *right = q[index];
        *left = r[index];
    }
    else{
        *right = r[index];
        *left = q[index];
    }
}

long long int count_triangle(int *p, int *q, int *r, int n, int *sum_array_pointer, int *duplicated_r_pointer){
    int now_idx = 0;
    int same_value;
    FT_t* ft = createFT(sum_array_pointer);
    long long int intersect = 0;
    int left, right;
    long long int temp;
    int duplicate_n = 0;
    int *duplicate_r = duplicated_r_pointer;

    mergeSort(p,q,r,n);
    while (now_idx < n - 1){
        if (p[now_idx] == p[now_idx + 1]){
            same_value = p[now_idx];
            duplicate_n = 0;
            while ((p[now_idx] == same_value) && (now_idx < n)){
                triangle_get_left_right(q, r, now_idx, &left, &right);

                temp = equalandlargerFenwickTree(ft, left);
                intersect += temp;

                duplicate_r[duplicate_n] = right;
                duplicate_n += 1;
                now_idx += 1;
            }

            temp = duplicate_n * (duplicate_n - 1) / 2;
            intersect += temp;
            for (int j = 0; j < duplicate_n; j++){
                addFenwickTree(ft, duplicate_r[j], 1);
            }


        }
        else {
            triangle_get_left_right(q, r, now_idx, &left, &right);
            

            temp = equalandlargerFenwickTree(ft, left);
            intersect += temp;
            addFenwickTree(ft, right, 1);

            now_idx += 1;
        }
    }
    if (now_idx == n - 1){
        triangle_get_left_right(q, r, now_idx, &left, &right);

        temp = equalandlargerFenwickTree(ft, left);
        intersect += temp;
        now_idx += 1;
    }
    deleteFT(ft);
    return intersect;
}

int main(){

    int _, t, n;
    static int sum_array_pointer[ELEMENTRANGE + 1];
    static int duplicated_r_pointer[ELEMENTNUMMAX];

    _ = scanf("%d",&t);

    for (int i=0;i<t;i++){

        if (i >= 1){
            resetSumArray(sum_array_pointer);
        }

        _ = scanf("%d",&n);
        int p[n];
        int q[n];
        int r[n];
        long long int answer = 0;
        for (int j =0;j<n;j++){
            _ = scanf("%d", &p[j]);
        }
        for (int j =0;j<n;j++){
            _ = scanf("%d", &q[j]);
        }
        for (int j =0;j<n;j++){
            _ = scanf("%d", &r[j]);
        }

        answer = count_triangle(p, q, r, n, sum_array_pointer, duplicated_r_pointer);
        printf("%lld\n", answer);

    }

    /*
    generator.init();
    int t = generator.getT();
    while (t--) {
        int n, *p, *q, *r;
        generator.getData(&n, &p, &q, &r);
        int ans = 0;
        ans = count_triangle(p, q, r, n);
        printf("%d\n", ans);
    }
    */
    return 0;
}