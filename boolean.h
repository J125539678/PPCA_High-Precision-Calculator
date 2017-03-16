# pragma once
# include "base.h"

class Boolean : public Base{
public:
	Boolean(bool flag = true);
	virtual ~Boolean();

	virtual void print();
	static Boolean *from_string(const char *expression);
	bool flag_;

	virtual Base *not_();
	virtual Base *equal_(Base *opr2);
};
