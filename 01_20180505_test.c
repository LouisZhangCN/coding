#include <stdio.h>
#include <malloc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct list_node {
    int num;
    struct list_node *next;
} t_list_node;

enum {
    CASE_LIST = 1,
    CASE_TCP_SERVER,
    CASE_TCP_CLIENT,
    CASE_FORK,
    CASE_SIGNAL,
    CASE_PTHREAD,
    CASE_SHARE_MEM,
    CASE_MSG_QUEUE,
};


void list_case(void);
void tcp_server_case(void);
void tcp_client_case(void);
void fork_case(void);
void signal_case(void);
void pthread_case(void);
void share_mem_case(void);
void msg_queue_case(void);
int main(int argc, char **argv)
{
    int caseid = 8;

    switch (caseid) {
        case CASE_LIST:
            list_case();
            break;

        case CASE_TCP_SERVER:
            tcp_server_case();
            break;

        case CASE_TCP_CLIENT:
            tcp_client_case();
            break;

        case CASE_FORK:
            fork_case();
            break;

        case CASE_SIGNAL:
            signal_case();
            break;

        case CASE_PTHREAD:
            pthread_case();
            break;

        case CASE_SHARE_MEM:
            share_mem_case();
            break;

        case CASE_MSG_QUEUE:
            msg_queue_case();
            break;

        default:
            printf("no case match\n");
            break;
    }
}


typedef struct msg_data {
    long int msg_type;
    char *data;
} t_msg_data;

#define BUF_SIZE 1024

void msg_queue_case(void)
{
    pid_t pid;
    key_t key = 1234539;
    int msgid = 0;
    t_msg_data sendmsg, recvmsg;
    long int msgtype = 3;
    int ret;

    msgid = msgget(key, IPC_PRIVATE|0666);
    printf("msgid:%d\n", msgid);

    pid = fork();

    if (!pid) {
        memset(&recvmsg, 0, sizeof(recvmsg));
        recvmsg.msg_type = msgtype;
        recvmsg.data = malloc(BUF_SIZE);

        while (1) {
            memset(recvmsg.data, 0, BUF_SIZE);
            ret = msgrcv(msgid, &recvmsg, BUF_SIZE + 1, msgtype, 0);
            printf("ret:%d, Recv: %s\n", ret, recvmsg.data);

            if (strncmp(recvmsg.data, "end", 3) == 0)
                break;
        }

        free(recvmsg.data);
        recvmsg.data = NULL;

        exit(0);
    }
    else {
        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.msg_type = msgtype;
        sendmsg.data = malloc(BUF_SIZE);

        while (1) {
            memset(sendmsg.data, 0, BUF_SIZE);
            printf("Enter msg:");
            fgets(sendmsg.data, BUF_SIZE, stdin);
            msgsnd(msgid, &sendmsg, strlen(sendmsg.data), 0);
            sleep(1);

            if (strncmp(sendmsg.data, "end", 3) == 0)
                break;
        }

        free(sendmsg.data);
        sendmsg.data = NULL;
    }

    msgctl(msgid, IPC_RMID, 0);
}


void share_mem_case(void)
{
    pid_t pid;
    int shmid;
    key_t key = 12345678; // i don't know why it failed with using this key generate the share memory
    int memsize = 2 * 4 * 1024;
    char *pmem = NULL;
    char *str = "hello beauty";

    shmid = shmget(IPC_PRIVATE, memsize, IPC_CREAT|IPC_EXCL|0666);
    printf("shmid:%d\n", shmid);

    pid = fork();
    if (!pid) {
        pmem = shmat(shmid, NULL, 0);
        memcpy(pmem, str, strlen(str));
        shmdt(pmem);
        exit(0);
    }
    else {
        wait(NULL);
        pmem = shmat(shmid, NULL, 0);
        printf("%s\n", pmem);
        shmdt(pmem);
        shmctl(shmid, IPC_RMID, 0);
    }
}


typedef struct thread_priv {
    int num;
    struct thread_priv *next;
} t_thread_priv;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thread_routine1(void *arg);
void *thread_routine2(void *arg);
void pthread_case(void)
{
    pthread_t tid1, tid2;
    t_thread_priv *list = NULL;

    pthread_create(&tid2, NULL, thread_routine2, &list);
    pthread_create(&tid1, NULL, thread_routine1, &list);

    sleep(2);
    pthread_cancel(tid1);
    pthread_cancel(tid2);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("this is main thread endline\n");
}

void *thread_routine2(void *arg)
{
    t_thread_priv *list;
    int list_len = 10;

    pthread_mutex_lock(&mtx);
    while (list_len--) {
        list = malloc(sizeof(t_thread_priv));
        list->num = list_len;
        printf("add %d in stack\n", list->num);
        list->next = *(t_thread_priv **)arg;
        *(t_thread_priv **)arg = list;
    }
    pthread_mutex_unlock(&mtx);
    pthread_cond_signal(&cond);
}

void *thread_routine1(void *arg)
{
    t_thread_priv *list = *(t_thread_priv **)arg;

    while (1) {
        pthread_mutex_lock(&mtx);
        list = *(t_thread_priv **)arg;
        while (list == NULL) {
            pthread_cond_wait(&cond, &mtx);
        }
        printf("Got %d from stack\n", list->num);
        *(t_thread_priv **)arg = list->next;
        free(list);
        pthread_mutex_unlock(&mtx);
    }
}


