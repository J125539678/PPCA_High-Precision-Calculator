#pragma once
#include "opt.h"
#include "float.h"
#include "rational.h"
#include "boolean.h"
#include "char.h"
#include "String.h"
#include "number.h"
#include "base.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <complex>
#include <cstdlib>
#define SCAST_NUMBER(x) static_cast<Number*>(x)
#define SCAST_RATIONAL(x) static_cast<Rational*>(x)
#define SCAST_FLOAT(x) static_cast<Float*>(x)
#define SCAST_COMPLEX(x) static_cast<Complex*>(x)
#define SCAST_BOOLEAN(x) static_cast<Boolean*>(x)
#define SCAST_CHARACTER(x) static_cast<Char*>(x)
#define SCAST_STRING(x) static_cast<String*>(x)
using namespace std;

class Add : public Opt {
    /* Use the lowest level type */
    Base *calc(Cons *con){
		Number *res = new Rational("0","1"); // res: a new instance
		Number *last; 
        for (; con; con = con->cdr){ // for all objects in con list
        	if(con->car->unitType!=2) throw 0;
        	Number *opr=SCAST_NUMBER(con->car),*conv;
        
			if(opr->type_>3||opr->type_<1) // check if the type is legal 
			{
				throw 0;
			}
            last = res; 
            if (res->type_ > opr->type_) // compare the type
			{ 
				res = res->add(conv = res->convert(opr));
			}
            else
			{
                res = (conv = opr->convert(res))->add(opr);
			}
			
            delete last;
            delete conv;
        }
        return res;
    }
};

class Sub:public Opt{
	Base *calc(Cons *con){
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if(tmp->car->unitType!=2)
			{
				throw 0;
			}
			cnt++;
		}
		Number *res = new Rational("0", "1"),*last;
		Number *opr = SCAST_NUMBER(con->car), *conv;
		last=res;
		if(cnt==1)
		{
			if (res->type_ > opr->type_)
				res = res->sub(conv = res->convert(opr));
			else
				res = (conv = opr->convert(res))->sub(opr);
			delete last;
			delete conv;
			return res;
		}
		if (res->type_ > opr->type_)
            res = res->add(conv = res->convert(opr));
        else
            res = (conv = opr->convert(res))->add(opr);
        con=con->cdr;
        delete last;
        delete conv;
		for(;con;con=con->cdr)
		{
			opr = SCAST_NUMBER(con->car);
			last=res;
			if(res->type_>opr->type_)
				res=res->sub(conv=res->convert(opr));
			else
				res=(conv=opr->convert(res))->sub(opr);
			delete last;
			delete conv;
		}
		return res;
	}
};

class Mul : public Opt {
    /* Use the lowest level type */
    Base *calc(Cons *con){
        Number *res = new Rational("1", "1"), *last;
        for (; con; con = con->cdr)
        {
        	if(con->car->unitType!=2) throw 0;
        	Number *opr=SCAST_NUMBER(con->car),*conv;
			if(opr->type_>3||opr->type_<1)
			{
				throw 0;
			}
            last = res;
            if (res->type_ > opr->type_)
                res = res->mul(conv = res->convert(opr));
            else
                res = (conv = opr->convert(res))->mul(opr);
            delete last;
            delete conv;
        }
        return res;
    }
};

class Div:public Opt{
	Base *calc(Cons *con){
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if(tmp->car->unitType!=2)
			{
				throw 0;
			}
			cnt++;
		}
		Number *res=new Rational("1","1"),*last;
		Number *opr=SCAST_NUMBER(con->car),*conv;
		last=res;
		Number *zero = new Float(0.0);
		if(cnt==1)
		{
			if(res->type_>opr->type_)
				res=res->div(conv=res->convert(opr));
			else
				res=(conv=opr->convert(res))->div(opr);
			delete last;
			delete conv;
			return res;
		}
		if(res->type_>opr->type_)
			res=res->mul(conv=res->convert(opr));
		else
			res=(conv=opr->convert(res))->mul(opr);
		con=con->cdr;
		delete last;
		delete conv;
		for(;con;con=con->cdr)
		{
			opr=SCAST_NUMBER(con->car);
			last=res;
			if(res->type_>opr->type_)
				res=res->div(conv=res->convert(opr));
			else
				res=(conv=opr->convert(res))->div(opr);
			delete last;
			delete conv;
		}
		return res;
	}
};

