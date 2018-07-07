#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include <lcd.h>
#include <math.h>
#include <macros.h>
#include <sprite.h>
#include <usb_serial.c>
#include <analog.c>
#include "lcd_model.h"
#define _USE_MATH_DEFINES
#define USB_DELAY				(10)
#define SEED_PRIME				(373)
#define SEED_INCREMENT			(10)
#define TRUE					(1)
#define ITOA_DIGITAL			(10)
#define FALSE					(0)
#define LARGE_BUFF				(120)
#define MED_BUFF				(60)
#define DOUBLE_DIGITS			(10)
#define MILLISECONDS_IN_SECOND	(1000.0)
#define SECONDS_IN_MIN			(60.0)
#define DELAY					(33.0)
#define COUNTDOWN_SECS			(1.0)
#define STICK_CENTRE			(0)
#define STICK_LEFT				(1)
#define STICK_DOWN				(7)
#define STICK_RIGHT				(0)
#define STICK_UP				(1)
#define BUTTON_LEFT				(6)
#define BUTTON_RIGHT			(5)
#define PIN_LED0				(2)
#define PIN_LED1				(3)
#define PORT_POT0				(0)
#define PORT_POT1				(1)
#define ASCII_INIT				(48)
#define PLAYER_WIDTH			(7)
#define PLAYER_HEIGHT			(8)
#define DOOR_WIDTH				(8)
#define DOOR_HEIGHT				(9)
#define CASTLE_WIDTH			(32)
#define CASTLE_HEIGHT			(24)
#define KEY_WIDTH				(8)
#define KEY_HEIGHT				(3)
#define MONSTER_WIDTH			(7)
#define MONSTER_HEIGHT			(6)
#define TREASURE_WIDTH			(7)
#define TREASURE_HEIGHT			(7)
#define SHIELD_WIDTH			(8)
#define SHIELD_HEIGHT			(7)
#define BOMB_WIDTH				(6)
#define BOMB_HEIGHT				(5)
#define BOW_WIDTH				(7)
#define BOW_HEIGHT				(6)
#define ARROW_WIDTH				(2)
#define ARROW_HEIGHT			(2)
#define PLAYER_SPEED			(1)
#define PUSH_HYP				(5)
#define AIM_HYP					(10.0)
#define MONSTER_SPEED			(0.1)
#define MAX_LIVES				(3)
#define MAX_ARROWS				(5)
#define ITEM_PICKUP_DELAY		(30)
#define _COUNTDOWN				(3)
#define ROOM_W					(126)
#define ROOM_H					(72)
#define MAX_TREASURES			(5)
#define MAX_MONSTERS			(5)
#define MAX_WALLS				(6)
#define MAX_TICKS				(20000)
#define ITEM_DIST				(10)
#define MONSTER_DIST			(30)
#define DICEROLL_MAX			(100)
#define ITEM_DROPRATE			(30)
#define WALL_LENGTH				(0.2)
#define EXPLOSION_LENGTH		(10)
#define SCORE_KILL				(10)
#define SCORE_LVL				(100)
#define SCORE_TREASURE			(10)
#define FREQ     (8000000.0)
#define PRESCALE (1024.0)
uint8_t player_image[] = {  0b00111000,
							0b00101000,
							0b00111000,
							0b01111100,
							0b10111010,
							0b10111010,
							0b00101000,
							0b00101000};
uint8_t castle_image[] = {		0b10101010, 0b10101010 , 0b10101010 , 0b10101010,
								0b11111111, 0b11111111 , 0b11111111 , 0b11111111,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000111, 0b11000000 , 0b00000011 , 0b11100011,
								0b11000101, 0b01000000 , 0b00000010 , 0b10100011,
								0b11000111, 0b11000000 , 0b00000011 , 0b11100011,
								0b11000101, 0b01000000 , 0b00000010 , 0b10100011,
								0b11000111, 0b11000000 , 0b00000011 , 0b11100011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b11000000, 0b00000000 , 0b00000000 , 0b00000011,
								0b00111111, 0b11111111 , 0b11111111 , 0b11111100,
								0b00011111, 0b11111111 , 0b11111111 , 0b11111000 };
uint8_t key_image[] = { 0b11100101,
						0b10111111,
						0b11100000};
uint8_t monster_image[] = { 0b01111100,
							0b10111010,
							0b10111010,
							0b11111110,
							0b10111010,
							0b10010010};
uint8_t door_image[] = {	0b11111111,
							0b10000001,
							0b10000001,
							0b10000001,
							0b10000111,
							0b10000101,
							0b10000001,
							0b10000001,
							0b11111111};
uint8_t treasure_image[] = {	0b00010000,
								0b01111110,
								0b10010000,
								0b01111100,
								0b00010010,
								0b11111100,
								0b00010000};
uint8_t shield_image[] = {	0b11111111,
							0b11011011,
							0b11100111,
							0b11000011,
							0b11111111,
							0b01111110,
							0b00111100,
							0b00000000,
							0b00000000 };
uint8_t arrow_image[] = {	0b11000000,
							0b11000000 };
uint8_t bow_image[] = {		0b11100000,
							0b01011000,
							0b01000100,
							0b01000100,
							0b01011000,
							0b11100000};
uint8_t bomb_image[] = {	0b00001000,
							0b00010100,
							0b01111000,
							0b11111100,
							0b01111000};
