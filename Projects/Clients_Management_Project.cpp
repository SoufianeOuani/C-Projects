#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

const string FileName = "Clients.txt";
void ShowMainMenu();

enum enMainMenuOption{
	ClientsList = 1, AddClient = 2,
	DeleteClient = 3, UpdateClient = 4,
	FindClient = 5, Exit = 6
};

struct StClientsInfo
{
	string AccountNumber = "",
		PinCode = "",
		ClientName = "",
		PhoneNumber = "";
	double AccountBalance = 0;
	bool MarkedForDelete = false;
};

string ReadString(string Message)
{
	string Text;
	cout << Message << endl;
	getline(cin >> ws, Text);
	return Text;
}

vector <string> SplitString(string Text, string Delim)
{
	short Position = 0;
	string Word = "";
	vector <string> vString;

	while ((Position = Text.find(Delim)) != std::string::npos)
	{
		Word = Text.substr(0, Position);
		if (Word != "")
		{
			vString.push_back(Word);
		}
		Text.erase(0, Position + Delim.length());
	}

	if (Text!="")
	{
		vString.push_back(Text);
	}

	return vString;

}

string ConvertRecordToLine(StClientsInfo Client, string Separator="#//#")
{
	string ClientDataLine;

	ClientDataLine += Client.AccountNumber + Separator;
	ClientDataLine += Client.PinCode + Separator;
	ClientDataLine += Client.ClientName + Separator;
	ClientDataLine += Client.PhoneNumber + Separator;
	ClientDataLine += to_string(Client.AccountBalance);

	return ClientDataLine;
}

StClientsInfo ConvertLineToRecord(string Line, string Separator="#//#")
{
	StClientsInfo Client;
	vector <string> vString = SplitString(Line, Separator);

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.ClientName = vString[2];
	Client.PhoneNumber = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector <StClientsInfo> vClients;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if(MyFile.is_open())
	{
		string Line;
		StClientsInfo Client;

		while(getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);

			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close(); 
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}

	return false;

}

StClientsInfo ReadNewClient()
{
	// Usage of std::ws will extract all the whitespace character

	StClientsInfo Client;
	
	cout << "\nEnter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);
	
	while (ClientExistsByAccountNumber(Client.AccountNumber, FileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number ? ";
		getline(cin >> ws, Client.AccountNumber);

	}

	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.ClientName);

	cout << "Enter Phone? ";
	getline(cin, Client.PhoneNumber);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}

vector <StClientsInfo> LoadClientsDataFromFile(string FileName)
{
	vector <StClientsInfo> vClientsData;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		StClientsInfo Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClientsData.push_back(Client);
		}

		MyFile.close();
	}

	return vClientsData;

}

void PrintClientRecordLine(StClientsInfo Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.ClientName;
	cout << "| " << setw(12) << left << Client.PhoneNumber;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAllClientsScreen()
{
	vector <StClientsInfo> vClients = LoadClientsDataFromFile(FileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0)
	{
		cout << "\t\t\tNo clients available in the system!";
	}
	else
	{
		for(StClientsInfo Client : vClients)
		{
			PrintClientRecordLine(Client);
			cout << endl;
		}
	}

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void PrintClientCard(StClientsInfo Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code : " << Client.PinCode;
	cout << "\nName : " << Client.ClientName;
	cout << "\nPhone : " << Client.PhoneNumber;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";
}

bool FinClientByAccountNumber(string AccountNumber, vector <StClientsInfo> vClients, StClientsInfo &Client)
{

	for (StClientsInfo ClientToFind : vClients)
	{
		if (ClientToFind.AccountNumber == AccountNumber)
		{
			Client = ClientToFind;
			return true;
		}
	}

	return false;
}

StClientsInfo ChangeClientRecord(string AccountNumber)
{
	StClientsInfo Client;

	Client.AccountNumber = AccountNumber;

	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.ClientName);

	cout << "Enter Phone? ";
	getline(cin, Client.PhoneNumber);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <StClientsInfo> &vClients)
{
	for(StClientsInfo &ClientToDelete : vClients)
	{
		if (ClientToDelete.AccountNumber == AccountNumber)
		{
			ClientToDelete.MarkedForDelete = true;
			return true;
		}
	}

	return false;
}

vector <StClientsInfo> SaveClientsDataToFile(string FileName, vector <StClientsInfo> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string ClientDataLine = "";

	if (MyFile.is_open())
	{
		for (StClientsInfo Client : vClients)
		{

			if (Client.MarkedForDelete == false)
			{
				ClientDataLine = ConvertRecordToLine(Client);
				MyFile << ClientDataLine << endl;
			}
		}

		MyFile.close();
	}

	return vClients;
}

void AddClientsToFile(string FileName, string ClientDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << ClientDataLine << endl;
	}

	MyFile.close();
}

