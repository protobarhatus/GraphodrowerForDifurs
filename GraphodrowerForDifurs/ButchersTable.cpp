#include "ButchersTable.h"
#include <assert.h>
#include "MyLib/MyLib.h"
#include <functional>
ButchersTable::ButchersTable()
{

}
ButchersTable::ButchersTable(const Table& itable, int res_str_amount, bool is_explicit) : amount_of_result_strings(res_str_amount)
{
	this->table = itable;
	this->is_explicit = is_explicit;
}
ButchersTable::number ButchersTable::C(int i) const
{
	assert(i - 1 < this->table.size() - this->amount_of_result_strings && i > 0);
	return this->table[i - 1][0];

}
ButchersTable::number ButchersTable::a(int i, int j) const
{
	assert(i - 1 < this->table.size() - this->amount_of_result_strings && i > 0);
	if (this->is_explicit)
		assert(j < i);
	assert(j >= 0);
	return this->table[i - 1][j];
}
ButchersTable::number ButchersTable::b(int i, int res_string) const
{
	assert(i <= this->table.back().size() && i > 0);
	assert(res_string < this->amount_of_result_strings&& res_string >= 0);
	return this->table[this->table.size() - this->amount_of_result_strings + res_string][i];
}
int ButchersTable::getAmountOfIntermediateSteps() const
{
	return this->table.size() - this->amount_of_result_strings;
}

bool ButchersTable::isExplicit() const
{
	return this->is_explicit;
}


ButchersTable dormanPrince()
{
#define V std::vector<long double>
	std::vector<std::vector<long double>> table = {
		V{0},
		V{1. / 5, 1. / 5},
		V{3. / 10, 3. / 40, 9. / 40},
		V{4. / 5, 44. / 45, -56. / 15, 32. / 9},
		V{8. / 9, 19372. / 6561, -25360. / 2187, 64448. / 6561, -212. / 729},
		V{1, 9017. / 3168, -355. / 33, 46732. / 5247, 49. / 176, -5103. / 18656},
		V{1, 35. / 384, 0, 500. / 1113, 125. / 192, -2187. / 6784, 11. / 84},
		V{nan(""), 35. / 384, 0, 500. / 1113, 125. / 192, -2187. / 6784, 11. / 84, 0},
		V{nan(""), 5179. / 57600, 0, 7571. / 16695, 393. / 640, -92097. / 339200, 187. / 2100, 1. / 40}
	};
	//2 is amount of output lines aka lines where nan("")
	return ButchersTable(table, 2, true);

}
ButchersTable gaussLegendreFourthOrder()
{
#define V std::vector<long double>
	std::vector<std::vector<long double>> table = {
		V{0.2113248654, 0.25, -0.03867513459},
		V{0.7886751346, 0.5386751346, 0.25},
		V{nan(""), 0.5, 0.5},
		V{nan(""), 1.366025404, -0.3660254038}
	};
	return ButchersTable(table, 2, false);
}

ButchersTable tableForRK4()
{
#define V std::vector<long double>
	std::vector<std::vector<long double>> table = {
		V{0, 0, 0, 0, 0},
		V{0.5, 0.5, 0, 0, 0},
		V{0.5, 0, 0.5, 0, 0},
		V{1, 0, 0, 1, 0},
		V{std::nan(""), 1.0 / 6, 1.0 / 3, 1.0 / 3, 1.0 / 6}
	};
	return ButchersTable(table, 1, true);

}

ButchersTable tableForRK6()
{
#define V std::vector<long double>
	std::vector<std::vector<long double>> table = {
		V{0   , 0   , 0   , 0   , 0   , 0    },
		V{0.25        , 0.25       , 0           , 0              , 0               , 0    },
		V{3.0 / 8       , 3.0 / 32     , 9.0 / 32      , 0              ,  0              ,    0    },
		V{12.0 / 13     , 1932.0 / 2197, -7200.0 / 2197, 7296.0 / 2197    , 0               , 0},
		V{1          , 439.0 / 216  , -8.0        ,      3680.0 / 513,      -845.0 / 4104, 0},
		V{1.0 / 2       , -8.0 / 27    , 2           , -3544.0 / 2565   , 1859.0 / 4104     , -11.0 / 40},
		V{std::nan(""), 16.0 / 135   ,    0        ,    6656.0 / 12825,    28561.0 / 56430,    -9.0 / 50,    2.0 / 55}
	};
	return ButchersTable(table, 1, true);
}



