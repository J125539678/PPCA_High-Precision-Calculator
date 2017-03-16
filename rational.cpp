# include "rational.h"
# include "float.h"
# include "complex.h"
# include "boolean.h"
# include "long_int.h"
# include "String.h"
# include "number.h"
# include "char.h"
# include <cassert>
# include <cstdlib>
# include <complex>
# include <math.h>
# include <cstdio>
# include <cstring>
# include <string>
# include <iostream>
# define SCAST_NUMBER(x) static_cast<Number*>(x)
# define SCAST_RATIONAL(x) static_cast<Rational*>(x)
# define SCAST_FLOAT(x) static_cast<Float*>(x)
# define SCAST_COMPLEX(x) static_cast<Complex*>(x)
# define SCAST_BOOLEAN(x) static_cast<Boolean*>(x)
using namespace std;

Rational::Rational(string numerator, string denominator)/*: numerator_(numerator), denominator_(denominator)*/{
	type_ = RATIONAL;
	//cout << "Now Rational begins to construct!" << endl;
	//cout << numerator << endl << denominator << endl;
	numerator_ = LongInt(numerator);
	//cout << "check numerator: ";
	//numerator_.print();
	//cout << endl;
	denominator_ = LongInt(denominator);
	reduce();
	//print();
	//cout << "Rational complete!" << endl;
}

Rational::Rational(LongInt numerator, LongInt denominator) : numerator_(numerator), denominator_(denominator){
	type_ = RATIONAL;
	reduce();
}

Rational::~Rational(){

}

void Rational::reduce(){
	//cout << "Now reduce!" << endl;
	assert(denominator_.vnum[0] && "denominator is zero");
	if(!numerator_.vnum[0]){
		denominator_ = LongInt("1");
		return;
	}
	if(!(denominator_ != LongInt("1"))){
		//cout << "Reduce completed!" << endl;
		return;
	}
	
	bool flag = true; // determine the sign
	if(denominator_.sign != numerator_.sign)
		flag = false;
	denominator_.sign = true; // change the sign
	numerator_.sign = true;
	
	LongInt big, tmp, small;
	big = max(numerator_, denominator_);
	small = min(numerator_, denominator_);
	tmp = big % small;
	while(tmp != LongInt("0")){
		big = small;
		small = tmp;
		tmp = big % small;
	}
	numerator_ = numerator_ / small;
	denominator_ = denominator_ / small;
	if (!flag)
		numerator_.sign = false;
	//cout << "Reduce completed!" << endl;
	//big.print();
	//cout << endl;
	//small.print();
	//cout << endl;
	//tmp.print();
	//cout << endl;
}

Number *Rational::convert(Number *number2){
	assert(number2->type_ <= type_);
	Rational *result = new Rational("0", "1");
	switch(number2->type_){
		case RATIONAL:{
			Rational *tmp = SCAST_RATIONAL(number2);
			result->numerator_ = tmp->numerator_;
			result->denominator_ = tmp->denominator_;
			break;
		}
		default:
			assert(0 && "type_ not defined");
	}
	result->reduce();
	return result;
}

Number *Rational::add( Number *number2) {
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational("0", "1");
	result->numerator_ = numerator_*tmp->denominator_ + denominator_*tmp->numerator_;
	result->denominator_ = denominator_ * tmp->denominator_;
	result->reduce();
	return result;
}

Number *Rational::sub(Number *number2) {
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational("0", "1");
	result->numerator_ = numerator_*tmp->denominator_ - denominator_*tmp->numerator_;
	result->denominator_ = denominator_ * tmp->denominator_;
	result->reduce();
	return result;
}

Number *Rational::mul(Number *number2) {
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational("0", "1");
	result->numerator_ = numerator_ * tmp->numerator_;
	result->denominator_ = denominator_ * tmp->denominator_;
	result->reduce();
	return result;
}

Number *Rational::div(Number *number2) {
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational("0", "1");
	result->numerator_ = numerator_ * tmp->denominator_;
	result->denominator_ = denominator_ * tmp->numerator_;
	result->reduce();
	return result;
}

Number *Rational::abs(){
	Rational *result = new Rational("0", "1");
	result->numerator_ = numerator_;
	result->denominator_ = denominator_;
	result->numerator_.sign = true;
	return result;
}

