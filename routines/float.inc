#define Float_included

#define Float: Fixed:


native Fixed:float(value) = fixed; 
native Fixed:strfloat(const string[]); = strfixed;
native Fixed:floatmul(Float:oper1, Float:oper2) = fmul;

//native Fixed:floatdiv(Float:oper1, Float:oper2); /* Divide the dividend float by the divisor float */
//native Fixed:floatadd(Float:oper1, Float:oper2); /* Add two floats together */
//native Fixed:floatsub(Float:oper1, Float:oper2); /* Subtract oper2 float from oper1 float */
//native Fixed:floatfract(Float:value); /* Return the fractional part of a float */

native floatround(Float:value, round_method:method=round_normal) = fround;
//native floatcmp(Float:oper1, Float:oper2); /* Compare two integers. If the two elements are equal, return 0. If the first argument is greater than the second argument, return 1. If the first argument is less than the second argument, return -1. */
//native Fixed:floatsqroot(Float:value); /* Return the square root of the input value, same as floatpower(value, 0.5) */
//native Fixed:floatpower(Float:value, Float:exponent); /* Return the value raised to the power of the exponent */
//native Fixed:floatlog(Float:value, Float:base=10.0); /* Return the logarithm */
//native Fixed:floatabs(Float:value); /* Return the absolute value */

/**************************************************/
/* Return the sine, cosine or tangent. The input angle may be in radian, degrees or grades. */
//native Float:floatsin(Float:value, anglemode:mode=radian);
//native Float:floatcos(Float:value, anglemode:mode=radian);
//native Float:floattan(Float:value, anglemode:mode=radian);
//native Float:floatatan2(Float:x, Float:y, anglemode:mode=radian );
