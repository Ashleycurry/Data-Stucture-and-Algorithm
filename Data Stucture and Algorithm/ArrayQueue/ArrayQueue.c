#include <stdio.h>
#include <stdlib.h>

/*
* @name 创建动态数组实现顺序队列
* @param data		存储队列元素
* @param capacity	逻辑数据
* @param size		物理数据
*/
typedef struct {
	int* data;
	size_t capacity;
	size_t size;
}ArrayQueue;

/*
* @name 初始化顺序队列
* @param queue		结构体指针
* @param capacity	初始化的逻辑大小
*/
void initArrayQueue(ArrayQueue* queue, size_t capacity){
	queue->data = (int*)malloc(capacity * sizeof(int));
	if (!queue->data) {
		printf("初始化顺序队列失败\n");
		return;
	}
	queue->capacity = capacity;
	queue->size = 0;
	printf("初始化顺序队列成功\n");
}

/*
* @name 返回顺序队列的大小
* @param queue		结构体指针
*/
size_t getSize(const ArrayQueue* queue) {
	return queue->size;
}

/*
* @name 扩容操作
* @param queue		结构体指针
* @param newsize	要扩展的新大小
*/
void resize(ArrayQueue* queue, size_t newsize) {
	int* newData = (int*)realloc(queue->data, newsize * sizeof(int));
	if (!newData) {
		printf("动态扩容失败\n");
		return;
	}
	queue->data = newData;
	queue->capacity = newsize;
	printf("原存储不够，扩容成功\n");
}

/*
* @name 入队
* @param queue		结构体指针
* @param element	要入队的数据
*/
void enqueue(ArrayQueue* queue, int element) {
	if (queue->size == queue->capacity) {
		resize(queue, queue->capacity + (queue->capacity << 1));
	}
	queue->data[queue->size] = element;
	queue->size++;
	printf("%d 入队成功\n");
}

/*
 * @name 出队
 * @param queue		结构体指针
 * @return: 队首元素
 */
int dequeue(ArrayQueue* queue) {
    if (queue->size == 0) {
        printf("队列为空，出队失败\n");
        return -1;
    }
    int frontNum = queue->data[0];
    for (size_t i = 0; i < queue->size - 1; i++) {
        queue->data[i] = queue->data[i + 1];
    }
    queue->size--;
    printf("数据 %d 已出队\n", frontNum);
    return frontNum;
}

/*
 * @name 打印当前队列中的所有元素
 * @param queue		结构体指针
 */
void printQueue(const ArrayQueue* queue) {
	if (queue->size == 0) {
		printf("队列为空\n");
		return;
	}
	for (size_t i = 0; i < queue->size; i++) {
		printf("%d ", queue->data[i]); // 依次打印每个元素
	}
	printf("\n");
}

/*
 * @name 释放
 * @param queue		结构体指针
 */
void destroyQueue(ArrayQueue* queue) {
    free(queue->data);             
    queue->data = NULL;           
    queue->capacity = 0;         
    queue->size = 0;
	printf("顺序队列已释放\n");
}

int main(int argc, char* argv[]) {
	ArrayQueue queue;
	return 0;
}