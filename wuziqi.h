//
// Created by cuihang on 2023/11/28.
//

#ifndef UNTITLED16_WUZIQI_H
#define UNTITLED16_WUZIQI_H
#define human 10
#define ai 1
#define five_5 10000000
#define live_4  100000
#define chong_4 1000
#define live_3 1000
#define sleep_3 100
#define INFINITY 1000000000
#endif //UNTITLED16_WUZIQI_H
#include <stdlib.h>
#include <string.h>
#include<stdio.h>
#include "raylib.h"
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
typedef struct node node;
struct node {
    int nodedepth;
    int eval;
    int chess[15][15];
    node *nextnode[225];
};
node beginning = {0, 0, {0}, 0,{NULL}}; //我方先手，奇数深度是我方落子后局面
node *ce_cur_node = &beginning;       //用于创建新一层结点
node waitnode;
node *wait_node=&waitnode;
int nummax(int i,int j)
{if(i>=j)
    {return i;}
    else    {return j;}
}
int nummin(int i,int j)
{if(i<=j)
    {return i;}
    else    {return j;}
}
int oppisite(int player)//获取 player的敌人的棋子值
{int enemy=0;
    if(player==10) {
        enemy = 1;
    }
    else {enemy=10;}
    return enemy;
}
struct direction{//表征方向的结构
    int dx;
    int dy;
};
typedef struct direction direction;
const direction right ={1, 0};
const direction down ={0, 1};
const direction right_down ={1, 1};
const direction left_down={-1, 1};
direction dir[4];

void init_direction() {
    dir[0] = right;
    dir[1] = down;
    dir[2] = right_down;
    dir[3] = left_down;
}
bool IsBeyond(int x,int y)//判断是否超出棋盘
{if(x<15&&x>=0&&y<15&&y>=0)
        return true;
    else return false;
}
int next_chess( node * np,direction d,int i,int j,int player)//获取某方向上的第几个棋子
{  if(IsBeyond(i%15+j*d.dx,i/15+j*d.dy))
        return np->chess[i/15+j*d.dy][i%15+j*d.dx];
    else return oppisite(player);
}
int se_5(node * np,int player ) {
    int quan = 0;
    for (int d = 0; d < 4; d++) {
        for (int i = 0; i < 225; i++) {
            quan = 0;
            for (int j = 1; j <= 5; j++) {

                quan = quan + next_chess(np, dir[d], i, j, player);
            }
            if (quan == 5 * player) {
                return 1;
            }

        }

    }
    return 0;
}
int jizhongdu(node*np,int player)
{int result=0;
    for(int i=0;i<225;i++)
    { if(np->chess[i/15][i%15]==player)
            result=result-((((i/15)-7)*((i/15)-7)+((i%15)-7)*((i%15)-7)));
    }
    return result;
}
int se_patterns(node *np, int player) {
    int result[5] = {0, 0, 0, 0, 0}; // 依次用于存储五子、四子、活三、冲四和眠三的数量

    for(int d=0; d<4; d++) {
        for(int i=0; i<225; i++) {
            int cnt = 0;
            for(int j=0; j<5; j++) {
                if(next_chess(np, dir[d], i, j, player) == player)
                    cnt++;
                else
                    break;
            }
          switch (cnt) {
              case 5:
                  result[0]++;
                  break;
              case 4:
                  if(next_chess(np, dir[d], i, 0, player) ==0|| next_chess(np, dir[d], i, 4, player) ==0)
                      result[1]++;
                  else result[3]++;
                  break;
                case 3:
                    if(next_chess(np, dir[d], i, 0, player) ==0|| next_chess(np, dir[d], i, 4, player) ==0)
                    result[2]++;
                    else result[4]++;
            break;
              default:
                  break;
          }
        }
    }
    return result[0]*five_5 + result[1]*live_4 + result[2]*live_3 + result[3]*chong_4 + result[4]*sleep_3;
}

