#include <stdio.h>
#include <stdlib.h>

/*
* @name 单链表节点结构体
* @param data		节点存储的数据
* @param next		指向下一个节点的指针
*/
typedef struct Node{
	int data;
	struct Node* next;
}Node;

/*
* @name 带虚拟头结点的单链表结构体
* @param size		当前链表中的数据个数
* @param next		指向第一个实际数据节点
*/
typedef struct {
	int size;
	Node* next;
}LinkedList;

/*
* @name 初始化链表
* @param LinkedList	 结构体指针
*/
void initLinkedList(LinkedList* list) {
	list->size = 0;
	list->next = NULL;
	printf("初始化链表成功\n");
}

/*
* @name 返回链表的长度
* @param LinkedList	  结构体指针
* @return 单链表中节点个数
*/
size_t getLength(const LinkedList* list) {
	return list->size;
}

/*
* @name 在指定位置插入元素
* @param LinkedList 	结构体指针
* @param index			指定位置的索引
* @param element		要插入的元素
*/
void insertAt(LinkedList* list, size_t index, int element) {
	if (index < 0 || index > list->size) {
		printf("输入数据的index数据非法\n");
		return ;
	}
	Node* node = (Node *)malloc(1 * sizeof(Node));
	node->data = element;
	//插入头结点处
	if (index == 0) {
		node->next = list->next; 
		list->next = node;
	} else {
		//插入索引处
		Node* currentNode = list->next; //指向有数据的首元素
		for (int i = 0; i<index-1; i++) {
			currentNode = currentNode->next;
		}
		node->next = currentNode->next;
		currentNode->next = node;
	}
	list->size++;
	printf("成功在链表中 [%d] 插入了数据 %d \n", index, element);
}

/*
* @name 在末尾插入元素
* @param LinkedList 	结构体指针
* @param element		要插入的元素
*/
void insertEnd(LinkedList* list, int element) {
	insertAt(list, list->size, element);
}

/*
* @name 删除指定位置的元素并返回被删除的元素
* @param LinkedList 	结构体指针
* @param index			要删除位置的索引
*/
int deleteAt(LinkedList* list, size_t index) {
	if (index < 0 || index >= list->size) {
		printf("删除输入的index下标不合法\n");
		return -1;
	}
	int deleteElement; //要删除的位置数据
	Node* deleteNode; //要删除的节点
	//删除头结点
	if (index == 0) {  
		deleteNode = list->next;
		list->next = deleteNode->next;
		deleteElement = deleteNode->data;
	} else {          
		//删除中间/结尾结点
		Node* currentNode = list->next; 
		for (int i = 0; i < index-1; i++) {
			currentNode = currentNode->next;
		}
		//currentNode指向要删除节点的前一个节点
		deleteNode = currentNode->next; 
		currentNode->next = deleteNode->next;
		deleteElement = deleteNode->data;
	}
	free(deleteNode); //释放空间
	list->size--;
	return deleteElement;
}

/*
* @name 删除末尾元素
* @param LinkedList 	结构体指针
* @param index			要删除位置的索引
*/
int deleteEnd(LinkedList* list) {
	deleteAt(list, list->size - 1);
}

/*
* @name 修改指定位置的元素
* @param LinkedList 	结构体指针
* @param index			要修改位置的索引
* @param newValue		要修改的新值
*/
void modifyAt(LinkedList* list, size_t index, int newValue) {
	if (index < 0 || index >= list->size) {
		printf("要修改的index下标不合法\n");
		return -1;
	}
	Node* currentNode = list->next;
	for (int i = 0; i < index; i++) {
		currentNode = currentNode->next;
	}
	currentNode->data = newValue; //改修值
}

/*
* @name 获取指定位置的元素
* @param LinkedList 	结构体指针
* @param index			要获取位置的索引
*/
int getElementAt(const LinkedList* list, size_t index) {
	if (index < 0 || index >= list->size) {
		printf("获取的index下标不合法\n");
		return;
	}
	Node* currentNode = list->next;
	for (int i = 0; i < index; i++) {
		currentNode = currentNode->next;
	}
	printf("成功获取了 [%d] 的元素 %d\n", index, currentNode->data);
	return currentNode->data;
}

