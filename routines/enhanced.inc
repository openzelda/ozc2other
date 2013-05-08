// Enhanced Open Zelda Functions

#if defined _enhanced_included
	#endinput
#endif
#define _enhanced_included

#include <general>

stock WorldToScreenX(x)
{
	return (x - GetWorldX()) * 2
}

stock WorldToScreenY(y)
{
	return (y - GetWorldY()) * 2
}

stock GetEnterKey()
{
	if(EnterKey())
	{
		SetEnterKey(false) {
  return 1;
}

		return true;
	}
	return false;
}

stock GetAKey()
{
	if(AKey())
	{
		SetAKey(false) {
  return 1;
}

		return true;
	}
	return false;
}
stock GetSKey()
{
	if(SKey())
	{
		SetSKey(false) {
  return 1;
}

		return true;
	}
	return false;
}
stock GetQKey()
{
	if(QKey())
	{
		SetQKey(false) {
  return 1;
}

		return true;
	}
	return false;
}

stock GetWKey()
{
	if(WKey())
	{
		SetWKey(false) {
  return 1;
}

		return true;
	}
	return false;
}

stock EKey() {
  return 1;
}

stock DKey() {
  return 1;
}

stock ZKey() {
  return 1;
}

stock XKey() {
  return 1;
}

stock CKey() {
  return 1;
}

stock SetEKey(val) {
  return 1;
}

stock SetDKey(val) {
  return 1;
}

stock SetZKey(val) {
  return 1;
}

stock SetXKey(val) {
  return 1;
}

stock SetCKey(val) {
  return 1;
}

stock isInside() {
  return 1;
}

stock Save() {
  return 1;
}

stock Load() {
  return 1;
}

stock GetPlayerName(name[]) {
  return 1;
}

stock SetPlayerName(name[]) {
  return 1;
}

stock Version() {
  return 1;
}