Number *Rational::quotient(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (denominator_ != LongInt("1") or tmp->denominator_ != LongInt("1"))
		assert(0 && "quotient fraction");
	Rational *result = new Rational("0", "1");
	result->numerator_ = numerator_ / tmp->numerator_;
	return result;
}

Number *Rational::remainder(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (denominator_ != LongInt("1") or tmp->denominator_ != LongInt("1"))
		assert(0 && "quotient fraction");
	Rational *result = new Rational("0", "1");
	result->numerator_ = numerator_ % tmp->numerator_;
	return result;
}

Number *Rational::modulo(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (denominator_ != LongInt("1") or tmp->denominator_ != LongInt("1"))
		assert(0 && "modulo fraction");
	Rational *result = new Rational("0", "1");
	result->numerator_ = numerator_ % tmp->numerator_;
	if(result->numerator_.sign != tmp->numerator_.sign)
		result->numerator_ = result->numerator_ + tmp->numerator_;
	return result;
}

Number *Rational::gcd(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	numerator_.sign = true;
	tmp->numerator_.sign = true;
	if (denominator_ != LongInt("1") or tmp->denominator_ != LongInt("1"))
		assert(0 && "gcd fraction");
	Rational *result = new Rational("0", "1");
	if (!(numerator_ != LongInt("0") and tmp->numerator_ != LongInt("0"))){
		result->numerator_ = max(numerator_, tmp->numerator_);
		return result;
	}
	
	LongInt big, small, temp;
	big = max(numerator_, tmp->numerator_);
	small = min(numerator_, tmp->numerator_);
	temp = big % small;
	while(temp != LongInt("0")){
		big = small;
		small = temp;
		temp = big % small;
	}
	result->numerator_ = small;
	return result;
}

Number *Rational::lcm(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	numerator_.sign = true;
	tmp->numerator_.sign = true;
	if (denominator_ != LongInt("1") or tmp->denominator_ != LongInt("1"))
		assert(0 && "lcm fraction");
	Rational *result = new Rational("0", "1");
	if (!(numerator_ != LongInt("0") and tmp->numerator_ != LongInt("0"))){
		result->numerator_ = LongInt("0");
		return result;
	}
	
	LongInt big, small, temp;
	big = max(numerator_, tmp->numerator_);
	small = min(numerator_, tmp->numerator_);
	temp = big % small;
	while(temp != LongInt("0")){
		big = small;
		small = temp;
		temp = big % small;
	}
	result->numerator_ = numerator_ * tmp->numerator_ / small;
	return result;
}

Number *Rational::max_(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational("0", "1");
	if (denominator_ * tmp->numerator_ < numerator_ * tmp->denominator_){
		result->numerator_ = numerator_;
		result->denominator_ = denominator_;
	}
	else{
		result->numerator_ = tmp->numerator_;
		result->denominator_ = tmp->denominator_;
	}
	return result;
}

Number *Rational::min_(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational("0", "1");
	if (numerator_ * tmp->denominator_ < denominator_ * tmp->numerator_){
		result->numerator_ = numerator_;
		result->denominator_ = denominator_;
	}
	else{
		result->numerator_ = tmp->numerator_;
		result->denominator_ = tmp->denominator_;
	}
	return result;
}

Number *Rational::expt(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	double temp = pow(double(numerator_)/double(denominator_), double(tmp->numerator_)/double(tmp->denominator_));
	if(double(round(temp)!=temp)){
		Float *result = new Float();
		result->number_ = temp;
		return result;
	}
	else{
		long long temp2 = temp + 0.1;
		string numerator = "";
		while(temp2 > 0){
			numerator = (char)((temp2%10)+'0') +  numerator;
			temp2 = temp2 / 10;
		}
		return new Rational(numerator, "1");
	}
}

Number *Rational::sqrt_(){
	Float *temp_number = new Float(double(numerator_)/double(denominator_));
	Number *res = temp_number->sqrt_();
	
	if (res->type_ == COMPLEX)
		return  res;

	Float *tmp = SCAST_FLOAT(res);
	double temp = tmp->number_;
	delete temp_number, res;
	
	if(double(round(temp)!=temp)){
		Float *result = new Float();
		result->number_ = temp;
		return result;
	}
	else{
		long long temp2 = temp + 0.1;
		string numerator = "";
		while(temp2 > 0){
			numerator = (char)((temp2%10)+'0') +  numerator;
			temp2 = temp2 / 10;
		}
		return new Rational(numerator, "1");
	}
}

