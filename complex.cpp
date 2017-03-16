# include "complex.h"
# include "float.h"
# include "rational.h"
# include "number.h"
# include "String.h"
# include "char.h"
# include "boolean.h"
# include <string>
# include <algorithm>
# include <iostream>
# include <cassert>
# include <math.h>
# include <stdio.h>
# include <string>
# include <cstring>
# include <complex>
using namespace std;

Complex::Complex(string real, string imag){
	type_ = COMPLEX;
	Number* res1 = Rational::from_string(real.c_str());
	Number* res2 = Rational::from_string(imag.c_str());
	if (!res1&&res2){
		res1 = Float::from_string(real.c_str());
		Number* res = res1->convert(res2);
		res2 = res;
	}
	else if (!res2&&res1){
		res2 = Float::from_string(imag.c_str());
		Number* res = res2->convert(res1);
		res1 = res;
	}
	else if (!res1&&!res2){
		res1 = Float::from_string(real.c_str());
		res2 = Float::from_string(imag.c_str());
	}
	if (!res1 || !res2) throw 0;
	real_ = res1;
	imag_ = res2;
	//cout << "from_string" << endl;
	//print();
}

/*Complex::Complex(Number *real, Number *imag) : real_(real), imag_(imag){
	type_ = COMPLEX;
	if (real->type_ != imag->type_){
		if (real->type_ == 2)
			imag_ = real->convert(imag);
		else if (imag->type_ == 2)
			real_ = imag->convert(real);
	}
}*/

Complex::Complex(Number *real, Number *imag){
	real_ = real;
	imag_ = imag;
	type_ = COMPLEX;
	if (real->type_ == 2 or imag->type_ == 2){
		Float *real = new Float();
		Float *imag = new Float();
		if (real_->type_ < 2)
			real_ = real->convert(real_);
		if (imag_->type_ < 2)
			imag_ = imag->convert(imag_);
	}
	//cout << "from_pointer" << endl;
	//print();
}

Complex::~Complex(){
	//delete real_, imag_;
}

Number *Complex::convert(Number *number2){
	assert(number2->type_ <= type_);
	Complex *result = new Complex();
	switch(number2->type_){
		case RATIONAL:{
			Rational *tmp = SCAST_RATIONAL(number2);
			result->real_ = new Rational(tmp->numerator_, tmp->denominator_);
			result->imag_ = new Rational("0", "1");
			break;
		}
		case FLOAT:{
			Float *tmp = SCAST_FLOAT(number2);
			result->real_ = new Float(tmp->number_);
			result->imag_ = new Float();
			break;
		}
		case COMPLEX:{
			Complex *tmp = SCAST_COMPLEX(number2);
			if (tmp->real_->type_ == FLOAT){
				Float *real = SCAST_FLOAT(tmp->real_);
				Float *imag = SCAST_FLOAT(tmp->imag_);
				result->real_ = new Float(real->number_);
				result->imag_ = new Float(imag->number_);
			}
			else{
				Rational *real = SCAST_RATIONAL(tmp->real_);
				Rational *imag = SCAST_RATIONAL(tmp->imag_);
				result->real_ = new Rational(real->numerator_, real->denominator_);
				result->imag_ = new Rational(imag->numerator_, imag->denominator_);
			}
			break;
		}
		default:
			assert(0 && "type_ not defined");
	}
	return result;
}

Number *Complex::add(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	Complex *result = new Complex();

	if (real_->type_ == tmp->real_->type_){
		result->real_ = real_->add(tmp->real_);
		result->imag_ = imag_->add(tmp->imag_);
	}
	else{
		if (real_->type_ == RATIONAL){
			Number *tmp1 = tmp->real_->convert(real_), *tmp2 = tmp->real_->convert(imag_);
			result->real_ = tmp->real_->add(tmp1); result->imag_=tmp->imag_->add(tmp2);
		}
		else{
			Number *tmp1 = real_->convert(tmp->real_), *tmp2 = imag_->convert(tmp->imag_);
			result->real_ = real_->add(tmp1); result->imag_ = imag_->add(tmp2);
		}
	}
	return result;
}

