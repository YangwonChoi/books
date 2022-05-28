#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define DATA_SIZE 30

typedef struct Data
{
    char name[DATA_SIZE];
    char phoneNum[DATA_SIZE];
} Data;

typedef struct Node
{
    struct Node *next;
    Data user_data;
} Node;

void menu();
int add(Node *head);
void showList(Node *head);
void reset(Node *head);
void delete(Node* cur, Node*temp);
void search(Node *head);
void change(Node *cur);
void swap(Node *cur, Node *temp);
void sort(Node *head);

int main()
{
    Node *head = (Node *)malloc(sizeof(Node));
    head->next = NULL;

    int select;
    char name[DATA_SIZE];
    char phoneNum[DATA_SIZE];

    while (1)
    {
        system("clear");
        menu();

        scanf("%d", &select);

        switch (select)
        {
        case 1: // 등록
            add(head);
            break;
        case 2: // 검색/변경
            search(head);
            break;
        case 3: // 목록
            showList(head);
            break;
        case 4: //초기화
            reset(head);
            break;
        case 5: // 종료
            puts("종료합니다.");
            return 0;
        default:
            puts("잘못된 입력!");
            break;
        }
    }
    return 0;
}

void menu() // 메뉴
{
    printf("\n");
    printf("-------------\n");
    printf("-    1. 등록    -\n");
    printf("-    2. 검색    -\n");
    printf("-    3. 목록    -\n");
    printf("-    4. 초기화  -\n");
    printf("-    5. 종료    -\n");
    printf("--------------\n");
    printf("선택 : ");
}

int add(Node *head) // 전화번호와 이름을 입력받는 함수
{
    char new_name[DATA_SIZE];
    char new_num[DATA_SIZE];

    printf("\n이름 : ");
    scanf("%s", new_name);
    printf("전화번호 : ");
    scanf("%s", new_num);

    if (head->next == NULL)
    {
        Node *new_node = malloc(sizeof(Node));
        strcpy(new_node->user_data.phoneNum, new_num);
        strcpy(new_node->user_data.name, new_name);
        new_node->next = NULL;
        head->next = new_node;
    }
    else
    {
        Node *cur = head;
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
        Node *new_node = malloc(sizeof(Node));
        strcpy(new_node->user_data.phoneNum, new_num);
        strcpy(new_node->user_data.name, new_name);
        new_node->next = NULL;

        cur->next = new_node;
    }
    sort(head);
}

void showList(Node *head) // 전화번호 목록
{
    Node *cur = head->next;
    if (cur == NULL) //예외처리
    {
        printf("저장된 데이터가 없습니다 \n");
        sleep(2);
    }
    while (cur != NULL)
    {
        printf("이름: %s  전화번호: %s  \n", cur->user_data.name, cur->user_data.phoneNum);
        cur = cur->next;
    }
    sleep(2);
}

void reset(Node *head) // 초기화
{
    Node *cur = head->next;
    Node *next;
    while (cur != NULL)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }
    head->next = NULL;
}

void delete(Node *cur, Node* temp)
{
    temp->next = cur->next;
    free(cur);
}
/*처음에 cur만 수정했더니 segmantion error 뜸
cur만 삭제하게 되면 중간에 연결리스트가 끊겨 temp로 copy본을 만들어 
cur삭제된 자리에 copy본이 삽이되어 연결리스트가 끊기지 않게함.*/

void search(Node *head) // 검색
{
    int select;
    char input[DATA_SIZE];
    Node* temp;
    printf("이름:");
    scanf("%s", input);

    Node *cur = head->next;
    while (cur != NULL)
    {
        if (strcmp(cur->user_data.name, input) == 0)
        {
            printf("이름: %s 번호: %s \n", cur->user_data.name, cur->user_data.phoneNum);
            sleep(2);
            break;
        }
        temp=cur;
        cur = cur->next;
    }
    system("clear");
    printf("\n");
    printf("------------------\n");
    printf("-  1. 변경       -\n");
    printf("-  2. 삭제       -\n");
    printf("-  3. 처음으로   -\n");
    printf("------------------\n");
    printf("선택 : ");
    scanf("%d", &select);
    if (select == 1)
        change(cur); 
    else if (select == 2)
        delete(cur, temp);
    else if (select == 3)
        return;
}

void change(Node *cur)// 데이터 변경/수정
{
    char changeName[DATA_SIZE];
    char changeNum[DATA_SIZE];

    printf("변경할 이름:");
    scanf("%s", changeName);
    printf("변경할 번호:");
    scanf("%s", changeNum);
    strcpy(cur->user_data.name, changeName);
    strcpy(cur->user_data.phoneNum, changeNum);
    printf("변경되었습니다. \n");
    sleep(2);
}

void swap(Node *cur, Node *temp)
{
    Data temp_t = cur->user_data;
    cur->user_data = temp->user_data;
    temp->user_data = temp_t;
}

void sort(Node *head) // 노드를 버블소트로 정렬
{
    Node *cur = head->next;
    Node *temp;
    while (cur != NULL)
    {
        temp = cur;
        while (temp != NULL)
        {
            if (strcmp(cur->user_data.name, temp->user_data.name) > 0)
                swap(cur, temp);
            temp = temp->next;
        }
        cur = cur->next;
    }
}
