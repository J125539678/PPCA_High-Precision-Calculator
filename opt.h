#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <bitset>
#include <cctype>
#include <cassert>
#include <complex>
#include "number.h"
#include "base.h"

struct Cons {
    Base *car; // this object
    Cons *cdr; // the next object
    Cons(Base *_car, Cons *_cdr) : car(_car), cdr(_cdr) {} // struction function
    bool check_length(int len) { // check_length
        int l = 0;
        for (Cons *p = this; p; p = p->cdr) l++;
        return l == len;
    }
};
 
class Opt {
    public:
        virtual Base *calc(Cons *con) = 0;
};
