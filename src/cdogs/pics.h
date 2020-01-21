/*
    C-Dogs SDL
    A port of the legendary (and fun) action/arcade cdogs.
    Copyright (C) 1995 Ronny Wester
    Copyright (C) 2003 Jeremy Chin 
    Copyright (C) 2003-2007 Lucas Martin-King 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    This file incorporates work covered by the following copyright and
    permission notice:

    Copyright (c) 2014, Cong Xu
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __PICS
#define __PICS

#include "blit.h"
#include "defs.h"
#include "pic_file.h"



#define PIC_DARKNESS        0
#define PIC_TALLDARKNESS    5
#define PIC_UZIBULLET      42
#define PIC_DRAINAGE      164
//#define PIC_EXIT          375
//#define PIC_EXITSHADOW    376
#define PIC_COUNT1        398
#define PIC_COUNT2        182
#define PIC_MAX           (PIC_COUNT1 + PIC_COUNT2)
#define P2                PIC_COUNT1
#define PIC_LOGO          (P2 + 122)

#define FIREBALL_MAX    16
#define DEATH_MAX       9
#define BLOOD_MAX       3

#define OFSPIC_BULLET                   0
#define OFSPIC_DOOR                     1
#define OFSPIC_SPARK                    2
#define OFSPIC_BARREL                   3
#define OFSPIC_BOX                      4
#define OFSPIC_BOX2                     5
#define OFSPIC_WRECKEDBARREL            6
#define OFSPIC_WRECKEDBOX               7
#define OFSPIC_BARREL2                  8
#define OFSPIC_BOX3                     9
#define OFSPIC_WRECKEDBOX3             10
#define OFSPIC_WRECKEDBARREL2          11
#define OFSPIC_FOLDER                  12
#define OFSPIC_DISK1                   13
#define OFSPIC_DISK2                   14
#define OFSPIC_DISK3                   15
#define OFSPIC_VDOOR                   16
#define OFSPIC_VDOOR_OPEN              17
#define OFSPIC_VDOOR_RED               18
#define OFSPIC_HDOOR_RED               19
#define OFSPIC_KEYCARD_RED             20
#define OFSPIC_VDOOR_BLUE              21
#define OFSPIC_HDOOR_BLUE              22
#define OFSPIC_KEYCARD_BLUE            23
#define OFSPIC_VDOOR_GREEN             24
#define OFSPIC_HDOOR_GREEN             25
#define OFSPIC_KEYCARD_GREEN           26
#define OFSPIC_VDOOR_YELLOW            27
#define OFSPIC_HDOOR_YELLOW            28
#define OFSPIC_KEYCARD_YELLOW          29
#define OFSPIC_CABINET                 30
#define OFSPIC_WRECKEDCABINET          31
#define OFSPIC_SAFE                    32
#define OFSPIC_WRECKEDSAFE             33
#define OFSPIC_CHAIR                   34
#define OFSPIC_WRECKEDCHAIR            35
#define OFSPIC_PLANT                   36
#define OFSPIC_WRECKEDPLANT            37
#define OFSPIC_TABLE                   38
#define OFSPIC_WRECKEDTABLE            39
#define OFSPIC_MOLOTOV                 40
#define OFSPIC_SNIPERBULLET            41
#define OFSPIC_DOOR2                   42
#define OFSPIC_VDOOR2                  43
#define OFSPIC_VDOOR2_OPEN             44
#define OFSPIC_VDOOR2_RED              45
#define OFSPIC_HDOOR2_RED              46
#define OFSPIC_KEY_RED                 47
#define OFSPIC_VDOOR2_BLUE             48
#define OFSPIC_HDOOR2_BLUE             49
#define OFSPIC_KEY_BLUE                50
#define OFSPIC_VDOOR2_GREEN            51
#define OFSPIC_HDOOR2_GREEN            52
#define OFSPIC_KEY_GREEN               53
#define OFSPIC_VDOOR2_YELLOW           54
#define OFSPIC_HDOOR2_YELLOW           55
#define OFSPIC_KEY_YELLOW              56
#define OFSPIC_SKULLBARREL_WOOD        57
#define OFSPIC_BARREL_WOOD             58
#define OFSPIC_WRECKEDBARREL_WOOD      59
#define OFSPIC_GRAYBOX                 60
#define OFSPIC_GREENBOX                61
#define OFSPIC_REDBOX                  62
#define OFSPIC_WRECKEDBOX_WOOD         63
#define OFSPIC_WOODTABLE               64
#define OFSPIC_WOODTABLE_CANDLE        65
#define OFSPIC_LABTABLE                66
#define OFSPIC_OGRESTATUE              67
#define OFSPIC_TERMINAL                68
// 69-77 available...
#define OFSPIC_ROD                     78
#define OFSPIC_TREE                    79
#define OFSPIC_MINE                    80
#define OFSPIC_DYNAMITE                81
#define OFSPIC_BOOKSHELF               82
#define OFSPIC_WOODENBOX               83
#define OFSPIC_ROCKET                  84
#define OFSPIC_CLOTHEDTABLE            85
#define OFSPIC_STEELTABLE              86
#define OFSPIC_BLUEPRINT               87
#define OFSPIC_CD                      88
#define OFSPIC_BAG                     89
#define OFSPIC_HOLO                    90
#define OFSPIC_BOTTLE                  91
#define OFSPIC_RADIO                   92
#define OFSPIC_CIRCUIT                 93
#define OFSPIC_PAPER                   94
#define OFSPIC_BURN                    95
#define OFSPIC_AUTUMNTREE              96
#define OFSPIC_AUTUMNTREE_REMAINS      97
#define OFSPIC_GREENTREE               98
#define OFSPIC_GREENTREE_REMAINS       99
#define OFSPIC_EGG                    100
#define OFSPIC_EGG_REMAINS            101
#define OFSPIC_TREE_REMAINS           102
#define OFSPIC_BLOODSTAIN             103
#define OFSPIC_VDOOR3_RED             104
#define OFSPIC_HDOOR3_RED             105
#define OFSPIC_KEY3_RED               106
#define OFSPIC_VDOOR3_BLUE            107
#define OFSPIC_HDOOR3_BLUE            108
#define OFSPIC_KEY3_BLUE              109
#define OFSPIC_VDOOR3_GREEN           110
#define OFSPIC_HDOOR3_GREEN           111
#define OFSPIC_KEY3_GREEN             112
#define OFSPIC_VDOOR3_YELLOW          113
#define OFSPIC_HDOOR3_YELLOW          114
#define OFSPIC_KEY3_YELLOW            115
#define OFSPIC_VDOOR3                 116
#define OFSPIC_HDOOR3                 117
#define OFSPIC_VDOOR4_RED             118
#define OFSPIC_HDOOR4_RED             119
#define OFSPIC_KEY4_RED               120
#define OFSPIC_VDOOR4_BLUE            121
#define OFSPIC_HDOOR4_BLUE            122
#define OFSPIC_KEY4_BLUE              123
#define OFSPIC_VDOOR4_GREEN           124
#define OFSPIC_HDOOR4_GREEN           125
#define OFSPIC_KEY4_GREEN             126
#define OFSPIC_VDOOR4_YELLOW          127
#define OFSPIC_HDOOR4_YELLOW          128
#define OFSPIC_KEY4_YELLOW            129
#define OFSPIC_VDOOR4                 130
#define OFSPIC_HDOOR4                 131
#define OFSPIC_WALL_SKULL             132
#define OFSPIC_BONE_N_BLOOD           133
#define OFSPIC_BULLET_MARKS           134
#define OFSPIC_BLOOD                  135
#define OFSPIC_SKULL                  136
#define OFSPIC_SCRATCH                137
#define OFSPIC_WALL_STUFF             138
#define OFSPIC_WALL_GOO               139
#define OFSPIC_GOO                    140
#define OFSPIC_COUNT                  141

#define WALL_SINGLE         0
#define WALL_LEFT           1
#define WALL_RIGHT          2
#define WALL_TOP            3
#define WALL_BOTTOM         4
#define WALL_TOPLEFT        5
#define WALL_TOPRIGHT       6
#define WALL_BOTTOMLEFT     7
#define WALL_BOTTOMRIGHT    8
#define WALL_LEFT_T         9
#define WALL_RIGHT_T       10
#define WALL_TOP_T         11
#define WALL_BOTTOM_T      12
#define WALL_VERTICAL      13
#define WALL_HORIZONTAL    14
#define WALL_CROSS         15
#define WALL_TYPES         16
const char *WallTypeStr(const int w);

typedef enum
{
	WALL_STYLE_STEEL,
	WALL_STYLE_BRICKS,
	WALL_STYLE_RED,
	WALL_STYLE_STEEL_N_WOOD,
	WALL_STYLE_STONE,
	WALL_STYLE_PLASTEEL,
	WALL_STYLE_GRANITE,
	WALL_STYLE_COUNT
} wall_style_e;
const char *WallStyleStr(const wall_style_e w);

#define FLOOR_SHADOW       0
#define FLOOR_NORMAL       1
#define FLOOR_1            2
#define FLOOR_2            3
#define FLOOR_TYPES        4
const char *FloorTypeStr(const int f);

#define ROOMFLOOR_SHADOW   0
#define ROOMFLOOR_NORMAL   1
#define ROOMFLOOR_TYPES    2
const char *RoomTypeStr(const int r);

typedef enum
{
	FLOOR_STYLE_GREEN,
	FLOOR_STYLE_PURPLE,
	FLOOR_STYLE_DIRT,
	FLOOR_STYLE_BLUE,
	FLOOR_STYLE_STRIPES,
	FLOOR_STYLE_SMALLSQUARES,
	FLOOR_STYLE_STONE,
	FLOOR_STYLE_WOOD,
	FLOOR_STYLE_WHITE,
	FLOOR_STYLE_GRID,
	FLOOR_STYLE_COUNT
} floor_style_e;
const char *FloorStyleStr(const floor_style_e f);

typedef enum
{
	ROOM_STYLE_RECESSED,
	ROOM_STYLE_TILE,
	ROOM_STYLE_DIRT,
	ROOM_STYLE_FLAT,
	ROOM_STYLE_STRIPED,
	ROOM_STYLE_SMALLSQUARES,
	ROOM_STYLE_STONE,
	ROOM_STYLE_WOOD,
	ROOM_STYLE_GRID,
	ROOM_STYLE_BIGGRID,
	ROOM_STYLE_CHECKER,
	ROOM_STYLE_COUNT
} RoomStyle;
const char *RoomStyleStr(const RoomStyle r);


struct Offset {
	int dx, dy;
};

typedef struct Offset OffsetTable[DIRECTION_COUNT];


extern TPalette origPalette;

extern const int cBodyPic[BODY_COUNT][DIRECTION_COUNT][STATE_COUNT];
extern const int cHeadPic[FACE_COUNT][DIRECTION_COUNT][STATE_COUNT+2];
extern const OffsetTable cBodyOffset[BODY_COUNT];
extern const OffsetTable cNeckOffset[BODY_COUNT];
extern const OffsetTable cGunHandOffset[BODY_COUNT];
extern const OffsetTable cHeadOffset[FACE_COUNT];
extern const TOffsetPic cGrenadePics[4];
extern const TOffsetPic cFlamePics[4];
extern const TOffsetPic cFireBallPics[FIREBALL_MAX];
#define BEAM_PIC_COUNT 2
extern const TOffsetPic cBeamPics[BEAM_PIC_COUNT][DIRECTION_COUNT];
extern const TOffsetPic cGeneralPics[OFSPIC_COUNT];
extern const int cWallPics[WALL_STYLE_COUNT][WALL_TYPES];
extern const int cFloorPics[FLOOR_STYLE_COUNT][FLOOR_TYPES];
extern const int cRoomPics[ROOM_STYLE_COUNT][ROOMFLOOR_TYPES];
extern const struct Offset cWallOffset;
extern const TOffsetPic cDeathPics[DEATH_MAX];
extern const TOffsetPic cBloodPics[BLOOD_MAX];

#endif
