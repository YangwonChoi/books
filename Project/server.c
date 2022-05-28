#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 30
#define EPOLL_SIZE 50
#define STOCK_SIZE 300
#define WAIT_TIME 3

typedef struct product_data
{
    char name[NAME_SIZE];
    int stock;
} prod_data;

typedef struct user_data
{
    char name[NAME_SIZE]; // 클라이언트(지점)의 이름
    int state;            // 클라이언트의 상태 (채팅관리용)
} user_data;

user_data user_data_list[EPOLL_SIZE];
prod_data MV_product_list[EPOLL_SIZE];
prod_data BD_product_list[EPOLL_SIZE][30];
prod_data HD_product_stock[30] = {
    {"사과주스", STOCK_SIZE},
    {"사이다", STOCK_SIZE},
    {"제로콜라", STOCK_SIZE},
    {"콜라", STOCK_SIZE},
    {"환타", STOCK_SIZE},
    {"abc주스", STOCK_SIZE},
    {"청포도주스", STOCK_SIZE},
    {"아메리카노", STOCK_SIZE},
    {"스위트아메리카노", STOCK_SIZE},
    {"카페라떼", STOCK_SIZE},
    {"레드볼", STOCK_SIZE},
    {"토마토주스", STOCK_SIZE},
    {"오렌지주스", STOCK_SIZE},
    {"딸기주스", STOCK_SIZE},
    {"하와이안주스", STOCK_SIZE},
    {"모히또", STOCK_SIZE},
    {"민트초코우유", STOCK_SIZE},
    {"초코우유", STOCK_SIZE},
    {"커피우유", STOCK_SIZE},
    {"닥터페퍼", STOCK_SIZE},
    {"솔의눈", STOCK_SIZE},
    {"갈배사이다", STOCK_SIZE},
    {"코코팜", STOCK_SIZE},
    {"비락식혜", STOCK_SIZE},
    {"맥콜", STOCK_SIZE},
    {"써니텐", STOCK_SIZE},
    {"밀키스", STOCK_SIZE},
    {"파워에이드", STOCK_SIZE},
    {"데미소다", STOCK_SIZE},
    {"토레타", STOCK_SIZE}};

void show_list();
void create_new_prod_list(int sock_num);
void remove_client(int sock_num);
void recv_ctrl(char msg[]);
void *user_ctrl(void *arg);
void error_handling(char *message);

pthread_mutex_t mutex; // 다른 스레드가 같은 데이터에 동시에 접근하지 못하게 하기위해 mutex 사용

// epoll용 변수 선언
struct epoll_event *ep_events;
struct epoll_event event;

int epfd, event_cnt;
int clnt_sock_list[EPOLL_SIZE]; // 채팅에 참가자 소켓번호 목록
int clnt_num = 0;               // 채팅 참가자 수
int roop_ctrl = 1;
int MV_cnt = 0;


