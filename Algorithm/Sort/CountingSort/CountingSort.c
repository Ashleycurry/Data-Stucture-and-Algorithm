#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 计数排序函数
void countingSort(int arr[], int n, int max_val) {
	// 1. 创建计数数组，大小为 max_val + 1（因为包含 0）
	int* count = (int*)calloc(max_val + 1, sizeof(int));
	if (!count) {
		printf("内存分配失败！\n");
		return;
	}

	// 2. 统计每个元素出现的次数
	for (int i = 0; i < n; i++) {
		count[arr[i]]++;
	}

	// 3. 将排序结果写回原数组（按顺序展开）
	int index = 0;
	//for (int i = max_val; i >= 0; i--) { //从大到小
	for (int i = 0; i <= max_val; i++) { //从小到大
		while (count[i] > 0) {
			arr[index++] = i;
			count[i]--;
		}
	}

	// 4. 释放临时计数数组
	free(count);
}

// 打印数组
void printArray(int arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

// 主函数：测试计数排序
int main() {
	int arr[] = { 4, 2, 2, 8, 3, 3, 1 };
	int n = sizeof(arr) / sizeof(arr[0]);

	// 找出最大值（实际应用中应动态计算）
	int max_val = 8;

	printf("原始数组: ");
	printArray(arr, n);

	countingSort(arr, n, max_val);

	printf("排序后数组: ");
	printArray(arr, n);

	return 0;
}