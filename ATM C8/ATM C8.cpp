// Project 2 ATM C8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using  namespace std;
struct sClient
{
	string AccountNumber = "";
	string PinCode = "";
	string PhoneNumber = "";
	string Name = "";
	double Palence = 0;
	bool Delete = false;

};
string ClientsFileName = "Clients.txt";
string ClientsComma = "#//#";
short EncryptionNumber = 900;
short DecryptionNumber = 900;
int IndexClient = 0;
vector <sClient> VClients;
vector <string> SplitString(string s1, string comma)
{
	vector <string> vString;
	short pos = s1.find(comma);
	while (s1.find(comma) != std::string::npos)
	{
		vString.push_back(s1.substr(0, pos));
		s1.erase(0, pos + comma.length());
		pos = s1.find(comma);

	}
	if (s1 != "")
		vString.push_back(s1);
	return vString;
}
sClient StringToClient(string s, string comma)
{
	vector <string> vString;
	sClient client;
	vString = SplitString(s, comma);
	if (vString.size() >= 5)
	{
		client.AccountNumber = vString[0];
		client.PinCode = vString[1];
		client.Name = vString[2];
		client.PhoneNumber = vString[3];
		client.Palence = stoi(vString[4]);
	}
	return client;
}

void PrintLine()
{
	cout << "========================================\n";

}
void PrintNameOfScreen(string NameOfScreen, bool cls = true)
{
	if (cls)
		system("cls");
	PrintLine();
	cout << "\t    " << NameOfScreen << endl;
	PrintLine();
}
void EncryptionString(string& s1, short encryptionNumber)
{
	for (short i = 0; i < s1.length(); i++)
	{
		s1[i] = char(s1[i] + encryptionNumber);
	}
}
void DecryptionString(string& s1, short decryptionNumber)
{
	for (short i = 0; i < s1.length(); i++)
	{
		s1[i] = char(s1[i] - decryptionNumber);
	}
}
void LoodFileToString(vector <string>& vString, string fileName, string comma)
{
	string s = "";
	vString.clear();
	fstream File;
	File.open(fileName, ios::in);
	if (File.is_open())
	{
		while (getline(File, s))
		{
			DecryptionString(s, ::DecryptionNumber);
			vString.push_back(s);
		}

	}
}
void LoodFileToVClients(vector <sClient>& vClients, string fileName, string comma)
{
	vClients.clear();
	vector <string> vString;
	LoodFileToString(vString, fileName, comma);
	sClient TempClient;
	for (string s : vString)
	{
		TempClient = StringToClient(s, comma);
		if (TempClient.AccountNumber != "")
			vClients.push_back(TempClient);
	}
	if (vClients.size() == 0)
	{
		cout << "You must copy Clients.txt from Project-1-Bank-Extension-2 to Project 2 ATM C8\n";
		system("pause");
	}

}
void SaveStringInFile(vector <string>& vString, string fileName, string comma)
{
	string s = "";

	fstream File;
	File.open(fileName, ios::out);
	if (File.is_open())
	{
		for (int i = 0; i < vString.size(); i++)
		{
			EncryptionString(vString[i], ::EncryptionNumber);
			File << vString[i] << endl;;
		}

	}
}
string ConverClientToString(sClient client, string comma)
{
	return client.AccountNumber + comma + client.PinCode + comma + client.Name + comma + client.PhoneNumber + comma + to_string(client.Palence);
}
void SaveVClientsInFile(vector <sClient>& vClients, string fileName, string comma)
{
	vector <string> vString;
	for (sClient& c : vClients)
	{
		vString.push_back(ConverClientToString(c, comma));
	}
	SaveStringInFile(vString, fileName, comma);
}
enum enEmptyIndex
{
	enEmpty = -1
};
int CheckLoginAndReturnIndex(vector <sClient>& vClients, string& AccountNumber, string& PinCode)
{
	for (int i = 0; i < vClients.size(); i++)
	{
		if (vClients[i].AccountNumber == AccountNumber)
		{
			if (vClients[i].PinCode == PinCode)
			{
				return i;
			}
			else
			{
				return enEmptyIndex::enEmpty;
			}
		}
	}
	return enEmptyIndex::enEmpty;
}