ButchersTable LStable3rdOrderImplicitRungeKutta()
{
#define V std::vector<long double>
	std::vector<std::vector<long double>> table = {
		V{0.5, 0.5, 0, 0, 0},
		V{0.66666, 1.0 / 6, 0.5, 0, 0},
		V{0.5, -0.5, 0.5, 0.5, 0},
		V{1, 1.5, -1.5, 0.5, 0.5},
		V{std::nan(""), 1.5, -1.5, 0.5, 0.5}
	};
	return ButchersTable(table, 1, false);
}

ButchersTable inexplicitEuler()
{
	std::vector<std::vector<long double>> table = {
		V{1, 1},
		V{std::nan(""), 1}
	};
	return ButchersTable(table, 1, false);
}
ButchersTable explicitEuler()
{
	std::vector<std::vector<long double>> table = {
		V{0, 0},
		V{std::nan(""), 1}
	};
	return ButchersTable(table, 1, true);
}

#undef V

Vector ButcherRK(const ButchersTable& table, int result_string, Vector d, long double t, long double step, const std::function < Vector(long double, const Vector&)>& f,
	std::vector<std::pair<double, Vector>>* saved_values_at_proceeded_times)
{
	int amount_of_steps = table.getAmountOfIntermediateSteps();
	std::vector<Vector> K(amount_of_steps);
	for (int i = 1; i < amount_of_steps + 1; ++i)
	{
		long double kt = t + table.C(i) * step;
		Vector k = d.null();
		for (int j = 1; j < i; ++j)
		{
			k += table.a(i, j) * K[j - 1];
		}
		K[i - 1] = f(kt, d + k * step);
	}

	if (saved_values_at_proceeded_times != nullptr)
	{
		saved_values_at_proceeded_times->resize(amount_of_steps);
		for (int i = 0; i < amount_of_steps; ++i)
			(*saved_values_at_proceeded_times)[i] = std::pair<double, Vector>{ table.C(i + 1), K[i] };
	}

	Vector change = d.null();
	for (int i = 1; i <= amount_of_steps; ++i)
	{
		change += table.b(i, result_string) * K[i - 1];
	}
	return d + step * change;
}




Vector RK4(const Vector& start_values, long double start, long double step, std::function<Vector(long double, const Vector&)> func)
{
	return ButcherRK(tableForRK4(), 0, start_values, start, step, func);
}


std::vector<Vector> explicitButcherRkForSystem(const ButchersTable& table, const std::vector<Vector>& start_values, long double t, long double step,
	const std::vector<std::function<Vector(long double, const std::vector<Vector>& variables)>>& funcs_for_each_var);


std::vector<Vector> inexplicitButcherRkForSystem(const ButchersTable& table, const std::vector<Vector>& start_values, long double t, long double step,
	const std::vector<std::function<Vector(long double, const std::vector<Vector>& variables)>>& funcs_for_each_var);







std::vector<Vector> ButcherRkForSystem(const ButchersTable& table, const std::vector<Vector>& start_values, long double t, long double step,
	const std::vector<std::function<Vector(long double, const std::vector<Vector>& variables)>>& funcs_for_each_var)
{
	if (table.isExplicit())
		return explicitButcherRkForSystem(table, start_values, t, step, funcs_for_each_var);
	else
		throw "Inexplicit methods are not supported yet\n";
}




std::vector<Vector> explicitButcherRkForSystem(const ButchersTable& table, const std::vector<Vector>& start_values, long double t, long double step,
	const std::vector<std::function<Vector(long double, const std::vector<Vector>& variables)>>& funcs_for_each_var)
{
	int amount_of_variables = start_values.size();
	int amount_of_steps = table.getAmountOfIntermediateSteps();
	//0 index is not used, num from 1
	std::vector<std::vector<Vector>> K(amount_of_steps + 1, std::vector<Vector>(amount_of_variables));

	for (int i = 1; i <= amount_of_steps; ++i)
	{
		std::vector<Vector> arguments = start_values;
		for (int var = 0; var < amount_of_variables; ++var)
		{
			for (int j = 1; j < i; ++j)
			{
				arguments[var] += step * table.a(i, j) * K[j][var];
			}
		}
		long double cur_t = t + table.C(i) * step;

		for (int var = 0; var < amount_of_variables; ++var)
		{
			K[i][var] = funcs_for_each_var[var](cur_t, arguments);
		}

	}

	std::vector<Vector> changes_without_step(amount_of_variables, start_values[0].null());

	for (int var = 0; var < amount_of_variables; ++var)
	{
		for (int i = 1; i <= amount_of_steps; ++i)
			changes_without_step[var] += table.b(i, 0) * K[i][var];
	}
	std::vector<Vector> result = start_values;
	for (int var = 0; var < amount_of_variables; ++var)
	{
		result[var] += step * changes_without_step[var];
	}
	return result;
}



