/*******************************************************************************************
*
*   raylib gamejam template
*
*   Template originally created with raylib 4.5-dev, last time updated with raylib 5.0
*
*   Template licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "res.h"

#include "vector.h"

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const int screen_w   = 128 * 4;
static const int screen_h   =  96 * 4;
static const int internal_w = 128;
static const int internal_h =  96;


static RenderTexture2D target = { 0 };  // Render texture to render our game

typedef enum {
    DIR_N,
    DIR_E,
    DIR_S,
    DIR_W,
} dir_e;

typedef enum {
    GS_MENU_LOAD,
    GS_MENU_IDLE,
    GS_GAME_LOAD,
    GS_GAME_PLAY,
    GS_GAME_WIN,
    GS_GAME_LOSE,
} game_state_e;

typedef struct {
    game_state_e state;
    float time_state_entered;
    dir_e player_dir;
    bool changed_dir;
    int frames_since_changed_dir;
    float win_time;
    bool player_attacking;
    float enemy_time;
    bool enemy[4];
    int enemies_killed;
    int player_health;
    int godhealth;
    float enemy_destroy_time[4];
    int nonenemy;
    int round;
    bool hit_last;
    dir_e hit_dir;
    vector * next_press;
} game_state_t;

game_state_t gs = {
    .state = GS_MENU_LOAD,
    .player_dir = DIR_S,
    .time_state_entered = 0,
    .changed_dir = false,
    .player_attacking = false,
    .frames_since_changed_dir = 0,
    .enemy = { 0 },
    .godhealth = 20,
    .player_health = 3,
    .enemies_killed = 0,
    .enemy_destroy_time = { 0 },
    .enemy_time = 0,
    .nonenemy = 0,
    .round = 0,
    .next_press = NULL,
};

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    SetTraceLogLevel(LOG_DEBUG);         // Disable raylib trace log messsages

    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_w, screen_h, "100 EYES");
    InitAudioDevice();
    SetMasterVolume(.7);
    SetMusicVolume(menu_music, 0.1);
    
    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(internal_w, internal_h);
    res_init();
    PlayMusicStream(menu_music);
    float t = GetTime();
    SetRandomSeed(t * 1000000);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    gs.time_state_entered = GetTime();
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);
    
    // TODO: Unload all loaded resources at this point

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

bool has_played = false;

void update_menu(){

    float t = GetTime();
    if(gs.state == GS_MENU_LOAD && t - gs.time_state_entered < 1.4){
        SetMusicVolume(menu_music, (t - gs.time_state_entered) / 2 );
    } else if (gs.state == GS_MENU_LOAD) {
        gs.state = GS_MENU_IDLE;
// #if defined(PLATFORM_WEB)
//         int x = EM_ASM_INT({
//         let f = localStorage.played100eyes;
//         if (f == undefined){
//             return 0;
//         }
//         return 1;
//         });
//         has_played = x;
// #endif
    }
    UpdateMusicStream(menu_music);

    BeginTextureMode(target);
        ClearBackground(BLACK);

        Texture * h = player_tex[gs.player_dir];
        DrawTexture(*h, internal_w / 2 - h->width / 2, internal_h / 2 - h->height / 2, WHITE);
        
    EndTextureMode();
    
    Color blit_color = WHITE;
    if(gs.state == GS_MENU_LOAD){
        blit_color.r *= (GetTime() - gs.time_state_entered) / 2;
        blit_color.g *= (GetTime() - gs.time_state_entered) / 2;
        blit_color.b *= (GetTime() - gs.time_state_entered) / 2;
    }

    // Render to screen (main framebuffer)
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, GetScreenWidth(), GetScreenHeight() }, (Vector2){ 0, 0 }, 0.0f, blit_color);
        if(gs.state == GS_MENU_IDLE && has_played){
            DrawText("100 EYES", GetScreenWidth() / 2 - MeasureText("100 EYES", 30) / 2, GetScreenHeight() / 4, 30, WHITE);
            DrawText("YOU HAVE ALREADY TRIED ONCE", GetScreenWidth() / 2 - MeasureText("YOU HAVE ALREADY TRIED ONCE", 20) / 2, GetScreenHeight() * 3 / 4, 20, RED);
        } else if (gs.state == GS_MENU_IDLE) {
            DrawText("100 EYES", GetScreenWidth() / 2 - MeasureText("100 EYES", 30) / 2, GetScreenHeight() / 4, 30, WHITE);
            if(GuiButton((Rectangle){.x = GetScreenWidth() / 2. - 50, .y = GetScreenHeight() * 3. / 4, .width = 100, .height = 30}, "PLAY")){
                StopMusicStream(menu_music);
                PlayMusicStream(play_music);
                PlaySound(transition_snd);
                gs.state = GS_GAME_LOAD;
            }
        }
    EndDrawing();

}

void game_load(){

// #if defined(PLATFORM_WEB)
//     emscripten_run_script("localStorage.played100eyes=1");
// #endif

    game_state_t t = {
        .state = GS_GAME_PLAY,
        .player_dir = DIR_S,
        .time_state_entered = GetTime(),
        .changed_dir = false,
        .player_attacking = false,
        .frames_since_changed_dir = 0,
        .godhealth = 20,
        .enemy = { true, true, true, true },
        .player_health = 3,
        .enemies_killed = 0,
        .enemy_destroy_time = { 0 },
        .enemy_time = 0,
        .nonenemy = GetRandomValue(0, 3),
        .round = 0,
        .next_press = vector_init(sizeof(dir_e)),
    };

    t.enemy_time = 2 - 2 * (t.round/66.);    
    t.enemy[gs.nonenemy] = false;

    if(gs.next_press != NULL)
        vector_free(gs.next_press);

    gs = t;
}

void game_play(){

    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update variables / Implement example logic at this point
    //----------------------------------------------------------------------------------

    UpdateMusicStream(play_music);

    bool new_wave = false;
    if (!gs.enemy[0] && !gs.enemy[1] && !gs.enemy[2] && !gs.enemy[3] && gs.round != 33){
        new_wave = true;
        gs.round++;
    }

    if(new_wave){
        gs.enemy[0] = true;
        gs.enemy[1] = true;
        gs.enemy[2] = true;
        gs.enemy[3] = true;
        gs.enemy_destroy_time[0] = 0;
        gs.enemy_destroy_time[1] = 0;
        gs.enemy_destroy_time[2] = 0;
        gs.enemy_destroy_time[3] = 0;
        gs.enemy_time = 2 - 2 * (gs.round/66.);
        gs.enemy[GetRandomValue(0, 3)] = false;
    }

    bool enemy_attacks = false;
    gs.enemy_time -= GetFrameTime();
    if(gs.enemy_time < 0 && gs.round != 33){
        enemy_attacks = true;
        gs.enemy_time = 2 - 2 * (gs.round/66.);
    }

    dir_e old_pd = gs.player_dir;
    bool pressed_key = false;
    int key = GetKeyPressed();
    while(key != 0){
        if(key == KEY_W || key == KEY_UP)
            vector_push(gs.next_press, &(dir_e){DIR_N});
        if(key == KEY_D || key == KEY_RIGHT)
            vector_push(gs.next_press, &(dir_e){DIR_E});
        if(key == KEY_S || key == KEY_DOWN)
            vector_push(gs.next_press, &(dir_e){DIR_S});
        if(key == KEY_A || key == KEY_LEFT)
            vector_push(gs.next_press, &(dir_e){DIR_W});
        key = GetKeyPressed();
    }

    if(!gs.player_attacking && !vector_empty(gs.next_press)){
        gs.player_dir = *(dir_e *)vector_at(gs.next_press, 0);
        pressed_key = true;
        vector_erase(gs.next_press, 0);
    }
    if(old_pd != gs.player_dir)
        gs.changed_dir = true;
    
    bool got_hit = false;
    dir_e hit_dir = DIR_N;

    if (pressed_key && gs.round == 33) {
        gs.godhealth--;
        PlaySound(*slash_snd[GetRandomValue(0, 3)]);
        gs.player_attacking = true;
        gs.frames_since_changed_dir = 0;
        if(gs.godhealth == 0){
            gs.win_time = GetTime();
            gs.state = GS_GAME_WIN;
        }
    } else if(pressed_key && gs.enemy[gs.player_dir] ){
        gs.enemy_destroy_time[gs.player_dir] = .2f;
        gs.enemies_killed++;
        PlaySound(*kill_snd[GetRandomValue(0, 5)]);
        PlaySound(*slash_snd[GetRandomValue(0, 3)]);
        gs.player_attacking = true;
        gs.frames_since_changed_dir = 0;
    } else if (pressed_key || enemy_attacks) {
        gs.player_health--;
        if(gs.player_health == 0){
            gs.state = GS_GAME_LOSE;
        }
        got_hit = true;
        PlaySound(hurt_snd);
        hit_dir = GetRandomValue(0, 3);
        while(!gs.enemy[hit_dir]){
            hit_dir = GetRandomValue(0, 3);
        }
    }

    // Draw
    //----------------------------------------------------------------------------------
    // Render game screen to a texture, 
    // it could be useful for scaling or further sahder postprocessing
    BeginTextureMode(target);
        ClearBackground(BLACK);

        Color c = WHITE;
        if (gs.player_attacking && gs.frames_since_changed_dir == 1){
            c = PURPLE;
        }
        int i = fmodf(GetTime(), 10) / 3.34;
        DrawTexture(*bg_tex[i], (internal_w - bg_tex[i]->width) / 2. + sinf(GetTime() * 5) * 2, (internal_h - bg_tex[i]->height) / 2. + sinf(GetTime() * 9) * 2, c);

        if(gs.round == 33){
            Color c = WHITE;
            if (gs.player_attacking){
                c = RED;
                c.r /= 2;
                c.g /= 2;
                c.b /= 2;
            }
            DrawTexture(god_tex, 8 + sinf(GetTime() * GetRandomValue(1, 7)) * 2, 8 + sinf(GetTime() * GetRandomValue(1, 7)) * 2, c);
            if(fmodf(GetTime(),1) > 0.5)
                DrawText("ATTACK!!!", internal_w / 2 - MeasureText("ATTACK!!!", 20) / 2, internal_h * 3 / 4, 20, RED);
        }

        Texture * h = player_tex[gs.player_dir];
        if(gs.changed_dir){
            gs.changed_dir = false;
            h = &hero_sp_tex;
        }
        DrawTexture(*h, internal_w / 2 - h->width / 2, internal_h / 2 - h->height / 2, WHITE);

        if(gs.player_attacking){
            Texture * s = slash_tex[gs.player_dir][gs.frames_since_changed_dir];
            DrawTexture(*s, internal_w / 2 - s->width / 2, internal_h / 2 - s->height / 2, WHITE);
            gs.frames_since_changed_dir++;
            if(gs.frames_since_changed_dir == 3)
                gs.player_attacking = 0;
        }

        if(gs.enemy[DIR_N] && gs.round != 33){
            Color c = WHITE;
            if (gs.enemy_destroy_time[DIR_N] != 0){
                gs.enemy[DIR_N] = false;
                c = RED;
            }
            if (new_wave)
                c = YELLOW;
            DrawTexture(*eye_tex[DIR_N], (internal_w - eye_tex[DIR_N]->width) / 2. + sinf(GetTime() * 5) * 2, (internal_h - eye_tex[DIR_N]->height) / 2. + sinf(GetTime() * 2) * 2, c);
        }
        if(gs.enemy[DIR_E] && gs.round != 33){
            Color c = WHITE;
            if (gs.enemy_destroy_time[DIR_E] != 0){
                gs.enemy[DIR_E] = false;
                c = RED;
            }
            if (new_wave)
                c = YELLOW;
            DrawTexture(*eye_tex[DIR_E], (internal_w - eye_tex[DIR_E]->width) / 2. + sinf(GetTime() * 3) * 2, (internal_h - eye_tex[DIR_E]->height) / 2. + sinf(GetTime() * 1) * 2, c);
        }
        if(gs.enemy[DIR_S] && gs.round != 33){
            Color c = WHITE;
            if (gs.enemy_destroy_time[DIR_S] != 0){
                gs.enemy[DIR_S] = false;
                c = RED;
            }
            if (new_wave)
                c = YELLOW;
            DrawTexture(*eye_tex[DIR_S], (internal_w - eye_tex[DIR_S]->width) / 2. + sinf(GetTime() * 5) * 2, (internal_h - eye_tex[DIR_S]->height) / 2. + sinf(GetTime() * 4) * 2, c);
        }
        if(gs.enemy[DIR_W] && gs.round != 33){
            Color c = WHITE;
            if (gs.enemy_destroy_time[DIR_W] != 0){
                gs.enemy[DIR_W] = false;
                c = RED;
            }
            if (new_wave)
                c = YELLOW;
            DrawTexture(*eye_tex[DIR_W], (internal_w - eye_tex[DIR_W]->width) / 2. + sinf(GetTime() * 1) * 2, (internal_h - eye_tex[DIR_W]->height) / 2. + sinf(GetTime() * 5) * 2, c);
        }

        if(got_hit || gs.hit_last){
            Color tint = WHITE;
            if(!got_hit && gs.hit_last){
                tint = BLUE;
                gs.hit_last = false;
                hit_dir = gs.hit_dir;
            } else {
                gs.hit_last = true;
                gs.hit_dir = hit_dir;
            }
            DrawTexture(*atk_tex[hit_dir], (internal_w - atk_tex[DIR_W]->width) / 2. + sinf(GetTime() * 1) * 2, (internal_h - atk_tex[hit_dir]->height) / 2. + sinf(GetTime() * 5) * 2, tint);
        }
        
    EndTextureMode();
    
    // Render to screen (main framebuffer)
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, GetScreenWidth(), GetScreenHeight() }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        if(gs.round != 33){
            char tmp[100] = { 0 };
            Color black = BLACK;
            black.a = 128;
            DrawRectangle(GetScreenWidth() / 4 - 4, GetScreenHeight() / 2 - 4, 76, 60, black);
            sprintf(tmp, "health: %d", gs.player_health);
            DrawText(tmp, GetScreenWidth() / 4, GetScreenHeight() / 2, 10, GREEN);
            memset(tmp, 0, 100);
            sprintf(tmp, "killed: %d", gs.enemies_killed);
            DrawText(tmp, GetScreenWidth() / 4, GetScreenHeight() / 2 + 20, 10, YELLOW);
            memset(tmp, 0, 100);
            sprintf(tmp, "atk_time: %.2f", gs.enemy_time);
            DrawText(tmp, GetScreenWidth() / 4, GetScreenHeight() / 2 + 40, 10, RED);
        }
    EndDrawing();
}

void game_lose(){

    
    if(IsMusicStreamPlaying(play_music)){
        StopMusicStream(play_music);
        PlaySound(transition_snd);
    }

    // Render to screen (main framebuffer)
    BeginDrawing();
        ClearBackground(BLACK);
        DrawText("DEFEAT", GetScreenWidth() / 2 - MeasureText("DEFEAT", 30) / 2, GetScreenHeight() / 4, 30, RED);

        if(GuiButton((Rectangle){.x = GetScreenWidth() / 2. - 50, .y = GetScreenHeight() * 3. / 4, .width = 100, .height = 30}, "MENU")){
            PlayMusicStream(menu_music);
            gs.state = GS_MENU_LOAD;
        }
    EndDrawing();

}

void game_win(){
    
    if(IsMusicStreamPlaying(play_music)){
        StopMusicStream(play_music);
        PlaySound(transition_snd);
    }

    // Render to screen (main framebuffer)
    BeginDrawing();
        ClearBackground(BLACK);
        DrawText("VICTORY", GetScreenWidth() / 2 - MeasureText("VICTORY", 30) / 2, GetScreenHeight() / 4, 30, GREEN);
        char tmp[100] = { 0 };
        sprintf(tmp, "total time: %.2fs", gs.win_time - gs.time_state_entered);
        DrawText(tmp, GetScreenWidth() / 2 - MeasureText(tmp, 20) / 2, GetScreenHeight() * 4 / 10, 20, YELLOW);
        memset(tmp, 0, 100);
        sprintf(tmp, "health bonus: %ds", gs.player_health * -5);
        DrawText(tmp, GetScreenWidth() / 2 - MeasureText(tmp, 20) / 2, GetScreenHeight() * 5 / 10, 20, SKYBLUE);
        memset(tmp, 0, 100);
        sprintf(tmp, "final time: %.2fs", (gs.win_time - gs.time_state_entered) - gs.player_health * 5);
        DrawText(tmp, GetScreenWidth() / 2 - MeasureText(tmp, 20) / 2, GetScreenHeight() * 6 / 10, 20, GREEN);
        memset(tmp, 0, 100);

        if(GuiButton((Rectangle){.x = GetScreenWidth() / 2. - 50, .y = GetScreenHeight() * 3. / 4, .width = 100, .height = 30}, "MENU")){
            PlayMusicStream(menu_music);
            gs.state = GS_MENU_LOAD;
            gs.player_dir = DIR_S;
        }
    EndDrawing();

}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{
    switch (gs.state){
        case GS_MENU_LOAD:
            update_menu();
            break;
        case GS_MENU_IDLE:
            update_menu();
            break;
        case GS_GAME_LOAD:
            game_load();
            // break; //todo
        case GS_GAME_PLAY:
            game_play();
            break;
        case GS_GAME_LOSE:
            game_lose();
            break;
        case GS_GAME_WIN:
            game_win();
            break;
        default:
            fprintf(stderr, "fuckstate\n");
            break;
    }
    //----------------------------------------------------------------------------------  
}
