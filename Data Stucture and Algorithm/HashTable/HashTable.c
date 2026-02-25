#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
* @name 哈希表节点状态
* @param EMPTY		空槽
* @param OCCUPIED	已占用
* @param DELETED	已删除（用于开放寻址）
*/
typedef enum {
	EMPTY,
	OCCUPIED,
	DELETED
}EntryState;

/*
* @name 键值对结构
* @param key		动态分配的字符串键
* @param value		整型值
* @param state		槽位当前的状态
*/
typedef struct {
	char* key;
	int value;
	EntryState state;
}HashEntry;

/*
* @name 哈希表的结构
* @param entries	指向哈希桶数组的指针
* @param size		表示总容量（桶的数量）
* @param count		当前实际存储的有效元素数量（OCCUPIED）
*/
typedef struct {
	HashEntry* entries;
	size_t size;
	size_t count;
}HashTable;

/*
* @name 全局变量；支持渐进式rehash的双表结构
* @param table		当前主哈希表（新表）
* @param old_table	正在迁移中的旧的哈希表
*/
static HashTable* table = NULL;
static HashTable* old_table = NULL;

/*
* @name 辅助函数：创建一个指定大小的新哈希表
* @param size	要创建的大小
*/
HashTable* create_table(size_t size) {
	HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
	if (!ht) return NULL;
	ht->entries = (HashEntry*)calloc(size, sizeof(HashEntry));
	if (!ht->entries) {
		free(ht);
		return NULL;
	}
	ht->size = size;
	ht->count = 0;
	return ht;
}

/*
* @name 辅助函数：安全释放整个哈希表（包括key字符串）
* @param ht		哈希表结构体指针
*/
void destroy_table(HashTable* ht) {
	if (!ht) return;
	for (size_t i = 0; i < ht->size; ++i) {
		if (ht->entries[i].state == OCCUPIED) {
			free(ht->entries[i].key);
		}
	}
	free(ht->entries);
	free(ht);
}

/*
* @name 哈希表的三大组件之一：哈希函数
*		对字符串key计算哈希值（模表大小）
*		使用经典的DJB2算法：h = h * 31 + c；
* @param key		哈希表三大件之一：关键字
* @param table_size
* @return 哈希表三大件之一：哈希值
* @tips 把任意字符串转换成一个固定范围内的整数，作为其在哈希表中的“位置”（索引）
*/
unsigned long hash(const char* key, size_t table_size) {
	unsigned long h = 0;
	while (*key) {
		h = (h * 31 + *key) % table_size;
		key++;
	}
	return h;
}

/*
* @name 在指定哈希表中查找key，返回其索引，没有返回-1
* @param ht		哈希表结构体指针
* @param key	关键字
* @return 返回关键字的索引
*/
int find_in_table(HashTable* ht, const char* key) {
	if (!ht) return -1;
	size_t index = hash(key, ht->size);
	size_t start = index;
	do {
		HashEntry* entry = &ht->entries[index];
		if (entry->state == EMPTY) {
			return -1; //空槽说明key不存在，探测断了
		}
		if (entry->state == OCCUPIED && strcmp(entry->key, key) == 0) {
			return (int)index; //找到匹配项
		}
		index = (index + 1) % ht->size; //线性探测：下一个槽位
	} while (index != start);
	return -1;
}

/*
* @name 全局查找：先找新表table，再找旧表oldtable
* @param key	关键字
* @return >=0	在table中的索引
*		  -2	在old_table中存在
*		  -1	完全不存在
*/
int find_global(const char* key) {
	int idx = find_in_table(table, key);
	if (idx != -1) return idx;
	if (old_table) {
		idx = find_in_table(old_table, key);
		if (idx != -1) return -2;
	}
	return -1;
}

/*
* @name 底层插入函数：直接向指定哈希表插入键值对（不触发扩容或 rehash）
*       仅用于迁移阶段，避免递归调用 put
* @param ht     目标哈希表
* @param key    关键字
* @param value  整型数据
* @return 成功 true，失败 false
*/
static bool _put_raw(HashTable* ht, const char* key, int value) {
	if (!ht) return false;
	size_t index = hash(key, ht->size);
	size_t start = index;
	do {
		HashEntry* entry = &ht->entries[index];
		if (entry->state != OCCUPIED) {
			entry->key = (char*)malloc(strlen(key) + 1);
			if (!entry->key) return false;
			strcpy(entry->key, key);
			entry->value = value;
			entry->state = OCCUPIED;
			ht->count++;
			return true;
		}
		if (strcmp(entry->key, key) == 0) {
			entry->value = value;
			return true;
		}
		index = (index + 1) % ht->size;
	} while (index != start);
	return false;
}

