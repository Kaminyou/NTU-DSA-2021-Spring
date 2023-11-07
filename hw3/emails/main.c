// reference
// https://www.geeksforgeeks.org/find-the-smallest-window-in-a-string-containing-all-characters-of-another-string/

#include <stdio.h>
#include <string.h>
#define MAXLEN 100001
#define MODULE 922337203685477

int char2int(char c) {
    if ('a' <= c && c <= 'z') return c - 'a';
    else if ('A' <= c && c <= 'Z') return c - 'A' + 26;
    return -1;
}

int min(int a, int b){
    if (a >= b) return b;
    else return a;
}

void min_winsow(char *string, char *pattern, int *start_pointer, int *len_pointer) {
 
    int pattern_count[52] = {0};
    int int_of_char_pattern;
    int out_len = MAXLEN;
    int start = 0;
    int count = 0;
    int s_len = strlen(string);
    int t_len = strlen(pattern);
    
    for (int i = 0; i < t_len; i++) {
        int_of_char_pattern = char2int(pattern[i]);
        if (pattern_count[int_of_char_pattern] == 0) count++;
        pattern_count[int_of_char_pattern]++;
    }
    int i = 0;
    int j = 0;
    while (j < s_len) {
        int_of_char_pattern = char2int(string[j]);
        pattern_count[int_of_char_pattern]--;
        if (pattern_count[int_of_char_pattern] == 0) count--;

        if (count == 0) {
            while (count == 0) {
                if (out_len > j - i + 1) {
                    out_len = min(out_len, j - i + 1);
                    start = i;
                }
                int_of_char_pattern = char2int(string[i]);
                pattern_count[int_of_char_pattern]++;
                if (pattern_count[int_of_char_pattern] > 0) count++;
                i++;
            }
        }
        j++;
    }
    *start_pointer = start;
    *len_pointer = out_len;
}

void print_string(char *string, int string_len){
    for (int i = 0; i < string_len; i++){
        printf("%c", string[i]);
    }
}

void divide_string(char *string, int string_len){
    if (string_len == 1){
        print_string(string, string_len);
        return;
    }
    int front_pointer = 0;
    int end_pointer = string_len - 1;
    int max_loop = string_len / 2;

    long long int front_checker = 0;
    long long int end_checker = 0;
    long long int power = 1;

    int front_now;
    int end_now;

    int same_flag = 0;
    
    // check if there is equal one
    for (int i = 0; i < max_loop; i++){
        front_now = char2int(string[front_pointer]);
        end_now = char2int(string[end_pointer]);

        front_checker = (front_checker * 52 + front_now) % MODULE;
        end_checker = (end_checker + end_now * power) % MODULE;
        if (front_checker == end_checker){
            same_flag = 1;
            for (int j = 0; j< i;j++){
                if (!(string[j] == string[end_pointer + j])){
                    same_flag = 0;
                    break;
                }
            }
            if (same_flag){
                break;
            }
        }
        front_pointer += 1;
        end_pointer -= 1;
        power = (power * 52) % MODULE;
    }

    if (same_flag){
        if ((end_pointer - front_pointer) == 1){
            print_string(string, max_loop);
            printf("|");
            print_string(&string[end_pointer], max_loop);
        }
        else {
            print_string(string, front_pointer + 1);
            printf("|");
            divide_string(&string[front_pointer + 1], (end_pointer - front_pointer - 1));
            printf("|");
            print_string(&string[end_pointer], front_pointer + 1);
        }

    } else {
        print_string(string, string_len);
    }


}

void each_loop(){
    int _;
    char string[MAXLEN];
    char pattern[MAXLEN];
    int string_len = 0;
    int start_pointer;
    int len_pointer;

    _ = scanf("%s", string);
    _ = scanf("%s", pattern);
    min_winsow(string, pattern, &start_pointer, &len_pointer);
    if (len_pointer != MAXLEN){
        int diff = len_pointer - 1;
        for (int i = start_pointer; i <MAXLEN;i++){
            string[i] = string[i+diff+1];
            if (string[i+diff+1] == '\0'){
                break;
            }
        }
    }
    string_len = strlen(string);
    divide_string(string, string_len);
}

int main(){
    int _, n;
    _ = scanf("%d",&n);
    for (int i = 0; i < n; i++){
        each_loop();
        printf("\n");
    }
    return 0;
}