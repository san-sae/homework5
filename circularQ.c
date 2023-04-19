/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  School of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // 큐의 최대 사이즈

typedef char element; // char형의 별칭 element 정의

// 구조체 QueueType 정의
typedef struct {
	element queue[MAX_QUEUE_SIZE]; // element(char형)으로 배열 선언
	int front, rear;               // queue의 원소 관련 변수
}QueueType;


QueueType *createQueue();                   // 큐 생성 함수 선언
int freeQueue(QueueType *cQ);               // 큐의 메모리 해제 함수 선언
int isEmpty(QueueType *cQ);                 // 큐가 비어 있는지 확인하는 함수 선언
int isFull(QueueType *cQ);                  // 큐가 가득 찼는지 확인하는 함수 선언 
void enQueue(QueueType *cQ, element item);  // 큐에 데이터(원소) 삽입하는 함수 선언
void deQueue(QueueType *cQ, element* item); // 큐에 데이터 삭제하는 함수 선언
void printQ(QueueType *cQ);                 // 큐의 데이터를 출력하는 함수 선언
void debugQ(QueueType *cQ);                 // 큐의 현재 상태 확인하는 함수 선언
element getElement();                       // 삽입할 데이터 받는 함수 선언

int main(void){

	QueueType *cQ = createQueue(); // 구조체 Queuetype에 대한 변수 cQ에 새로운 큐 생성
	element data;                  // 입력받을 데이터
	char command;                  // 입력받을 커맨드(명령어)

	printf("[----- [choi yoolim] [2020022001] -----]");

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); // 커맨드 입력

		switch(command) {
			case 'i': case 'I':
				data = getElement(); // 삽입할 데이터를 입력받아 변수 data에 할당
				enQueue(cQ, data);   // 큐 cQ에 data 삽입
				break;
			case 'd': case 'D':
				deQueue(cQ, &data); // 큐 cQ에서 데이터 삭제
				break;
			case 'p': case 'P':
				printQ(cQ);         // 큐 cQ에 있는 데이터 출력
				break;
			case 'b': case 'B':
				debugQ(cQ);         // 큐 cQ의 현재 상태 확인
				break;
			case 'q': case 'Q':
				freeQueue(cQ);     // 큐 cQ의 메모리 해제
				break;
			default:               // 커맨드로 정해진 문자 이외의 문자를 입력받았을 경우
				printf("\n       >>>>>   Concentration!!   <<<<<     \n");
				break;
		}

	}while(command != 'q' && command != 'Q'); // 'q' 또는  'Q'를 입력 받을 때까지 do-while문 반복


	return 1;
}

QueueType *createQueue(){                        // 구조체 QueueType형의 주소로 반환
	QueueType *cQ;                               // 구조체 포인터 선언
	cQ = (QueueType *)malloc(sizeof(QueueType)); // QueueType의 size만큼 동적할당하여 그 주소를 cQ에 대입
	cQ->front = 0;                               // cQ의 멤버 변수인 front를 0으로 초기화
	cQ->rear = 0;                                // cQ의 멤버 변수인 rear를 0으로 초기화
	return cQ;                                   // cQ의 주소 반환
}

int freeQueue(QueueType *cQ){ // 구조체 QueueType형의 주소를 전달 
    if(cQ == NULL) return 1;  // cQ에 메모리 할당을 받지 않아 메모리 해제할 필요 없이 함수 종료
    free(cQ);	          	  // 동적 할당한 cQ에 대한 메모리 해제
    return 0;                 // 메모리 해제한 경우 0 return      
}

element getElement(){           // 삽입할 데이터를 입력받아 element형으로 반환
	element item;               // element(char형)으로 변수 item 선언
	printf("Input element = ");
	scanf(" %c", &item);	    // 변수 item 입력받음
	return item;                // 변수 item 반환
}


/* completed function */
int isEmpty(QueueType *cQ){    // 큐 cQ가 비어 있는지 확인 
	if(cQ->front == cQ->rear)  // 현재 큐 cQ가 비어 있을 경우
		return 1;              // 비어 있을 경우 1 리턴
	else
		return 0;                  // 현재 큐 cQ가 비어 있지 않는 경우 0리턴
}

/* completed function */
int isFull(QueueType *cQ){                       // 큐 cQ가 가득 차 있는지 확인
    if(cQ->front == (cQ->rear+1)%MAX_QUEUE_SIZE) // 큐 cQ가 가득 차 있는 경우
		return 1;                                // 가득 차 있는 경우 1 리턴
	// 큐의 인덱스가 0 ~ (MAX_QUEUE_SIZE-1)까지 반복되기에 모듈러 연산 사용
	else
    	return 0;                                    // 현재 큐 cQ가 가득 차 있지 않는 경우 0 리턴
}


/* completed function */
void enQueue(QueueType *cQ, element item){      // 큐 cQ에 원소 item을 삽입하는 함수
	if(isFull(cQ)){                             // 큐 cQ가 가득 차 있는 경우
		return;                                 // 함수 종료
	}
	else{                                       // 큐 cQ에 원소를 삽입할 공간이 있는 경우
		cQ->rear = (cQ->rear+1)%MAX_QUEUE_SIZE; // 원형 큐 전제하에 rear 값 증가
		cQ->queue[cQ->rear] = item;             // 증가시킨 rear 값이 가리키는 메모리에 데이터 삽입
		return;                                 // 함수 종료
	}
}

/* completed function */
void deQueue(QueueType *cQ, element *item){       // 큐 cQ에서 원소 item을 삭제하는 함수
	if(isEmpty(cQ)){                              // 큐 cQ가 비어 있는 경우
		return;                                   // 함수 종료
	}                                    
    else{                                         // 큐 cQ가 비어 있지 않는 경우
		*item = cQ->queue[cQ->front];             // 현재 front 값이 가리키는 메모리의 데이터를 item에 저장
		cQ->front = (cQ->front+1)%MAX_QUEUE_SIZE; // 원형 큐 전제하에 front 값 증가시켜 데이터 삭제
		return;                                   // 함수 종료
	}
}


void printQ(QueueType *cQ){                 // 큐 cQ에 저장된 데이터 출력하는 함수
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; // 큐 cQ의 front 값을 first에 대입
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;   // 큐 cQ의 rear 값을 last에 대입

	printf("Circular Queue : [");

	i = first;
	while(i != last){                // 큐 cQ가 비어 있지 않는 경우
		printf("%3c", cQ->queue[i]); // 큐 cQ에 저장된 데이터 출력
		i = (i+1)%MAX_QUEUE_SIZE;    // 원형 큐 전제하에 i(first)값 증가
	}

	printf(" ]\n");
}


void debugQ(QueueType *cQ){                                 // 현재 큐 cQ의 상태 확인하는 함수

	printf("\n---DEBUG\n");
	
	for(int i = 0; i < MAX_QUEUE_SIZE; i++){                // 0 ~ 큐의 최대사이즈 반복
		if(i == cQ->front) {                                // i가 큐의 front와 같을경우
			printf("  [%d] = front\n", i);                  // i값과 front 출력
			continue;                                       
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);           // i값과 큐 cQ에 저장된 데이터 출력 

	}
	
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // 큐 cQ의 front와 rear의 위치 출력
}
