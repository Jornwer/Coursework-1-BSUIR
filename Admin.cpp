#include "Menus.h"

using namespace std;

void copyAdminFile(vector<Credentials>& admins)
{
	ifstream file("admins.txt");
	while (file)
	{
		Credentials temp;
		getline(file, temp.login);
		getline(file, temp.password);
		admins.push_back(temp);
	}
	if (!admins.empty()) admins.erase(admins.end() - 1);
	file.close();
}

void rewriteAdminFile(vector<Credentials>& admins)
{
	ofstream file("admins.txt", ios::trunc);

	for (auto i : admins)
		file << i.login << endl << i.password << endl;

	file.close();
}