class Abs:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType == 2){
				Number *res = SCAST_NUMBER(con->car)->abs();
				delete con->car;
				return res;
			}
			else
				throw 0;
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Quotient:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(2)){
			if(con->car->unitType!=2 or con->cdr->car->unitType!=2) throw 0;
			Number *opr1=SCAST_NUMBER(con->car), *opr2=SCAST_NUMBER(con->cdr->car);
			if(opr1->type_ == 1 and opr2->type_ == 1){
				Rational *number1 = SCAST_RATIONAL(opr1), *number2 = SCAST_RATIONAL(opr2);
				Number *res = number1->quotient(number2);
				delete number1;
				delete number2;
				return res;
			}
			else
				assert(0 && "accepted integers only");
		}
		else
			assert(0 && "accepted 2 but given more");
	}
};

class Remainder:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(2)){
			if(con->car->unitType!=2 or con->cdr->car->unitType!=2) throw 0;
			Number *opr1=SCAST_NUMBER(con->car), *opr2=SCAST_NUMBER(con->cdr->car);
			if(opr1->type_ == 1 and opr2->type_ == 1){
				Rational *number1 = SCAST_RATIONAL(opr1), *number2 = SCAST_RATIONAL(opr2);
				Number *res = number1->remainder(number2);
				delete number1;
				delete number2;
				return res;
			}
			else
				assert(0 && "accepted integers only");
		}
		else
			assert(0 && "accepted 2 but given more");
	}
};

class Modulo:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(2)){
			if(con->car->unitType!=2 or con->cdr->car->unitType!=2) throw 0;
			Number *opr1=SCAST_NUMBER(con->car), *opr2=SCAST_NUMBER(con->cdr->car);
			if(opr1->type_ == 1 and opr2->type_ == 1){
				Rational *number1 = SCAST_RATIONAL(opr1), *number2 = SCAST_RATIONAL(opr2);
				Number *res = number1->modulo(number2);
				delete number1;
				delete number2;
				return res;
			}
			else
				assert(0 && "accepted integers only");
		}
		else
			assert(0 && "accepted 2 but given more");
	}
};

class Gcd:public Opt{
	Base *calc(Cons *con){	
		Number *res = new Rational("0","1");
		Number *last;
        for (; con; con = con->cdr){
        	if(con->car->unitType!=2) throw 0;
        	Number *opr=SCAST_NUMBER(con->car),*conv;
			if(opr->type_>3||opr->type_<1) 
				throw 0;
            last = res;
            if (opr->type_ == 1)
				res = res->gcd(conv = res->convert(opr));
			else
				assert(0 && "accepted integers only");
            delete last;
            delete conv;	
        }
        return res;
    }
};

class Lcm:public Opt{
	Base *calc(Cons *con){	
		Number *res = SCAST_NUMBER(con->car);
		con = con->cdr;
		Number *last;
        for (; con; con = con->cdr){
        	if(con->car->unitType!=2) throw 0;
        	Number *opr = SCAST_NUMBER(con->car), *conv;
			if(opr->type_>3||opr->type_<1) 
				throw 0;
            last = res;
            if (opr->type_ == 1)
				res = res->lcm(conv = res->convert(opr));
			else
				assert(0 && "accepted integers only");
            delete last;
            delete conv;	
        }
        return res->abs();
    }
};

class Max:public Opt{
	Base *calc(Cons *con){	
		Number *res = SCAST_NUMBER(con->car);
		con = con->cdr;
		Number *last;
        for (; con; con = con->cdr){
        	if(con->car->unitType!=2) throw 0;
        	Number *opr = SCAST_NUMBER(con->car), *conv;
			if(opr->type_>3||opr->type_<1) 
				throw 0;
            last = res;
            if (opr->type_ == 1 or opr->type_ == 2){
            	if (res->type_ > opr->type_) // compare the type
					res = res->max_(conv = res->convert(opr));
            	else
                	res = (conv = opr->convert(res))->max_(opr);
            }
			else
				assert(0 && "accepted real numbers only");
            delete last;
            delete conv;	
        }
        return res;
    }
};

