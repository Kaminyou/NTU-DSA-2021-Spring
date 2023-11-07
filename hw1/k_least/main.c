// REFERENCE
// https://www.geeksforgeeks.org/doubly-linked-list/
// https://www.geeksforgeeks.org/reverse-a-doubly-linked-list/
// https://www.geeksforgeeks.org/reverse-a-list-in-groups-of-given-size/
// https://www.sanfoundry.com/c-program-find-kth-smallest-element-by-method-partitioning-array/
// https://openhome.cc/Gossip/AlgorithmGossip/QuickSort3.htm
// https://www.geeksforgeeks.org/search-insert-and-delete-in-a-sorted-array/
// https://www.geeksforgeeks.org/count-smaller-equal-elements-sorted-array/
// https://stackoverflow.com/questions/6182488/median-of-5-sorted-arrays

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#define MAXELEMENT 400
#define KEEPELEMENT 200
#define INITLIMIT 300 // should must be <= MAXELEMENT - 1
#define REVERSESTACKNUM 2000
#define PILESMAX 2000

typedef struct node {
    int data[MAXELEMENT];
    int sorted_data[MAXELEMENT];
    int max_len;
    int now_len;
    int min;
    int max;
    int reversed;
    struct node *next;
    struct node *prev;
} Node_t;

typedef struct sorted_array {
    int *sorted_array;
    int low;
    int high;
    int len;
} SortArray_t;


Node_t* createNode(){
    Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));
    new_node->max_len = MAXELEMENT;
    new_node->now_len = 0;
    new_node->reversed = 0;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node; 
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(int number[], int left, int right){
    int i = left;
    for (int j = left; j < right; j++) {
        if (number[j] < number[right]) {
            swap(&(number[j]), &(number[i]));
            i++;
        }
    }
    swap(&(number[right]), &(number[i]));
    return i;
}

void InSituQuickSort(int number[], int left, int right){ 
    if (left < right) { 
        int q = partition(number, left, right); 
        InSituQuickSort(number, left, q-1); 
        InSituQuickSort(number, q+1, right); 
    } 
} 

void updateNodeSortedArray(Node_t * node) {
    int len = node->now_len;
    for (int i = 0; i < len; i++){
        node->sorted_data[i] = node->data[i];
    }
    InSituQuickSort(node->sorted_data, 0, len-1);
}

// return index of a given value in sorted array
int searchSortedArray(int array[], int low, int high, int value){
    if (low > high){
        return -1;
    }
    else {
        int middle = (low + high) / 2;
        if (array[middle] == value){
            return middle;
        } else if (array[middle] < value) {
            return searchSortedArray(array, (middle + 1), high, value);
        } else {
            return searchSortedArray(array, low, (middle - 1), value);
        }
    }
}

int searchRightInSortedArray(int array[], int low, int high, int value, int len_now){
    if (low > high){
        return -1;
    }
    if (array[0] > value){
        return 0;
    }
    if (array[len_now-1] < value){
        return len_now;
    }
    else {
        int middle = (low + high) / 2;
        if (array[middle] == value){
            return middle + 1;
        } else if ((array[middle + 1] > value) && (array[middle] < value)){
            return middle + 1;
        } else if ((array[middle - 1] < value) && (array[middle] > value)){
            return middle;
        } else if (array[middle] < value) {
            return searchRightInSortedArray(array, (middle + 1), high, value, len_now);
        } else {
            return searchRightInSortedArray(array, low, (middle - 1), value, len_now);
        }
    }
}

void countSmallerAndSmallerEqual(int array[], int low, int high, int value, int len_now, int* S, int *SE){
    int leftSmaller = -1;
    int leftSmallerOrEqual = -1;
    
    while (low <= high) {
        int m = low + (high - low) / 2;
        if (array[m] < value) {
            leftSmaller = m;
            low = m + 1;
        }
        else
            high = m - 1;
    }
    leftSmallerOrEqual = leftSmaller;
    
    if ((leftSmallerOrEqual + 1 < len_now) && (array[leftSmallerOrEqual + 1] == value)){
        while ((array[leftSmallerOrEqual+1] == value) && (leftSmallerOrEqual + 1 < len_now)){
            leftSmallerOrEqual++;
        }
    }
    *S = leftSmaller + 1;
    *SE = leftSmallerOrEqual+1;
}
 

// num of values < value in sorted array 
int searchNumOfLeftInSortedArray(int array[], int low, int high, int value, int len_now){
    int index;
    index = searchRightInSortedArray(array, low, high, value, len_now);
    int num = array[low + index];
    if (value < num){
        return len_now - index;
    } else {
        while (num == array[low + index]){
            index += 1;
        }
        return len_now - index;
    }    
}

