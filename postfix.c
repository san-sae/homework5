/* postfix.c
 *
 * Data Structures, Homework #5
 * School of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 10       // stack의 최대 사이즈
#define MAX_EXPRESSION_SIZE 20  // 수식의 최대 사이즈

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
    lparen = 0, /* ( 왼쪽 괄호 */
    rparen = 9, /* ) 오른쪽 괄호*/
    times = 7,  /* * 곱셈 */
    divide = 6, /* / 나눗셈 */
    plus = 5,   /* + 덧셈 */
    minus = 4,  /* - 뺄셈 */
    operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   /* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE]; /* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];    /* postfix로 변환을 위해 필요한 스택*/
int evalStack[MAX_STACK_SIZE];        /* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;             /* postfixStack용 top */
int evalStackTop = -1;                /* evalStack용 top */
int evalResult = 0;                   /* 계산 결과를 저장 */

void postfixPush(char x);         // postfixstack에 문자를 push하는 함수
char postfixPop();                // postfixstack에서 pop하는 함수
void evalPush(int x);             // 계산 값을 스택에 push하는 함수
int evalPop();                    // 계산 값을 스택에서 pop하는 함수
void getInfix();                  // 중위표기법으로 된 수식을 받아오는 함수
precedence getToken(char symbol); // 문자를 token으로 리턴하는 함수 
precedence getPriority(char x);   // 문자를 token으로 리턴하는 함수
void charCat(char *c);            // 후위표기법으로 된 문자열이 비어 있는지 확인하는 함수
void toPostfix();                 // 중위표기법으로 된 수식을 후위 표기법으로 변환하는 함수
void debug();                     // 중위표기법, 후위표기법, 계산된 결과, postfixstack에 남아 있는 원소 출력
void reset();                     // 리셋하는 함수
void evaluation();                // 변환한 수식 계산하는 함수

int main(){
    
    char command; // 입력받을 커맨드

    printf("[----- [choi yoolim] [2020022001] -----]\n");

    do{
        printf("----------------------------------------------------------------\n");
        printf(" Infix to Postfix, then Evaluation\n");
        printf("----------------------------------------------------------------\n");
        printf(" Infix=i, Postfix=p, Eval=e, Debug=d,Reset=r, Quit=q \n");
        printf("----------------------------------------------------------------\n");
        printf("Command = ");
        scanf(" %c", &command); // 커맨드(명령어) 입력받음
        
        switch(command) {
            case 'i':
            case 'I':   
                getInfix();   // 중위표기법으로 된 수식 받아옴
                break;
            case 'p':
            case 'P':
                toPostfix();  // 중위표기법에서 후위표기법으로 변환
                break;
            case 'e':
            case 'E':
                evaluation(); // 변환한 수식 계산
                break;
            case 'd':
            case 'D':
                debug();      // 현재 상태 출력
                break;
            case 'r':
            case 'R':
                reset();      // 리셋
                break;
            case 'q':
            case 'Q':
                break;
            default:          // 그 이외의 문자를 입력받은 경우
                printf("\n       >>>>>   Concentration!!   <<<<<     \n");
                break;
        }

    } while (command != 'q' && command != 'Q'); // 'q' 또는 'Q'를 입력받을 때까지 반복

    return 1;
}


void postfixPush(char x){                // postfixstack에 x push
    postfixStack[++postfixStackTop] = x; // postfixstack의 top을 증가시킨 후 그 top이 가리키는 메모리에 x 대입
}

char postfixPop(){                           
    char x;                                  // pop할 문자
    if (postfixStackTop == -1){              // postfixstack이 비어 있을 경우
        return '\0';                         // null 리턴
    }
    else{                                    // postfixstack이 비어 있지 않을 경우
        x = postfixStack[postfixStackTop--]; // postfixstack의 top을 감소시킨 후 그 top이 가리키는 메모리 안의 값을 x에 대입 
    }
    return x;                                // pop한 문자 return
}

void evalPush(int x){              // 계산한 값을 evalStack에 저장
    evalStack[++evalStackTop] = x; // evalStack의 top을 증가시킨 후 그 top이 가리키는 메모리에 x 대입
}

