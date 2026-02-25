#include <stdio.h>

// 插入排序函数
void insertionSort(int arr[], int n) {
	int i, key, j;
	for (i = 1; i < n; i++) {
		key = arr[i];
		j = i - 1;
		//升序 / 降序  =  arr[j] > key / arr[j] < key
		while (j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

// 打印数组
void printArray(int arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main() {
	int arr[] = { 12, 11, 13, 5, 6 };
	int n = sizeof(arr) / sizeof(arr[0]);
	printf("原始数组: ");
	printArray(arr, n);
	insertionSort(arr, n);
	printf("排序后数组: ");
	printArray(arr, n);
	return 0;
}