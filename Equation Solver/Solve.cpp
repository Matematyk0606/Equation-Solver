#include "Solve.h"


double Solve::linearFunc(vector<double> factors)
{
	if(isLinear(factors))
	{ 
		double a = factors[1];
		double b = factors[0];

		if (a < 0)
		{
			a *= -1;
			b *= -1;
		}

		double x = ((-1 * b) / a);

		return x;
	}
	else
	{
		wcout << L"ALERT (Funkcja liniowa): Przekazano funkcję inną niż liniową!!!" << endl;
		throw runtime_error("ALERT (Funkcja liniowa): Przekazano funkcję inną niż liniowa!!!");

		return 0.0f;
	}
}

double Solve::linearFunc(Formula formula)
{
	return linearFunc(formula.getAllFactors());
}

vector<double> Solve::quadraticFunc(vector<double> factors)
{
	if (isQuadratic(factors))
	{
		double a = factors[2];
		double b = factors[1];
		double c = factors[0];

		double delta = pow(b, 2) - 4*a*c; 

		if(delta < 0) return vector<double>();
		else if (delta == 0)
		{
			double x0 = ((-b) / (2 * a));

			vector<double> root;
			root.push_back(x0);

			return root;
		}
		else if (delta > 0)
		{
			double deltaSquareRoot = sqrt(delta);

			double x1 = ((-b - deltaSquareRoot) / (2 * a));
			double x2 = ((-b + deltaSquareRoot) / (2 * a));

			vector<double> roots;
			roots.push_back(x1);
			roots.push_back(x2);

			return roots;
		}
	}
	else
	{
		wcout << L"ALERT (Funkcja kwadratowa): Przekazano funkcję inną niż kwadratowa!!!" << endl;
		throw runtime_error("ALERT (Funkcja kwadratowa): Przekazano funkcję inną niż kwadratowa!!!");

		return vector<double>();
	}
}

vector<double> Solve::quadraticFunc(Formula formula)
{
	return quadraticFunc(formula.getAllFactors());
}

set<double> Solve::cubicFuncAndHigher(Formula formula)
{
	set<double> roots;

	auto newFormulas = pullOutBeforeBracket(formula);

	if (newFormulas.first.isEmpty() == true)
	{
		roots = substitutionToQuadraticFunc(formula);

		if (roots.size() > 0) return roots;

		roots = theoremOfBezout(formula);
	}
	else if((newFormulas.first.isEmpty() == false) && (newFormulas.first.getDegree() > 0))
	{
		// Jeśli metoda 'pullOutBeforeBracket()' zdołała coś wyciągnąć przed nawias,
		// to oznacza to, że znaleziono kolejny pierwiastek, który jest równy 0
		roots.insert(0);

		wcout << newFormulas.second.convertToText().c_str() << endl;

		if (isLinear(newFormulas.second))
		{
			roots.insert(linearFunc(newFormulas.second));

			return roots;
		}
		else if (isQuadratic(newFormulas.second))
		{
			auto quadraticFunctionRoots = quadraticFunc(newFormulas.second);
			for (auto i : quadraticFunctionRoots) roots.insert(i);

			return roots;
		}

		auto nextRoots = substitutionToQuadraticFunc(newFormulas.second);
		if (nextRoots.size() > 0)
		{
			for (auto i : nextRoots) roots.insert(i);
			return roots;
		}

		nextRoots = theoremOfBezout(newFormulas.second);
		for (auto i : nextRoots) roots.insert(i);
	}

	return roots;
}

