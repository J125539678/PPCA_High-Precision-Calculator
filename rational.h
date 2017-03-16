#pragma once
#include "number.h"
#include "long_int.h"
using namespace std;
#define SCAST_RATIONAL(x) static_cast<Rational*>(x)

class Rational : public Number {
public:
	Rational(string numerator = "0", string denominator = "1");
	Rational(LongInt numerator = LongInt("0"), LongInt denominator = LongInt("1"));
	virtual ~Rational();

	void reduce();
	virtual Number *convert(Number *number2);
	virtual Number *add(Number *number2);
	virtual Number *sub(Number *number2);
	virtual Number *mul(Number *number2);
	virtual Number *div(Number *number2);
	
	virtual Number *abs();
	virtual Number *quotient(Number *number2);
	virtual Number *remainder(Number *number2);
	virtual Number *modulo(Number *number2);
	virtual Number *gcd(Number *number2);
	virtual Number *lcm(Number *number2);
	virtual Number *max_(Number *number2);
	virtual Number *min_(Number *number2);
	virtual Number *expt(Number *number2);
	virtual Number *sqrt_();
	virtual Number *numerator();
	virtual Number *denominator();
	virtual Number *floor_();
	virtual Number *ceiling_();
	virtual Number *truncate_();
	virtual Number *round_();
	virtual Number *inexact_exact();
	virtual Number *exact_inexact();
	virtual Number *real_part();
	virtual Number *imag_part();
	
	virtual Number *sin_();
	virtual Number *cos_();
	virtual Number *tan_();
	virtual Number *asin_();
	virtual Number *acos_();
	virtual Number *atan_();
	virtual Number *exp_();
	virtual Number *log_();
	virtual Number *make_rec(Number *number2);
	virtual Number *make_pol(Number *number2);
	virtual Number *magnitude_();
	virtual Number *angle_();
	
	virtual Base *is_zero();
    virtual Base *is_negative();
    virtual Base *is_positive();
    virtual Base *is_odd();
    virtual Base *is_even();
    virtual Base *is_integer();
	virtual Base *is_rational();
	virtual Base *is_real();
    virtual Base *is_complex();
    virtual Base *is_exact();
    virtual Base *is_inexact();
    virtual Base *less(Number *number2);
    virtual Base *less_equal(Number *number2);
    virtual Base *more(Number *number2);
    virtual Base *more_equal(Number *number2);
    
    virtual Base *int_to_char();
    virtual Base *num_to_str();
    virtual Base *equal_(Base *opr2);
	
	virtual void printt();
	virtual void print();
	static Rational *from_string(const char *expression);
	LongInt numerator_;
	LongInt denominator_;

};