Sprite player;
float player_x;
float player_y;
Sprite door;
float door_x;
float door_y;
Sprite castle;
int item_pickup;
Sprite shield;
int shield_exists;
float shield_x;
float shield_y;
int shield_following;
Sprite bow;
int bow_exists;
float bow_x;
float bow_y;
int bow_following;
int bow_ammo;
float bow_cooldown;
Sprite bomb;
int bomb_exists;
float bomb_x;
float bomb_y;
int bomb_following;
int bomb_fuse;
int arrow_exists[MAX_ARROWS];
Sprite arrow[MAX_ARROWS];
float arrow_x[MAX_ARROWS];
float arrow_y[MAX_ARROWS];
float arrow_dir[MAX_ARROWS];
Sprite key;
float key_x;
float key_y;
int key_following;
float room_w = ROOM_W;
float room_h = ROOM_H;
float view_x = FALSE;
float view_y = FALSE;
int treasure_exists[MAX_TREASURES];
Sprite treasure[MAX_TREASURES];
float treasure_x[MAX_TREASURES];
float treasure_y[MAX_TREASURES];
int monster_exists[MAX_MONSTERS];
Sprite monster[MAX_MONSTERS];
float monster_x[MAX_MONSTERS];
float monster_y[MAX_MONSTERS];
int wall_exists[MAX_WALLS];
float wall_x1[MAX_WALLS];
float wall_x2[MAX_WALLS];
float wall_y1[MAX_WALLS];
float wall_y2[MAX_WALLS];
char * start_string_1 = "Zaine Ashe";
char * start_string_2 = "n9469010";
char * load_string = "Loading...";
char * gameover_string_1 = "Game over!";
char * score_string = " Score ";
char * lives_string = " Lives ";
char * floor_string = " Floor ";
char * time_string = " Time ";
char * init_string = "Connect USB";
char * msg_welcome = "Welcome to ANSI Tower!";
char * msg_kill = "Killed a monster!";
char * msg_die = "Got Killed!";
char * msg_pickup = "Picked up ";
char * msg_lvlup = "Next floor!";
int initialised;
float start_countdown = _COUNTDOWN + TRUE;
float gameover_countdown = FALSE;
float loading_countdown = FALSE;
int player_dying;
int explosion;
int game_score;
int game_lives;
int game_floor;
float game_time;
float aim_angle;
float seed;

// PORT & BIT HANDLING

void init_ddrs(void) {

	// Timer overflow on.
	TCCR0A = 0;
	TCCR0B = 5;
	TIMSK0 = 1;

	// Enable timer overflow, and turn on interrupts.
	sei();


	CLEAR_BIT(DDRB, STICK_CENTRE);
	CLEAR_BIT(DDRB, STICK_LEFT);
	CLEAR_BIT(DDRB, STICK_DOWN);
	CLEAR_BIT(DDRD, STICK_RIGHT);
	CLEAR_BIT(DDRD, STICK_UP);
	CLEAR_BIT(DDRF, BUTTON_LEFT);
	CLEAR_BIT(DDRF, BUTTON_RIGHT);
	SET_BIT(DDRB, PIN_LED0);
	SET_BIT(DDRB, PIN_LED1);
	CLEAR_BIT(DDRF, PORT_POT1);
	CLEAR_BIT(DDRF, PORT_POT0);
	SET_BIT(DIDR0, 1);
}

// TIMER OVERFLOW HANDLER
volatile int overflow_counter = 0;

ISR(TIMER0_OVF_vect) {
	overflow_counter++;
}


int stick_pressed(void) {
	return BIT_IS_SET(PINB, STICK_CENTRE);
}
int stick_left(void) {
	return BIT_IS_SET(PINB, STICK_LEFT);
}
int stick_right(void) {
	return BIT_IS_SET(PIND, STICK_RIGHT);
}
int stick_up(void) {
	return BIT_IS_SET(PIND, STICK_UP);
}
int stick_down(void) {
	return BIT_IS_SET(PINB, STICK_DOWN);
}
int button_pressed(void) {
	return BIT_IS_SET(PINF, BUTTON_LEFT) || BIT_IS_SET(PINF, BUTTON_RIGHT);
}
float pot1_value(void) {
	return BIT_VALUE(PINF, PORT_POT1);
}
void toggle_leds(void) {
	if (BIT_IS_SET(PORTB, PIN_LED0)) {
		CLEAR_BIT(PORTB, PIN_LED0);
	} else {
		SET_BIT(PORTB, PIN_LED0);
	}
	if (BIT_IS_SET(PORTB, PIN_LED1)) {
		CLEAR_BIT(PORTB, PIN_LED1);
	}
	else {
		SET_BIT(PORTB, PIN_LED1);
	}
}
void clear_leds(void) {
	CLEAR_BIT(PORTB, PIN_LED0);
	CLEAR_BIT(PORTB, PIN_LED1);
}

//START SCREEN DISPLAY
void display_startscreen(void) {
	if (start_countdown > _COUNTDOWN) {
		if (start_countdown > _COUNTDOWN + TRUE) {
			start_countdown -= TRUE;
		} else {
			start_countdown = _COUNTDOWN + TRUE;
			draw_string(LCD_X / 2 - (strlen(start_string_1) / 2) * CHAR_WIDTH, LCD_Y / 4, start_string_1, FG_COLOUR);
			draw_string(LCD_X / 2 - (strlen(start_string_2) / 2) * CHAR_WIDTH, LCD_Y / 2, start_string_2, FG_COLOUR);
			if (button_pressed()) {
				start_countdown = _COUNTDOWN;
			}
		}
	} else {
		start_countdown -= _COUNTDOWN / (MILLISECONDS_IN_SECOND / DELAY);
		draw_char(LCD_X / 2, LCD_Y / 2, ASCII_INIT + ceil(start_countdown), FG_COLOUR);
	}
}

