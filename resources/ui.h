#include "raylib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "raygui.h"

class Button{
    public:std::string text;
    Color color = {200,191,231,255};
    public:bool isPressed;
    public: Vector4 xywh = {0,0,0,0};
    void update(){
        isPressed = 0;
            if(GetMousePosition().y > xywh.y && GetMousePosition().y < xywh.y + xywh.w && GetMousePosition().x > xywh.x && GetMousePosition().x < xywh.x + xywh.z){
                color = WHITE;
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    isPressed = 1;
            }else color = {200,191,231,255};
        
        DrawRectangle(xywh.x,xywh.y,xywh.z,xywh.w,color);
        DrawText(text.c_str(),xywh.x+10,xywh.y+10,20,BLACK);
    }
};

Color UIdebugColor = {92,88,133,200};
int uidebugtextsize = 20;
void debugUi(){
    DrawRectangle(0,0,350,150,UIdebugColor);
    DrawText("P <- Reload Level",10,0,uidebugtextsize,WHITE);
    DrawText("F1 <- Force quit",10,30,uidebugtextsize,WHITE);

}