/*
* @name 查找链表中中间节点的数据
* @param LinkedList 	结构体指针
*/
void getMiddleAt(const LinkedList* list) {
	int target = getElementAt(list, (list->size - 1) / 2);
	printf("链表中的中间值为 %d\n", target);
}

/*
* @name 查找倒数第k个元素（快慢指针）   
* @param LinkedList 	结构体指针
* @param k				要查找倒数第k位置
*/
void getKthFromEnd(const LinkedList* list, int k) {
	if (list->size == 0 || k > list->size) {
		printf("无效的k值或链表为空\n");
		return;
	}
	Node* fast = list->next;
	Node* slow = list->next;
	// 快指针先走 k-1 步（因为k=1表示最后一个）
	for (int i = 0; i < k - 1; i++) {
		fast = fast->next;
	}
	// 快慢指针同时前进，直到fast到达最后一个节点
	while (fast->next != NULL) {
		slow = slow->next;
		fast = fast->next;
	}
	printf("倒数第 %d 个节点的值是: %d\n", k, slow->data);
}

/*
* @name 合并链表并且升序排序
* @param LinkedList 	结构体指针1
* @param LinkedList 	结构体指针2
* @param LinkedList 	结构体指针3
*/
//升序回调函数
int cmp(const void* a, const void* b) {
	return (*(const int*)a) - (*(const int*)b); //升序 
}
void mergeAndsortLists(LinkedList* list1, LinkedList* list2, LinkedList* list3) {
	//初始化 
	initLinkedList(list3);
	//判断是否遗漏 
	int total_size = list1->size + list2->size;
	if (total_size == 0) {
		printf("合并完成\n");
		return;
	}
	//开辟空间将链表数据转到数组
	int* arr = (int*)malloc(total_size * sizeof(int));
	if (arr == NULL) {
		printf("内存分配失败\n");
		return;
	}
	//将list1和list2的数据拷贝到数组
	int index = 0;
	Node* curr = list1->next;
	while (curr != NULL) {
		arr[index++] = curr->data;
		curr = curr->next;
	}
	curr = list2->next;
	while (curr != NULL) {
		arr[index++] = curr->data;
		curr = curr->next;
	}
	//升序排序数组
	qsort(arr, total_size, sizeof(int), cmp);
	//排序后的数据数据插入新链表list3中 
	for (int i = 0; i < total_size; i++) {
		insertAt(list3, i, arr[i]);
	}
	free(arr);

	printf("拼接并升序完成，新链表大小: %d\n", list3->size);
	printf("新链表list3: ");
	Node* temp = list3->next;
	while (temp != NULL) {
		printf("%d ", temp->data);
		temp = temp->next;
	}
	printf("\n");
}

/*
* @name 反转链表（三指针法）
* @param LinkedList 	结构体指针
*/
void reverseList(LinkedList* list) {
	if (list->size == 0) {
		printf("反转失败，没有数据\n");
		return;
	}
	Node* curr = list->next;
	Node* next = NULL;
	Node* prev = NULL;
	while (curr != NULL) {
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	list->next = prev; //prev成为首个节点 
	printf("成功反转链表\n");
}

/*
* @name 检测链表是否循环/带环（快慢指针法）
* @param LinkedList 	结构体指针
*/
void isCycleList(const LinkedList* list) {
	// 安全检查：空链表或无数据节点
	if (list->size == 0) {
		printf("链表中没有元素，无法判断\n");
		return;
	}
	//初始化指向第一个实际结点 
	Node* slow = list->next;
	Node* fast = list->next;

	while (fast != NULL && fast->next != NULL) {
		slow = slow->next;           //走1步
		fast = fast->next->next;     //走2步
		if (slow == fast) {
			printf("链表有环\n");
			return;
		}
	}
	printf("链表无环\n");
}

/*
* @name 释放链表内存
* @param LinkedList 	结构体指针
*/
void destroyLinkedList(LinkedList* list) {
	Node* currentNode = list->next;
	for (int i = 0; i < list->size; i++) {
		Node* tempNode = currentNode;
		currentNode = currentNode->next;
		free(tempNode);
	}
	list->next = NULL;
	list->size = 0;
	printf("链表已经成功释放\n");
}

int main(int argc, char *argv[]) {
	LinkedList list;

	return 0;
}