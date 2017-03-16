#include "float.h"
#include "rational.h"
#include "complex.h"
#include "boolean.h"
#include "String.h"
#include "char.h"
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <cmath>
#include <cassert>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <complex>
#include <algorithm>
#include <iostream>
#define ABS(x) ((x)<0?(-(x)):(x))
using namespace std;

Float::Float(double number) : number_(number){
	type_ = FLOAT;
}

Float::~Float(){

}

Number *Float::convert(Number *number2){
	assert(number2->type_ <= type_);
	Float *result = new Float();
	switch(number2->type_){
		case RATIONAL:{
			Rational *tmp = SCAST_RATIONAL(number2);
			result->number_ = (double)tmp->numerator_ / (double)tmp->denominator_;
			//printf("%.16g\n", (double)tmp->numerator_ );
			//printf("%.16g\n", (double)tmp->denominator_);
			//result->print();
			break;
		}
		case FLOAT:{
			Float *tmp = SCAST_FLOAT(number2);
			result->number_ = tmp->number_;
			break;
		}
		default:
			assert(0 && "type_ not defined");
	}
	return result;
}

Number *Float::add(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float(number_ + tmp->number_);
	return result;
}

Number *Float::sub(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float(number_ - tmp->number_);
	return result;
}

Number *Float::mul(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float(number_ * tmp->number_);
	if (number_ == 0.0 or tmp->number_ == 0.0)
		result->number_ = 0.0;
	return result;
}

Number *Float::div(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	assert(tmp->number_ && "divide zero");
	Float *result = new Float(number_ / tmp->number_);
	return result;
}

Number *Float::abs(){
	Float *result = new Float();
	if (number_ < 0)
		result->number_ = number_ * -1;
	else
		result->number_ = number_;
	return result;
}

Number *Float::quotient(Number *number2){
	assert(0 && "quotient float");
}

Number *Float::remainder(Number *number2){
	assert(0 && "remainder float");
}

Number *Float::modulo(Number *number2){
	assert(0 && "modulo float");
}

Number *Float::gcd(Number *number2){
	assert(0 && "gcd float");
}

Number *Float::lcm(Number *number2){
	assert(0 && "lcm float");
}

Number *Float::max_(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float();
	if (number_ > tmp->number_)
		result->number_ = number_;
	else
		result->number_ = tmp->number_;
	return result;
}

Number *Float::min_(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float();
	if (number_ < tmp->number_)
		result->number_ = number_;
	else
		result->number_ = tmp->number_;
	return result;
}

Number *Float::expt(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float();
	result->number_ = pow(number_, tmp->number_);
	return result;
}

Number *Float::sqrt_(){
	if (number_ > 0){
		Float *result = new Float();
		result->number_ = sqrt(number_);
		return result;
	}
	else{
		complex<double> base;
		base.real(number_);
		base.imag(0);
		complex<double> result = sqrt(base);
		double r = result.real(), i = result.imag();
		return new Complex(new Float(r), new Float(i));
	}
}

Number *Float::floor_(){
	Float *result = new Float();
	result->number_ = floor(number_);
	
	double temp = result->number_;
	if(double(round(temp)!=temp)){
		long long temp2 = temp + 0.1;
		string numerator = "";
		while(temp2 > 0){
			numerator = (char)((temp2%10)+'0') +  numerator;
			temp2 = temp2 / 10;
		}
		return new Rational(numerator, "1");
	}
	
	return result;
}

Number *Float::ceiling_(){
	Float *result = new Float();
	result->number_ = ceil(number_);
	
	double temp = result->number_;
	if(double(round(temp)!=temp)){
		long long temp2 = temp + 0.1;
		string numerator = "";
		while(temp2 > 0){
			numerator = (char)((temp2%10)+'0') +  numerator;
			temp2 = temp2 / 10;
		}
		return new Rational(numerator, "1");
	}
	
	return result;
}

Number *Float::round_(){
	Float *result = new Float();
	result->number_ = round(number_);
	
	double temp = result->number_;
	if(double(round(temp)!=temp)){
		long long temp2 = temp + 0.1;
		string numerator = "";
		while(temp2 > 0){
			numerator = (char)((temp2%10)+'0') +  numerator;
			temp2 = temp2 / 10;
		}
		return new Rational(numerator, "1");
	}
	
	return result;
}

