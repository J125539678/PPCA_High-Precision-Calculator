# include "boolean.h"
# include <cstdio>
# include <cstring>

Boolean::Boolean(bool flag){
	flag_ = flag;
	unitType = BOOLEAN;
}

Boolean::~Boolean(){

}

Base *Boolean::not_(){
	return new Boolean(!flag_);
}

Base *Boolean::equal_(Base *opr2){
	Boolean *tmp = SCAST_BOOLEAN(opr2);
	if (flag_ == tmp->flag_)
		return new Boolean(true);
	else
		return new Boolean(false);
}

void Boolean::print(){
	if (flag_)
		printf("#t\n");
	else
		printf("#f\n");
}

Boolean *Boolean::from_string(const char *expression){
	if (expression[0] == '#' and strlen(expression) == 2){
		if (expression[1] == 't')
			return new Boolean(true);
		else if (expression[1] == 'f')
			return new Boolean(false);
		else
			return NULL;
	}
	else
		return NULL;
}
