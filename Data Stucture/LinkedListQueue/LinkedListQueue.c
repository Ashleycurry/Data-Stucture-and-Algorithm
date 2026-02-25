#include <stdio.h>
#include <stdlib.h>

/*
* @name 节点结构体
* @param data		节点中存储的数据
* @param next		指向下一个节点的指针
*/
typedef struct Node {
	int data;
	struct Node* next;
} Node;

/*
* @name 链式队列结构体
* @param head		虚拟头节点
* @param tail		指向下一个真实节点
* @param size		当前的队列中的元素个数
*/
typedef struct {
	Node* head; 
	Node* tail; 
	size_t size;
} LinkedListQueue;

/*
* @name 初始化链式队列
* @param queue		结构体指针
*/
void initQueue(LinkedListQueue* queue) {
	queue->head = (Node*)malloc(sizeof(Node));
	if (!queue->head) {
		printf("初始化链式队列失败\n");
	}
	queue->head->next = NULL;
	queue->tail = queue->head;
	queue->size = 0;
	printf("链式队列初始化成功\n");
}

/*
* @name 返回链式队列个数
* @param queue		结构体指针
* @return 链式队列的元素个数
*/
size_t getSize(const LinkedListQueue* queue) {
	return queue->size;
}

/*
* @name 入队
* @param queue		结构体指针
* @param element	要插入的数据
*/
void enqueue(LinkedListQueue* queue, int element) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (!newNode) {
		printf("内存分配失败，入队失败\n");
		return;
	}
	newNode->data = element;
	newNode->next = NULL;
	queue->tail->next = newNode;
	queue->tail = newNode;
	queue->size++;
	printf("数据 %d 已入队\n", element);
}

/*
* @name 出队
* @param queue		结构体指针
*/
void dequeue(LinkedListQueue* queue) {
	if (queue->size == 0) {
		printf("队列为空，出队失败\n");
		return;
	}
	Node* first = queue->head->next;
	int value = first->data;
	queue->head->next = first->next;
	// 如果出队后队列变空，tail 回到 head
	if (queue->head->next == NULL) {
		queue->tail = queue->head;
	}
	free(first);
	queue->size--;
	printf("数据 %d 已出队\n", value);
}

/*
* @name 输出队列
* @param queue		结构体指针
*/
void printQueue(const LinkedListQueue* queue) {
	if (queue->size == 0) {
		printf("队列为空\n");
		return;
	}
	Node* cur = queue->head->next;
	while (cur != NULL) {
		printf("%d ", cur->data);
		cur = cur->next;
	}
	printf("\n");
}

/*
* @name 释放
* @param queue		结构体指针
*/
void destroyQueue(LinkedListQueue* queue) {
	Node* cur = queue->head;
	while (cur != NULL) {
		Node* temp = cur;
		cur = cur->next;
		free(temp);
	}
	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;
}

// 测试
int main() {
	LinkedListQueue queue;
	return 0;
}