void insertSortedArray(int array[], int value, int len_now){   
    if (len_now == 0){
        array[0] = value;
        return;
    }
    int index, move;
    index = searchRightInSortedArray(array, 0, len_now-1, value, len_now);
    move = len_now - index;
    //shift
    for (int i = 0; i < move; i++){
        array[len_now - i] = array[len_now - i - 1];
    }
    array[index] = value;
}

void removeSortedArray(int array[], int value, int len_now){ 
    int index, move;
    index = searchSortedArray(array, 0, len_now-1, value);
    move = len_now - index - 1;
    //shift
    for (int i = 0; i < move; i++){
        array[index + i] = array[index + i + 1];
    }
}

SortArray_t* createSortArray(int *array, int low, int high, int len, int already_sorted){
    SortArray_t *new_sort_array = (SortArray_t*)malloc(sizeof(SortArray_t));
    if (!already_sorted){
        InSituQuickSort(array, low, high);
    }
    new_sort_array->sorted_array = array;
    new_sort_array->low = low;
    new_sort_array->high = high;
    new_sort_array->len = len;
    return new_sort_array; 
}

int kleastOfNArrayV2(SortArray_t* piles[], int n, int k){
    int left = -100001;
    int right = 100001;
    int middle;
    int L_sum, L_E_sum;
    int flag;
    int L_sum_i, L_E_sum_i;
    int L_sum_i_test, L_E_sum_i_test;
    while (left < right){
        middle = (left + right) >> 1;
        L_sum = 0;
        L_E_sum = 0;
        flag = 0; // 1->exist in one of the array
        for (int j = 0; j < n; j++){
            countSmallerAndSmallerEqual(piles[j]->sorted_array, piles[j]->low, piles[j]->high, middle, piles[j]->len, &L_sum_i, &L_E_sum_i);
            if (L_sum_i != L_E_sum_i){
                flag = 1;
            }
            L_sum += L_sum_i;
            L_E_sum += L_E_sum_i;
        }
        if (!flag){
            if (k > L_sum){
                left = middle;
            } else {
                right = middle;
            }
        } else {
            if ((L_sum < k) && (k <= L_E_sum)){
                
                return middle;
            }
            else if (k > L_sum){
                left = middle;
            } else {
                right = middle;
            }
        }

    }
    return -1;
}

int kth_smallest_qsort(int number[], int left, int right, int k)
{
    if (left > right)
        return INT_MAX;
    int p = partition(number, left, right);
    if ((p + 1) == k)
        return number[p];
    else if ((p + 1) < k)
        return kth_smallest_qsort(number, p + 1, right, k);
    else
        return kth_smallest_qsort(number, left, p - 1, k);
}

void printAllNode(Node_t *head){
    if (head == NULL){
        printf("No data\n");
        return;
    }
    int sss = 0;
    int rrr = 0;
    int data_num = 0;
    Node_t *last;
    while (head != NULL){
        data_num = head->now_len;
        if (!head->reversed){
            for (int i = 0; i < data_num; i ++) {
                printf("%d ", head->data[i]);
                sss += 1;
            }
        }
        else {
            for (int i = data_num - 1; i >= 0; i --) {
                printf("%d ", head->data[i]);
                sss += 1;
            }
        }

        printf("\n ");
        for (int i = 0; i < data_num; i ++) {
            printf("%d ", head->sorted_data[i]);
        }

        last = head;
        head = head->next;
        
        printf("\n\n ");
    }
    printf("\n");
    printf("-> %d\n", sss);
}

void printAllNodePara(Node_t *head){
    if (head == NULL){
        printf("No data\n");
        return;
    }
    int data_num = 0;
    while (head != NULL){
        data_num = head->now_len;
        if (!head->reversed){
            for (int i = 0; i < data_num; i ++) {
                printf("%d ", head->data[i]);
            }
        }
        else {
            for (int i = data_num - 1; i >= 0; i --) {
                printf("%d ", head->data[i]);
            }
        }
        printf("|| max=%d min=%d", head->max,  head->min);
        head = head->next;
        printf(" next\n");
    }
    printf("\n");
}

