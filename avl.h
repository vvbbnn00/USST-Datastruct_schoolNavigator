//
// Created by vvbbnn00 on 2023/08/28.
//
#ifndef COURSESYSTEM2023_AVL_H
#define COURSESYSTEM2023_AVL_H

#include <stdlib.h>
#include <stdint.h>
#include "models.h"
#include "math_ext.h"


// 索引链表
typedef struct IndexListNode {
    Index index; // 索引
    struct IndexListNode *next; // 下一个节点
} IndexListNode;

// AVL树节点定义
typedef struct AVLNode {
    IndexListNode *list;    // 索引链表
    int height;             // 节点高度
    struct AVLNode *left;   // 左子树
    struct AVLNode *right;  // 右子树
} AVLNode;

// 节点列表，在搜索多个节点时使用
typedef struct NodeList {
    IndexListNode *indexNode;   // 索引链表节点
    struct NodeList *next;      // 下一个节点
} NodeList;


/**
 * 创建新的索引节点
 * @param hash
 * @param type
 * @param data
 * @return
 */
IndexListNode *Index_newIndexListNode(int64 hash, IndexType type, void *data) {
    IndexListNode *node = (IndexListNode *) calloc(1, sizeof(IndexListNode));
    if (!node) {
        return NULL;
    }

    node->index.hash = hash;
    node->index.type = type;
    node->index.data = data;
    node->next = NULL;

    return node;
}


/**
 * 深复制链表
 * @param node
 * @return
 */
IndexListNode *IndexListNode_deepCopy(IndexListNode *node) {
    IndexListNode *newNode = (IndexListNode *) calloc(1, sizeof(IndexListNode));
    if (!newNode) {
        return NULL;
    }

    if (node == NULL) {
        return newNode;
    }

    newNode->index.hash = node->index.hash;
    newNode->index.type = node->index.type;
    newNode->index.data = node->index.data;

    for (IndexListNode *p = node->next, *q = newNode; p != NULL; p = p->next, q = q->next) {
        q->next = Index_newIndexListNode(p->index.hash, p->index.type, p->index.data);
    }


    return newNode;
}


/**
 * 删除索引链表
 * @param node
 */
void IndexListNode_delete(IndexListNode *node) {
    if (node == NULL) {
        return;
    }
    IndexListNode *p = node;
    while (p) {
        IndexListNode *temp = p;
        p = p->next;
        if (temp->index.type == INDEX_TYPE_STRING) {
            safe_free(&temp->index.data);
        }
        safe_free(&temp);
    }
}


/**
 * 获取节点的高度
 * @param node
 * @return
 */
int AVL_getHeight(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

/**
 * 右旋
 * @param y
 * @return
 */
AVLNode *AVL_rightRotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T3 = x->right;

    x->right = y;
    y->left = T3;

    y->height = max(AVL_getHeight(y->left), AVL_getHeight(y->right)) + 1;
    x->height = max(AVL_getHeight(x->left), AVL_getHeight(x->right)) + 1;

    return x;
}

/**
 * 左旋
 * @param x
 * @return
 */
AVLNode *AVL_leftRotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(AVL_getHeight(x->left), AVL_getHeight(x->right)) + 1;
    y->height = max(AVL_getHeight(y->left), AVL_getHeight(y->right)) + 1;

    return y;
}

/**
 * 获取节点的平衡因子
 * @param node
 * @return
 */
int AVL_getBalance(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return AVL_getHeight(node->left) - AVL_getHeight(node->right);
}

/**
 * 插入新的节点
 * @param node
 * @param hash
 * @param type
 * @param data
 * @return
 */
