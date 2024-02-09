
#ifndef TJ_RES_H
#define TJ_RES_H

#include "raylib.h"

void res_init();

extern Texture * player_tex[4];
extern Texture hero_sp_tex;
extern Texture * eye_tex[4];
extern Texture * atk_tex[4];
extern Texture * slash_tex[4][3];
extern Texture god_tex;

extern Sound * kill_snd[6];
extern Sound * slash_snd[4];
extern Sound transition_snd;
extern Sound hurt_snd;

extern Music menu_music;
extern Music play_music;

#endif
