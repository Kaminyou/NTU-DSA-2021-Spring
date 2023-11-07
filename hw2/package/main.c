// Reference
// https://www.geeksforgeeks.org/implementation-binomial-heap-set-2/
// https://www.coders-hub.com/2013/04/c-code-for-binomial-heap-tree.html
// https://github.com/dpwoo/binomial_heap
// https://github.com/onnple/leftistheap
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#define MAXN 100001
#define MAXL 100001
#define MAXO 200002

typedef struct NODE{
    int key;
    int dist;
    struct NODE *left;
    struct NODE *right;
} Node_t;

typedef struct LH{
    int nodes;
    Node_t *root;
} LH_t;

typedef struct QUEUENODE {
    int data;
    struct QUEUENODE *prev;
    struct QUEUENODE *next;
} QN_t;

typedef struct QUEUE {
    struct QUEUENODE *front;
    struct QUEUENODE *back;
    int nodes;
} Q_t;

typedef struct LINE {
    struct LH *h;
    struct QUEUE *q;
    int length;
    int max;
    int first;
    int last;
} Line_t;

void swapCH(Node_t *node);
LH_t *initLH();
Node_t *pushLH(LH_t *h, int key);
Node_t *maxLH(LH_t *h);
Node_t *popLH(LH_t *h);
Node_t *merge(Node_t *h1, Node_t *h2);
Node_t *merge_node(Node_t *h1, Node_t *h2);
Q_t *initQueue();
QN_t *initQueueNode(int data);
QN_t *enQueue(Q_t *q, int data);
QN_t *deQueue(Q_t *q);
QN_t *seeDeQueue(Q_t *q);
QN_t *popQueue(Q_t *q);
QN_t *seePopQueue(Q_t *q);
Q_t *unionQueue(Q_t *q1, Q_t *q2);
QN_t *deleteQueue(Q_t *q, QN_t *qn);
void traverseQueue(Q_t *q);
void RtraverseQueue(Q_t *q);
Line_t *initLine();
void insertLine(Line_t *l, int key, Node_t *heap_node_table[], QN_t *queue_node_table[]);
Line_t *unionLine(Line_t *l1, Line_t *l2);
void updateLine(Line_t *l, int *already_pop);
int extractMaxAndUpdataLine(Line_t *l, int value, QN_t *queue_node_table[], Node_t *heap_node_table[], int *already_pop);
int dequeueAndUpdateLine(Line_t *l, int value, Node_t *heap_node_table[], int *already_pop);
int popAndUpdateLine(Line_t *l, int value, Node_t *heap_node_table[], int *already_pop);
void printLine(Line_t *l);
int line_to_line_path_compression(int *line_map_to_line, int original_line);
int checkPossible(int *operation_a, int *operation_b, int *operation_c, int *arrangement, int N, int O, int L);

LH_t *initLH(){
    LH_t *h = (LH_t*)malloc(sizeof(LH_t));
    h->nodes = 0;
    h->root = NULL;
    return h;
}

int distance(Node_t *node){
    if(node == NULL)
        return -1;
    return node->dist;
}

void swapCH(Node_t *node){
    Node_t *temp = node->left;
    node->left = node->right;
    node->right = temp;
}

Node_t *merge(Node_t *h1, Node_t *h2){
    if(h1->left == NULL)
        h1->left = h2;
    else{
        h1->right = merge_node(h1->right, h2);
        if(h1->left->dist < h1->right->dist)
            swapCH(h1);
        h1->dist = 1 + distance(h1->right);
    }
    return h1;
}

Node_t *merge_node(Node_t *h1, Node_t *h2){
    if(h1 == NULL)
        return h2;
    if(h2 == NULL)
        return h1;
    if(h1->key > h2->key)
        return merge(h1, h2);
    else
        return merge(h2, h1);
}

Node_t *pushLH(LH_t *h, int key){
    if(h == NULL)
        return NULL;
    Node_t *node = (Node_t*)malloc(sizeof(Node_t));
    node->key = key;
    node->left = node->right = NULL;
    node->dist = 1 + distance(node->right);
    h->root = merge_node(h->root, node);
    h->nodes += 1;
    return node;
}

