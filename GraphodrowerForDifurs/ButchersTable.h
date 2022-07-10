#pragma once
#include <vector>
#include "MyLib/MyLib.h"
#include <functional>
class ButchersTable
{
private:
	typedef long double number;
	typedef std::vector<std::vector<number>> Table;
public:
	ButchersTable();
	ButchersTable(const Table& table, int amount_of_result_strings, bool is_explicit);
	//ахтунг нумерация здесь начинается с 1
	number C(int i) const;
	number a(int i, int j) const;
	number b(int i, int res_string) const;
	int getAmountOfIntermediateSteps() const;

	bool isExplicit() const;
private:
	Table table;
	int amount_of_result_strings;
	bool is_explicit;
};

ButchersTable dormanPrince();
ButchersTable gaussLegendreFourthOrder();
ButchersTable tableForRK4();
ButchersTable LStable3rdOrderImplicitRungeKutta();
ButchersTable fifthOrderRadau();
ButchersTable tableForRK6();
ButchersTable inexplicitEuler();
ButchersTable explicitEuler();


Vector ButcherRK(const ButchersTable& table, int result_string, Vector d, long double t, long double step, const std::function < Vector(long double, const Vector&)>& f,
	std::vector<std::pair<double, Vector>>* saved_values_at_proceeded_times = nullptr);




std::vector<Vector> ButcherRkForSystem(const ButchersTable& table, const std::vector<Vector>& start_values, long double t, long double step,
	const std::vector<std::function<Vector(long double, const std::vector<Vector>& variables)>>& funcs_for_each_var);


Vector RK4(const Vector& start_values, long double start, long double step, std::function<Vector(long double, const Vector&)> func);