void initializeSeqData(Node_t **head, int data_array[], int total){
    // only for the initialization
    int node_required = total / INITLIMIT;
    if (total % INITLIMIT != 0){
        node_required += 1;
    }
    Node_t* pointer_now;
    for (int i = 0; i < node_required; i++){
        Node_t *new_node = createNode();
        int start = INITLIMIT * i;
        int end = INITLIMIT * (i + 1);
        int min = 100000;
        int max = -100000;
        if (end > total){
            end = total;
        }
        for (int j = start; j< end; j++){
            new_node->data[j - start] = data_array[j];
            insertSortedArray(new_node->sorted_data, data_array[j], (j - start));
            if (data_array[j] > max){
                max = data_array[j];
            }
            if (data_array[j] < min){
                min = data_array[j];
            }
        }
        new_node->min = min;
        new_node->max = max;
        new_node->now_len = end - start;

        if (*head == NULL){
            *head = new_node;
            pointer_now = *head;
        }
        else {
            pointer_now->next = new_node;
            new_node->prev = pointer_now;
            pointer_now = new_node;
        }
    }
}

void initializeSeqDataDirectly(Node_t **head, int total){
    // only for the initialization
    int temp, _;
    int node_required = total / INITLIMIT;
    if (total % INITLIMIT != 0){
        node_required += 1;
    }
    Node_t* pointer_now;
    for (int i = 0; i < node_required; i++){
        Node_t *new_node = createNode();
        int start = INITLIMIT * i;
        int end = INITLIMIT * (i + 1);
        int min = 100000;
        int max = -100000;
        if (end > total){
            end = total;
        }
        for (int j = start; j< end; j++){
            _ = scanf("%d", &temp);
            new_node->data[j - start] = temp;
            insertSortedArray(new_node->sorted_data, temp, (j - start));
            if (temp > max){
                max = temp;
            }
            if (temp < min){
                min = temp;
            }
        }
        new_node->min = min;
        new_node->max = max;
        new_node->now_len = end - start;

        if (*head == NULL){
            *head = new_node;
            pointer_now = *head;
        }
        else {
            pointer_now->next = new_node;
            new_node->prev = pointer_now;
            pointer_now = new_node;
        }
    }
}

void seperateFullNode(Node_t **head_p, Node_t *fullNode){
    if (fullNode->now_len == MAXELEMENT) {
        Node_t *new_node = createNode();
        int move_element_num = MAXELEMENT - KEEPELEMENT;
        int min = 100000;
        int max = -100000;
        for (int i = 0; i < move_element_num; i ++) {
            new_node->data[i] = fullNode->data[KEEPELEMENT+i];
            insertSortedArray(new_node->sorted_data, fullNode->data[KEEPELEMENT+i], i);
            if (fullNode->data[KEEPELEMENT+i] < min){
                min = fullNode->data[KEEPELEMENT+i];
            }
            if (fullNode->data[KEEPELEMENT+i] > max){
                max = fullNode->data[KEEPELEMENT+i];
            }
        }
        new_node->now_len = move_element_num;
        new_node->max = max;
        new_node->min = min;
        new_node->reversed = fullNode->reversed;

        min = 100000;
        max = -100000;
        for (int i = 0; i < KEEPELEMENT; i ++) {
            if (fullNode->data[i] < min){
                min = fullNode->data[i];
            }
            if (fullNode->data[i] > max){
                max = fullNode->data[i];
            }
        }
        fullNode->now_len = KEEPELEMENT;
        updateNodeSortedArray(fullNode);
        fullNode->max = max;
        fullNode->min = min;

        if (!fullNode->reversed){
            new_node->next = fullNode->next;
            if (fullNode->next != NULL){
                fullNode->next->prev = new_node;
            }
            new_node->prev = fullNode;
            fullNode->next = new_node;
        }else{
            new_node->next = fullNode;
            new_node->prev = fullNode->prev;
            if (fullNode->prev == NULL){
                *head_p = new_node;
            } else {
                fullNode->prev->next = new_node;
            }
            fullNode->prev = new_node;
        }
    }
    else{
        return;
    }
}

void insertUpdateMinMax(Node_t *node, int x){
    if (node->min > x){
        node->min = x;
    } 
    if (node->max < x) {
        node->max = x;
    }
}

void deleteUpdateMinMax(Node_t *node, int deleted_x){
    int data_num = node->now_len;
    if (node->min == deleted_x){
        int min = 100000;
        for (int k =0; k < data_num; k++){
            if (node->data[k] < min){
                min = node->data[k];
            }
        }
        node->min = min;
    } 
    if (node->max == deleted_x) {
        int max = -100000;
        for (int k =0; k < data_num; k++){
            if (node->data[k] > max){
                max = node->data[k];
            }
        }
        node->max = max;
    }
}

