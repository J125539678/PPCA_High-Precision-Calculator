# include "long_int.h"
# include <cassert>
# include <cstdio>
# include <vector>
# include <string>
# include <algorithm>
# include <iostream>
# include <iomanip>
using namespace std;

LongInt::LongInt(string number) : number_(number) {
	int temp, i, j;
	vnum.clear();
	//cout << "longint construct begins!" << endl;
	if (number_[0] == '-'){
		sign = false;
		number_.erase(0,1);
	}
	else
		sign = true;
	
	int pointer = number_.size();
	while(pointer >= 4){
		temp = 0;
		for (i=4; i>=1; --i){
			temp *= 10;
			temp += (number_[pointer-i] - '0');
		}
		pointer -= 4;
		vnum.insert(vnum.begin(), temp);
	}
	temp = 0;
	for (i=0; i<pointer; ++i){
		temp *= 10;
		temp += (number_[i] - '0');
	}
	if (temp != 0 or vnum.size() == 0)
		vnum.insert(vnum.begin(), temp);
	while (vnum[0] == 0 and vnum.size()!=1)
		vnum.erase(vnum.begin());
	
	//cout << "Now check the longint: ";	
	//print();
	//cout << "longint construct finished!" << endl;
}

LongInt::LongInt(const LongInt &long_int2) : sign(long_int2.sign){
	vnum.clear();
	int size = long_int2.vnum.size();
	for (int i=0; i<size; ++i)
		vnum.push_back(long_int2.vnum[i]);
}

LongInt::~LongInt(){
	//print();
	//cout << "longint destruct\n";
}

int abs_compare(const vector<int> &vnum1, const vector<int> &vnum2){
	if (vnum1.size() < vnum2.size())
		return -1;
	else if (vnum1.size() > vnum2.size())
		return 1;
	else
		for (int i=0; i<vnum1.size(); ++i)
			if (vnum1[i] < vnum2[i])
				return -1;
			else if (vnum1[i] > vnum2[i])
				return 1;
	return 0;
}

LongInt::operator bool(){
	if (vnum[0] == 0)
		return false;
	else
		return true;
}

LongInt::operator double(){
	double result = 0;
	int size = vnum.size(), i;
	for (i=0; i<size; ++i){
		result *= 10000;
		result += vnum[i];
	}
	
	if (!sign)
		result *= -1;
	return result;
}

bool LongInt::operator!=(const LongInt &long_int2) const{
	if (sign != long_int2.sign)
		return true;
	if (abs_compare(vnum, long_int2.vnum) != 0)
		return true;
	else
		return false;
} 

bool LongInt::operator<(const LongInt &long_int2) const{
	if (sign and !long_int2.sign) // + > -
		return false;
	else if (!sign and long_int2.sign) // - < +
		return true; 
	else if (sign and long_int2.sign){ // + + compare absolute value
		if (vnum.size() < long_int2.vnum.size())
			return true;
		else if (vnum.size() > long_int2.vnum.size())
			return false;
		else
			for (int i=0; i<vnum.size(); ++i)
				if (vnum[i] != long_int2.vnum[i])
					return vnum[i] < long_int2.vnum[i];
		return false;
	}
	else{ // - - compare absolute value
		if (vnum.size() < long_int2.vnum.size())
			return false;
		else if (vnum.size() > long_int2.vnum.size())
			return true;
		else
			for (int i=0; i<vnum.size(); ++i)
				if (vnum[i] != long_int2.vnum[i])
					return vnum[i] > long_int2.vnum[i];
		return false;
	}
}

LongInt &LongInt::operator=(const LongInt &long_int2){
	vnum.clear();
	sign = long_int2.sign;
	int size = long_int2.vnum.size();
	for (int i=0; i<size; ++i)
		vnum.push_back(long_int2.vnum[i]);
	return *this;
}

