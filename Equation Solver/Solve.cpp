#include "Solve.h"

float Solve::linearFunc(vector<float> factors)
{
	if(isLinear(factors))
	{ 
		float a = factors[1];
		float b = factors[0];

		if (a < 0)
		{
			a *= -1;
			b *= -1;
		}

		float x = ((-1 * b) / a);

		return x;
	}
	else
	{
		wcout << L"ALERT (Funkcja liniowa): Przekazano funkcjê inn¹ ni¿ liniow¹!!!" << endl;
		throw runtime_error("ALERT (Funkcja liniowa): Przekazano funkcjê inn¹ ni¿ liniowa!!!");

		return 0.0f;
	}
}

float Solve::linearFunc(Formula formula)
{
	return linearFunc(formula.getAllFactors());
}

vector<float> Solve::quadraticFunc(vector<float> factors)
{
	if (isQuadratic(factors))
	{
		float a = factors[2];
		float b = factors[1];
		float c = factors[0];

		float delta = pow(b, 2) - 4*a*c; 

		if(delta < 0) return vector<float>();
		else if (delta == 0)
		{
			float x0 = ((-b) / (2 * a));

			vector<float> root;
			root.push_back(x0);

			return root;
		}
		else if (delta > 0)
		{
			float deltaSquareRoot = sqrt(delta);

			float x1 = ((-b - deltaSquareRoot) / (2 * a));
			float x2 = ((-b + deltaSquareRoot) / (2 * a));

			vector<float> roots;
			roots.push_back(x1);
			roots.push_back(x2);

			return roots;
		}
	}
	else
	{
		wcout << L"ALERT (Funkcja kwadratowa): Przekazano funkcjê inn¹ ni¿ kwadratowa!!!" << endl;
		throw runtime_error("ALERT (Funkcja kwadratowa): Przekazano funkcjê inn¹ ni¿ kwadratowa!!!");

		return vector<float>();
	}
}

vector<float> Solve::quadraticFunc(Formula formula)
{
	return quadraticFunc(formula.getAllFactors());
}

set<float> Solve::cubicFuncAndHigher(Formula& formula)
{
	set<float> roots;
	
	auto newFormulas = pullOutBeforeBracket(formula);

	if(newFormulas.first.isEmpty() == true) roots = theoremOfBezout(formula);
	else
	{
		// Jeœli metoda 'pullOutBeforeBracket()' zdo³a³a coœ wyci¹gn¹æ przed nawias,
		// to oznacza to, ¿e znaleziono kolejny pierwiastek, który jest równy 0
		roots.insert(0);

		auto nextRoots = cubicFuncAndHigher(newFormulas.second);

		for (auto i : nextRoots) roots.insert(i);
	}

	return roots;
}

set<float> Solve::substitutionToQuadraticFunc(Formula formula)
{
	formula.removeZeroFactorsFromTheBeginning();

	if ((formula.isEmpty() == true) || 
		(formula.getDegree() < 3) || 
		(formula.getDegree() % 2 != 0)) return set<float>();

	if (formula.getNumberOfNonzeroFactors() <= 3)
	{
		// Sprawdzenie czy "œrodkowy" wspó³czynnik (wspó³czynnik niewiadomej podniesionej do potêgi wy¿szej ni¿ 0
		//  i ni¿szej ni¿ najwy¿sza potêga), o ile istnieje, stoi przy niewiadomej podniesionej do potêgi równej po³owie
		// wyk³adnika najwy¿szej potêgi.
		// To jest warunkiem skorzystania z mo¿liwoœci takiego przyk³adowego zapisu: t=x^2
		for (int i = 1; i < formula.getDegree(); i++)
		{
			if (formula.getFactor(i) == 0) continue;
			else
			{
				if (formula.getFactor(i) != formula.getDegree() / 2) return set<float>();

				break;
			}
		}
	}

	Formula modifiedFormula;

	for (auto i : formula.getAllFactors()) modifiedFormula.setFactor();


	return set<float>();
}

set<float> Solve::theoremOfBezout(Formula &formula)
{
	if (formula.getFactor(0) == 0) return set<float>();

	/////////////////////////////////////////////////////////////////////////////////

	auto potentialRationalRoots = getPotentialRationalRoots(formula);

	float firstRoot;
	for (int i = 0; i < potentialRationalRoots.size(); i++)
	{
		if (formula.getY(potentialRationalRoots[i]) == 0)
		{
			firstRoot = potentialRationalRoots[i];
			break;
		}
	}

	set<float> roots;
	roots.insert(firstRoot);

	Formula subtrahend(vector<float>{-firstRoot, 1});
	Formula divisionResult = divisionFormulas(formula, subtrahend).first;

	if (isLinear(divisionResult)) roots.insert(-divisionResult.getFactor(0));
	else if (isQuadratic(divisionResult))
	{
		auto quadraticFunctionRoots = quadraticFunc(divisionResult);
		for (auto i : quadraticFunctionRoots) roots.insert(i);
	}
	else if (divisionResult.getDegree() >= 3)
	{
		auto nextRoots = theoremOfBezout(divisionResult);
		for (auto i : nextRoots)
		{
			wcout << i << endl;
			roots.insert(i);
		}
	}

	return roots;
}

