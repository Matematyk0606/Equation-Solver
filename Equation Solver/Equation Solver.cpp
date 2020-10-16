#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <stdexcept>
#include <windows.h>
#include <conio.h>
#include "Formula.h"
#include "Solve.h"

using namespace std;

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);

	unsigned int degree;
	vector<double> factors;
	bool firstFactors = true;
	Formula initialFormula(factors);
	string formulaConvertedToText;

START:

	wcout << L"Podaj stopień równania: ";

	//unsigned int degree;
	wcin >> degree;

	wcout << endl;

	//vector<double> factors;
	//bool firstFactors = true;

	for (int i = degree; i >= 0; i--)
	{
		wcout << L"Podaj "<< i << L". współczynnik równania: ";

		double factor;
		wcin >> factor;

		if (firstFactors && factor == 0 && factors.size() != 0) continue;
		else if (firstFactors && factor != 0) firstFactors = false;

		factors.insert(factors.begin(), factor);
	}

	//Formula initialFormula(factors);
	initialFormula.setAllFactors(factors);

	//string formulaConvertedToText = initialFormula.convertToText();
	formulaConvertedToText = initialFormula.convertToText();

	wcout << endl << formulaConvertedToText.c_str() << endl << endl;

	//Formula formula2(vector<double>{-1, 3, -3, 1});
		
	//text1 = formula2.convertToText();

	//wcout << text1.c_str() << endl;

	//auto para = Solve::divisionFormulas(initialFormula, formula2);

	//wcout << "Wynik dzielenia:   " << para.first.convertToText().c_str() << "    R: " << para.second.convertToText().c_str() << endl;
	//wcout << L"Wynik mnożenia:   " << Solve::multiplicationFormulas(initialFormula, formula2).convertToText().c_str() << endl;
	//wcout << "Wynik odejmowania:   " << Solve::subtractingFormulas(initialFormula, formula2).convertToText().c_str() << endl;

	// Równanie 1. stopnia
	if (factors.size() == 2)
	{
		double x = Solve::linearFunc(factors);

		wcout << L"Rozwiązanie wielomianu 1. stopnia: x = " << x << endl;
	}

	// Równanie 2. stopnia
	else if (factors.size() == 3)
	{
		auto roots = Solve::quadraticFunc(factors);

		if (roots.size() == 0) wcout << L"Przekazana funkcja kwadratowa NIE posiada rozwiązań" << endl;
		else if(roots.size() == 1) wcout << L"Rozwiązanie wielomianu 2. stopnia: x0 = " << roots[0] << endl;
		else if(roots.size() == 2) wcout << L"Rozwiązanie wielomianu 2. stopnia: x1 = " << roots[0] << "   x2 = " << roots[1] << endl;
	}

	// Równanie stopnia wyższego niż 2.
	else if (factors.size() > 3)
	{
		//auto roots = Solve::theoremOfBezout(initialFormula);
		auto roots = Solve::cubicFuncAndHigher(initialFormula);

		if (roots.size() > 0)
		{
			wcout << L"Rozwiązania wielomianu " << initialFormula.getDegree() << L". stopnia, które udało mi się znaleźć:" << endl;

			for (auto i : roots) wcout << "x = " << i << endl;
		}
		else
			wcout << L"Nie potrafię znaleźć rozwiązań podanego wielomianu!" << endl;
	}

	initialFormula.clear();
	factors.clear();
	goto START;

	_getch();
}
