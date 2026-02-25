#include <stdio.h>
#include <stdlib.h>

/*
* @name 双向链表节点结构体
* @param data		数据域
* @param next		后继指针
* @param prev		前驱指针
*/
typedef struct Node{
	int data;
	struct Node* next;
	struct Node* prev;
}Node;

/*
* @name 定义带虚拟头结点的双向链表结构体
* @param size		节点个数
* @param next		指向实际的第一个数据节点
* @param prev //虚拟头节点不存储数据，无prev
*/
typedef struct{
	size_t size;
	Node* next;
}DoublyLinkedList;

/*
* @name 初始化双向链表
* @param DoublyLinkedList	结构体指针
*/
void initDoublyLinkedList(DoublyLinkedList* dlist) {
	dlist->size = 0;
	dlist->next = NULL;
	printf("双向链表初始化完成\n");
}

/*
* @name 双向链表长度
* @param DoublyLinkedList	结构体指针
*/
size_t getLength(const DoublyLinkedList* dlist) {
	return dlist->size;
}

/*
* @name 根据下表位置返回节点的结构体指针
* @param DoublyLinkedList	结构体指针
* @param index				要返回位置的下标
* @return 返回节点的结构体指针
*/
static Node* getNodeAt(const DoublyLinkedList* dlist, size_t index) {
	if (index >= dlist->size) return NULL;
	Node* currentment = dlist->next;;
	for (size_t i = 0; i < index; i++) {
		currentment = currentment->next;
	}
	return currentment;
}

/*
* @name 在双向链表指定位置插入节点
* @param DoublyLinkedList	结构体指针
* @param index				要插入位置的下标
* @param Element			要插入的数据
*/
void insertAt(DoublyLinkedList* dlist, size_t index, int Element) {
	if (index < 0 || index > dlist->size) {
		printf("插入的位置超出范围\n");
		return ;
	}
	Node* node = (Node*)malloc(sizeof(Node));
	if (!node) {
		printf("内存申请失败\n");
		return ;
	}
	node->data = Element;
	//头节点插入
	if (index == 0) {
		node->next = dlist->next;
		node->prev = NULL;
		if (dlist->next != NULL) {
			dlist->next->prev = node;
		}
		dlist->next = node;
	} else {
		//下标节点插入
		Node* prevNode = getNodeAt(dlist, index - 1);
		Node* nextNode = prevNode->next;
		//插入 node 在 prevNode 和 nextNode 之间
		prevNode->next = node;
		node->prev = prevNode;
		node->next = nextNode;
		if (nextNode != NULL) {
			nextNode->prev = node;
		}
	}
	dlist->size++;
	printf("数据 %d 插入 [%u] 成功\n",Element,index);
}

/*
* @name 在末尾插入元素
* @param DoublyLinkedList	结构体指针
* @param Element			要插入位置的数据
*/
void insertEnd(DoublyLinkedList* dlist, int Element) {
	insertAt(dlist, dlist->size, Element);
}

/*
* @name 删除指定位置的数据并且返回数据
* @param DoublyLinkedList	结构体指针
* @param index				要删除数据的下标
* @return 返回被删除的数据
*/
int deleteAt(DoublyLinkedList* dlist, size_t index) {
	if (index < 0 || index >= dlist->size) {
		printf("删除位置超出范围\n");
		return -1;
	}
	Node* delNode = getNodeAt(dlist, index);
	int value = delNode->data;
	Node* prevNode = delNode->prev;
	Node* nextNode = delNode->next;
	if (prevNode == NULL) {
		//删除的是第一个数据节点
		dlist->next = nextNode;
	} else { 
		prevNode->next = nextNode;
	}
	if (nextNode != NULL) {
		nextNode->prev = prevNode;
	}
	free(delNode);
	dlist->size--;
	return value;
}

/*
* @name 删除末尾元素
* @param DoublyLinkedList	结构体指针
* @return 被删除的元素的数据
*/
int deleteEnd(DoublyLinkedList* dlist) {
	if (dlist->size == 0) {
		printf("链表为空，无法删除末尾元素\n");
		return -1;
	}
	return deleteAt(dlist, dlist->size - 1);
}

/*
* @name 修改指定位置的元素
* @param DoublyLinkedList	结构体指针
* @param index				要修改数据的下标
* @param newValue			修改的数据
*/
void modifyAt(DoublyLinkedList* dlist, size_t index, int newValue) {
	if (index < 0 || index >= dlist->size) {
		printf("修改失败，下标不合法\n");
		return ;
	}
	Node* node = getNodeAt(dlist, index);
	node->data = newValue;
}

/*
* @name 正向遍历双向链表
* @param DoublyLinkedList 	结构体指针
*/
void printForward(const DoublyLinkedList* dlist) {
	if (dlist->size == 0) {
		printf("正向遍历失败，链表中没有元素\n");
		return ;
	}
	Node* currentNode = dlist->next;
	int count = 0;
	while (currentNode != NULL) {
		printf("第 %d 个节点元素是 %d\n", count++, currentNode->data);
		currentNode = currentNode->next;
	}
}

/*
* @name 反向遍历双向链表
* @param DoublyLinkedList 	结构体指针
*/
void printBackward(const DoublyLinkedList* dlist) {
	if (dlist->size == 0) {
		printf("反向遍历失败，链表中没有元素\n");
		return ;
	}
	Node* currentNode = dlist->next;
	while (currentNode != NULL && currentNode->next != NULL) {
		currentNode = currentNode->next;
	}
	while (currentNode != NULL) {
		int count = dlist->size - 1;
		currentNode = currentNode->prev;
		printf("第 %d 个节点元素是 %d\n", count, currentNode->data);
		count--;
	}
}

/*
* @name 释放双向链表内存
* @param DoublyLinkedList 	结构体指针
*/
void destroyLinkedList(DoublyLinkedList* dlist) {
	Node* current = dlist->next;
	while (current != NULL) {
		Node* next = current->next;
		free(current);
		current = next;
	}
	dlist->next = NULL;
	dlist->size = 0;
	printf("双向链表销毁完成\n");
}

int main(int argc,char* argv[]) {
	DoublyLinkedList dlist;
	return 0;
}