void updateNodeMinMax(Node_t *node){
    int data_num = node->now_len;
    int min = 100000;
    int max = -100000;
    for (int k =0; k < data_num; k++){
        if (node->data[k] > max){
            max = node->data[k];
        }
        if (node->data[k] < min){
            min = node->data[k];
        }
    }
    node->max = max;
    node->min = min;
}

void insertData(Node_t **head_p, Node_t *head, int i, int x){
    // if there is no node existing
    Node_t *prev_N;
    if (head == NULL){
        Node_t *new_node = createNode();
        *head_p = new_node;
        new_node->data[0] = x;
        insertSortedArray(new_node->sorted_data, x, 0);
        new_node->now_len = 1;
        new_node->max = x;
        new_node->min = x;
        return;
    }
    // insert front!
    // step 1: find where to insert
    int index = i - 1;
    int cum = 0;
    int data_num = 0;
    while (head != NULL){
        data_num = head->now_len;
        
        // the normal case
        if ((cum <= index) && (cum + data_num > index)) {
            
            int loc;
            if (!head->reversed){
                loc = index - cum;
            } else {
                loc = data_num - (index - cum);
            }
            // shift the array
            for (int k = data_num - loc - 1; k >=0 ; k--){
                head->data[loc+k+1] = head->data[loc+k];
            }
            head->data[loc] = x;
            insertSortedArray(head->sorted_data, x, head->now_len);
            head->now_len += 1;
            insertUpdateMinMax(head, x);
            seperateFullNode(head_p, head);
            return;
        } 
        
        else {
            cum += data_num;
            prev_N = head;
            head = head->next;
        }
    }
    // N+1 case
    if (cum == index){
        if (!prev_N->reversed){
           
            prev_N->data[data_num] = x;
        } else {
            for (int k = data_num; k > 0; k--){
                prev_N->data[k] = prev_N->data[k - 1];
            }
            prev_N->data[0] = x;
        }
        insertSortedArray(prev_N->sorted_data, x, prev_N->now_len);
        prev_N->now_len += 1;
        insertUpdateMinMax(prev_N, x);
        seperateFullNode(head_p, prev_N);
        return;
    }
    printf("cum=%d i=%d\n", cum, i);
    return;
}

void deleteEmptyNode(Node_t **head_p, Node_t *node){
    if (node->now_len == 0){
        Node_t* prev_node = node->prev;
        Node_t* next_node = node->next;
        if ((prev_node == NULL) && (next_node == NULL)){
            *head_p = NULL;
            free(node);
            return;
        } else if (prev_node == NULL){
            *head_p = next_node;
            next_node->prev = NULL;
        } else if (next_node == NULL){
            prev_node->next = NULL;
        } else {
            prev_node->next = next_node;
            next_node->prev = prev_node;
        }
        free(node);
    } else {
        return;
    }
}

void deleteData(Node_t **head_p, Node_t *head, int i){
    int index = i - 1;
    int cum = 0;
    int data_num = 0;
    while (head != NULL){
        data_num = head->now_len;
        
        // the normal case
        if ((cum <= index) && (cum + data_num > index)) {
            int loc;
            if (!head->reversed){
                loc = index - cum;                
            } else {
                loc = data_num - (index - cum) - 1;
            }
            int deleted_x = head->data[loc];
            // shift the array
            for (int k = 0; k < data_num - loc - 1; k++){
                head->data[loc+k] = head->data[loc+k+1];
            }
            removeSortedArray(head->sorted_data, deleted_x, head->now_len);
            head->now_len -= 1;
            
            deleteUpdateMinMax(head, deleted_x);
            
            deleteEmptyNode(head_p, head);
            return;
        } 
        else {
            cum += data_num;
            head = head->next;
        }
    }
    printf("cum=%d i=%d\n", cum, i);
    return;
}

void partialReverseData(Node_t *node, int l_index, int r_index){
    
    int temp[MAXELEMENT];
    int num_to_reverse = r_index - l_index + 1;
    if (num_to_reverse == node->now_len){
        node->reversed = !node->reversed;
        return;
    }
    if (!node->reversed){
        for (int k = 0; k < num_to_reverse; k++){
            temp[k] = node->data[l_index + k];
        }
        for (int k = 0; k < num_to_reverse; k++){
            node->data[l_index + k] = temp[num_to_reverse - k - 1];
        }
    }
    else {
        int reverse_l_index = node->now_len - r_index - 1;
        int reverse_r_index = node->now_len - l_index - 1;
        for (int k = 0; k < num_to_reverse; k++){
            temp[k] = node->data[reverse_l_index + k];
        }
        for (int k = 0; k < num_to_reverse; k++){
            node->data[reverse_l_index + k] = temp[num_to_reverse - k - 1];
        }
    }
}

