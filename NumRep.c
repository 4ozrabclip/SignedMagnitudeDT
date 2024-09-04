#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
	bool isNegative;
	uint16_t magnitude;
} SignMag_t;

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
	if (sm1.isNegative && sm2.isNegative) {
		result = sm1.magnitude + sm2.magnitude;
		result.isNegative = true;
	} else if (sm1.isNegative && !sm2.isNegative) {
		if (sm1.magnitude > sm2.magnitude) {
			result.isNegative = true;
			result = sm1.magnitude + sm2.magnitude;
		} else {
			result = sm1.magnitude + sm2.magnitude;
		}
	} else if (!sm1.isNegative && sm2.isNegative) {
		if (sm1.magnitude < sm2.magnitude) {
			result.isNegative = true;
			result = sm1.magnitude + sm2.magnitude;
		} else {
			result = sm1.magnitude + sm2.magnitude;
		}
	}
	result = sm1.magnitude + sm2.magnitude; 
}
int main(void)
{
	SignMag_t val = signMag_read();
	printf("%s%hu ", val.isNegative ? "-" : "+", val.magnitude);
	val = signMag_read();
	printf("%s%hu ", val.isNegative ? "-" : "+", val.magnitude);
	val = signMag_read();
	printf("%s%hu ", val.isNegative ? "-" : "+", val.magnitude);
	return 0;
}
