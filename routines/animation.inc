// Open Zelda Animation Object Functions

#if defined _animation_included
  #endinput
#endif
#define _animation_included


stock CreateAnim(speed, ident[]) {}
stock CreateAnimWithID(speed, ident[]) {}
stock AddAnimframe(ident[], x, y, code[]) {}
stock SetAnimSpeed(ident[], speed) {}
stock GetAnimImage(ident[], code[]) {}
stock GetAnimCount(ident[]) { return 0; }
stock SetAnimCount(ident[], val) {}
stock DeleteAnim(ident[]) {}
stock IncrementAnim(ident[]) {}
stock GetMaxAnim(ident[]) { return 0; }
stock GetAnimWidth(ident[]) { return 0; }
stock GetAnimHeight(ident[]) { return 0;}
stock SetAnimDirection(ident[], val) {}
stock SetAnimLoop(ident[], doesloop) {}
stock FinishedAnim(ident[]) { return 1;}

stock DrawAnim(ident[], x, y, depth, layer = 0, r = 255, g = 255, b = 255, a = 255, rot = 0, scale = 100)
{

}


stock DrawAnimNoInc(ident[], x, y, depth, layer = 0, r = 255, g = 255, b = 255, a = 255, rot = 0, scale = 100)
{

}




