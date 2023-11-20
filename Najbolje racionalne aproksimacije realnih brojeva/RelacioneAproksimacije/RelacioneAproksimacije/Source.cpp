#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <string>
#include <numeric>

using namespace std;
int numberofspaces = 0;

class Fraction {
	 int p;
	 int q;
	vector <int> decimals;
	int charactersnum;
	double errorValue;
	bool isFirst;
	bool isSecond;
	int index;
	 
public:
	Fraction( int p,  int q, double alfa) {
		this->p = p;
		this->q = q;
		this->isFirst = false;
		this->isSecond = false;
		this->errorValue = 0;
		this->charactersnum = 0;
		this->index = -1;
		this->calculateDecimals();
		this->calculateErrorValue(alfa);
	}
	void calculateErrorValue(double alfa) {
		errorValue = abs(alfa - (double)p / q);
	}
	
	void calculateDecimals() {
		double alfa = ((double)p / q);
		vector <double> x;
		vector <double> ds;
		double d;
		int i = 0;
		x.push_back(alfa);
		this->decimals.push_back(floor(x.at(i)));
		d = x.at(i) - this->decimals.at(i);

		this->charactersnum += to_string(this->decimals.at(i)).length() + 1; //poravnanje za ispis

		while (d > 1e-12){
			ds.push_back(d);
			i++;
			x.push_back(((double)1 / (ds.at(i - 1))));
			this->decimals.push_back(floor(x.at(i)));
			d = x.at(i) - this->decimals.at(i);

			this->charactersnum += to_string(this->decimals.at(i)).length() + 1; //poravnanje za ispis
		};
	
		while (this->decimals.at(this->decimals.size() - 1) == 1) {
			this->decimals.pop_back();
			this->decimals.at(this->decimals.size() - 1)++;

			if (this->decimals.at(this->decimals.size() - 1) >= 10)this->charactersnum++; //poravnanje za ispis
			this->charactersnum -= 2;
		}
		if (numberofspaces < this->charactersnum) {
			numberofspaces = this->charactersnum;
		}

	}
	void setFirst(bool value) {
		isFirst = value;
	}
	void setSecond(bool value) {
		isSecond = value;
	}
	void setIndex(int index) {
		this->index = index;
	}

	bool getIsFirst() {
		return isFirst;
	}
	bool getIsSecond() {
		return isSecond;
	}
	int getP() {
		return p;
	}
	int getQ() {
		return q;
	}
	int getIndex() {
		return this->index;
	}
	double getErrorValue() {
		return errorValue;
	}
	string getFraction() {
		return to_string(this->p) + "/" + to_string(this->q);
	}
	void printFraction() {
		printf("%d/%d", p, q);
	}
	void printErrorVal() {
		printf("%0.20lf", errorValue);
	}
	void printDecimals() {

		printf("\t[");
		for (int i = 0; i < this->decimals.size(); i++) {
			cout << this->decimals.at(i);
			if (i == 0)  cout << ";";
			else if (i != this->decimals.size() - 1)cout << ",";

		}
		printf("]");
		
		for (int i = this->charactersnum; i < numberofspaces; i++) {
			printf(" ");
		}
		
	}
	void printAprox() {
		if (isSecond) printf("II\n");
		else if (isFirst) printf("I\n");
		else printf("N\n");
	}

	bool operator<(const Fraction &fraction ) {
		return this->errorValue < fraction.errorValue;
	}
};
double firstErr(double alfa, int p, int q) {
	return	abs(alfa - ((double)p / q));
};
double secondErr(double alfa, int p, int q) {
	return	abs((alfa*q - p));
};
int calculateNumerator(double alfa, int q) {
	int p = round(alfa * q);  // najblizi prirodan broj 
	return p;
};

void FirstorSecond(double alfa, int q, vector <Fraction>& fractions) {
	double firstMin = INT16_MAX;
	double secondMin = INT16_MAX;
	int s = 1;
	for (s = 1; s < q; s++) {
		//first
		double r = calculateNumerator(alfa, s);
		if (s == 1) {
			firstMin = firstErr(alfa, r, s);
		}
		double currentErr = firstErr(alfa, r, s);
		if (currentErr < firstMin) {
			firstMin = currentErr;
		}
		//second
		if (s == 1) {
			secondMin = secondErr(alfa, r, s);
		}
		currentErr = secondErr(alfa, r, s);
		if (currentErr < secondMin) {
			secondMin = currentErr;
		}
	}

	for (int i = 0; i < fractions.size(); i++) {
		double currentErr = secondErr(alfa, fractions.at(i).getP(), fractions.at(i).getQ());
		if (currentErr < secondMin) {
			secondMin = currentErr;
			fractions.at(i).setSecond(true);
			fractions.at(i).setFirst(true);
			currentErr = firstErr(alfa, fractions.at(i).getP(), fractions.at(i).getQ());
			if (currentErr < firstMin) {
				firstMin = currentErr;
			}
		}
		else {
			fractions.at(i).setSecond(false);
			currentErr = firstErr(alfa, fractions.at(i).getP(), fractions.at(i).getQ());
			if (currentErr < firstMin) {
				firstMin = currentErr;
				fractions.at(i).setFirst(true);
			}
			else {
				fractions.at(i).setFirst(false);
			}
		}
		

	}

	

};

void printSortedFractions(vector <Fraction>& fractions) {
	printf("Razlomak \t Verizni zapis \t\t Odstupanje \t\t Vrsta \n");
	for (int i = 0; i < fractions.size(); i++) {
		Fraction f = fractions.at(i);
		f.printFraction();
		printf("\t");
		f.printDecimals();
		printf("\t\t");
		f.printErrorVal();
		printf("\t\t");
		f.printAprox();
	}
}

int main() {
	double alfa;
	int n;
	int m;
	cout << "Unesite pozitivan realan broj alfa\n";
	cin >> alfa;
	cout << "Unesite prirodan broj n\n";
	cin >> n;
	cout << "Unesite prirodan broj m tako da vazi n<m\n";
	cin >> m;

	if (alfa < 0) {
		printf("Nekorektan unos za alfa - alfa nije pozitivan realan broj\n");
		return -1;
	}
	if (n <= 0 || m <= 0 ) {
		printf("Nekorektan unos za n i m - brojevi ne pripadaju skupu prirodnih brojeva\n");
		return -1;
	}
	if (n >= m) {
		printf("Nekoretkan unos za n i m - nije ispunjen uslov n<m \n");
		return -1;
	}

	vector <Fraction> fractions;
	for (int q = n; q <= m; q++) {
		int p = calculateNumerator(alfa, q);
		if (gcd(p, q) != 1) {
			//Preskacemo neredukovane razlomke
			continue;
		}
		Fraction f(p, q, alfa);
		fractions.push_back(f);
		
	}

	FirstorSecond(alfa, n, fractions);

	printf("Za konstantu alfa = %0.20lf pri izboru n = %d i m = %d \n", alfa,n,m);
	
	string first = "Najbolje racionalne aproksimacije I vrste su ";
	string second = "Najbolje racionalne aproksimacije II vrste su ";
	for (int i = 0; i < fractions.size(); i++) {
		if (fractions.at(i).getIsFirst()) {
			first.append(fractions.at(i).getFraction() + " ");
		}
		if (fractions.at(i).getIsSecond()) {
			second.append(fractions.at(i).getFraction()+" ");
		}
	}
	printf((first +"\n").c_str());
	printf((second +"\n").c_str());

	sort(fractions.begin(), fractions.end());
	printSortedFractions(fractions);
	

}
