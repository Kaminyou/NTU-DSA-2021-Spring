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
#define MAXELEMENT 2000
#define KEEPELEMENT 1000
#define INITLIMIT 1250 // should must be <= MAXELEMENT - 1
#define REVERSESTACKNUM 4000

typedef struct node {
    int data[MAXELEMENT];
    int max_len;
    int now_len;
    int max;
    int reversed;
    struct node *next;
    struct node *prev;
} Node_t;


Node_t* createNode(){
    Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));
    new_node->max_len = MAXELEMENT;
    new_node->now_len = 0;
    new_node->reversed = 0;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node; 
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
        printf("|| max=%d", head->max);
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
        int max = INT_MIN;
        if (end > total){
            end = total;
        }
        for (int j = start; j< end; j++){
            new_node->data[j - start] = data_array[j];
            if (data_array[j] > max){
                max = data_array[j];
            }
        }
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
        int max = INT_MIN;
        if (end > total){
            end = total;
        }
        for (int j = start; j< end; j++){
            _ = scanf("%d", &temp);
            new_node->data[j - start] = temp;
            if (temp > max){
                max = temp;
            }
        }
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
        int max = INT_MIN;
        for (int i = 0; i < move_element_num; i ++) {
            new_node->data[i] = fullNode->data[KEEPELEMENT+i];
            if (fullNode->data[KEEPELEMENT+i] > max){
                max = fullNode->data[KEEPELEMENT+i];
            }
        }
        new_node->now_len = move_element_num;
        new_node->max = max;
        new_node->reversed = fullNode->reversed;

        max = INT_MIN;
        for (int i = 0; i < KEEPELEMENT; i ++) {
            if (fullNode->data[i] > max){
                max = fullNode->data[i];
            }
        }
        fullNode->now_len = KEEPELEMENT;
        fullNode->max = max;

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
    if (node->max < x) {
        node->max = x;
    }
}