Number *Complex::sub(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	Complex *result = new Complex();

	if (real_->type_ == tmp->real_->type_){
		result->real_ = real_->sub(tmp->real_);
		result->imag_ = imag_->sub(tmp->imag_);
	}
	else{
		if (real_->type_ == RATIONAL){
			Number *tmp1 = tmp->real_->convert(real_), *tmp2 = tmp->real_->convert(imag_);
			result->real_ = tmp1->sub(tmp->real_); result->imag_ = tmp2->sub(tmp->imag_);
		}
		else{
			Number *tmp1 = real_->convert(tmp->real_), *tmp2 = imag_->convert(tmp->imag_);
			result->real_ = real_->sub(tmp1); result->imag_ = imag_->sub(tmp2);
		}
	}

	return result;
}

Number *Complex::mul(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	Complex *result = new Complex();

	if (real_->type_ == tmp->real_->type_){
		result->real_ = (real_->mul(tmp->real_))->sub(imag_->mul(tmp->imag_));
		result->imag_ = (real_->mul(tmp->imag_))->add(imag_->mul(tmp->real_));
	}
	else{
		if (real_->type_ == RATIONAL){
			result->real_ = ((tmp->real_->convert(real_))->mul(tmp->real_))->sub((tmp->real_->convert(imag_))->mul(tmp->imag_));
			result->imag_ = ((tmp->real_->convert(real_))->mul(tmp->imag_))->add((tmp->real_->convert(imag_))->mul(tmp->real_));
		}
		else{
			result->real_ = (real_->mul(real_->convert(tmp->real_)))->sub(imag_->mul(imag_->convert(tmp->imag_)));
			result->imag_ = (real_->mul(imag_->convert(tmp->imag_)))->add(imag_->mul(real_->convert(tmp->real_)));
		}
	}
	return result;
}

Number *Complex::div(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	//assert(tmp->real_ && "divide zero");
	Complex *result = new Complex();

	if (real_->type_ == tmp->real_->type_){
		result->real_ = ((real_->mul(tmp->real_))->add((imag_->mul(tmp->imag_))))->div(((tmp->real_)->mul(tmp->real_))->add((tmp->imag_)->mul(tmp->imag_)));
		result->imag_ = ((imag_->mul(tmp->real_))->sub((real_->mul(tmp->imag_))))->div(((tmp->real_)->mul(tmp->real_))->add((tmp->imag_)->mul(tmp->imag_)));
	}
	else if (real_->type_ == RATIONAL&&tmp->real_->type_ == FLOAT){
		Number *tmpreal_ = tmp->real_->convert(real_);
		Number *tmpimag_ = tmp->imag_->convert(imag_);
		result->real_ = ((tmpreal_->mul(tmp->real_))->add((tmpimag_->mul(tmp->imag_))))->div(((tmp->real_)->mul(tmp->real_))->add((tmp->imag_)->mul(tmp->imag_)));
		result->imag_ = ((tmpimag_->mul(tmp->real_))->sub((tmpreal_->mul(tmp->imag_))))->div(((tmp->real_)->mul(tmp->real_))->add((tmp->imag_)->mul(tmp->imag_)));
		delete tmpreal_, tmpimag_;
	}
	else{
		Number *tmpreal_ = real_->convert(tmp->real_);
		Number *tmpimag_ = imag_->convert(tmp->imag_);
		result->real_ = ((real_->mul(tmpreal_))->add((imag_->mul(tmpimag_))))->div(((tmpreal_)->mul(tmpreal_))->add((tmpimag_)->mul(tmpimag_)));
		result->imag_ = ((imag_->mul(tmpreal_))->sub((real_->mul(tmpimag_))))->div(((tmpreal_)->mul(tmpreal_))->add((tmpimag_)->mul(tmpimag_)));
		delete tmpreal_, tmpimag_;
	}

	return result;
}

Number *Complex::abs(){
	assert(0 && "absolute complex");
	return NULL;
}

Number *Complex::quotient(Number *number2){
	assert(0 && "quotient complex");
	return NULL;
}

Number *Complex::remainder(Number *number2){
	assert(0 && "remainder complex");
	return NULL;
}

Number *Complex::modulo(Number *number2){
	assert(0 && "modulo complex");
	return NULL;
}

