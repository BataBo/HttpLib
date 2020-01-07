

#include <iostream>
#include "SupportHeader.h"
#pragma comment (lib, "HttpLib.lib")


int main()
{
	HttpRequest request;

	request.AddHeader("Hello", "World");
	request.AddHeader("Smily:face");
	//request.AddProxy("127.0.0.1:8888", http);
	request.KeepAlive = true;
	HttpResponse response = request.Start(HEAD, "https://www.google.com");
	printf("Status code %d\n", response.ReturnCode);
	printf("Response headers:\n");
	for (int i = 0;;i++) {
		if (response.headers[i] == "")break;
		std::cout << response.headers[i] << std::endl;
	}
	printf("Response cookies:\n");
	for (int i = 0;; i++) {
		if (response.cookies[i] == "")break;
		std::cout << response.cookies[i] << std::endl;
	}
	printf("\nBody:none since this is a head request");
	request.ClearAllHeaders();
	request.AddHeader("Some", "Header");
	HttpResponse response2 = request.Start(POST, "https://www.google.com", "parameter1=1&parameter2=2", "application/www-form-urlencoded");
	printf("\n\nStatus code %d\n", response2.ReturnCode);
	printf("Response headers:\n");
	for (int i = 0;; i++) {
		if (response2.headers[i] == "")break;
		std::cout << response2.headers[i] << std::endl;
	}
	printf("Response cookies:\n");
	for (int i = 0;; i++) {
		if (response2.cookies[i] == "")break;
		std::cout << response2.cookies[i] << std::endl;
	}
	printf("\nBody:\n");
	std::cout << response2.responseBody << std::endl;
}
