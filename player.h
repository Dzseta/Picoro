#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <hagl_hal.h>
#include <hagl.h>
#include "bitmap_wolf_idle.h"
#include "bitmap_wolf_attack.h"
#include "bitmap_wolf_jump.h"
#include "bitmap_wolf_dodge.h"
#include "bitmap_wolf_mikiri.h"
#include "bitmap_wolf_heal.h"
#include "bitmap_wolf_firecracker.h"
#include "bitmap_wolf_stagger.h"
#include "bitmap_wolf_death.h"
#include "bitmap_icons.h"

static const hagl_color_t red = 0x00d8;
static const hagl_color_t darkgray = 0xcb5a;
static const hagl_color_t yellow = 0xa0fe;
static const hagl_color_t lightgreen = 0x2407;
static const hagl_color_t lightblue = 0xbf6c;

// player status
u_int8_t player_status = 0;
// player frame
u_int8_t player_frame = 0;
// player invincibility
bool player_invincible = false;
// player business
bool player_busy = false;
// block
bool player_block = false;
// jump
bool player_jump = false;
// dodge
bool player_dodge = false;
// game over
bool game_over = false;
// player bitmap
hagl_bitmap_t player_bitmap;
// health
int16_t player_health = 100;
// posture
int16_t player_posture = 50;
// heals
u_int8_t player_heal = 1;
// heals
u_int8_t player_firecracker = 1;
// quantity
wchar_t number[2];
// icon bitmap
hagl_bitmap_t* icon_bitmap_1;

void init_player_bitmap(hagl_backend_t *display, hagl_bitmap_t* icon_bitmap) {
    player_health = 100;
    player_posture = 50;
    player_status = 0;
    player_frame = 0;
    player_invincible = false;
    player_busy = false;
    player_block = false;
    player_jump = false;
    player_dodge = false;
    player_heal = 1;
    player_firecracker = 1;
    icon_bitmap_1  = icon_bitmap;
}

void player_take_damage(hagl_backend_t *display, uint8_t damage) {
    if(player_status == 11) return;
    if(player_posture == 0) player_health = 0;
    else player_health -= damage;
    if(player_health <= 0) {
        player_health = 0;
        player_status = 11;
        player_frame = 0;  
        player_busy = true;
        game_over = true;
    }
    hagl_fill_rectangle_xywh(display, 6+(player_health/2), 100, 50-(player_health/2), 5, darkgray);
}

void player_take_posture(hagl_backend_t *display, uint8_t posture) {
    player_posture -= posture;
    if(player_posture <= 0) {
        player_posture = 0;
        player_status = 10;
        player_frame = 0;  
        player_busy = true;
    }
    hagl_fill_rectangle_xywh(display, 6+(player_posture), 110, 50-(player_posture), 5, darkgray);
}

void player_drink_heal(hagl_backend_t *display) {
    // change
    if(player_heal <= 0) return;
    player_busy = true;
    player_frame = 0;
    player_status = 8;
    player_heal--;
    player_health += 33;
    if(player_health > 100) player_health = 100;
    // decrease number
    swprintf(number, sizeof(number), L"%u", player_heal);
    if(player_heal == 0) {
        hagl_put_text(display, number, 23, 20, red, font6x9);
    } else {
        hagl_put_text(display, number, 23, 20, lightgreen, font6x9);
    }
    hagl_fill_rectangle_xywh(display, 6, 100, (player_health/2), 5, red);
}

