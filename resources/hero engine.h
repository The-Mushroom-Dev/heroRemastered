#include "ui.h"
Font font ;
bool isRunning = 1;
int scene = 0;

int tileSize = 48, playerSize = 64;
Color colcolor = {255,0,0,100};

class Player{
public:
        public:Vector2 position = {100, 0};
        public:Vector2 velocity = {0, 0};
        float speed = 0.4;
        float maxSpeed = 7;
        public: bool isGrounded = 0;
        public: int life = 3;
        public: int InvincibilityTime = 120;
        Texture2D sprite;
        void Update(){
            if(InvincibilityTime > 0) InvincibilityTime--;
           static Rectangle rec = {0,0,32,32};
            if (IsKeyDown(KEY_A)){
                rec = {0,0,-32,32};
                velocity.x -= speed;
                if (velocity.x < -maxSpeed) velocity.x = -maxSpeed;
            }
            else if (IsKeyDown(KEY_D)){
                rec = {0,0,32,32};
                velocity.x += speed;
                if (velocity.x > maxSpeed) velocity.x = maxSpeed;
            }
            else{
                if (velocity.x > 0){
                    velocity.x -= speed;
                    if (velocity.x < 0) velocity.x = 0;
                }
                else if (velocity.x < 0){
                    velocity.x += speed;
                    if (velocity.x > 0) velocity.x = 0;
                }
            }

            if (!isGrounded) {
                velocity.y += 2;
                if (velocity.y > 8)
                    velocity.y = 8;
            }
            if (IsKeyPressed(KEY_SPACE) && isGrounded) 
                velocity.y -= 20;
            
            if(isRunning == 1){
                position.x += velocity.x;
                position.y += velocity.y;
            }

            Rectangle rec1 = {position.x,position.y,playerSize,playerSize};
            DrawTexturePro(sprite,rec,rec1,{0,0},0,WHITE);
            if(InvincibilityTime != 0)
                DrawCircle(position.x+playerSize/2,position.y+playerSize/2,45,{255,0,0,100});
        }
};

class Slime{
        public:Vector2 position = {1000, 0};
        public:Vector2 velocity = {0, 0};
        float speed = 1;
        public: bool isGrounded = 0;
        public:Texture2D sprite;
        public:void update(){
            static Rectangle rec = {0,0,16,16};

            if (!isGrounded) {
                velocity.y += 1;
                if (velocity.y > 8)
                    velocity.y = 8;
            }else velocity.y -= 20;

            if(isRunning == 1){
                position.x += velocity.x;
                position.y += velocity.y;
            }

            Rectangle rec1 = {position.x,position.y,playerSize,playerSize};
            DrawTexturePro(sprite,rec,rec1,{0,0},0,WHITE); // <- works with flip
        }
        
};


class Tile{
    public:Vector2 position = {0,600};
    public: Texture2D texture;
    public:void update(){
        DrawTextureEx(texture,position,0,3,WHITE);
    }
};

Player player;
std::vector<Tile> GrassTiles;
std::vector<Slime> Slimes;
void Initialize(){
    font = LoadFont("resources/font.ttf");
    Texture2D grassTexture = LoadTexture("resources/grass.png");
    Texture2D groundTexture = LoadTexture("resources/dirt.png");
    Texture2D dirtboneTexture = LoadTexture("resources/dirt_bone.png");
    Texture2D bonegrassTexture = LoadTexture("resources/grass_bone.png");
    Texture2D slimeTexture = LoadTexture("resources/slime.png");
    player.sprite = LoadTexture("resources/player.png");
    for(int i = 0; i < 50; i++){
        Tile newTile;
        newTile.position.x = i * tileSize;
        if(i % 10 == 0)
        newTile.texture = bonegrassTexture;
        else newTile.texture = grassTexture;
        GrassTiles.push_back(newTile);
    }

    for(int i = 0; i < 50;i++)
        for(int j = 0; j < 50; j++){
            Tile newTile;
            newTile.position.x = i * tileSize;
            newTile.position.y = j*tileSize + 600+tileSize;

            newTile.texture = groundTexture;
            GrassTiles.push_back(newTile);
        }
    Slime newslime;
    Slimes.push_back(newslime);
    for(Slime &slime : Slimes)
        slime.sprite = slimeTexture;
}


void ReloadLevel(){
    std::cout << "Level Reloaded\n";
    while(!GrassTiles.empty())
        GrassTiles.pop_back();
    Player newPlayer;
    player = newPlayer;
    Slime newslime;
    for(Slime &slime : Slimes)
    slime = newslime;
    Initialize();
}