//DRAW ANY KIND OF UI TEXT (WITH PROVIDED OFFSET)
void draw_text(char * string1, float value, float offset) {

	char final_buffer[20];
	char buffer[20];
	strcpy(final_buffer, ": ");
	draw_string(5, offset * 8, string1, FG_COLOUR);
	itoa(value, buffer, ITOA_DIGITAL);
	strcat(final_buffer, buffer);
	draw_string(LCD_X / 2, offset * 8, final_buffer, FG_COLOUR);

}

//DRAW GAME SCORE & GAME FLOOR (UNANIMOUS BETWEEN GAME OVER & STATUS SCREENS)
void score_floor(void) {
	draw_text(score_string, game_score, 1);
	draw_text(floor_string, game_floor, 2);
}

void calculate_timestring(char final_buffer[]) {
	char buffer[20];
	
	int time_secs = fmod(floor(game_time),SECONDS_IN_MIN);
	int time_mins = floor(game_time / SECONDS_IN_MIN);
	strcpy(final_buffer, ": ");
	itoa(time_mins, buffer, ITOA_DIGITAL);
	if (time_mins < DOUBLE_DIGITS) {
		strcat(final_buffer, "0");
	}
	strcat(final_buffer, buffer);
	strcat(final_buffer, ":");
	itoa(time_secs, buffer, ITOA_DIGITAL);
	if (time_secs < DOUBLE_DIGITS) {
		strcat(final_buffer, "0");
	}
	strcat(final_buffer, buffer);
}

//DISPLAY STATUS - (TIME, SCORE, FLOOR, )
void display_status(void) {
	score_floor();
	draw_text(lives_string, game_lives, 3);
	draw_string(5, 32, time_string, FG_COLOUR);
	char time_buff[20];
	calculate_timestring(time_buff);
	draw_string(LCD_X / 2, 32, time_buff, FG_COLOUR);
}
void display_gameover(void) {
	draw_string(5, 0, gameover_string_1, FG_COLOUR);
	score_floor();
}
void display_sprite(sprite_id sprite, float x, float y, uint8_t width, uint8_t height, uint8_t bitmap[]) {
	if (x - width / 2 < view_x + LCD_X && y - height / 2 < view_y + LCD_Y && x + width / 2 > view_x && y + height / 2 > view_y) {
		sprite_init(sprite, x - width / 2 - view_x, y - height / 2 - view_y, width, height, bitmap);
		sprite_draw(sprite);
	}
}
void draw_border(int comparitor, float x1, float y1, float x2, float y2) {
	float top_val;
	float bot_val;
	float left_val;
	float right_val;
	if (comparitor) {
		top_val = y1;
		bot_val = y2;
		left_val = x1;
		right_val = x2;
		if (top_val < 0) {
			top_val = 0;}
		if (bot_val > LCD_Y) {
			bot_val = LCD_Y;}
		if (left_val < 0) {
			left_val = 0;}
		if (right_val > LCD_X) {
			right_val = LCD_X;}
		draw_line(left_val, top_val, right_val, bot_val, FG_COLOUR);
	}
}
void draw_wall(float x1, float y1, float x2, float y2) {
	float top_val;
	float bot_val;
	float left_val;
	float right_val;
	if (x1 + view_x < view_x + LCD_X && y1 + view_y < view_y + LCD_Y && x2 + view_x > view_x && y2 + view_y >view_y) {
		top_val = y1;
		bot_val = y2;
		left_val = x1;
		right_val = x2;
		if (top_val < 0) {
			top_val = 0;}
		if (bot_val > LCD_Y) {
			bot_val = LCD_Y;}
		if (left_val < 0) {
			left_val = 0;}
		if (right_val > LCD_X) {
			right_val = LCD_X;}
		for (float m_val = top_val; m_val < bot_val; m_val += 1) {
			draw_line(left_val, m_val, right_val, m_val, FG_COLOUR);
		}
	}
}

int col_sprite(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2) {

	return (!(x1 + width1 / 2 < x2 - width2 / 2 || x1 - width1 / 2 > x2 + width2 / 2
		|| y1 + height1 / 2 < y2 - height2 / 2 || y1 - height1 / 2 > y2 + height2 / 2)) == 1;

}

int col_wall(float x, float y, float width, float height, int wallID, float padding) {

	return (!(x + width / 2 + padding < wall_x1[wallID] || x - width / 2 - padding > wall_x2[wallID]
		|| y + height / 2 + padding < wall_y1[wallID] || y - height / 2 - padding > wall_y2[wallID]) == 1);

}