void player_animate_heal(hagl_backend_t *display) {
    if (player_frame == 0){
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &idle_1);
    } else if (player_frame == 2) {
        hagl_bitmap_init(&player_bitmap, HEAL_WIDTH, HEAL_HEIGHT, sizeof(hagl_color_t), &heal_1);
    } else if (player_frame == 5) {
        hagl_bitmap_init(&player_bitmap, HEAL_WIDTH, HEAL_HEIGHT, sizeof(hagl_color_t), &heal_2);
    } else if (player_frame == 7) {
        hagl_bitmap_init(&player_bitmap, HEAL_WIDTH, HEAL_HEIGHT, sizeof(hagl_color_t), &heal_3);
    } else if (player_frame == 10) {
        hagl_bitmap_init(&player_bitmap, HEAL_WIDTH, HEAL_HEIGHT, sizeof(hagl_color_t), &heal_4);
    } else if (player_frame == 12) {
        hagl_bitmap_init(&player_bitmap, HEAL_WIDTH, HEAL_HEIGHT, sizeof(hagl_color_t), &heal_5);
    } else if (player_frame == 40) {
        hagl_bitmap_init(&player_bitmap, HEAL_WIDTH, HEAL_HEIGHT, sizeof(hagl_color_t), &heal_4);
    } else if (player_frame == 42) {
        hagl_bitmap_init(&player_bitmap, HEAL_WIDTH, HEAL_HEIGHT, sizeof(hagl_color_t), &heal_3);
    } else if (player_frame == 45) {
        hagl_bitmap_init(&player_bitmap, HEAL_WIDTH, HEAL_HEIGHT, sizeof(hagl_color_t), &heal_2);
    } else if (player_frame == 47) {
        hagl_bitmap_init(&player_bitmap, HEAL_WIDTH, HEAL_HEIGHT, sizeof(hagl_color_t), &heal_1);
    } else if (player_frame == 60) {
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &idle_1);
        player_status = 0;
        player_frame = 0;
        if(!game_over) player_busy = false;
    }
    // draw player
    hagl_blit(display, 12, 40, &player_bitmap);
}

void player_animate_idle(hagl_backend_t *display) {
    if (player_frame >= 60){
        player_frame = 0;
        if(!game_over) player_busy = false;
    }
    if (player_frame == 0){
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &idle_1);
    } else if (player_frame == 30) {
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &idle_2);
    }
    // draw player
    hagl_blit(display, 12, 40, &player_bitmap);
}

void player_animate_block(hagl_backend_t *display) {
    if (player_frame == 0){
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_3);
    } else if (player_frame == 2) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_2);
    } else if (player_frame == 5) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &parry);
        player_block = true;
    } else if (player_frame == 6) {
        player_frame = 5;
    } else if (player_frame == 22) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_2);
        player_block = false;
    } else if (player_frame == 25) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_3);
        player_status = 0;
        player_frame = 0;
        if(!game_over) player_busy = false;
    }
    // draw player
    hagl_blit(display, 12, 40, &player_bitmap);
}

void player_animate_parry(hagl_backend_t *display) {
    if (player_frame == 0){
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_3);
    } else if (player_frame == 2) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_2);
    } else if (player_frame == 5) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &parry);
        player_invincible = true;
    } else if (player_frame == 45) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_2);
        player_invincible = false;
    } else if (player_frame == 47) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_3);
    } else if (player_frame == 57) {
        player_status = 0;
        player_frame = 0;
        if(!game_over) player_busy = false;
    }
    // draw player
    hagl_blit(display, 12, 40, &player_bitmap);
}

