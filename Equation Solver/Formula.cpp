#include "Formula.h"

Formula::Formula() 
{
	factors = vector<float>(1);

	factors[0] = 0;
}

Formula::Formula(int degree)
{
	factors = vector<float>(degree + 1);
	
	factors[degree] = 1;
}

Formula::Formula(vector<float> factors) 
{
	this->factors = factors;
}

Formula::~Formula() {}

void Formula::setFactor(int factorNumber, float value)
{
	if (isPositiveValue(factorNumber) == false)
	{
		wcout << L"ALERT (Formula::setFactor()): Przekazano ujemny numer wsp�czynnika!" << endl;
		throw runtime_error("ALERT (Formula::setFactor()): Przekazano ujemny numer wsp�czynnika!");
	}

	if (factorNumber <= getDegree())
	{
		//if (factorNumber < getDegree() && factorNumber > 0 && value == 0)
			//factors.erase(factors.begin() + factorNumber);

		factors[factorNumber] = value;
	}
	else if (factorNumber > getDegree())
	{
		int degreeBeforeIncrease = getDegree();

		for (int i = degreeBeforeIncrease; i < factorNumber; i++)
		{
			//wcout << degreeBeforeIncrease << "   " << factorNumber << endl;
			factors.push_back(0);
		}
		
		factors[factorNumber] = value;
	}
}

void Formula::setDegree(int degree, bool setFactorValueOfFactorDegreeNumber, float factorValue)
{
	if (isPositiveValue(degree) == false)
	{
		wcout << L"ALERT (Formula::setDegree()): Przekazano ujemny numer wsp�czynnika!" << endl;
		throw runtime_error("ALERT (Formula::setDegree()): Przekazano ujemny numer wsp�czynnika!");
	}

	if (degree > getDegree())
	{
		if(setFactorValueOfFactorDegreeNumber == false) setFactor(degree, 1);
		else setFactor(degree, factorValue);
	}
	else
	{
		factors.erase(factors.begin() + degree + 1, factors.end());

		if (setFactorValueOfFactorDegreeNumber == true) setFactor(degree, factorValue);
	}
}

float Formula::getFactor(int factorNumber)
{
	if (isPositiveValue(factorNumber) == false)
	{
		wcout << L"ALERT (Formula::getFactor()): Przekazano ujemny numer wsp�czynnika!" << endl;
		throw runtime_error("ALERT (Formula::getFactor()): Przekazano ujemny numer wsp�czynnika!");
	}

	if (factorNumber < factors.size()) 
		return factors[factorNumber];
	else
	{
		wcout << L"ALERT (Formula::getFactor()): Pr�ba pobrania nieistniej�cego wsp�czynnika wielomianu! Za niski stopie� wielomianu!" << endl;
		throw runtime_error("ALERT (Formula::getFactor()): Pr�ba pobrania nieistniej�cego wsp�czynnika wielomianu! Za niski stopie� wielomianu!");
	}
}

vector<float> Formula::getAllFactors()
{
	return factors;
}

int Formula::getNumberOfNonzeroFactors()
{
	int numberOfNonzeroFactors = 0;

	for (auto i : factors)
		if (i != 0) 
			numberOfNonzeroFactors++;

	return numberOfNonzeroFactors;
}

int Formula::getDegree()
{
	return factors.size() - 1;
}

double Formula::getY(float x)
{
	double y = 0;

	for (int i = 0; i < factors.size(); i++)
		y += factors[i] * (double)pow(x, i);

	return y;
}

void Formula::removeZeroFactorsFromTheBeginning()
{
	// Usuwanie wszystkich wsp�czynnik�w r�wnych zero z pocz�tku wielomianu
	// id�c od x z najwy�sz� pot�g� (do momentu napotkania wsp�czynnika r�nego od 0) 
	// poza wyrazem wolnym wielomianu
	for (int i = getDegree(); i >= 1; i--)
	{
		if (factors[i] != 0) break;
		else factors.pop_back();
	}
}

void Formula::clear()
{
	setDegree(0, true, 0);
}

void Formula::addToFactor(int factorNumber, float value)
{
	if (isPositiveValue(factorNumber) == false)
	{
		wcout << L"ALERT (Formula::addToFactor()): Przekazano ujemny numer wsp�czynnika!" << endl;
		throw runtime_error("ALERT (Formula::addToFactor()): Przekazano ujemny numer wsp�czynnika!");
	}

	if (factorNumber <= getDegree())
	{
		factors[factorNumber] += value;
	}
	else if (factorNumber > getDegree())
		setFactor(factorNumber, value);
}

string Formula::convertToText()
{
	string text;

	Formula duplicate(factors);
	duplicate.removeZeroFactorsFromTheBeginning();

	for (int i = duplicate.getDegree(); i >= 0; i--)
	{
		// getDegree() > 0 w celu niepomijania wsp�czynnika zerowego kiedy jest r�wny 0, je�li jest jedynym wsp�czynnikiem;
		// Je�li zerowy wsp�czynnik nie jest jedynym wsp�czynnikiem i jest r�wny 0, nie nale�y go wy�wietla�
		if (duplicate.getDegree() > 0 && duplicate.getFactor(i) == 0) continue;

		////////////////////////////////////////////////////////////////////

		if (duplicate.getFactor(i) >= 0)
		{
			if (i != duplicate.getDegree()) text += "  +";

			if (duplicate.getFactor(i) != 1 || i == 0) text += to_string(duplicate.getFactor(i));
		}
		else if (duplicate.getFactor(i) < 0)
		{
			if(i != duplicate.getDegree()) text += "  -";
			else text += "-";
			
			if (duplicate.getFactor(i) != -1 || i == 0) text += to_string(-1 * duplicate.getFactor(i));
		}

		if (i >= 1) text += "x";

		if(i >= 2) text += to_string(i);
	}

	return text;
}

bool Formula::isEmpty()
{
	if (factors.size() == 0)
	{
		wcout << L"ALERT (Formula::isEmpty()): factors.size() sprawdzanego wielomianu JEST R�WNE 0!   Wielomian nie posiada �adnych wsp�czynnik�w!"<< endl;
		throw runtime_error("ALERT (Formula::isEmpty()): factors.size() sprawdzanego wielomianu JEST R�WNE 0!   Wielomian nie posiada �adnych wsp�czynnik�w!");

		return true;
	}

	bool empty = true;
	for (int i = getDegree(); i >= 0; i--)
	{
		if (factors[i] != 0)
		{
			empty = false;
		}
	}

	return empty;
}

bool Formula::isPositiveValue(int value)
{
	if (value >= 0) return true;
	else return false;
}