int main(int argc, char *argv[])
{
    // 소켓통신용 변수 선언
    int serv_sock, clnt_sock;
    int str_len, i, j;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    pthread_t t_id;
    char buf[BUF_SIZE];
    char user_input[30];

    if (argc != 2) // 실행파일 경로/PORT번호 를 입력으로 받아야 함
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&mutex, NULL);
    serv_sock = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성

    /* 서버 주소정보 초기화 */
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    {
        error_handling("bind() error");
    }

    if (listen(serv_sock, 5) == -1)
    {
        error_handling("listen() error");
    }

    epfd = epoll_create(EPOLL_SIZE); // epoll 인스턴스 생성(관심 대상 파일 디스크립터 저장소)
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    pthread_create(&t_id, NULL, user_ctrl, (void *)&clnt_sock);

    while (roop_ctrl)
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1); // 이벤트가 발생할 때까지 무한대기

        if (event_cnt == -1)
        {
            puts("epoll_wait() error");
            break;
        }

        for (i = 0; i < event_cnt; ++i)
        {
            if (ep_events[i].data.fd == serv_sock) // 서버에 수신된 데이터가 존재하는지 확인해 클라이언트의 연결요청이 있었는지 확인한다.
            {
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz); // 클라이언트 연결요청 수락
                if (clnt_sock == -1)
                    error_handling("accept fail");
                str_len = read(clnt_sock, user_data_list[clnt_num].name, NAME_SIZE);

                // 클라이언트와의 송수신을 위해 새로 생성된 소켓에 이벤트 등록
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);

                // 채팅 클라이언트 목록에 추가
                clnt_sock_list[clnt_num] = clnt_sock;
                clnt_num++;

                create_new_prod_list(clnt_num - 1);
                printf("%s지점 접속 \n", user_data_list[clnt_num - 1].name);
            }
            else // 클라이언트의 메세지를 수신하는 소켓(accept 함수로 호출된 소켓)
            {
                str_len = read(ep_events[i].data.fd, buf, BUF_SIZE); // 클라이언트에게 데이터 수신
                if (str_len == 0)                                    // EoF 수신되었을시
                {
                    printf("%s지점 접속 종료,", user_data_list[clnt_num - 1].name);
                    remove_client(ep_events[i].data.fd);
                }
                else
                {
                    recv_ctrl(buf);
                }
            }
        }
    }
    close(serv_sock); // 서버소켓 소멸
    close(epfd);      // epoll 인스턴스 소멸
    return 0;
}

void show_list() // 재고 출력함수z
{
    int i, j;
    sleep(1);
    system("clear");
    printf("==창고 재고==\n");
    for (i = 0; i < 30; i++)
    {
        printf("[%s : %d]", HD_product_stock[i].name, HD_product_stock[i].stock);
        if (i % 5 == 4) // 5개씩 나눠서 출력!!
        {
            printf("\n");
        }
    }
    for (i = 0; i < event_cnt; ++i)
    {
        printf("==지점 %d 재고==", i);
        for (j = 0; j < 30; j++)
        {
            printf("[%s : %d]", BD_product_list[i][j].name, BD_product_list[i][j].stock);
            if (i % 5 == 4) // 5개씩 나눠서 출력!!
            {
                printf("\n");
            }
        }
    }
} // show_list()

void create_new_prod_list(int sock_num)
{
    int i;
    for (i = 0; i < 31; i++)
    {
        strcpy(BD_product_list[sock_num][i].name, HD_product_stock[i].name);
        BD_product_list[sock_num][i].stock = 0;
    }
    return;
}

void remove_client(int sock_num)
{
    int i;

    // 폴링 해제, 클라이언트 소켓 닫기
    epoll_ctl(epfd, EPOLL_CTL_DEL, sock_num, NULL);
    close(sock_num);

    // 제거되고 뒤에 있는 것을 앞으로 당기는 작업
    for (i = 0; i < clnt_num; i++)
    {
        if (clnt_sock_list[i] == sock_num)
        {
            // strcpy(user_data_list[i].name)
            memcpy(&clnt_sock_list[i], &clnt_sock_list[i + 1], sizeof(int) * (clnt_num - (i + 1))); // 소켓 리스트를 한칸씩 앞으로 당겨서 지워진 부분 매꾸기
            break;
        }
    }
    clnt_num--;
    printf("현재 참가자 수 = %d\n", clnt_num);
}

void recv_ctrl(char msg[])
{
    if (strstr(msg, "배송요청") != NULL)
    {
        // 지점의 배송 요청 처리
    }
}

