#include <stdio.h>
#include <stdlib.h>

// 获取数组最大值
int getMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}

// 对 arr[] 按照指定位数（exp=1,10,100...）进行计数排序（稳定）
void countingSortForRadix(int arr[], int n, int exp) {
    int *output = (int *)malloc(n * sizeof(int)); // 输出数组
    int count[10] = {0}; // 十进制，0-9 共10个桶

    // 1. 统计当前位上各数字（0-9）的出现次数
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
    }

    // 2. 计算前缀和，得到每个数字在 output 中的结束位置
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // 3. 从后往前遍历原数组，保证稳定性
    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    // 4. 将排序结果复制回原数组
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    free(output);
}

// 基数排序主函数
void radixSort(int arr[], int n) {
    // 1. 找到最大值，确定需要处理的位数
    int max_val = getMax(arr, n);

    // 2. 从个位开始，对每一位进行计数排序
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        countingSortForRadix(arr, n, exp);
    }
}

// 打印数组
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 主函数：测试
int main() {
    int arr[] = {170, 45, 75, 90, 2, 802, 24, 66};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("原始数组: ");
    printArray(arr, n);

    radixSort(arr, n);

    printf("排序后数组: ");
    printArray(arr, n); // 输出: 2 24 45 66 75 90 170 802
    return 0;
}