void player_animate_dodge(hagl_backend_t *display) {
    if (player_frame == 0){
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &idle_1);
        hagl_blit(display, 12, 40, &player_bitmap);
    } else if (player_frame == 1) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_1);
        hagl_blit(display, 11, 40, &player_bitmap);
    } else if (player_frame == 2) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_1);
        hagl_blit(display, 10, 40, &player_bitmap);
    } else if (player_frame == 3) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_2);
        hagl_blit(display, 9, 40, &player_bitmap);
    } else if (player_frame == 4) {
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &dodge_2);
        hagl_blit(display, 8, 40, &player_bitmap);
    } else if (player_frame == 5) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_1);
        hagl_blit(display, 7, 40, &player_bitmap);
    } else if (player_frame == 6) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_1);
        hagl_blit(display, 6, 40, &player_bitmap);
    } else if (player_frame == 7) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &idle_1);
        hagl_blit(display, 5, 40, &player_bitmap);
    } else if (player_frame == 8) {
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &dodge_3);
        hagl_blit(display, 4, 40, &player_bitmap);
    } else if (player_frame == 9) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_3);
        hagl_blit(display, 3, 40, &player_bitmap);
    } else if (player_frame == 10) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_4);
        hagl_blit(display, 2, 40, &player_bitmap);
    } else if (player_frame == 11) {
        player_dodge = true;
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &dodge_4);
        hagl_blit(display, 1, 40, &player_bitmap);
    } else if (player_frame == 31) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_3);
        hagl_blit(display, 2, 40, &player_bitmap);
        player_dodge = false;
    } else if (player_frame == 32) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_3);
        hagl_blit(display, 3, 40, &player_bitmap);
    } else if (player_frame == 33) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &idle_1);
        hagl_blit(display, 4, 40, &player_bitmap);
    } else if (player_frame == 34) {
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &dodge_1);
        hagl_blit(display, 5, 40, &player_bitmap);
    } else if (player_frame == 35) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_1);
        hagl_blit(display, 6, 40, &player_bitmap);
    } else if (player_frame == 36) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_2);
        hagl_blit(display, 7, 40, &player_bitmap);
    } else if (player_frame == 37) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_2);
        hagl_blit(display, 8, 40, &player_bitmap);
    } else if (player_frame == 38) {
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &dodge_1);
        hagl_blit(display, 9, 40, &player_bitmap);
    } else if (player_frame == 39) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_1);
        hagl_blit(display, 10, 40, &player_bitmap);
    } else if (player_frame == 40) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &idle_1);
        hagl_blit(display, 11, 40, &player_bitmap);
    } else if (player_frame == 41) {
        hagl_bitmap_init(&player_bitmap, DODGE_WIDTH, DODGE_HEIGHT, sizeof(hagl_color_t), &dodge_3);
        hagl_blit(display, 12, 40, &player_bitmap);
    } else if (player_frame == 42) {
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &dodge_3);
        hagl_blit(display, 12, 40, &player_bitmap);
    } else if (player_frame == 43) {
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &idle_1);
        hagl_blit(display, 12, 40, &player_bitmap);
    }  else if (player_frame == 60) {
        player_status = 0;
        player_frame = 0;
        if(!game_over) player_busy = false;
    }
}

void player_animate_attack(hagl_backend_t *display, bool* enemy_invincible, uint8_t* enemy_frame, uint8_t* enemy_status, int16_t* enemy_health, int16_t* enemy_posture) {
    if (player_frame == 0){
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_3);
    } else if (player_frame == 2) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_2);
    } else if (player_frame == 5) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_1);
    } else if (player_frame == 15) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_2);
    } else if (player_frame == 17) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_3);
    } else if (player_frame == 20) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_4);
    } else if (player_frame == 22) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_5);
        // deal dmg
        if(!*enemy_invincible) { 
            enemy_take_damage(display, 10);
        } else player_take_posture(display, 10);
    } else if (player_frame == 32) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_4);
    } else if (player_frame == 35) {
        hagl_bitmap_init(&player_bitmap, ATTACK_WIDTH, ATTACK_HEIGHT, sizeof(hagl_color_t), &attack_3);
    } else if (player_frame == 40) {
        player_status = 0;
        player_frame = 0;
        if(!game_over) player_busy = false;
    }
    // draw player
    hagl_blit(display, 12, 40, &player_bitmap);
}

