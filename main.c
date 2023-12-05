#include"wuziqi.h"
int cu_chess[15][15]={0};
bool shouldmove=true;
bool shouldenmove=false;
bool playerTurn = false;
Vector2 mouseposition;
int x1,y1;
int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
     SetTargetFPS(10)   ;
   while (!WindowShouldClose() )
   {;
       BeginDrawing();
       ClearBackground(RAYWHITE) ;
      // scan(wait_node)  ;
       for (int k = 0; k < 15; k++)
       {
           DrawLine(50, 50 + k * 50, 750, 50 + k * 50, BLACK);
       }
       for (int eo = 0; eo < 15; eo++)
       {
           DrawLine(50 + eo * 50, 50, 50 + eo * 50, 750, BLACK);
       }




       if(shouldmove)
       {  move();
           shouldmove=false;
           wait_node=ce_cur_node;

       }
       if(shouldenmove)
       {
       en_move(y1,x1);
       shouldenmove=false;
           wait_node=ce_cur_node;
           shouldmove=true;
       }
       if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
       {mouseposition=GetMousePosition();
          x1= ((mouseposition.x-50)/25)/2;
          y1=((mouseposition.y-50)/25)/2;
          shouldenmove=true;

       }
























       scan(wait_node);
       EndDrawing();
   }


    return 0;
}
