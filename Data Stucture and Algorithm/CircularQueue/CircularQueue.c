#include <stdio.h>
#include <stdlib.h>

/*
* @name 声明循环队列中的相关数据
* @param data		保存数据
* @param capacity	逻辑大小
* @param size		物理大小
* @param front		记录出队的索引位置-头指针
* @param rear		记录入队后的索引位置-尾指针
*/
typedef struct {
	int* data; 
	int capacity; 
	int size; 
	int front; 
	int rear; 
}CircularQueue;

/*
* @name 初始化队列
* @param capacity	设置逻辑大小
* @param queue		结构体指针
*/
void initQueue(CircularQueue* queue, size_t capacity) {
	queue->data = (int *)malloc(capacity * sizeof(int));
	queue->capacity = capacity;
	queue->size = 0;
	queue->front = 0;
	queue->rear = 0;
	printf("初始化队列成功\n");
}

/*
* @name 返回队列元素个数
* @param queue		结构体指针
*/
size_t getSize(const CircularQueue* queue) {
	return queue->size;
}

/*
 * @name 动态扩容函数
 * @param queue      队列指针
 * @param newCap     新容量
 */
void resize(CircularQueue* queue, int newCap) {
	int* newData = (int*)malloc(newCap * sizeof(int));
	if (!newData) {
		printf("扩容失败：内存不足\n");
		return;
	}
	// 将旧队列中元素按逻辑顺序（从 front 到 rear-1）拷贝到新数组开头
	for (int i = 0; i < queue->size; i++) {
		newData[i] = queue->data[(queue->front + i) % queue->capacity];
	}
	free(queue->data);          
	queue->data = newData;    
	queue->capacity = newCap;
	queue->front = 0;           
	queue->rear = queue->size;
	printf("扩容成功！\n");
}

/*
* @name 入队
* @param element	要入对的元素
* @param queue		结构体指针
*/
void enqueue(CircularQueue* queue, int element) {
	if (queue->size == queue->capacity) {
		printf("队列已满，进行扩容\n");
		resize(queue, queue->capacity + (queue->capacity >> 1));
	}
	queue->data[queue->rear] = element;
	queue->size++;
	// 加一取余总长，当队列满了
	// 使得指向队尾的rear重新指向开头，不会越界；
	queue->rear = (queue->rear + 1) % queue->capacity;
	printf("数据 %d 入队成功\n",element);
}

/*
* @name 出队
* @param queue		结构体指针
*/
void dequeue(CircularQueue* queue) {
	//当这条语句生效的时候，证明队列为满或为空
	//if (queue->front == queue->rear);
	if (queue->size == 0) {
		printf("队列为空，出队失败\n");
		return;
	}
	int dequeueData = queue->data[queue->front];
	queue->size--;
	queue->front = (queue->front + 1) % queue->capacity;
	printf("数据 %d 出队成功\n",dequeueData);
}

//遍历队列
void printQueue(CircularQueue* queue) {
	for (int i = queue->front,j = 0; i < queue->size; i++, j++) {
		printf("%d ",queue->data[i % queue->capacity]);
	}
	printf("\n");
}

//释放队列内存
void destroyQueue(CircularQueue* queue) {
	free(queue->data);
	queue->capacity = 0;
	queue->size = 0;
	queue->front = 0;
	queue->rear = 0;
	printf("队列成功释放\n");
}

int main(int argc, char *argv[]) { 
	CircularQueue queue;
	return 0;
}