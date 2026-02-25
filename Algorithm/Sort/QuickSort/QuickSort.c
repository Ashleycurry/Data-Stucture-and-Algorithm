#include <stdio.h>

// 交换两个整数
void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

// 分区函数（Lomuto 分区方案）
int partition(int arr[], int low, int high) {
	int pivot = arr[high]; // 选择最后一个元素作为基准
	int i = low - 1;       // i 是小于基准的区域的索引

	for (int j = low; j < high; j++) {
		//升序 / 降序 = arr[j] <= pivot / arr[j] >= pivot
		if (arr[j] <= pivot) {
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return i + 1; // 返回基准的正确位置
}

// 快速排序主函数
void quickSort(int arr[], int low, int high) {
	if (low < high) {
		int pi = partition(arr, low, high); // 获取分区索引
		quickSort(arr, low, pi - 1);        // 递归排序左半部分
		quickSort(arr, pi + 1, high);       // 递归排序右半部分
	}
}

// 打印数组
void printArray(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

// 主函数：测试快速排序
int main() {
	int arr[] = { 10, 7, 8, 9, 1, 5 };
	int size = sizeof(arr) / sizeof(arr[0]);

	printf("原始数组: ");
	printArray(arr, size);

	quickSort(arr, 0, size - 1);

	printf("排序后数组: ");
	printArray(arr, size);

	return 0;
}