Number *Rational::floor_(){
	Rational *result = new Rational("0", "1");
	
	if (!(denominator_ != LongInt("1"))){ // integers
		result->numerator_ = numerator_;
		return result;
	}
	
	else{
		result->numerator_ = numerator_ / denominator_;
		if(!result->numerator_.sign)
			result->numerator_ = result->numerator_ - LongInt("1");
		return result;
	}
}

Number *Rational::ceiling_(){
	Rational *result = new Rational("0", "1");
	
	if (!(denominator_ != LongInt("1"))){ // integers
		result->numerator_ = numerator_;
		return result;
	}
	
	else{
		result->numerator_ = numerator_ / denominator_;
		if(result->numerator_.sign)
			result->numerator_ = result->numerator_ + LongInt("1");
		return result;
	}
}

Number *Rational::truncate_(){
	Rational *result = new Rational("0", "1");
	
	if (!(denominator_ != LongInt("1"))){ // integers
		result->numerator_ = numerator_;
		return result;
	}
	
	else{
		result->numerator_ = numerator_ / denominator_;
		return result;
	}
}

Number *Rational::round_(){
	Rational *result = new Rational("0", "1");
	
	if (!(denominator_ != LongInt("1"))){ // integers
		result->numerator_ = numerator_;
		return result;
	}
	
	else{
		result->numerator_ = numerator_ / denominator_;
		if (denominator_ < (numerator_ % denominator_)* LongInt("2")){
			if (result->numerator_.sign)
				result->numerator_ = result->numerator_ + LongInt("1");
			else
				result->numerator_ = result->numerator_ - LongInt("1");
		}
		return result;
	}
}

Number *Rational::numerator(){
	Rational *result = new Rational("0", "1");
	result->numerator_ = numerator_;
	return result;
}

Number *Rational::denominator(){
	Rational *result = new Rational("0", "1");
	result->numerator_ = denominator_;
	return result;
}

Number *Rational::inexact_exact(){
	Rational *result = new Rational("0", "1");
	result->numerator_ = numerator_;
	result->denominator_ = denominator_;
	return result;
}

Number *Rational::exact_inexact(){
	Float *result = new Float();
	result->number_ = double(numerator_) / double(denominator_);
	return result;
}

Number *Rational::real_part(){
	Rational *result = new Rational("0", "1");
	result->numerator_ = numerator_;
	result->denominator_ = denominator_;
	return result;
}

Number *Rational::imag_part(){
	return new Rational("0", "1");
}

Number *Rational::sin_(){
	Float *res = new Float;
	Float *tmp = SCAST_FLOAT(res->convert(this));
	res->number_ = sin(tmp->number_);
	delete tmp;
	return res;
}

Number *Rational::cos_(){
	Float *res = new Float;
	Float *tmp = SCAST_FLOAT(res->convert(this));
	res->number_ = cos(tmp->number_);
	delete tmp;
	return res;
}

Number *Rational::tan_(){
	Float *res = new Float;
	Float *tmp = SCAST_FLOAT(res->convert(this));
	res->number_ = tan(tmp->number_);
	delete tmp;
	return res;
}

Number *Rational::asin_(){
	Float *res = new Float;
	Float *tmp = SCAST_FLOAT(res->convert(this));
	if (tmp->number_ < -1 or tmp->number_ > 1){
		Float *real = new Float(tmp->number_);
		Float *imag = new Float(0);
		Complex *tmp2 = new Complex(real, imag);
		delete tmp;
		return tmp2->asin_();
	}
	res->number_ = asin(tmp->number_);
	delete tmp;
	return res;
}

Number *Rational::acos_(){
	Float *res = new Float;
	Float *tmp = SCAST_FLOAT(res->convert(this));
	if (tmp->number_ < -1 or tmp->number_ > 1){
		Float *real = new Float(tmp->number_);
		Float *imag = new Float(0);
		Complex *tmp2 = new Complex(real, imag);
		delete tmp;
		return tmp2->acos_();
	}
	res->number_ = acos(tmp->number_);
	delete tmp;
	return res;
}

