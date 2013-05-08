// Open Zelda Counter Object Functions

#if defined _counter_included
#endinput
#endif
#define _counter_included
#include <enums>

stock CreateCounter(min, max, ident[]) { return EntityCreate("counter", ident,0,0,0, GLOBAL_MAP, "dd", min, max); }
stock CreateCounterWithID(min, max, ident[]) { return EntityCreate("counter", ident,0,0,0, GLOBAL_MAP, "dd", min, max); }
stock SetCounterMin(ident[], val) { return EntityPublicFunction(ident, "@SetMin", "n", val); }
stock SetCounterMax(ident[], val) { return EntityPublicFunction(ident, "@SetMax", "n", val); }
stock SetCounterValue(ident[], val) { return EntityPublicFunction(ident, "@SetValue", "n", val); }
stock SetCounterTarget(ident[], val) { return EntityPublicFunction(ident, "@SetTarget", "n", val); }
stock IncCounterTarget(ident[], val) { return EntityPublicFunction(ident, "@IncreaseTarget", "n", val); }
stock IncCounterValue(ident[], val) { return EntityPublicFunction(ident, "@IncreaseValue", "n", val); }
stock GetCounterValue(ident[]) { return EntityPublicFunction(ident, "@GetValue", ""); }
stock SetCounterSpeed(ident[], val) { return EntityPublicFunction(ident, "@SetSpeed", "n", val); }
stock DeleteCounter(ident[]) { EntityDelete(ident[]); }
stock GetCounterMin(ident[]) { return EntityPublicFunction(ident, "@GetMin", ""); }
stock GetCounterMax(ident[]) { return EntityPublicFunction(ident, "@GetMax", ""); }