class Min:public Opt{
	Base *calc(Cons *con){	
		Number *res = SCAST_NUMBER(con->car);
		con = con->cdr;
		Number *last;
        for (; con; con = con->cdr){
        	if(con->car->unitType!=2) throw 0;
        	Number *opr = SCAST_NUMBER(con->car), *conv;
			if(opr->type_>3||opr->type_<1) 
				throw 0;
            last = res;
            if (opr->type_ == 1 or opr->type_ == 2){
            	if (res->type_ > opr->type_) // compare the type
					res = res->min_(conv = res->convert(opr));
            	else
                	res = (conv = opr->convert(res))->min_(opr);
            }
			else
				assert(0 && "accepted real numbers only");
            delete last;
            delete conv;	
        }
        return res;
    }
};

class Expt:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(2)){
			if(con->car->unitType!=2 or con->cdr->car->unitType!=2) throw 0;
			Number *opr1=SCAST_NUMBER(con->car), *opr2=SCAST_NUMBER(con->cdr->car);
			if(opr1->type_ >= 1 and opr2->type_ <= 3){
				if(opr1->type_ > opr2->type_){
					Number *res = opr1->expt(opr1->convert(opr2));
					delete opr1, opr2;
					return res;
				}
				else{
					Number *res = (opr2->convert(opr1))->expt(opr2);
					delete opr1, opr2;
					return res;
				}
			}
			else
				throw 0;
		}
		else
			assert(0 && "accepted 2 but given more");
	}
};

class Sqrt:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			if(opr->type_ < 1 or opr->type_ >3){
				throw 0;
			}
			else{
				Number *res = opr->sqrt_();
				delete opr;
				return res;
			}
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Numerator:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			if(opr->type_ == 1 or opr->type_ == 2){
				Number *res = opr->numerator();
				delete opr;
				return res;
			}
			else
				throw 0;
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Denominator:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			if(opr->type_ == 1){
				Number *res = opr->denominator();
				delete opr;
				return res;
			}
			else
				throw 0;
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Floor:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			if(opr->type_ < 1 or opr->type_ >2){
				throw 0;
			}
			else{
				Number *res = opr->floor_();
				delete opr;
				return res;
			}
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Ceiling:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			if(opr->type_ < 1 or opr->type_ >2){
				throw 0;
			}
			else{
				Number *res = opr->ceiling_();
				delete opr;
				return res;
			}
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Truncate:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			if(opr->type_ < 1 or opr->type_ >2){
				throw 0;
			}
			else{
				Number *res = opr->truncate_();
				delete opr;
				return res;
			}
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Round:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			if(opr->type_ < 1 or opr->type_ >2){
				throw 0;
			}
			else{
				Number *res = opr->round_();
				delete opr;
				return res;
			}
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Inexact_exact:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			if(opr->type_ < 1 or opr->type_ >3){
				throw 0;
			}
			else{
				Number *res = opr->inexact_exact();
				delete opr;
				return res;
			}
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Exact_inexact:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			if(opr->type_ < 1 or opr->type_ >3){
				throw 0;
			}
			else{
				Number *res = opr->exact_inexact();
				delete opr;
				return res;
			}
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Real_part:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			if(opr->type_ < 1 or opr->type_ >3){
				throw 0;
			}
			else{
				Number *res = opr->real_part();
				delete opr;
				return res;
			}
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Imag_part:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			if(opr->type_ < 1 or opr->type_ >3){
				throw 0;
			}
			else{
				Number *res = opr->imag_part();
				delete opr;
				return res;
			}
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Is_number:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType == 2)
				return new Boolean(true);
			else
				return new Boolean(false);
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Is_char:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType == 3)
				return new Boolean(true);
			else
				return new Boolean(false);
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Is_string:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType == 4)
				return new Boolean(true);
			else
				return new Boolean(false);
		}
		else
			assert(0 && "accepted 1 but given more");
	}
};

