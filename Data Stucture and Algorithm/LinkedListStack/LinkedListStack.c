#include <stdio.h>
#include <stdlib.h>

/*
* @name 链式栈的节点
* @param data      数据域
* @param next      后继指针
*/
typedef struct Node {
    int data;
    struct Node* next;
} Node;

/*
 * @name 利用单链表实现链式栈
 * @param head      虚拟链表头节点
 * @param size      当前栈中元素个数
 */
typedef struct {
    size_t size;
    Node* head;
} LinkedListStack;

/*
 * @name 初始化链式栈
 * @param stack     结构体指针
 */
void initStack(LinkedListStack* stack) {
    stack->head = NULL;
    stack->size = 0;
    printf("链式栈初始化成功\n");
}

/*
 * @name 返回链式栈中元素个数
 * @param stack     结构体指针
 * @return          栈中元素数量
 */
size_t getSize(const LinkedListStack* stack) {
    return stack->size;
}

/*
 * @name 入栈
 * @param stack     结构体指针
 * @param element   要入栈的元素
 */
void pushStack(LinkedListStack* stack, int element) {
    Node* newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        printf("内存分配失败，入栈失败\n");
        return ;
    }
    newNode->data = element;
    newNode->next = stack->head;
    stack->head = newNode;
    stack->size++;
    printf("数据 %d 已入栈\n", element);
}

/*
 * @name 出栈
 * @param stack     结构体指针
 */
void popStack(LinkedListStack* stack) {
    if (stack->size == 0) {
        printf("栈为空，出栈失败\n");
        return;
    }
    Node* temp = stack->head;
    stack->head = temp->next;
    stack->size--;
    printf("数据 %d 已出栈\n", temp->data);
    free(temp);
}

/*
 * @name 修改链式栈中指定位置的数据（从栈底开始索引：0=最底，size-1=栈顶）
 * @param stack         结构体指针
 * @param index         要修改的下标（0-based，从栈底算起）
 * @param newElement    新值
 */
void modifyAt(LinkedListStack* stack, size_t index, int newElement) {
    if (index < 0 || index >= stack->size) {
        printf("下标索引不合法\n");
        return;
    }
    size_t steps = stack->size - 1 - index; //距离栈顶的位置
    Node* current = stack->head;
    for (size_t i = 0; i < steps; i++) {
        current = current->next;
    }
    current->data = newElement;
    printf("成功将下标 %d 的数据修改为 %d\n", index, newElement);
}

/*
 * @name 查找链式栈中指定位置的数据（从栈底开始索引）
 * @param stack     结构体指针
 * @param index     要查找的位置索引（0=栈底，size-1=栈顶）
 * @return          对应元素的值
 */
int getElementAt(const LinkedListStack* stack, size_t index) {
    if (index >= stack->size) {
        printf("下标索引不合法\n");
        return -1;
    }
    size_t steps = stack->size - 1 - index;
    Node* current = stack->head;
    for (size_t i = 0; i < steps; i++) {
        current = current->next;
    }
    return current->data;
}

/*
 * @name 释放链式栈内存
 * @param stack     结构体指针
 */
void destroyStack(LinkedListStack* stack) {
    Node* current = stack->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    stack->head = NULL;
    stack->size = 0;
}

int main(int argc, char* argv[]) {
    LinkedListStack stack;
    return 0;
}