std::vector<Vector> inexplicitButcherRkForSystem(const ButchersTable& table, const std::vector<Vector>& start_values, long double t, long double step,
	const std::vector<std::function<Vector(long double, const std::vector<Vector>& variables)>>& funcs_for_each_var)
{
	//хз оно не работает
	assert(false);
	int amount_of_variables = start_values.size();
	int amount_of_steps = table.getAmountOfIntermediateSteps();
	//0 index is not used, num from 1
	std::vector<std::vector<Vector>> K(amount_of_steps + 1, std::vector<Vector>(amount_of_variables));

	for (int i = 1; i <= amount_of_steps; ++i)
	{
		std::vector<Vector> arguments = start_values;
		for (int var = 0; var < amount_of_variables; ++var)
		{
			for (int j = 1; j < i; ++j)
			{
				arguments[var] += step * table.a(i, j) * K[j][var];
			}
		}
		long double cur_t = t + table.C(i) * step;

		for (int var = 0; var < amount_of_variables; ++var)
		{
			K[i][var] = funcs_for_each_var[var](cur_t, arguments);
		}

	}

	for (int i = 1; i <= amount_of_steps; ++i)
	{
		std::vector<Vector> arguments = start_values;
		for (int var = 0; var < amount_of_variables; ++var)
		{
			for (int j = 1; j <= amount_of_steps; ++j)
			{
				arguments[var] += step * table.a(i, j) * K[j][var];
			}
		}
		long double cur_t = t + table.C(i) * step;

		for (int var = 0; var < amount_of_variables; ++var)
		{
			K[i][var] = funcs_for_each_var[var](cur_t, arguments);
		}

	}

	std::vector<Vector> changes_without_step(amount_of_variables, start_values[0].null());

	for (int var = 0; var < amount_of_variables; ++var)
	{
		for (int i = 1; i <= amount_of_steps; ++i)
			changes_without_step[var] += table.b(i, 0) * K[i][var];
	}
	std::vector<Vector> result = start_values;
	for (int var = 0; var < amount_of_variables; ++var)
	{
		result[var] += step * changes_without_step[var];
	}
	return result;
}



