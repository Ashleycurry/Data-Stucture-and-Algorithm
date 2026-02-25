#include <stdio.h>
#include <stdlib.h>

// 合并两个已排序的子数组 arr[left..mid] 和 arr[mid+1..right]
void merge(int arr[], int left, int mid, int right) {
	int i, j, k;
	int n1 = mid - left + 1;      // 左子数组长度
	int n2 = right - mid;         // 右子数组长度
	// 创建临时数组
	int* L = (int*)malloc(n1 * sizeof(int));
	int* R = (int*)malloc(n2 * sizeof(int));
	// 拷贝数据到临时数组
	for (i = 0; i < n1; i++)
		L[i] = arr[left + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[mid + 1 + j];
	// 合并临时数组回 arr[left..right]
	i = 0; // L 的初始索引
	j = 0; // R 的初始索引
	k = left; // arr 的初始索引
	while (i < n1 && j < n2) {
		//升序 / 降序 = L[i] <= R[j] / L[i] >= R[j]
		if (L[i] <= R[j]) { 
			arr[k] = L[i];
			i++;
		} else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}
	// 复制 L[] 中剩余的元素（如果有）
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}
	// 复制 R[] 中剩余的元素（如果有）
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
	// 释放临时内存
	free(L);
	free(R);
}

// 归并排序主函数
void mergeSort(int arr[], int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2; // 防止溢出
		// 递归排序左右两半
		mergeSort(arr, left, mid);
		mergeSort(arr, mid + 1, right);
		// 合并已排序的两半
		merge(arr, left, mid, right);
	}
}

// 打印数组
void printArray(int arr[], int size) {
	for (int i = 0; i < size; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

// 主函数：测试归并排序
int main() {
	int arr[] = { 38, 27, 43, 3, 9, 82, 10 };
	int arr_size = sizeof(arr) / sizeof(arr[0]);
	printf("原始数组: ");
	printArray(arr, arr_size);
	mergeSort(arr, 0, arr_size - 1);
	printf("排序后数组: ");
	printArray(arr, arr_size);

	return 0;
}