class Equal:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(2)){
			Base *opr1 = con->car, *opr2 = con->cdr->car;
			if (opr1->unitType != opr2->unitType){
				delete opr1, opr2;
				return new Boolean(false);
			}
			else{
				Base *res = opr1->equal_(opr2);
				delete opr1, opr2;
				return res;
			}
		}
		else
			assert(0 && "expected 2 arguments!");
	}
};

class Less:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
		Boolean *res = new Boolean(true);
		Number *tmp = SCAST_NUMBER(con->car);
		con = con->cdr;
		Number *last;
        for (; con; con = con->cdr){
        	if(con->car->unitType!=2) throw 0;
        	Number *opr = SCAST_NUMBER(con->car), *conv;
            last = tmp;
            if (opr->type_ == 1 or opr->type_ == 2){
            	if (tmp->type_ > opr->type_) // compare the type
					res = SCAST_BOOLEAN(tmp->less(conv = tmp->convert(opr)));
            	else
                	res = SCAST_BOOLEAN((conv = opr->convert(tmp))->less(opr));
                if (res->flag_ == false)
        			return res;
            }
			else
				assert(0 && "accepted real numbers only");
			tmp = opr;
            delete last;
            delete conv;	
        }
        return res;
    }
};

class Less_equal:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
		Boolean *res = new Boolean(true);
		Number *tmp = SCAST_NUMBER(con->car);
		con = con->cdr;
		Number *last;
        for (; con; con = con->cdr){
        	if(con->car->unitType!=2) throw 0;
        	Number *opr = SCAST_NUMBER(con->car), *conv;
            last = tmp;
            if (opr->type_ == 1 or opr->type_ == 2){
            	if (tmp->type_ > opr->type_) // compare the type
					res = SCAST_BOOLEAN(tmp->less_equal(conv = tmp->convert(opr)));
            	else
                	res = SCAST_BOOLEAN((conv = opr->convert(tmp))->less_equal(opr));
                if (res->flag_ == false)
        			return res;
            }
			else
				assert(0 && "accepted real numbers only");
			tmp = opr;
            delete last;
            delete conv;	
        }
        return res;
    }
};

class More:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
		Boolean *res = new Boolean(true);
		Number *tmp = SCAST_NUMBER(con->car);
		con = con->cdr;
		Number *last;
        for (; con; con = con->cdr){
        	if(con->car->unitType!=2) throw 0;
        	Number *opr = SCAST_NUMBER(con->car), *conv;
            last = tmp;
            if (opr->type_ == 1 or opr->type_ == 2){
            	if (tmp->type_ > opr->type_) // compare the type
					res = SCAST_BOOLEAN(tmp->more(conv = tmp->convert(opr)));
            	else
                	res = SCAST_BOOLEAN((conv = opr->convert(tmp))->more(opr));
                if (res->flag_ == false)
        			return res;
            }
			else
				assert(0 && "accepted real numbers only");
			tmp = opr;
            delete last;
            delete conv;	
        }
        return res;
    }
};

class More_equal:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
		Boolean *res = new Boolean(true);
		Number *tmp = SCAST_NUMBER(con->car);
		con = con->cdr;
		Number *last;
        for (; con; con = con->cdr){
        	if(con->car->unitType!=2) throw 0;
        	Number *opr = SCAST_NUMBER(con->car), *conv;
            last = tmp;
            if (opr->type_ == 1 or opr->type_ == 2){
            	if (tmp->type_ > opr->type_) // compare the type
					res = SCAST_BOOLEAN(tmp->more_equal(conv = tmp->convert(opr)));
            	else
                	res = SCAST_BOOLEAN((conv = opr->convert(tmp))->more_equal(opr));
                if (res->flag_ == false)
        			return res;
            }
			else
				assert(0 && "accepted real numbers only");
			tmp = opr;
            delete last;
            delete conv;	
        }
        return res;
    }
};

