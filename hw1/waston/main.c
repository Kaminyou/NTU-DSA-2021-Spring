//https://www.geeksforgeeks.org/xor-linked-list-remove-last-node-of-the-linked-list/
//https://www.geeksforgeeks.org/xor-linked-list-reversal-of-a-list/
//https://www.geeksforgeeks.org/xor-linked-list-remove-first-node-of-the-linked-list/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

typedef struct node {
    int data;
    struct node *neighbor;
} Node_t;

Node_t* node_create(int data){
    Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));
    new_node->data = data;
    new_node->neighbor = NULL;
    return new_node;
}

Node_t* node_xor(Node_t* node_a, Node_t* node_b){
    return (Node_t*)((uintptr_t)(node_a) ^ (uintptr_t)(node_b));
}

void node_insert_begin(Node_t** head, Node_t** tail, int data){
    Node_t* new_node = node_create(data);

    if (*head == NULL){
        new_node->neighbor = node_xor(NULL, NULL);
        *head = new_node;
        *tail = new_node;
        return;
    } else {
        Node_t* current_node = *head;
        Node_t* previous_node = NULL;
        current_node->neighbor = node_xor(new_node, node_xor(NULL, current_node->neighbor));
        new_node->neighbor = node_xor(NULL, current_node);
        *head = new_node;
        return;
    }
}

void node_delete_begin(Node_t** head, Node_t** tail){
    if (*head == NULL){
        return;
    } else {
        Node_t* current_node = *head;
        *head = node_xor(NULL, current_node->neighbor);
        if (*head != NULL) { 
            (*head)->neighbor = node_xor(NULL, node_xor(current_node, (*head)->neighbor)); 
        } else if (*head == NULL) { 
            *tail = NULL;
        }
        free(current_node); 
    }
}

void node_reverse(Node_t** head, Node_t** tail){
    Node_t* temp = *head;
    *head = *tail;
    *tail = temp;
}

void node_print(Node_t** head){ 
    Node_t* previous_node = NULL;
    Node_t* current_node = *head;
    Node_t* next_node = NULL;

    while (current_node != NULL) { 
        printf("%d ", current_node->data); 
        next_node = node_xor(previous_node, current_node->neighbor); 
        previous_node = current_node; 
        current_node = next_node; 
    } 
    printf("\n");
} 

void node_migrate(Node_t** head_a, Node_t** tail_a, Node_t** head_b, Node_t** tail_b){
    // a empty
    if ((*head_a==NULL) && (*tail_a==NULL)){
        return;
    }
    // b empty
    if ((*head_b==NULL) && (*head_b==NULL)){
        *head_b = *tail_a;
        *tail_b = *head_a;
        *tail_a = NULL;
        *head_a = NULL;
        return;
    }

    Node_t* head_a_temp = *head_a;
    Node_t* head_b_temp = *head_b;

    (*head_a)->neighbor = node_xor(node_xor(head_a_temp->neighbor, NULL), head_b_temp);
    (*head_b)->neighbor = node_xor(node_xor(head_b_temp->neighbor, NULL), head_a_temp);
    *head_b = *tail_a;
    *tail_a = NULL;
    *head_a = NULL;
}

int main() { 
    int k, n;
    int _;
    char command[15];
    int param1, param2;
    
    _ = scanf("%d",&k);
    _ = scanf("%d",&n);

    Node_t *rail_heads[k];
    for (int i =0;i<k;i++){
        rail_heads[i] = NULL;
    }
    Node_t *rail_tails[k];
    for (int i =0;i<k;i++){
        rail_tails[i] = NULL;
    }

    for (int i = 0; i < n; i++){
        _ = scanf("%s", command);

        if (!strcmp(command, "enter")) {
            _ = scanf("%d", &param1);
            _ = scanf("%d", &param2);
            node_insert_begin(&rail_heads[param1], &rail_tails[param1], param2);

        } else if (!strcmp(command, "leave")){
            _ = scanf("%d", &param1);
            node_delete_begin(&rail_heads[param1], &rail_tails[param1]); 

        } else if (!strcmp(command, "migrate")){
            _ = scanf("%d", &param1);
            _ = scanf("%d", &param2);
            node_migrate(&rail_heads[param1], &rail_tails[param1], &rail_heads[param2], &rail_tails[param2]);
        } 
    }
    for (int i =0;i<k;i++){
        node_print(&rail_tails[i]);
    }

    return (0); 
} 