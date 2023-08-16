#include "raylib.h"
#include "resources/hero engine.h"
int main()
{
    const int screenWidth = 1;
    const int screenHeight = 1;
    InitWindow(screenWidth, screenHeight, " ");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowIcon(LoadImage("resources/icon.png"));
    SetWindowMinSize(1280,720);
    SetTargetFPS(60);
    Initialize();
    Font font = LoadFont("resources/font.ttf");
    SetExitKey(KEY_F1);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (scene)
        {
        case 0:
            MainMenu();
            if(IsKeyPressed(KEY_ENTER)) scene = 1;
            break;
        
        case 1:
            update();
            break;
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
