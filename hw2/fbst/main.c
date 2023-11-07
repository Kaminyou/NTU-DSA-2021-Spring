// Reference
// https://stackoverflow.com/questions/67109470/find-the-number-of-nodes-in-a-general-binary-tree-that-can-be-searched-using-bst

#include <stdio.h>

int count_available_node_by_BST(int node_id, int min, int max, int *key, int *left, int *right, int n){
    //for (int i = 0; i < n; i ++){
    //    printf("%d ",right[i]);
    //}
    //printf("\n");
    //printf("%d %d %d\n", node_id, min, max);
    if (min > max) {
        return 0;
    }
    if (node_id == -2){
        return 0;
    }
    else if (key[node_id] < min){
        //printf("%d %d\n", key[node_id], min);
        return 0 + count_available_node_by_BST(right[node_id] - 1, min, max, key, left, right, n);
    }
    else if (key[node_id] > max){
        return 0 + count_available_node_by_BST(left[node_id] - 1, min, max, key, left, right, n);
    }
    else {
        //printf("%d\n", key[node_id]);
        return 1 + count_available_node_by_BST(left[node_id] - 1, min, key[node_id], key, left, right, n) + count_available_node_by_BST(right[node_id] - 1, key[node_id], max, key, left, right, n);
    }
   
}

int main(){
    int _, n;
    int answer;

    _ = scanf("%d",&n);
    
    int key[n];
    int left[n];
    int right[n];

    for (int i=0;i<n;i++){
        _ = scanf("%d", &key[i]);
        _ = scanf("%d", &left[i]);
        _ = scanf("%d", &right[i]);
    }

    answer = count_available_node_by_BST(0, 0, 1000000000, key, left, right, n);
    
    printf("%d\n", answer);
    /*
    for (int i=1;i<n+1;i++){
        printf("%d ",key[i]);
        printf("%d ",left[i]);
        printf("%d ",right[i]);
        printf("\n");
    }
    */


    return 0;
}