Node_t *maxLH(LH_t *h){
    if(h == NULL || h->nodes == 0)
        return NULL;
    return h->root;
}

Node_t *popLH(LH_t *h){
    if(h == NULL || h->nodes == 0)
        return 0;
    Node_t *root = h->root;
    Node_t *left = root->left;
    Node_t *right = root->right;
    h->nodes--;
    h->root = merge_node(left, right);
    return root;
}

Q_t *initQueue(){
    Q_t *new_q = (Q_t*)malloc(sizeof(Q_t));
    new_q->nodes = 0;
    new_q->front = NULL;
    new_q->back = NULL;
    return new_q;
}

QN_t *initQueueNode(int data){
    QN_t *qn = (QN_t*)malloc(sizeof(QN_t));
    qn->data = data;
    qn->prev = NULL;
    qn->next = NULL;
    return qn;
}

QN_t *enQueue(Q_t *q, int data){
    QN_t *qn = initQueueNode(data);
    // a new one
    if (q->front == NULL && q->back == NULL){
        q->front = qn;
        q->back = qn;
    }
    else {
        q->back->next = qn;
        qn->prev = q->back;
        q->back = qn;
    }
    q->nodes += 1;
    return qn;
}

QN_t *deQueue(Q_t *q){
    if (q->front == NULL){
        return NULL;
    }
    QN_t *result;
    // only one node
    if (q->nodes == 1){
        result = q->front;
        q->front = NULL;
        q->back = NULL;
    }
    else {
        result = q->front;
        q->front->next->prev = NULL;
        q->front = q->front->next;
    }
    q->nodes -= 1;
    return result;
}

QN_t *seeDeQueue(Q_t *q){
    if (q->front == NULL){
        return NULL;
    }
    QN_t *result;
    result = q->front;
    return result;
}

QN_t *popQueue(Q_t *q){
    if (q->back == NULL){
        return NULL;
    }
    QN_t *result;
    // only one node
    if (q->nodes == 1){
        result = q->back;
        q->front = NULL;
        q->back = NULL;
    }
    else {
        result = q->back;
        q->back->prev->next = NULL;
        q->back = q->back->prev;
    }
    q->nodes -= 1;
    return result;
}

QN_t *seePopQueue(Q_t *q){
    if (q->back == NULL){
        return NULL;
    }
    QN_t *result;
    result = q->back;
    return result;
}

Q_t *unionQueue(Q_t *q1, Q_t *q2){
    // q2 to q1
    if (q1->nodes == 0){
        return q2;
    }
    if (q2->nodes == 0){
        return q1;
    }
    q1->back->next = q2->front;
    q2->front->prev = q1->back;
    q1->back = q2->back;
    q1->nodes += q2->nodes; 
    return q1;
}

QN_t *deleteQueue(Q_t *q, QN_t *qn){
    assert(q->nodes != 0);
    // if only one node
    if (q->nodes == 1){
        q->front = NULL;
        q->back = NULL;
    }

    // the node is the first
    else if (q->front == qn){
        q->front->next->prev = NULL;
        q->front = q->front->next;
    }
    // the node is the last
    else if (q->back == qn){
        q->back->prev->next = NULL;
        q->back = q->back->prev;
    }

    // the node is in the middle
    else {
        QN_t *prev_n, *next_n;
        prev_n = qn->prev;
        next_n = qn->next;
        prev_n->next = next_n;
        next_n->prev = prev_n;
    }
    q->nodes -= 1;
    return qn;
}

void traverseQueue(Q_t *q){
    if (q->front == NULL){
        printf("empty queue\n");
        return;
    }
    QN_t *node = q->front;
    while(node->next != NULL){
        printf("%d ", node->data);
        node = node->next;
    }
    printf("%d ", node->data);
    printf("\n");
}
void RtraverseQueue(Q_t *q){
    if (q->back == NULL){
        printf("empty queue\n");
        return;
    }
    QN_t *node = q->back;
    while(node->prev != NULL){
        printf("%d ", node->data);
        node = node->prev;
    }
    printf("%d ", node->data);
    printf("\n");
}

Line_t *initLine(){
    Line_t *l = (Line_t*)malloc(sizeof(Line_t));
    LH_t *h = initLH();
    Q_t *q = initQueue();
    l->h = h;
    l->q = q;
    l->length = 0;
    l->max = INT_MIN;
    l->first = -1;
    l->last = -1;
    return l;
}

