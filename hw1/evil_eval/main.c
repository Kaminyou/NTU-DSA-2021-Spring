#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXCHRINLINE 100000

double expression_calculator(char expression[], int *pos_now);

typedef struct stack{
    int top;
    int max_size;
    double *data;
} stack_t;

void stack_create(stack_t *st, int max_size);
void push_double(stack_t *st, double number);
double pop_double(stack_t *st);
int is_empty(stack_t *st);
double stack_sum(stack_t *st);

void stack_create(stack_t *st, int max_size){
    st->top = -1;
    st->max_size = max_size - 1;
    st->data = (double *)malloc(sizeof(double)*max_size);
}

void push_double(stack_t *st, double number){
    st->top++;
    st->data[st->top] = number;
}

double pop_double(stack_t *st){
    double out = st->data[st->top];
    st->top--;
    return out;
}

int is_empty(stack_t *st){
    if (st->top == -1){
        return 1;
    }
    else {
        return 0;
    }
}

void stack_print(stack_t *st) {
    if (st->top == -1){
        printf("empty \n");
        return;
    }
    for (int i = 0; i <= st->top; i++){
        printf("%f ", st->data[i]);
    }
    printf("\n");
}

double stack_sum(stack_t *st) {
    double out = 0;
    if (st->top == -1){
        return out;
    }
    for (int i = 0; i <= st->top; i++){
        out += st->data[i];
    }
    return out;
}

double expression_calculator(char expression[], int *pos_now){
    int expression_length = strlen(expression);
    if (expression[*pos_now] == '\n'){
        return 0.0;
    }
    stack_t *s = (stack_t *)malloc(sizeof(stack_t));
    stack_create(s, 10);
    char operator = '+';
    double num = 0.0;
    while (*pos_now <= expression_length){
        char chr = expression[*pos_now];
        *pos_now = *pos_now + 1;
        
        if (isdigit(chr) != 0) {
            int chr_int = chr - '0';
            num = num * 10 + chr_int;
        }
        
        if (chr == '('){
            num = expression_calculator(expression, pos_now);
        }
        
        double temp;
        if (chr == '\n' || (chr == '+' || chr == '-' || chr == '*' || chr == '/' || chr == ')' )) {
            switch(operator){
                case '+':
                    push_double(s, num);
                break; 
                
                case '-':
                    push_double(s, -num);
                break;

                case '*':
                    //printf("multiply\n");
                    temp = pop_double(s) * num;
                    push_double(s, temp);
                break;

                case '/':
                    temp = pop_double(s) / num;
                    push_double(s, temp);
                break;
            }

            operator = chr;
            num = 0.0;
            if (chr == ')'){
                break;
            }
        }  
        //stack_print(s);
    }
    
    double out = stack_sum(s);
    return out;
    //printf("%d\n", expression_length);
    //for (int i=0; i < expression_length; i++){
    //   printf("%c", expression[i]);
    //}
}

int main(){
    char line[MAXCHRINLINE];
    while (fgets(line, MAXCHRINLINE, stdin))
    {
        int loc_now = 0;
        int* loc_now_pointer = &loc_now;
        double result;
        result = expression_calculator(line, loc_now_pointer);
        printf("%.12lf\n", result);
        
    }

    /*stack_t *s=(stack_t *)malloc(sizeof(stack_t));

    printf("Create a stack which size is 5.\n");
    stack_create(s, 5);
    printf("Push:1,2,3,4,5:\n");
    push_double(s, 1);
    push_double(s, 2);
    push_double(s, 3);
    push_double(s, 4);
    push_double(s, 5);
    double a = pop_double(s);
    printf("out %lf\n", a);
    double b = stack_sum(s);
    printf("%f\n", b);
    double c = 1.0/7.0;
    printf("%.12lf\n", c);
    */

    return 0;
}