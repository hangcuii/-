//
// Created by cuihang on 2023/11/28.
//

#ifndef UNTITLED16_WUZIQI_H
#define UNTITLED16_WUZIQI_H

#endif //UNTITLED16_WUZIQI_H
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include<stdio.h>

#define WIDTH 15
#define LENGTH 15 //绘制时棋盘的长宽
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
int curdepth = 0; //当前等待决策的深度
int sear_depth = 4; //每次搜索的深度
int chess[15][15] = {0};
typedef struct node node;
struct node {
    int nodedepth;
    int eval;
    int chess[15][15];
    node *nextnode[225];
};
node beginning = {0, 0, {0}, {NULL}}; //我方先手，奇数深度是我方落子后局面
node *ce_cur_node = &beginning;       //用于创建新一层结点
node *se_cur_node = &beginning;       // 当前搜索起点
node *wait_node=NULL;
void c_n_s(node *np) {
    for (int k = 0, l = 0; k < 225; k++) //
    {
        if (np->nextnode[l] == NULL) {
            np->nextnode[l] = malloc(sizeof(node));
            memset(np->nextnode[l], 0, sizeof(node));
            np->nextnode[l]->nodedepth = np->nodedepth + 1;
            memcpy(np->nextnode[l]->chess, np->chess, sizeof(np->chess));
        }
        if (np->nextnode[l]->chess[k / 15][k % 15] == 0 || (l == 0 || np->nextnode[l - 1]->chess[k / 15][k % 15] == 0)) {
            switch (np->nodedepth % 2) {
                case 0: {
                    np->nextnode[l]->chess[k / 15][k % 15] = 1;
                    break;
                }
                case 1: {
                    np->nextnode[l]->chess[k / 15][k % 15] = 2;
                    break;
                }
            }
            l++;
        }
    }
}

void createnode(node *np) {
    for (int i = 0; i < 225 - np->nodedepth; i++) {
        np->nextnode[i] = malloc(sizeof(node));
        memset(np->nextnode[i], 0, sizeof(node));
        np->nextnode[i]->nodedepth = np->nodedepth + 1;
        memcpy(np->nextnode[i]->chess, np->chess, sizeof(np->chess));
    }
}

int evaluate(node *np) {
    int score = 0;
    for (int j = 0; j < 225; j++) {
        switch (np->chess[j / 15][j % 15]) {
            case 0:
                break;
            case 1:{
                    score = score-((j / 15) - 7)*((j / 15) - 7)-((j % 15) - 7)*((j % 15) - 7);
                    break;
                }
            case 2:{
                score = score +((j / 15) - 7)*((j / 15) - 7)+((j % 15) - 7)*((j % 15) - 7);
                    break;
                }
        }
    }
    return score;
}

node *max(node *np) {
    int _max = -100000;
    node *maxnode = NULL;
    for (int kl = 0; kl < 225 - np->nodedepth; kl++) {
        if (np->nextnode[kl]->eval > _max) {
            _max = np->nextnode[kl]->eval;
            maxnode = (np->nextnode[kl]);
        }
    }
    return maxnode;
}

node *min(node *np) {
    int _min = 100000;
    node *minnode = NULL;
    for (int kl = 0; kl < 225 - np->nodedepth; kl++) {
        if (np->nextnode[kl]->eval < _min) {
            _min = np->nextnode[kl]->eval;
            minnode = (np->nextnode[kl]);
        }
    }
    return minnode;
}

void scan(node *np) {
    for (int i = 0; i < 225; i++) {
        switch (np->chess[i / 15][i % 15]) {
            case 0:
                break;
            case 1:
                DrawCircle((i % 15 + 1) * 50, (i / 15 + 1) * 50, 30, BLACK);
                break;
            case 2:
                DrawCircle((i % 15 + 1) * 50, (i / 15 + 1) * 50, 30, WHITE);
                break;
        }
    }
}

void move()
{
    createnode(ce_cur_node);
    c_n_s(ce_cur_node); //我方走子
    for (int b = 0; b < 225 - ce_cur_node->nodedepth; b++) {
        createnode(ce_cur_node->nextnode[b]);
        c_n_s(ce_cur_node->nextnode[b]); //敌方走子
        for (int c = 0; c < 225 - ce_cur_node->nodedepth - 1; c++) {
           ce_cur_node->nextnode[b]->nextnode[c]->eval = evaluate(ce_cur_node->nextnode[b]->nextnode[c]);
        }
        ce_cur_node->nextnode[b]->eval = min(ce_cur_node->nextnode[b])->eval;
    }
    ce_cur_node=max(ce_cur_node);
    scan(ce_cur_node);
  //for(int x=0;x<225-ce_cur_node->nodedepth;x++)
  // {free (ce_cur_node->nextnode[x]);
   //}
}
void en_move(int y,int x )
{int hang=y, lie=x;
    if(ce_cur_node->chess[hang][lie]==0)
    {
        ce_cur_node->chess[hang][lie]=2;
    }
    else printf("wrong");
    scan(  ce_cur_node);
    ce_cur_node->nodedepth++;

}