void insertLine(Line_t *l, int key, Node_t *heap_node_table[], QN_t *queue_node_table[]){
    Node_t *node = pushLH(l->h, key);
    QN_t *qn = enQueue(l->q, key);

    heap_node_table[key] = node;
    queue_node_table[key] = qn;

    if (key > l->max){
        l->max = key;
    }
    l->last = key;
    if (l->length == 0){
        l->first = key;
    }
    l->length += 1;
}

Line_t *unionLine(Line_t *l1, Line_t *l2){
    // broken l1, shift to l2
    // deal with bh
    if (l1->length == 0){
        return l2;
    }
    if (l2->length == 0){
        return l1;
    }
    l2->h->root = merge_node(l2->h->root, l1->h->root);
    l2->h->nodes += l1->h->nodes;
    l2->q = unionQueue(l2->q, l1->q);
    
    if (l1->max > l2->max){
        l2->max = l1->max;
    }
    l2->last = l1->last;
    l2->length = l1->length + l2->length;
    return l2;
}

void updateLine(Line_t *l, int *already_pop){
    Node_t *node;
    QN_t *qn;
    if (l->length != 0){
        node = maxLH(l->h);
        while (already_pop[node->key] == 1){
            node = popLH(l->h);
            node = maxLH(l->h);
        }
        
        l->max = node->key;
        
        qn = seeDeQueue(l->q);
        l->first = qn->data;
        qn = seePopQueue(l->q);
        l->last = qn->data;
        
    } else {
        l->max = INT_MIN;
        l->first = -1;
        l->last = -1;
        l->h->root = NULL;
        l->h->nodes = 0;
    }
}

int extractMaxAndUpdataLine(Line_t *l, int value, QN_t *queue_node_table[], Node_t *heap_node_table[], int *already_pop){

    Node_t *node = popLH(l->h);
    assert(node->key == value);

    QN_t *qn = queue_node_table[value];
    qn = deleteQueue(l->q, qn);
    assert(qn->data == value);

    l->length -= 1;
    
    updateLine(l, already_pop);
    
    return value;
}
int dequeueAndUpdateLine(Line_t *l, int value, Node_t *heap_node_table[], int *already_pop){
    QN_t *qn = deQueue(l->q);
    assert(qn->data == value);

    Node_t *node = heap_node_table[value];
    assert(node->key == value);
    already_pop[value] = 1;
    

    l->length -= 1;
    if (l->length != 0){
        qn = seeDeQueue(l->q);
        l->first = qn->data;
        qn = seePopQueue(l->q);
        l->last = qn->data;
    } else {
        l->max = INT_MIN;
        l->first = -1;
        l->last = -1;
    }
    return value;
}

int popAndUpdateLine(Line_t *l, int value, Node_t *heap_node_table[], int *already_pop){
    QN_t *qn = popQueue(l->q);
    assert(qn->data == value);

    Node_t *node = heap_node_table[value];
    assert(node->key == value);
    already_pop[value] = 1;
    
    l->length -= 1;
    if (l->length != 0){
        qn = seePopQueue(l->q);
        l->last = qn->data;
    } else {
        l->max = INT_MIN;
        l->first = -1;
        l->last = -1;
    }
    return value;
}