void reverseSelfNode(Node_t *node){
    int temp_array[MAXELEMENT];
    int len = node->now_len;
    for (int i = 0; i < len; i++){
        temp_array[i] = node->data[i];
    }
    for (int i = 0; i < len; i++){
        node->data[i] = temp_array[len - i - 1];
    }
    node->reversed = !node->reversed;
}

void combineLeftRightNode(Node_t **head_p, Node_t *l_node, Node_t *r_node){
    
    if ((l_node == NULL) || (r_node == NULL)){
        return;
    } else {
        
        if ((l_node->now_len + r_node->now_len) > INITLIMIT){
            return;
        }
        if ((l_node->reversed == 1) && (r_node->reversed == 0)){
            // reverse left node
            int l_len = l_node->now_len;
            int r_len = r_node->now_len;
            reverseSelfNode(l_node);
            //move r to l'r
            for (int i = 0; i < r_len; i++){
                l_node->data[l_len + i] = r_node->data[i];
                insertSortedArray(l_node->sorted_data, r_node->data[i], l_len + i);
                if (r_node->data[i] < l_node->min){
                    l_node->min = r_node->data[i];
                }
                if (r_node->data[i] > l_node->max){
                    l_node->max = r_node->data[i];
                }
            }
            l_node->reversed = 0;
            l_node->now_len = l_len + r_len;
            l_node->next = r_node->next;

            if(r_node->next != NULL){
                r_node->next->prev = l_node;
            }
            free(r_node);

        } else if ((l_node->reversed == 1) && (r_node->reversed == 1)){

            int l_len = l_node->now_len;
            int r_len = r_node->now_len;
            //move l to r'r
            for (int i = 0; i < l_len; i++){
                r_node->data[r_len + i] = l_node->data[i];
                insertSortedArray(r_node->sorted_data, l_node->data[i], r_len + i);
                if (l_node->data[i] < r_node->min){
                    r_node->min = l_node->data[i];
                }
                if (l_node->data[i] > r_node->max){
                    r_node->max = l_node->data[i];
                }
            }
            r_node->now_len = l_len + r_len;
            r_node->prev = l_node->prev;
            if(l_node->prev != NULL){
                l_node->prev->next = r_node;
            } else {
                *head_p = r_node;
            }
            free(l_node);
            
        } else if ((l_node->reversed == 0) && (r_node->reversed == 1)){

            int l_len = l_node->now_len;
            int r_len = r_node->now_len;
            // reverse right node
            reverseSelfNode(r_node);
            //move r to l'r
            for (int i = 0; i < r_len; i++){
                l_node->data[l_len + i] = r_node->data[i];
                insertSortedArray(l_node->sorted_data, r_node->data[i], l_len + i);
                if (r_node->data[i] < l_node->min){
                    l_node->min = r_node->data[i];
                }
                if (r_node->data[i] > l_node->max){
                    l_node->max = r_node->data[i];
                }
            }
            l_node->now_len = l_len + r_len;
            l_node->next = r_node->next;

            if(r_node->next != NULL){
                r_node->next->prev = l_node;
            }
            free(r_node);
        } else if ((l_node->reversed == 0) && (r_node->reversed == 0)){

            int l_len = l_node->now_len;
            int r_len = r_node->now_len;
            for (int i = 0; i < r_len; i++){
                l_node->data[l_len + i] = r_node->data[i];
                insertSortedArray(l_node->sorted_data, r_node->data[i], l_len + i);
                if (r_node->data[i] < l_node->min){
                    l_node->min = r_node->data[i];
                }
                if (r_node->data[i] > l_node->max){
                    l_node->max = r_node->data[i];
                }
            }
            l_node->now_len = l_len + r_len;
            l_node->next = r_node->next;

            if(r_node->next != NULL){
                r_node->next->prev = l_node;
            }
            free(r_node);
            
        }
    }
}

