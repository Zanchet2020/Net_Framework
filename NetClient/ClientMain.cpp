#include <iostream>

#include <networking.h>
#include <message_types.h>


int main() {
	networking::message<CustomMessages> message;
	message.header.id = CustomMessages::Ping;

	int a = 10;
	bool b = true;
	float c = 3.123235f;

	message << a << b << c;

	std::cout << message << std::endl;

	message >> c;

	std::cout << message << std::endl;


	system("pause");

	return 0;
}