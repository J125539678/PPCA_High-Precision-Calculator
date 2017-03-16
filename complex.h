# ifndef COMPLEX_H
# define COMPLEX_H
# include "float.h"
# include "rational.h"
# include "number.h"
# include <string>
#define SCAST_COMPLEX(x) static_cast<Complex*>(x)
//using namespace std;

class Complex : public Number{
public:
	Number *real_;
	Number *imag_;
		
	Complex(string real = "0", string imag = "0");
	Complex(Number *real, Number *imag);
	virtual ~Complex();
		
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
    virtual Base *equal_(Base *opr);
	
	virtual void printt();
	virtual void print();
	static Complex *from_string(const char *expression);
};

#endif
