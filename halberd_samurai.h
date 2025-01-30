#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <hagl_hal.h>
#include <hagl.h>
#include "bitmap_halberd_samurai.h"

// enemy status
u_int8_t* enemy_status_1;
// enemy frame
u_int8_t* enemy_frame_1;
// enemy invincibility
bool* enemy_invincible_1;
// enemy bitmap
hagl_bitmap_t* enemy_bitmap_1;
// enemy health
int16_t* enemy_health_1;
// enemy posture
int16_t* enemy_posture_1;
// enemy health
int16_t* enemy_max_health_1;
// enemy posture
int16_t* enemy_max_posture_1;

void init_halberd_samurai_bitmap(hagl_backend_t *display, u_int8_t* enemy_status, u_int8_t* enemy_frame, bool* enemy_invincible, hagl_bitmap_t* enemy_bitmap, hagl_bitmap_t* icon_bitmap, int16_t* enemy_health, int16_t* enemy_posture, int16_t* enemy_max_health, int16_t* enemy_max_posture) {
    enemy_status_1 = enemy_status;
    enemy_frame_1 = enemy_frame;
    enemy_invincible_1 = enemy_invincible;
    enemy_bitmap_1 = enemy_bitmap;
    icon_bitmap_1 = icon_bitmap;
    enemy_health_1 = enemy_health;
    enemy_posture_1 = enemy_posture;
    enemy_max_health_1 = enemy_max_health;
    enemy_max_posture_1 = enemy_max_posture;

    *enemy_status_1 = 0;
    *enemy_frame_1 = 0;
    *enemy_invincible_1 = false;
    *enemy_health_1 = 300;
    *enemy_posture_1 = 250;
    *enemy_max_health_1 = 300;
    *enemy_max_posture_1 = 250;

    hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void halberd_samurai_animate_idle(hagl_backend_t *display) {
    if (*enemy_frame_1 >= 60){
        *enemy_frame_1 = 0;
    }
    if (*enemy_frame_1 == 0){
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 30) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_2);
    }
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void halberd_samurai_animate_attack(hagl_backend_t *display) {
    if (*enemy_frame_1 == 0){
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 2) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_4);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_3);
    } else if (*enemy_frame_1 == 8) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_2);
    } else if (*enemy_frame_1 == 10) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_1);
    } else if (*enemy_frame_1 == 50) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_2);
    } else if (*enemy_frame_1 == 52) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_3);
    } else if (*enemy_frame_1 == 55) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_4);
    } else if (*enemy_frame_1 == 55) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 57) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_6);
    } else if (*enemy_frame_1 == 60) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_7);
        // deal dmg
        if(player_invincible) enemy_take_posture(display, 10);
        else if(player_block) {
            enemy_take_posture(display, 5);
            player_take_posture(display, 5);
        }
        else player_take_damage(display, 10);
    } else if (*enemy_frame_1 == 65) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_8);
    } else if (*enemy_frame_1 == 90) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_7);
    } else if (*enemy_frame_1 == 92) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_6);
    } else if (*enemy_frame_1 == 95) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 135) {
        *enemy_status_1 = 0;
        *enemy_frame_1 = 0;
    }
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void halberd_samurai_animate_thrust(hagl_backend_t *display) {
    if (*enemy_frame_1 == 1){
        hagl_bitmap_init(icon_bitmap_1, ICON_WIDTH, ICON_HEIGHT, sizeof(hagl_color_t), &icon_danger);
        hagl_blit(display, 80, 16, icon_bitmap_1);
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 2) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_6);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_7);
    } else if (*enemy_frame_1 == 7) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_thrust_1);
    } else if (*enemy_frame_1 == 10) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_thrust_2);
    } else if (*enemy_frame_1 == 12) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_thrust_3);
    } else if (*enemy_frame_1 == 15) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_thrust_4);
    } else if (*enemy_frame_1 == 17) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_thrust_5);
    } else if (*enemy_frame_1 == 20) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_thrust_6);
    } else if (*enemy_frame_1 == 60) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_thrust_5);
    } else if (*enemy_frame_1 == 62) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_thrust_4);
    } else if (*enemy_frame_1 == 65) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_thrust_3);
    } else if (*enemy_frame_1 == 67) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_thrust_2);
    } else if (*enemy_frame_1 == 70) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_thrust_1);
    } else if (*enemy_frame_1 == 72) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_7);
        // deal dmg
        if(player_status == 6 && player_frame < 22) {
            *enemy_status_1 = 6;
            *enemy_frame_1 = 0;
            enemy_take_posture(display, 25);
        }
        else player_take_damage(display, 25);
        hagl_fill_rectangle_xywh(display, 80, 16, ICON_WIDTH, ICON_HEIGHT, color_lightblue);
    } else if (*enemy_frame_1 == 92) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_6);
    } else if (*enemy_frame_1 == 95) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 125) {
        *enemy_status_1 = 0;
        *enemy_frame_1 = 0;
    }
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void halberd_samurai_animate_sweep(hagl_backend_t *display) {
    if (*enemy_frame_1 == 1){
        hagl_bitmap_init(icon_bitmap_1, ICON_WIDTH, ICON_HEIGHT, sizeof(hagl_color_t), &icon_danger);
        hagl_blit(display, 80, 16, icon_bitmap_1);
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 2) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_6);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_0);
    } else if (*enemy_frame_1 == 7) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_1);
    } else if (*enemy_frame_1 == 10) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_2);
    } else if (*enemy_frame_1 == 12) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_3);
    } else if (*enemy_frame_1 == 15) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_4);
    } else if (*enemy_frame_1 == 17) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_5);
    } else if (*enemy_frame_1 == 20) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_6);
    } else if (*enemy_frame_1 == 60) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_5);
    } else if (*enemy_frame_1 == 62) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_4);
    } else if (*enemy_frame_1 == 65) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_3);
    } else if (*enemy_frame_1 == 67) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_2);
    } else if (*enemy_frame_1 == 70) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_1);
    } else if (*enemy_frame_1 == 72) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_0);
        // deal dmg
        if(player_jump) {
            enemy_take_posture(display, 25);
        }
        else player_take_damage(display, 25);
        hagl_fill_rectangle_xywh(display, 80, 16, ICON_WIDTH, ICON_HEIGHT, color_lightblue);
    } else if (*enemy_frame_1 == 92) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_6);
    } else if (*enemy_frame_1 == 95) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 125) {
        *enemy_status_1 = 0;
        *enemy_frame_1 = 0;
    }
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void halberd_samurai_animate_heavy(hagl_backend_t *display) {
    if (*enemy_frame_1 == 1){
        hagl_bitmap_init(icon_bitmap_1, ICON_WIDTH, ICON_HEIGHT, sizeof(hagl_color_t), &icon_danger);
        hagl_blit(display, 80, 16, icon_bitmap_1);
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_4);
    } else if (*enemy_frame_1 == 2) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_parry);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_heavy_7);
    } else if (*enemy_frame_1 == 7) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_heavy_8);
    } else if (*enemy_frame_1 == 47) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_heavy_7);
    } else if (*enemy_frame_1 == 50) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_heavy_6);
    } else if (*enemy_frame_1 == 52) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_heavy_5);
    } else if (*enemy_frame_1 == 55) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_heavy_4);
    } else if (*enemy_frame_1 == 57) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_heavy_3);
    } else if (*enemy_frame_1 == 60) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_heavy_2);
    } else if (*enemy_frame_1 == 62) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_heavy_1);
        // deal dmg
        if(player_status == 5 && player_frame >= 5 && player_frame <= 36) {
            enemy_take_posture(display, 10);
        }
        else player_take_damage(display, 25);
        hagl_fill_rectangle_xywh(display, 80, 16, ICON_WIDTH, ICON_HEIGHT, color_lightblue);
    } else if (*enemy_frame_1 == 112) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_sweep_0);
    } else if (*enemy_frame_1 == 115) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_6);
    } else if (*enemy_frame_1 == 120) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 160) {
        *enemy_status_1 = 0;
        *enemy_frame_1 = 0;
    }
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void halberd_samurai_animate_parry(hagl_backend_t *display) {
    if (*enemy_frame_1 == 0){
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 2) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_4);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_parry);
        *enemy_invincible_1 = true;
    } else if (*enemy_frame_1 == 45) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_attack_4);
        *enemy_invincible_1 = false;
    } else if (*enemy_frame_1 == 50) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
        *enemy_status_1 = 0;
        *enemy_frame_1 = 0;
    }
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void halberd_samurai_animate_mikiried(hagl_backend_t *display) {
    if (*enemy_frame_1 == 0){
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_mikiried_1);
    } else if (*enemy_frame_1 == 10) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_mikiried_2);
    } else if (*enemy_frame_1 == 15) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_mikiried_3);
    } else if (*enemy_frame_1 == 20) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_mikiried_4);
    } else if (*enemy_frame_1 == 60) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_mikiried_3);
    } else if (*enemy_frame_1 == 65) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_mikiried_2);
    } else if (*enemy_frame_1 == 70) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_mikiried_1);
    } else if (*enemy_frame_1 == 75) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
        *enemy_status_1 = 0;
        *enemy_frame_1 = 0;
    } 
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void halberd_samurai_animate_stagger(hagl_backend_t *display) {
    if (*enemy_frame_1 == 0){
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_idle_1);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_stagger_1);
    } else if (*enemy_frame_1 == 10) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_stagger_2);
    } else if (*enemy_frame_1 == 15) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_stagger_3);
    } else if (*enemy_frame_1 == 20) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_stagger_4);
    } else if (*enemy_frame_1 > 20) {
        *enemy_frame_1 = 22;
    }
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void halberd_samurai_animate_death(hagl_backend_t *display) {
    if (*enemy_frame_1 == 0){
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_stagger_4);
        hagl_blit(display, 60, 40, enemy_bitmap_1);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_death_1);
        hagl_blit(display, 60, 40, enemy_bitmap_1);
    } else if (*enemy_frame_1 == 10) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_death_2);
        hagl_blit(display, 60, 40, enemy_bitmap_1);
    } else if (*enemy_frame_1 == 15) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_death_3);
        hagl_blit(display, 60, 40, enemy_bitmap_1);
    } else if (*enemy_frame_1 == 20) {
        hagl_bitmap_init(enemy_bitmap_1, HALBERD_SAMURAI_WIDTH, HALBERD_SAMURAI_HEIGHT, sizeof(hagl_color_t), &halberdsamurai_death_4);
        hagl_blit(display, 60, 40, enemy_bitmap_1);
    } else if (*enemy_frame_1 == 21) {
        *enemy_frame_1 = 20;
        hagl_fill_rectangle_xywh(display, 5, 5, 60, ICON_HEIGHT, color_lightblue);
        hagl_bitmap_init(icon_bitmap_1, ICON_WIDTH, ICON_HEIGHT, sizeof(hagl_color_t), &icon_shinobi);
        hagl_blit(display, 48, 5, icon_bitmap_1);
        hagl_bitmap_init(icon_bitmap_1, ICON_WIDTH, ICON_HEIGHT, sizeof(hagl_color_t), &icon_execution);
        hagl_blit(display, 48, 29, icon_bitmap_1);
    } else if (*enemy_frame_1 > 21) {
        *enemy_frame_1 = 1;
    }
}

