#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <hagl_hal.h>
#include <hagl.h>
#include "bitmap_katana_ashigaru.h"
#include "math.h"

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

void init_katana_ashigaru_bitmap(hagl_backend_t *display, u_int8_t* enemy_status, u_int8_t* enemy_frame, bool* enemy_invincible, hagl_bitmap_t* enemy_bitmap, int16_t* enemy_health, int16_t* enemy_posture, int16_t* enemy_max_health, int16_t* enemy_max_posture) {
    enemy_status_1 = enemy_status;
    enemy_frame_1 = enemy_frame;
    enemy_invincible_1 = enemy_invincible;
    enemy_bitmap_1 = enemy_bitmap;
    enemy_health_1 = enemy_health;
    enemy_posture_1 = enemy_posture;
    enemy_max_health_1 = enemy_max_health;
    enemy_max_posture_1 = enemy_max_posture;

    *enemy_status_1 = 0;
    *enemy_frame_1 = 0;
    *enemy_invincible_1 = false;
    *enemy_health_1 = 100;
    *enemy_posture_1 = 100;
    *enemy_max_health_1 = 100;
    *enemy_max_posture_1 = 100;

    hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_idle_1);
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void enemy_take_damage(hagl_backend_t *display, uint8_t damage) {
    if(*enemy_posture_1 == 0) *enemy_health_1 = 0;
    else *enemy_health_1 -= damage;
    if(*enemy_health_1 <= 0) {
        *enemy_health_1 = 0;
        *enemy_status_1 = 11;
        *enemy_frame_1 = 0;
        player_busy = true;
        game_over = true;
    }
    int16_t hp = floor(50.0 * *enemy_health_1 / *enemy_max_health_1);
    hagl_fill_rectangle_xywh(display, 64+(hp), 100, 50-(hp), 5, darkgray);
}

void enemy_take_posture(hagl_backend_t *display, uint8_t posture) {
    *enemy_posture_1 -= posture;
    if(*enemy_posture_1 <= 0) {
        *enemy_posture_1 = 0;
        *enemy_status_1 = 10;
        *enemy_frame_1 = 0;  
    }
    int16_t po = floor(50.0 * *enemy_posture_1 / *enemy_max_posture_1);
    hagl_fill_rectangle_xywh(display, 64+(po), 110, 50-(po), 5, darkgray);
}

void katana_ashigaru_animate_idle(hagl_backend_t *display) {
    if (*enemy_frame_1 >= 60){
        *enemy_frame_1 = 0;
    }
    if (*enemy_frame_1 == 0){
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_idle_1);
    } else if (*enemy_frame_1 == 30) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_idle_2);
    }
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void katana_ashigaru_animate_attack(hagl_backend_t *display) {
    if (*enemy_frame_1 == 0){
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_idle_1);
    } else if (*enemy_frame_1 == 2) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_attack_2);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_attack_1);
    } else if (*enemy_frame_1 == 45) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_attack_2);
    } else if (*enemy_frame_1 == 47) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_idle_1);
    } else if (*enemy_frame_1 == 50) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_attack_4);
    } else if (*enemy_frame_1 == 52) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_attack_5);
        // deal dmg
        if(player_invincible) enemy_take_posture(display, 10);
        else if(player_block) {
            enemy_take_posture(display, 5);
            player_take_posture(display, 5);
        }
        else player_take_damage(display, 10);
    } else if (*enemy_frame_1 == 72) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_attack_4);
    } else if (*enemy_frame_1 == 75) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_idle_1);
    } else if (*enemy_frame_1 == 125) {
        *enemy_status_1 = 0;
        *enemy_frame_1 = 0;
    }
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void katana_ashigaru_animate_parry(hagl_backend_t *display) {
    if (*enemy_frame_1 == 0){
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_idle_1);
    } else if (*enemy_frame_1 == 2) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_attack_2);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_parry);
        *enemy_invincible_1 = true;
    } else if (*enemy_frame_1 == 45) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_attack_2);
        *enemy_invincible_1 = false;
    } else if (*enemy_frame_1 == 47) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_idle_1);
    } else if (*enemy_frame_1 == 57) {
        *enemy_status_1 = 0;
        *enemy_frame_1 = 0;
    }
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void katana_ashigaru_animate_stagger(hagl_backend_t *display) {
    if (*enemy_frame_1 == 0){
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_idle_1);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_stagger_1);
    } else if (*enemy_frame_1 == 10) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_stagger_2);
    } else if (*enemy_frame_1 == 15) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_stagger_3);
    } else if (*enemy_frame_1 == 20) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_WIDTH, KATANA_ASHIGARU_HEIGHT, sizeof(hagl_color_t), &katanaashigaru_stagger_4);
    } else if (*enemy_frame_1 > 20) {
        *enemy_frame_1 = 22;
    }
    // draw enemy
    hagl_blit(display, 60, 40, enemy_bitmap_1);
}

void katana_ashigaru_animate_death(hagl_backend_t *display) {
    if (*enemy_frame_1 == 0){
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_HEIGHT, KATANA_ASHIGARU_WIDTH, sizeof(hagl_color_t), &katanaashigaru_stagger_4);
        hagl_blit(display, 60, 40, enemy_bitmap_1);
    } else if (*enemy_frame_1 == 5) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_HEIGHT, KATANA_ASHIGARU_WIDTH, sizeof(hagl_color_t), &katanaashigaru_death_1);
        hagl_blit(display, 60, 40, enemy_bitmap_1);
    } else if (*enemy_frame_1 == 10) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_HEIGHT, KATANA_ASHIGARU_WIDTH, sizeof(hagl_color_t), &katanaashigaru_death_2);
        hagl_blit(display, 60, 40, enemy_bitmap_1);
    } else if (*enemy_frame_1 == 15) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_HEIGHT, KATANA_ASHIGARU_WIDTH, sizeof(hagl_color_t), &katanaashigaru_death_3);
        hagl_blit(display, 60, 40, enemy_bitmap_1);
    } else if (*enemy_frame_1 == 20) {
        hagl_bitmap_init(enemy_bitmap_1, KATANA_ASHIGARU_HEIGHT, KATANA_ASHIGARU_WIDTH, sizeof(hagl_color_t), &katanaashigaru_death_4);
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

void katana_ashigaru_animate(hagl_backend_t *display) {
    if(*enemy_status_1 == 0) {
        katana_ashigaru_animate_idle(display);
    } else if (*enemy_status_1 == 1) {
        katana_ashigaru_animate_parry(display);
    } else if (*enemy_status_1 == 2) {
        katana_ashigaru_animate_attack(display);
    } else if (*enemy_status_1 == 10) {
        katana_ashigaru_animate_stagger(display);
    } else if (*enemy_status_1 == 11) {
        katana_ashigaru_animate_death(display);
    }
}

void katana_ashigaru_action() {
    if(*enemy_status_1 == 0 && player_status != 11) {
        if(player_status == 3) {
            *enemy_status_1 = 1;
            *enemy_frame_1 = 0;
        } else {
            int r = rand() % 60;
            if(r == 0) {
                *enemy_status_1 = 2;
                *enemy_frame_1 = 0;
            }
        }
    }
}