set<double> Solve::substitutionToQuadraticFunc(Formula formula)
{
	formula.removeZeroFactorsFromTheBeginning();

	// Wstępny filtr, dopuszczający tylko wielomiany wielomiany, których stopień jest 'n' razy większy od 2
	if ((formula.isEmpty() == true) || 
		(formula.getDegree() < 4) || 
		(formula.getDegree() % 2 != 0) ||
		(formula.getNumberOfNonzeroFactors() > 3)) return set<double>();

	// Sprawdzenie czy przesłany wielomian da się zapisać w psotaci: a*t^(n*2) + b*t^n + c
	if (formula.getNumberOfNonzeroFactors() <= 3)
	{
		// Sprawdzenie czy "środkowy" współczynnik (współczynnik niewiadomej podniesionej do potęgi wyższej niż 0
		// i niższej niż najwyższa potęga), o ile istnieje, stoi przy niewiadomej podniesionej do potęgi równej połowie
		// wykładnika najwyższej potęgi.
		// To jest warunkiem skorzystania z możliwości takiego przykładowego zapisu: t=x^2
		for (int i = 1; i < formula.getDegree(); i++)
		{
			if (formula.getFactor(i) == 0) continue;
			else
			{
				if (((i == formula.getDegree()/2) && (formula.getFactor(i) == 0)) ||
					((i != formula.getDegree() / 2) && (formula.getFactor(i) != 0))) return set<double>();

				break;
			}
		}
	}

	// Określa ile razy większy jest najwyższy wykładnik potęgi przy niewiadomej 'x' 
	// z przesłanego wielomianu od potęgi 2. funkcji kwadratowej, do której sprowadzamy przesłane równanie
	int howManyTimesBiggerTheHighestExponentThanTwo = formula.getDegree() / 2;

	// Stworzenie wielomianu kwadratowego na podstawie przesłanego wielomianu
	Formula modifiedFormula;

	modifiedFormula.setFactor(0, formula.getFactor(0));
	modifiedFormula.setFactor(1, formula.getFactor(formula.getDegree() / 2));
	modifiedFormula.setFactor(2, formula.getFactor(formula.getDegree()));

	auto rootsFromSolvePreparedFormula = quadraticFunc(modifiedFormula);

	set<double> roots;

	// Obliczanie pierwiastków n-tego stopnia (mam na myśli działanie matematyczne: √) 
	// (n-ty stopień równy jest połowie stopnia całego przesłanego wielomianu)
	// z uzyskanych miejsc zerowych funkcji kwadratowej, którą przed chwilą utworzyliśmy
	for (auto i : rootsFromSolvePreparedFormula)
	{
		// Jeśli wspomiane wyżej 'n' (czyli połowa stopnia przesłanego wielomianu) jest parzyste, 
		// to ostateczne obliczane przez nas rozwiązania wielomianu mogą być tylko wynikiem pierwiastkowana 
		// liczb dodatnich (bo np.: x^2!=-4 ), a skoro przykładowe: x^2=4 to x=2 lub x=-2, to otrzymujemy 
		// zawsze dwa wyniki pierwiastkowania ('a' oraz '-a')
		if ((howManyTimesBiggerTheHighestExponentThanTwo % 2 == 0) && (i >= 0))
		{
			roots.insert(-pow(i, (1.0 / ((double)formula.getDegree() / 2.0))));
			roots.insert(pow(i, (1.0 / ((double)formula.getDegree() / 2.0))));
		}

		// Jeśli 'n' jest nieparzyste, to ostateczne obliczane przez nas rozwiązania wielomianu mogą być 
		// wynikiem pierwiastkowana liczb dodatnich i ujemnych (bo np.: x^3 może być równe -8), 
		// a skoro przykładowe: x^3=-8 to x=-2, a zatem otrzymujemy tylko jeden wynik pierwiastkowania
		else if (howManyTimesBiggerTheHighestExponentThanTwo % 2 != 0)
		{
			if (i >= 0) roots.insert(pow(i, (1.0 / ((double)formula.getDegree() / 2.0))));
			else if (i < 0) roots.insert(-pow(-i, (1.0 / ((double)formula.getDegree() / 2.0)) ));
		}
	}

	return roots;
}

set<double> Solve::theoremOfBezout(Formula formula)
{
	if (formula.getFactor(0) == 0) return set<double>();

	/////////////////////////////////////////////////////////////////////////////////

	auto potentialRationalRoots = getPotentialRationalRoots(formula);

	double firstRoot = 0;
	for (auto i : potentialRationalRoots)
	{
		if (formula.getY(i) == 0)
		{
			firstRoot = i;
			break;
		}
	}

	// Nie można dzielić przez 0, dlatego zero nie mogło żadnym z potencjalnych znalezionych pierwiastków,
	// a co za tym idzie dla tego wielomianu nie jest spełnione twierdznie o pierwiastkach rzeczywistych
	// i dalsze wykonywanie tej metody przyniesie tylko błędy
	if(firstRoot == 0) return set<double>();

	/////////////////////////////////////////////////////////////////////////////////

	set<double> roots;
	roots.insert(firstRoot);

	Formula subtrahend(vector<double>{-firstRoot, 1});
	Formula divisionResult = divisionFormulas(formula, subtrahend).first;

	if (isLinear(divisionResult)) roots.insert(-divisionResult.getFactor(0));
	else if (isQuadratic(divisionResult))
	{
		auto quadraticFunctionRoots = quadraticFunc(divisionResult);
		for (auto i : quadraticFunctionRoots) roots.insert(i);
	}
	else if (divisionResult.getDegree() >= 3)
	{
		auto nextRoots = cubicFuncAndHigher(divisionResult);
		for (auto i : nextRoots) roots.insert(i);
	}

	return roots;
}