void crossReverseData(Node_t **head_p, Node_t *node, int l_index, int r_index){
    int num_to_reverse;
    int max, min;
    int cum = 0;
    Node_t * most_left_node = node;
    Node_t * most_right_node;
    // make a new node for the first part
    int data_num = node->now_len;
    Node_t *start_node = createNode();
    num_to_reverse = data_num - l_index;
    
    max = -100000;
    min = 100000;
    if (!node->reversed){
        for (int k = 0; k < num_to_reverse; k++){
            start_node->data[k] = node->data[l_index+k];
            insertSortedArray(start_node->sorted_data, node->data[l_index+k], k);
            if (node->data[l_index+k] < min){
                min = node->data[l_index+k];
            }
            if (node->data[l_index+k] > max){
                max = node->data[l_index+k];
            }
        }
    } else {
        for (int k = 0; k < num_to_reverse; k++){
            start_node->data[k] = node->data[k];
            insertSortedArray(start_node->sorted_data, node->data[k], k);
            if (node->data[k] < min){
                min = node->data[k];
            }
            if (node->data[k] > max){
                max = node->data[k];
            }
        }
        for (int k = 0; k < data_num - num_to_reverse; k++){
            node->data[k] = node->data[num_to_reverse + k]; //shift to front
        }
    }
    
    start_node->max = max;
    start_node->min = min;
    start_node->reversed = !node->reversed;

    node->now_len -= num_to_reverse;
    start_node->now_len = num_to_reverse;
    updateNodeSortedArray(node);
    updateNodeMinMax(node);
    
    // shift to next segment
    node = node->next;
    cum += data_num;
    
    // next segment
    Node_t *stack_node_pointers[REVERSESTACKNUM];
    stack_node_pointers[0] = start_node;
    int node_num = 1;

    while (1){
        data_num = node->now_len;
        if (r_index == cum + data_num - 1){
            most_right_node = node->next;
            node->reversed = !node->reversed;
            stack_node_pointers[node_num] = node;
            node_num += 1;
            break;

        } else if (r_index < cum + data_num - 1){
            most_right_node = node;
            num_to_reverse = r_index - cum + 1;
            Node_t *end_node = createNode();
            max = -100000;
            min = 100000;
            if (!node->reversed){
                for (int k = 0; k < num_to_reverse; k++){
                    end_node->data[k] = node->data[k];
                    insertSortedArray(end_node->sorted_data, node->data[k], k);
                    if (node->data[k] < min){
                        min = node->data[k];
                    }
                    if (node->data[k] > max){
                        max = node->data[k];
                    }
                }
                for (int k = 0; k < data_num - num_to_reverse; k++){
                    node->data[k] = node->data[num_to_reverse + k]; //shift to front
                }
            } else {
                for (int k = 0; k < num_to_reverse; k++){
                    end_node->data[k] = node->data[data_num - num_to_reverse + k];
                    insertSortedArray(end_node->sorted_data, node->data[data_num - num_to_reverse + k], k);
                    if (end_node->data[k] < min){
                        min = end_node->data[k];
                    }
                    if (end_node->data[k] > max){
                        max = end_node->data[k];
                    }
                }
            }
            node->now_len -= num_to_reverse;
            end_node->now_len = num_to_reverse;
            end_node->max = max;
            end_node->min = min;
            end_node->reversed = !node->reversed;
            updateNodeSortedArray(node);
            updateNodeMinMax(node);
            stack_node_pointers[node_num] = end_node;
            node_num += 1;

            break;
        } else {
            node->reversed = !node->reversed;
            stack_node_pointers[node_num] = node;
            node_num += 1;

            node = node->next;
            cum += data_num;
        }
    }
    // combine most_left_node, node stack, most_right_node
    // node_num must >= 2
    Node_t* prev_N, *curr_N, *next_N;
    
    most_left_node->next = stack_node_pointers[node_num - 1];
    stack_node_pointers[node_num - 1]->prev = most_left_node;
    stack_node_pointers[node_num - 1]->next = stack_node_pointers[node_num - 2];
    
    for (int k = node_num - 2; k >= 1; k--){
        curr_N = stack_node_pointers[k];
        prev_N = stack_node_pointers[k+1];
        next_N = stack_node_pointers[k-1];
        curr_N->prev = prev_N;
        curr_N->next = next_N;
    }
    stack_node_pointers[0]->prev = stack_node_pointers[1];
    stack_node_pointers[0]->next = most_right_node;

    if (most_right_node != NULL){
        most_right_node->prev = stack_node_pointers[0];
    } // mind!!!
    
    // to deal with the empty most left node
    if(most_left_node->now_len == 0) {
        most_left_node->next->prev = most_left_node->prev;
        if (most_left_node->prev == NULL){
            *head_p = most_left_node->next;
        } else {
            most_left_node->prev->next = most_left_node->next;
        }
        
        free(most_left_node);
    }

    // left
    combineLeftRightNode(head_p, stack_node_pointers[node_num - 1]->prev, stack_node_pointers[node_num - 1]);
    //right
    combineLeftRightNode(head_p, stack_node_pointers[0], stack_node_pointers[0]->next);
}

