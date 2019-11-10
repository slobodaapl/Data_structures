#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include "funcs.h"

struct node_m{
    unsigned long int val;
    struct node_m * left;
    struct node_m * right;
    int height;
};

struct node_m * root;
unsigned char flag_g;

int max(struct node_m * x, struct node_m * y){
    int a;
    int b;
    if(x == NULL){
        a = 0;
    } else
        a = x->height;
    if(y == NULL){
        b = 0;
    } else
        b = y->height;
    return (a > b) ? a : b;
}

struct node_m * r_rot(struct node_m * temp){
    struct node_m * temp_d = temp->left;
    struct node_m * temp_dd = temp_d->right;

    temp_d->right = temp;
    temp->left = temp_dd;
    temp->height = 1 + max(temp->left, temp->right);
    temp_d->height = 1 + max(temp_d->left, temp_d->right);

    return temp_d;
}

struct node_m * l_rot(struct node_m * temp){
    struct node_m * temp_d = temp->right;
    struct node_m * temp_dd = temp_d->left;

    temp_d->left = temp;
    temp->right = temp_dd;
    temp->height = 1 + max(temp->left, temp->right);
    temp_d->height = 1 + max(temp_d->left, temp_d->right);

    return temp_d;
}

struct node_m * insert_m(struct node_m * node, unsigned long int val){
    if(node == NULL){
        struct node_m * leaf = malloc(sizeof(struct node_m));
        leaf->height = 1;
        leaf->val = val;
        leaf->left = NULL;
        leaf->right = NULL;
        return leaf;
    }else if(node->val > val){
        node->left = insert_m(node->left, val);
    } else if(node->val < val){
        node->right = insert_m(node->right, val);
    } else {
        return node;
    }

    if(!flag_g) {
        node->height = 1 + max(node->left, node->right);
        int l, r;

        if (node->left == NULL) {
            l = 0;
        } else
            l = node->left->height;

        if (node->right == NULL) {
            r = 0;
        } else
            r = node->right->height;

        int balance = l - r;

        if (balance < -1 && val > node->right->val) {
            node = l_rot(node);
            return node;
        }

        if (balance < -1 && val < node->right->val) {
            node->right = r_rot(node->right);
            node = l_rot(node);
            return node;
        }

        if (balance > 1 && val < node->left->val) {
            node = r_rot(node);
            return node;
        }

        if (balance > 1 && val > node->left->val) {
            node->left = l_rot(node->left);
            node = r_rot(node);
            return node;
        }
    }

    return node;
}

void tree_terminate_avl(struct node_m * node){
    if(node != NULL){
        tree_terminate_avl(node->left);
        tree_terminate_avl(node->right);
        free(node);
    }
}

void iot(struct node_m * node){
    if(node != NULL){
        iot(node->left);
        iot(node->right);
        printf("%lu\n", node->val);
    }
}

float driver_mytree(int x, unsigned char flag) {
    unsigned long int * randlist = malloc(x * sizeof(unsigned long int));
    floyd_rand(randlist, x);
    flag_g = 0;
    root = malloc(sizeof(struct node_m));
    root->height = 1;
    root->left = NULL;
    root->right = NULL;
    root->val = randlist[0];
    clock_t start = clock();
    for(int i = 1; i < x; i++){
        root = insert_m(root, randlist[i]);
    }
    if(flag){
        flag_g = flag;
        start = clock();
        for(int i = x - 1; i > 0; i--){
            root = insert_m(root, randlist[i]);
        }
    }
    clock_t end = clock();

    tree_terminate_avl(root);
    free(randlist);

    return (float) (end - start) / CLOCKS_PER_SEC * 1000;
}