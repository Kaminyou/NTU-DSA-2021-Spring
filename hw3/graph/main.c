// Reference
// No
#include <stdio.h>
#define MAXVERTEX 100001
#define MAXNUMSUM 500000
void delete_element(int i, int *adjacency_list_num, int *adjacency_list_start, int *value_array, int *adjacency_list_sum, int *out_a_array, int *out_b_array, int *out_pos, int *success);

void delete_element(int i, int *adjacency_list_num, int *adjacency_list_start, int *value_array, int *adjacency_list_sum, int *out_a_array, int *out_b_array, int *out_pos, int *success){
    int target;
    if (adjacency_list_num[i] != 0){
        target = value_array[adjacency_list_start[i]];
        if (value_array[adjacency_list_start[target - 1]] == i + 1){
            adjacency_list_start[i] += 1;
            adjacency_list_start[target-1] += 1;
            *adjacency_list_sum -= 2;
            adjacency_list_num[i] -= 1;
            adjacency_list_num[target-1] -= 1;
            out_a_array[*out_pos] = i + 1;
            out_b_array[*out_pos] = target;
            *out_pos += 1;
            *success = 1;

            delete_element(i, adjacency_list_num, adjacency_list_start, value_array, adjacency_list_sum, out_a_array, out_b_array, out_pos, success);
            delete_element(target - 1, adjacency_list_num, adjacency_list_start, value_array, adjacency_list_sum, out_a_array, out_b_array, out_pos, success);

        } else{
            return;
        }
    } else {
        return;
    }
}

int main(){
    int _, n;
    int num, value;
    int adjacency_list_num[MAXVERTEX];
    int adjacency_list_start[MAXVERTEX];
    int value_array[MAXNUMSUM];
    int now_pos = 0;
    int out_a_array[MAXNUMSUM];
    int out_b_array[MAXNUMSUM];
    int out_pos = 0;

    _ = scanf("%d", &n);
    for (int i = 0; i < n; i++){
        _ = scanf("%d", &num);
        adjacency_list_num[i] = num;
        adjacency_list_start[i] = now_pos;
        for (int j = 0; j < num;j++){
            _ = scanf("%d", &value);
            value_array[now_pos] = value;
            now_pos += 1;
        }
    }

    int counting = 0;
    int quick_while = 0;
    int success = 0;
    while (now_pos != 0){
        success = 0;
        for (int i = 0; i < n; i++){
            delete_element(i, adjacency_list_num, adjacency_list_start, value_array, &now_pos, out_a_array, out_b_array, &out_pos, &success);
            if (!success){
                counting += 1;
            }
            if (counting > n){
                printf("No\n");
                quick_while = 1;
                break;
            }   
        }
        if (quick_while){
            break;
        }
    }

    if (now_pos == 0){
        printf("Yes\n");
        for (int i = 0; i < out_pos;i++){
            printf("%d %d\n", out_a_array[i], out_b_array[i]);
        }
    }

    return 0;
}