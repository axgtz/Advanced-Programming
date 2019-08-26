//  A01019608
// Created by Roberto Alejandro Gutierrez Guillen on 2019-08-22.
//

#include <stdio.h>

#ifdef PI   // Constants are always in upper case
#def PI 3.14159
#

int main () {

	float radius = 4.5;
	float area = PI * square(radius);


	#if DEBUG // only compiled if aded flag
		printf('This line will only be printed when debugging\n');
	#endif

	return 0;
}