class Not:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if (con->car->unitType == 1){
				Boolean *tmp = SCAST_BOOLEAN(con->car);
				Base *res = tmp->not_();
				delete tmp;
				return res;
			}
			else
				return new Boolean(false);
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Is_exact:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType != 2)
				return new Boolean(false);
			Number *opr = SCAST_NUMBER(con->car);
			Base *res = opr->is_exact();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Is_inexact:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType != 2)
				return new Boolean(false);
			Number *opr = SCAST_NUMBER(con->car);
			Base *res = opr->is_inexact();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Is_integer:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2)
				return new Boolean(false);
			Number *opr=SCAST_NUMBER(con->car);
			Base *res=opr->is_integer();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Is_rational:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2) 
				return new Boolean(false);
			Number *opr=SCAST_NUMBER(con->car);
			Base *res=opr->is_rational();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Is_real:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2)
				return new Boolean(false);
			Number *opr=SCAST_NUMBER(con->car);
			Base *res=opr->is_real();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Is_complex:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2)
				return new Boolean(false);
			Number *opr=SCAST_NUMBER(con->car);
			Base *res=opr->is_complex();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Is_zero:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2)
				return new Boolean(false);
			Number *opr=SCAST_NUMBER(con->car);
			Base *res=opr->is_zero();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Is_negative:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2)
				return new Boolean(false);
			Number *opr=SCAST_NUMBER(con->car);
			Base *res=opr->is_negative();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Is_positive:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2)
				return new Boolean(false);
			Number *opr=SCAST_NUMBER(con->car);
			Base *res=opr->is_positive();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Is_odd:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2)
				return new Boolean(false);
			Number *opr=SCAST_NUMBER(con->car);
			Base *res=opr->is_odd();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Is_even:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2)
				return new Boolean(false);
			Number *opr=SCAST_NUMBER(con->car);
			Base *res=opr->is_even();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Sin:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			Number *res=opr->sin_();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Cos:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			Number *res=opr->cos_();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Tan:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			Number *res=opr->tan_();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Asin:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			Number *res=opr->asin_();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Acos:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			Number *res=opr->acos_();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Atan:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			Number *res=opr->atan_();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Exp:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			Number *res=opr->exp_();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Log:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			Number *res=opr->log_();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Make_Rec:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(2)){
			if(con->car->unitType!=2 or con->cdr->car->unitType!=2) throw 0;
			Number *opr1=SCAST_NUMBER(con->car), *opr2=SCAST_NUMBER(con->cdr->car);
			Number *res,*conv;
			if(opr1->type_ > opr2->type_){
				res=opr1->make_rec(conv=opr1->convert(opr2));
			}
			else {
				res=(conv=opr2->convert(opr1))->make_rec(opr2);
			}
			delete conv;
			return res;
		}
		else
			assert(0 && "accepted 2 but given more");
	}
};

class Make_pol:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(2)){
			if(con->car->unitType!=2 or con->cdr->car->unitType!=2) throw 0;
			Number *opr1=SCAST_NUMBER(con->car), *opr2=SCAST_NUMBER(con->cdr->car);
			Number *res,*conv;
			if(opr1->type_ > opr2->type_){
				res=opr1->make_pol(conv=opr1->convert(opr2));
			}
			else {
				res=(conv=opr2->convert(opr1))->make_pol(opr2);
			}
			delete conv;
			return res;
		}
		else
			assert(0 && "accepted 2 but given more");
	}
};

class Magnitude:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			Number *res=opr->magnitude_();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

class Angle:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if(con->car->unitType!=2) throw 0;
			Number *opr=SCAST_NUMBER(con->car);
			Number *res=opr->angle_();
			return res;
		}
		else assert(0 && "accepted 1 but given more");
	}
};