/*std::vector<Vector> explicitButcherRkForSystem(const ButchersTable& table, int result_string, const std::vector<Vector>& start_values, long double t, long double step,
	const std::vector<std::function<Vector(long double, const Vector&, int)>>& funcs_for_each_var,
	std::vector<std::vector<Vector>>& variables_intermediate_values,
	std::vector<std::vector<Vector>>& variables_intermediate_integrals)
{
	int amount_of_steps = table.getAmountOfIntermediateSteps();

	variables_intermediate_values.resize(start_values.size());
	for (int i = 0; i < variables_intermediate_values.size(); ++i)
		variables_intermediate_values[i].resize(amount_of_steps);

	variables_intermediate_integrals.resize(start_values.size());
	for (int i = 0; i < variables_intermediate_integrals.size(); ++i)
		variables_intermediate_integrals[i].resize(amount_of_steps);


	for (int i = 1; i < amount_of_steps + 1; ++i)
	{
		for (int var = 0; var < start_values.size(); ++var)
		{
			long double kt = t + table.C(i) * step;
			Vector k = start_values[var].null();
			for (int j = 1; j < i; ++j)
			{
				k += table.a(i, j) * variables_intermediate_values[var][j - 1];
			}
			variables_intermediate_values[var][i - 1] = funcs_for_each_var[var](kt, start_values[var] + k * step, i);
			if (i - 1 == 0)
				variables_intermediate_integrals[var][i - 1] = start_values[var] + table.C(i) * step * variables_intermediate_values[var][i - 1];
			//variables_intermediate_integrals[var][i - 1] = start_values[var] + step * table.b(i, result_string) * variables_intermediate_values[var][i - 1];
			else
				variables_intermediate_integrals[var][i - 1] = variables_intermediate_integrals[var][i - 1 - 1] + (table.C(i) - table.C(i - 1)) * step * variables_intermediate_values[var][i - 1];
			//variables_intermediate_integrals[var][i - 1] = variables_intermediate_integrals[var][i - 1 - 1] + step * table.b(i, result_string) * variables_intermediate_values[var][i - 1];
		}
	}


	std::vector<Vector> changes(start_values.size());
	for (int i = 0; i < start_values.size(); ++i)
	{
		changes[i] = start_values[i].null();
	}

	for (int var = 0; var < start_values.size(); ++var)
	{
		for (int i = 1; i <= amount_of_steps; ++i)
		{
			changes[var] += table.b(i, result_string) * variables_intermediate_values[var][i - 1];
		}
	}

	std::vector<Vector> res(start_values.size());
	for (int i = 0; i < start_values.size(); ++i)
	{
		res[i] = start_values[i] + step * std::move(changes[i]);
	}
	return res;
}



std::vector<Vector> inexplicitButcherRkForSystem(const ButchersTable& table, int result_string, const std::vector<Vector>& start_values, long double t, long double step,
	const std::vector<std::function<Vector(long double, const Vector&, int)>>& funcs_for_each_var,
	std::vector<std::vector<Vector>>& variables_intermediate_values,
	std::vector<std::vector<Vector>>& variables_intermediate_integrals)
{
	int amount_of_steps = table.getAmountOfIntermediateSteps();

	variables_intermediate_values.resize(start_values.size());
	for (int i = 0; i < variables_intermediate_values.size(); ++i)
		variables_intermediate_values[i].resize(amount_of_steps);

	variables_intermediate_integrals.resize(start_values.size());
	for (int i = 0; i < variables_intermediate_integrals.size(); ++i)
		variables_intermediate_integrals[i].resize(amount_of_steps);


	for (int i = 1; i < amount_of_steps + 1; ++i)
	{
		for (int var = 0; var < start_values.size(); ++var)
		{
			long double kt = t + table.C(i) * step;
			Vector k = start_values[var].null();
			for (int j = 1; j < i; ++j)
			{
				k += table.a(i, j) * variables_intermediate_values[var][j - 1];
			}
			variables_intermediate_values[var][i - 1] = funcs_for_each_var[var](kt, start_values[var] + k * step, i);
			if (i - 1 == 0)
				variables_intermediate_integrals[var][i - 1] = start_values[var] + table.C(i) * step * variables_intermediate_values[var][i - 1];
			//variables_intermediate_integrals[var][i - 1] = start_values[var] + step * table.b(i, result_string) * variables_intermediate_values[var][i - 1];
			else
				variables_intermediate_integrals[var][i - 1] = variables_intermediate_integrals[var][i - 1 - 1] + (table.C(i) - table.C(i - 1)) * step * variables_intermediate_values[var][i - 1];
			//variables_intermediate_integrals[var][i - 1] = variables_intermediate_integrals[var][i - 1 - 1] + step * table.b(i, result_string) * variables_intermediate_values[var][i - 1];
		}
	}
	for (int i = 1; i < amount_of_steps + 1; ++i)
	{
		for (int var = 0; var < start_values.size(); ++var)
		{
			long double kt = t + table.C(i) * step;
			Vector k = start_values[var].null();
			for (int j = 1; j < amount_of_steps + 1; ++j)
			{
				k += table.a(i, j) * variables_intermediate_values[var][j - 1];
			}
			variables_intermediate_values[var][i - 1] = funcs_for_each_var[var](kt, start_values[var] + k * step, i);
			if (i - 1 == 0)
				variables_intermediate_integrals[var][i - 1] = start_values[var] + table.C(i) * step * variables_intermediate_values[var][i - 1];
			//variables_intermediate_integrals[var][i - 1] = start_values[var] + step * table.b(i, result_string) * variables_intermediate_values[var][i - 1];
			else
				variables_intermediate_integrals[var][i - 1] = variables_intermediate_integrals[var][i - 1 - 1] + (table.C(i) - table.C(i - 1)) * step * variables_intermediate_values[var][i - 1];
			//variables_intermediate_integrals[var][i - 1] = variables_intermediate_integrals[var][i - 1 - 1] + step * table.b(i, result_string) * variables_intermediate_values[var][i - 1];
		}
	}

	std::vector<Vector> changes(start_values.size());
	for (int i = 0; i < start_values.size(); ++i)
	{
		changes[i] = start_values[i].null();
	}

	for (int var = 0; var < start_values.size(); ++var)
	{
		for (int i = 1; i <= amount_of_steps; ++i)
		{
			changes[var] += table.b(i, result_string) * variables_intermediate_values[var][i - 1];
		}
	}

	std::vector<Vector> res(start_values.size());
	for (int i = 0; i < start_values.size(); ++i)
	{
		res[i] = start_values[i] + step * std::move(changes[i]);
	}
	return res;
}*/
