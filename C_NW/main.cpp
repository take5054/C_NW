#include <iostream>
#include <string>
#include "TCP.hpp"

#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; } }

int main()
{
	CTCP* pTCPConnection = new CTCP("", 8080);
	pTCPConnection->Bind();
	pTCPConnection->Send("Hello, World!");
	std::cout << pTCPConnection->Recv() << std::endl;
	SAFE_DELETE(pTCPConnection);

	return 0;
}