Number *Complex::gcd(Number *number2){
	assert(0 && "gcd complex");
	return NULL;
}

Number *Complex::lcm(Number *number2){
	assert(0 && "lcm complex");
	return NULL;
}

Number *Complex::max_(Number *number2){
	assert(0 && "max complex");
	return NULL;
}

Number *Complex::min_(Number *number2){
	assert(0 && "min complex");
	return NULL;
}

Number* Complex::expt(Number* number2){
	Complex* tmp = SCAST_COMPLEX(number2);
	complex<double> base, power;
	if (real_->type_ == RATIONAL){
		Rational *tmp1 = SCAST_RATIONAL(real_), *tmp2 = SCAST_RATIONAL(imag_);
		base.real((double)tmp1->numerator_ / (double)tmp1->denominator_);
		base.imag((double)tmp2->numerator_ / (double)tmp2->denominator_);
	}
	else if (real_->type_ == FLOAT){
		Float *tmp1 = SCAST_FLOAT(real_), *tmp2 = SCAST_FLOAT(imag_);
		base.real(tmp1->number_);
		base.imag(tmp2->number_);
	}
	if (tmp->real_->type_ == RATIONAL){
		Rational *tmp1 = SCAST_RATIONAL(tmp->real_), *tmp2 = SCAST_RATIONAL(tmp->imag_);
		power.real ((double)tmp1->numerator_ / (double)tmp1->denominator_);
		power.imag ((double)tmp2->numerator_ / (double)tmp2->denominator_);
	}
	else if (tmp->real_->type_ == FLOAT){
		Float *tmp1 = SCAST_FLOAT(tmp->real_), *tmp2 = SCAST_FLOAT(tmp->imag_);
		power.real (tmp1->number_);
		power.imag (tmp2->number_);
	}
	complex<double> result = pow(base, power);
	double r = result.real(), i = result.imag();
	return new Complex(new Float(r),new Float(i));
}

Number *Complex::sqrt_(){
	complex<double> base;
	if (real_->type_ == RATIONAL){
		Rational *tmp1 = SCAST_RATIONAL(real_), *tmp2 = SCAST_RATIONAL(imag_);
		base.real((double)tmp1->numerator_ / (double)tmp1->denominator_);
		base.imag((double)tmp2->numerator_ / (double)tmp2->denominator_);
	}
	else if (real_->type_ == FLOAT){
		Float *tmp1 = SCAST_FLOAT(real_), *tmp2 = SCAST_FLOAT(imag_);
		base.real(tmp1->number_);
		base.imag(tmp2->number_);
	}
	complex<double> result = sqrt(base);
	double r = result.real(), i = result.imag();
	return new Complex(new Float(r),new Float(i));
}

Number *Complex::floor_(){
	assert(0 && "floor complex");
	return NULL;
}

Number *Complex::ceiling_(){
	assert(0 && "ceiling complex");
	return NULL;
}

Number *Complex::round_(){
	assert(0 && "round complex");
	return NULL;
}

Number *Complex::truncate_(){
	assert(0 && "truncate complex");
	return NULL;
}

Number *Complex::numerator(){
	assert(0 && "numerator complex");
	return NULL;
}

Number *Complex::denominator(){
	assert(0 && "denominator complex");
	return NULL;
}

Number* Complex::inexact_exact(){
	Complex* result = new Complex();
	if (real_->type_ == RATIONAL){
		Rational* tmp1 = SCAST_RATIONAL(real_), *tmp2 = SCAST_RATIONAL(imag_);
		result->real_ = new Rational(tmp1->numerator_, tmp1->denominator_);
		result->imag_ = new Rational(tmp2->numerator_, tmp2->denominator_);
	}
	else{
		Float* tmp1 = SCAST_FLOAT(real_), *tmp2 = SCAST_FLOAT(imag_);
		result->real_ = tmp1->inexact_exact();
		result->imag_ = tmp2->inexact_exact();
	}
	return result;
}