void Login(vector <sClient>& vClients)
{
	string AccountNumber, PinCode;
	int Index = 0;
	while (true)
	{
		PrintNameOfScreen("Login Screen");
		cout << "Enter AccountNumber : ";
		getline(cin >> ws, AccountNumber);
		cout << "Enter Pin Code : ";
		getline(cin >> ws, PinCode);
		Index = CheckLoginAndReturnIndex(vClients, AccountNumber, PinCode);
		if (Index == enEmptyIndex::enEmpty)
		{
			cout << "Account Number Or PinCode Error\n";
			system("pause");
		}
		else
			break;
	}
	::IndexClient = Index;
}
short ReadShortNumber(string messege, short from, short to)
{
	short input = 0;
	cout << messege;
	cin >> input;
	while (input<from || input>to || cin.fail())
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
		cout << "Enter number from " << from << " to " << to << " : ";
		cin >> input;
	}
	return input;
}
int ReadIntNumber(string messege, int from, int to)
{
	int input = 0;
	cout << messege;
	cin >> input;
	while (input<from || input>to || cin.fail())
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
		cout << "Enter number from " << from << " to " << to << " : ";
		cin >> input;
	}
	return input;
}
void PrintBalance(bool printNameOfScreen = true)
{
	if (printNameOfScreen)
		PrintNameOfScreen("Check Balance Screen");
	printf("Your Balance is : %.*f\n", 2, VClients[::IndexClient].Palence);
	PrintLine();
}
enum enQuickWithdraw
{
	en20 = 1, en50, en100, en200, en400, en600, en800, en1000, enLogout
};
short QuickWithdrawScreenAndReadChoose()
{
	PrintNameOfScreen("Quick Withdraw");
	cout << "\t" << setw(15) << left << "[1] 20" << "[2] 50\n";
	cout << "\t" << setw(15) << left << "[3] 100" << "[4] 200\n";
	cout << "\t" << setw(15) << left << "[5] 400" << "[6] 600\n";
	cout << "\t" << setw(15) << left << "[7] 800" << "[8] 1000\n";
	cout << "\t" << "[9] Exit\n";
	PrintLine();
	PrintBalance(false);
	return ReadShortNumber("Choos what to withdraw from 1 to 8 : ", 1, 9);
}
bool CheckAmountAvailable(int withdrawalAmount)
{
	return (withdrawalAmount <= ::VClients[::IndexClient].Palence);
}
bool AreYouSure(string messege)
{
	string check = "";
	cout << messege << " Y/N ? ";
	getline(cin >> ws, check);
	if (check == "y" || check == "Y" || check == "yes" || check == "Yes" || check == "YES")
		return true;
	return false;
}
bool WithdrawBalance(int withdrawalAmount, bool ItMustBeAMultipleOf5 = true)
{
	bool checkMultipleOf5;
	if (ItMustBeAMultipleOf5)
	{
		checkMultipleOf5 = withdrawalAmount % 5 == 0;
	}

	else
	{
		checkMultipleOf5 = true;
	}

	if (checkMultipleOf5)
	{
		if (CheckAmountAvailable(withdrawalAmount))
		{
			::VClients[::IndexClient].Palence -= withdrawalAmount;
			if (AreYouSure("Are you want to withdraw " + to_string(withdrawalAmount)))
			{
				SaveVClientsInFile(::VClients, ::ClientsFileName, ::ClientsComma);
				PrintLine();
				cout << "Correct withdrawal\n";
				PrintBalance(false);
				return true;
			}

			return false;
		}
		cout << "There is not enough balance.\n";
		return false;
	}
	cout << "The withdrawal number is not a multiple of 5.\n";
	return false;

}
void QuickWithdraw()
{
	switch (QuickWithdrawScreenAndReadChoose())
	{
	case enQuickWithdraw::en20:
		WithdrawBalance(20);
		break;
	case enQuickWithdraw::en50:
		WithdrawBalance(50);
		break;
	case enQuickWithdraw::en100:
		WithdrawBalance(100);
		break;
	case enQuickWithdraw::en200:
		WithdrawBalance(200);
		break;
	case enQuickWithdraw::en400:
		WithdrawBalance(400);
		break;
	case enQuickWithdraw::en600:
		WithdrawBalance(600);
		break;
	case enQuickWithdraw::en800:
		WithdrawBalance(800);
		break;
	case enQuickWithdraw::en1000:
		WithdrawBalance(1000);
		break;
	case enQuickWithdraw::enLogout:
		return;
	default:
		cout << "Not Exist\n";
		break;
	}
}
void NormalWithdraw()
{
	int input = 0;
	PrintNameOfScreen("Normal Withdraw Screen");
	PrintBalance();
	do
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
		if (input > 100000000)
		{
			cout << "Enter number < 100000000 \n";
		}
		cout << "Enter an amount multiple of 5's : ";
		cin >> input;
	} while (input % 5 != 0 || cin.fail() || input > 100000000);
	WithdrawBalance(input);
}
void Deposit()
{
	int input = 0;
	PrintNameOfScreen("Deposit Screen");
	PrintBalance();
	input = ReadIntNumber("Enter amount from 0 to 1000000 : ", 0, 1000000);
	if (AreYouSure("Are You Sure Add " + to_string(input)))
	{
		::VClients[::IndexClient].Palence += input;
		SaveVClientsInFile(::VClients, ::ClientsFileName, ::ClientsComma);
	}

	PrintBalance(false);
}
enum enMainMunue
{
	enQuickWithdraw = 1,
	enNormalWithdraw = 2,
	enDeposit = 3,
	enCheckBalance = 4,
	eLogout = 5
};
enMainMunue PrintMenueAndReadingTheChoose()
{
	PrintNameOfScreen("ATM Main Menue");
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	PrintLine();
	return (enMainMunue)ReadShortNumber("Choose a number from 1 to 5 : ", 1, 5);
}
void MainMenue()
{
	while (true)
	{

		switch (PrintMenueAndReadingTheChoose())
		{
		case enMainMunue::enQuickWithdraw:
			QuickWithdraw();
			break;
		case enMainMunue::enNormalWithdraw:
			NormalWithdraw();
			break;
		case enMainMunue::enDeposit:
			Deposit();
			break;
		case enMainMunue::enCheckBalance:
			PrintBalance();
			break;
		case enMainMunue::eLogout:
			system("pause");
			return;
		default:
			cout << "Not Exist\n";
			break;
		}
		cout << "Press any kay to go back to Main Menue...";
		system("pause");
	}
}
void ATM()
{
	system("color 70");
	while (true)
	{
		LoodFileToVClients(::VClients, ::ClientsFileName, ::ClientsComma);

		Login(::VClients);
		MainMenue();
		::IndexClient = enEmptyIndex::enEmpty;
		SaveVClientsInFile(::VClients, ::ClientsFileName, ::ClientsComma);

	}
}
int main()
{
	ATM();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