void add(const vector<int> &vnum1, const vector<int> &vnum2, vector<int> &vnum3){ // vnum1 < vnum2
	vnum3.clear();
	
	int len1 = vnum1.size(), len2 = vnum2.size();
	int i, counter = 0, temp;
	
	for (i=1; i<=len1; ++i){
		temp = vnum1[len1-i] + vnum2[len2-i] + counter;
		counter = temp / 10000;
		temp = temp % 10000;
		vnum3.insert(vnum3.begin(), temp);
	}
	
	for (i=len1+1; i<=len2; ++i){
		temp = vnum2[len2-i] + counter;
		counter = temp / 10000;
		temp = temp % 10000;
		vnum3.insert(vnum3.begin(), temp);
	}
	
	if (counter > 0)
		vnum3.insert(vnum3.begin(), counter);
}

void del(const vector<int> &vnum1, const vector<int> &vnum2, vector<int> &vnum3){ // vnum1 < vnum2
	vnum3.clear();	
	
	int len1 = vnum1.size(), len2 = vnum2.size();
	int i, counter = 0, temp;
	
	for (i=1; i<=len1; ++i){
		temp = vnum2[len2-i] - vnum1[len1-i] - counter;
		if (temp < 0){
			counter = 1;
			temp += 10000;
		}
		else
			counter = 0;
		vnum3.insert(vnum3.begin(), temp);
	}
	
	for (i=len1+1; i<=len2; ++i){
		temp = vnum2[len2-i] - counter;
		if (temp < 0){
			counter = 1;
			temp += 10000;
		}
		else
			counter = 0;
		vnum3.insert(vnum3.begin(), temp);
	}
	
	while (vnum3[0] == 0 and vnum3.size()!=1)
		vnum3.erase(vnum3.begin());
}

LongInt LongInt::operator+(const LongInt &long_int2) const{
	LongInt result;
	if (sign == true and long_int2.sign == true){ // + + 
		result.sign = true;
		if (vnum.size() < long_int2.vnum.size())
			add(vnum, long_int2.vnum, result.vnum);
		else
			add(long_int2.vnum, vnum, result.vnum);
	}
	else if (sign == false and long_int2.sign == false){ // - -
		result.sign = false;
		if (vnum.size() < long_int2.vnum.size())
			add(vnum, long_int2.vnum, result.vnum);
		else
			add(long_int2.vnum, vnum, result.vnum);
	}
	else{ // + -
		int com = abs_compare(vnum, long_int2.vnum);
		if (com == -1){
			result.sign = long_int2.sign;
			del(vnum, long_int2.vnum, result.vnum);
		}
		else if (com == 1){
			result.sign = sign;
			del(long_int2.vnum, vnum, result.vnum);
		}
		else{
			result.sign = true;
			return result;
		}
	}
	
	return result;
}

LongInt LongInt::operator-(const LongInt &long_int2) const{
	LongInt result;
	if (sign == true and long_int2.sign == false){ // + -
		result.sign = true;
		if (vnum.size() < long_int2.vnum.size())
			add(vnum, long_int2.vnum, result.vnum);
		else
			add(long_int2.vnum, vnum, result.vnum);
	}
	else if (sign == false and long_int2.sign == true){ // - +
		result.sign = false;
		if (vnum.size() < long_int2.vnum.size())
			add(vnum, long_int2.vnum, result.vnum);
		else
			add(long_int2.vnum, vnum, result.vnum);
	}
	else{ // + + or - - 
		int com = abs_compare(vnum, long_int2.vnum);
		if (com == -1){
			result.sign = !sign;
			del(vnum, long_int2.vnum, result.vnum);
		}
		else if (com == 1){
			result.sign = sign;
			del(long_int2.vnum, vnum, result.vnum);
		}
		else{
			result.sign = true;
			return result;
		}
	}
	
	return result;
}

LongInt LongInt::operator*(const LongInt &long_int2) const{
	
	if (vnum[0] == 0 or long_int2.vnum[0] == 0)
		return LongInt("0");
	
	LongInt result;	
	if (sign == long_int2.sign)
		result.sign = true;
	else
		result.sign = false;
			
	int i, j, temp;
	int len1 = vnum.size(), len2 = long_int2.vnum.size(), len3 = len1 + len2;
	for (i=0; i<len3-1; ++i)
		result.vnum.push_back(0);
	
	for (i=0; i<len1; ++i){
		for (j=0; j<len2; ++j){
			temp = vnum[i] * long_int2.vnum[j];
			result.vnum[i+j] += temp / 10000;
			result.vnum[i+j+1] += temp % 10000;
			
			if (result.vnum[i+j+1] > 9999){
				++result.vnum[i+j];
				result.vnum[i+j+1] -= 10000; 
			}
			if (result.vnum[i+j] > 9999){
				++result.vnum[i+j-1];
				result.vnum[i+j] -= 10000; 
			}
		}
	}
	
	while (result.vnum[0] == 0 and result.vnum.size()!= 1){
		result.vnum.erase(result.vnum.begin());
	}
	
	return result;
}

