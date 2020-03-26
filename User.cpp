#include "Menus.h"

using namespace std;

std::vector<Credentials> users;
User user;
Catalog catalog;

void userHaveAccount()
{
	int8_t row = 0;

	while (true)
	{
		system("cls");
		cout << "� ��� ���� �������?\n\n";
		cout << "  ��";
		if (!row) cout << "\t<--";
		cout << "\n\n  ���";
		if (row == 1) cout << "\t<--";
		cout << "\n\n  �����";
		if (row == 2) cout << "\t<--";
		cout << "\n\n";

		char a = ' ';
		a = _getch();
		if (a == 72) row = (row + 2) % 3;
		else if (a == 80) row = (row + 1) % 3;
		else if (a == 13)
		{
			if (row == 0) user.enterAccount();
			else if (row == 1) user.createAccount();
			else if (row == 2)
			{
				system("cls");
				break;
			}
			return;
		}
	}
}

void User::userMenu()
{
	int8_t row = 0;
	int8_t colNum = 4;
	while (true)
	{
		system("cls");

		cout << "\n �������� �������";
		if (!row) cout << "  <--";
		cout << "\n\n �������� ������";
		if (row == 1) cout << "  <--";
		cout << "\n\n ������� �������";
		if (row == 2) cout << "  <--";
		cout << "\n\n �����";
		if (row == 3) cout << "  <--";
		cout << "\n\n";

		char a = ' ';
		a = _getch();
		if (a == 72) row = (row + colNum - 1) % colNum;
		else if (a == 80) row = (row + 1) % colNum;
		else if (a == 13)
		{
			if (row == 0) catalog.changeCatalog();
			else if (row == 1) user.changePassword();
			else if (row == 2) { user.deleteAccount(); break; }
			else if (row == 3)
			{
				system("cls");
				break;
			}
		}
	}

}


void User::createAccount()
{
	system("cls");

	haveAccess = true;
	leave = false;

	do {
		if (haveAccess) enterLogin(0);
		else enterLogin(2);
		if (leave) break;
		enterPassword();
		if (leave) break;

		for (auto i : users)
		{
			if (i.login == this->credentials.login)
			{
				haveAccess = false;
				break;
			}
		}	

	} while (!haveAccess);

	if (!leave)
	{
		addCredentials();
		rewriteUserFile();
		userMenu();
	}
}

void User::enterAccount()
{
	system("cls");
	haveAccess = true;
	leave = false;

	do {
		if (haveAccess) enterLogin(0);
		else enterLogin(1);
		if (leave) break;
		enterPassword();
		if (leave) break;

		haveAccess = true;

		Credentials tmp(credentials.login, sha256(credentials.password));
		if ( find(users.begin(), users.end(), tmp) == users.end())
			haveAccess = false;

	} while (!haveAccess);

	if (!leave)
	{
		userMenu();
	}
}


void User::enterLogin(int mode)
{
	int8_t error;
	switch (mode)
	{
	case 0: credentials.login = (haveAccess ? getString("������� �����. ��� ������ ������� exit")
												: getString("������� ������ �����. ��� ������ ������� exit"));
		break;
	case 1: credentials.login =  getString("������������ ����� �/��� ������. ��������� ����. ��� ������ ������� exit"); break;
	case 2: credentials.login =  getString("������� � ����� ������� ��� ����������. ��������� ����. ��� ������ ������� exit"); break;
	}

	do
	{
		error = 0;

		if (credentials.login == "exit")
		{
			leave = true;
			break;
		}

		if (credentials.login.size() < 4)
		{
			error = 1;
			credentials.login = getString("������� ����� �� 4 ��� ����� ��������. ��� ������ ������� exit");
		}

		for (int i = 0; i < credentials.login.size(); ++i)
			if (!isalnum(credentials.login[i]))
			{
				if (i == 0 && credentials.login[0] == ' ')
				{
					error = 2;
					credentials.login = getString("����� �������� ������������ �������. ��� ������ ������� exit");
					break;
				}
			}

	} while (error);
}

void User::enterPassword()
{
	int8_t error;

	credentials.password = getPassword("������� ������. ��� ������ ������� exit");

	do{
		error = 0;

		if (credentials.password == "exit")
			leave = true;

		for (auto a : credentials.password)
			if (!(isalnum(a) || a == '_'))
			{
				error = 1;
				credentials.password = getPassword("������ �������� ������������ �������. ��� ������ ������� exit");
				break;
			}

		if (credentials.password.size() < 8)
		{
			error = 2;
			credentials.password = getPassword("������ ������ ��������� ������� 8 ��������. ��� ������ ������� exit");
		}

	} while (error);
}

string getString(string msg)
{
	system("cls");
	string tmp;
	cout << msg << endl;
	getline(cin, tmp);
	return tmp;
}

string getPassword(string msg)
{
	string password = "";
	char a = '1';

	system("cls");
	cout << msg << endl;

	while ((a = _getch()) != 13)
	{
		if ((int)a == 8)
		{
			if (password.empty()) continue;

			password.erase(password.size() - 1, password.size());

			cout << '\b' << ' ' << '\b';
		}
		else
		{
			password += a;
			cout << '*';
		}
	}
	return password;
}

void getCharacter(string msg)
{
	system("cls");
	cout << msg << endl;
	char a = _getch();
}


void rewriteUserFile()
{
	ofstream file("users.txt");
	remove("users.txt");
	for(auto i : users)
	{
		file << i.login << endl << i.password << endl;
	}
	file.close();
}

void copyUserFile()
{
	ifstream fUsers;
	fUsers.open("users.txt");
	while (fUsers)
	{
		Credentials temp;
		getline(fUsers, temp.login);
		getline(fUsers, temp.password);
		users.push_back(temp);
	}
	if (!users.empty()) users.erase(users.end()-1);
	fUsers.close();
}


bool Credentials::operator==(Credentials a)
{
	return (this->login == a.login && this->password == a.password) ? true : false;
}

void User::addCredentials()
{
	Credentials tmp(user.credentials.login, sha256(user.credentials.password));
	users.push_back(tmp);
}


Credentials::Credentials(string login, string password)
{
	this->login = login;
	this->password = password;
}

Credentials::Credentials(){}


int User::checkPasswords()
{
	string pass1 = getPassword("������ ������ �� ��������. ��� ������ ������� exit");
	if (pass1 == "exit") return 2;
	string pass2 = getPassword("������ ������ �� �������� ��� ���. ��� ������ ������� exit");
	if (pass2 == "exit") return 2;

	if (pass1 != pass2 || pass1 != this->credentials.password)
	{
		getCharacter("������ �� ���������. ��� ����������� ������� ����� �������");
		return 1;
	}
	return 0;
}

void Catalog::changeCatalog()
{
	cout << "merry christmas";
}

void User::deleteAccount()
{
	do {
		int i = checkPasswords();

		if (!i)
		{
			Credentials tmp(this->credentials.login, sha256(this->credentials.password));

			auto ptr = find(users.begin(), users.end(), tmp);
			if (ptr != users.end())
			{
				users.erase(ptr);
				rewriteUserFile();
				return;
			}
		}
		else if (i == 2) break;

	} while (true);
}

void User::changePassword()
{
	do {
		int i = checkPasswords();

		if (!i)
		{
			Credentials tmp(this->credentials.login, sha256(this->credentials.password));

			auto ptr = find(users.begin(), users.end(), tmp);
			if (ptr != users.end())
			{
				enterPassword();
				if (leave) return;

				ptr->password = sha256(credentials.password);
				rewriteUserFile();
				return;
			}
		}
		else if (i == 2) break;
	} while (true);
}
