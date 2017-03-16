#include "calc_exp.h"
#include <iostream>
using namespace std;

int main() {
    try {
		//for (Base *res;;) {
			Base *res = calc_exp();
			res -> print();
		//}
	}
	catch(int){cerr<<"Wrong";}
	//system("pause");
    return 0;
}