void *user_ctrl(void *arg)
{
    char *user_input = (char *)arg;
    char stock_input[50];
    char name_input[50];
    char send_buf[BUF_SIZE];
    int num_input;
    int i, j, k;
    int cur;
    if (strstr(user_input, "지점배송") != NULL)
    {
        printf("소스 입력: ");
        scanf("%s", stock_input);
        for (i = 0; i <= 30; i++)
        {
            if (strstr(HD_product_stock[i].name, stock_input) != NULL)
            {
                printf("개수 입력: ");
                scanf("%d", &num_input);
                sprintf(send_buf, "%s %d", HD_product_stock[i].name, num_input);
                break;
            }
        }
        if (i >= 31)
        {
            printf("잘못입력하셨습니다\n");
            return (void *)-1;
        }
        printf("지점 이름: ");
        scanf("%s", name_input);
        for (j = 0; j < clnt_num; j++)
        {
            if (strstr(user_data_list[j].name, name_input) != NULL)
            {
                pthread_mutex_lock(&mutex);
                HD_product_stock[i].stock -= num_input;
                strcpy(MV_product_list[MV_cnt].name, HD_product_stock[i].name);
                MV_product_list[MV_cnt].stock = num_input;
                cur = MV_cnt++;
                pthread_mutex_unlock(&mutex);
                sleep(WAIT_TIME);
                write(clnt_sock_list[j], "지점배송", strlen("지점배송")); //"지점배송"메세지 보내기
                sleep(1);
                write(clnt_sock_list[j], send_buf, strlen(send_buf)); //소스이름+개수+지점 합친 메세지 보내기
                pthread_mutex_lock(&mutex);

                for (k = 0; k < MV_cnt; k++)
                {
                    if (k == cur)
                    {
                        memcpy(&MV_product_list[k], &MV_product_list[k + 1], sizeof(prod_data) * (MV_cnt - (k + 1))); // 소켓 리스트를 한칸씩 앞으로 당겨서 지워진 부분 매꾸기
                        break;
                    }
                }
                MV_cnt--;
                pthread_mutex_unlock(&mutex);
                BD_product_list[j][i].stock += num_input;
                break;
            }
        }
        if (j >= clnt_num)
        {
            printf("잘못입력하셨습니다\n");
            return (void *)-1;
        }
    }
    else if (strstr(user_input, "창고반품") != NULL)
    {
        printf("소스 입력: ");
        scanf("%s", stock_input);
        for (i = 0; i <= 30; i++)
        {
            if (strstr(HD_product_stock[i].name, stock_input) != NULL)
            {
                printf("개수 입력: ");
                scanf("%d", &num_input);
                sprintf(send_buf, "%s %d", HD_product_stock[i].name, num_input);
                break;
            }
        }
        if (i >= 31)
        {
            printf("잘못입력하셨습니다\n");
            return (void *)-1;
        }
        printf("지점 이름: ");
        scanf("%s", name_input);
        for (j = 0; j < clnt_num; j++)
        {
            if (strstr(user_data_list[j].name, name_input) != NULL)
            {
                pthread_mutex_lock(&mutex);
                BD_product_list[j][i].stock -= num_input;
                strcpy(MV_product_list[MV_cnt].name, HD_product_stock[i].name);
                MV_product_list[MV_cnt].stock = num_input;
                cur = MV_cnt++;
                pthread_mutex_unlock(&mutex);
                
                sleep(WAIT_TIME);
                write(clnt_sock_list[j], "창고반품", strlen("창고반품")); //"창고반품"메세지 보내기
                sleep(1);
                write(clnt_sock_list[j], send_buf, strlen(send_buf)); //소스이름+개수+지점이름 합친 메세지 보내기
                HD_product_stock[i].stock += num_input;

                pthread_mutex_lock(&mutex);
                for (k = 0; k < MV_cnt; k++)
                {
                    if (k == cur)
                    {
                        memcpy(&MV_product_list[k], &MV_product_list[k + 1], sizeof(prod_data) * (MV_cnt - (k + 1))); // 소켓 리스트를 한칸씩 앞으로 당겨서 지워진 부분 매꾸기
                        break;
                    }
                }
                MV_cnt--;
                pthread_mutex_unlock(&mutex);

                

                break;
            }
        }
        if (j >= clnt_num)
        {
            printf("잘못입력하셨습니다\n");
            return (void *)-1;
        }
    }
    return (void *)0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(EXIT_FAILURE); // -1
}