#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <utility> 
#include <initializer_list> 
#include <set> 
#include "Formula.h"

using namespace std;

class Solve
{

public:
	static double linearFunc(vector<double> factors);
	static double linearFunc(Formula formula);

	static vector<double> quadraticFunc(vector<double> factors);
	static vector<double> quadraticFunc(Formula formula);

	static set<double> cubicFuncAndHigher(Formula formula);

	static set<double> substitutionToQuadraticFunc(Formula formula);

	static set<double> theoremOfBezout(Formula formula);


	static set<int> getDividers(double numberToDivide);

	static set<double> getPotentialRationalRoots(Formula formula);

	static set<double> commonPartOfCollections(vector<vector<double>> collections);
	static set<double> commonPartOfCollections(initializer_list<vector<double>> collections);

	static pair<Formula, Formula> pullOutBeforeBracket(Formula formula);
	static pair<vector<double>, vector<double>> pullOutBeforeBracket(vector<double> factors);

	static Formula addingFormulas(Formula formula1, Formula formula2);
	static vector<double> addingFormulas(vector<double> factors1, vector<double> factors2);

	static Formula subtractingFormulas(Formula formula1, Formula formula2);
	static vector<double> subtractingFormulas(vector<double> factors1, vector<double> factors2);

	static Formula multiplicationFormulas(Formula formula1, Formula formula2);
	static vector<double> multiplicationFormulas(vector<double> factors1, vector<double> factors2);

	// Zwraca jedn¹ parê równañ:
	// 1. Pierwsze równanie jest wielomianem, który jest wynikiem dzielenia
	// 2. Drugie równanie jest reszt¹ z dzielenia, równie¿ zwracane jako równanie 
	static pair<Formula, Formula> divisionFormulas(Formula formula1, Formula formula2);
	static pair<vector<double>, vector<double>> divisionFormulas(vector<double> factors1, vector<double> factors2);


	static bool isLinear(Formula formula);
	static bool isLinear(vector<double> factors);

	static bool isQuadratic(Formula formula);
	static bool isQuadratic(vector<double> factors);
	
	static bool isCubicOrHigher(Formula formula);
	static bool isCubicOrHigher(vector<double> factors);
};