void reverseData(Node_t **head_p, Node_t *head, int l, int r){
    
    // no need to reverse
    if (r - l == 0) {
        return;
    }
    int l_index = l - 1;
    int r_index = r - 1;
    int cum = 0;
    int data_num = 0;
    int l_boundary, r_boundary;

    while (head != NULL){
        data_num = head->now_len;
        l_boundary = cum;
        r_boundary = cum + data_num; //not included

        // only in one segment
        if ((l_boundary <= l_index) && (r_boundary > r_index)){
            
            partialReverseData(head, l_index - l_boundary, r_index - l_boundary);
            return;
        }
        // step into one segment
        else if ((l_index - l_boundary >= 0) &&(l_index - l_boundary <= data_num - 1)) {
            
            crossReverseData(head_p, head, l_index - l_boundary, r_index - l_boundary);
            return;
        }
        
        cum += data_num;
        head = head->next;
    }
}

int getMinInSegData(Node_t *node, int l_index, int r_index){
    int min = 100000;
    int num_to_count = r_index - l_index + 1;
    if (!node->reversed){
        for (int k = 0; k < num_to_count; k++){
            if (node->data[l_index + k] < min) {
                min = node->data[l_index + k];
            }
        }
    }
    else {
        int reverse_l_index = node->now_len - r_index - 1;
        int reverse_r_index = node->now_len - l_index - 1;
        for (int k = 0; k < num_to_count; k++){
            if (node->data[reverse_l_index + k] < min) {
                min = node->data[reverse_l_index + k];
            }
        }
    }
    return min;
}

int getKMinInSegData(Node_t *node, int l_index, int r_index, int k_least){
    int sorted_temp[MAXELEMENT];
    int num_to_count = r_index - l_index + 1;
    int kmin;
    if (!node->reversed){
        for (int k = 0; k < num_to_count; k++){
            sorted_temp[k] = node->data[l_index + k];
        }
    }
    else {
        int reverse_l_index = node->now_len - r_index - 1;
        int reverse_r_index = node->now_len - l_index - 1;
        for (int k = 0; k < num_to_count; k++){
            sorted_temp[k] = node->data[reverse_l_index + k];
        }
    }
    kmin = kth_smallest_qsort(sorted_temp, 0, num_to_count-1, k_least);
    return kmin;
}

int getMinInCrossData(Node_t *node, int l_index, int r_index){
    int min = 100000;
    int num_to_count;
    int cum = 0;
    int data_num = node->now_len;
    num_to_count = data_num - l_index;
    if (!node->reversed){
        for (int k = 0; k < num_to_count; k++){
            if (node->data[l_index+k] < min){
                min = node->data[l_index+k];
            }
        }
    } else {
        for (int k = 0; k < num_to_count; k++){
            if (node->data[k] < min){
                min = node->data[k];
            }
        }
    }
    // shift to next segment
    node = node->next;
    cum += data_num;
    // next segment

    while (1){
        data_num = node->now_len;
        if (r_index == cum + data_num - 1){
            if (node->min < min){
                min = node->min;
            }
            break;

        } else if (r_index < cum + data_num - 1){
            num_to_count = r_index - cum + 1;
            if (!node->reversed){
                for (int k = 0; k < num_to_count; k++){
                    if (node->data[k] < min){
                        min = node->data[k];
                    }
                }
            } else {
                for (int k = 0; k < num_to_count; k++){
                    if (node->data[data_num - num_to_count + k] < min){
                        min = node->data[data_num - num_to_count + k];
                    }
                }
            }

            break;
        } else {
            if (node->min < min){
                min = node->min;
            }

            node = node->next;
            cum += data_num;
        }
    }
    return min;
}

