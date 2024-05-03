#pragma once
#define gMap 0x3C2560 //0x3BC560
#define gGame 0x3C2320 //0x3BC320 //3C2320
#define gThing 0x3C152C//0x3BB52C
//dword_7C152C 3C152C 3C2560 3C152C


#define knownCreatures 0x1C0
#define battleList gMap + knownCreatures
//UnorderedMapOffsets
#define Unknown 0x0
#define BufferPointer 0x8
#define Count 0xC

//UnorderedMapBufferOffsets
#define NodePointer 0x0

//UnorderedMapNodeOffsets
#define	NextPointer 0x0
#define	PreviousPointer 0x4
#define	Key 0x8
#define	Value 0x8

//Functions
#define cAttack 0xE2540//0x13ADA0
#define cCancelAttack 0x12DDF0 //12DDF0
#define cOnUseItem  0x13F850
#define cOnUseItemWith 0x13F9A0
#define cOnUseItemOnCreature 0x13F6A0
#define cOnTalk  0x13C7C0
#define cOnAddThing  0xF32A0


//LocalPlayer offset
#define gHealth 0x390
#define gMaxHealth 0x398
#define gMana 0x3C8
#define gMaxMana 0x3D0

//Creatures Battlelist OffSets
#define cHealth 0x38
#define cPosX 0xC
#define cPosY 0x10
#define cPosZ 0x14
#define cId 0x1C
#define cName 0x28
#define cDir 0x3C


