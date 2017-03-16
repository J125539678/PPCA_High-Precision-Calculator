# pragma once
#define SCAST_NUMBER(x) static_cast<Number*>(x)
#define SCAST_RATIONAL(x) static_cast<Rational*>(x)
#define SCAST_FLOAT(x) static_cast<Float*>(x)
#define SCAST_COMPLEX(x) static_cast<Complex*>(x)
#define SCAST_BOOLEAN(x) static_cast<Boolean*>(x)
#define SCAST_CHARACTER(x) static_cast<Char*>(x)
#define SCAST_STRING(x) static_cast<String*>(x)

class Base{
public:
	enum{
		BOOLEAN = 1,
		NUMBER = 2,
		CHARACTER = 3,
		STRING = 4
	} unitType;
	Base(){}
	virtual ~Base(){}
	virtual void print() = 0;
	
	virtual Base *equal_(Base *opr2) = 0;
};
