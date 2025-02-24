#include <iostream>
#include <raylib.h>
#include <ctime>
#include <string>

using namespace std;

int player_score = 0;
int cpu_score = 0;

Sound oh_sound;
Sound bruh_sound;

Color White = {255, 255, 255, 255};    
Color Green = {0, 255, 0, 255};    
Color Blue = {130, 130, 255, 255};         
Color LightBlue = {173, 216, 255, 255}; 
Color Red = {255, 0, 0, 255};          
Color Yellow = {255, 255, 0, 255};     
Color Orange = {255, 165, 0, 255};     
Color Pink = {255, 192, 203, 255};

Color colors[] = {White, Green, Blue, LightBlue, Red, Yellow, Orange, Pink};

class Ball 
{
    public:
    float x, y;
    int speed_x, speed_y;
    int radius;
    Color color = colors[0];

    void Draw()
    {
        DrawCircle(x, y, radius, color); // Ball
    }

    void ChangeColor()
    {
        int numColors = sizeof(colors) / sizeof(colors[0]);
        color = colors[GetRandomValue(0, numColors - 1)];
    }

    void Update()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
            SetSoundVolume(bruh_sound, 0.1f);
            PlaySound(bruh_sound);
            ChangeColor();
        }
        
        // Scoring
        if (x - radius <= 0)
        {
            player_score++;
            ResetBall();
            SetSoundVolume(oh_sound, 0.1f);
            PlaySound(oh_sound);
        }
        if (x + radius >= GetScreenWidth())
        {
            cpu_score++;
            ResetBall();
            SetSoundVolume(oh_sound, 0.1f);
            PlaySound(oh_sound);
        }
    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
    protected:

    void LimitMovement() {
        if (y <= 10)
        {
            y = 10;
        }
        if (y >= GetScreenHeight() - height - 10)
        {
            y = GetScreenHeight() - height - 10;
        }
    }

    public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        DrawRectangle(x, y, width, height, WHITE); // Paddle
    }

    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y += speed;
        }

        LimitMovement();

    }
};

class CpuPaddle: public Paddle
{
    public:

    void Update(int ball_y)
    {
        if (y + height/2 >= ball_y)
        {
            y -= speed;
        }
        if (y + height/2 <= ball_y)
        {
            y += speed;
        }
        
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main()
{
    cout << "Starting the game" << endl;
    const int screen_width = 1200;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);
    InitAudioDevice();
    
    bruh_sound = LoadSound("src/bruh.wav");
    oh_sound = LoadSound("src/oh.wav");

    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 6;

    while(WindowShouldClose() == false)
    {
        // Drawing
        BeginDrawing();

        // Update element movement
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        
        // Check colision
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speed_x *= -1;
            PlaySound(bruh_sound);
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speed_x *= -1;
            PlaySound(bruh_sound);
        }
        
        // Update before clear
        ClearBackground(BLACK);
        
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        DrawCircle(screen_width/2, screen_height/2, 5, WHITE);
        ball.Draw(); // Ball
        player.Draw(); // Player Paddle
        cpu.Draw(); // AI puddle
        DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), screen_width* 3/4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}