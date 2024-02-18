
#include "res.h"
#include <raylib.h>

Sound hurt_snd = { 0 };

Music menu_music = { 0 };
Music play_music = { 0 };

Sound * kill_snd[6] = { 0 };

Sound kill0 = { 0 };
Sound kill1 = { 0 };
Sound kill2 = { 0 };
Sound kill3 = { 0 };
Sound kill4 = { 0 };
Sound kill5 = { 0 };

Sound * slash_snd[4] = { 0 };

Sound slash0 = { 0 };
Sound slash1 = { 0 };
Sound slash2 = { 0 };
Sound slash3 = { 0 };

Texture god_tex = { 0 };

Sound transition_snd = { 0 };

Texture * player_tex[4] = { 0 };

Texture hero_dn_tex    = { 0 };
Texture hero_lt_tex    = { 0 };
Texture hero_rt_tex    = { 0 };
Texture hero_up_tex    = { 0 };

Texture * bg_tex[3] = { 0 };

Texture bg_tex0    = { 0 };
Texture bg_tex1    = { 0 };
Texture bg_tex2    = { 0 };

Texture * eye_tex[4] = { 0 };

Texture eye_dn_tex    = { 0 };
Texture eye_lt_tex    = { 0 };
Texture eye_rt_tex    = { 0 };
Texture eye_up_tex    = { 0 };

Texture * atk_tex[4] = { 0 };

Texture atk_dn_tex    = { 0 };
Texture atk_lt_tex    = { 0 };
Texture atk_rt_tex    = { 0 };
Texture atk_up_tex    = { 0 };

Texture hero_sp_tex    = { 0 };

Texture * slash_tex[4][3] = { 0 };

Texture slash_dn_0_tex = { 0 };
Texture slash_dn_1_tex = { 0 };
Texture slash_dn_2_tex = { 0 };
Texture slash_lt_0_tex = { 0 };
Texture slash_lt_1_tex = { 0 };
Texture slash_lt_2_tex = { 0 };
Texture slash_rt_0_tex = { 0 };
Texture slash_rt_1_tex = { 0 };
Texture slash_rt_2_tex = { 0 };
Texture slash_up_0_tex = { 0 };
Texture slash_up_1_tex = { 0 };
Texture slash_up_2_tex = { 0 };

void res_init(){
    
    hurt_snd = LoadSound("res/hurt.wav");
    transition_snd = LoadSound("res/transition.wav");

    god_tex = LoadTexture("res/god.png");

    kill0 = LoadSound("res/kill0.wav");
    kill1 = LoadSound("res/kill1.wav");
    kill2 = LoadSound("res/kill2.wav");
    kill3 = LoadSound("res/kill3.wav");
    kill4 = LoadSound("res/kill4.wav");
    kill5 = LoadSound("res/kill5.wav");
	
    kill_snd[0] = &kill0;
    kill_snd[1] = &kill1;
    kill_snd[2] = &kill2;
    kill_snd[3] = &kill3;
    kill_snd[4] = &kill4;
    kill_snd[5] = &kill5;

    slash0 = LoadSound("res/slash0.wav");
    slash1 = LoadSound("res/slash1.wav");
    slash2 = LoadSound("res/slash2.wav");
    slash3 = LoadSound("res/slash3.wav");
	
    slash_snd[0] = &slash0;
    slash_snd[1] = &slash1;
    slash_snd[2] = &slash2;
    slash_snd[3] = &slash3;
    
    menu_music = LoadMusicStream("res/menu_music.ogg");
    play_music = LoadMusicStream("res/play_music.ogg");

	hero_sp_tex    = LoadTexture("res/hero_sp.png");

	hero_dn_tex    = LoadTexture("res/hero_dn.png");
	hero_lt_tex    = LoadTexture("res/hero_lt.png");
	hero_rt_tex    = LoadTexture("res/hero_rt.png");
	hero_up_tex    = LoadTexture("res/hero_up.png");

    player_tex[0] = &hero_up_tex;
    player_tex[1] = &hero_rt_tex;
    player_tex[2] = &hero_dn_tex;
    player_tex[3] = &hero_lt_tex;

	eye_dn_tex    = LoadTexture("res/eye_dn.png");
	eye_lt_tex    = LoadTexture("res/eye_lt.png");
	eye_rt_tex    = LoadTexture("res/eye_rt.png");
	eye_up_tex    = LoadTexture("res/eye_up.png");

    eye_tex[0] = &eye_up_tex;
    eye_tex[1] = &eye_rt_tex;
    eye_tex[2] = &eye_dn_tex;
    eye_tex[3] = &eye_lt_tex;

	bg_tex0    = LoadTexture("res/bg0.png");
	bg_tex1    = LoadTexture("res/bg1.png");
	bg_tex2    = LoadTexture("res/bg2.png");
    
    bg_tex[0] = &bg_tex0;
    bg_tex[1] = &bg_tex1;
    bg_tex[2] = &bg_tex2;

	atk_dn_tex    = LoadTexture("res/atk_dn.png");
	atk_lt_tex    = LoadTexture("res/atk_lt.png");
	atk_rt_tex    = LoadTexture("res/atk_rt.png");
	atk_up_tex    = LoadTexture("res/atk_up.png");

    atk_tex[0] = &atk_up_tex;
    atk_tex[1] = &atk_rt_tex;
    atk_tex[2] = &atk_dn_tex;
    atk_tex[3] = &atk_lt_tex;

	slash_dn_0_tex = LoadTexture("res/slash_dn_0.png");
	slash_dn_1_tex = LoadTexture("res/slash_dn_1.png");
	slash_dn_2_tex = LoadTexture("res/slash_dn_2.png");
	slash_lt_0_tex = LoadTexture("res/slash_lt_0.png");
	slash_lt_1_tex = LoadTexture("res/slash_lt_1.png");
	slash_lt_2_tex = LoadTexture("res/slash_lt_2.png");
	slash_rt_0_tex = LoadTexture("res/slash_rt_0.png");
	slash_rt_1_tex = LoadTexture("res/slash_rt_1.png");
	slash_rt_2_tex = LoadTexture("res/slash_rt_2.png");
	slash_up_0_tex = LoadTexture("res/slash_up_0.png");
	slash_up_1_tex = LoadTexture("res/slash_up_1.png");
	slash_up_2_tex = LoadTexture("res/slash_up_2.png");

    slash_tex[0][0] = &slash_up_0_tex;
    slash_tex[0][1] = &slash_up_1_tex;
    slash_tex[0][2] = &slash_up_2_tex;
    slash_tex[1][0] = &slash_rt_0_tex;
    slash_tex[1][1] = &slash_rt_1_tex;
    slash_tex[1][2] = &slash_rt_2_tex;
    slash_tex[2][0] = &slash_dn_0_tex;
    slash_tex[2][1] = &slash_dn_1_tex;
    slash_tex[2][2] = &slash_dn_2_tex;
    slash_tex[3][0] = &slash_lt_0_tex;
    slash_tex[3][1] = &slash_lt_1_tex;
    slash_tex[3][2] = &slash_lt_2_tex;
		
}