int getKMinInCrossData(Node_t *node, int l_index, int r_index, int k_least){

    int total_num_involved = r_index - l_index + 1;
    int kmin;
    int num_to_count;
    int cum = 0;
    int data_num = node->now_len;
    int sorted_tempA[MAXELEMENT];
    int sorted_tempB[MAXELEMENT];
    num_to_count = data_num - l_index;

    SortArray_t *tempSA;
    SortArray_t* stackSAs[REVERSESTACKNUM];
    int num_stackSAs = 0;

    // FIRST NODE
    if (!node->reversed){
        for (int k = 0; k < num_to_count; k++){
            sorted_tempA[k] = node->data[l_index+k];
        }
    } else {
        for (int k = 0; k < num_to_count; k++){
            sorted_tempA[k] = node->data[k];
        }
    }

    tempSA = createSortArray(sorted_tempA, 0, num_to_count - 1, num_to_count, 0);
    stackSAs[num_stackSAs] = tempSA;
    num_stackSAs += 1;

    // shift to next segment
    node = node->next;
    cum += data_num;
    // next segment

    while (1){
        data_num = node->now_len;
        if (r_index == cum + data_num - 1){
            
            tempSA = createSortArray(node->sorted_data, 0, data_num - 1, data_num, 1);
            stackSAs[num_stackSAs] = tempSA;
            num_stackSAs += 1;

            break;

        } else if (r_index < cum + data_num - 1){
            num_to_count = r_index - cum + 1;
            if (!node->reversed){
                for (int k = 0; k < num_to_count; k++){
                    sorted_tempB[k] = node->data[k];
                }
            } else {
                for (int k = 0; k < num_to_count; k++){
                    sorted_tempB[k] = node->data[data_num - num_to_count + k];
                }
            }
            tempSA = createSortArray(sorted_tempB, 0, num_to_count - 1, num_to_count, 0);
            stackSAs[num_stackSAs] = tempSA;
            num_stackSAs += 1;

            break;
        } else {
            tempSA = createSortArray(node->sorted_data, 0, data_num - 1, data_num, 1);
            stackSAs[num_stackSAs] = tempSA;
            num_stackSAs += 1;

            node = node->next;
            cum += data_num;
        }
    }
    kmin = kleastOfNArrayV2(stackSAs, num_stackSAs, k_least);
    for (int t=0;t<num_stackSAs;t++){
        free(stackSAs[t]);
    }
    return kmin;
}

int queryKLEAST(Node_t *head, int l, int r, int k){
    int l_index = l - 1;
    int r_index = r - 1;
    int cum = 0;
    int data_num = 0;
    int l_boundary, r_boundary;
    int minimum = 100000;

    if (k == 1){
        while (head != NULL){
            data_num = head->now_len;
            l_boundary = cum;
            r_boundary = cum + data_num; //not included
            // only in one segment
            if ((l_boundary <= l_index) && (r_boundary > r_index)){
                
                minimum = getMinInSegData(head, l_index - l_boundary, r_index - l_boundary);
                return minimum;
            }
            // step into one segment
            else if ((l_index - l_boundary >= 0) &&(l_index - l_boundary <= data_num - 1)) {
                minimum = getMinInCrossData(head, l_index - l_boundary, r_index - l_boundary);
                return minimum;
            }
            
            cum += data_num;
            head = head->next;
            }
    } else {
        while (head != NULL){
            data_num = head->now_len;
            l_boundary = cum;
            r_boundary = cum + data_num; //not included
            // only in one segment
            if ((l_boundary <= l_index) && (r_boundary > r_index)){
                
                minimum = getKMinInSegData(head, l_index - l_boundary, r_index - l_boundary, k);
                return minimum;
            }
            // step into one segment
            else if ((l_index - l_boundary >= 0) &&(l_index - l_boundary <= data_num - 1)) {
                minimum = getKMinInCrossData(head, l_index - l_boundary, r_index - l_boundary, k);
                return minimum;
            }
            
            cum += data_num;
            head = head->next;
            }
    }
}

int main(){
    clock_t start, end;
    start = clock();
    Node_t *head = NULL;
    
    int temp;
    char command[15];
    int param1, param2, param3;
    int _;
    int kmin;

    int vector_length, operation_num;
    _ = scanf("%d", &vector_length);
    _ = scanf("%d", &operation_num);
    initializeSeqDataDirectly(&head, vector_length);
    for (int i=0;i<operation_num;i++){
        _ = scanf("%s", command);

        if (!strcmp(command, "Delete")) {
            _ = scanf("%d", &param1);
            //printf("%d %s %d\n", i, command, param1);

            deleteData(&head, head, param1);
            //printAllNode(head);

        } else if (!strcmp(command, "Insert")){
            _ = scanf("%d", &param1);
            _ = scanf("%d", &param2);
            //printf("%d %s %d %d\n", i, command, param1, param2);
            insertData(&head, head, param1, param2);
            //printAllNode(head);
            
        } else if (!strcmp(command, "Reverse")){
            _ = scanf("%d", &param1);
            _ = scanf("%d", &param2);
            //printf("%d %s %d %d\n", i, command, param1, param2);
            

            reverseData(&head, head, param1, param2);
            //printAllNode(head);

        } else if (!strcmp(command, "Query")){
            _ = scanf("%d", &param1);
            _ = scanf("%d", &param2);
            _ = scanf("%d", &param3);
            //printf("%d %s %d %d %d\n", i, command, param1, param2, param3);

            kmin = queryKLEAST(head, param1, param2, param3);
            printf("%d\n", kmin);
        }
    }
    return 0;
}