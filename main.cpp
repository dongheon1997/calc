#include <string>
#include "Calc.h"

using namespace std;

int main()
{
	string input;
	Calc calc;

	while (true)
	{
		cout << "������ �Է��ϼ���. (����: quit) �� ";
		getline(cin, input);

		if (input.compare("quit") == 0) break;

		calc.run(input);
	}

	return 0;
}
