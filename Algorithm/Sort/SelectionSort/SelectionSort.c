#include <stdio.h>

// 选择排序函数
void selectionSort(int arr[], int n) {
	for (int i = 0; i < n - 1; i++) { 
		int minIndex = i; // 假设当前位置 i 是未排序部分
		// 内层循环：在未排序部分 [i+1, n-1] 中找真正的最小值下标
		//升序 / 降序 = arr[j] < arr[minIndex] / arr[j] > arr[minIndex]
		for (int j = i + 1; j < n; j++) {
			if (arr[j] > arr[minIndex]) {
				minIndex = j;  // 更新最小值的下标
			}
		}
		// 如果最小值不在当前位置 i，则交换
		if (minIndex != i) {
			int temp = arr[i];
			arr[i] = arr[minIndex];
			arr[minIndex] = temp;
		}
	}
}

// 打印数组
void printArray(int arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

// 主函数：测试选择排序
int main() {
	int arr[] = { 64, 25, 12, 22, 11 };
	int n = sizeof(arr) / sizeof(arr[0]);
	printf("原始数组: ");
	printArray(arr, n);
	selectionSort(arr, n);
	printf("排序后数组: ");
	printArray(arr, n);
	return 0;
}