Number* Complex::exact_inexact(){
	Complex* result = new Complex();
	if (real_->type_ == RATIONAL){
		Rational* tmp1 = SCAST_RATIONAL(real_), *tmp2 = SCAST_RATIONAL(imag_);
		result->real_ = new Float((double)tmp1->numerator_ / (double)tmp1->denominator_);
		result->imag_ = new Float((double)tmp2->numerator_ / (double)tmp2->denominator_);
		return result;
	}
	else{
		Float* tmp1 = SCAST_FLOAT(real_), *tmp2 = SCAST_FLOAT(imag_);
		result->real_ = new Float(tmp1->number_);
		result->imag_ = new Float(tmp2->number_);
		return result;
	}
}

Number *Complex::real_part(){
	return real_->real_part();
}

Number *Complex::imag_part(){
	return imag_->real_part();
}

Number *Complex::sin_(){
	Complex *tmp = SCAST_COMPLEX(exact_inexact());
	complex<double> c_x(SCAST_FLOAT(tmp->real_)->number_, SCAST_FLOAT(tmp->imag_)->number_);
	complex<double> c_res = sin(c_x);
	Complex *res = new Complex;
	res->real_ = new Float(c_res.real());
	res->imag_ = new Float(c_res.imag());
	delete tmp;
	return res;
}

Number *Complex::cos_(){
	Complex *tmp = SCAST_COMPLEX(exact_inexact());
	complex<double> c_x(SCAST_FLOAT(tmp->real_)->number_, SCAST_FLOAT(tmp->imag_)->number_);
	complex<double> c_res = cos(c_x);
	Complex *res = new Complex;
	res->real_ = new Float(c_res.real());
	res->imag_ = new Float(c_res.imag());
	delete tmp;
	return res;
}

Number *Complex::tan_(){
	Complex *tmp = SCAST_COMPLEX(exact_inexact());
	complex<double> c_x(SCAST_FLOAT(tmp->real_)->number_, SCAST_FLOAT(tmp->imag_)->number_);
	complex<double> c_res = tan(c_x);
	Complex *res = new Complex;
	res->real_ = new Float(c_res.real());
	res->imag_ = new Float(c_res.imag());
	delete tmp;
	return res;
}

Number *Complex::asin_(){
	Float a(-1), b(0), c(1);
	Complex n(&b,&a), e(&c,&b), m(&b,&c);
	return n.mul((e.sub( this->mul(this) )->sqrt_()->add(m.mul(this)))->log_());	
}

Number *Complex::acos_(){
	Float a(3.1415926535897932384626433/2), b(0);
	Complex tmp(&a, &b);
	return tmp.sub(this->asin_());	
}

Number *Complex::atan_(){
	Float a(1), b(0), c(-1), d(2);
	Complex m(&a,&b), n(&b,&a), p(&b,&c), q(&b,&d); //1 , i , -i, 2i 
	return (m.add(n.mul(this))->log_()->sub((m.sub(this->mul(&n)))->log_()))->div(&q);
}

/*Number *Complex::asin_(){
	Complex *tmp = SCAST_COMPLEX(exact_inexact());
	complex<double> c_x(SCAST_FLOAT(tmp->real_)->number_, SCAST_FLOAT(tmp->imag_)->number_);
	complex<double> c_res = asin(c_x);
	Complex *res = new Complex;
	res->real_ = new Float(c_res.real());
	res->imag_ = new Float(c_res.imag());
	delete tmp;
	return res;
}



Number *Complex::acos_(){
	Complex *tmp = SCAST_COMPLEX(exact_inexact());
	complex<double> c_x(SCAST_FLOAT(tmp->real_)->number_, SCAST_FLOAT(tmp->imag_)->number_);
	complex<double> c_res = acos(c_x);
	Complex *res = new Complex;
	res->real_ = new Float(c_res.real());
	res->imag_ = new Float(c_res.imag());
	delete tmp;
	return res;
}

Number *Complex::atan_(){
	Complex *tmp = SCAST_COMPLEX(exact_inexact());
	complex<double> c_x(SCAST_FLOAT(tmp->real_)->number_, SCAST_FLOAT(tmp->imag_)->number_);
	complex<double> c_res = atan(c_x);
	Complex *res = new Complex;
	res->real_ = new Float(c_res.real());
	res->imag_ = new Float(c_res.imag());
	delete tmp;
	return res;
}*/