/*
* @name 插入或更新键值对
*		核心函数：支持渐进式 rehash
* @param key	关键字
* @param value	整形数据
* @return 成功 true
*		  失败 false
*/
bool put(const char* key, int value) {
	if (table && (double)(table->count + 1) / table->size >= 0.75) {
		//创建新表（大小翻倍）
		HashTable* new_table = create_table(table->size * 2);
		if (!new_table) return false;
		//防御性编程：清除残留的old_table;(理论上不存在)；
		if (old_table) {
			destroy_table(old_table);
		}
		//切换表：当前表变为旧表，新表成为主表
		old_table = table;
		table = new_table;
	}
	//如果全局table尚未创建，初始化大小为 8
	if (!table) {
		table = create_table(8);
		if (!table) return false;
	}
	//在新表中插入或更新
	size_t index = hash(key, table->size);
	size_t start = index;
	HashEntry* target = NULL;
	do {
		HashEntry* entry = &table->entries[index];
		if (entry->state != OCCUPIED) {
			//找到空槽
			target = entry;
			break;
		}
		if (strcmp(entry->key, key) == 0) {
			//键已存在，直接更新值
			entry->value = value;
			return true;
		}
		index = (index + 1) % table->size;
	} while (index != start);
	//若未找到可插入位置（理论上扩容不会发生）
	if (!target) {
		return false;
	}
	//插入执行
	if (target->state == EMPTY || target->state == DELETED) {
		//复制字符串key，需要free
		target->key = (char*)malloc(strlen(key) + 1);
		if (!target->key) return false;
		strcpy(target->key, key);
		// if (!target->key) return false; // ← 冗余，已检查过 malloc
		target->value = value;
		target->state = OCCUPIED;
		table->count++; //有效元素数增加
	}
	// 【渐进式迁移】：每次 put 都尝试从 old_table 迁移一个元素到新表
	if (old_table) {
		for (size_t i = 0; i < old_table->size; ++i) {
			HashEntry* e = &old_table->entries[i];
			if (e->state == OCCUPIED) {
				// 使用底层插入函数，避免递归调用 put
				_put_raw(table, e->key, e->value);
				// 释放旧 key 并标记为 DELETED
				free(e->key);
				e->state = DELETED;
				old_table->count--;
				break; // 每次只迁移一个元素
			}
		}
		// 如果 old_table 已空，释放它
		if (old_table->count == 0) {
			destroy_table(old_table);
			old_table = NULL;
		}
	}
	return true;
}

/*
* @name 根据键获取对应的值
* @param key		要查找的键，以 C 字符串形式传入
* @param out_value	输出参数，用于返回查找到的值
* @return 若找到对应键则返回 true，并通过 out_value 返回值；否则返回 false
*/
bool get(const char* key, int* out_value) {
	// 先查新表
	int idx = find_in_table(table, key);
	if (idx != -1) {
		*out_value = table->entries[idx].value;
		return true;
	}
	// 再查旧表（若存在）
	if (old_table) {
		idx = find_in_table(old_table, key);
		if (idx != -1) {
			*out_value = old_table->entries[idx].value;
			return true;
		}
	}
	return false; // 未找到
}

/*
* @name 删除指定键对应的条目
* @param key 要删除的键，以 C 字符串形式传入
* @return 若成功删除（即键存在）则返回 true，否则返回 false
*/
bool delete_key(const char* key) {
	// 先在新表中查找
	int idx = find_in_table(table, key);
	if (idx != -1) {
		free(table->entries[idx].key);
		table->entries[idx].state = DELETED;
		table->count--;
		return true;
	}
	// 再在旧表中查找
	if (old_table) {
		idx = find_in_table(old_table, key);
		if (idx != -1) {
			free(old_table->entries[idx].key);
			old_table->entries[idx].state = DELETED;
			old_table->count--;
			return true;
		}
	}
	return false;
}

/*
* @name 打印哈希表内容（仅打印 OCCUPIED 状态的槽位）
* @param ht		要打印的哈希表指针
* @param name	表的名称（用于调试标识）
* @return 无返回值
*/
void print_table(HashTable* ht, const char* name) {
	if (!ht) return;
	printf("%s (size=%zu, count=%zu):\n", name, ht->size, ht->count);
	for (size_t i = 0; i < ht->size; ++i) {
		if (ht->entries[i].state == OCCUPIED) {
			printf("  [%zu] %s => %d\n", i, ht->entries[i].key, ht->entries[i].value);
		}
	}
}

int main(int argc, char* argv[]) {
	// 插入 5 个元素（初始表大小为 8，装载因子 = 5/8 = 0.625 < 0.75，不扩容）
	put("apple", 10);
	put("banana", 20);
	put("cherry", 30);
	put("date", 40);
	put("elderberry", 50);

	// 测试 get
	int val;
	if (get("banana", &val)) {
		printf("banana = %d\n", val);
	}

	// 插入第 6 个元素：6/8 = 0.75 → 触发扩容（新表大小 16）
	put("fig", 60);
	// 再插入一个，继续迁移旧表中的一个元素
	put("grape", 70);

	// 打印当前状态（此时 old_table 可能还有未迁移元素）
	print_table(table, "Current Table");
	if (old_table) {
		print_table(old_table, "Old Table (migrating)");
	}

	// 测试删除
	delete_key("cherry");
	if (!get("cherry", &val)) {
		printf("cherry deleted.\n");
	}

	// 继续插入多个元素，促使 old_table 完全迁移完毕
	for (int i = 0; i < 10; ++i) {
		char key[32];
		snprintf(key, sizeof(key), "key%d", i);
		put(key, i * 10);
	}

	// 最终状态：old_table 应已被释放
	print_table(table, "Final Table");
	if (old_table) {
		print_table(old_table, "Old Table (should be empty)");
	}

	// 清理内存
	if (table) destroy_table(table);
	if (old_table) destroy_table(old_table);
	return 0;
}