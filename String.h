# pragma once
# include "base.h"
# include <string>
using namespace std;

class String : public Base{
public:
	String(string str = "");
	String(const String *str);
	virtual ~String();
	
	virtual void print();
	static String *from_string(const char *expression);
	string str_;
	
	virtual Base *str_equal(Base *str2);
	virtual Base *str_ci_equal(Base *str2);
	virtual Base *str_less(Base *str2);
	virtual Base *str_ci_less(Base *str2);
	virtual Base *str_less_equal(Base *str2);
	virtual Base *str_ci_less_equal(Base *str2);
	virtual Base *make_str_1(Base *k);
	virtual Base *make_str_2(Base *k, Base *char1);
	virtual Base *string_(Base *char1);
	virtual Base *str_len();
	virtual Base *str_ref(Base *k);
	virtual Base *sub_str_1(Base *start);
	virtual Base *sub_str_2(Base *start, Base *end);
	virtual Base *str_append(Base *str2);
	virtual Base *str_copy(Base *str2);
	virtual Base *str_to_num();
	
	virtual Base *equal_(Base *opr2);
};