Number *Float::truncate_(){
	Float *result = new Float();
	result->number_ = trunc(number_);
	
	double temp = result->number_;
	if(double(round(temp)!=temp)){
		long long temp2 = temp + 0.1;
		string numerator = "";
		while(temp2 > 0){
			numerator = (char)((temp2%10)+'0') +  numerator;
			temp2 = temp2 / 10;
		}
		return new Rational(numerator, "1");
	}
	
	return result;
}

Number *Float::numerator(){
	LongInt denominator("1"), x("2");
	double temp = number_;
	while(double(round(temp)!=temp)){
		temp *= 2;
		denominator = x * denominator;
	}
	string numerator = "";
	long long temp2 = temp + 0.01;
	while(temp2>0){
		numerator = (char)((temp2%10)+'0') +  numerator;
		temp2 = temp2 / 10;
	}
	return new Rational(numerator, LongInt("1"));
}

Number *Float::denominator(){
	LongInt denominator("1"), x("2");
	double temp = number_;
	while(double(round(temp)!=temp)){
		temp *= 2;
		denominator = x * denominator;
	}
	return new Rational(denominator, LongInt("1"));
}

/*Number *Float::inexact_exact(){
	LongInt denominator("1"), x("2");
	string numerator = "";
	bool sign = true;
	double temp = number_;
	if (temp < 0){
		sign = false;
		temp *= -1;
	}
	if (double(round(temp)==temp))
		return this->inexact_exact2();
	
	while(double(round(temp)!=temp)){
		temp *= 2;
		denominator = x * denominator;
	}
	long long temp2 = temp + 0.01;
	while(temp2>0){
		numerator = (char)((temp2%10)+'0') +  numerator;
		temp2 = temp2 / 10;
	}
	if (!sign)
		numerator = '-' + numerator;
	return new Rational(numerator, denominator);
}

Number *Float::inexact_exact2(){

	if(fabs(nearbyint(number_)-number_)<1e-300){
		stringstream ss1;
		ss1<<fixed<<setprecision(5)<<number_;
		string s1;
		ss1>>s1;
		int dot_pos=s1.find('.');
		string num1_s=s1.substr(0,dot_pos);
		Rational *res=new Rational(num1_s,"1");
		return res;
	}
	double *px=&number_;
	long long int *py=reinterpret_cast<long long int*>(px);
	long long int y=*py;
	long long int p=1;
	bool v[100]={0};
	for(int i=1;i<=64;++i){
		v[i]=y&p;
		y>>=1;
	}
	int tmp=0;
	for(int i=63;i>=53;--i){
		tmp<<=1;
		tmp+=v[i];
	}
	tmp-=1022;
	for(int i=1;i<=11;++i){
		v[52+i]=tmp&p;
		tmp>>=1;
	}
	long long int i_num=0;
	for(int i=63;i>=1;--i){
		i_num<<=1;
		i_num+=v[i];
	}
	string s_num="";
	while(i_num!=0){
		s_num+=(char)(i_num%10+'0');
		i_num/=10;
	}
	reverse(s_num.begin(),s_num.end());
	//LongInt num(s_num);
	Rational *res=new Rational(s_num,"1");
	return res;
}*/

Number* Float::inexact_exact(){
    double c_number_ = number_;
    double bs = 1.0;
    bool sign = true;
    if (c_number_ < 0){
    	c_number_ = 0 - c_number_;
    	sign = false;
	}
    
    while(fabs(c_number_-nearbyint(c_number_))>1e-300){
        c_number_ *= 2;
        bs *= 2;
    }
    stringstream ss1,ss2;
    string s1,s2;
    ss1 << fixed << setprecision(5) << c_number_;
    ss2 << fixed << setprecision(5) << bs;
    ss1 >> s1; 
	ss2 >> s2;
    s1=s1.substr(0,s1.find("."));
    s2=s2.substr(0,s2.find("."));
    
    if (!sign)
    	s1 = '-' + s1;

    return new Rational(s1,s2);
}

Number *Float::exact_inexact(){
	Float *result = new Float();
	result->number_ = number_;
	return result;
}

Number *Float::real_part(){
	Float *result = new Float();
	result->number_ = number_;
	return result;
}

Number *Float::imag_part(){
	return new Rational("0", "1");
}

Number *Float::sin_(){
	return new Float(sin(number_));
}

Number *Float::cos_(){
	return new Float(cos(number_));
}
Number *Float::tan_(){
	return new Float(tan(number_));
}
Number *Float::asin_(){
	if (number_ < -1 or number_ > 1){
		Float *real = new Float(number_);
		Float *imag = new Float(0);
		Complex *tmp = new Complex(real, imag);
		return tmp->asin_();
	}
	return new Float(asin(number_));
}
Number *Float::acos_(){
	if (number_ < -1 or number_ > 1){
		Float *real = new Float(number_);
		Float *imag = new Float(0);
		Complex *tmp = new Complex(real, imag);
		return tmp->acos_();
	}
	return new Float(acos(number_));
}
Number *Float::atan_(){
	return new Float(atan(number_));
}

