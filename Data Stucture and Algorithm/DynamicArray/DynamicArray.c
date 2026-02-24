#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/*
* @name 声明动态数组的结构体
* @param int *data			存储数据的数组   
* @param size_t capacity	记录数组的逻辑上限个数
* @param size				记录数组的物理个数
* @return DynamicArray		动态数组
*/
typedef struct {
	int *data;
	size_t capacity;
	size_t size;
}DynamicArray;

/*
* @name 动态数组初始化函数
* @param DynamicArray		结构体指针
* @param initialCapacity	初始化逻辑上限个数
*/
void initDynamicArray(DynamicArray* array, size_t initialCapacity) {
	array->data = (int *)malloc(initialCapacity * sizeof(int));
	array->capacity = initialCapacity;
	array->size = 0;
	printf("初始化动态数组成功，内存已申请\n");
}


/*
* @name 释放动态数组内存
* @param DynamicArray		结构体指针
*/
void destroyDynamicArray(DynamicArray* array) {
	free(array->data);
	array->capacity = 0;
	array->size = 0;
	printf("动态数组内存释放成功\n");
}

/*
* @name 调整动态数组的内存大小
* @param DynamicArray		结构体指针
* @param newCapacity		新的逻辑值（需要扩容的大小）	
*/
void resizeDynamicArray(DynamicArray* array, size_t newCapacity) {
	array->data = (int *)realloc(array->data, newCapacity * sizeof(int));
	array->capacity = newCapacity;
	printf("动态数组内存已调整\n");
}

/*
* @name 获取动态数组长度（元素个数）
* @param DynamicArray		结构体指针
* @return					返回数组当前的物理个数
*/
size_t getLength(const DynamicArray* array) {
	return array->size;
}

/*
* @name 在指定位置上插入新元素
* @param DynamicArray		结构体指针
* @param index				需要插入位置的索引
* @param element			准备插入的元素
*/
void insertAt(DynamicArray* array, size_t index, int element) {
	if (index < 0 || index > array->size){
		printf("index下标不合法, 插入失败\n");
		return ; 
	}
	if (array->size == array->capacity){
		resizeDynamicArray(array, array->capacity << 1);
	}
	//将index后续的元素依此后移
	for (size_t i = array->size; i > index; i--) {
		array->data[i] = array->data[i-1]; //最后一个元素的下标是array->size[i-1];
	}
	array->data[index] = element;
	array->size++;
	printf("指定位置插入元素完成\n");
}

/*
* @name 在末尾插入新元素
* @param DynamicArray		结构体指针
* @param element			准备插入的元素
*/
void insertEnd(DynamicArray* array, int element) {
	insertAt(array, array->size, element);
	printf("末尾插入元素已完成\n");
}

/*
* @name 删除指定位置的元素并返回被删除的元素
* @param DynamicArray		结构体指针
* @param index				指定删除元素位置的索引
*/
int deleteAt(DynamicArray* array, size_t index) {
	if (index < 0 || index >= array->size) {
		printf("index下标不合法, 删除失败\n");
		return -1; 
	}
	int deleteData = array->data[index];
	//更新数组，将index后续的元素依此前移（覆盖/删除index下标元素）
	for (size_t i = index; i < array->size; i++){
		array->data[i] = array->data[i + 1];
	}
	array->size--;
	return deleteData;
	printf("已删除指定位置的元素\n");
}

/*
* @name 删除末尾的元素并返回被删除的元素
* @param DynamicArray		结构体指针
*/
int deleteEnd(DynamicArray* array) {
	deleteAt(array, array->size-1);
	printf("已删除末尾位置的元素\n");
}

/*
* @name 遍历所有的元素
* @param DynamicArray		结构体指针
*/
void print(DynamicArray* array) {
	for (int i = 0; i < array->size; i++) {
		printf("%d ",array->data[i]);
	}
	printf("\n");
	printf("已成功遍历所有的元素\n");
}

int main() {
	DynamicArray array;

	return 0;
}