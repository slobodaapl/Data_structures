#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "funcs.h"

struct cell{
    unsigned long int val;
    struct cell * right;
    struct cell * left;
};

int flag;

int primality(unsigned long int p){
    if(p <= 3)
        return p > 1;
    if(p % 2 == 0 || p % 3 == 0)
        return 0;
    int i = 5;
    while(i * i <= p){
        if(p % i == 0 || p % (i + 2) == 0)
            return 0;
        i += 6;
    }
    return 1;
}

unsigned long int primegen(unsigned long int n){
    for(unsigned long int i = n; i <= 2*n; i++)
        if(primality(i))
            return i;
    return -1;
}

struct cell * insert_ha(struct cell * cell, unsigned long int val){
    if(cell == NULL){
        struct cell * leaf = malloc(sizeof(struct cell));
        leaf->val = val;
        leaf->left = NULL;
        leaf->right = NULL;
        return leaf;
    }else if(cell->val > val){
        cell->left = insert_ha(cell->left, val);
    } else if(cell->val < val){
        cell->right = insert_ha(cell->right, val);
    } else {
        flag = 1;
        return cell;
    }
    return cell;
}

struct cell ** gen(unsigned long int n){
    struct cell ** temp = malloc(n * sizeof(struct cell *));
    for(unsigned long int i = 0; i < n; i++)
        temp[i] = NULL;
    return temp;
}

void tree_rehash(struct cell * node, unsigned long int new_size, struct cell ** new_field){
    if(node != NULL){
        tree_rehash(node->left, new_size, new_field);
        tree_rehash(node->right, new_size, new_field);
        unsigned long int key = node->val % new_size;
        new_field[key] = insert_ha(new_field[key], node->val);
        free(node);
    }
}

struct cell ** rehash(struct cell ** old, unsigned long int new_size, unsigned long int old_size){
    struct cell ** new = gen(new_size);
    for(unsigned long int i = 0; i < old_size; i++)
        tree_rehash(old[i], new_size, new);
    free(old);
    return new;
}

void tree_terminate(struct cell * node){
    if(node != NULL){
        tree_terminate(node->left);
        tree_terminate(node->right);
        free(node);
    }
}

void terminate(struct cell ** kill, unsigned long m){
    for(unsigned long int i = 0; i < m; i++) {
        tree_terminate(kill[i]);
    }
    free(kill);
}


float driver_myhash(unsigned long x, float alpha_ratio){
    unsigned long int m = 23;
    float alpha_lim = 3;
    unsigned long int n = 0;
    float alpha = 0;

    unsigned long int * nums = malloc(x * sizeof(unsigned long int));
    floyd_rand(nums, x);

    struct cell ** field = gen(m);

    clock_t start = clock();
    for(unsigned long int i = 0; i < x; i++){
        flag = 0;
        unsigned long int key = nums[i] % m;
        field[key] = insert_ha(field[key], nums[i]);
        alpha = (float) ++n / (3 * (float) m);
        if(alpha > (float) (alpha_ratio)){
            unsigned long int t = m;
            m = primegen(2*m);
            field = rehash(field, m, t);
        }
    }
    clock_t end = clock();

    terminate(field, m);
    free(nums);

    return (float) (end - start) / CLOCKS_PER_SEC * 1000;
}