void deleteUpdateMinMax(Node_t *node, int deleted_x){
    int data_num = node->now_len;
    if (node->max == deleted_x) {
        int max = INT_MIN;
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
    int max = INT_MIN;
    for (int k =0; k < data_num; k++){
        if (node->data[k] > max){
            max = node->data[k];
        }
    }
    node->max = max;
}

void insertData(Node_t **head_p, Node_t *head, int i, int x, int *total_len){
    *total_len += 1;
    // if there is no node existing
    Node_t *prev_N;
    if (head == NULL){
        Node_t *new_node = createNode();
        *head_p = new_node;
        new_node->data[0] = x;
        new_node->now_len = 1;
        new_node->max = x;
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

void deleteData(Node_t **head_p, Node_t *head, int i, int *total_len){
    *total_len -= 1;
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
    
    max = INT_MIN;
    if (!node->reversed){
        for (int k = 0; k < num_to_reverse; k++){
            start_node->data[k] = node->data[l_index+k];

            if (node->data[l_index+k] > max){
                max = node->data[l_index+k];
            }
        }
    } else {
        for (int k = 0; k < num_to_reverse; k++){
            start_node->data[k] = node->data[k];

            if (node->data[k] > max){
                max = node->data[k];
            }
        }
        for (int k = 0; k < data_num - num_to_reverse; k++){
            node->data[k] = node->data[num_to_reverse + k]; //shift to front
        }
    }
    
    start_node->max = max;
    start_node->reversed = !node->reversed;

    node->now_len -= num_to_reverse;
    start_node->now_len = num_to_reverse;
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
            max = INT_MIN;
            if (!node->reversed){
                for (int k = 0; k < num_to_reverse; k++){
                    end_node->data[k] = node->data[k];
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
                    if (end_node->data[k] > max){
                        max = end_node->data[k];
                    }
                }
            }
            node->now_len -= num_to_reverse;
            end_node->now_len = num_to_reverse;
            end_node->max = max;
            end_node->reversed = !node->reversed;
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

int getMaxInSegData(Node_t *node, int l_index, int r_index){
    int max = INT_MIN;
    int num_to_count = r_index - l_index + 1;
    if (!node->reversed){
        for (int k = 0; k < num_to_count; k++){
            if (node->data[l_index + k] > max) {
                max = node->data[l_index + k];
            }
        }
    }
    else {
        int reverse_l_index = node->now_len - r_index - 1;
        int reverse_r_index = node->now_len - l_index - 1;
        for (int k = 0; k < num_to_count; k++){
            if (node->data[reverse_l_index + k] > max) {
                max = node->data[reverse_l_index + k];
            }
        }
    }
    return max;
}

int getMaxInCrossData(Node_t *node, int l_index, int r_index){
    int max = INT_MIN;
    int num_to_count;
    int cum = 0;
    int data_num = node->now_len;
    num_to_count = data_num - l_index;
    if (!node->reversed){
        for (int k = 0; k < num_to_count; k++){
            if (node->data[l_index+k] > max){
                max = node->data[l_index+k];
            }
        }
    } else {
        for (int k = 0; k < num_to_count; k++){
            if (node->data[k] > max){
                max = node->data[k];
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
            if (node->max > max){
                max = node->max;
            }
            break;

        } else if (r_index < cum + data_num - 1){
            num_to_count = r_index - cum + 1;
            if (!node->reversed){
                for (int k = 0; k < num_to_count; k++){
                    if (node->data[k] > max){
                        max = node->data[k];
                    }
                }
            } else {
                for (int k = 0; k < num_to_count; k++){
                    if (node->data[data_num - num_to_count + k] > max){
                        max = node->data[data_num - num_to_count + k];
                    }
                }
            }

            break;
        } else {
            if (node->max > max){
                max = node->max;
            }

            node = node->next;
            cum += data_num;
        }
    }
    return max;
}


int queryMax(Node_t *head, int l, int r){
    int l_index = l - 1;
    int r_index = r - 1;
    int cum = 0;
    int data_num = 0;
    int l_boundary, r_boundary;
    int maximum;

    while (head != NULL){
        data_num = head->now_len;
        l_boundary = cum;
        r_boundary = cum + data_num; //not included
        // only in one segment
        if ((l_boundary <= l_index) && (r_boundary > r_index)){
            
            maximum = getMaxInSegData(head, l_index - l_boundary, r_index - l_boundary);
            return maximum;
        }
        // step into one segment
        else if ((l_index - l_boundary >= 0) &&(l_index - l_boundary <= data_num - 1)) {
            maximum = getMaxInCrossData(head, l_index - l_boundary, r_index - l_boundary);
            return maximum;
        }
        
        cum += data_num;
        head = head->next;
        }
}


int getMaxIdxInSegData(Node_t *node, int l_index, int r_index){
    int max = INT_MIN;
    int idx;
    int num_to_count = r_index - l_index + 1;
    if (!node->reversed){
        for (int k = 0; k < num_to_count; k++){
            if (node->data[l_index + k] > max) {
                max = node->data[l_index + k];
                idx = l_index + k;
            }
        }
    }
    else {
        int reverse_l_index = node->now_len - r_index - 1;
        int reverse_r_index = node->now_len - l_index - 1;
        for (int k = 0; k < num_to_count; k++){
            if (node->data[reverse_l_index + k] > max) {
                max = node->data[reverse_l_index + k];
                idx = reverse_l_index + k;
            }
        }
        idx = node->now_len - idx - 1;
    }
    return idx;
}

int getMaxIdxInCrossData(Node_t *node, int l_index, int r_index, int max_to_find){
    int num_to_count;
    int cum = 0;
    int idx;
    int data_num = node->now_len;
    num_to_count = data_num - l_index;
    if (!node->reversed){
        for (int k = 0; k < num_to_count; k++){
            if (node->data[l_index+k] == max_to_find){
                idx = l_index+k;
                return idx;
            }
        }
    } else {
        for (int k = 0; k < num_to_count; k++){
            if (node->data[k] ==max_to_find){
                idx = k;
                return data_num - idx - 1;
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
            for (int k = 0; k < data_num; k++){
                if (node->data[k] == max_to_find){
                    idx = k;
                    break;
                }
            }
            if (!node->reversed) return cum + idx;
            else return cum + (data_num - idx - 1);

            break;

        } else if (r_index < cum + data_num - 1){
            num_to_count = r_index - cum + 1;
            if (!node->reversed){
                for (int k = 0; k < num_to_count; k++){
                    if (node->data[k] == max_to_find){
                        return cum + k;
                    }
                }
            } else {
                for (int k = 0; k < num_to_count; k++){
                    if (node->data[data_num - num_to_count + k] == max_to_find){
                        idx = data_num - num_to_count + k;
                        return cum + (data_num - idx - 1);
                    }
                }
            }

            break;
        } else {
            if (node->max == max_to_find){
                for (int k = 0; k < data_num; k++){
                    if (node->data[k] == max_to_find){
                        idx = k;
                        break;
                    }
                }
                if (!node->reversed) return cum + idx;
                else return cum + (data_num - idx - 1);

            }

            node = node->next;
            cum += data_num;
        }
    }
}


int queryMaxIdx(Node_t *head, int l, int r, int max){
    int l_index = l - 1;
    int r_index = r - 1;
    int cum = 0;
    int data_num = 0;
    int l_boundary, r_boundary;
    int idx;

    while (head != NULL){
        data_num = head->now_len;
        l_boundary = cum;
        r_boundary = cum + data_num; //not included
        // only in one segment
        if ((l_boundary <= l_index) && (r_boundary > r_index)){
            
            idx = getMaxIdxInSegData(head, l_index - l_boundary, r_index - l_boundary);
            return cum + idx;
        }
        // step into one segment
        else if ((l_index - l_boundary >= 0) &&(l_index - l_boundary <= data_num - 1)) {
            idx = getMaxIdxInCrossData(head, l_index - l_boundary, r_index - l_boundary, max);
            return cum + idx;
        }
        
        cum += data_num;
        head = head->next;
        }
}

void increaseInSegData(Node_t *node, int l_index, int r_index, int p){
    int num_to_count = r_index - l_index + 1;
    if (!node->reversed){
        for (int k = 0; k < num_to_count; k++){
            node->data[l_index + k] += p;   
        }
        updateNodeMinMax(node);
    }
    else {
        int reverse_l_index = node->now_len - r_index - 1;
        int reverse_r_index = node->now_len - l_index - 1;
        for (int k = 0; k < num_to_count; k++){
            node->data[reverse_l_index + k] += p;   
        }
        updateNodeMinMax(node);
    }
}

void increaseInCrossData(Node_t *node, int l_index, int r_index, int p){
    int num_to_count;
    int cum = 0;
    int data_num = node->now_len;
    num_to_count = data_num - l_index;
    if (!node->reversed){
        for (int k = 0; k < num_to_count; k++){
            node->data[l_index+k] += p;
        }
    } else {
        for (int k = 0; k < num_to_count; k++){
            node->data[k] += p;
        }
    }
    updateNodeMinMax(node);
    // shift to next segment
    node = node->next;
    cum += data_num;
    // next segment

    while (1){
        data_num = node->now_len;
        if (r_index <= cum + data_num - 1){
            num_to_count = r_index - cum + 1;
            if (!node->reversed){
                for (int k = 0; k < num_to_count; k++){
                    node->data[k] += p;  
                }
                updateNodeMinMax(node);
            } else {
                for (int k = 0; k < num_to_count; k++){
                    node->data[data_num - num_to_count + k] += p;
                }
                updateNodeMinMax(node);
            }

            break;
        } else {
            for (int k = 0; k < data_num; k ++){
                node->data[k] += p;
                //updateNodeMinMax(node);
            }
            node->max += p;

            node = node->next;
            cum += data_num;
        }
    }
}


void increaseData(Node_t *head, int l, int r, int p){
    int l_index = l - 1;
    int r_index = r - 1;
    int cum = 0;
    int data_num = 0;
    int l_boundary, r_boundary;
    int maximum;

    while (head != NULL){
        data_num = head->now_len;
        l_boundary = cum;
        r_boundary = cum + data_num; //not included
        // only in one segment
        if ((l_boundary <= l_index) && (r_boundary > r_index)){
            increaseInSegData(head, l_index - l_boundary, r_index - l_boundary, p);
        }
        // step into one segment
        else if ((l_index - l_boundary >= 0) &&(l_index - l_boundary <= data_num - 1)) {
            increaseInCrossData(head, l_index - l_boundary, r_index - l_boundary, p);
        }
        cum += data_num;
        head = head->next;
    }
}

void removeData(Node_t **head_p, Node_t *head, int *total_len){
    int max, idx;
    max = queryMax(head, 1, *total_len);
    //printf("max %d\n", max);
    idx = queryMaxIdx(head, 1, *total_len, max);
    //printf("idx %d\n", idx);
    deleteData(head_p, head, idx+1, total_len);
    return;
}

int main(){
    Node_t *head = NULL;
    
    int temp, max;
    int total_len;
    int command;
    int l, r, p, k;
    int _;

    int vector_length, operation_num;
    _ = scanf("%d", &vector_length);
    _ = scanf("%d", &operation_num);
    total_len = vector_length;

    initializeSeqDataDirectly(&head, vector_length);
    //printAllNodePara(head);
    for (int i=0;i<operation_num;i++){
        _ = scanf("%d",&command);

        if (command == 1){
            _ = scanf("%d",&p);
            _ = scanf("%d",&k);
            insertData(&head, head, k+1, p, &total_len);
            //printf("insert %d %d\n", p, k);
        } 
        else if (command == 2){
            _ = scanf("%d",&k);
            deleteData(&head, head, k, &total_len);
            //printf("delete %d\n", k);

        }
        else if (command == 3){
            _ = scanf("%d",&l);
            _ = scanf("%d",&r);
            _ = scanf("%d",&p);
            increaseData(head, l, r, p);
            //printf("increase %d %d %d\n", l, r, p);

        }
        else if (command == 4){
            _ = scanf("%d",&l);
            _ = scanf("%d",&r);
            max = queryMax(head, l, r);
            //printf("query %d %d\n", l, r);
            printf("%d\n", max);
            //if (l == 57){
            //    exit(0);
            //}

        }
        else if (command == 5){
            _ = scanf("%d",&l);
            _ = scanf("%d",&r);
            reverseData(&head, head, l, r);
            //printf("reverse %d %d\n", l, r);
        }
        else if (command == 6){
            removeData(&head, head, &total_len);
            //printf("remove\n");
        }
        //printAllNodePara(head);

    }
    return 0;
}