// General Open Zelda Functions

#if defined _general_included
  #endinput
#endif
#define _general_included

#include <string>

native strcpy(dest[], const source[], maxlength=sizeof dest) = strcopy;

// Define 8 Directions
enum _Directions{
  north,
  east,
  south,
  west,
  northeast,
  southeast,
  southwest,
  northwest,
  }


stock PutSprite(ident[], valx, valy, vald, layer = 0, r = 255, g = 255, b = 255, a = 255, rot = 0, scale = 100)
{
    GraphicsDraw( ident, SPRITE, valx, valy, (layer*1000)+1, 0, 0, (r << 24 | g << 16 | b << 8 | a) );
}

stock DrawImage(code[], x, y, r = 255, g = 255, b = 255, a = 255, rot = 0, scale = 100) {
  return 1;
}

stock GetWidth(ident[]) {
  return 1;
}

stock GetHeight(ident[]) {
  return 1;
}

stock QKey() {
  return 1;
}

stock AKey() {
  return 1;
}

stock SKey() {
  return 1;
}

stock WKey() {
  return 1;
}

stock EnterKey() {
  return 1;
}

stock UpKey() {
  return 1;
}

stock DownKey() {
  return 1;
}

stock LeftKey() {
  return 1;
}

stock RightKey() {
  return 1;
}

stock SetQKey(val) {
  return 1;
}

stock SetAKey(val) {
  return 1;
}

stock SetSKey(val) {
  return 1;
}

stock SetWKey(val) {
  return 1;
}

stock SetEnterKey(val) {
  return 1;
}

stock SetUpKey(val) {
  return 1;
}

stock SetDownKey(val) {
  return 1;
}

stock SetLeftKey(val) {
  return 1;
}

stock SetRightKey(val) {
  return 1;
}

stock SetWorldX(val) {
  return 1;
}

stock SetWorldY(val) {
  return 1;
}

stock GetWorldX() {
  return 1;
}

stock GetWorldY() {
  return 1;
}

stock GetGroupMinX() {
  return 1;
}

stock GetGroupMinY() {
  return 1;
}

stock GetGroupMaxX() {
  return 1;
}

stock GetGroupMaxY() {
  return 1;
}

stock UpdateWorldCo(x, y) {
  return 1;
}

stock NearPoint(x1, y1, x2, y2, dist) {
  return 1;
}

stock SetPauseLevel(val) {
  return 1;
}

stock GetPauseLevel() {
  return 1;
}

stock FirstRun() {
  return 1;
}

stock InitialPlayerX() {
  return 1;
}

stock InitialPlayerY() {
  return 1;
}


stock CheckForHoleXY(x,y) {
  return 1;
}

stock FadeTo(red, green, blue, speed) {
  return 1;
}

stock Wipe(direction, speed) {
  return 1;
}

stock CalculateAngle(x1,y1,x2,y2) {
  return 1;
}


// Saving loading functions
stock SetRestartTable(index, x, y, desc[] ) {
  return 1;
}

stock SetRestartPoint(x, y) {
  return 1;
}

stock ClearRestartTable() {
  return 1;
}


// Text Drawing Functions
stock SetTextSize( size ) {
  return 1;
}

stock DrawText(string[], x, y, red = 255, green = 255, blue = 255, alpha = 255) {
  return 1;
}


// Functions for drawing shapes
stock DrawRectangle(x1, y1, x2, y2, red, green, blue, alpha = 255, angle = 0) {
  return 1;
}

stock DrawLine(x1, y1, x2, y2, width, red, green, blue, aplha = 255) {
  return 1;
}


// Enables or Disables user input
stock EnableControls( enable ) {
  return 1;
}


// Copies a numeric value in val to the string
stock ToString(val, string[]) {
  return 1;
}


// Sets and Gets the screen something falls to when it falls down a hole
stock SetLowerLevel(x, y) {
  return 1;
}

stock GetLowerLevelX() {
  return 1;
}

stock GetLowerLevelY() {
  return 1;
}


// TextBox Functions
stock InitTextBox(string[], wid, priority) {
  return 1;
}

stock SetTextBoxColor(red, green, blue, alpha) {
  return 1;
}

stock FinishedReading() {
  return 1;
}

stock GetLastAnswer() {
  return 1;
}


// Time Functions for Day/Night
stock GetMinuteCount() {
  return 1;
}

stock GetHourCount() {
  return 1;
}

stock GetDayCount() {
  return 1;
}

stock SetMinuteCount(val) {
  return 1;
}

stock SetHourCount(val) {
  return 1;
}

stock SetDayCount(val) {
  return 1;
}

stock SetDayLength(val) {
  return 1;
}


// Global\Local Functions
stock SetWorldValue(index, val) {
  return 1;
}

stock GetWorldValue(index) {
  return 1;
}

stock SetGroupValue(index, val) {
  return 1;
}

stock GetGroupValue(index) {
  return 1;
}


stock SetGlobal(index, val) {
  return 1;
}

stock GetGlobal(index) {
  return 1;
}

stock SetLocal(index, val) {
  return 1;
}

stock GetLocal(index) {
  return 1;
}


// Sound functions
stock PlaySound(code[], volume = 200) {
  return 1;
}

stock SetMusicVolume(volume) {
  return 1;
}

stock PlayMusic(code[], fade) {
  return 1;
}

stock SetMusicFadeSpeed(speed = 110) {
  return 1;
}


// Starts a gameover screen
stock GameOver() {
  return 1;
}


// Used for changing the alpha value of a background sprite
stock SetSpriteAlpha( spriteCode[], newAlpha) {
  return 1;
}


// Debugging functions
stock ToFile(value) {
  return 1;
}

stock TextFile(string[]) {
  return 1;
}


stock DrawBitmapText(string[], x, y, style = 1, red = 255, green = 255, blue = 255, alpha = 255, scale = 200) {
  return 1;
}

stock CheckMask(x, y, checkForObjects = 0) {
  return 1;
}



stock GetTimeDelta()
{
	new q = GameFrame();
	return float(q)/1000;
}



stock Cos(a)
{
	  return 1;
}


stock Sin(a)
{
	  return 1;
}

stock isNull(str[])
{
		return !!str[0];
}