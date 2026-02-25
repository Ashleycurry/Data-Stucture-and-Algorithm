#include <stdio.h>
#include <stdlib.h>

// ========================
// 非递归（迭代）二分查找
// ========================
int binarySearchIterative(int arr[], int size, int target) {
	int low = 0;
	int high = size - 1;

	while (low <= high) {
		int mid = low + (high - low) / 2; // 防止 (low + high) 溢出

		if (arr[mid] == target) {
			return mid;
		}
		else if (arr[mid] > target) {
			high = mid - 1;  // 升序：目标在左半部分
		}
		else {
			low = mid + 1;   // 升序：目标在右半部分
		}
	}
	return -1; // 未找到
}

// ========================
// 递归二分查找（辅助函数）
// 递归二分查找（对外接口）
// ========================
int binarySearchRecursiveHelper(int arr[], int low, int high, int target) {
	if (low > high) {
		return -1; // 未找到
	}

	int mid = low + (high - low) / 2;

	if (arr[mid] == target) {
		return mid;
	}
	else if (arr[mid] > target) {
		return binarySearchRecursiveHelper(arr, low, mid - 1, target);
	}
	else {
		return binarySearchRecursiveHelper(arr, mid + 1, high, target);
	}
}

int binarySearchRecursive(int arr[], int size, int target) {
	return binarySearchRecursiveHelper(arr, 0, size - 1, target);
}

// ========================
// 主函数
// ========================
int main() {
	int arr[] = { 4, 7, 9, 12, 16, 19, 22, 23, 24, 57, 69, 78, 85, 90, 95 };
	int size = sizeof(arr) / sizeof(arr[0]);

	int target = 12;

	printf("数组: ");
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	// 测试非递归版本
	int index1 = binarySearchIterative(arr, size, target);
	if (index1 != -1) {
		printf("【迭代】找到元素 %d，下标为 [%d]\n", target, index1);
	}
	else {
		printf("【迭代】未找到元素 %d\n", target);
	}

	// 测试递归版本
	int index2 = binarySearchRecursive(arr, size, target);
	if (index2 != -1) {
		printf("【递归】找到元素 %d，下标为 [%d]\n", target, index2);
	}
	else {
		printf("【递归】未找到元素 %d\n", target);
	}

	return 0;
}