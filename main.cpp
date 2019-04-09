#include <string>
#include "Calc.h"

using namespace std;

int main()
{
	string input;
	Calc calc;

	while (true)
	{
		cout << "계산식을 입력하세요. (종료: quit) ☞ ";
		getline(cin, input);

		if (input.compare("quit") == 0) break;

		calc.run(input);
	}

	return 0;
}
