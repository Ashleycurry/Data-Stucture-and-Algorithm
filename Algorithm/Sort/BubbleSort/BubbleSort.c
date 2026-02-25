#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//冒泡排序1 时间复杂度O(n^2)
void BubbleSort1(int arr[], int size) {
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - 1 - i; j++) {
			if (arr[j] > arr[j + 1]) { //默认升序，降序：arr[j] < arr[j+1];
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

//冒泡排序2 时间复杂度最好可达O(n)
void BubbleSort2(int arr[],int n){
	for(int i = 0 ; i < n - 1; i++){   
		//优化标志
		bool swapped = false; //是否需要交换 
		for(int j = 0; j < n - 1 -i;++j){
			if(arr[j] > arr[j + 1]){
				//交换元素
				int temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
				swapped = true; //标记有交换 
			} 
		}
		//提前终止检查
		if(!swapped) break;
	} 
} 

int main(int agrc, char* argv[]) {
	int arr[] = {23,45,2,46,77,2,99,-9,-32,0,66};
	int size = sizeof(arr) / sizeof(arr[0]);
	//升序 / 降序
	BubbleSort1(arr, size);
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	return 0;
}