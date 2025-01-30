#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <sys/time.h>

// Display libraries
#include <hagl_hal.h>
#include <hagl.h>
#include <fps.h>
#include <font6x9.h>

// Buttons
#include "buttons.h"

// Colors
#include "colors.h"

// Characters
#include "player.h"
#include "yari_ashigaru.h"
#include "katana_ashigaru.h"
#include "ono_ashigaru.h"
#include "naginata_samurai.h"
#include "halberd_samurai.h"

// Bitmaps
#include "bitmap_icons.h"
#include "bitmap_menu.h"
#include "bitmap_enemy_icons.h"

// fps counter
volatile bool fps_flag = false;
static fps_instance_t fps;
static const uint64_t US_PER_FRAME_60_FPS = 1000000 / 60;
static const uint64_t US_PER_FRAME_30_FPS = 1000000 / 30;

// menu bitmap
hagl_bitmap_t icon_menu;

// message
wchar_t message[20];

// display
static hagl_backend_t *display;

// enemy status
u_int8_t enemy_status = 0;
// enemy frame
u_int8_t enemy_frame = 0;
// enemy invincibility
bool enemy_invincible = false;
// enemy bitmap
hagl_bitmap_t enemy_bitmap;
// enemy health
int16_t enemy_health = 100;
// enemy posture
int16_t enemy_posture = 50;
// enemy health
int16_t enemy_max_health = 100;
// enemy posture
int16_t enemy_max_posture = 50;

bool show_timer_callback(struct repeating_timer *t)
{
    fps_flag = true;
    return true;
}

void static inline show_fps() {
    hagl_color_t green = hagl_color(display, 0, 255, 0);

    fps_flag = 0;

    /* Print the message on lower left corner. */
    swprintf(message, sizeof(message), L"%.*f FPS  ", 0, fps.current);
    //swprintf(message, sizeof(message), L"%d hp   ", (enemy_health/2));
    hagl_put_text(display, message, 80, 0, green, font6x9);
}

void init_menu() {
    // background
    hagl_fill_rectangle_xywh(display, 0, 0, 120, 120, color_black);
    // logo
    icon_menu.buffer = (uint8_t *) malloc(LOGO_WIDTH * LOGO_HEIGHT * sizeof(hagl_color_t));
    hagl_bitmap_init(&icon_menu, LOGO_WIDTH, LOGO_HEIGHT, sizeof(hagl_color_t), &logo);
    hagl_blit(display, 0, 0, &icon_menu);
    free(*icon_menu.buffer);
    // chosen option
    hagl_fill_ellipse(display, 60, 79, 35, 8, color_darkorange);
    // menu points
    swprintf(message, sizeof(message), L"PLAY");
    hagl_put_text(display, message, 47, 75, color_white, font6x9);
    swprintf(message, sizeof(message), L"CONTROLS");
    hagl_put_text(display, message, 34, 95, color_white, font6x9);
    hagl_flush(display);
}

void init_enemy_picker() {
    // background
    hagl_fill_rectangle_xywh(display, 0, 0, 120, 120, color_black);
    // enemy
    hagl_bitmap_init(&player_bitmap, ENEMY_ICON_WIDTH, ENEMY_ICON_HEIGHT, sizeof(hagl_color_t), &enemy_icon_1);
    hagl_blit(display, 36, 36, &player_bitmap);
    // text
    swprintf(message, sizeof(message), L"<");
    hagl_put_text(display, message, 20, 55, color_white, font6x9);
    swprintf(message, sizeof(message), L">");
    hagl_put_text(display, message, 94, 55, color_white, font6x9);
    swprintf(message, sizeof(message), L"KATANA ASHIGARU");
    hagl_put_text(display, message, 15, 20, color_white, font6x9);
    swprintf(message, sizeof(message), L"*");
    hagl_put_text(display, message, 57, 91, color_yellow, font6x9);
    hagl_flush(display);
}