void printLine(Line_t *l){
    printf("first %d || last %d || max %d",l->first, l->last, l->max);
    printf("\n");
    printf("l->length %d || l->q->nodes %d || l->h->nodes %d\n", l->length, l->q->nodes, l->h->nodes);
    assert(l->length == l->q->nodes);    

    QN_t *node = l->q->front;
    while(node != NULL){
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");

    node = l->q->back;
    while(node != NULL){
        printf("%d ", node->data);
        node = node->prev;
    }
    printf("\n");

}


int line_to_line_path_compression(int *line_map_to_line, int original_line){
    if (original_line != line_map_to_line[original_line]){
        line_map_to_line[original_line] = line_to_line_path_compression(line_map_to_line, line_map_to_line[original_line]);
    }
    return line_map_to_line[original_line];
}

// 1->possible 0->impossible
int checkPossible(int *operation_a, int *operation_b, int *operation_c, int *arrangement, int N, int O, int L) {
    int already_in[100001] = {0};
    int already_pop[100001] = {0};

    Node_t *heap_node_table[100001];
    QN_t *queue_node_table[100001];

    int arrangement_pos = 0;
    int operation_pos = 0;
    int num_obtain, num_to_check;
    int pass_flag = 0;

    int line_id, temp_id, ori_line_id;
    
    Line_t *line_set[L];
    Line_t *line_temp;
    int value_map_to_line[100001];
    int line_map_to_line[L];
    for (int i=0;i<L;i++){
        line_set[i] = initLine();
        line_map_to_line[i] = i;
    }

    for (int i=0;i<O;i++){
        if (operation_a[i] == 1){
            value_map_to_line[operation_b[i]] = operation_c[i];
        }
    }
    
    while (arrangement_pos < N){
        // match, do nothing
        if (arrangement[arrangement_pos] == operation_b[operation_pos] && operation_a[operation_pos] == 1){
            already_in[operation_b[operation_pos]] = 1;
            arrangement_pos += 1;
            operation_pos += 1;
            
        } 
        // already in, check possibility
        else if (already_in[arrangement[arrangement_pos]] == 1){
            pass_flag = 0;
            line_id = line_to_line_path_compression(line_map_to_line, value_map_to_line[arrangement[arrangement_pos]]);   
            if (1){
                //check max

                if (line_set[line_id]->max == arrangement[arrangement_pos]){
                    extractMaxAndUpdataLine(line_set[line_id], line_set[line_id]->max, queue_node_table, heap_node_table, already_pop);
                    pass_flag = 1;
                }    
    
                // dequeue from first
                if (!pass_flag){
                    if (line_set[line_id]->first == arrangement[arrangement_pos]){
                        dequeueAndUpdateLine(line_set[line_id], line_set[line_id]->first, heap_node_table, already_pop);
                        pass_flag = 1;
                    }    
                }

                // pop from last
                if (!pass_flag){
                    if (line_set[line_id]->last == arrangement[arrangement_pos]){
                        popAndUpdateLine(line_set[line_id], line_set[line_id]->last, heap_node_table, already_pop);
                        pass_flag = 1;
                    }  
                }
            }
            if (!pass_flag){
                return 0;
            }
            
            arrangement_pos += 1;
        }
        // do operation
        else {
            // insert
            if (operation_a[operation_pos] == 1){
                insertLine(line_set[operation_c[operation_pos]], operation_b[operation_pos], heap_node_table, queue_node_table);
                already_in[operation_b[operation_pos]] = 1;
                operation_pos += 1;
            }
            // merge
            else if (operation_a[operation_pos] == 2){
                line_temp = unionLine(line_set[operation_b[operation_pos]], line_set[operation_c[operation_pos]]);
                line_set[operation_b[operation_pos]] = line_temp;
                line_set[operation_c[operation_pos]] = line_temp;
                line_map_to_line[operation_b[operation_pos]] = operation_c[operation_pos];

                operation_pos += 1;
            }
        }
    }
    return 1;
}


int main(){
    int _, t;
    int N, O, L;
    int possible = 0;
    int operation_a[MAXO]; // push 1 merge 2
    int operation_b[MAXO];
    int operation_c[MAXO];
    int arrangement[MAXN];
    char command[5];
    _ = scanf("%d", &t);
    for (int i = 0; i < t;i++){
        _ = scanf("%d", &N);
        _ = scanf("%d", &O);
        _ = scanf("%d", &L);
        for (int j = 0; j < O; j++){
            _ = scanf("%s", command);
            if (!strcmp(command, "push")) {
                operation_a[j] = 1;
            } else {
                operation_a[j] = 2;
            }
            _ = scanf("%d", &operation_b[j]);
            _ = scanf("%d", &operation_c[j]);
        }
        for (int j = 0; j < N;j++){
            _ = scanf("%d", &arrangement[j]);
        }

        possible = checkPossible(operation_a, operation_b, operation_c, arrangement, N, O, L);
        if (possible){
            printf("possible\n");
        } else {
            printf("impossible\n");
        }

    }
    return 0;

}