int evalPop(){                            // 계산한 값을 pop
    if (evalStackTop == -1)               // stack이 비어 있는 경우
        return -1;                        // -1 return
    else                                  // stack이 비어 있지 않는 경우
        return evalStack[evalStackTop--]; // stackTop이 가리키는 메모리에 들어있는 값을 return
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix(){                        
    printf("Type the expression >>> "); 
    scanf("%s", infixExp);              // infix로 된 수식 입력받음
}

precedence getToken(char symbol){ // 문자를 토큰으로 return
    switch (symbol){
        case '(':
            return lparen;
        case ')':
            return rparen;
        case '+':
            return plus;
        case '-':
            return minus;
        case '/':
            return divide;
        case '*':
            return times;
        default:
            return operand;
    }
}

precedence getPriority(char x){ // 문자를 토큰으로 return
    return getToken(x);         // getToken() 호출
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char *c){             // 문자열 포인터 c를 받아
    if (postfixExp == '\0')        // postfix 수식이 비어 있는 경우
        strncpy(postfixExp, c, 1); // c를 postfixExp에 저장
    else                           // postfix 수식이 비어 있지 않는 경우
        strncat(postfixExp, c, 1); // c를 postfixExp 끝에 이어 붙임
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix(){ // infix를 postfix로 변환
    /* infixExp의 문자 하나씩을 읽기위한 포인터 */
    char *exp = infixExp;
    char x; /* 문자하나를 임시로 저장하기 위한 변수 */

    /* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
    while (*exp != '\0'){                   // 문자열에서 null에 도달할 경우 종료
        if(getPriority(*exp)==operand){     // 토큰이 피연산자일 경우
            x=*exp;                         // infix에 있는 문자를 x에 대입
            charCat(&x);                    // postfixExp에 붙임
        }
        else if(getPriority(*exp)==lparen){ // 토큰이 '('일 경우
            postfixPush(*exp);              // stack에서 push
        }
        else if(getPriority(*exp)==rparen){ // 토큰이 ')'일 경우
            while((x=postfixPop())!='('){   // stack의 top에 '('이 나올 때까지 반복
                charCat(&x);                // 이어붙임
            }
        }
        else{                               // 토큰이 연산자일 경우
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)){
            // stack의 top에 있는 연산자가 현재 토큰의 연산자보다 우선순위가 높거나 같을 경우
                x=postfixPop();             // stack에서 연산자를 pop
                charCat(&x);                // postfixExp 뒤에 이어붙임
            } 
            postfixPush(*exp);              // 현재 토큰을 stack에 push
        }
        exp++;                              // exp 증가
    }

    while(postfixStackTop != -1){            // stack의 top이 -1이 될 떄까지 반복
        x=postfixPop();                      // stack에서 pop
        charCat(&x);                         // postfixExp 뒤에 이어붙임
    }
}

void debug(){                                 
    printf("\n---DEBUG\n");                   
    printf("infixExp = %s\n", infixExp);      // 중위표기법으로 출력
    printf("postExp = %s\n", postfixExp);     // 후위표기법으로 출력
    printf("eval result = %d\n", evalResult); // 결과 값 출력
    
    printf("postfixStack : ");

    for (int i = 0; i < MAX_STACK_SIZE; i++)  // 0 ~ 스택 최대 사이즈까지 반복
        printf("%c ", postfixStack[i]);       // 문자 출력
    
    printf("\n");
}

void reset(){
    infixExp[0] = '\0';   // infix 수식 초기화
    postfixExp[0] = '\0'; // postfix 수식 초기화

    for (int i = 0; i < MAX_STACK_SIZE; i++) // 0 ~ 스택 최대 사이즈까지 반복
        postfixStack[i] = '\0';              // postfixstack 초기화

    postfixStackTop = -1; // postfix의 top 초기상태로 초기화
    evalStackTop = -1;    // evalstack의 top 초기상태로 초기화
    evalResult = 0;       // 결과 값 초기화
}

void evaluation(){
    /* postfixExp, evalStack을 이용한 계산 */
    int opr1, opr2, i;

    int length = strlen(postfixExp); // postfixExp의 길이를 변수 length에 대입
    char symbol;                     // postfixExp의 문자를 저장할 변수
    evalStackTop = -1;               // evalStack 초기화

    for(i=0; i<length; i++){           // postfixExp의 모든 문자 loop
        symbol = postfixExp[i];        // 
        if(getToken(symbol)==operand){ // 피연산자일 경우
            evalPush(symbol- '0');     // evalStack에 저장
        }
        else{                          // 연산자일 경우
            opr2 = evalPop();          // evalStack에서 피연산자를 pop하여 저장
            opr1 = evalPop();          // evalStack에서 피연산자를 pop하여 저장
            switch(getToken(symbol)){                      
                case plus: evalPush(opr1 + opr2); break;   // 덧셈 연산 후 push
                case minus: evalPush(opr1 - opr2); break;  // 뺄셈 연산 후 push
                case times: evalPush(opr1 * opr2); break;  // 곱셈 연산 후 push
                case divide: evalPush(opr1 / opr2); break; // 나눗셈 연산 후 push
                default: break;                            // 그 이외는 break
            }
        }
    }
    evalResult = evalPop();  // evalStack에서 결과값을 pop하여 evalResult에 저장
}