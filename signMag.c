#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#include "signMag.h"

SignMag_t signMag_init(bool isNegative, uint16_t magnitude)
{
	SignMag_t signMag;
	
	signMag.isNegative = isNegative;
	signMag.magnitude = magnitude;

	return signMag;

}

void signMag_print(SignMag_t value)
{
	if(value.isNegative) {
		printf("-%u\n", value.magnitude);
	} else {
		printf("%u\n", value.magnitude);
	}
}
SignMag_t signMag_read(void)
{
	SignMag_t signMag;
	char c;
	char magStr[7];
	size_t count = 0;
	bool flag = true;
	bool valid = false;
	signMag.isNegative = false;
	while(flag) {
		c = getchar();
		if (c == EOF || c == '\n') {
			flag = false;
		}
		if (isdigit(c)) {
			if (count < 6) {
				magStr[count] = c;
				count++;
				valid = true;
			}
		} else if (c == '-') {
			signMag.isNegative = true;
		} else if (c == '+') {
			signMag.isNegative = false;
		} else {
			flag = false;
		}
	}

	magStr[count] = '\0';
	
	if(!valid) {
		signMag.magnitude = 0;
		signMag.isNegative = true;
	}

	if (atoi(magStr) > 65535) {
		signMag.magnitude = 65535;
	} else {
		signMag.magnitude = atoi(magStr);
	}
	return signMag;
}
SignMag_t signMag_sum(SignMag_t sm1, SignMag_t sm2)
{
	SignMag_t result;
	result.isNegative = false;
	if (sm1.isNegative && !sm2.isNegative) {
		if (sm1.magnitude > sm2.magnitude) {
			result.isNegative = true;
			result.magnitude = sm1.magnitude - sm2.magnitude;
		} else {
		    result.magnitude = sm2.magnitude - sm1.magnitude;
		}
	} else if (!sm1.isNegative && sm2.isNegative) {
		if (sm1.magnitude < sm2.magnitude) {
			result.isNegative = true;
			result.magnitude = sm2.magnitude - sm1.magnitude;
		} else {
			result.magnitude = sm1.magnitude - sm2.magnitude;
		}
			    
	} else {
		if(sm1.isNegative && sm2.isNegative) {
			result.isNegative = true;
		}
		result.magnitude = sm1.magnitude + sm2.magnitude;
		if (sm1.magnitude + sm2.magnitude > result.magnitude) {
			result.magnitude = 65535; 
	    }
	}

	return result;
}
SignMag_t signMag_accumulate(const SignMag_t* array, size_t arraySize)
{
	SignMag_t result;
	for (size_t arrayIndex = 1; arrayIndex <= arraySize; arrayIndex++) {
		result = signMag_sum(array[arrayIndex-1], result);
	}
	return result;
}

bool signMag_max(SignMag_t* num1, SignMag_t* num2, SignMag_t** max)
{
	if (num1->magnitude == 65535) {
		*max = num1;
		return true;
	}
	if (num2->magnitude == 65535) {
		*max = num2;
		return true;
	}
	if (num1->isNegative && !num2->isNegative) {
		*max = num2;
		return true;
	} else if (!num1->isNegative && num2->isNegative) {
		*max = num1;
		return true;
	} else if (!num1->isNegative && !num2->isNegative) {
		if (num1->magnitude >= num2->magnitude) {
			*max = num1;
		} else {
			*max = num2;
		}
		return true;
	} else if (num1->isNegative && num2->isNegative) {
		if (num1->magnitude >= num2->magnitude) {
			*max = num2;
		} else {
			*max = num1;
		}
		return true;
	}
	return false;
}
int main(void) {
    // Define some SignMag_t values for testing
    SignMag_t num1 = signMag_init(true, 30000);  // -30000
    SignMag_t num2 = signMag_init(false, 20000); // 20000
    SignMag_t num3 = signMag_init(true, 60000);  // -60000
    SignMag_t num4 = signMag_init(false, 65535); // 65535
    
    // Pointers to hold the maximum values
    SignMag_t* max;

    // Test 1: Comparing -30000 and 20000
    if (signMag_max(&num1, &num2, &max)) {
        printf("Max of %d and %d is: ", num1.isNegative ? -num1.magnitude : num1.magnitude, num2.isNegative ? -num2.magnitude : num2.magnitude);
        signMag_print(*max);
    } else {
        printf("Error in finding max.\n");
    }

    // Test 2: Comparing -60000 and 65535
    if (signMag_max(&num3, &num4, &max)) {
        printf("Max of %d and %d is: ", num3.isNegative ? -num3.magnitude : num3.magnitude, num4.isNegative ? -num4.magnitude : num4.magnitude);
        signMag_print(*max);
    } else {
        printf("Error in finding max.\n");
    }

    // Test 3: Comparing two negative numbers
    SignMag_t num5 = signMag_init(true, 40000); // -40000
    SignMag_t num6 = signMag_init(true, 45000); // -45000

    if (signMag_max(&num5, &num6, &max)) {
        printf("Max of %d and %d is: ", num5.isNegative ? -num5.magnitude : num5.magnitude, num6.isNegative ? -num6.magnitude : num6.magnitude);
        signMag_print(*max);
    } else {
        printf("Error in finding max.\n");
    }

    return 0;
}