void PauseMenu(){
    if(IsKeyPressed(KEY_F11)){
            if(!IsWindowFullscreen()){
                SetWindowSize(GetMonitorWidth(0),GetMonitorHeight(0));
                SetWindowState(FLAG_FULLSCREEN_MODE);
            }else {
                SetWindowState(FLAG_WINDOW_RESIZABLE);
                SetWindowSize(720,480);
                SetWindowPosition(50,50);
            }
    }

    DrawRectangle(0,0,GetScreenWidth()+200,GetScreenHeight()+200,UIdebugColor);
    static Button Restart;
    Restart.update();
    Restart.text = "Restart";
    Restart.xywh = {100,250,100,40};
    if(Restart.isPressed){
        ReloadLevel();
        isRunning = !isRunning;
    }
    static Button Menu;
    Menu.update();
    Menu.text = "Menu";
    Menu.xywh = {100,350,100,40};
    if(Menu.isPressed){
        scene = 0;
        isRunning = !isRunning;
        ReloadLevel();
    }
}

void update(){
    for(int i = 0; i < GrassTiles.size();i++){
        GrassTiles[i].update();
    }
    for(int i = 0; i < GrassTiles.size();i++){
        if(player.position.y + playerSize >= GrassTiles[i].position.y
        && player.position.y <= GrassTiles[i].position.y + tileSize
        && player.position.x + playerSize >= GrassTiles[i].position.x 
        && player.position.x <= GrassTiles[i].position.x + tileSize)
        {
            player.velocity.y = 0;
            player.position.y = GrassTiles[i].position.y - playerSize;
            player.isGrounded = 1;
            break;
        }else player.isGrounded = 0;
    }


    for(Slime &slime : Slimes){
        slime.update();

        for(int i = 0; i < GrassTiles.size();i++){
            if(slime.position.y + 64 >= GrassTiles[i].position.y
            && slime.position.y <= GrassTiles[i].position.y + tileSize
            && slime.position.x + 64 >= GrassTiles[i].position.x 
            && slime.position.x <= GrassTiles[i].position.x + tileSize)
            {
                slime.velocity.y = 0;
                slime.position.y = GrassTiles[i].position.y - 64;
                slime.isGrounded = 1;
                break;
            }else slime.isGrounded = 0;
        }
        if(slime.position.x - player.position.x > 0 && slime.position.x - player.position.x < 100)
            slime.velocity.x = -slime.speed;
        else if(player.position.x - slime.position.x > 0 && player.position.x - slime.position.x < 100)
            slime.velocity.x = slime.speed;
        else slime.velocity.x = 0;

        if(slime.position.y + playerSize > player.position.y && slime.position.y < player.position.y + playerSize){
            if((slime.position.x + playerSize > player.position.x && slime.position.x < player.position.x + playerSize)
            || player.position.x == slime.position.x)
                if(player.InvincibilityTime == 0){
                    player.life--;
                    player.InvincibilityTime = 120;
                    if(player.life <= 0) ReloadLevel();
                }
        }
    }

    player.Update();

    if(IsKeyPressed(KEY_ESCAPE)) 
        isRunning = !isRunning;
    if(!isRunning){
         PauseMenu();
    }

    if(IsKeyPressed(KEY_P)) ReloadLevel();
    debugUi();
    DrawText(std::to_string(player.life).c_str(),7*uidebugtextsize,60,uidebugtextsize,WHITE);
    DrawText("lives: ",10,60,uidebugtextsize,WHITE);


}




void MainMenu(){
    static Color bgColor = { 86,90,164, 200 };
    static Texture2D mainArt = LoadTexture("resources/MainMenuArt.png");

    DrawTexturePro(mainArt,Rectangle{0,0,1920,1080},
    Rectangle{0,0,GetScreenWidth(),GetScreenHeight()},Vector2{0,0},
    0,WHITE);

    DrawRectangle(0,GetScreenHeight()-70,GetScreenWidth(),300,bgColor);
    DrawTextEx(font,"Enter <- START",Vector2{50 , GetScreenHeight()-50},30,5,WHITE);
    DrawTextEx(font,"F11 <- toggle FullScreen",Vector2{GetScreenWidth()- 400 , GetScreenHeight()-50},25,5,WHITE);
    if(IsKeyPressed(KEY_F11)){
            if(!IsWindowFullscreen()){
                SetWindowSize(GetMonitorWidth(0),GetMonitorHeight(0));
                SetWindowState(FLAG_FULLSCREEN_MODE);
            }else {
                SetWindowState(FLAG_WINDOW_RESIZABLE);
                SetWindowSize(720,480);
                SetWindowPosition(50,50);
            }
        }
    //DrawTextPro(font,"Game Art for the menu", Vector2{GetScreenWidth()/2- 300, GetScreenHeight()/2-300},Vector2{0,0},60,40,10,WHITE);

    /* rendering an invisible square to fix some shader/buffer issues*/{
        static int sqx , sqy, mx = 50,my = 50;static  Color col = {0,0,0,0}; DrawRectangle(sqx,sqy,100,100,col);sqx+= mx; sqy+=my;if(sqx + 100 > GetScreenWidth() || sqx <= 0)mx = -mx;if(sqy + 100 > GetScreenHeight() || sqy <= 0)my = -my;
    }

}