void player_animate_jump(hagl_backend_t *display) {
    if (player_frame == 0){
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &idle_1);
    } else if (player_frame == 2) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_2);
    } else if (player_frame == 5) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_1);
    } else if (player_frame == 7) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_2);
    } else if (player_frame == 10) {
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &idle_1);
    } else if (player_frame == 12) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_4);
    } else if (player_frame == 15) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_5);
    } else if (player_frame == 17) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_6);
    } else if (player_frame == 20) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_7);
    } else if (player_frame == 22) {
        player_jump = true;
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_8);
    } else if (player_frame == 25) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_9);
    } else if (player_frame == 27) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_10);
    } else if (player_frame == 47) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_9);
    } else if (player_frame == 50) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_8);
    } else if (player_frame == 52) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_7);
    } else if (player_frame == 55) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_6);
    } else if (player_frame == 57) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_5);
        player_jump = false;
    } else if (player_frame == 60) {
        hagl_bitmap_init(&player_bitmap, JUMP_WIDTH, JUMP_HEIGHT, sizeof(hagl_color_t), &jump_4);
    } else if (player_frame == 62) {
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &idle_1);
    } else if (player_frame == 72) {
        player_status = 0;
        player_frame = 0;
        if(!game_over) player_busy = false;
    }
    // draw player
    if(player_frame < 20 || player_frame >= 55) {
        hagl_fill_rectangle_xywh(display, 12, 37, 48, 3, lightblue);
        hagl_blit(display, 12, 40, &player_bitmap);
    } else if(player_frame == 20 || player_frame == 52) {
        hagl_fill_rectangle_xywh(display, 12, 35, 48, 2, lightblue);
        hagl_blit(display, 12, 37, &player_bitmap);
    } else if(player_frame == 22 || player_frame == 50) {
        hagl_fill_rectangle_xywh(display, 12, 33, 48, 2, lightblue);
        hagl_blit(display, 12, 35, &player_bitmap);
    } else if(player_frame == 25 || player_frame == 47) {
        hagl_fill_rectangle_xywh(display, 12, 31, 48, 2, lightblue);
        hagl_blit(display, 12, 33, &player_bitmap);
    } else if(player_frame == 27) {
        hagl_fill_rectangle_xywh(display, 12, 32, 48, 2, lightblue);
        hagl_blit(display, 12, 31, &player_bitmap);
    }
}

void player_animate_mikiri(hagl_backend_t *display) {
    if (player_frame == 0){
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &idle_1);
    } else if (player_frame == 2) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &mikiri_1);
    } else if (player_frame == 7) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &mikiri_2);
    } else if (player_frame == 12) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &mikiri_3);
    } else if (player_frame == 17) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &mikiri_4);
    } else if (player_frame == 20) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &mikiri_5);
    } else if (player_frame == 22) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &mikiri_6);
    } else if (player_frame == 42) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &mikiri_5);
    } else if (player_frame == 45) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &mikiri_4);
    } else if (player_frame == 47) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &mikiri_3);
    } else if (player_frame == 50) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &mikiri_2);
    } else if (player_frame == 52) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &mikiri_1);
    } else if (player_frame == 55) {
        hagl_bitmap_init(&player_bitmap, MIKIRI_WIDTH, MIKIRI_HEIGHT, sizeof(hagl_color_t), &idle_1);
    }else if (player_frame == 70) {
        player_status = 0;
        player_frame = 0;
        if(!game_over) player_busy = false;
    }
    // draw player
    hagl_blit(display, 12, 40, &player_bitmap);
}

void player_animate_firecracker(hagl_backend_t *display, uint8_t* enemy_frame, uint8_t* enemy_status, int16_t* enemy_posture) {
    if (player_frame == 0 || player_frame == 45){
        hagl_bitmap_init(&player_bitmap, FIRECRACKER_WIDTH, FIRECRACKER_HEIGHT, sizeof(hagl_color_t), &idle_1);
    } else if (player_frame == 2 || player_frame == 12) {
        hagl_bitmap_init(&player_bitmap, FIRECRACKER_WIDTH, FIRECRACKER_HEIGHT, sizeof(hagl_color_t), &firecracker_1);
    } else if (player_frame == 5 || player_frame == 10) {
        hagl_bitmap_init(&player_bitmap, FIRECRACKER_WIDTH, FIRECRACKER_HEIGHT, sizeof(hagl_color_t), &firecracker_2);
    } else if (player_frame == 7) {
        hagl_bitmap_init(&player_bitmap, FIRECRACKER_WIDTH, FIRECRACKER_HEIGHT, sizeof(hagl_color_t), &firecracker_3);
    } else if (player_frame == 10) {
        hagl_bitmap_init(&player_bitmap, FIRECRACKER_WIDTH, FIRECRACKER_HEIGHT, sizeof(hagl_color_t), &firecracker_4);
    } else if (player_frame == 15 || player_frame == 40) {
        hagl_bitmap_init(&player_bitmap, FIRECRACKER_WIDTH, FIRECRACKER_HEIGHT, sizeof(hagl_color_t), &firecracker_5);
    } else if (player_frame == 18 || player_frame == 22 || player_frame == 25 || player_frame == 28 || player_frame == 31) {
        hagl_bitmap_init(&player_bitmap, FIRECRACKER_WIDTH, FIRECRACKER_HEIGHT, sizeof(hagl_color_t), &firecracker_6);
    } else if (player_frame == 20 || player_frame == 23 || player_frame == 26 || player_frame == 29 || player_frame == 33) {
        hagl_bitmap_init(&player_bitmap, FIRECRACKER_WIDTH, FIRECRACKER_HEIGHT, sizeof(hagl_color_t), &firecracker_7);
    } else if (player_frame == 21 || player_frame == 24 || player_frame == 27 || player_frame == 30 || player_frame == 35) {
        hagl_bitmap_init(&player_bitmap, FIRECRACKER_WIDTH, FIRECRACKER_HEIGHT, sizeof(hagl_color_t), &firecracker_8);
    } else if (player_frame == 60) {
        player_status = 0;
        player_frame = 0;
        if(!game_over) player_busy = false;
    }
    if(player_frame == 30) {
        enemy_take_posture(display, 20);
    }
    // draw player
    hagl_blit(display, 12, 40, &player_bitmap);
}

