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
	} else if (!num1->isNegative && num2->isNegative) {
		*max = num1;
	} else if (!num1->isNegative && !num2->isNegative) {
		if (num1->magnitude >= num2->magnitude) {
			*max = num1;
		} else {
			*max = num2;
		}
	} else if (num1->isNegative && num2->isNegative) {
		if (num1->magnitude >= num2->magnitude) {
			*max = num2;
		} else {
			*max = num1;
		}
	}
	return false;
}

SignMag_t* arrayMax(SignMag_t* array, size_t size, bool (*func)(SignMag_t*, SignMag_t*, SignMag_t**))
{
	SignMag_t* maxValueInArray = &array[0];
	SignMag_t* valuePtr;
	SignMag_t* currentMax;
	for(size_t arrayIndex = 0; arrayIndex < size; arrayIndex++) {
		valuePtr = &array[arrayIndex];
		currentMax = maxValueInArray;
		if(func(valuePtr, currentMax, &maxValueInArray)) {
			return currentMax;
		}
	}
	return maxValueInArray;
}
int main(void)
{
	size_t count = 0;
	do{
		SignMag_t test = signMag_read();
		signMag_print(test);
	}while (count < 100);

	    SignMag_t values[100];
    size_t count = 0;

    while (count < 100) {
        printf("Enter a sign magnitude value (or 'quit' to stop): ");
        SignMag_t input = signMag_read();
        if (input.magnitude == 0 && input.isNegative) {
            char term[5];
            if (scanf("%4s", term) == 1 && strcmp(term, "quit") == 0) {
                break;
                while (getchar() != '\n');
                continue;
            }
        }
        values[count++] = input;
    }

    printf("Entered values:\n");
    for (size_t i = 0; i < count; i++) {
        signMag_print(values[i]);
    }
    print_separator();

    SignMag_t accumulated_sum = signMag_accumulate(values, count);
    printf("Accumulated sum:\n");
    signMag_print(accumulated_sum);
    print_separator();

    SignMag_t squared_sum = signMag_multiply(accumulated_sum, accumulated_sum);
    printf("Square of accumulated sum:\n");
    signMag_print(squared_sum);
    print_separator();

    SignMag_t* max_value = arrayMax(values, count, signMag_max);
    printf("Maximum value:\n");
    signMag_print(*max_value);
}