void intr_isr(int sig);
void alrm_isr(int sig);
void signal_case(void)
{
    pid_t pid;

    (void *)signal(SIGINT, intr_isr);

    if (!pid) {
        printf("alarm start after 5 seconds\n");
        sleep(5);
        kill(getppid(), SIGALRM);
        exit(0);
    }
    else {
        (void *)signal(SIGALRM, alrm_isr);
        wait(NULL);
    }

    while(1) {
        printf("This process is alive\n");
        sleep(1);
    }
}

void alrm_isr(int sig)
{
    printf("Alarm Done\n");
}

void intr_isr(int sig)
{
    printf("hello beauty, give me a kiss\n");
    (void *)signal(SIGINT, SIG_DFL);
}


void fork_case(void)
{
    pid_t pid, cid;
    char *msg;
    int loop = 0;
    int exit_code = 0;
    int stat;

    pid = fork();

    switch (pid) {
        case -1:
            printf("fork failed\n");
            break;

        case 0:
            msg = "This is a child process";
            loop = 3;
            exit_code = 33;
            break;

        default:
            msg = "This is a father process";
            loop = 5;
            cid = wait(&stat);
            if (WIFEXITED(stat))
                printf("child process:%d exit with code:%d\n", cid, WEXITSTATUS(stat));
            else
                printf("child process exit abnormally\n");
            break;
    }

    while (loop--) {
        printf("%s\n", msg);
    }

    exit(exit_code);
}


#define PORT_NUM 1999
#define SEVER_IP "10.0.0.11"
void tcp_client_case(void)
{
    int sockfd;
    struct sockaddr_in saddr;
    int port = PORT_NUM;
    char *serverip = SEVER_IP;
    char *buf = malloc(BUF_SIZE);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(serverip);

    connect(sockfd, (const struct sockaddr *)&saddr, sizeof(saddr));

    memset(buf, 0, BUF_SIZE);
    printf("Enter msg:");
    fgets(buf, BUF_SIZE, stdin);
    write(sockfd, buf, strlen(buf));

    free(buf);
    close(sockfd);
}


void tcp_server_case(void)
{
    int sfd, cfd;
    struct sockaddr_in saddr, c_addr;
    int c_len = 0;
    int port = PORT_NUM;
    char *buf = malloc(BUF_SIZE);
    int nbytes = 0;

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sfd, (const struct sockaddr *)&saddr, sizeof(saddr));

    if (listen(sfd, 5) == 0)
        printf("Listening\n");
    else
        printf("Listen failed\n");

    memset(&c_addr, 0, sizeof(c_addr));
    while (1) {
        cfd = accept(sfd, (struct sockaddr *)&c_addr, &c_len);
        if (cfd) {
            memset(buf, 0, BUF_SIZE);
            nbytes = read(cfd, buf, BUF_SIZE);
            printf("Recv %dBytes: %s\n", nbytes, buf);
            close(cfd);
        }
    }

    free(buf);
    buf = NULL;
    close(sfd);
}


void list_add_head(t_list_node **head, int num);
void print_list(t_list_node *head);
void list_add_tail(t_list_node **head, int num);
void list_add_pos(t_list_node **head, int num, int pos);
void list_reverse(t_list_node **head);
void list_del_head(t_list_node **head);
void list_del_tail(t_list_node **head);
void list_case(void)
{
    t_list_node *head = NULL;

    list_add_head(&head, 3);
    list_add_head(&head, 2);
    list_add_head(&head, 1);
    print_list(head);
    list_add_tail(&head, 4);
    list_add_tail(&head, 5);
    print_list(head);
    list_add_pos(&head, 9, 2);
    print_list(head);
    list_reverse(&head);
    print_list(head);
    list_del_head(&head);
    print_list(head);
    list_del_tail(&head);
    print_list(head);
}


void list_del_tail(t_list_node **head)
{
    t_list_node *pre_node = *head, *node = *head;

    while (node->next) {
        pre_node = node;
        node = node->next;
    }

    pre_node->next = NULL;
    free(node);
}


void list_del_head(t_list_node **head)
{
    t_list_node *node = *head;

    *head = node->next;
    node->next = NULL;
    free(node);
}


void list_reverse(t_list_node **head)
{
    t_list_node *t1, *t2, *t;
    t1 = *head;
    t2 = t1->next;
    while (t2) {
        t = t2->next;
        t2->next = t1;
        t1 = t2;
        t2 = t;
    }
    (*head)->next = NULL;
    *head = t1;
}


void list_add_pos(t_list_node **head, int num, int pos)
{
    t_list_node *phead = *head;
    t_list_node *node = malloc(sizeof(t_list_node));
    node->num = num;
    node->next = NULL;

    if (!pos) {
        list_add_head(head, num);
    }
    else {
        while (--pos) {
            phead = phead->next;
        }
        node->next = phead->next;
        phead->next = node;
    }
}


void list_add_tail(t_list_node **head, int num)
{
    t_list_node *phead = *head;
    t_list_node *node = malloc(sizeof(t_list_node));
    node->num = num;
    node->next = NULL;

    if (*head == NULL) {
        *head = node;
    }
    else {
        while (phead->next) {
            phead = phead->next;
        }
        phead->next = node;
    }
}


void print_list(t_list_node *head)
{
    printf("list is:\n");
    while (head) {
        printf("%d ", head->num);
        head = head->next;
    }
    printf("\n");
}


void list_add_head(t_list_node **head, int num)
{
    t_list_node *node = malloc(sizeof(t_list_node));
    node->num = num;
    node->next = NULL;

    if (*head == NULL) {
        *head = node;
    }
    else {
        node->next = *head;
        *head = node;
    }
}
