#pragma once
namespace suku
{
#define SAVE_DIR L"Data\\Save"
#define WINDOWWIDTH 800
#define WINDOWHEIGHT 608
#define ROOMWIDTH 800
#define ROOMHEIGHT 608
#define WINDOWX 100
#define WINDOWY 50
#define CLEAR_COLOR RGB(255,255,255)

#define MAXFPS 50

#define DIR_LEFT 0
#define DIR_RIGHT 1
#define DIR_UP 2
#define DIR_DOWN 3

#define MAXKIND 100
#define ID_NULL 0
#define ID_WALL 1
#define ID_SPIKE 2
#define ID_WATER_NOEXTRAJUMP 3
#define ID_WATER_EXTRAJUMP 4
#define ID_TRIGGER 5
#define ID_PLAYER 6
#define ID_BLOOD 7
#define ID_CHERRY 8
#define ID_PLATFORM 9
#define ID_PLACECHANGER 10
#define ID_WALLJUMPL 11
#define ID_WALLJUMPR 12
#define ID_BOX 13

#define BLOOD_WIDTH 2
#define BLOOD_HEIGHT 2
#define BLOOD_PDX 0
#define BLOOD_PDY 0
#define BLOOD_PDWIDTH BLOOD_WIDTH
#define BLOOD_PDHEIGHT BLOOD_HEIGHT

#define PLAYER_WIDTH 34
#define PLAYER_HEIGHT 32
#define PLAYER_PDX 12
#define PLAYER_PDY 11
#define PLAYER_PDWIDTH 11
#define PLAYER_PDHEIGHT 21
#define PLAYER_CENTERX 17
#define PLAYER_CENTERY 23

#define VALUE_FPS 20
#define VALUE_G 0.4f
#define VALUE_MAXPAINT 50
#define VALUE_MAXTRIGGERNUM 10

#define PLAYER_JUMPTIME 2
#define PLAYER_MOVINGSPEED 3
#define PLAYER_JUMPHEIGHT1 8.5
#define PLAYER_JUMPHEIGHT2 7
#define PLAYER_MAXFALLINGSPEED 9
#define PLAYER_MAXFALLINGSPEED_INWATER 2
#define PLAYER_BLOODNUMONCE 10
#define PLAYER_BLOODNUMMAX 300
#define PLAYER_BLOODWOFFSET 5
#define PLAYER_BLOODHOFFSET 6

#define SIDE_LEFT 1
#define SIDE_RIGHT 0

#define STATE_STAND 0x0001
#define STATE_MOVING 0x0002
#define STATE_JUMP 0x0004
#define STATE_FALL 0x0008
#define STATE_SLIDING 0x0010
#define STATE_DIED 0x0020

#define BMPNUM_STAND 4
#define BMPNUM_RUNNING 2
#define BMPNUM_SLIDING 2
#define BMPNUM_JUMP 2
#define BMPNUM_FALL 2

#define BMPSPEED_STAND 5
#define BMPSPEED_RUNNING 3
#define BMPSPEED_SLIDING 3
#define BMPSPEED_JUMP 4
#define BMPSPEED_FALL 4
}