AVLNode *AVL_insertNode(AVLNode *node, int64 hash, IndexType type, void *data) {
    // 正常的BST插入
    if (node == NULL) {
        AVLNode *newNode = (AVLNode *) calloc(1, sizeof(AVLNode));
        if (!newNode) {
            return NULL;
        }
        IndexListNode *newList = Index_newIndexListNode(hash, type, data);
        newNode->list = newList;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    if (hash < node->list->index.hash) {
        node->left = AVL_insertNode(node->left, hash, type, data);
    } else if (hash > node->list->index.hash) {
        node->right = AVL_insertNode(node->right, hash, type, data);
    } else {
        // 如果关键字相同，则插入到链表中
        IndexListNode *newNode = Index_newIndexListNode(hash, type, data);
        newNode->next = node->list;
        node->list = newNode;
        return node;
    }

    // 更新高度
    node->height = 1 + max(AVL_getHeight(node->left), AVL_getHeight(node->right));

    // 获取平衡因子并进行旋转
    int balance = AVL_getBalance(node);

    // 左左
    if (balance > 1 && hash < node->left->list->index.hash) {
        return AVL_rightRotate(node);
    }

    // 右右
    if (balance < -1 && hash > node->right->list->index.hash) {
        return AVL_leftRotate(node);
    }

    // 左右
    if (balance > 1 && hash > node->left->list->index.hash) {
        node->left = AVL_leftRotate(node->left);
        return AVL_rightRotate(node);
    }

    // 右左
    if (balance < -1 && hash < node->right->list->index.hash) {
        node->right = AVL_rightRotate(node->right);
        return AVL_leftRotate(node);
    }

    return node;
}

/**
 * 查找指定哈希值的节点
 * @param node
 * @return
 */
AVLNode *AVL_minValueNode(AVLNode *node) {
    AVLNode *current = node;

    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}

/**
 * 查找指定哈希值的节点
 * @param root
 * @param hash
 * @return
 */
IndexListNode *AVL_searchExact(AVLNode *root, int64 hash) {
    if (root == NULL) {
        return NULL;
    }

    if (hash < root->list->index.hash) {
        return AVL_searchExact(root->left, hash);
    }
    if (hash > root->list->index.hash) {
        return AVL_searchExact(root->right, hash);
    }
    return root->list;
}


/**
 * 获取AVL树中节点的数量
 * @param root
 * @return
 */
int64 AVL_countNodes(AVLNode *root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + AVL_countNodes(root->left) + AVL_countNodes(root->right);
}


/**
 * 新建一个节点链表
 * @param indexNode
 * @return
 */
NodeList *NodeList_newNodeList(IndexListNode *indexNode) {
    NodeList *node = (NodeList *) calloc(1, sizeof(NodeList));
    if (!node) {
        return NULL;
    }

    node->indexNode = indexNode;
    node->next = NULL;

    return node;
}


/**
 * 中序遍历AVL树并将结果保存到链表中
 * @param root
 * @param resultList
 */
void AVL_inOrderTraverse(AVLNode *root, NodeList **resultList) {
    if (root == NULL) {
        return;
    }

    AVL_inOrderTraverse(root->left, resultList);

    NodeList *newNode = NodeList_newNodeList(root->list);
    if (*resultList == NULL) {
        *resultList = newNode;
    } else {
        NodeList *temp = *resultList;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    AVL_inOrderTraverse(root->right, resultList);
}


/**
 * 搜索指定范围内的节点
 * @param root
 * @param startHash
 * @param endHash
 * @param resultList
 */
void AVL_inOrderSearch(AVLNode *root, int64 startHash, int64 endHash, NodeList **resultList) {
    if (root == NULL) {
        return;
    }

    AVL_inOrderSearch(root->left, startHash, endHash, resultList);

    // 检查当前节点的哈希值是否在给定范围内
    if (root->list->index.hash >= startHash && root->list->index.hash <= endHash) {
        // printf("找到节点：%lld\n", root->list->index.hash);
        // system("pause");
        NodeList *newNode = NodeList_newNodeList(root->list);
        if (*resultList == NULL) {
            *resultList = newNode;
        } else {
            NodeList *temp = *resultList;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    AVL_inOrderSearch(root->right, startHash, endHash, resultList);
}


#endif