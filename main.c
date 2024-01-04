#include"wuziqi.h"
bool shouldmove=true;
bool shouldenmove=false;
Vector2 mouseposition;
bool Isover(node*np)
{if(se_5(np,ai)==1||se_5(np,human)==1)
    {return true;}
    else
    {return false;}
}
int x1,y1;
int main() {
    init_direction();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
     SetTargetFPS(10)   ;
   while (!WindowShouldClose() )
   {;
       BeginDrawing();
       ClearBackground(RAYWHITE) ;
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
           if (Isover(wait_node))
           {printf("ai win");
               return 0;
           }
           shouldmove=false;
       }
       if(shouldenmove)
       {
       en_move(y1,x1);
           if (Isover(wait_node))
           {printf("player win");
               return 0;
           }
       shouldenmove=false;
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
