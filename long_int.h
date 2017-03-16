# pragma once
# define MAX(x,y) ((x)<(y)?(y):(x))
# define MIN(x,y) ((x)<(y)?(x):(y))
# include <vector>
# include <string>
using namespace std;

class LongInt {	
public:
	LongInt(string number = "0");
	LongInt(const LongInt &long_int2);
	~LongInt();

	operator bool();
	operator double();
	bool operator!=(const LongInt &long_int2) const;
	bool operator<(const LongInt &long_int2) const;
	LongInt &operator=(const LongInt &long_int2);
	LongInt operator+(const LongInt &long_int2) const;
	LongInt operator-(const LongInt &long_int2) const;
	LongInt operator*(const LongInt &long_int2) const;
	LongInt operator/(const LongInt &long_int2) const;
	LongInt operator%(const LongInt &long_int2) const;
	friend LongInt max(const LongInt &long_int1, const LongInt &long_int2);
	friend LongInt min(const LongInt &long_int1, const LongInt &long_int2);
	friend void add(const vector<int> &vnum1, const vector<int> &vnum2, vector<int> &vnum3);
	friend void del(const vector<int> &vnum1, const vector<int> &vnum2, vector<int> &vnum3);
	void print();

	string number_;
	vector <int> vnum;
	bool sign;

};