void init_background() {
    // sky
    hagl_fill_rectangle_xywh(display, 0, 0, 120, 120, color_lightblue);
    // grass
    hagl_fill_rectangle_xywh(display, 0, 75, 120, 45, color_green);
    // set player healthbar and posturebar
    hagl_draw_rectangle_xywh(display, 5, 99, 52, 7, color_black);
    hagl_fill_rectangle_xywh(display, 6, 100, 50, 5, color_red);
    hagl_draw_rectangle_xywh(display, 5, 109, 52, 7, color_black);
    hagl_fill_rectangle_xywh(display, 6, 110, 50, 5, color_yellow);
    // set enemy healthbar and posturebar
    hagl_draw_rectangle_xywh(display, 63, 99, 52, 7, color_black);
    hagl_fill_rectangle_xywh(display, 64, 100, 50, 5, color_red);
    hagl_draw_rectangle_xywh(display, 63, 109, 52, 7, color_black);
    hagl_fill_rectangle_xywh(display, 64, 110, 50, 5, color_yellow);
}

void init_controls() {
    // background
    hagl_fill_rectangle_xywh(display, 0, 0, 120, 120, color_black);
    // text
    swprintf(message, sizeof(message), L"UP - JUMP");
    hagl_put_text(display, message, 10, 10, color_white, font6x9);
    swprintf(message, sizeof(message), L"LEFT - DODGE");
    hagl_put_text(display, message, 10, 20, color_white, font6x9);
    swprintf(message, sizeof(message), L"RIGHT - ATTACK");
    hagl_put_text(display, message, 10, 30, color_white, font6x9);
    swprintf(message, sizeof(message), L"DOWN - MIKIRI");
    hagl_put_text(display, message, 10, 40, color_white, font6x9);
    swprintf(message, sizeof(message), L"A - BLOCK");
    hagl_put_text(display, message, 10, 50, color_white, font6x9);
    swprintf(message, sizeof(message), L"B - PARRY");
    hagl_put_text(display, message, 10, 60, color_white, font6x9);
    swprintf(message, sizeof(message), L"X - HEAL");
    hagl_put_text(display, message, 10, 70, color_white, font6x9);
    swprintf(message, sizeof(message), L"Y - FIRECRACKER");
    hagl_put_text(display, message, 10, 80, color_white, font6x9);
    swprintf(message, sizeof(message), L"<- Y");
    hagl_put_text(display, message, 10, 101, color_white, font6x9);
    hagl_flush(display);
}

