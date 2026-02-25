#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 堆类型：大顶堆 或 小顶堆
typedef enum {
	MAX_HEAP,
	MIN_HEAP
} HeapType;

// 堆结构体
typedef struct {
	int* data;          // 存储堆元素的数组
	int size;           // 当前元素个数
	int capacity;       // 最大容量
	HeapType type;      // 堆类型
} Heap;

// 辅助函数：比较两个元素，根据堆类型决定顺序
bool shouldSwap(Heap* heap, int parent, int child) {
	if (heap->type == MAX_HEAP) {
		return heap->data[parent] < heap->data[child];
	}
	else {
		return heap->data[parent] > heap->data[child];
	}
}

// 初始化堆
Heap* heap_init(int capacity, HeapType type) {
	Heap* heap = (Heap*)malloc(sizeof(Heap));
	if (!heap) return NULL;

	heap->data = (int*)malloc(sizeof(int) * capacity);
	if (!heap->data) {
		free(heap);
		return NULL;
	}

	heap->size = 0;
	heap->capacity = capacity;
	heap->type = type;
	return heap;
}

// 上浮操作（插入后调整）
void heap_up(Heap* heap, int index) {
	while (index > 0) {
		int parent = (index - 1) / 2;
		if (!shouldSwap(heap, parent, index)) break;

		// 交换
		int temp = heap->data[parent];
		heap->data[parent] = heap->data[index];
		heap->data[index] = temp;

		index = parent;
	}
}

// 下沉操作（删除堆顶后调整）
void heap_down(Heap* heap, int index) {
	int left, right, target;

	while (true) {
		left = 2 * index + 1;
		right = 2 * index + 2;
		target = index;

		if (left < heap->size && shouldSwap(heap, target, left))
			target = left;
		if (right < heap->size && shouldSwap(heap, target, right))
			target = right;

		if (target == index) break;

		// 交换
		int temp = heap->data[index];
		heap->data[index] = heap->data[target];
		heap->data[target] = temp;

		index = target;
	}
}

// 插入元素
bool heap_insert(Heap* heap, int value) {
	if (heap->size >= heap->capacity) {
		return false; // 堆已满
	}

	heap->data[heap->size] = value;
	heap_up(heap, heap->size);
	heap->size++;
	return true;
}

// 获取堆顶元素（不删除）
bool heap_peek(Heap* heap, int* value) {
	if (heap->size == 0) return false;
	*value = heap->data[0];
	return true;
}

// 删除堆顶元素
bool heap_pop(Heap* heap, int* value) {
	if (heap->size == 0) return false;

	*value = heap->data[0];
	heap->data[0] = heap->data[heap->size - 1];
	heap->size--;
	if (heap->size > 0) {
		heap_down(heap, 0);
	}
	return true;
}

// 释放堆内存
void heap_free(Heap* heap) {
	if (heap) {
		free(heap->data);
		free(heap);
	}
}

// 打印堆（调试用）
void heap_print(Heap* heap) {
	printf("Heap: ");
	for (int i = 0; i < heap->size; i++) {
		printf("%d ", heap->data[i]);
	}
	printf("\n");
}

// 示例主函数
int main() {
	// 创建一个最大堆
	Heap* max_heap = heap_init(10, MAX_HEAP);
	heap_insert(max_heap, 10);
	heap_insert(max_heap, 20);
	heap_insert(max_heap, 5);
	heap_insert(max_heap, 30);
	heap_print(max_heap); // 应该是 [30, 20, 5, 10] 或类似结构

	int val;
	heap_pop(max_heap, &val);
	printf("Popped from max heap: %d\n", val); // 应为 30

	// 创建一个最小堆
	Heap* min_heap = heap_init(10, MIN_HEAP);
	heap_insert(min_heap, 10);
	heap_insert(min_heap, 20);
	heap_insert(min_heap, 5);
	heap_insert(min_heap, 30);
	heap_print(min_heap); // 应该是 [5, 10, 20, 30] 或类似结构

	heap_pop(min_heap, &val);
	printf("Popped from min heap: %d\n", val); // 应为 5

	heap_free(max_heap);
	heap_free(min_heap);

	return 0;
}