vector<int> Solve::getDividers(float numberToDivide)
{
	vector<int> dividers;
	
	if (numberToDivide != 0)
	{
		dividers.push_back(1);
		dividers.push_back(-1);
	}

	if (numberToDivide > 1)
	{
		dividers.push_back(numberToDivide);
		dividers.push_back(-numberToDivide);
	}

	for (int i = 2; i <= numberToDivide / 2; i++) 
	{
		if (fmod(numberToDivide, i) == 0)
		{
			dividers.push_back(i);
			dividers.push_back(-i);
		}
	}

	return dividers;
}

vector<float> Solve::getPotentialRationalRoots(Formula formula)
{
	float zeroFactor = formula.getFactor(0);
	float lastFactor = formula.getFactor(formula.getDegree());

	auto p = getDividers(zeroFactor);
	auto q = getDividers(lastFactor);

	vector<float> potentialRationalRoots;

	for (int i = 0; i < p.size(); i++)
	{
		for (int j = 0; j < q.size(); j++)
		{
			float pq = (float)p[i] / (float)q[j];

			auto iterator = find(potentialRationalRoots.begin(), potentialRationalRoots.end(), pq);

			if (iterator == potentialRationalRoots.end()) potentialRationalRoots.push_back(pq);
		}
	}

	return potentialRationalRoots;
}

set<float> Solve::commonPartOfCollections(vector<vector<float>> collections)
{
	if(collections.size() < 1) return set<float>();

	set<float> commonPart;

	for (auto i : collections[0])
	{
		float numberToCheck = i;
		bool isCommon = true;

		for (auto j : collections)
		{
			auto iterator = find(j.begin(), j.end(), numberToCheck);

			if (iterator == j.end())
			{
				isCommon = false;
				break;
			}
		}

		if (isCommon == true) commonPart.insert(numberToCheck);
	}

	return commonPart;
}

set<float> Solve::commonPartOfCollections(initializer_list<vector<float>> collections)
{
	vector<vector<float>> vector_collections(collections.begin(), collections.end());

	auto commonPart = commonPartOfCollections(vector_collections);

	return commonPart;
}

pair<Formula, Formula> Solve::pullOutBeforeBracket(Formula formula)
{
	if(formula.getDegree() < 1) return pair<Formula, Formula>(Formula(), formula);

	vector<vector<float>> dividersOfAllFactors;

	for (auto i : formula.getAllFactors())
	{
		if (i == 0) continue;

		auto int_dividers = getDividers(i);
		vector<float> float_dividers(int_dividers.begin(), int_dividers.end());

		dividersOfAllFactors.push_back(float_dividers);
	}

	auto factorValueDividers = commonPartOfCollections(dividersOfAllFactors);

	// Wybieramy najwy¿szy wspólny dzielnik wspó³czynników wielomianu:
	// S¹ one ju¿ posortowane w kontenerze std::set<>, co powoduje, ¿e wystarczy
	// wybraæ ostatni element, który z automatu jest najwy¿szym
	float highestFactorValueDivider = *factorValueDividers.rbegin(); // *obj.rbegin() zwraca ostatni element

	int factorNumber = 0;

	for (int i = 0; i <= formula.getDegree(); i++)
	{
		if (formula.getFactor(i) != 0)
		{
			factorNumber = i;
			break;
		}
	}

	Formula pulledOutBeforeBracket;
	pulledOutBeforeBracket.setDegree(factorNumber, true, highestFactorValueDivider);

	Formula formulaInBracket = divisionFormulas(formula, pulledOutBeforeBracket).first;

	return pair<Formula, Formula>(pulledOutBeforeBracket, formulaInBracket);
}

pair<vector<float>, vector<float>> Solve::pullOutBeforeBracket(vector<float> factors)
{
	auto result = pullOutBeforeBracket(Formula(factors));

	return pair<vector<float>, vector<float>>(result.first.getAllFactors(), result.second.getAllFactors());
}

Formula Solve::addingFormulas(Formula formula1, Formula formula2)
{
	if (formula1.getDegree() > formula2.getDegree()) formula2.setFactor(formula1.getDegree(), 0);
	else if (formula1.getDegree() < formula2.getDegree()) formula1.setFactor(formula2.getDegree(), 0);

	int initialDegree = formula1.getDegree();

	Formula newFormula(initialDegree);

	for (int i = 0; i <= initialDegree; i++)
	{
		float formula1x = formula1.getFactor(i);
		float formula2x = formula2.getFactor(i);
			
		newFormula.setFactor(i, formula1x + formula2x);
	}

	return newFormula;
}

vector<float> Solve::addingFormulas(vector<float> factors1, vector<float> factors2)
{
	Formula outputFormula = addingFormulas(Formula(factors1), Formula(factors2));
	vector<float> outputFactors = outputFormula.getAllFactors();

	return outputFactors;
}

