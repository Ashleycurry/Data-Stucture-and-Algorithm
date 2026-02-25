#include <stdio.h>
#include <stdlib.h>

/*
* @name 定义循环链表结构体
* @param data	存储数据
* @param next	后继指针
*/
typedef struct Node {
	int data;
	struct Node* next;
} Node;

/*
* @name 定义循环链表的虚拟头节点结构体
* @param size	节点个数
* @param next	指向实际的第一个数据节点
*/
typedef struct{
	size_t size;
	Node* next;
}CircularLinkedList;

/*
* @name 初始化循环链表
* @param CircularLinkedList		结构体指针
*/
void initCircularLinkedList(CircularLinkedList* clist) {
	if (!clist) {
		printf("循环链表初始化失败\n");
		return;
	}
	clist->size = 0;
	clist->next = NULL;
	printf("初始化循环链表成功\n");
}

/*
* @name 返回循环链表长度
* @param CircularLinkedList		结构体指针
*/
size_t getLength(const CircularLinkedList* clist) {
	return clist->size;
}

/*
* @name 根据下标位置返回节点的结构体指针
* @param CircularLinkedList		结构体指针
* @param index					需要返回节点的下标
* @return
*/
static Node* getNodeAt(const CircularLinkedList* clist, size_t index) {
	if (index >= clist->size) {
		printf("要返回的指针下标不合法\n");
		return NULL;
	}
	Node* current = clist->next;
	for (int i = 0; i < index; i++) {
		current = current->next;
	}
	return current;
}

/*
* @name 在循环链表指定位置插入节点
* @param CircularLinkedList		结构体指针
* @param index					需要插入节点的下标
* @param data					需要插入的数据
*/
void insertAt(CircularLinkedList* clist, size_t index, int data) {
	if (index < 0 || index > clist->size) {
		printf("要插入位置的下标不合法\n");
		return ;
	}
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (!newNode) {
		printf("节点内存申请失败\n");
		return ;
	}
	newNode->data = data;
	if (clist->size == 0) {
		newNode->next = newNode;
		clist->next = newNode;
	} else {
		if (index == 0) {
			//插入到头部
			Node* tail = getNodeAt(clist, clist->size - 1);
			newNode->next = clist->next;
			tail->next = newNode;
			clist->next = newNode;
		} else {
			//插入到指定位置
			Node* prev = getNodeAt(clist, index - 1);
			newNode->next = prev->next;
			prev->next = newNode;
			}
	}
	clist->size++;
}

/*
* @name 在循环链表尾部插入元素
* @param CircularLinkedList		结构体指针
* @param data					需要插入的数据
* @return
*/
void insertEnd(CircularLinkedList* clist, int data) {
	return insertAt(clist, clist->size, data);
}

/*
* @name 删除循环链表中指定位置的数据并返回
* @param CircularLinkedList		结构体指针
* @param index					需要删除节点的下标
* @param outData					需要返回的数据
* @return 返回被删除的数据
*/
size_t deleteAt(CircularLinkedList* clist, size_t index) {
	if (index >= clist->size || clist->size == 0) {
		printf("下标不合法/链表中没元素\n");
		return 0;
	}
	int outData = 0;
	Node* delNode;
	if (clist->size == 1) {
		delNode = clist->next;
		outData = delNode->data;
		free(delNode);
		clist->next = NULL;
	} else {
		if (index == 0) {
			// 删除头节点
			Node* tail = getNodeAt(clist, clist->size - 1);
			delNode = clist->next;
			clist->next = delNode->next;
			tail->next = clist->next;
		} else {
			Node* prev = getNodeAt(clist, index - 1);
			delNode = prev->next;
			prev->next = delNode->next;
			}
		if (outData) outData = delNode->data;
		free(delNode);
	}
	clist->size--;
	printf("成功删除节点\n");
	return outData;
}

/*
* @name 删除循环链表尾部元素
* @param CircularLinkedList		结构体指针
*/
void deleteEnd(CircularLinkedList* clist) {
	deleteAt(clist, clist->size - 1);
	printf("已删除尾部元素 %d",clist->size-1);
}

/*
* @name 修改循环链表指定位置的元素
* @param CircularLinkedList		结构体指针
* @param index					需要修改节点的下标
* @param outData				需要修改的数据
* @return
*/
void modifyAt(CircularLinkedList* clist,size_t index,int newData) {
	Node* node = getNodeAt(clist, index);
	node->data = newData;
	printf("成功修改数据为 %d\n",node->data);
}

/*
* @name 正向遍历循环链表
* @param CircularLinkedList		结构体指针
* @return
*/
void printForward(const CircularLinkedList* clist) {
	if (clist->size == 0) {
		printf("循环链表中没有元素，无法遍历\n");
		return ;
	}
	Node* current = clist->next;
	for (size_t i = 0; i < clist->size; i++) {
		printf("%d ", current->data);
		current = current->next;
	}
	printf("\n");
}

/*
* @name 反向遍历循环链表
* @param
* @param
* @return
*/
void printBackward(const CircularLinkedList* clist) {
	if (clist->size == 0) {
		printf("循环链表中没有元素，无法反向遍历\n");
		return ;
	}
	int* stack = (int*)malloc(clist->size * sizeof(int));
	if (!stack) {
		printf("栈存储空间申请失败\n");
		return ;
	}
	Node* current = clist->next;
	for (int i = 0; i < clist->size; i++) {
		stack[i] = current->data;
		current = current->next;
	}
	for (int i = clist->size-1; i >= 0; i--) {
		printf("%d ",stack[i]);
	}
	printf("\n");
	free(stack);
}

/*
* @name 释放循环链表
* @param
* @return
*/
void destroyCircularLinkedList(CircularLinkedList* clist) {
	Node* current = clist->next;
	for (size_t i = 0; i < clist->size; i++) {
		Node* temp = current;
		current = current->next;
		free(temp);
	}
	clist->next = NULL;
	clist->size = 0;
	printf("循环链表释放完成");
}

int main(int argc, char* argv[]) {
	CircularLinkedList clist;
	return 0;
}