Number *Rational::atan_(){
	Float *res = new Float;
	Float *tmp = SCAST_FLOAT(res->convert(this));
	res->number_ = atan(tmp->number_);
	delete tmp;
	return res;
}

Number *Rational::exp_(){
	Float *res = new Float;
	Float *tmp = SCAST_FLOAT(res->convert(this));
	res->number_ = exp(tmp->number_);
	delete tmp;
	return res;
}

Number *Rational::log_(){
	if (!(numerator_ != LongInt("0")))
		assert(0 && "undefined log0!");
	
	Float *res = new Float;
	Float *tmp = SCAST_FLOAT(res->convert(this));	
	
	if (numerator_.sign){ // +
		res->number_ = log(tmp->number_);
		delete tmp;
		return res;
	}
	else{ // -
		complex<double> c_x(tmp->number_, 0.0);
		complex<double> c_res = log(c_x);
		delete tmp;
		delete res;
		return new Complex(new Float(c_res.real()), new Float(c_res.imag()));
	}
}

Number *Rational::make_rec(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	Complex *res = new Complex;
	res->real_ = new Rational(numerator_, denominator_);
	res->real_ = new Rational(tmp->numerator_, tmp->denominator_);
	return res;
}

Number *Rational::make_pol(Number *number2){
	Float *temp = new Float();
	Float *tmp1 = SCAST_FLOAT(temp->convert(this));
	Float *tmp2 = SCAST_FLOAT(temp->convert(number2));
	complex<double> c_res = polar(tmp1->number_, tmp2->number_);
	Complex *res = new Complex;
	res->real_ = new Float(c_res.real());
	res->imag_ = new Float(c_res.imag());
	delete temp, tmp1, tmp2;
	return res;
}

Number *Rational::magnitude_(){
	return abs();
}

Number *Rational::angle_(){
	if (!(numerator_ != LongInt("0")))
		assert(0 && "undefined angle for 0!");
	if (numerator_.sign) // +
		return new Rational("0", "1");
	else
		return new Float(3.1415926535897932384);
}