LongInt LongInt::operator/(const LongInt &long_int2) const{
	if (vnum[0] == 0)
		return LongInt("0");
	
	assert(long_int2.vnum[0] && "devide zero");
	LongInt result;
	if (sign == long_int2.sign) // determine the sign;
		result.sign = true; 
	else
		result.sign = false;
	if (abs_compare(vnum, long_int2.vnum) == -1) // case |a| < |b|;
		return result;
	else if (abs_compare(vnum, long_int2.vnum) == 0){ // case |a| == |b|;
		result.vnum[0] = 1;
		return result;
	}
	
	vector <int> temp_vnum = vnum, temp_vnum2 = long_int2.vnum;
	int temp_ans = 0, counter = 1;
	
	for (int i=0; i<temp_vnum.size()-long_int2.vnum.size(); ++i){
			temp_vnum2.push_back(0);
			++counter;
	}
	if (abs_compare(temp_vnum, temp_vnum2) == -1){
		temp_vnum2.erase(temp_vnum2.begin()+temp_vnum2.size()-1);
		--counter;
	}
	int size = counter;
	for (int i=1; i<size; ++i)
		result.vnum.push_back(0);
	
	vector<int> temp_vnum3;
	while(counter > 0){
		temp_ans = 0;
		while (abs_compare(temp_vnum, temp_vnum2) != -1){
			del(temp_vnum2, temp_vnum, temp_vnum3);
			temp_vnum = temp_vnum3;
			++temp_ans;
		}
		result.vnum[size-counter] += temp_ans;
		temp_vnum2.erase(temp_vnum2.begin()+temp_vnum2.size()-1);
		--counter;
	}
		
	return result;
}

LongInt LongInt::operator%(const LongInt &long_int2) const{
	if (vnum[0] == 0)
		return LongInt("0");
	
	assert(long_int2.vnum[0] && "devide zero");
	LongInt result;
	
	if (sign == long_int2.sign) // determine the sign;
		result.sign = true; 
	else
		result.sign = false;
	if (abs_compare(vnum, long_int2.vnum) == -1) // case |a| < |b|;
		return *this;
	else if (abs_compare(vnum, long_int2.vnum) == 0){ // case |a| == |b|;
		result.vnum[0] = 0;
		return result;
	}
	
	result.vnum = vnum;
	vector <int> temp_vnum2 = long_int2.vnum;
	int counter = 1;
	
	for (int i=0; i<result.vnum.size()-long_int2.vnum.size(); ++i){
			temp_vnum2.push_back(0);
			++counter;
	}
	if (abs_compare(result.vnum, temp_vnum2) == -1){
		temp_vnum2.erase(temp_vnum2.begin()+temp_vnum2.size()-1);
		--counter;
	}
	
	vector<int> temp_vnum3;
	while(counter > 0){
		while (abs_compare(result.vnum, temp_vnum2) != -1){
			del(temp_vnum2, result.vnum, temp_vnum3);
			result.vnum = temp_vnum3;
		}
		temp_vnum2.erase(temp_vnum2.begin()+temp_vnum2.size()-1);
		--counter;
	}
		
	return result;
}

LongInt max(const LongInt &long_int1, const LongInt &long_int2){
	if (long_int1 < long_int2)
		return long_int2;
	else
		return long_int1;
}

LongInt min(const LongInt &long_int1, const LongInt &long_int2){
	if (long_int1 < long_int2)
		return long_int1;
	else
		return long_int2;
}

void LongInt::print(){
	if (!sign and vnum[0] != 0)
		cout << "-";
	cout << vnum[0];
	for (int i=1; i<vnum.size(); ++i){
		cout << setw(4) << setfill('0') << vnum[i]; 
	}
	//cout << endl;
}
