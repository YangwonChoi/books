#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 30
#define STOCK_SIZE 300

typedef struct product_data
{
    char name[NAME_SIZE];
    int stock;
} prod_data;

prod_data product_stock[30] = {
    {"사과주스", 0},
    {"사이다", 0},
    {"제로콜라", 0},
    {"콜라", 0},
    {"환타", 0},
    {"abc주스", 0},
    {"청포도주스", 0},
    {"아메리카노", 0},
    {"스위트아메리카노", 0},
    {"카페라떼", 0},
    {"레드볼", 0},
    {"토마토주스", 0},
    {"오렌지주스", 0},
    {"딸기주스", 0},
    {"하와이안주스", 0},
    {"모히또", 0},
    {"민트초코우유", 0},
    {"초코우유", 0},
    {"커피우유", 0},
    {"닥터페퍼", 0},
    {"솔의눈", 0},
    {"갈배사이다", 0},
    {"코코팜", 0},
    {"비락식혜", 0},
    {"맥콜", 0},
    {"써니텐", 0},
    {"밀키스", 0},
    {"파워에이드", 0},
    {"데미소다", 0},
    {"토레타", 0}};

void show_list();
void *send_msg(void *arg);
void error_handling(char *msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    pthread_t snd_thread, rcv_thread;
    void *thread_return;

    if (argc != 4)
    {
        printf("Usage: %s <IP> <port> <name>\n", argv[0]);
        exit(1);
    }

    sprintf(name, "[%s]", argv[3]);
    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    show_list();

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    write(sock, name, NAME_SIZE); // 아이디 서버로 보냄
    read(sock, product_stock, sizeof(product_stock));
    printf("%s\n", product_stock->name);
    pthread_create(&snd_thread, NULL, send_msg, (void *)&sock); //통신은 이 쓰레드 두개가 처리함 메인은 대기
    pthread_create(&rcv_thread, NULL, recv_msg, (void *)&sock);
    pthread_join(snd_thread, &thread_return);
    pthread_join(rcv_thread, &thread_return);
    close(sock);
    return 0;
}

void show_list() // 재고 출력함수------------
{
    sleep(1);
    system("clear");
    printf("창고재고\n");
    for (int i = 0; i < 30; i++)
    {
        printf("[%s : %d]", product_stock[i].name, product_stock[i].stock);
        if (i % 5 == 4) // 5개씩 나눠서 출력!!
        {
            printf("\n");
        }
    }
}

void *send_msg(void *arg)
{
    int sock = *((int *)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    char send_buf[BUF_SIZE];
    char stock_input[50];
    char *temp_stock;
    char *temp_num;
    int num_input;
    int i = 0;
    int str_len;
    while (1)
    {
        printf("1)배송요청 2)반품요청");
        fgets(msg, sizeof(msg), stdin);
        if (strstr(msg, "1") != NULL)
        {
            printf("물품 입력: ");
            scanf("%s", stock_input);
            for (i = 0; i <= 30; i++)
            {
                if (strstr(product_stock[i].name, stock_input) != NULL)
                {
                    printf("개수 입력: ");
                    scanf("%d", &num_input);
                    sprintf(send_buf, "%s %d", product_stock[i].name, num_input);
                    write(sock, "배송요청", strlen("배송요청")); //"배송요청"메세지 서버로 보내기
                    sleep(1);
                    write(sock, send_buf, strlen(send_buf)); //음료이름,개수 서버로 보내기

                    str_len = read(sock, name_msg, BUF_SIZE - 1);
                    if (strstr(name_msg, "거절") != NULL)
                    {
                        printf("배송요청이 거절되었습니다\n");
                        break;
                    }
                    product_stock[i].stock += atoi(name_msg);
                    break;
                }
            }    
        }
        else if (strstr(name_msg, "2") != NULL)
        {
            str_len = read(sock, name_msg, BUF_SIZE - 1); //"창고반품"읽으면 그떄 읽음 
            
            name_msg[str_len] = 0;

            //temp_stock = strtok(name_msg, "%");
            temp_stock = strtok(temp_stock, " ");
            temp_num = strtok(NULL, " "); //자른 문자 다음부터 구분자 또 찾기

            // strcpy(stock.name, temp_stock);

            for (int i = 0; i <= 30; i++)
            {
                if (strstr(temp_stock, product_stock[i].name) != NULL)
                {
                    product_stock[i].stock -= atoi(temp_num);
                    //printf("이름: %s \n", product_stock[i].name);
                    //printf("숫자: %d\n", product_stock[i].stock);
                    //printf("%d개 추가됨 \n", product_stock[i].stock);
                    show_list_visual();
                }
            }
        }
        else if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
        {
            close(sock);
            exit(0);
        }
    }
    return NULL;
}

void error_handling(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}