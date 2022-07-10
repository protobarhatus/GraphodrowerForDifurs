#include"Conversions.h"
#include"..\Math and data works\Math.h"
#include"Some string functions.h"
#include <cmath>
#include <string.h>
//переделать, доделать, убрать. ужасный файл

int toInt(std::string convertible)
{
	if (convertible == "")
		return 0;
	int result = 0;
	int length = convertible.length();
	for (int i = (convertible[0] == '-' ? 1 : 0); i< length;i++)
		result = result + (convertible[i] - 48) * pow(10, length - i - 1);
	if (convertible[0] == '-')
		result = result * -1;
	return result;
}
double conDouble(std::string convertible)
{
	if (convertible == "")
		return 0;
	convertible += "\a";
	double result = 0.00; int a = 0;
	std::string b;
	std::string af;
	if (convertible[0] == '-')
		a++;
	for (; convertible[a] != '.'; a++)
	{

		b += convertible[a];
	}
	for (a++; convertible[a] != '\a'; a++)
	{
		af += convertible[a];
	}

	result += toInt(af);
	result /= Discharge(1, af.length());
	result += toInt(b);
	if (convertible[0] == '-')
		result = result * -1;
	return result;
}
std::string String(int value)
{
	std::string result;
	bool is_negative = value < 0;
	if (is_negative)
		value = -value;
	do
	{
		result += value % 10 + '0';
	} while ((value /= 10) > 0);
	if (is_negative)
		result += '-';

	reverse(result);
	return result;
}
std::string string(double value)
{

	int chkkk(value);
	if (value == chkkk)
		return String(int(value)) + ".0";

	std::string rb;
	std::string ra;
	std::string chan;
	if (value < 0)
	{
		rb += "-";
		value *= -1;
	}
	int fch = value;
	rb += String(fch);
	float f;
	f = value - fch;
	int a = 0;
	float sf;
	int si;
	sf = f * Discharge(1, a);
	si = sf;
	for (; si != sf; a++)
	{

		sf = f * Discharge(1, a);
		si = sf;
	}

	f = f * Discharge(1, a - 1);

	fch = f;
	chan = String(fch);

	for (int fchkn = chan.length(); fchkn < a - 1; fchkn++)
	{
		ra += "0";
	}
	ra += String(fch);
	return rb + "." + ra;
}
