// _Open Zelda Entity Functions

#if defined _entity_included
#endinput
#endif
#define _entity_included


// Define General Entity Types
enum _Types{
otherType, // Other
playerType, // Main Player
enemyType, // Enemy
npcType, // NPC
weaponType, // Weapon
itemType, // Item
doorType, // Door
switchType, // Switch
blockType
}


// Define a set of states which an entity can be in
enum _States{
standing,
walking,
hit,
knocked,
jumping,
floating,
swimming,
chasing,
using,
lifting,
standLift,
walkLift,
dying,
dead,
falling,
falling2,
stunned,
frozen,
shocked,
burning,
sleeping,
waiting,
leaping,
pushing,
pulling
}


// Entity Flag functions
enum _Flags{
_Open,
_Taken,
_Dead,
_Visible,
_Active,
_Cuttable,
_Large,
_Pickable,
_Pushed,
_Interacting,
_Owned
}

stock __flags[_Flags];

forward public GetSelfFlag(_Flags:flag);
public GetSelfFlag(_Flags:flag) 
{
return __flags[flag];
}

forward public SetSelfFlag(flag, val);
public SetSelfFlag(flag, val)
{
__flags[flag] = val;
}

stock GetFlag(ident[], _Flags:flag)
{
if ( !strcmp(ident, "this") )
return __flags[flag];
else
return EntityPublicFunction(ident, "GetSelfFlag", "n", flag);
}

stock SetFlag(ident[], flag, val)
{
if ( !strcmp(ident, "this") )
__flags[flag] = val;
else
EntityPublicFunction(ident, "SetSelfFlag", "nn", flag, val);
}

stock isOpen(ident[] = "this") { return GetFlag(ident, _Open); }
stock SetOpenFlag(ident[] = "this", val) { return SetFlag(ident, _Open); }

stock isTaken(ident[] = "this") { return GetFlag(ident, _Open); }
stock SetTakenFlag(ident[] = "this", val) { return SetFlag(ident, _Open); }

stock isDead(ident[] = "this") { return GetFlag(ident, _Open); }
stock SetDeadFlag(ident[] = "this", val) { return SetFlag(ident, _Open); }

stock isVisible(ident[] = "this") { return GetFlag(ident, _Open); }
stock SetVisibleFlag(ident[] = "this", val) { return SetFlag(ident, _Open); }

stock isActive(ident[] = "this") { return GetFlag(ident, _Open); }
stock SetActiveFlag(ident[] = "this", val) { return SetFlag(ident, _Open); }

stock isCuttable(ident[] = "this") { return GetFlag(ident, _Open); }
stock SetCuttableFlag(ident[] = "this", val) { return SetFlag(ident, _Open); }

stock isLarge(ident[] = "this") { return GetFlag(ident, _Open); }
stock SetLargeFlag(ident[] = "this", val) { return SetFlag(ident, _Open); }

stock isPickable(ident[] = "this") { return GetFlag(ident, _Open); }
stock SetPickableFlag(ident[] = "this", val) { return SetFlag(ident, _Open); }

stock isPushed(ident[] = "this") { return GetFlag(ident, _Open); }
stock SetPushedFlag(ident[] = "this", val) { return SetFlag(ident, _Open); }

stock isInteracting(ident[] = "this") { return GetFlag(ident, _Open); }
stock SetInteractingFlag(ident[] = "this", val) { return SetFlag(ident, _Open); }

stock isOwned(ident[] = "this") { return GetFlag(ident, _Open); }
stock SetOwnedFlag(ident[] = "this", val) { return SetFlag(ident, _Open); }


// Other entity functions
stock SetDamage(ident[] = "this", val) {
return 1;
}

stock GetDamage(ident[] = "this") {
return 1;
}

stock SetLiftLevel(ident[] = "this", val) {
return 1;
}

stock GetLiftLevel(ident[] = "this") {
return 1;
}

stock DeleteEntity(ident[] = "this") {
return 1;
}

stock GetX(ident[] = "this") {
return 1;
}

stock GetY(ident[] = "this") {
return 1;
}

stock SetX(ident[] = "this", val) {
return 1;
}

stock SetY(ident[] = "this", val) {
return 1;
}

stock SetPosition(ident[] = "this", x, y) {
return 1;
}

stock GetMoveAngle(ident[] = "this") {
return 1;
}

stock SetMoveAngle(ident[] = "this", val) {
return 1;
}