void AddNewClient()
{
	StClientsInfo Client;
	Client = ReadNewClient();
	AddClientsToFile(FileName, ConvertRecordToLine(Client));
}

void AddClients()
{
	char AddMore = 'Y';

	do
	{
		AddNewClient();
		cout << "\nClient added succefully, do you want to add a new client (Y / N)? ";
		cin >> AddMore;

	} while (AddMore == 'Y' || AddMore == 'y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <StClientsInfo>& vClients)
{
	StClientsInfo Client;
	char Answer = 'n';

	if (FinClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		
		cout << "\nAre you sure you want to delete this client (Y / N)? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{

			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(FileName, vClients);

			//Refresh clients after delete.
			vClients = LoadClientsDataFromFile(FileName);

			cout << "\nClient succesfully deleted." << endl;
			return true;
		}
	}
	else
	{
		cout << "\nClient with account number (" << AccountNumber << ") not found!" << endl;
		return false;
	}

}

bool UpdatedClientByAccountNumber(string AccountNumber, vector <StClientsInfo> &vClients)
{
	StClientsInfo Client;
	char Answer = 'n';

	if (FinClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre you sure you want to update this client (Y / N)? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			for (StClientsInfo &ClientToUpdate : vClients)
			{
				if (ClientToUpdate.AccountNumber == AccountNumber)
				{
					ClientToUpdate = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(FileName, vClients);
			cout << "\nClient succesfully updated." << endl;
			return true;
		}
	}
	else
	{
		cout << "\nClient with account number (" << AccountNumber << ") not found!" << endl;
		return false;
	}

}

void ShowDeleteScreen()
{
	cout << "=====================================================\n";
	cout << "\t\tUpdate Client Screen\n";
	cout << "=====================================================\n";

	vector <StClientsInfo> vClients = LoadClientsDataFromFile(FileName);
	string AccountNumber = ReadString("Please enter an account number to update? ");
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateScreen()
{
	cout << "=====================================================\n";
	cout << "\t\tUpdate Client Screen\n";
	cout << "=====================================================\n";

	vector <StClientsInfo> vClients = LoadClientsDataFromFile(FileName);
	string AccountNumber = ReadString("Please enter an account number to delete? ");
	UpdatedClientByAccountNumber(AccountNumber, vClients);
}

void ShowAddClientsScreen()
{
	cout << "=====================================================\n";
	cout << "\t\tAdd New Client Screen\n";
	cout << "=====================================================\n";

	AddClients();
}

void ShowFindClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";

	vector <StClientsInfo> vClients = LoadClientsDataFromFile(FileName);
	StClientsInfo Client;
	string AccountNumber = ReadString("Please enter an account number to looking for: ");
	
	if(FinClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "Client with account number (" << AccountNumber << ") not found!" << endl;
	}
}

void ShowEndScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n-----------------------------------\n";
}

void GoBackToMainMenu()
{
	cout << "\nPress any key to go back to a main menu...";
	system("pause>0");
	ShowMainMenu();
}

short ReadMainMenuOptions()
{
	short Option = 0;
	cout << "Choose what do you want to do [1 to 6]: ";
	cin >> Option;
	return Option;
}

void PerformMainMenuOption(enMainMenuOption MainMenuOption)
{
	switch (MainMenuOption)
	{
	case enMainMenuOption::ClientsList:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOption::AddClient:
		system("cls");
		ShowAddClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOption::DeleteClient:
		system("cls");
		ShowDeleteScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOption::UpdateClient:
		system("cls");
		ShowUpdateScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOption::FindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOption::Exit:
		system("cls");
		ShowEndScreen();
		break;
	}
}
void ShowMainMenu()
{
	system("cls");
	cout << "=====================================================\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "=====================================================\n";
		
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Exit.\n";
	cout << "===========================================\n";

	PerformMainMenuOption((enMainMenuOption)ReadMainMenuOptions());

}

int main()
{

	ShowMainMenu();

	return 0;
}