// ####################################################### MAIN ##############################################################
int main() {
    // Timer for the FPS counter
    struct repeating_timer show_timer;
    // Init I/O
    stdio_init_all();
    // Init FPS
    fps_init(&fps);
    // Init display
    display = hagl_init();
    // Clear the display
    hagl_clear(display);
    /* Update displayed FPS counter every 250 ms. */
    add_repeating_timer_ms(250, show_timer_callback, NULL, &show_timer);
    // Init buttons
    buttons_init();
    // Init player and enemy bitmap
    enemy_bitmap.buffer = (uint8_t *) malloc(NAGINATA_SAMURAI_WIDTH * IDLE_HEIGHT * sizeof(hagl_color_t));
    player_bitmap.buffer = (uint8_t *) malloc(IDLE_WIDTH * IDLE_HEIGHT * sizeof(hagl_color_t));

    while(1) {
        // ################################################# Main Menu ##############################################################
        // Init main menu
        init_menu();
        uint8_t menu_pos = 0;
        bool controls = false;
        while (true) {
            if(!gpio_get(KEY_A)) {
                if(menu_pos == 0) {
                    break;
                } else if(menu_pos == 1) {
                    init_controls();
                    controls = true;
                }
            } else if(!gpio_get(JOY_UP) && menu_pos == 1 && !controls) {
                menu_pos = 0;
                // chosen option
                hagl_fill_rectangle_xywh(display, 0, 70, 120, 50, color_black);
                hagl_fill_ellipse(display, 60, 79, 35, 8, color_darkorange);
                // text
                swprintf(message, sizeof(message), L"PLAY");
                hagl_put_text(display, message, 47, 75, color_white, font6x9);
                swprintf(message, sizeof(message), L"CONTROLS");
                hagl_put_text(display, message, 34, 95, color_white, font6x9);
                hagl_flush(display);
            } else if(!gpio_get(JOY_DOWN) && menu_pos == 0 && !controls) {
                menu_pos = 1;
                // chosen option
                hagl_fill_rectangle_xywh(display, 0, 70, 120, 50, color_black);
                hagl_fill_ellipse(display, 60, 99, 35, 8, color_darkorange);
                // text
                swprintf(message, sizeof(message), L"PLAY");
                hagl_put_text(display, message, 47, 75, color_white, font6x9);
                swprintf(message, sizeof(message), L"CONTROLS");
                hagl_put_text(display, message, 34, 95, color_white, font6x9);
                hagl_flush(display);
            } else if(!gpio_get(KEY_Y) && controls) {
                controls = false;
                menu_pos = 0;
                init_menu();
            }
        }

        // ################################################# Enemy picker ##############################################################
        // Init main menu
        init_enemy_picker();
        busy_wait_until(time_us_64() + 500000);
        uint8_t enemy_selected_pos = 0;
        while (true) {
            uint64_t start = time_us_64();
            if(!gpio_get(JOY_RIGHT) && enemy_selected_pos < 4) {
                enemy_selected_pos++;
            } else if(!gpio_get(JOY_LEFT) && enemy_selected_pos > 0) {
                enemy_selected_pos--;
            } else if(!gpio_get(KEY_A)) {
                break;
            } else {
                continue;
            }
            hagl_fill_rectangle_xywh(display, 0, 20, 120, 9, color_black);
            hagl_fill_rectangle_xywh(display, 0, 91, 120, 9, color_black);
            if(enemy_selected_pos == 0) {
                hagl_bitmap_init(&player_bitmap, ENEMY_ICON_WIDTH, ENEMY_ICON_HEIGHT, sizeof(hagl_color_t), &enemy_icon_1);
                hagl_blit(display, 36, 36, &player_bitmap);
                swprintf(message, sizeof(message), L"KATANA ASHIGARU");
                hagl_put_text(display, message, 15, 20, color_white, font6x9);
                swprintf(message, sizeof(message), L"*");
                hagl_put_text(display, message, 57, 91, color_yellow, font6x9);
            } else if(enemy_selected_pos == 1) {
                hagl_bitmap_init(&player_bitmap, ENEMY_ICON_WIDTH, ENEMY_ICON_HEIGHT, sizeof(hagl_color_t), &enemy_icon_2);
                hagl_blit(display, 36, 36, &player_bitmap);
                swprintf(message, sizeof(message), L"YARI ASHIGARU");
                hagl_put_text(display, message, 21, 20, color_white, font6x9);
                swprintf(message, sizeof(message), L"* *");
                hagl_put_text(display, message, 51, 91, color_yellow, font6x9);
            } else if(enemy_selected_pos == 2) {
                hagl_bitmap_init(&player_bitmap, ENEMY_ICON_WIDTH, ENEMY_ICON_HEIGHT, sizeof(hagl_color_t), &enemy_icon_3);
                hagl_blit(display, 36, 36, &player_bitmap);
                swprintf(message, sizeof(message), L"AXE ASHIGARU");
                hagl_put_text(display, message, 24, 20, color_white, font6x9);
                swprintf(message, sizeof(message), L"* * *");
                hagl_put_text(display, message, 45, 91, color_yellow, font6x9);
            } else if(enemy_selected_pos == 3) {
                hagl_bitmap_init(&player_bitmap, ENEMY_ICON_WIDTH, ENEMY_ICON_HEIGHT, sizeof(hagl_color_t), &enemy_icon_4);
                hagl_blit(display, 36, 36, &player_bitmap);
                swprintf(message, sizeof(message), L"NAGINATA SAMURAI");
                hagl_put_text(display, message, 12, 20, color_white, font6x9);
                swprintf(message, sizeof(message), L"* * * *");
                hagl_put_text(display, message, 39, 91, color_yellow, font6x9);
            } else if(enemy_selected_pos == 4) {
                hagl_bitmap_init(&player_bitmap, ENEMY_ICON_WIDTH, ENEMY_ICON_HEIGHT, sizeof(hagl_color_t), &enemy_icon_5);
                hagl_blit(display, 36, 36, &player_bitmap);
                swprintf(message, sizeof(message), L"HALBERD SAMURAI");
                hagl_put_text(display, message, 15, 20, color_white, font6x9);
                swprintf(message, sizeof(message), L"* * * * *");
                hagl_put_text(display, message, 33, 91, color_yellow, font6x9);
            }
            hagl_flush(display);
            busy_wait_until(start + 500000);
        }

        // ################################################# Game logic ##############################################################
        // Init background
        init_background();
        // icon bitmap
        hagl_bitmap_t icon_bitmap;
        icon_bitmap.buffer = (uint8_t *) malloc(ICON_WIDTH * ICON_HEIGHT * sizeof(hagl_color_t));
        // heal
        hagl_bitmap_init(&icon_bitmap, ICON_WIDTH, ICON_HEIGHT, sizeof(hagl_color_t), &icon_heal);
        hagl_blit(display, 5, 5, &icon_bitmap);
        swprintf(message, sizeof(message), L"%.*d", 0, 1);
        hagl_put_text(display, message, 23, 20, color_lightgreen, font6x9);
        // firecracker
        hagl_bitmap_init(&icon_bitmap, ICON_WIDTH, ICON_HEIGHT, sizeof(hagl_color_t), &icon_firecracker);
        hagl_blit(display, 32, 5, &icon_bitmap);
        swprintf(message, sizeof(message), L"%.*d", 0, 1);
        hagl_put_text(display, message, 50, 20, color_lightgreen, font6x9);
        // player
        init_player_bitmap(display, &icon_bitmap);
        hagl_bitmap_init(&player_bitmap, IDLE_WIDTH, IDLE_HEIGHT, sizeof(hagl_color_t), &idle_1);
        hagl_blit(display, 12, 40, &player_bitmap);
        // enemy
        if(enemy_selected_pos == 0) {
            init_katana_ashigaru_bitmap(display, &enemy_status, &enemy_frame, &enemy_invincible, &enemy_bitmap, &enemy_health, &enemy_posture, &enemy_max_health, &enemy_max_posture);
        } else if (enemy_selected_pos == 1) {
            init_yari_ashigaru_bitmap(display, &enemy_status, &enemy_frame, &enemy_invincible, &enemy_bitmap, &icon_bitmap, &enemy_health, &enemy_posture, &enemy_max_health, &enemy_max_posture);
        } else if (enemy_selected_pos == 2) {
            init_ono_ashigaru_bitmap(display, &enemy_status, &enemy_frame, &enemy_invincible, &enemy_bitmap, &icon_bitmap, &enemy_health, &enemy_posture, &enemy_max_health, &enemy_max_posture);
        } else if (enemy_selected_pos == 3) {
           init_naginata_samurai_bitmap(display, &enemy_status, &enemy_frame, &enemy_invincible, &enemy_bitmap, &icon_bitmap, &enemy_health, &enemy_posture, &enemy_max_health, &enemy_max_posture);
        } else if (enemy_selected_pos == 4) {
           init_halberd_samurai_bitmap(display, &enemy_status, &enemy_frame, &enemy_invincible, &enemy_bitmap, &icon_bitmap, &enemy_health, &enemy_posture, &enemy_max_health, &enemy_max_posture);
        }
        uint64_t start = time_us_64();
        hagl_flush(display);
        busy_wait_until(start + 1000000);

        while (true) {
            // Start timer
            uint64_t start = time_us_64();
            // Set frame
            player_frame++;
            enemy_frame++;

            // get pressed key(s)
            if(!player_busy) {
                if(!gpio_get(KEY_A)) {
                    // block
                    player_status = 1;
                    player_frame = 0;
                    player_busy = true;
                    player_block = true;
                } else if(!gpio_get(KEY_B)) {
                    // parry
                    player_status = 2;
                    player_frame = 0;
                    player_busy = true;
                } else if(!gpio_get(KEY_X)) {
                    // heal
                    player_drink_heal(display);
                } else if(!gpio_get(KEY_Y)) {
                    // firecracker
                    if(player_firecracker > 0) {
                        player_status = 7;
                        player_frame = 0;
                        player_busy = true;
                        player_firecracker--;
                        // decrease number
                        swprintf(number, sizeof(number), L"%u", player_firecracker);
                        if(player_firecracker == 0) {
                            hagl_put_text(display, number, 50, 20, color_red, font6x9);
                        } else {
                            hagl_put_text(display, number, 50, 20, color_lightgreen, font6x9);
                        }
                        hagl_fill_rectangle_xywh(display, 6, 100, player_health/2, 5, red);
                    }
                } else if(!gpio_get(JOY_UP)) {
                    // jump
                    player_status = 4;
                    player_frame = 0;
                    player_busy = true;
                } else if(!gpio_get(JOY_DOWN)) {
                    // mikiri
                    player_status = 6;
                    player_frame = 0;
                    player_busy = true;
                } else if(!gpio_get(JOY_LEFT)) {
                    // dodge
                    player_status = 5;
                    player_frame = 0;
                    player_busy = true;
                } else if(!gpio_get(JOY_RIGHT)) {
                    // attack
                    player_status = 3;
                    player_frame = 0;
                    player_busy = true;
                } else if(!gpio_get(JOY_CENTRE)) {
                    // TODO
                }
            } else if(player_block) {
                if(gpio_get(KEY_A)) {
                    player_frame = 20;
                    player_block = false;
                }
            } else if(game_over) {
                if(!gpio_get(KEY_A)) {
                    game_over = false;
                    break;
                }
            }

            // get player status
            if(player_status == 0) {
                player_animate_idle(display);
            } else if (player_status == 1) {
                player_animate_block(display);
            } else if (player_status == 2) {
                player_animate_parry(display);
            } else if (player_status == 3) {
                player_animate_attack(display, &enemy_invincible, &enemy_frame, &enemy_status, &enemy_health, &enemy_posture);
            } else if (player_status == 4) {
                player_animate_jump(display);
            } else if (player_status == 5) {
                player_animate_dodge(display);
            } else if (player_status == 6) {
                player_animate_mikiri(display);
            } else if (player_status == 7) {
                player_animate_firecracker(display, &enemy_frame, &enemy_status, &enemy_posture);
            } else if (player_status == 8) {
                player_animate_heal(display);
            } else if (player_status == 10) {
                player_animate_stagger(display);
            } else if (player_status == 11) {
                player_animate_death(display);
            }

            // get enemy action
            if(enemy_selected_pos == 0) {
                katana_ashigaru_action();
            } else if (enemy_selected_pos == 1) {
                yari_ashigaru_action();
            } else if (enemy_selected_pos == 2) {
                ono_ashigaru_action();
            } else if (enemy_selected_pos == 3) {
                naginata_samurai_action();
            } else if (enemy_selected_pos == 4) {
                halberd_samurai_action();
            }

            // get enemy animation
            if(enemy_selected_pos == 0) {
                katana_ashigaru_animate(display);
            } else if (enemy_selected_pos == 1) {
                yari_ashigaru_animate(display);
            } else if (enemy_selected_pos == 2) {
                ono_ashigaru_animate(display);
            } else if (enemy_selected_pos == 3) {
                naginata_samurai_animate(display);
            } else if (enemy_selected_pos == 4) {
                halberd_samurai_animate(display);
            }
            
            // ####################################################################################################

            /* Update the displayed fps if requested
            if (fps_flag) {
                show_fps();
            }*/

            // Flush back buffer contents to display
            hagl_flush(display);

            // Update FPS
            fps_update(&fps);

            // Cap to 60 fps
            busy_wait_until(start + US_PER_FRAME_60_FPS);
        }
        start = time_us_64();
        busy_wait_until(start + 500000);
    }
    return 0;
}