# include "String.h"
# include "boolean.h"
# include "long_int.h"
# include "number.h"
# include "rational.h"
# include "float.h"
# include "complex.h"
# include "char.h"
# include <string>
# include <iostream>
# include <cstring>
# include <cassert>
# include <sstream>
#define SCAST_NUMBER(x) static_cast<Number*>(x)
#define SCAST_RATIONAL(x) static_cast<Rational*>(x)
#define SCAST_FLOAT(x) static_cast<Float*>(x)
#define SCAST_COMPLEX(x) static_cast<Complex*>(x)
#define SCAST_BOOLEAN(x) static_cast<Boolean*>(x)
#define SCAST_CHARACTER(x) static_cast<Char*>(x)
#define SCAST_STRING(x) static_cast<String*>(x)

String::String(string str){
	str_ = str;
	unitType = STRING;
}

String::String(const String *str){
	str_ = str->str_;
	unitType = STRING;
}

String::~String(){
	
}

string str_ci(string str){
	string res = "";
	int i, len = str.size();
	for (i=0; i<len; ++i){
		if (str[i] >= 'A' and str[i] <= 'Z')
			res += str[i] - ('A' - 'a');
		else
			res += str[i];
	}
	return res;
}

Base *String::str_equal(Base *str2){
	String *tmp = SCAST_STRING(str2);
	if(tmp->str_ == str_) 
		return new Boolean(true);
	else 
		return new Boolean(false);
}

Base *String::str_ci_equal(Base *str2){
	String *tmp = SCAST_STRING(str2);
	string tmp1 = str_ci(str_), tmp2 = str_ci(tmp->str_);
	if (tmp1 == tmp2) 
		return new Boolean(true);
	else 
		return new Boolean(false);
}

Base *String::str_less(Base *str2){
	String *tmp = SCAST_STRING(str2);
	if(str_ < tmp->str_ ) 
		return new Boolean(true);
	else 
		return new Boolean(false);
}

Base *String::str_ci_less(Base *str2){
	String *tmp = SCAST_STRING(str2);
	string tmp1 = str_ci(str_), tmp2 = str_ci(tmp->str_);
	if (tmp1 < tmp2) 
		return new Boolean(true);
	else 
		return new Boolean(false);
}

Base *String::str_less_equal(Base *str2){
	String *tmp = SCAST_STRING(str2);
	if(str_ <= tmp->str_ ) 
		return new Boolean(true);
	else 
		return new Boolean(false);
}

Base *String::str_ci_less_equal(Base *str2){
	String *tmp = SCAST_STRING(str2);
	string tmp1 = str_ci(str_), tmp2 = str_ci(tmp->str_);
	if (tmp1 <= tmp2) 
		return new Boolean(true);
	else 
		return new Boolean(false);
}

Base *String::make_str_1(Base *k){
	if(k->unitType != 2)
		assert(0 && "expected integers!");
	Number *num = SCAST_NUMBER(k);
	if(num->type_ != 1)
		assert(0 && "expected integers!");
	Rational *tmp = SCAST_RATIONAL(num);
	if(tmp->denominator_!=LongInt("1"))
		assert(0 && "expected integers!");
	String *res=new String;
	int len = (int)(double)tmp->numerator_;
	for(int i=0;i<len;++i)
		res->str_ += "\u0000";
	return res;
}

Base *String::make_str_2(Base *k, Base *char1){
	Number *num = SCAST_NUMBER(k);
	Char *char1_ = SCAST_CHARACTER(char1);
	if(num->type_ != 1)
		assert(0 && "expected integers!");
	Rational *tmp = SCAST_RATIONAL(num);
	if(tmp->denominator_!=LongInt("1"))
		assert(0 && "expected integers!");
	String *res=new String;
	int len=(int)(double)tmp->numerator_;
	for(int i=0;i<len;++i)
		res->str_ += char1_->ch_;
	return res;
}

Base *String::string_(Base *char1){
	Char *char1_ = SCAST_CHARACTER(char1);
	String *res = new String(str_);
	res->str_ += char1_->ch_;
	return res; 
}

Base *String::str_len(){
	int len = str_.size();
	string str;
	stringstream ss;
	ss << len;
	ss >> str;
	return new Rational(str,"1");
}

Base *String::str_ref(Base *k){
	Number *num = SCAST_NUMBER(k);
	if (num->type_ != 1)
		assert(0 && "expected integer");
	Rational *num1 = SCAST_RATIONAL(num);
	if (num1->denominator_ != LongInt("1"))
		assert(0 && "expected integer");
	int pos=(int)(double)num1->numerator_;
	if (pos < 0 or pos >= str_.size())
		assert(0 && "given integer beyond scope");
	return new Char(str_[pos]);
}

Base *String::sub_str_1(Base *start){
	Number *num1 = SCAST_NUMBER(start);
	if(num1->type_ != 1)
		assert(0 && "expected integer");
	Rational *tmp1 = SCAST_RATIONAL(num1);
	if(tmp1->denominator_ !=LongInt("1"))
		assert(0 && "expected integer");
	int pos1 = (int)(double)tmp1->numerator_, len = str_.size(), i;
	if(pos1 < 0 or pos1>=len)
		assert(0 && "given integer beyond scope");
	String *res = new String();
	for(i=pos1; i<len; ++i)
		res->str_ += str_[i];
	return res;
}

Base *String::sub_str_2(Base *start, Base *end){
	Number *num1 = SCAST_NUMBER(start), *num2 = SCAST_NUMBER(end);
	if(num1->type_ != 1 or num2->type_ != 1)
		assert(0 && "expected integer");
	Rational *tmp1 = SCAST_RATIONAL(num1), *tmp2 = SCAST_RATIONAL(num2);
	if(tmp1->denominator_ !=LongInt("1") or tmp2->denominator_ !=LongInt("1"))
		assert(0 && "expected integer");
	int pos1 = (int)(double)tmp1->numerator_, pos2 = (int)(double)tmp2->numerator_, len = str_.size(), i;
	if(pos1 < 0 or pos1>=len or pos2 < pos1)
		assert(0 && "given integer beyond scope");
	String *res = new String();
	for(i=pos1; i<pos2; ++i)
		res->str_ += str_[i];
	return res;
}

Base *String::str_append(Base *str2){
	String *str2_ = SCAST_STRING(str2);
	return new String(str_ + str2_->str_);
}

Base *String::str_copy(Base *str2){
	return new String(str_);
}

Base *String::str_to_num(){
	Base *res;
	res = Rational::from_string(str_.c_str());
	if(!res) res = Float::from_string(str_.c_str());
	if(!res) res = Complex::from_string(str_.c_str());
	if(!res) return new Boolean(false);
	return res;
}

Base *String::equal_(Base *opr2){
	return str_equal(opr2);
}

void String::print(){
	cout << "\"" << str_ << "\"" << endl;
}

String *String::from_string(const char *expression){
	int len = strlen(expression), i;
	if (len < 2) // not a string
		return NULL;
	if (expression[0] != '\"' or expression[len-1] != '\"') // not a string
		return NULL;
		
	string s = "";
	
	for(i=1; i<len-1; ++i){
		if(expression[i] == '\\'){
			++i;
			if (expression[i] == 'n')
				s += '\n';
			else if (expression[i] == 't')
				s += '\t';
			else if (expression[i] == 'f')
				s += '\f';
			else if (expression[i] == 'r')
				s += '\r';
			else if (expression[i] == 'b')
				s += '\b';
			else
				s += expression[i];
		}
		else
			s += expression[i];
	}
	
	return new String(s);
}