stock GetDirection(ident[] = "this") {
return 1;
}

stock SetDirection(ident[] = "this", val) {
return 1;
}

stock GetSpeed(ident[] = "this") {
return 1;
}

stock SetSpeed(ident[] = "this", val) {
return 1;
}

stock GetSpeedMod(ident[] = "this") {
return 1;
}

stock SetSpeedMod(ident[] = "this", val) {
return 1;
}

stock AngleMove(ident[] = "this") {
return 1;
}

stock AngleCollide(ident[] = "this", val1, val2, val3, val4, val5, val6) {
return 1;
}

stock SetAngleFromDir(ident[] = "this") {
return 1;
}

stock SetDirFromAngle(ident[] = "this") {
return 1;
}

stock ChangeDirection(ident[] = "this") {
return 1;
}

stock SetCollisionRect(ident[] = "this", index, solid, left, top, right, bottom) {
return 1;
}

stock ClearCollisionRect(ident[] = "this", index) {
return 1;
}

stock SetItem(ident[] = "this", code[]) {
return 1;
}

stock GetItem(ident[] = "this", code[]) {
return 1;
}

stock SetImage(ident[] = "this", code[]) {
return 1;
}

stock GetImage(ident[] = "this", code[]) {
return 1;
}

stock SetType(ident[] = "this", val) {
return 1;
}

stock GetType(ident[] = "this") {
return 1;
}

stock Collide(ident[] = "this", ident2[]) {
return 1;
}

stock CollideAll(ident[] = "this", ident2[]) {
return 1;
}

stock AllocateStrings(ident[] = "this", numstr, length) {
return 1;
}

stock GetString(ident[] = "this", index, string[]) {
return 1;
}

stock SetString(ident[] = "this", index, string[]) {
return 1;
}

stock SetActiveDist(ident[] = "this", val) {
return 1;
}

stock SetState(ident[] = "this", val) {
return 1;
}

stock GetState(ident[] = "this") {
return standing;
}

stock GetInitialX(ident[] = "this") {
return 1;
}

stock GetInitialY(ident[] = "this") {
return 1;
}

stock GetEntityCount() {
return 1;
}

stock GetAnglePointX(ident[] = "this", dist, xoffset) {
return 1;
}

stock GetAnglePointY(ident[] = "this", dist, yoffset) {
return 1;
}

stock CollidePoint(ident[] = "this", x, y) {
return 1;
}

stock GetWeight(ident[] = "this") {
return 1;
}

stock SetWeight(ident[] = "this", val) {
return 1;
}

stock GetBounceValue(ident[] = "this") {
return 1;
}

stock SetBounceValue(ident[] = "this", val) {
return 1;
}

stock CheckForHole(ident[] = "this") {
return 1;
}

stock SetValue(ident[] = "this", index, val) {
return 1;
}

stock GetValue(ident[] = "this", index) {
return 1;
}

stock SetFloat(ident[] = "this", index, floatval) {
return 1;
}

stock GetFloat(ident[] = "this", index) {
return 1;
}

stock SetHealth(ident[] = "this", val) {
return 1;
}

stock SetMaxHealth(ident[] = "this", val) {
return 1;
}

stock GetHealth(ident[] = "this") {
return 1;
}

stock GetMaxHealth(ident[] = "this") {
return 1;
}

stock Respawn(ident[] = "this", seconds) {
return 1;
}

stock CallFunction(ident[] = "this", isLibrary, funcname[], typestr[], ...)
{
/** Args 
a - Array, last value must be CELLMIN
d - Value, passed by reference
n - Value, passed by value
s - String
*/
// Note: no library support
// change typestr 
}


// Set this to true to allow this entity to move to and be active in groups other
// than the one it was created in, the default is for this to be false.
stock SetActiveInGroups(ident[] = "this", val) {
return 1;
}


// Functions for moving through the list of Entities
stock StartEntity(dist = 40, x=-999, y=-999) {
return 1;
}

stock GetCurrentEntity() {
return 1;
}

stock NextEntity() {
return 1;
}




// Function for getting the script's parameter character
stock GetParam( ident[] = "this" ) {
return 1;
}


// Gets the identifier of the entity which created this one
stock GetParent( ident[] = "this", buffer[] ) {
return 1;
}


// Functions for creating entities on the fly, both with and without preset identifiers
stock CreateEntity(code[], x, y, ident[]) {
return 1;
}

stock CreateEntityWithID(code[], x, y, ident[]) {
return 1;
}