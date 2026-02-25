#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEVEL 16
#define PROMOTE_PROB 0.5  // 向上层提升的概率

/*
* @name 跳表节点
* @param key	节点键
* @param score	节点分数（用于排序）
* @param next	后继指针数组（每层的后继节点）
*/
typedef struct SkipListNode {
	char* key;
	double score;
	struct SkipListNode* next[];  // 柔性数组：每层的后继指针
} SkipListNode;

/*
* @name 跳表结构
* @param head 			虚拟头节点
* @param current_level	当前最大层数
*/
typedef struct SkipList {
	SkipListNode* head;   // 虚拟头节点（不存数据）
	int current_level;    // 当前最大层数（从 1 开始计，但代码中按 0-index 使用）
} SkipList;

// 创建新节点
static SkipListNode* create_node(int level, const char* key, double score) {
	size_t size = sizeof(SkipListNode) + level * sizeof(SkipListNode*);
	SkipListNode* node = (SkipListNode*)malloc(size);
	node->key = _strdup(key);
	node->score = score;
	for (int i = 0; i < level; ++i) {
		node->next[i] = NULL;
	}
	return node;
}

// 创建跳表
SkipList* skiplist_create(void) {
	SkipList* sl = (SkipList*)malloc(sizeof(SkipList));
	sl->current_level = 1;  // 初始为第 1 层（实际索引为 0）
	sl->head = create_node(MAX_LEVEL, "", 0.0);
	return sl;
}

// 随机生成节点层数（至少 1 层）
static int random_level(void) {
	int level = 1;
	while ((rand() / (double)RAND_MAX) < PROMOTE_PROB && level < MAX_LEVEL) {
		++level;
	}
	return level;
}

// 查找：返回节点指针，未找到返回 NULL
SkipListNode* skiplist_search(SkipList* sl, const char* key) {
	SkipListNode* current = sl->head;

	// 从最高层向下搜索
	for (int layer = sl->current_level - 1; layer >= 0; --layer) {
		while (current->next[layer] != NULL &&
			strcmp(current->next[layer]->key, key) < 0) {
			current = current->next[layer];
		}
	}

	current = current->next[0];  // 到达底层
	if (current != NULL && strcmp(current->key, key) == 0) {
		return current;
	}
	return NULL;
}

// 插入键值对（若 key 已存在，则更新 score）
void skiplist_insert(SkipList* sl, const char* key, double score) {
	SkipListNode* update[MAX_LEVEL];
	SkipListNode* current = sl->head;

	// 从高层到低层，记录每层的前驱节点
	for (int layer = sl->current_level - 1; layer >= 0; --layer) {
		while (current->next[layer] != NULL &&
			strcmp(current->next[layer]->key, key) < 0) {
			current = current->next[layer];
		}
		update[layer] = current;
	}

	current = current->next[0];

	// 如果 key 已存在，更新 score
	if (current != NULL && strcmp(current->key, key) == 0) {
		current->score = score;
		return;
	}

	// 生成新节点层数
	int new_level = random_level();

	// 如果新层数超过当前最大层数，扩展 update 数组
	if (new_level > sl->current_level) {
		for (int i = sl->current_level; i < new_level; ++i) {
			update[i] = sl->head;
		}
		sl->current_level = new_level;
	}

	// 创建新节点并插入各层
	SkipListNode* new_node = create_node(new_level, key, score);
	for (int i = 0; i < new_level; ++i) {
		new_node->next[i] = update[i]->next[i];
		update[i]->next[i] = new_node;
	}
}

// 删除指定 key，成功返回 1，失败返回 0
int skiplist_delete(SkipList* sl, const char* key) {
	SkipListNode* update[MAX_LEVEL];
	SkipListNode* current = sl->head;

	// 找到各层前驱
	for (int layer = sl->current_level - 1; layer >= 0; --layer) {
		while (current->next[layer] != NULL &&
			strcmp(current->next[layer]->key, key) < 0) {
			current = current->next[layer];
		}
		update[layer] = current;
	}

	current = current->next[0];

	if (current == NULL || strcmp(current->key, key) != 0) {
		return 0;  // 未找到
	}

	// 从各层删除节点
	for (int i = 0; i < sl->current_level; ++i) {
		if (update[i]->next[i] != current) break;
		update[i]->next[i] = current->next[i];
	}

	// 释放内存
	free(current->key);
	free(current);

	// 降低当前层数（如果顶层为空）
	while (sl->current_level > 1 && sl->head->next[sl->current_level - 1] == NULL) {
		--sl->current_level;
	}

	return 1;
}

// 打印所有元素（按 key 升序）
void skiplist_print(SkipList* sl) {
	SkipListNode* node = sl->head->next[0];
	while (node != NULL) {
		printf("(%s, %.1f) ", node->key, node->score);
		node = node->next[0];
	}
	printf("\n");
}

// 释放整个跳表
void skiplist_free(SkipList* sl) {
	SkipListNode* node = sl->head->next[0];
	while (node != NULL) {
		SkipListNode* next = node->next[0];
		free(node->key);
		free(node);
		node = next;
	}
	free(sl->head);
	free(sl);
}

// 测试
int main() {
	srand((unsigned)time(NULL));

	SkipList* sl = skiplist_create();

	skiplist_insert(sl, "Bob", 95.0);
	skiplist_insert(sl, "Alice", 100.0);
	skiplist_insert(sl, "Charlie", 105.0);
	skiplist_insert(sl, "David", 90.0);

	printf("插入后: ");
	skiplist_print(sl);

	SkipListNode* found = skiplist_search(sl, "Alice");
	if (found) {
		printf("查找到 Alice, score = %.1f\n", found->score);
	}

	skiplist_delete(sl, "Bob");
	printf("删除 Bob 后: ");
	skiplist_print(sl);

	skiplist_free(sl);
	return 0;
}