# pragma once
# include "base.h"

class Char : public Base{
public:
	Char(char ch = '\0');
	Char(const Char *ch);
	virtual ~Char();
	
	virtual void print();
	static Char *from_string(const char *expression);	
	char ch_;
	
	virtual Base *char_equal(Char *ch2);
	virtual Base *char_ci_equal(Char *ch2);
	virtual Base *char_less(Char *ch2);
	virtual Base *char_ci_less(Char *ch2);
	virtual Base *char_less_equal(Char *ch2);
	virtual Base *char_ci_less_equal(Char *ch2);
	virtual Base *char_alp();
	virtual Base *char_num();
	virtual Base *char_space();
	virtual Base *char_upper();
	virtual Base *char_lower();
	virtual Base *char_to_int();
	virtual Base *char_up();
	virtual Base *char_down();
	
	virtual Base *equal_(Base *opr2);
};