// String
class Str_equal:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 4 or con->cdr->car->unitType != 4) throw 0;
        	String *opr1 = SCAST_STRING(con->car), *opr2 = SCAST_STRING(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->str_equal(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Str_ci_equal:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 4 or con->cdr->car->unitType != 4) throw 0;
        	String *opr1 = SCAST_STRING(con->car), *opr2 = SCAST_STRING(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->str_ci_equal(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Str_less:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 4 or con->cdr->car->unitType != 4) throw 0;
        	String *opr1 = SCAST_STRING(con->car), *opr2 = SCAST_STRING(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->str_less(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Str_ci_less:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 4 or con->cdr->car->unitType != 4) throw 0;
        	String *opr1 = SCAST_STRING(con->car), *opr2 = SCAST_STRING(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->str_ci_less(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Str_less_equal:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 4 or con->cdr->car->unitType != 4) throw 0;
        	String *opr1 = SCAST_STRING(con->car), *opr2 = SCAST_STRING(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->str_less_equal(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Str_ci_less_equal:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 4 or con->cdr->car->unitType != 4) throw 0;
        	String *opr1 = SCAST_STRING(con->car), *opr2 = SCAST_STRING(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->str_ci_less_equal(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Make_str:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){ // make-string k
			if (con->car->unitType != 2)
				assert(0 && "expected integer");
			String *res = new String();
			res = SCAST_STRING(res->make_str_1(con->car));
			delete con->car;
			return res;
		}
		else if(con->check_length(2)){ // make-string k char
			if (con->car->unitType != 2 or con->cdr->car->unitType != 3)
				assert(0 && "illegal input");
			String *res = new String();
			res = SCAST_STRING(res->make_str_2(con->car, con->cdr->car));
			delete con->cdr->car;
			delete con->car;
			return res;
		}
		else assert(0 && "expected 1 or 2 arguments");
	}
};

class Str:public Opt{
	Base *calc(Cons *con){
		String *res = new String();
		String *last;
        for (; con; con = con->cdr){
        	if(con->car->unitType != 3)
        		assert(0 && "expected characters");
            last = res; 
            res = SCAST_STRING(res->string_(con->car));
            delete last;
        }
        return res;
	}
};

class Str_len:public Opt{
	Base *calc(Cons *con){
		Base *res = new Rational("0", "1");
		if (con->check_length(1)){
			if (con->car->unitType != 4)
				assert(0 && "expected string");
			res = SCAST_STRING(con->car)->str_len();
			delete con->car;
			return res;
		}
		else
			assert(0 && "expected 1 argument");
	}
};

class Str_ref:public Opt{
	Base *calc(Cons *con){
		Base *res = new String();
		if (con->check_length(2)){
			Base *opr1 = con->car, *opr2 = con->cdr->car;
			res = SCAST_STRING(opr1)->str_ref(opr2);
			delete opr1, opr2;
			return res;
		}
		else
			assert(0 && "expected 2 arguments");
	}
};

class Sub_str:public Opt{
	Base *calc(Cons *con){
		Base *res = new String();
		if (con->check_length(2)){ // substring string begin
			Base *opr1 = con->car, *opr2 = con->cdr->car;
			res = SCAST_STRING(opr1)->sub_str_1(opr2);
			delete opr1, opr2;
			return res;
		}
		else if(con->check_length(3)){ // substring string begin end
			Base *opr1 = con->car, *opr2 = con->cdr->car, *opr3 = con->cdr->cdr->car;
			res = SCAST_STRING(opr1)->sub_str_2(opr2, opr3);
			delete opr1, opr2, opr3;
			return res;
		}
		else
			assert(0 && "expected 2 or 3 arguments");
	}
};

class Str_append:public Opt{
	Base *calc(Cons *con){
		String *res = new String();
		String *last;
        for (; con; con = con->cdr){
        	if(con->car->unitType != 4)
        		assert(0 && "expected strings");
            last = res; 
            res = SCAST_STRING(res->str_append(con->car));
            delete last;
        }
        return res;
	}
};

class Str_copy:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1))
			return con->car;
		else
			assert(0 && "expected 1 argument");
	}
};

class Str_to_num:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if (con->car->unitType != 4)
				assert(0 && "expected string");
			String *opr = SCAST_STRING(con->car);
			Base *res = opr->str_to_num();
			delete opr;
			return res;
		}
		else 
			assert(0 && "expected 1 argument");
	}
};

class Num_to_str:public Opt{
	Base *calc(Cons *con){
		if(con->check_length(1)){
			if (con->car->unitType != 2)
				assert(0 && "expected number");
			Number *opr = SCAST_NUMBER(con->car);
			Base *res = opr->num_to_str();
			delete opr;
			return res;
		}
		else 
			assert(0 && "expected 1 argument");
	}
};

