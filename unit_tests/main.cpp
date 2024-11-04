#include "Test.h"
#include "Keencloo.h"

int main(int argc, char** argv)
{
	Test example;

	//MediaPlayer w;

	TEST_CASE_ADD(example, "just check 1", [&example]() {
		
		int a;
		a = 2 + 2;
		example.assertEquals(6, a);

		int c;
		c = 2 + 2;
		example.assertEquals(9, c);

		double b;
		b = 5.0;
		example.assertEquals(5.0, b);

		return example.getCaseResult();
	});

	TEST_CASE_ADD(example, "just check 2", [&example]() {

		float a;
		a = 4.0f;
		example.assertEquals(4.0f, a);

		long long b;
		b = 256;
		example.assertEquals(256ll, b);

		return example.getCaseResult();
	});

	TEST_CASE_ADD(example, "just check 3", [&example]() {

		float a;
		a = 4.0f;
		example.assertEquals(4.0f, a);

		long long b;
		b = 256;
		example.assertEquals(256ll, b);

		return example.getCaseResult();
	});

	TEST_CASE_ADD(example, "just check 4", [&example]() {

		float a;
		a = 4.0f;
		example.assertEquals(4.0f, a);

		long long b;
		b = 256;
		example.assertEquals(256ll, b);

		return example.getCaseResult();
	});

	TEST_CASE_ADD(example, "just check 5", [&example]() {

		float a;
		a = 4.0f;
		example.assertEquals(4.0f, a);

		long long b;
		b = 256;
		example.assertEquals(257ll, b);

		return example.getCaseResult();
	});

	example.run();

	return 0;
}
