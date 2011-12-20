#pragma once

// Window constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Column constants
#define COLUMN_WIDTH 64
#define NUM_COLUMNS_LEFT 4
#define NUM_COLUMNS 8

// TargetSet constants
const int MIN_NUM_TARGETS = 3;
const int MAX_NUM_TARGETS = 15;
const int TARGET_SPEED = 300;
const float SPEED_RATIO = .5;
const int MAX_HIT_DISTANCE = 40;
const int MISS_PENALTY = -5;
const int SPEED_START = 125;
const int SPEED_INCREASE = 25;
const int BLOCK_BOUND = 50;
const int HIT_BOUND = 80;
const int CRITICAL_BOUND = 90;

// Menu constants
const int NUM_MENU_ITEMS = 2;
const int NUM_DIFFICULTY_LEVELS = 3;

// Special move constants
const int SPECIAL_PERCENTAGE = 10;

// Goal animation constants
const int HIT_FRAMES = 10;
const int OK_FRAMES = 10;
const int ROTATE_FRAMES = 10;
const float HIT_RATIO = 0.5;
const float OK_RATIO = 0.5;

// Damage constants
const int BASIC_ACTION_DAMAGE = 10;
const int CRITICAL_ACTION_DAMAGE = 20;
const int COMBO_ACTION_DAMAGE = 20;

// Action animation constants
const int NUM_PASSES = 3;

// Character constants
const int BASIC_ENEMY_HEALTH = 100;
const int PLAYER_HEALTH = 70;

const int NUM_IDLE_FRAMES = 5;
const int NUM_CRITICAL_FRAMES = 100;

const int NUM_KICK_HIT_FRAMES = 35;
const int NUM_KICK_BLOCK_FRAMES = 30;
const int NUM_KICK_COUNTER_FRAMES = 44;

const int NUM_PUNCH_HIT_FRAMES = 21;
const int NUM_PUNCH_BLOCK_FRAMES = 18;
const int NUM_PUNCH_COUNTER_FRAMES = 22;

const int NUM_HEADBUTT_HIT_FRAMES = 19;
const int NUM_HEADBUTT_BLOCK_FRAMES = 17;
const int NUM_HEADBUTT_COUNTER_FRAMES = 22;

const int NUM_PUNCH_PUNCH_FRAMES = 31;
const int NUM_PUNCH_KICK_FRAMES = 37;
const int NUM_PUNCH_HEADBUTT_FRAMES = 32;
const int NUM_KICK_PUNCH_FRAMES = 41;
const int NUM_KICK_KICK_FRAMES = 57;
const int NUM_KICK_HEADBUTT_FRAMES = 44;
const int NUM_HEADBUTT_PUNCH_FRAMES = 26;
const int NUM_HEADBUTT_KICK_FRAMES = 33;

const int NUM_SLIDE_TACKLE_FRAMES = 39;
const int NUM_CARTWHEEL_FRAMES = 53;
const int NUM_PUNCH_BEHIND_PUNCH_FRAMES = 44;
const int NUM_JUMP_KICK_FRAMES = 50;

const int NUM_CLIMB_FRAMES = 23;
const int NUM_ENTER_DOJO_FRAMES = 33;