set<int> Solve::getDividers(double numberToDivide)
{
	set<int> dividers;
	
	if (numberToDivide == 0) return set<int>();

	dividers.insert(1);
	dividers.insert(-1);

	dividers.insert(numberToDivide);
	dividers.insert(-numberToDivide);

	if (numberToDivide < 0) numberToDivide *= -1;

	for (int i = 2; i <= numberToDivide / 2; i++) 
	{
		if (fmod(numberToDivide, i) == 0)
		{
			dividers.insert(i);
			dividers.insert(-i);
		}
	}

	return dividers;
}

set<double> Solve::getPotentialRationalRoots(Formula formula)
{
	double zeroFactor = formula.getFactor(0);
	double lastFactor = formula.getFactor(formula.getDegree());

	auto p = getDividers(zeroFactor);
	auto q = getDividers(lastFactor);

	set<double> potentialRationalRoots;

	for (auto i : p)
		for (auto j : q)
			potentialRationalRoots.insert(((double)i / (double)j));

	return potentialRationalRoots;
}

set<double> Solve::commonPartOfCollections(vector<vector<double>> collections)
{
	if(collections.size() < 1) return set<double>();

	set<double> commonPart;

	for (auto i : collections[0])
	{
		double numberToCheck = i;
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

set<double> Solve::commonPartOfCollections(initializer_list<vector<double>> collections)
{
	vector<vector<double>> vector_collections(collections.begin(), collections.end());

	auto commonPart = commonPartOfCollections(vector_collections);

	return commonPart;
}

pair<Formula, Formula> Solve::pullOutBeforeBracket(Formula formula)
{
	if(formula.getDegree() < 1) return pair<Formula, Formula>(Formula(), formula);

	vector<vector<double>> dividersOfAllFactors;

	for (auto i : formula.getAllFactors())
	{
		if (i == 0) continue;

		auto int_dividers = getDividers(i);
		vector<double> double_dividers(int_dividers.begin(), int_dividers.end());

		dividersOfAllFactors.push_back(double_dividers);
	}

	auto factorValueDividers = commonPartOfCollections(dividersOfAllFactors);

	// Wybieramy najwyższy wspólny dzielnik współczynników wielomianu:
	// Są one już posortowane w kontenerze std::set<>, co powoduje, że wystarczy
	// wybrać ostatni element, który z automatu jest najwyższym
	double highestFactorValueDivider = *factorValueDividers.rbegin(); // *obj.rbegin() zwraca ostatni element

	int factorNumber = 0;

	for (int i = 0; i <= formula.getDegree(); i++)
	{
		if (formula.getFactor(i) != 0)
		{
			factorNumber = i;
			break;
		}
	}

	if(factorNumber == 0 && highestFactorValueDivider == 1) return pair<Formula, Formula>(Formula(), formula);

	Formula pulledOutBeforeBracket;
	pulledOutBeforeBracket.setDegree(factorNumber, true, highestFactorValueDivider);

	Formula formulaInBracket = divisionFormulas(formula, pulledOutBeforeBracket).first;

	return pair<Formula, Formula>(pulledOutBeforeBracket, formulaInBracket);
}

pair<vector<double>, vector<double>> Solve::pullOutBeforeBracket(vector<double> factors)
{
	auto result = pullOutBeforeBracket(Formula(factors));

	return pair<vector<double>, vector<double>>(result.first.getAllFactors(), result.second.getAllFactors());
}

Formula Solve::addingFormulas(Formula formula1, Formula formula2)
{
	if (formula1.getDegree() > formula2.getDegree()) formula2.setFactor(formula1.getDegree(), 0);
	else if (formula1.getDegree() < formula2.getDegree()) formula1.setFactor(formula2.getDegree(), 0);

	int initialDegree = formula1.getDegree();

	Formula newFormula(initialDegree);

	for (int i = 0; i <= initialDegree; i++)
	{
		double formula1x = formula1.getFactor(i);
		double formula2x = formula2.getFactor(i);
			
		newFormula.setFactor(i, formula1x + formula2x);
	}

	return newFormula;
}

vector<double> Solve::addingFormulas(vector<double> factors1, vector<double> factors2)
{
	Formula outputFormula = addingFormulas(Formula(factors1), Formula(factors2));
	vector<double> outputFactors = outputFormula.getAllFactors();

	return outputFactors;
}

Formula Solve::subtractingFormulas(Formula formula1, Formula formula2)
{
	for (int i = 0; i <= formula2.getDegree(); i++) 
		formula2.setFactor(i, -1 * formula2.getFactor(i));

	Formula newFormula = addingFormulas(formula1, formula2);

	return newFormula;
}

vector<double> Solve::subtractingFormulas(vector<double> factors1, vector<double> factors2)
{
	Formula outputFormula = subtractingFormulas(Formula(factors1), Formula(factors2));
	vector<double> outputFactors = outputFormula.getAllFactors();

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

			// Numer potęgi, do której będzie podniesiony x
			int factorNumber = i + j;
			double factorValue = formula1.getFactor(i) * formula2.getFactor(j);

			newFormula.addToFactor(factorNumber, factorValue);
		}
	}

	return newFormula;
}

