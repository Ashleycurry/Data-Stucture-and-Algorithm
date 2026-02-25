#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 插入排序（用于对单个桶排序）
void insertionSort(int arr[], int n) {
	for (int i = 1; i < n; i++) {
		int key = arr[i];
		int j = i - 1;
		//默认升序，降序 改为 arr[j] < key
		while (j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

// 桶排序主函数
void bucketSort(int arr[], int n, int max_val) {
	// 创建桶：每个桶是一个动态数组
	int bucket_count = n; // 通常桶的数量等于元素个数
	int** buckets = (int**)calloc(bucket_count, sizeof(int*));
	int* bucket_sizes = (int*)calloc(bucket_count, sizeof(int));
	int* bucket_capacities = (int*)calloc(bucket_count, sizeof(int));

	// 初始化每个桶（初始容量为 2）
	for (int i = 0; i < bucket_count; i++) {
		bucket_capacities[i] = 2;
		buckets[i] = (int*)malloc(2 * sizeof(int));
	}

	// 将元素分配到桶中
	for (int i = 0; i < n; i++) {
		// 计算桶索引：将 [0, max_val] 映射到 [0, bucket_count-1]
		int index = (arr[i] * bucket_count) / (max_val + 1);
		if (index >= bucket_count) index = bucket_count - 1; // 边界保护

		// 如果桶满了，扩容
		if (bucket_sizes[index] >= bucket_capacities[index]) {
			bucket_capacities[index] *= 2;
			buckets[index] = (int*)realloc(buckets[index], bucket_capacities[index] * sizeof(int));
		}

		buckets[index][bucket_sizes[index]++] = arr[i];
	}

	// 对每个非空桶进行插入排序，并放回原数组
	int k = 0;
	//默认升序 降序：
	//for (int i = bucket_count - 1; i>=0; i--){
	for (int i = 0; i < bucket_count; i++) {
		if (bucket_sizes[i] > 0) {
			insertionSort(buckets[i], bucket_sizes[i]);
			for (int j = 0; j < bucket_sizes[i]; j++) {
				arr[k++] = buckets[i][j];
			}
		}
	}

	// 释放内存
	for (int i = 0; i < bucket_count; i++) {
		free(buckets[i]);
	}
	free(buckets);
	free(bucket_sizes);
	free(bucket_capacities);
}

// 打印数组
void printArray(int arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

// 主函数：测试桶排序
int main() {
	int arr[] = { 78, 17, 39, 26, 72, 94, 21, 12, 23, 68 };
	int n = sizeof(arr) / sizeof(arr[0]);
	int max_val = 94; // 已知最大值（实际中可先遍历求出）

	printf("原始数组: ");
	printArray(arr, n);

	bucketSort(arr, n, max_val);

	printf("排序后数组: ");
	printArray(arr, n);

	return 0;
}