# pragma once
# include "base.h"
# include "boolean.h"
#define SCAST_NUMBER(x) static_cast<Number*>(x)
#define SCAST_RATIONAL(x) static_cast<Rational*>(x)
#define SCAST_FLOAT(x) static_cast<Float*>(x)
#define SCAST_COMPLEX(x) static_cast<Complex*>(x)
#define SCAST_BOOLEAN(x) static_cast<Boolean*>(x)

class Number : public Base {
public:
	enum{
		RATIONAL = 1,
		FLOAT = 2,
		COMPLEX = 3
	} type_;
	Number(){unitType = NUMBER;}
	virtual ~Number(){}
	
	virtual Number *convert(Number *number2) = 0;
	virtual Number *add(Number *number2) = 0;
	virtual Number *sub(Number *number2) = 0;
	virtual Number *mul(Number *number2) = 0;
	virtual Number *div(Number *number2) = 0;
	
	virtual Number *abs() = 0;
	virtual Number *quotient(Number *number2) = 0;
	virtual Number *remainder(Number *number2) = 0;
	virtual Number *modulo(Number *number2) = 0;
	virtual Number *gcd(Number *number2) = 0;
	virtual Number *lcm(Number *number2) = 0;
	virtual Number *max_(Number *number2) = 0;
	virtual Number *min_(Number *number2) = 0;
	virtual Number *expt(Number *number2) = 0;
	virtual Number *sqrt_() = 0;
	virtual Number *numerator() = 0;
	virtual Number *denominator() = 0;
	virtual Number *floor_() = 0;
	virtual Number *ceiling_() = 0;
	virtual Number *truncate_() = 0;
	virtual Number *round_() = 0;
	virtual Number *inexact_exact() = 0;
	virtual Number *exact_inexact() = 0;
	virtual Number *real_part() = 0;
	virtual Number *imag_part() = 0;
	
	virtual Number *sin_() = 0;
	virtual Number *cos_() = 0;
	virtual Number *tan_() = 0;
	virtual Number *asin_() = 0;
	virtual Number *acos_() = 0;
	virtual Number *atan_() = 0;
	virtual Number *exp_() = 0;
	virtual Number *log_() = 0;
	virtual Number *make_rec(Number *number2) = 0;
	virtual Number *make_pol(Number *number2) = 0;
	virtual Number *magnitude_() = 0;
	virtual Number *angle_() = 0;
	
	virtual Base *is_zero() = 0 ;
    virtual Base *is_negative() = 0;
    virtual Base *is_positive() = 0;
    virtual Base *is_odd() = 0;
    virtual Base *is_even() = 0;
    virtual Base *is_integer() = 0;
	virtual Base *is_rational() = 0;
	virtual Base *is_real() = 0;
    virtual Base *is_complex() = 0;
    virtual Base *is_exact() = 0;
    virtual Base *is_inexact() = 0;
    virtual Base *less(Number *number2) = 0;
    virtual Base *less_equal(Number *number2) = 0;
    virtual Base *more(Number *number2) = 0;
    virtual Base *more_equal(Number *number2) = 0;
    
    virtual Base *int_to_char() = 0;
    virtual Base *num_to_str() = 0;
    virtual Base *equal_(Base *opr2) = 0;
	
	virtual void printt() = 0;
	virtual void print() = 0;
};