vector<double> Solve::multiplicationFormulas(vector<double> factors1, vector<double> factors2)
{
	Formula outputFormula = multiplicationFormulas(Formula(factors1), Formula(factors2));
	vector<double> outputFactors = outputFormula.getAllFactors();

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
		double factorValueOfNewFactor = formula1Duplicate.getFactor(i) / formula2.getFactor(a);

		newFormula.setFactor(factorNumberOfNewFactor, factorValueOfNewFactor);

		// Wielomian zawierający pojedynczy a(n)*x^n (nazwijmy to współczynnikiem), który został przed chwilą
		// wyliczony i przez, który zaraz zostanie przemnożony wielomian-dzielnik (formula2),
		// w celu stworzenia wielomianu-oddemnika, który zostanie odjęty od duplikatu pierwotnego wielomianu (formula1Duplicate)
		// czego skutkiem będzie zniwielowanie ixa z najwyższą potęgą
		Formula newCalculatedFactor(factorNumberOfNewFactor);
		newCalculatedFactor.setFactor(factorNumberOfNewFactor, factorValueOfNewFactor);

		// Wielomian, który będzie każdorazowo odejmowany (odjemnik) od ciąglę modyfikowanego duplikatu
		// pierwotnego wielomianu - formula1Duplicate (odjemna), tak jak ma to miejsce podczas dzielenia wielomianów "pod kreską"
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

pair<vector<double>, vector<double>> Solve::divisionFormulas(vector<double> factors1, vector<double> factors2)
{
	auto divisionResult = divisionFormulas(Formula(factors1), Formula(factors2));
	vector<double> productForm = divisionResult.first.getAllFactors();
	vector<double> rest = divisionResult.second.getAllFactors();

	return pair<vector<double>, vector<double>>(productForm, rest);
}

bool Solve::isLinear(vector<double> factors)
{
	if (factors.size() == 2 && factors[1] != 0) return true;
	else return false;
}

bool Solve::isLinear(Formula formula)
{
	vector<double> factors = formula.getAllFactors();

	return isLinear(factors);
}

bool Solve::isQuadratic(vector<double> factors)
{
	if(factors.size() == 3 && factors[2] != 0) return true;
	else return false;
}

bool Solve::isQuadratic(Formula formula)
{
	vector<double> factors = formula.getAllFactors();

	return isQuadratic(factors);
}

bool Solve::isCubicOrHigher(Formula formula)
{
	if (formula.getDegree() >= 3 && formula.getFactor(formula.getDegree()) != 0) return true;
	else return false;
}

bool Solve::isCubicOrHigher(vector<double> factors)
{
	return isCubicOrHigher(Formula(factors));
}