Number *Complex::exp_(){
	Complex *tmp = SCAST_COMPLEX(exact_inexact());
	complex<double> c_x(SCAST_FLOAT(tmp->real_)->number_, SCAST_FLOAT(tmp->imag_)->number_);
	complex<double> c_res = exp(c_x);
	Complex *res = new Complex;
	res->real_ = new Float(c_res.real());
	res->imag_ = new Float(c_res.imag());
	delete tmp;
	return res;
}

Number *Complex::log_(){
	Complex *tmp = SCAST_COMPLEX(exact_inexact());
	complex<double> c_x(SCAST_FLOAT(tmp->real_)->number_, SCAST_FLOAT(tmp->imag_)->number_);
	complex<double> c_res = log(c_x);
	Complex *res = new Complex;
	res->real_ = new Float(c_res.real());
	res->imag_ = new Float(c_res.imag());
	delete tmp;
	return res;
}

Number *Complex::make_rec(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	if(imag_->type_ == FLOAT or tmp->imag_->type_ == FLOAT){
		Float *conv = new Float();
		Float *tmp1_imag = SCAST_FLOAT(conv->convert(imag_));
		Float *tmp2_imag = SCAST_FLOAT(conv->convert(tmp->imag_));
		if(tmp1_imag->number_ != 0.0 or tmp2_imag->number_ != 0.0)
			assert(0 && "make-rectangular only for real numbers!");
		Float *tmp1_real = SCAST_FLOAT(conv->convert(real_));
		Float *tmp2_real = SCAST_FLOAT(conv->convert(tmp->real_));
		Complex *res = new Complex;
		res->real_ = new Float(tmp1_real->number_);
		res->imag_ = new Float(tmp2_real->number_);
		delete conv, tmp1_imag, tmp2_imag, tmp1_real, tmp2_real;
		return res;
	}
	else{
		Rational *tmp1_imag = SCAST_RATIONAL(imag_);
		Rational *tmp2_imag = SCAST_RATIONAL(tmp->imag_);
		if(tmp1_imag->numerator_ != LongInt("0") or tmp2_imag->numerator_!=LongInt("0"))
			assert(0 && "make-rectangular only for real numbers!");
		Rational *tmp1_real = SCAST_RATIONAL(real_);
		Rational *tmp2_real = SCAST_RATIONAL(tmp->real_);
		Complex *res = new Complex;
		res->real_ = new Rational(tmp1_real->numerator_,tmp1_real->denominator_);
		res->imag_ = new Rational(tmp2_real->numerator_ ,tmp2_real->denominator_);
		delete tmp1_imag, tmp2_imag, tmp1_real, tmp2_real;
		return res;
	}
}

Number *Complex::make_pol(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	if(imag_->type_ == FLOAT or tmp->imag_->type_ == FLOAT){
		Float *conv = new Float;
		Float *tmp1_imag = SCAST_FLOAT(conv->convert(imag_));
		Float *tmp2_imag = SCAST_FLOAT(conv->convert(tmp->imag_));
		if(tmp1_imag->number_ != 0.0 or tmp2_imag->number_ != 0.0)
			assert(0 && "make-rectangular only for real numbers!");
		Float *tmp1_real = SCAST_FLOAT(conv->convert(real_));
		Float *tmp2_real = SCAST_FLOAT(conv->convert(tmp->real_));
		Number *res = tmp1_real->make_pol(tmp2_real);
		delete conv, tmp1_imag, tmp2_imag, tmp1_real, tmp2_real;
		return res;
	}
	else{
		Rational *tmp1_imag = SCAST_RATIONAL(imag_);
		Rational *tmp2_imag = SCAST_RATIONAL(tmp->imag_);
		if(tmp1_imag->numerator_ != LongInt("0") or tmp2_imag->numerator_!=LongInt("0"))
			assert(0 && "make-rectangular only for real numbers!");
		Rational *tmp1_real = SCAST_RATIONAL(real_);
		Rational *tmp2_real = SCAST_RATIONAL(tmp->real_);
		Number *res = tmp1_real->make_pol(tmp2_real);
		delete tmp1_imag, tmp2_imag, tmp1_real, tmp2_real;
		return res;
	}
}