// Char
class Char_equal:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 3 or con->cdr->car->unitType != 3) throw 0;
        	Char *opr1 = SCAST_CHARACTER(con->car), *opr2 = SCAST_CHARACTER(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->char_equal(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Char_ci_equal:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 3 or con->cdr->car->unitType != 3) throw 0;
        	Char *opr1 = SCAST_CHARACTER(con->car), *opr2 = SCAST_CHARACTER(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->char_ci_equal(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Char_less:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 3 or con->cdr->car->unitType != 3) throw 0;
        	Char *opr1 = SCAST_CHARACTER(con->car), *opr2 = SCAST_CHARACTER(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->char_less(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Char_ci_less:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 3 or con->cdr->car->unitType != 3) throw 0;
        	Char *opr1 = SCAST_CHARACTER(con->car), *opr2 = SCAST_CHARACTER(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->char_ci_less(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Char_less_equal:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 3 or con->cdr->car->unitType != 3) throw 0;
        	Char *opr1 = SCAST_CHARACTER(con->car), *opr2 = SCAST_CHARACTER(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->char_less_equal(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Char_ci_less_equal:public Opt{
	Base *calc(Cons *con){
		bool flag = true;
        for (; con->cdr; con = con->cdr){
        	if(con->car->unitType != 3 or con->cdr->car->unitType != 3) throw 0;
        	Char *opr1 = SCAST_CHARACTER(con->car), *opr2 = SCAST_CHARACTER(con->cdr->car);
        	Boolean *temp_res = SCAST_BOOLEAN(opr1->char_ci_less_equal(opr2));
        	if (!temp_res->flag_){
        		delete temp_res, opr1;
        		return new Boolean(false);
			}
			delete opr1;
        }
        return new Boolean(true);
    }
};

class Char_alp:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType != 3)
				assert(0 && "expected character");
			Base *res = SCAST_CHARACTER(con->car)->char_alp();
			delete con->car;
			return res;
		}
		else
			assert(0 && "expected 1 argument");
	}
};

class Char_num:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType != 3)
				assert(0 && "expected character");
			Base *res = SCAST_CHARACTER(con->car)->char_num();
			delete con->car;
			return res;
		}
		else
			assert(0 && "expected 1 argument");
	}
};

class Char_space:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType != 3)
				assert(0 && "expected character");
			Base *res = SCAST_CHARACTER(con->car)->char_space();
			delete con->car;
			return res;
		}
		else
			assert(0 && "expected 1 argument");
	}
};

class Char_upper:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType != 3)
				assert(0 && "expected character");
			Base *res = SCAST_CHARACTER(con->car)->char_upper();
			delete con->car;
			return res;
		}
		else
			assert(0 && "expected 1 argument");
	}
};

class Char_lower:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType != 3)
				assert(0 && "expected character");
			Base *res = SCAST_CHARACTER(con->car)->char_lower();
			delete con->car;
			return res;
		}
		else
			assert(0 && "expected 1 argument");
	}
};

class Char_to_int:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType != 3)
				assert(0 && "expected character");
			Base *res = SCAST_CHARACTER(con->car)->char_to_int();
			delete con->car;
			return res;
		}
		else
			assert(0 && "expected 1 argument");
	}
};

class Int_to_char:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType != 2)
				assert(0 && "expected integer");
			Number *number = SCAST_NUMBER(con->car);
			if (number->type_ != 1)
				assert(0 && "expected integer");
			Base *res = SCAST_RATIONAL(number)->int_to_char();
			delete con->car;
			return res;
		}
		else
			assert(0 && "expected 1 argument");
	}
};

class Char_up:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType != 3)
				assert(0 && "expected character");
			Base *res = SCAST_CHARACTER(con->car)->char_up();
			delete con->car;
			return res;
		}
		else
			assert(0 && "expected 1 argument");
	}
};

class Char_down:public Opt{
	Base *calc(Cons *con){
		if (con->check_length(1)){
			if (con->car->unitType != 3)
				assert(0 && "expected character");
			Base *res = SCAST_CHARACTER(con->car)->char_down();
			delete con->car;
			return res;
		}
		else
			assert(0 && "expected 1 argument");
	}
};