Formula Solve::subtractingFormulas(Formula formula1, Formula formula2)
{
	for (int i = 0; i <= formula2.getDegree(); i++) 
		formula2.setFactor(i, -1 * formula2.getFactor(i));

	Formula newFormula = addingFormulas(formula1, formula2);

	return newFormula;
}

vector<float> Solve::subtractingFormulas(vector<float> factors1, vector<float> factors2)
{
	Formula outputFormula = subtractingFormulas(Formula(factors1), Formula(factors2));
	vector<float> outputFactors = outputFormula.getAllFactors();

	return outputFactors;
}

Formula Solve::multiplicationFormulas(Formula formula1, Formula formula2)
{
	formula1.removeZeroFactorsFromTheBeginning();
	formula2.removeZeroFactorsFromTheBeginning();

	Formula newFormula;
	int degree1 = formula1.getDegree();
	int degree2 = formula2.getDegree();
	newFormula.setDegree(degree1 + degree2, true, 0);

	for (int i = 0; i <= formula1.getDegree(); i++)
	{
		if (formula1.getFactor(i) == 0) continue;

		for (int j = 0; j <= formula2.getDegree(); j++)
		{
			if (formula2.getFactor(j) == 0) continue;

			// Numer potêgi, do której bêdzie podniesiony x
			int factorNumber = i + j;
			float factorValue = formula1.getFactor(i) * formula2.getFactor(j);

			newFormula.addToFactor(factorNumber, factorValue);
		}
	}

	return newFormula;
}

vector<float> Solve::multiplicationFormulas(vector<float> factors1, vector<float> factors2)
{
	Formula outputFormula = multiplicationFormulas(Formula(factors1), Formula(factors2));
	vector<float> outputFactors = outputFormula.getAllFactors();

	return outputFactors;
}

pair<Formula, Formula> Solve::divisionFormulas(Formula formula1, Formula formula2)
{
	Formula newFormula;

	Formula formula1Duplicate = formula1;

	for (int i = formula1.getDegree(); i > formula2.getDegree() - 1; i--)
	{
		// 
		int factorNumberOfNewFactor = i - formula2.getDegree();
		int a = formula2.getDegree();
		float factorValueOfNewFactor = formula1Duplicate.getFactor(i) / formula2.getFactor(a);

		newFormula.setFactor(factorNumberOfNewFactor, factorValueOfNewFactor);

		// Wielomian zawieraj¹cy pojedynczy a(n)*x^n (nazwijmy to wspó³czynnikiem), który zosta³ przed chwil¹
		// wyliczony i przez, który zaraz zostanie przemno¿ony wielomian-dzielnik (formula2),
		// w celu stworzenia wielomianu-oddemnika, który zostanie odjêty od duplikatu pierwotnego wielomianu (formula1Duplicate)
		// czego skutkiem bêdzie zniwielowanie ixa z najwy¿sz¹ potêg¹
		Formula newCalculatedFactor(factorNumberOfNewFactor);
		newCalculatedFactor.setFactor(factorNumberOfNewFactor, factorValueOfNewFactor);

		// Wielomian, który bêdzie ka¿dorazowo odejmowany (odjemnik) od ci¹glê modyfikowanego duplikatu
		// pierwotnego wielomianu - formula1Duplicate (odjemna), tak jak ma to miejsce podczas dzielenia wielomianów "pod kresk¹"
		Formula subtrahend = multiplicationFormulas(newCalculatedFactor, formula2);

		formula1Duplicate = subtractingFormulas(formula1Duplicate, subtrahend);
	}

	if (formula1Duplicate.isEmpty())	return pair<Formula, Formula>(newFormula, Formula());
	else
	{
		formula1Duplicate.removeZeroFactorsFromTheBeginning();
		return pair<Formula, Formula>(newFormula, formula1Duplicate);
	}
}

pair<vector<float>, vector<float>> Solve::divisionFormulas(vector<float> factors1, vector<float> factors2)
{
	auto divisionResult = divisionFormulas(Formula(factors1), Formula(factors2));
	vector<float> productForm = divisionResult.first.getAllFactors();
	vector<float> rest = divisionResult.second.getAllFactors();

	return pair<vector<float>, vector<float>>(productForm, rest);
}

bool Solve::isLinear(vector<float> factors)
{
	if (factors.size() == 2 && factors[1] != 0) return true;
	else return false;
}

bool Solve::isLinear(Formula formula)
{
	vector<float> factors = formula.getAllFactors();

	return isLinear(factors);
}

bool Solve::isQuadratic(vector<float> factors)
{
	if(factors.size() == 3 && factors[2] != 0) return true;
	else return false;
}

bool Solve::isQuadratic(Formula formula)
{
	vector<float> factors = formula.getAllFactors();

	return isQuadratic(factors);
}

bool Solve::isCubicOrHigher(Formula formula)
{
	if (formula.getDegree() >= 3 && formula.getFactor(formula.getDegree()) != 0) return true;
	else return false;
}

bool Solve::isCubicOrHigher(vector<float> factors)
{
	return isCubicOrHigher(Formula(factors));
}