void draw_game(void) {
	if (game_floor == 0) {
		display_sprite(&castle, room_w / 2, room_h / 2, CASTLE_WIDTH, CASTLE_HEIGHT, castle_image);
	}
	display_sprite(&door, door_x, door_y, DOOR_WIDTH, DOOR_HEIGHT, door_image);
	display_sprite(&player, player_x, player_y , PLAYER_WIDTH, PLAYER_HEIGHT, player_image);


	if ((bomb_exists == 1 && bomb_following == 1) || (bow_exists == 1 && bow_following == 1)) {
		draw_line(player_x - view_x, player_y - view_y, player_x + cos(aim_angle) * AIM_HYP - view_x, player_y + sin(aim_angle) * AIM_HYP - view_y, FG_COLOUR);
	}


	display_sprite(&key, key_x, key_y, KEY_WIDTH, KEY_HEIGHT, key_image);
	if (bomb_exists == 1) {
		if (bomb_fuse == 0 || bomb_fuse % 4 >2) {
			display_sprite(&bomb, bomb_x, bomb_y, BOMB_WIDTH, BOMB_HEIGHT, bomb_image);
		}
	}
	if (bow_exists == 1) {
		display_sprite(&bow, bow_x, bow_y, BOW_WIDTH, BOW_HEIGHT, bow_image);
	}
	if (shield_exists == 1) {
		display_sprite(&shield, shield_x, shield_y, SHIELD_WIDTH, SHIELD_HEIGHT, shield_image);
	}
	for (int o = 0; o < MAX_ARROWS; o+=1) {
		if (arrow_exists[o] == 1) {
			display_sprite(&arrow[o], arrow_x[o], arrow_y[o], ARROW_WIDTH, ARROW_HEIGHT, arrow_image);
		}
	}
	for (int i = 0; i < MAX_TREASURES; i += 1) {
		if (treasure_exists[i] == 1) {
			display_sprite(&treasure[i], treasure_x[i], treasure_y[i], TREASURE_WIDTH, TREASURE_HEIGHT, treasure_image);
		}
	}
	for (int i = 0; i < MAX_MONSTERS; i += 1) {
		if (monster_exists[i] == 1) {
			display_sprite(&monster[i], monster_x[i], monster_y[i], MONSTER_WIDTH, MONSTER_HEIGHT, monster_image);
		}
	}
	if (view_x < 0) {
		for (int i = view_x - 1; i < 0; i += 1) {
			draw_border((view_x < 0), -i, -view_y, -i, room_h - view_y);
		}
	}
	if (view_x + LCD_X > room_w) {
		for (int i = view_x + LCD_X; i > room_w; i -= 1) {
			draw_border((view_x + LCD_X > room_w), i - view_x, -view_y, i - view_x, room_h - view_y);
		}
	}
	if (view_y < 0) {
		for (int i = view_y; i < 0; i += 1) {
			draw_border((view_y < 0), 1, -i, LCD_X - 1, -i);
		}
	}
	if (view_y + LCD_Y > room_h) {
		for (int i = view_y + LCD_Y; i > room_h; i -= 1) {
			draw_border((view_y + LCD_Y > room_h), 1, i - view_y, LCD_X - 1, i - view_y);
		}
	}
	for (int i = 0; i < MAX_WALLS; i += 1) {
		if (wall_exists[i] == 1 && game_floor >= 1) {
			draw_wall(wall_x1[i] - view_x, wall_y1[i] - view_y, wall_x2[i] - view_x, wall_y2[i] - view_y);
		}
	}
}
void generate_wall(int id, int is_chain) {

	if (is_chain) {
		wall_exists[id] = 1;
		wall_x1[id] = PLAYER_WIDTH * 2 + fmod((float)rand(), room_w - PLAYER_WIDTH * 4 - room_w * 0.35 - 3);
		wall_y1[id] = PLAYER_HEIGHT * 2 + fmod((float)rand(), room_h - PLAYER_HEIGHT * 4 - 3);
		wall_x2[id] = wall_x1[id] + room_w * WALL_LENGTH;
		wall_y2[id] = wall_y1[id] + 3;
		wall_exists[id + 1] = 1;
		wall_x1[id + 1] = wall_x1[id] + fmod((float)rand(), wall_x2[id] - wall_x1[id]);
		wall_y1[id + 1] = wall_y1[id] - fmod(rand(), (room_h * WALL_LENGTH * 0.85));
		if (wall_y1[id + 1] + 3 < PLAYER_HEIGHT * 2) {
			wall_y1[id + 1] = PLAYER_HEIGHT * 2 - 3;
		}
		if (wall_y1[id + 1] + room_h * WALL_LENGTH > room_h - PLAYER_HEIGHT * 2) {
			wall_y1[id + 1] = room_h - room_h*WALL_LENGTH - PLAYER_HEIGHT * 2;
		}
		wall_x2[id + 1] = wall_x1[id + 1] + 3;
		wall_y2[id + 1] = wall_y1[id + 1] + room_h * WALL_LENGTH;
	}
	else {
		wall_exists[id] = TRUE;
		float w;
		float h;
		if ((int)(rand() % 2) == FALSE) {
			w = 3;
			h = room_h * WALL_LENGTH;
		}
		else {
			w = room_w * WALL_LENGTH;
			h = 3;
		}
		wall_x1[id] = PLAYER_WIDTH + fmod((float)rand(), room_w - PLAYER_WIDTH * 2 - w);
		wall_y1[id] = PLAYER_HEIGHT + fmod((float)rand(), room_h - PLAYER_HEIGHT * 2 - h);
		wall_x2[id] = wall_x1[id] + w;
		wall_y2[id] = wall_y1[id] + h;
		int compatible = TRUE;
		int while_ticker = FALSE;
		while (compatible) {
			wall_x1[id] = PLAYER_WIDTH + fmod((float)rand(), room_w - PLAYER_WIDTH - w);
			wall_y1[id] = PLAYER_HEIGHT + fmod((float)rand(), room_h - PLAYER_HEIGHT - h);
			wall_x2[id] = wall_x1[id] + w;
			wall_y2[id] = wall_y1[id] + h;
			compatible = FALSE;
			if (wall_x1[id] > PLAYER_WIDTH && wall_x2[id] < room_w - PLAYER_WIDTH && wall_y1[id] > PLAYER_HEIGHT && wall_y2[id] < room_h - PLAYER_HEIGHT) {
				for (int i = 0; i < MAX_WALLS; i += 1) {
					if (wall_exists[i] && i != id) {
						if (wall_x2[id] > wall_x1[i] - PLAYER_WIDTH * 2 && wall_x1[id] < wall_x2[i] + PLAYER_WIDTH * 2 
							&& wall_y2[id] > wall_y1[i] - PLAYER_HEIGHT * 2 && wall_y1[id] < wall_y2[i] + PLAYER_HEIGHT * 2) {
							compatible = 1;
						}
					}
				}
			}
			else {
				compatible = TRUE;
			}
			while_ticker += TRUE;
			if (while_ticker > MAX_TICKS) {
				wall_exists[id] = FALSE;
				compatible = FALSE;
			}
		}
	}
}
int dist_comp(float x1, float x2, float y1, float y2, float dist) {
	return sqrt(abs(pow(x2 - x1, 2) + pow(y2 - y1, 2))) < dist;
}
void place_sprite(float *x, float *y, float width, float height, int who_is) {
	*x = width + fmod((float)rand(), room_w - width / 2);
	*y = height + fmod((float)rand(), room_h - height / 2);
	int incompatible = 1;
	int while_ticker = 0;
	while (incompatible) {
		seed += SEED_INCREMENT;
		srand(seed);
		*x = width + fmod((float)rand(), room_w - width * 2);
		*y = height + fmod((float)rand(), room_h - height * 2);
		incompatible = 0;
		for (int i = 0; i < MAX_WALLS; i += 1) {
			if (i < MAX_MONSTERS) {
				if (monster_exists[i] == 1) {
					if (dist_comp(*x, monster_x[i], *y, monster_y[i], MONSTER_DIST)) {
						incompatible = 1;
					}
				}
			}
			if (wall_exists[i] == 1) {
				if ( !(*x + width < wall_x1[i] || *x - width > wall_x2[i] || *y + height < wall_y1[i] || *y - height > wall_y2[i])) {
					incompatible = 1;
				}
			}
		}
		if (who_is!=1 && dist_comp(*x, shield_x, *y, shield_y, ITEM_DIST)) {
			incompatible = 1;
		} else if (who_is!=2 && dist_comp(*x, bomb_x, *y, bomb_y, ITEM_DIST)) {
			incompatible = 1;
		} else if (who_is != 3 && dist_comp(*x, bow_x, *y, bow_y, ITEM_DIST)) {
			incompatible = 1;
		} else if (who_is != 4 && dist_comp(*x, key_x, *y, key_y, ITEM_DIST)) {
			incompatible = 1;
		}
		while_ticker += 1;
		if (while_ticker > MAX_TICKS) {
			incompatible = 0;
		}
	}
}
void reset_gamearrs(void) {
	for (int i = 0; i < MAX_WALLS; i += 1) {
		wall_exists[i] = 0;
		if (i < MAX_MONSTERS) {
			monster_exists[i] = 0;
		}
		if (i < MAX_TREASURES) {
			treasure_exists[i] = 0;
		}
	}

}
void reset_items(void) {
	bow_following = 0;
	shield_following = 0;
	key_following = 0;
	bomb_following = 0;
	bow_exists = 0;
	bow_x = 0;
	bow_y = 0;
	bomb_exists = 0;
	bomb_x = 0;
	bomb_y = 0;
	shield_exists = 0;
	shield_x = 0;
	shield_y = 0;
}
void generate_room(int wall_count, int treasure_count, int monster_count) {
	reset_items();
	reset_gamearrs();
	generate_wall(0, 1);
	for (int i = 2; i < wall_count; i += 1) {
		generate_wall(i, 0);
	}
	for (int i = 0; i < treasure_count; i += 1) {
		place_sprite(&treasure_x[i], &treasure_y[i], TREASURE_WIDTH, TREASURE_HEIGHT, 0);
		treasure_exists[i] = 1;
	}
	for (int i = 0; i < monster_count; i += 1) {
		place_sprite(&monster_x[i], &monster_y[i], MONSTER_WIDTH, MONSTER_HEIGHT, 0);
		monster_exists[i] = 1;
	}
	place_sprite(&door_x, &door_y, DOOR_WIDTH, DOOR_HEIGHT, 0);
	key_following = 0;
	place_sprite(&key_x, &key_y, KEY_WIDTH, KEY_HEIGHT, 4);
	place_sprite(&player_x, &player_y, PLAYER_WIDTH, PLAYER_HEIGHT, 0);
	if (fmod(rand() * DICEROLL_MAX, DICEROLL_MAX) <= ITEM_DROPRATE) {
		place_sprite(&shield_x, &shield_y, SHIELD_WIDTH, SHIELD_HEIGHT, 1);
		shield_exists = 1;
	}
	seed += SEED_INCREMENT;
	srand(seed);
	if (fmod(rand() * DICEROLL_MAX, DICEROLL_MAX) <= ITEM_DROPRATE) {
		place_sprite(&bow_x, &bow_y, BOW_WIDTH, BOW_HEIGHT, 3);
		bow_exists = 1;
		bow_ammo = MAX_ARROWS - 1;
	}
	seed += SEED_INCREMENT;
	srand(seed);
	if (fmod(rand() * DICEROLL_MAX, DICEROLL_MAX) <= ITEM_DROPRATE) {
		place_sprite(&bomb_x, &bomb_y, BOW_WIDTH, BOW_HEIGHT, 2);
		bomb_exists = 1;
	}
}
void display_loading(void) {
	draw_string(LCD_X / 2 - (strlen(load_string) / 2) * CHAR_WIDTH, LCD_Y / 4, load_string, FG_COLOUR);
	loading_countdown -= 2 / (MILLISECONDS_IN_SECOND / DELAY);
	if (loading_countdown <= (2 / (MILLISECONDS_IN_SECOND / DELAY)) * 1.1) {
		bomb_fuse = 0;
		loading_countdown = 0;
		game_floor += TRUE;
		game_score += SCORE_LVL;
		generate_room(6, (int)(rand() % 5), 1 + (int)(rand() % 4));
		start_countdown = 3;
	}
}
void setup_game(void) {
	initialised = 0;
	reset_gamearrs();
	aim_angle = 0;
	reset_items();
	explosion = 0;
	gameover_countdown = 0;
	start_countdown = USB_DELAY;
	treasure_exists[0] = 1;
	treasure_x[0] = 20;
	treasure_y[0] = room_h / 1.5;
	wall_exists[0] = 1;
	wall_x1[0] = room_w / 2 - CASTLE_WIDTH / 2;
	wall_y1[0] = room_h / 2 - CASTLE_HEIGHT / 2;
	wall_x2[0] = room_w / 2 + CASTLE_WIDTH / 2;
	wall_y2[0] = room_h / 2 + CASTLE_HEIGHT / 2 - 1;
	monster_x[0] = room_w / 1.5;
	monster_y[0] = room_h * 0.75;
	monster_exists[0] = 1;
	door_x = room_w / 2;
	door_y = room_h / 2 + CASTLE_HEIGHT / 4 + 2;
	key_following = 0;
	player_dying = 0;
	game_score = 0;
	game_lives = MAX_LIVES;
	game_floor = 0;
	game_time = 0;
	player_x = room_w / 2;
	player_y = room_h / 1.1;
	key_x = room_w / 4;
	key_y = room_h / 2 + MONSTER_HEIGHT * 2;
}
void setup(void) {
	usb_init();
	setup_game();
	set_clock_speed(CPU_8MHz);
	init_ddrs();
	lcd_init(LCD_DEFAULT_CONTRAST);
}

