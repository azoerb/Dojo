#pragma once

// Column constants
#define COLUMN_WIDTH 64
#define NUM_COLUMNS 4

// TargetSet constants
#define MIN_NUM_TARGETS 4
#define MAX_NUM_TARGETS 10
#define TARGET_SPEED 300
#define SPEED_RATIO .5
#define MAX_HIT_DISTANCE 50
#define MISS_PENALTY -10
#define SPEED_START 150
#define SPEED_INCREASE 50

// Goal animation constants
#define HIT_FRAMES 10
#define HIT_RATIO 0.3

// Damage constants
#define BASIC_ACTION_DAMAGE 10
#define COMBO_ACTION_DAMAGE 20

// Action animation constants
#define NUM_PASSES 3

// Character constants
#define BASIC_ENEMY_HEALTH 100
#define PLAYER_HEALTH 100

#define NUM_IDLE_FRAMES 5

#define NUM_KICK_HIT_FRAMES 35
#define NUM_KICK_BLOCK_FRAMES 30
#define NUM_KICK_COUNTER_FRAMES 44

#define NUM_PUNCH_HIT_FRAMES 21
#define NUM_PUNCH_BLOCK_FRAMES 18
#define NUM_PUNCH_COUNTER_FRAMES 22

#define NUM_HEADBUTT_HIT_FRAMES 19
#define NUM_HEADBUTT_BLOCK_FRAMES 17
#define NUM_HEADBUTT_COUNTER_FRAMES 22

#define NUM_PUNCH_PUNCH_FRAMES 31
#define NUM_PUNCH_KICK_FRAMES 37
#define NUM_PUNCH_HEADBUTT_FRAMES 32
#define NUM_KICK_PUNCH_FRAMES 41
#define NUM_KICK_KICK_FRAMES 57
#define NUM_KICK_HEADBUTT_FRAMES 44
#define NUM_HEADBUTT_PUNCH_FRAMES 26
#define NUM_HEADBUTT_KICK_FRAMES 33