Base *Rational::is_zero(){
	if (!(numerator_ != LongInt("0")))
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Rational::is_negative(){
	if (!numerator_.sign)
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Rational::is_positive(){
	if (numerator_.sign)
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Rational::is_odd(){
	if (denominator_ != LongInt("1"))
		return new Boolean(false);
	if (numerator_ % LongInt("2") != LongInt("0"))
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Rational::is_even(){
	if (denominator_ != LongInt("1"))
		return new Boolean(false);
	if (numerator_ % LongInt("2") != LongInt("0"))
		return new Boolean(false);
	else
		return new Boolean(true);
}

Base *Rational::is_integer(){
	if (denominator_ != LongInt("1"))
		return new Boolean(false);
	else
		return new Boolean(true);
}

Base *Rational::is_rational(){
	return new Boolean(true);
}

Base *Rational::is_real(){
	return new Boolean(true);
}

Base *Rational::is_complex(){
	return new Boolean(true);
}

Base *Rational::is_exact(){
	return new Boolean(true);
}

Base *Rational::is_inexact(){
	return new Boolean(false);
}

Base *Rational::less(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (numerator_ * tmp->denominator_ < tmp->numerator_ * denominator_)
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Rational::less_equal(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (tmp->numerator_ * denominator_ < numerator_ * tmp->denominator_)
		return new Boolean(false);
	else
		return new Boolean(true);
}

Base *Rational::more(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (tmp->numerator_ * denominator_ < numerator_ * tmp->denominator_)
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Rational::more_equal(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (numerator_ * tmp->denominator_ < tmp->numerator_ * denominator_)
		return new Boolean(false);
	else
		return new Boolean(true);
}

Base *Rational::int_to_char(){
	if (denominator_ != LongInt("1"))
		assert(0 && "expected integer");
	int number = (int)(double)numerator_;
	char ch = number;
	return new Char(ch);
}

string int2str(int num){
	string result = "";
	for (int i=0; i<4; ++i){
		result = char(('0' + (num % 10))) + result;
		num /= 10;
	}
	return result;
}

string int2str_0(int num){
	string result = "";
	if (num == 0)
		return "0";
	else{
		while (num > 0){
			result = char(('0' + (num % 10))) + result;
			num /= 10;
		}
	}
	return result;
}

Base *Rational::num_to_str(){
	string res = "";
	if (numerator_.vnum[0] == 0) // number 0
		return new String("0");
	if (!numerator_.sign) // negative
		res += '-';
	int len1 = numerator_.vnum.size(), len2 = denominator_.vnum.size(), i, temp;
	res += int2str_0(numerator_.vnum[0]);
	for (i=1; i<len1; ++i)
		res += int2str(numerator_.vnum[i]);
	if (denominator_ != LongInt("1")){ // fraction
		res += "/";
		res += int2str_0(denominator_.vnum[0]);
		for (i=1; i<len2; ++i)
			res += int2str(denominator_.vnum[i]);
	}
	return new String(res);
}

Base *Rational::equal_(Base *opr2){
	Number *number2 = SCAST_NUMBER(opr2);
	if (number2->type_ != 1)
		return new Boolean(false);
	else{
		Rational *tmp = SCAST_RATIONAL(number2);
		if (numerator_ != tmp->numerator_ or denominator_ != tmp->denominator_)
			return new Boolean(false);
		else
			return new Boolean(true);
	}
}

void Rational::printt(){
	numerator_.print();
	if(denominator_ != LongInt("1")){
		printf("/");
		denominator_.print();
	}
}

void Rational::print(){
	numerator_.print();
	if(denominator_ != LongInt("1")){
		printf("/");
		denominator_.print();
	}
	printf("\n");
}

//Rational *Rational::from_string(char *expression){
    //char* end_pointerr;
    //char *separate_pos = strchr(expression, '/');
    //if(separate_pos){
        //int numerator = (int)strtol(expression,&end_pointerr,10);
        //if( end_pointerr == expression || end_pointerr != separate_pos )
        	//return NULL;
        //int denominator = (int)strtol(separate_pos + 1,&end_pointerr,10);
        //if( end_pointerr == separate_pos + 1 || end_pointerr != expression + strlen(expression) )
        	//return NULL;
        //return new Rational(numerator,denominator);
    //}else{
    	//int numerator = (int)strtol(expression,&end_pointerr,10);
        //if( end_pointerr == expression || end_pointerr != expression + strlen(expression) )
        	//return NULL;
        //return new Rational(numerator , 1);
    //}
//}

Rational *Rational::from_string(const char *expression){
    int i, len = strlen(expression);
    bool flag1 = true, flag2 = false;
    string numerator = "", denominator = "";
	//cout << "Rational::from_string: " << endl;
    if(expression[len-1] == 'i') // Complex
   			return NULL;
   	if(expression[0] == '#') // Boolean
   		return NULL;
    
   	for (i=0; i<len; ++i){
   		if(expression[i] == '.' or expression[i] == 'e') // float;
    		return NULL;
    	if(expression[i] == '-') // fill numerator
    		numerator += "-";
    	else if(expression[i] - '0' >= 0 and expression[i] - '0' <= 9 and flag1)
    		numerator += expression[i];
    	else
   			flag1 = false;
		if(expression[i] == '/') // fill denominator
			flag2 = true;

		else if (expression[i] - '0' >= 0 and expression[i] - '0' <= 9 and flag2)
			denominator += expression[i];
	}
	if (!flag2)
		denominator += '1';
		
	if (numerator == "" or numerator == "-" or (flag2 and (denominator == "" or denominator == "-")))
		return NULL;
	
	else{
		while(numerator[0] == '0' and numerator.size() !=1)
			numerator.erase(0,1);
		while(numerator[0] == '-' and numerator[1] == '0' and numerator.size() != 2)
			numerator.erase(1,1);
		while(denominator[0] == '0' and denominator.size() != 1)
			denominator.erase(0,1);
		return new Rational(numerator, denominator);
	}
}

//int main(){
 	//Rational *a = new Rational("-2","3");
 	//Rational *b = new Rational("4","5");
 	//a->numerator_.print();
 	//cout << "/";
	//a->denominator_.print();
	//cout << endl;
 	//(a->div(b))->print();
 	//delete a;
 	//delete b;
//}