void console_alert(char message[]) {
	usb_serial_putchar("\n"[0]);
	for (int i = 0; i < strlen(message); i += 1) {
		usb_serial_putchar(message[i]);
	}
	usb_serial_putchar("\n"[0]);
	usb_serial_putchar("\r"[0]);
}

void pickup_item(char message[]) {
	char pickup_buff[MED_BUFF];
	strcpy(pickup_buff, msg_pickup);
	strcat(pickup_buff, message);
	console_alert(pickup_buff);
}


void update_console(void ) {

		char final_buffer[LARGE_BUFF];
		calculate_timestring(final_buffer);
		char buffer[LARGE_BUFF];
		strcat(final_buffer, score_string);
		itoa(game_score, buffer, ITOA_DIGITAL);
		strcat(final_buffer, buffer);
		strcat(final_buffer, lives_string);
		itoa(game_lives, buffer, ITOA_DIGITAL);
		strcat(final_buffer, buffer);
		strcat(final_buffer, floor_string);
		itoa(game_floor, buffer, ITOA_DIGITAL);
		strcat(final_buffer, buffer);
		itoa(player_x, buffer, ITOA_DIGITAL);
		strcat(final_buffer, buffer);
		itoa(player_y, buffer, ITOA_DIGITAL);
		strcat(final_buffer, buffer);
		console_alert(final_buffer);
}

