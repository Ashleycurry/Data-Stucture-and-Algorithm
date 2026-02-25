#include <stdio.h>
#include <stdlib.h>

// 树节点结构
typedef struct TreeNode {
	int key;
	struct TreeNode* left;
	struct TreeNode* right;
	struct TreeNode* parent; // 用于方便找前驱和后继
} TreeNode;

// 创建新节点
TreeNode* createNode(int key) {
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return node;
}

// 插入节点（递归方式，并维护 parent 指针）
TreeNode* insert(TreeNode* root, TreeNode* parent, int key) {
	if (root == NULL) {
		TreeNode* newNode = createNode(key);
		newNode->parent = parent;
		return newNode;
	}

	if (key < root->key) {
		root->left = insert(root->left, root, key);
	}
	else if (key > root->key) {
		root->right = insert(root->right, root, key);
	}
	// 若 key 相等，不插入重复值（BST 不允许重复）
	return root;
}

// 查找节点
TreeNode* search(TreeNode* root, int key) {
	if (root == NULL || root->key == key)
		return root;

	if (key < root->key)
		return search(root->left, key);
	else
		return search(root->right, key);
}

// 查找最小值（最左子节点）
TreeNode* findMin(TreeNode* root) {
	if (root == NULL) return NULL;
	while (root->left != NULL)
		root = root->left;
	return root;
}

// 查找最大值（最右子节点）
TreeNode* findMax(TreeNode* root) {
	if (root == NULL) return NULL;
	while (root->right != NULL)
		root = root->right;
	return root;
}

// 查找后继节点（中序后继）
TreeNode* findSuccessor(TreeNode* node) {
	if (node == NULL) return NULL;

	// 情况1：有右子树，则后继是右子树的最小值
	if (node->right != NULL) {
		return findMin(node->right);
	}

	// 情况2：无右子树，向上找第一个“左孩子”的祖先
	TreeNode* parent = node->parent;
	while (parent != NULL && node == parent->right) {
		node = parent;
		parent = parent->parent;
	}
	return parent;
}

// 查找前驱节点（中序前驱）
TreeNode* findPredecessor(TreeNode* node) {
	if (node == NULL) return NULL;

	// 情况1：有左子树，则前驱是左子树的最大值
	if (node->left != NULL) {
		return findMax(node->left);
	}

	// 情况2：无左子树，向上找第一个“右孩子”的祖先
	TreeNode* parent = node->parent;
	while (parent != NULL && node == parent->left) {
		node = parent;
		parent = parent->parent;
	}
	return parent;
}

// 查找以 key 为根的子树的最小节点（用于删除）
TreeNode* minValueNode(TreeNode* node) {
	TreeNode* current = node;
	while (current && current->left != NULL)
		current = current->left;
	return current;
}

// 删除节点（递归）
TreeNode* deleteNode(TreeNode* root, int key) {
	if (root == NULL) return root;

	if (key < root->key) {
		root->left = deleteNode(root->left, key);
	}
	else if (key > root->key) {
		root->right = deleteNode(root->right, key);
	}
	else {
		// 找到要删除的节点

		// 情况1：只有右子树 或 无子节点
		if (root->left == NULL) {
			TreeNode* temp = root->right;
			free(root);
			return temp;
		}
		// 情况2：只有左子树
		else if (root->right == NULL) {
			TreeNode* temp = root->left;
			free(root);
			return temp;
		}

		// 情况3：有两个子节点
		// 找到中序后继（右子树最小值）
		TreeNode* temp = minValueNode(root->right);
		// 替换值
		root->key = temp->key;
		// 删除后继节点（它最多只有一个右孩子）
		root->right = deleteNode(root->right, temp->key);
	}
	return root;
}

// 中序遍历（用于验证 BST）
void inorder(TreeNode* root) {
	if (root != NULL) {
		inorder(root->left);
		printf("%d ", root->key);
		inorder(root->right);
	}
}

// 辅助函数：打印节点信息（包括父节点）
void printNodeInfo(TreeNode* node) {
	if (node == NULL) {
		printf("Node is NULL\n");
		return;
	}
	printf("Key: %d", node->key);
	if (node->parent)
		printf(", Parent: %d", node->parent->key);
	else
		printf(", Parent: NULL");
	printf("\n");
}

// 主函数测试
int main() {
	TreeNode* root = NULL;

	// 插入测试数据
	int keys[] = { 50, 30, 70, 20, 40, 60, 80 };
	int n = sizeof(keys) / sizeof(keys[0]);

	for (int i = 0; i < n; i++) {
		root = insert(root, NULL, keys[i]);
	}

	printf("Inorder traversal: ");
	inorder(root);
	printf("\n");

	// 测试查找
	TreeNode* found = search(root, 40);
	if (found) {
		printf("Found node with key 40\n");
		printNodeInfo(found);
	}

	// 测试最小/最大
	printf("Min: %d\n", findMin(root)->key);
	printf("Max: %d\n", findMax(root)->key);

	// 测试前驱和后继
	TreeNode* node40 = search(root, 40);
	TreeNode* pred = findPredecessor(node40);
	TreeNode* succ = findSuccessor(node40);
	if (pred) printf("Predecessor of 40: %d\n", pred->key);
	if (succ) printf("Successor of 40: %d\n", succ->key);

	// 测试删除
	printf("\nDeleting 30...\n");
	root = deleteNode(root, 30);
	printf("Inorder after deletion: ");
	inorder(root);
	printf("\n");

	// 再次测试前驱后继（比如 40 的前驱现在应为 20）
	pred = findPredecessor(search(root, 40));
	if (pred) printf("New predecessor of 40: %d\n", pred->key);

	return 0;
}