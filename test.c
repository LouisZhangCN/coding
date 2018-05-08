#include <malloc.h>
#include <stdlib.h>

enum {
    CASE_DLINK_LIST = 1,
    CASE_LIST,
    CASE_SORT_ALGORITHM,
};

void dlink_list_case(void);
void list_case(void);
void sort_algorithm_case(void);
int main(int argc, char **argv)
{

    int caseid = 3;

    switch (caseid) {
        case CASE_DLINK_LIST:
            dlink_list_case();
            break;

        case CASE_LIST:
            list_case();
            break;

        case CASE_SORT_ALGORITHM:
            sort_algorithm_case();
            break;

        default:
            printf("no case match\n");
            break;
    }
}


enum {
    CASE_BUBBLE = 1,
    CASE_STRAIGHT_INSERT,
};
void bubble_case(int *array, int len);
void straight_insert_case(int *array, int len);
void sort_algorithm_case(void)
{
    int len = 10;
    int *array = malloc(sizeof(int) * len);
    int i = 0;
    int caseid = 2;

    printf("array is:");
    for (; i < len; i++) {
        array[i] = random() % 9 ;
        printf("%d ", array[i]);
    }
    printf("\n");

    switch (caseid) {
        case CASE_BUBBLE:
            bubble_case(array, len);
            break;

        case CASE_STRAIGHT_INSERT:
            straight_insert_case(array, len);
            break;

        default:
            printf("no case match\n");
            break;
    }

    printf("array is:");
    for (i = 0; i < len; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void straight_insert_case(int *array, int len)
{
    int i, j;
    int tmp, idx;

    for (i = 0; i < len - 1; i++) {
        for (j = i + 1; j > 0; j--) {
            if (array[j] > array[j - 1]) {
                break;
            }

            tmp = array[j];
            array[j] = array[j - 1];
            array[j - 1] = tmp;
        }
    }
}

void bubble_case(int *array, int len)
{
    int i, j;

    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - 1 - i; j++) {
            if (array[j + 1] < array[j]) {
                int tmp = array[j + 1];
                array[j + 1] = array[j];
                array[j] = tmp;
            }
        }
    }
}


typedef struct list {
    int data;
    struct list *next;
} t_list;

static inline void *list_add_head(t_list *new, t_list **head);
static inline void __list_add_tail(t_list *new, t_list **head);
static inline void print_list(t_list *head);
static inline void list_add_pos(t_list *new, t_list **head, int pos);
static inline void list_reverse(t_list **head);
static inline void list_del_head(t_list **head);
static inline void list_del_tail(t_list **head);
void list_case(void)
{
    t_list *head = NULL, *node;
    int i = 0;

    for (; i < 10; i++) {
        node = malloc(sizeof(t_list));
        node->data = i;
        node->next = NULL;

        if ((i % 2) == 0)
            list_add_head(node, &head);
        else if ((i % 9) == 0)
            list_add_pos(node, &head, 3);
        else
            __list_add_tail(node, &head);
    }
    print_list(head);

    list_reverse(&head);
    print_list(head);

    list_del_head(&head);
    list_del_tail(&head);
    print_list(head);
}

static inline void list_del_tail(t_list **head)
{
    t_list *tail = *head, *prev;

    while (tail->next) {
        prev = tail;
        tail = tail->next;
    }

    prev->next = NULL;
}

static inline void list_del_head(t_list **head)
{
    t_list *node = *head;

    *head = node->next;
    node->next = NULL;
    free(node);
}

static inline void list_reverse(t_list **head)
{
    t_list *t1, *t2, *t;
    t1 = *head;
    t2 = (*head)->next;

    while (t2) {
        t = t2->next;
        t2->next = t1;
        t1 = t2;
        t2 = t;
    }

    (*head)->next = NULL;
    *head = t1;
}

static inline void list_add_pos(t_list *new, t_list **head, int pos)
{
    t_list *phead = *head;

    while (--pos) {
        phead = phead->next;
    }
    new->next = phead->next;
    phead->next = new;
}

static inline void print_list(t_list *head)
{
    printf("list is:");
    while (head) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

static inline void __list_add_tail(t_list *new, t_list **head)
{
    t_list *phead = *head;

    if (!(*head)) {
        *head = new;
    }
    else {
        while (phead->next) {
            phead = phead->next;
        }
        phead->next = new;
    }
}

static inline void *list_add_head(t_list *new, t_list **head)
{
    if (!(*head)) {
        *head = new;
    }
    else {
        new->next = *head;
        *head = new;
    }
}


typedef struct list_head {
    struct list_head *next;
    struct list_head *prev;
} t_list_head;

static inline void INIT_LIST_HEAD(t_list_head *list)
{
    list->next = list;
    list->prev = list;
}

static inline void __list_add(t_list_head *new, t_list_head *prev, t_list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

static inline void list_add_tail(t_list_head *new, t_list_head *head)
{
    __list_add(new, head->prev, head);
}

typedef struct fox {
    int tail_length;
    int weight;
    struct list_head list_node;
} t_fox;

t_fox *create_fox(int len, int weight)
{
    t_fox *tmp = malloc(sizeof(t_fox));

    tmp->tail_length = len;
    tmp->weight = weight;

    return tmp;
}



#define list_entry(ptr, type, member) \
    ((char *)ptr - (char *)(&(((type *)0)->member)))

#define list_for_each(pos, head) \
    for (pos = head->next; pos != head; pos = pos->next)

void dlink_list_case(void)
{
    t_fox *fox_list = malloc(sizeof(t_fox));
    t_fox *fox;
    t_list_head *node;

    t_list_head *head = &(fox_list->list_node);
    INIT_LIST_HEAD(head);

    fox = create_fox(100, 100);
    list_add_tail(&(fox->list_node), head);

    fox = create_fox(10, 900);
    list_add_tail(&(fox->list_node), head);

    list_for_each(node ,head) {
        fox = (t_fox *)list_entry(node, t_fox, list_node);
        printf("fox: tail_length=%d, weight=%d\n", fox->tail_length, fox->weight);
    }
}
