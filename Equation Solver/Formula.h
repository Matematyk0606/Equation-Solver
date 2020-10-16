#pragma once
#include <vector>
#include <xutility>
#include <iostream>
#include <string>
#include <utility>
#include <sstream>

using namespace std;

class Formula
{
public:
	Formula();
	Formula(int degree);
	Formula(vector<double> factors);
	~Formula();

	void setFactor(int factorNumber, double value);
	void setAllFactors(vector<double> factors);
	void setDegree(int degree, bool setFactorValueOfFactorDegreeNumber = false, double factorValue = 1);

	double getFactor(int factorNumber);
	vector<double> getAllFactors();
	int getNumberOfNonzeroFactors();
	int getDegree();
	double getY(double x);

	void removeZeroFactorsFromTheBeginning();
	void clear();

	void addToFactor(int factorNumber, double value);

	string convertToText();

	bool isEmpty();

private:
	bool isPositiveValue(int value);
	string toString(double number);

	vector<double> factors;

	// Postaæ iloczynowa przedstawiona jako zestaw wielomianów w nawiasach przemno¿onych przez siebie (czysta postaæ iloczynowa)
	vector<Formula> productForm;

	/*
	Do przeanalizowania czy jest potrzebne:

	// Ewentualna reszta bêd¹c¹ skutkiem dzielenia przez wielomian (x-a),
	// gdzie 'a', to miejsce zerowe tego ca³êgo równania
	Formula *productRest;

	*/
};