void player_animate_stagger(hagl_backend_t *display) {
    if (player_frame == 0){
        hagl_bitmap_init(&player_bitmap, STAGGER_HEIGHT, STAGGER_WIDTH, sizeof(hagl_color_t), &idle_1);
    } else if (player_frame == 5) {
        hagl_bitmap_init(&player_bitmap, STAGGER_HEIGHT, STAGGER_WIDTH, sizeof(hagl_color_t), &stagger_1);
    } else if (player_frame == 10) {
        hagl_bitmap_init(&player_bitmap, STAGGER_HEIGHT, STAGGER_WIDTH, sizeof(hagl_color_t), &stagger_2);
    } else if (player_frame == 15) {
        hagl_bitmap_init(&player_bitmap, STAGGER_HEIGHT, STAGGER_WIDTH, sizeof(hagl_color_t), &stagger_3);
    } else if (player_frame == 20) {
        hagl_bitmap_init(&player_bitmap, STAGGER_HEIGHT, STAGGER_WIDTH, sizeof(hagl_color_t), &stagger_4);
    } else if (player_frame > 20) {
        player_frame = 20;
    }
    // draw player
    hagl_blit(display, 12, 40, &player_bitmap);
}

void player_animate_death(hagl_backend_t *display) {
    if (player_frame == 0){
        hagl_bitmap_init(&player_bitmap, DEATH_HEIGHT, DEATH_WIDTH, sizeof(hagl_color_t), &idle_1);
        hagl_blit(display, 12, 40, &player_bitmap);
    } else if (player_frame == 5) {
        hagl_bitmap_init(&player_bitmap, DEATH_HEIGHT, DEATH_WIDTH, sizeof(hagl_color_t), &death_1);
        hagl_blit(display, 2, 40, &player_bitmap);
    } else if (player_frame == 10) {
        hagl_bitmap_init(&player_bitmap, DEATH_HEIGHT, DEATH_WIDTH, sizeof(hagl_color_t), &death_2);
        hagl_blit(display, 2, 40, &player_bitmap);
    } else if (player_frame == 15) {
        hagl_bitmap_init(&player_bitmap, DEATH_HEIGHT, DEATH_WIDTH, sizeof(hagl_color_t), &death_3);
        hagl_blit(display, 2, 40, &player_bitmap);
    } else if (player_frame == 20) {
        hagl_bitmap_init(&player_bitmap, DEATH_HEIGHT, DEATH_WIDTH, sizeof(hagl_color_t), &death_4);
        hagl_blit(display, 2, 40, &player_bitmap);
        hagl_fill_rectangle_xywh(display, 5, 5, 60, ICON_HEIGHT, color_lightblue);
        hagl_bitmap_init(icon_bitmap_1, ICON_WIDTH, ICON_HEIGHT, sizeof(hagl_color_t), &icon_death);
        hagl_blit(display, 48, 16, icon_bitmap_1);
    } else if (player_frame > 20) {
        player_frame = 20;
    }
}