void halberd_samurai_animate(hagl_backend_t *display) {
    if(*enemy_status_1 == 0) {
        halberd_samurai_animate_idle(display);
    } else if (*enemy_status_1 == 1) {
        halberd_samurai_animate_parry(display);
    } else if (*enemy_status_1 == 2) {
        halberd_samurai_animate_attack(display);
    } else if (*enemy_status_1 == 3) {
        halberd_samurai_animate_thrust(display);
    } else if (*enemy_status_1 == 4) {
        halberd_samurai_animate_sweep(display);
    } else if (*enemy_status_1 == 5) {
        halberd_samurai_animate_heavy(display);
    } else if (*enemy_status_1 == 6) {
        halberd_samurai_animate_mikiried(display);
    } else if (*enemy_status_1 == 10) {
        halberd_samurai_animate_stagger(display);
    } else if (*enemy_status_1 == 11) {
        halberd_samurai_animate_death(display);
    }
}

void halberd_samurai_action() {
    if(*enemy_status_1 == 0 && player_status != 11) {
        if(player_status == 3) {
            *enemy_status_1 = 1;
            *enemy_frame_1 = 0;
        } else {
            int r = rand() % 60;
            if(r <= 2) {
                *enemy_status_1 = 2;
                *enemy_frame_1 = 0;
            } else if(r == 3) {
                *enemy_status_1 = 4;
                *enemy_frame_1 = 0;
            } else if(r == 4) {
                *enemy_status_1 = 3;
                *enemy_frame_1 = 0;
            } else if(r == 5) {
                *enemy_status_1 = 5;
                *enemy_frame_1 = 0;
            }
        }
    }
}