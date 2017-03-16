# include "char.h"
# include "number.h"
# include "boolean.h"
# include "rational.h"
# include <cstdio>
# include <cstring>
# include <string>
# include <sstream>
#define SCAST_NUMBER(x) static_cast<Number*>(x)
#define SCAST_RATIONAL(x) static_cast<Rational*>(x)
#define SCAST_FLOAT(x) static_cast<Float*>(x)
#define SCAST_COMPLEX(x) static_cast<Complex*>(x)
#define SCAST_BOOLEAN(x) static_cast<Boolean*>(x)
#define SCAST_CHARACTER(x) static_cast<Char*>(x)
#define SCAST_STRING(x) static_cast<String*>(x)
using namespace std;

Char::Char(char ch){
	ch_ = ch;
	unitType = CHARACTER;
}

Char::Char(const Char *ch){
	ch_ = ch->ch_;
	unitType = CHARACTER;
}

Char::~Char(){
	
}

Base *Char::char_equal(Char *ch2){
	Char *tmp = SCAST_CHARACTER(ch2);
	if (tmp->ch_ == ch_) 
		return new Boolean(true);
	else 
		return new Boolean(false);
}

Base *Char::char_ci_equal(Char *ch2){
	Char *tmp1 = SCAST_CHARACTER(char_down());
	Char *tmp2 = SCAST_CHARACTER(ch2->char_down());
	return new Boolean(tmp1->ch_ == tmp2->ch_);
}

Base *Char::char_less(Char *ch2){
	Char *tmp = SCAST_CHARACTER(ch2);
	if (ch_ < tmp->ch_) 
		return new Boolean(true);
	else 
		return new Boolean(false);
}

Base *Char::char_ci_less(Char *ch2){
	Char *tmp1 = SCAST_CHARACTER(char_down());
	Char *tmp2 = SCAST_CHARACTER(ch2->char_down());
	return new Boolean(tmp1->ch_ < tmp2->ch_);
}

Base *Char::char_less_equal(Char *ch2){
	Char *tmp = SCAST_CHARACTER(ch2);
	if (ch_ <= tmp->ch_) 
		return new Boolean(true);
	else 
		return new Boolean(false);
}

Base *Char::char_ci_less_equal(Char *ch2){
	Char *tmp1 = SCAST_CHARACTER(char_down());
	Char *tmp2 = SCAST_CHARACTER(ch2->char_down());
	return new Boolean(tmp1->ch_ <= tmp2->ch_);
}

Base *Char::char_alp(){
	if ((ch_ >= 'a' and ch_ <= 'z') or (ch_ >= 'A' and ch_ <= 'Z'))
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Char::char_num(){
	if (ch_ >= '0' and ch_ <= '9')
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Char::char_space(){
	if (ch_ == '\n' or ch_ == ' ' or ch_ == '\r' or ch_ == '\t' or ch_ == '\f')
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Char::char_upper(){
	if (ch_ >= 'A' and ch_ <= 'Z')
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Char::char_lower(){
	if (ch_ >= 'a' and ch_ <= 'z')
		return new Boolean(true);
	else
		return new Boolean(false);
}

Base *Char::char_up(){
	if (ch_ >= 'a' and ch_ <= 'z')
		return new Char(ch_+('A'-'a'));
	else
		return new Char(ch_);
}

Base *Char::char_down(){
	if (ch_ >= 'A' and ch_ <= 'Z')
		return new Char(ch_-('A'-'a'));
	else
		return new Char(ch_);
}

Base *Char::char_to_int(){
	int temp = int(ch_);
	string str;
	stringstream ss;
	ss << temp;
	ss >> str;
	return new Rational(str,"1");
}

Base *Char::equal_(Base *opr2){
	return char_equal(SCAST_CHARACTER(opr2));
}

void Char::print(){
	if (ch_ == '\n')
		printf("#\\newline\n");
	else if (ch_ == ' ')
		printf("#\\space\n");
	else if (ch_ == '\r')
		printf("#\\return\n");
	else if (ch_ == '\t')
		printf("#\\tab\n");
	else if (ch_ == '\f')
		printf("#\\page\n");
	else if (ch_ == '\b')
		printf("#\\backspace\n");
	else
		printf("#\\%c\n", ch_);
}

Char *Char::from_string(const char *expression){
	int i, len = strlen(expression);
	if (len < 3)
		return NULL;
	if (expression[0] != '#' or expression[1] != '\\')
		return NULL;
	if (len == 3)
		return new Char(expression[2]);
	
	string str = "";
	for (i=2; i<len; ++i)
		str += expression[i];
		
	if (str == "newline")
		return new Char('\n');
	else if (str == "space")
		return new Char(' ');
	else if (str == "return")
		return new Char('\r');
	else if (str == "tab")
		return new Char('\t');
	else if (str == "page")
		return new Char('\f');
	else if (str == "backspace")
		return new Char('\b');
	else
		return NULL;
}