Number *Complex::magnitude_(){
	Float *res = new Float();
	if (real_->type_ == 1){ // Rational
		Float *tmp_real = SCAST_FLOAT(res->convert(real_));
		Float *tmp_imag = SCAST_FLOAT(res->convert(imag_));
		double ans = sqrt(tmp_real->number_ * tmp_real->number_ + tmp_imag->number_ * tmp_imag->number_);
		if (double(round(ans))==ans){
			Float *tmp = new Float(ans);
			Number *res2 = tmp->round_();
			delete tmp, res;
			return res2;
		}
		else{
			res->number_ = ans;
			return res;
		}
	}
	else{ // Float
		Float *tmp_real = SCAST_FLOAT(real_);
		Float *tmp_imag = SCAST_FLOAT(imag_);
		double ans = sqrt(tmp_real->number_ * tmp_real->number_ + tmp_imag->number_ * tmp_imag->number_);
		if (double(round(ans))==ans){
			Float *tmp = new Float(ans);
			Number *res2 = tmp->round_();
			delete tmp, res;
			return res2;
		}
		else{
			res->number_ = ans;
			return res;
		}
	}
}

Number *Complex::angle_(){
	Complex *tmp = SCAST_COMPLEX(exact_inexact());
	complex<double> c_res(SCAST_FLOAT(tmp->real_)->number_,SCAST_FLOAT(tmp->imag_)->number_);
	Float *res = new Float(arg(c_res));
	delete tmp;
	return res;
}

Base *Complex::is_zero(){
	if (real_->type_ == 1){
		Rational *real=SCAST_RATIONAL(real_), *imag=SCAST_RATIONAL(imag_);
		if (real->numerator_ != LongInt("0") or imag->numerator_ != LongInt("0"))
			return new Boolean(false);
		else
			return new Boolean(true);
	}
	else if (real_->type_ == 2){
		Float *real=SCAST_FLOAT(real_), *imag=SCAST_FLOAT(imag_);
		if (real->number_ == 0.0 and imag->number_ == 0.0)
			return new Boolean(true);
		else
			return new Boolean(false);
	}
}

Base *Complex::is_negative(){
	assert(0 && "only for real numbers!");
	return NULL;
}

Base *Complex::is_positive(){
	assert(0 && "only for real numbers!");
	return NULL;
}

Base *Complex::is_odd(){
	assert(0 && "only for real numbers!");
	return NULL;
}
Base *Complex::is_even(){
	assert(0 && "only for real numbers!");
	return NULL;
}

Base *Complex::is_integer(){
	return new Boolean(false);
}

Base *Complex::is_rational(){
	return new Boolean(false);
}

Base *Complex::is_real(){
	return new Boolean(false);
}

Base *Complex::is_complex(){
	return new Boolean(true);
}

Base *Complex::is_exact(){
	return real_->is_exact();
}

Base *Complex::is_inexact(){
	return real_->is_inexact();
}

Base *Complex::less(Number *number2){
	assert(0 && "only for real numbers!");
	return NULL;
}

Base *Complex::less_equal(Number *number2){
	assert(0 && "only for real numbers!");
	return NULL;
}

Base *Complex::more(Number *number2){
	assert(0 && "only for real numbers!");
	return NULL;
}

Base *Complex::more_equal(Number *number2){
	assert(0 && "only for real numbers!");
	return NULL;
}

Base *Complex::int_to_char(){
	assert(0 && "expected integer");
	return NULL;
}

Base *Complex::num_to_str(){
	String *res = new String();
	if(real_->type_ == RATIONAL){
		Rational *real = SCAST_RATIONAL(real_), *imag = SCAST_RATIONAL(imag_);
		if (imag->numerator_.sign)
			res->str_ = SCAST_STRING(real->num_to_str())->str_ + "+" + SCAST_STRING(imag->num_to_str())->str_ + "i";
		else
			res->str_ = SCAST_STRING(real->num_to_str())->str_ + SCAST_STRING(imag->num_to_str())->str_ + "i";
	}
	else if(real_->type_ == FLOAT){
		Float *real = SCAST_FLOAT(real_), *imag = SCAST_FLOAT(imag_);
		if (imag->number_ > 0)
			res->str_ = SCAST_STRING(real->num_to_str())->str_ + "+" + SCAST_STRING(imag->num_to_str())->str_ + "i";
		else
			res->str_ = SCAST_STRING(real->num_to_str())->str_ + SCAST_STRING(imag->num_to_str())->str_ + "i";
	}
	return res;
}