Number *Float::exp_(){
	return new Float(exp(number_));
}

Number *Float::log_(){
	if (number_ == 0)
		assert(0 && "undefined log0!");	
	
	if (number_ > 0){ // +
		Float *res = new Float(log(number_));
		return res;
	}
	else{ // -
		complex<double> c_x(number_, 0.0);
		complex<double> c_res = log(c_x);
		return new Complex(new Float(c_res.real()), new Float(c_res.imag()));
	}
}

Number *Float::make_rec(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	Complex *res = new Complex;
	res->real_ = new Float(number_);
	res->imag_ = new Float(tmp->number_);
	return res;
}
Number *Float::make_pol(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	complex<double> c_res = polar(number_,tmp->number_);
	Complex *res = new Complex;
	res->real_ = new Float(c_res.real());
	res->imag_ = new Float(c_res.imag());
	return res;
}

Number *Float::magnitude_(){
	return new Float(fabs(number_));
}

Number *Float::angle_(){
	if (number_ == 0.0)
		assert(0 && "angle undefined for 0");
	if (number_ > 0.0) 
		return new Rational("0","1");
	else 
		return new Float(3.1415926535897932384);
}

Base *Float::is_zero(){
	if (number_ == 0.0)
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Float::is_negative(){
	if (number_ < 0.0)
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Float::is_positive(){
	if (number_ > 0.0)
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Float::is_odd(){
	if (double(round(number_)==number_)){
		if (int(number_) % 2 != 0)
			return new Boolean(true);
		else
			return new Boolean(false);
	}
	else
		return new Boolean(false);
}

Base *Float::is_even(){
	if (double(round(number_)==number_)){
		if (int(number_) % 2 == 0)
			return new Boolean(true);
		else
			return new Boolean(false);
	}
	else
		return new Boolean(false);
}

Base *Float::is_integer(){
	if (double(round(number_)==number_)){
		return new Boolean(true);
	}
	else
		return new Boolean(false);
}

Base *Float::is_rational(){
	return new Boolean(true);
}

Base *Float::is_real(){
	return new Boolean(true);
}

Base *Float::is_complex(){
	return new Boolean(true);
}

Base *Float::is_exact(){
	return new Boolean(false);
}

Base *Float::is_inexact(){
	return new Boolean(true);
}

Base *Float::less(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if(number_ < tmp->number_)
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Float::less_equal(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if(number_ <= tmp->number_)
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Float::more(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if(number_ > tmp->number_)
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Float::more_equal(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if(number_ >= tmp->number_)
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Float::int_to_char(){
	assert(0 && "expected integer");
	return NULL;
}

Base *Float::num_to_str(){
	String *res = new String();
	stringstream ss;
	ss << number_;
	ss >> res->str_;
	unsigned int pos = res->str_.find("e");
	if (pos != string::npos){
		if(res->str_[pos+1] >= '0' and res->str_[pos+1] <= '9'){// e100->e+100
			int len = res->str_.length();
			res->str_ = res->str_.substr(0,pos+1) + "+" + res->str_.substr(pos+1,len-1-pos);
		}
	}
	if (res->str_[0] == '+')
		res->str_.erase(0, 1);
	return res;
}

Base *Float::equal_(Base *opr2){
	Number *number2 = SCAST_NUMBER(opr2);
	if (number2->type_ != 2)
		return new Boolean(false);
	else{
		Float *tmp = SCAST_FLOAT(number2);
		return new Boolean(number_ == tmp->number_);
	}
}

void Float::printt(){
	printf("%.18g", number_);
}

void Float::print(){
	printf("%.18g\n", number_);
}

Float *Float::from_string(const char *expression){
    char *end_pointer;
    double val = strtod(expression, &end_pointer);
    
    if(expression[strlen(expression)-1] == 'i') // Complex
   		return NULL;
   	if(expression[0] == '#') // Boolean
   		return NULL;
    
    if (end_pointer == expression || end_pointer != expression + strlen(expression))
	    return NULL;

    return new Float(val);
}

//int main(){
 	//Float *a = new Float(1.2);
 	//Float *b = new Float(1.3);
 	//Rational c("1", "3");
 	//c.print();
 	//cout << endl;
//}
