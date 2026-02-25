#include <stdio.h>
#include <stdlib.h>

/*
* @name AVL树节点结构定义
* @param key	节点的键值
* @param left	左指针
* @param right	右指针
* @param height	节点高度，计算平衡因子
* @return
*/
typedef struct AVLNode {
	int key;
	struct AVLNode* left;
	struct AVLNode* right;
	int height;
}AVLNode;

/*
* @name 辅助函数：获取节点的高度
* @param node		AVL树节点结构体指针
* @return 节点的高度
*/
int getHeight(AVLNode* node) {
	if (node == NULL) return -1;
	return node->height;
}

/*
 * @name     比较两数最大值
 * @param    a - 整数 a
 * @param    b - 整数 b
 * @return   a 和 b 中的较大值
 */
int my_max(int a, int b) {
	return (a > b) ? a : b;
}

/*
* @name 创建AVL树的节点
* @param key		节点的键值
* @return AVL节点的指针，左右指向NULL，高度为0
*/
AVLNode* createNode(int key) {
	AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
	node->left = NULL;
	node->right = NULL;
	node->height = 0;
	node->key = key;
	return node;
}

/*
* @name LL型失衡
* @param y - 失衡子树的根节点
* @return 旋转后新的子树根结点指针
*		  原 y 的左孩子 x
*/
AVLNode* rotateRight(AVLNode* y) {
	AVLNode* x = y->left;
	AVLNode* b = x->right;

	x->right = y;
	y->left = b;

	y->height = my_max(getHeight(y->left), getHeight(y->right)) + 1;
	x->height = my_max(getHeight(x->left), getHeight(x->right)) + 1;

	return x;
}

/*
* @name RR型失衡
* @param x - 失衡子树的根节点
* @return 旋转后新的子树根结点指针
*		  原 x 的左孩子 y
*/
AVLNode* rotateLeft(AVLNode* x) {
	AVLNode* y = x->right;
	AVLNode* b = y->left;

	y->left = x;
	x->right = b;

	x->height = my_max(getHeight(x->left), getHeight(x->right)) + 1;
	y->height = my_max(getHeight(y->left), getHeight(y->right)) + 1;

	return y;
}

/*
* @name 获取平衡因子
* @param node		AVL树节点
* @return 该节点的平衡因子
*/
int getBalance(AVLNode* node) {
	if (node == NULL) return 0;
	return getHeight(node->left) - getHeight(node->right);
}

/*
* @name 插入并完成必要旋转后的新子树根节点指针
* @param root		当前子树的根节点
* @param key		要插入的键值
* @return 返回旋转后的新子树根节点指针
*/
AVLNode* insert(AVLNode* root, int key) {
	if (root == NULL) {
		return createNode(key);
	}
	if (key < root->key) {
		root->left = insert(root->left, key);
	} else if (key > root->key){
		root->right = insert(root->right, key);
	} else {
		return root;
	}
	root->height = my_max(getHeight(root->left), getHeight(root->right)) + 1;
	int balance = getBalance(root);
	// LL 型失衡
	if (balance > 1 && root->left != NULL && key < root->left->key) {
		return rotateRight(root);
	}

	// RR 型失衡
	if (balance < -1 && root->right != NULL && key > root->right->key) {
		return rotateLeft(root);
	}

	// LR 型失衡
	if (balance > 1 && root->left != NULL && key > root->left->key) {
		root->left = rotateLeft(root->left);
		return rotateRight(root);
	}

	// RL 型失衡
	if (balance < -1 && root->right != NULL && key < root->right->key) {
		root->right = rotateRight(root->right);
		return rotateLeft(root);
	}
	return root;
}

/*
* @name 找到子树中键值最小的节点指针（最左侧）
* @param node		子数根节点指针
* @return 该子树中键值最小的节点指针
*/
AVLNode* findMin(AVLNode* node) {
	while (node->left != NULL) {
		node = node->left;
	}
	return node;
}