Base *Complex::equal_(Base *opr2){
	Number *number2 = SCAST_NUMBER(opr2);
	if (number2->type_ != 3)
		return new Boolean(false);
	else{
		Complex *tmp = SCAST_COMPLEX(number2);
		Boolean *flag1 = SCAST_BOOLEAN(real_->equal_(tmp->real_));
		Boolean *flag2 = SCAST_BOOLEAN(imag_->equal_(tmp->imag_));
		if (flag1->flag_ and flag2->flag_)
			return new Boolean(true);
		else
			return new Boolean(false);
	}
}

void Complex::printt(){

	real_->printt();

	if (imag_->type_ == 1){ // Rational
		Rational *tmp2 = SCAST_RATIONAL(imag_);
		if (tmp2->numerator_.vnum[0] != 0){
			if (tmp2->numerator_.sign)
				printf("+");
			tmp2->printt();
			printf("i");
		}
	}
	else if (imag_->type_ == 2){ // Float
		Float *tmp2 = SCAST_FLOAT(imag_);
		if (tmp2->number_ != 0){
			if (tmp2->number_ > 0)
				printf("+");
			tmp2->printt();
			printf("i");
		}
	}
	else
		imag_->printt();
}

void Complex::print(){
	printt();
	printf("\n");
}

Complex *Complex::from_string(const char *expression){
	string a = "", b = "";
	int i, j, len = strlen(expression), type = 0; // a + 0i

	if(expression[0] == '#') // Boolean
   		return NULL;
	
	a += expression[0];
	for (i=1; i<len; ++i){
		if (expression[i] == '+' and expression[i-1] != 'e'){ // a + bi
			type = 1;
			break;
		}
		if (expression[i] == '-' and expression[i-1] != 'e'){ // a - bi
			type = 1;
			b += '-';
			break;
		}
		if (expression[i] == 'i'){ // bi
			type = 2;
			return NULL; // this case doesn't be defined in racket
		}
		a += expression[i];
	}

	for (j=i+1; j<len; ++j){
		if (expression[j] != 'i')
			b += expression[j];
	}
	/*for (i=len-2; i>=0; --i){
		if (expression[i] == '+'){ // a + bi
			type = 1;
			break;
		}
		if (expression[i] == '-'){ // a - bi
			type = 1;
			b = "-" + b;
			break;
		}
		b = expression[i] + b;
	}
	for (j=0; j<i; ++j)
		a += expression[j];*/

	if (b == "" or b == "-")
		b += '1';

	Number *real, *imag;
	//cout << a << " " << b << endl;
	string c = "0";
	switch(type){
		case 0: {
			real = Rational::from_string(a.c_str());
			if(!real)
				real = Float::from_string(a.c_str());
			if(real==NULL)
				throw 0;
			imag = Rational::from_string(c.c_str());
			if(!imag)
				imag = Float::from_string(c.c_str());
			if(imag==NULL)
				throw 0;
			break;
		}
		case 1: {
			real = Rational::from_string(a.c_str());
			if(!real)
				real = Float::from_string(a.c_str());
			if(real==NULL)
				throw 0;
			imag = Rational::from_string(b.c_str());
			if(!imag)
				imag = Float::from_string(b.c_str());
			if(imag==NULL)
				throw 0;
			break;
		}
		case 2: {
			real = Rational::from_string(c.c_str());
			if(!real)
				real = Float::from_string(c.c_str());
			if(real==NULL)
				throw 0;
			imag = Rational::from_string(a.c_str());
			if(!imag)
				imag = Float::from_string(a.c_str());
			if(imag==NULL)
				throw 0;
			break;
		}
		default:
			return NULL;
	}
	//real->print();
	//imag->print();
	return new Complex(real, imag);
}