int evaluate(node *np) {
    int score = 0;
    score = se_patterns(np, ai) - se_patterns(np, human) + jizhongdu(np, ai) - jizhongdu(np, human);
    return score;
}
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
                    np->nextnode[l]->chess[k / 15][k % 15] = ai;
                    np->nextnode[l]->eval=evaluate(np->nextnode[l]);
                    break;
                }
                case 1: {
                    np->nextnode[l]->chess[k / 15][k % 15] = human;
                    np->nextnode[l]->eval =evaluate(np->nextnode[l]);
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
node *max(node *np) {
    int _max = -10000000000;
    node *maxnode = NULL;
    for (int kl = 0; kl < 225 - np->nodedepth; kl++) {
        if (np->nextnode[kl]->eval > _max) {
            _max = np->nextnode[kl]->eval;
            maxnode = (np->nextnode[kl]);
        }
    }
    return maxnode;
}

void scan(node *np) {
    for (int i = 0; i < 225; i++) {
        switch (np->chess[i / 15][i % 15]) {
            case 0:
                break;
            case 1:
                DrawCircle((i % 15 + 1) * 50, (i / 15 + 1) * 50, 30, BLACK);
                break;
            case human:
                DrawCircle((i % 15 + 1) * 50, (i / 15 + 1) * 50, 30, WHITE);
                break;
        }
    }
}
void chuanshu(node *np)
{
    memcpy(waitnode.chess,np->chess,sizeof(np->chess));
waitnode.nodedepth=np->nodedepth;
}
void quicksort_min(node* arr[], int left, int right) {
    if (left >= right) {
        return;
    }
    int pivot = arr[left]->eval;
    int i = left;
    int j = right;

    while (i < j) {
        while (arr[j]->eval >= pivot && i < j) {
            j--;
        }
        while (arr[i]->eval <= pivot && i < j) {
            i++;
        }
        if (i < j) {
            node* temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    node* temp = arr[left];
    arr[left] = arr[i];
    arr[i] = temp;
    quicksort_min(arr, left, i - 1);
    quicksort_min(arr, i + 1, right);
}
void quicksort_max(node* arr[], int left, int right) {
    if (left >= right) {
        return;
    }
    int pivot = arr[left]->eval;
    int i = left;
    int j = right;

    while (i < j) {
        while (arr[j]->eval <= pivot && i < j) {
            j--;
        }
        while (arr[i]->eval >= pivot && i < j) {
            i++;
        }
        if (i < j) {
            node* temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    node* temp = arr[left];
    arr[left] = arr[i];
    arr[i] = temp;
    quicksort_max(arr, left, i - 1);
    quicksort_max(arr, i + 1, right);
}
int alpha_beta(node *np, int depth, int alpha, int beta, int maximizingPlayer) {
    if (np == NULL) {
        return 0;
    }
    if (depth == 0) {
        return evaluate(np);
    }
    int num_valid_nodes =225- np->nodedepth;
    createnode(np);
    c_n_s(np);
    node *nextnode[num_valid_nodes];
    for (int i = 0, j = 0; i < 225 - np->nodedepth && j < num_valid_nodes; i++) {
        if (np->nextnode[i] != NULL) {
            nextnode[j++] = np->nextnode[i];
        }
    }
if(depth%2==0)
    quicksort_min(nextnode, 0, num_valid_nodes - 1);
if(depth%2==1)
    quicksort_max(nextnode, 0, num_valid_nodes - 1);
    if (maximizingPlayer) {
        int value = -INFINITY;
        for (int i = 0; i < 20; i++) {
            value = nummax(value, alpha_beta(nextnode[i], depth - 1, alpha, beta, 0));
            alpha = nummax(alpha, value);
            if (alpha >= beta) {
                break;
            }
        }
        for (int i = 0; i < num_valid_nodes; i++) {
            free(np->nextnode[i]);
        }
        return value;
    } else {
        int value = INFINITY;
        for (int i = 0; i < 20; i++) {
            value = nummin(value, alpha_beta(nextnode[i], depth - 1, alpha, beta, 1));
            beta = nummin(beta, value);
            if (beta <= alpha) {
                break;
            }
        }
        for (int i = 0; i < num_valid_nodes; i++) {
            free(np->nextnode[i]);
        }
        return value;
    }
}

void move() {
    createnode(ce_cur_node);
    c_n_s(ce_cur_node);
    quicksort_max(ce_cur_node->nextnode, 0, 225 - ce_cur_node->nodedepth-1);
    for (int a = 0; a < 20; a++) {
        ce_cur_node->nextnode[a]->eval = alpha_beta(ce_cur_node->nextnode[a], 3, -INFINITY, INFINITY, 0);
    }
     chuanshu(max(ce_cur_node));
    scan(wait_node);
    for (int i = 0; i < 225 - ce_cur_node->nodedepth; i++) { free(ce_cur_node->nextnode[i]); }
}

void en_move(int y,int x )
{int hang=y, lie=x;
    if(waitnode.chess[hang][lie]==0)
    {
        waitnode.chess[hang][lie]=human;
    }
    else printf("wrong");
    scan(  wait_node);
    ce_cur_node=wait_node;
    ce_cur_node->nodedepth++;

}