/*
* @name 删除并完成再平衡后的新子树根节点指针
* @param root		当前子树的根节点
* @param key		要删除的键值
* @return 
*/
AVLNode* deleteNode(AVLNode* root, int key) {
	if (root == NULL) return root;
	if (key < root->key) {
		root->left = deleteNode(root->left, key);
	} else if (key > root->key) {
		root->right = deleteNode(root->right, key);
	} else {
		if (root->left == NULL || root->right == NULL) {
			AVLNode* temp = root->left ? root->left : root->right;
			if (temp == NULL) {
				temp = root;
				root = NULL;
			} else {
				*root = *temp;
				free(temp);
			}
		} else {
			AVLNode* temp = findMin(root->right);
			root->key = temp->key;
			root->right = deleteNode(root->right, temp->key);
		}
	}
	if (root == NULL) return root;

	root->height = my_max(getHeight(root->left), getHeight(root->right)) + 1;
	int balance = getBalance(root);
	// Left Left Case
	if (balance > 1 && root->left != NULL && getBalance(root->left) >= 0) {
		return rotateRight(root);
	}

	// Left Right Case
	if (balance > 1 && root->left != NULL && getBalance(root->left) < 0) {
		root->left = rotateLeft(root->left);
		return rotateRight(root);
	}

	// Right Right Case
	if (balance < -1 && root->right != NULL && getBalance(root->right) <= 0) {
		return rotateLeft(root);
	}

	// Right Left Case
	if (balance < -1 && root->right != NULL && getBalance(root->right) > 0) {
		root->right = rotateRight(root->right);
		return rotateLeft(root);
	}
	return root;
}

/*
* @name 查找
* @param root		AVL数根节点
* @param key		要查找的键值
* @return 找到返回1，没找到返回0
*/
int search(AVLNode* root, int key) {
	if (root == NULL) return 0;
	if (key == root->key) return 1;
	if (key < root->key) return search(root->left, key);
	return search(root->right, key);
}

/*
* @name 中序遍历
* @param root - AVL 树根节点指针
* @return  void - 中序遍历输出所有键值（升序）
*/
void inorder(AVLNode* root) {
	if (root != NULL) {
		inorder(root->left);
		printf("%d ", root->key);
		inorder(root->right);
	}
}

/*
* @name 释放
*/
void freeTree(AVLNode* root) {
	if (root == NULL) return;
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}

int main() {
	AVLNode* root = NULL;

	// LL: 30 → 20 → 10
	root = insert(root, 30);
	root = insert(root, 20);
	root = insert(root, 10);
	printf("LL 旋转后根: %d (应为 20)\n", root->key);
	freeTree(root);

	// RR: 30 → 40 → 50
	root = NULL;
	root = insert(root, 30);
	root = insert(root, 40);
	root = insert(root, 50);
	printf("RR 旋转后根: %d (应为 40)\n", root->key);
	freeTree(root);

	// LR: 30 → 10 → 20
	root = NULL;
	root = insert(root, 30);
	root = insert(root, 10);
	root = insert(root, 20);
	printf("LR 旋转后根: %d (应为 20)\n", root->key);
	freeTree(root);

	// RL: 30 → 50 → 40
	root = NULL;
	root = insert(root, 30);
	root = insert(root, 50);
	root = insert(root, 40);
	printf("RL 旋转后根: %d (应为 40)\n", root->key);
	freeTree(root);

	//查找
	root = NULL;
	root = insert(root, 10);
	root = insert(root, 20);
	root = insert(root, 30);
	root = insert(root, 40);
	root = insert(root, 50);
	root = insert(root, 60);
	printf("查找 35: %s\n", search(root, 35) ? "存在" : "不存在");
	freeTree(root);

	//删除
	root = NULL;
	root = insert(root, 10);
	root = insert(root, 20);
	root = insert(root, 30);
	root = insert(root, 40);
	root = insert(root, 50);
	root = insert(root, 60);
	root = deleteNode(root, 20);
	printf("删除后中序遍历: ");
	inorder(root);
	printf("\n新根节点: %d\n\n", root ? root->key : -1);
	
	freeTree(root);

	return 0;
}