#ifndef __ERROR_H__
#define __ERROR_H__

#include <string>
using namespace std;

class Error {

private:
	int err; // 

public:
	Error();
	int num();
	bool success();
	void outputError(int n);
};

#endif