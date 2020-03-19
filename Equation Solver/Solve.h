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
	static float linearFunc(vector<float> factors);
	static float linearFunc(Formula formula);

	static vector<float> quadraticFunc(vector<float> factors);
	static vector<float> quadraticFunc(Formula formula);

	static set<float> cubicFuncAndHigher(Formula &formula);

	static set<float> substitutionToQuadraticFunc(Formula formula);

	static set<float> theoremOfBezout(Formula &formula);


	static vector<int> getDividers(float numberToDivide);

	static vector<float> getPotentialRationalRoots(Formula formula);

	static set<float> commonPartOfCollections(vector<vector<float>> collections);
	static set<float> commonPartOfCollections(initializer_list<vector<float>> collections);

	static pair<Formula, Formula> pullOutBeforeBracket(Formula formula);
	static pair<vector<float>, vector<float>> pullOutBeforeBracket(vector<float> factors);

	static Formula addingFormulas(Formula formula1, Formula formula2);
	static vector<float> addingFormulas(vector<float> factors1, vector<float> factors2);

	static Formula subtractingFormulas(Formula formula1, Formula formula2);
	static vector<float> subtractingFormulas(vector<float> factors1, vector<float> factors2);

	static Formula multiplicationFormulas(Formula formula1, Formula formula2);
	static vector<float> multiplicationFormulas(vector<float> factors1, vector<float> factors2);

	// Zwraca jedn� par� r�wna�:
	// 1. Pierwsze r�wnanie jest wielomianem, kt�ry jest wynikiem dzielenia
	// 2. Drugie r�wnanie jest reszt� z dzielenia, r�wnie� zwracane jako r�wnanie 
	static pair<Formula, Formula> divisionFormulas(Formula formula1, Formula formula2);
	static pair<vector<float>, vector<float>> divisionFormulas(vector<float> factors1, vector<float> factors2);


	static bool isLinear(Formula formula);
	static bool isLinear(vector<float> factors);

	static bool isQuadratic(Formula formula);
	static bool isQuadratic(vector<float> factors);
	
	static bool isCubicOrHigher(Formula formula);
	static bool isCubicOrHigher(vector<float> factors);
};

