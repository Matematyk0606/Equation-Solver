#pragma once
#include <vector>
#include <xutility>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

class Formula
{
public:
	Formula();
	Formula(int degree);
	Formula(vector<float> factors);
	~Formula();

	void setFactor(int factorNumber, float value);
	void setDegree(int degree, bool setFactorValueOfFactorDegreeNumber = false, float factorValue = 1);

	float getFactor(int factorNumber);
	vector<float> getAllFactors();
	int getNumberOfNonzeroFactors();
	int getDegree();
	double getY(float x);

	void removeZeroFactorsFromTheBeginning();
	void clear();

	void addToFactor(int factorNumber, float value);

	string convertToText();

	bool isEmpty();

private:
	bool isPositiveValue(int value);

	vector<float> factors;

	// Postaæ iloczynowa przedstawiona jako zestaw wielomianów w nawiasach przemno¿onych przez siebie (czysta postaæ iloczynowa)
	vector<Formula> productForm;

	/*
	Do przeanalizowania czy jest potrzebne:

	// Ewentualna reszta bêd¹c¹ skutkiem dzielenia przez wielomian (x-a),
	// gdzie 'a', to miejsce zerowe tego ca³êgo równania
	Formula *productRest;

	*/
};

