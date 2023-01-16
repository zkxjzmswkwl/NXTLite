#pragma once
#include "Util.h"

struct Skill {
	int totalExp;
	int maxLevel;
	int curLevel;
};

struct SkillTree {
	Skill attack;
	char awidojawriuo3qw[0xC];
	Skill defense;
	char asdijasda[0xC];
	Skill strength;
	char aiosdjasoid[0xC];
	Skill constitution;
	char asoidjasiodq[0xC];
	Skill ranged;
	char oasjdiasdj[0xC];
	Skill prayer;
	char oiqwjedqwod[0xC];
	Skill magic;
	char qweqwoieqwe[0xC];
	Skill cooking;
	char qwoiejqwoe[0xC];
	Skill woodcutting;
	char wiqoejqwoeijq[0xC];
	Skill fletching;
	char iawojdoaiwd[0xC];
	Skill fishing;
	char qiwojeqwe[0xC];
	Skill firemaking;
	char qojweqioejqwied[0xC];
	Skill crafting;
	char qojweqwe[0xC];
	Skill smithing;
	char qwioejqwoeijqweiojqwe[0xC];
	Skill mining;
	char iwqjdioweqwe[0xC];
	Skill herblore;
	char qdqwjidoqjwdqwdqwedoqi[0xC];
	Skill agility;
	char qwieojqwoeiqwjeqweq[0xC];
	Skill thieving;
	char oqjedqwodasdaskldasjdqwoeiq[0xC];
	Skill slayer;
	char qwioejqwoeijqweq[0xC];
	Skill farming;
	char zsdaiojsdiajd[0xC];
	Skill runecrafting;
	char opqweioqwjeriqrf[0xC];
	Skill hunter;
	char qwieojqweoiqwjejqiowe[0xC];
	Skill construction;
	char qwioejqweazv[0xC];
	Skill summoning;
	char vjiodnvja[0xC];
	Skill dungeoneering;
	char vndjnfkjsdnfsdjkf[0xC];
	Skill divination;
	char nbvjnbjkfdgnj[0xC];
	Skill invention;
	char qwoierwfgbn[0xC];
	Skill archaeology;
};

SkillTree* loadSkills(uintptr_t clientBase) {
	return (SkillTree*)(iterateChain((uintptr_t)(clientBase + 0x00BF4458), {0x30, 0xB40, 0x20, 0x1B8, 0x190, 0x10, 0xC}));
}
