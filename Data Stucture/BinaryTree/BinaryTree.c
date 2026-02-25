#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/*
* @name 定义二叉树节点结构
* @param data	节点数据
* @param left	左指针
* @param right	右指针
*/
typedef struct TreeNode{
	char data;
	struct TreeNode* left;
	struct TreeNode* right;
}TreeNode;

/*
* @name 创建新节点
* @param data	节点数据
*/
TreeNode* createNode(char data) {
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	if (!node) {
		printf("创建树的节点失败\n");
		return NULL;
	}
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return node;
}

/*
* @name 前序遍历：根、左、右
* @param root	树的根节点
*/
void preorder(TreeNode* root) {
	if (root != NULL) {
		printf("%c ", root->data);
		preorder(root->left);
		preorder(root->right);
	}
}

/*
* @name 中序遍历：左、根、右
* @param root	树的根节点
*/
void inorder(TreeNode* root) {
	if (root != NULL) {
		inorder(root->left);
		printf("%c ", root->data);
		inorder(root->right);
	}
}

/*
* @name 中序遍历：左、右、根
* @param root	树的根节点
*/
void postorder(TreeNode* root) {
	if (root != NULL) {
		postorder(root->left);
		postorder(root->right);
		printf("%c ", root->data);
	}
}

/*
* @name 层序遍历BFS：从上到下，从左到右		
*/
#define MAX_QUEUE_SIZE 100

typedef struct Queue {
	TreeNode* items[MAX_QUEUE_SIZE];
	int front;
	int rear;
	int size;
} Queue;

Queue* createQueue() {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->front = 0;
	q->rear = -1;
	q->size = 0;
	return q;
}

bool isQueueEmpty(Queue* q) {
	return q->size == 0;
}

void enqueue(Queue* q, TreeNode* node) {
	if (q->size >= MAX_QUEUE_SIZE) {
		fprintf(stderr, "队列已满！\n");
		return;
	}
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
	q->items[q->rear] = node;
	q->size++;
}

TreeNode* dequeue(Queue* q) {
	if (isQueueEmpty(q)) {
		return NULL;
	}
	TreeNode* node = q->items[q->front];
	q->front = (q->front + 1) % MAX_QUEUE_SIZE;
	q->size--;
	return node;
}

// 层序遍历（广度优先）
void levelOrder(TreeNode* root) {
	if (root == NULL) return;

	Queue* q = createQueue();
	enqueue(q, root);

	while (!isQueueEmpty(q)) {
		TreeNode* current = dequeue(q);
		printf("%c ", current->data);

		if (current->left != NULL)
			enqueue(q, current->left);
		if (current->right != NULL)
			enqueue(q, current->right);
	}
	free(q); // 释放队列结构（注意：不释放树节点）
}


int main() {
	TreeNode* root = createNode('A');
	root->left = createNode('B');
	root->right = createNode('C');
	root->left->left = createNode('D');
	root->left->right = createNode('E');
	root->right->right = createNode('F');
	root->left->right->left = createNode('G');

	printf("前序遍历: ");
	preorder(root);
	printf("\n");

	printf("中序遍历: ");
	inorder(root);
	printf("\n");

	printf("后序遍历: ");
	postorder(root);
	printf("\n");

	printf("层序遍历: ");
	levelOrder(root);
	printf("\n");
	return 0;
}