void time_passes() {

	game_time = (overflow_counter * 256.0 + TCNT0) * PRESCALE / FREQ;;
	if (floor(fmod(game_time * DELAY, USB_DELAY)) == 0) {
		update_console();
	}

}

void process(void) {
	clear_screen();
	if (usb_serial_putchar(0) == -1) {
		draw_string(LCD_X / 2 - (strlen(init_string) / 2) * CHAR_WIDTH, LCD_Y / 4, init_string, FG_COLOUR);
	}
	else {

		// ************************************************

		aim_angle = fmod(adc_read(PORT_POT1) * (720.0 / 1024.0), 360.0);
		aim_angle = aim_angle * (M_PI) / 180;

		// ************************************************
		if (explosion > 0) {
			if (explosion % 4 == 0) {
				toggle_leds();
			}
			explosion -= 1;
		}
		else {
			clear_leds();
			explosion = 0;
		}
		seed = (int)(game_time + SEED_PRIME) * SEED_PRIME;
		srand(seed);
		if (start_countdown > 0 || stick_pressed() || player_dying > 0 || loading_countdown > 0 || gameover_countdown > 0) {
			if (start_countdown > 0) {
				display_startscreen();

				if (initialised == 0) {
					console_alert(msg_welcome);
					initialised = 1;
				}
				
			}
			else if (gameover_countdown > 0) {
				display_gameover();
				if (gameover_countdown > 0.5) {
					gameover_countdown -= 0.1;
				}
				else {
					if (button_pressed()) {
						gameover_countdown = 0;
						clear_screen();
						setup_game();
					}
				}
			}
			else if (loading_countdown > 0) {
				display_loading();
			}
			else if (stick_pressed()) {
				time_passes();
				display_status();
			}
			else if (player_dying > 0) {
				player_dying -= 1;
				if (player_dying < 2) {
					if (game_lives <= 0) {
						gameover_countdown = 0.6;
					}
				}
				if (player_dying == 0) {
					bow_ammo = MAX_ARROWS - 1;
					bomb_fuse = 0;
					gameover_countdown = -1;
					key_following = 0;
					bomb_following = 0;
					bow_following = 0;
					shield_following = 0;

					place_sprite(&player_x, &player_y, PLAYER_WIDTH, PLAYER_HEIGHT, 0);
				}
			}
		}
		else {

			time_passes();

			
			if (item_pickup > 0) {
				item_pickup -= 1;
			}
			float projected_x = player_x + (stick_right() - stick_left()) * PLAYER_SPEED;
			float projected_y = player_y + (stick_down() - stick_up()) * PLAYER_SPEED;
			if (projected_x - PLAYER_WIDTH / 2 > 0 && projected_y - PLAYER_HEIGHT / 2 > 0 && projected_x + PLAYER_WIDTH / 2 < room_w && projected_y + PLAYER_HEIGHT / 2 - 2 < room_h) {
				int can_pass = 1;
				for (int i = 0; i < MAX_WALLS; i += 1) {
					if (wall_exists[i] == 1) {
						if (!key_following) {
							if (col_wall(key_x, key_y, KEY_WIDTH, KEY_HEIGHT, i, 0)) {
								key_x = key_x - 2 + fmod(rand(), 4);
								key_y = key_y - 2 + fmod(rand(), 4);
							}
						}
						if (col_wall(projected_x, projected_y, PLAYER_WIDTH, PLAYER_HEIGHT, i, 0)) {
							can_pass = 0;
						}
						if (col_wall(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, i, 0)) {
							player_x = player_x - 2 + fmod(rand(), 4);
							player_y = player_y - 2 + fmod(rand(), 4);
						}
					}
				}
				if (can_pass == 1) {
					player_x = projected_x;
					player_y = projected_y;
				}
			}
			for (int i = 0; i < MAX_MONSTERS; i += 1) {
				if (monster_exists[i] == 1) {
					if (monster_x[i] > view_x && monster_x[i] < view_x + LCD_X && monster_y[i] > view_y && monster_y[i] < view_y + LCD_Y) {
						float projected_x = monster_x[i] + ((monster_x[i] < player_x) - (monster_x[i] > player_x)) * MONSTER_SPEED;
						float projected_y = monster_y[i] + ((monster_y[i] < player_y) - (monster_y[i] > player_y)) * MONSTER_SPEED;
						if (projected_x - MONSTER_WIDTH / 2 > 0 && projected_y - MONSTER_HEIGHT / 2 > 0 && projected_x + MONSTER_WIDTH / 2 < room_w && projected_y + MONSTER_HEIGHT / 2 < room_h) {
							int can_pass = 1;
							for (int n = 0; n < MAX_WALLS; n += 1) {
								if (wall_exists[n] == 1) {
									if ( col_wall(projected_x, projected_y, MONSTER_WIDTH, MONSTER_HEIGHT, n, 1)) {
										can_pass = 0;
									}
								}
								if (col_wall(monster_x[i], monster_y[i], MONSTER_WIDTH, MONSTER_HEIGHT, n, -1)) {
									float dirbuffer = fmod(rand(), 360.0) * M_PI /180.0;
									monster_x[i]+= cos(dirbuffer) * PUSH_HYP;
									monster_y[i] += sin(dirbuffer) * PUSH_HYP;
								}
							}
							if (can_pass == 1) {
								monster_x[i] = projected_x;
								monster_y[i] = projected_y;
							}
						}
						if (col_sprite(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, monster_x[i], monster_y[i], MONSTER_WIDTH, MONSTER_HEIGHT)) {
							if (shield_exists == 1 && shield_following == 1) {
								console_alert(msg_kill);
								monster_exists[i] = 0;
								shield_exists = 0;
								shield_following = 0;
								item_pickup = 0;
							}
							else {
								game_lives -= 1;
								console_alert(msg_die);
								player_dying = 10;
								explosion = EXPLOSION_LENGTH;
							}
						}
					}
				}
			}
			for (int i = 0; i < MAX_TREASURES; i += 1) {
				if (treasure_exists[i] == 1) {
					if (col_sprite(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, treasure_x[i], treasure_y[i], TREASURE_WIDTH, TREASURE_HEIGHT)) {
						pickup_item("Treasure!");
						treasure_exists[i] = 0;
						game_score += SCORE_TREASURE;
					}
				}
			}
			if (!key_following) {

				key_x += (key_x - KEY_WIDTH < 0)- (key_x + KEY_WIDTH > room_w);
				key_y += (key_y - KEY_HEIGHT < 0)- (key_y + KEY_HEIGHT > room_h);

				if (col_sprite(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, key_x, key_y, KEY_WIDTH, KEY_HEIGHT)) {
					key_following = 1;
					pickup_item("Key!");
				}
			}
			else {
				key_x += (player_x - KEY_WIDTH / 2 - key_x) / 8;
				key_y += (player_y - key_y) / 8;
				if (projected_x + PLAYER_WIDTH > door_x && projected_x < door_x + DOOR_WIDTH && projected_y + PLAYER_HEIGHT > door_y && projected_y < door_y + DOOR_HEIGHT) {
					if (loading_countdown == 0) {
						console_alert(msg_lvlup);
						loading_countdown = (2 / (MILLISECONDS_IN_SECOND / DELAY)) * 12 ;
					}
				}
			}
			if (shield_exists == 1) {
				if (!shield_following) {
					if (item_pickup <= 0 && col_sprite(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, shield_x, shield_y, SHIELD_WIDTH, SHIELD_HEIGHT)) {
						bow_following = 0;
						bomb_following = 0;
						shield_following = 1;
						
						pickup_item("Shield!");
						
						item_pickup = ITEM_PICKUP_DELAY;
					}
				}
				else {
					shield_x += (player_x + SHIELD_WIDTH / 2 - shield_x) / 3;
					shield_y += (player_y - shield_y) / 3;
				}
			}
			if (bomb_exists == 1) {
				if (bomb_fuse > 0) {
					bomb_fuse -= 1;
					if (bomb_fuse <= 0) {
						bomb_following = 0;
						bomb_exists = 0;
						bomb_fuse = 0;
						explosion = EXPLOSION_LENGTH;
						for (int i = 0; i < MAX_MONSTERS; i += 1) {
							if (monster_exists[i] == 1) {
								if (monster_x[i] > view_x && monster_x[i] < view_x + LCD_X && monster_y[i] > view_y && monster_y[i] < view_y + LCD_Y) {
									console_alert(msg_kill);
									monster_exists[i] = 0;
									game_score += SCORE_KILL;
								}
							}
						}
					}
				}
				else {
					if (!bomb_following) {
						if (item_pickup <= 0 && col_sprite(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, bomb_x, bomb_y, BOMB_WIDTH, BOMB_HEIGHT)) {
							bow_following = 0;
							bomb_following = 1;
							shield_following = 0;

							pickup_item("Bomb!");

							item_pickup = ITEM_PICKUP_DELAY;
						}
					}
					else {
						bomb_x += (player_x + BOMB_WIDTH / 2 - bomb_x) / 3;
						bomb_y += (player_y - bomb_y) / 3;
						if (button_pressed()) {
							bomb_x = player_x + cos(aim_angle) * (PUSH_HYP + ( PLAYER_WIDTH + PLAYER_HEIGHT ) / 4);
							bomb_y = player_y + sin(aim_angle) * (PUSH_HYP + ( PLAYER_WIDTH + PLAYER_HEIGHT) / 4);
							bomb_following = 0;
							bomb_fuse = 2 * (MILLISECONDS_IN_SECOND / DELAY);
							item_pickup = 0;
						}
					}
				}
			}
			if (bow_exists == 1) {
				if (bow_ammo < 0) {
					bow_following = 0;
					bow_exists = 0;
					item_pickup = 0;
				}
				if (bow_cooldown > 0) {
					bow_cooldown -= 1;
				}
				else {
					bow_cooldown = 0;
				}
				if (!bow_following) {
					if (item_pickup <= 0 && col_sprite(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, bow_x, bow_y, BOW_WIDTH, BOW_HEIGHT)) {
						bow_following = 1;
						bomb_following = 0;
						shield_following = 0;
						pickup_item("Bow!");
						item_pickup = ITEM_PICKUP_DELAY;
					}
				}
				else {
					if (bow_cooldown == 0 && button_pressed() && bow_ammo >= 0) {
						arrow_exists[MAX_ARROWS - 1 - bow_ammo] = 1;
						arrow_x[MAX_ARROWS - 1 - bow_ammo] = player_x + cos(aim_angle) * PUSH_HYP;
						arrow_y[MAX_ARROWS - 1 - bow_ammo] = player_y + sin(aim_angle) * PUSH_HYP;
						arrow_dir[MAX_ARROWS - 1 - bow_ammo] = aim_angle;
						bow_cooldown = 0.5 * (MILLISECONDS_IN_SECOND / DELAY);
						bow_ammo -= 1;
					}
					bow_x += (player_x + BOW_WIDTH / 2 - bow_x) / 1.5;
					bow_y += (player_y - bow_y) / 1.5;
				}
			}
			for (int o = 0; o < MAX_ARROWS; o += 1) {
				if (arrow_exists[o] == 1) {
					if (arrow_x[o] > room_w || arrow_y[o] > room_h || arrow_y[o] < 0 || arrow_x[o] < 0) {
						arrow_exists[o] = 0;
					}
					for (int i = 0; i < MAX_WALLS; i += 1) {
						if (i < MAX_MONSTERS) {
							if (monster_exists[i] == 1) {
								if (monster_x[i] > view_x && monster_x[i] < view_x + LCD_X && monster_y[i] > view_y && monster_y[i] < view_y + LCD_Y) {
									if (col_sprite(arrow_x[o], arrow_y[o], ARROW_WIDTH, ARROW_HEIGHT, monster_x[i], monster_y[i], MONSTER_WIDTH, MONSTER_HEIGHT)) {
										console_alert(msg_kill);
										monster_exists[i] = 0;
										arrow_exists[o] = 0;
										game_score += SCORE_KILL;
									}
								}
							}
						}
						if (wall_exists[i] == 1) {
							if (col_wall(arrow_x[o], arrow_y[o], ARROW_WIDTH, ARROW_HEIGHT, i, 0)) {
								arrow_exists[o] = 0;
							}
						}
					}
					arrow_x[o] += cos(arrow_dir[o]) * PLAYER_SPEED * 2;
					arrow_y[o] += sin(arrow_dir[o]) * PLAYER_SPEED * 2;
				}
			}
			view_x = player_x - LCD_X / 2;
			view_y = player_y - LCD_Y / 2;
			draw_game();
		}
	}
	show_screen();
}

int main(void) {
	CPU_PRESCALE(0);
	setup();
	while (1) {
		process();
		_delay_ms(DELAY);
	}
	return 0;
}