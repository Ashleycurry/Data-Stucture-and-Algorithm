#include <stdio.h>

// 顺序查找函数 时间复杂度O(n)
int sequentialSearch(int arr[], int size, int target) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == target) {
			return i; // 找到目标，返回索引
		}
	}
	return -1; // 未找到目标，返回标识
}

int main() {
	int arr[] = { 4, 2, 8, 6, 1, 9, 5 };
	int size = sizeof(arr) / sizeof(arr[0]);
	int target = 6;

	printf("原始数组: ");
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	int result = sequentialSearch(arr, size, target);

	if (result != -1) {
		printf("元素 %d 找到，索引为 %d\n", target, result);
	}
	else {
		printf("元素 %d 未找到\n", target);
	}

	return 0;
}
