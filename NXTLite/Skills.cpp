#include "pch.h"
#include "Skills.h"

SkillTree* loadSkills(uintptr_t clientBase) {
	return (SkillTree*)(Util::iterateChain((uintptr_t)(clientBase + 0x00BF4458), {0x30, 0xB40, 0x20, 0x1B8, 0x190, 0x10, 0xC}));
}
