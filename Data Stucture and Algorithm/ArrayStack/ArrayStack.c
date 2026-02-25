#include <stdio.h>
#include <stdlib.h>

/*
* @name 利用动态数组逻辑实现顺序栈
* @param data		指向动态数组的指针，存储数据
* @param capacity	逻辑数据数量
* @param size		物理数据数量
*/
typedef struct {
	int* data;
	size_t capacity;
	size_t size;
}Stack;

/*
* @name 初始化顺序栈
* @param stack		结构体指针
* @param capacity	逻辑数据数量
*/
void initStack(Stack* stack, int capacity) {
	stack->data = (int*)malloc(capacity * sizeof(int));
	stack->capacity = capacity;
	stack->size = 0;
}

/*
* @name 返回顺序栈中元素个数
* @param stack		结构体指针
* @return 栈中的物理数据个数
*/
size_t getSize(Stack* stack) {
	return stack->size;
}

/*
* @name 动态扩容
* @param stack			结构体指针
* @param newCapacit		要扩展的大小
*/
void resizeCapacity(Stack* stack, int newCapacity) {
	stack->data = (int*)realloc(stack->data, newCapacity * sizeof(int));
	stack->capacity = newCapacity;
}

/*
* @name 入栈
* @param stack			结构体指针
* @param element		要加入的数据
*/
void pushStack(Stack* stack, int element) {
	if (stack->size == stack->capacity) {
		resizeCapacity(stack, stack->capacity + (stack->capacity >> 1));//扩容1.5倍
	}
	stack->data[stack->size] = element;
	stack->size++;
	printf("数据 %d 已入栈\n",element);
}

/*
* @name 出栈
* @param stack			结构体指针
*/
void popStack(Stack* stack) {
	if (stack->size == 0) {
		printf("栈为空，出栈失败\n");
		return;
	}
	printf("数据 %d 已出栈\n",stack->data[--stack->size]);
}

/*
* @name 修改顺序栈中指定位置的数据
* @param stack			结构体指针
* @param index			要修改的下标
* @param newElement		要修改的数据
*/
void modifyAt(Stack* stack, size_t index, int newElement) {
	if (index < 0 || index >= stack->size) {
		printf("下标索引不合法\n");
		return;
	}
	stack->data[index] = newElement;
	printf("成功将下标 %d 的数据修改 %d\n",index,newElement);
}

/*
* @name 查找顺序栈中指定位置的数据
* @param stack			结构体指针
* @param index		    要查找的位置索引
*/
size_t getElementAt(Stack* stack, size_t index) {
	if (index < 0 || index >= stack->size) {
		printf("下标索引不合法\n");
		return -1;
	}
	return stack->data[index];
}

/*
* @name 释放顺序栈内存
* @param stack		结构体指针
*/
void destroyStack(Stack* stack) {
	free(stack->data);
	stack->data = NULL;
	stack->capacity = 0;
	stack->size = 0;
}

int main(int argc, char* argv[]) {
	Stack stack;

	return 0;
}