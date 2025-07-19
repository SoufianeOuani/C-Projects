#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";


enum enMainMenuOptions { ClientsList = 1, eAddNewClients = 2, DeleteClients = 3, UpdateClients = 4, FindClients = 5, Transactions = 6, ManageUsers = 7, Logout = 8 };
enum enTransactionsMenuChoices { DepositMenu = 1, Withdraw = 2, TotalBalances = 3, MainMenu = 4 };
enum enUserMenuOptions { ShowUsersList = 1, AddNewUser = 2, DeleteUser = 3, UpdateUser = 4, FindUser = 5, MainMenue = 6 };
enum enMainMenuePermissions{ eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64 };

void ShowMainMenuScreen();
void ShowTransactionsMenuScreen();
void ShowUsersMenuScreen();
bool CheckAccessPermissions(enMainMenuePermissions Permission);
void Login();

struct StClientsInfo
{
	string AccountNumber = "",
		PinCode = "",
		ClientName = "",
		PhoneNumber = "";
	double AccountBalance = 0;
	bool MarkedForDelete = false;
};

struct StUserInfo
{
	string Username = "";
	string Password = "";
	short Permission;
	bool MarkedForDelete = false;
};
StUserInfo CurrentUser;

string ReadString(string Message)
{
	string Text = "";
	cout << Message << endl;
	getline(cin >> ws, Text);
	return Text;
}

vector <string> SplitString(string Text, string Delim)
{
	string Word = "";
	short Position = 0;
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

	if (Text != "")
	{
		vString.push_back(Text);
	}

	return vString;

}

string ConvertRecordToLine(StClientsInfo Client, string Separator = "#//#")
{
	string ClientDataLine;

	ClientDataLine += Client.AccountNumber + Separator;
	ClientDataLine += Client.PinCode + Separator;
	ClientDataLine += Client.ClientName + Separator;
	ClientDataLine += Client.PhoneNumber + Separator;
	ClientDataLine += to_string(Client.AccountBalance);

	return ClientDataLine;
}

StClientsInfo ConvertLineToRecord(string Line, string Separator = "#//#")
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

string ConvertUserRecordToLine(StUserInfo User, string Separator = "#//#")
{
	string ClientDataLine;

	ClientDataLine += User.Username + Separator;
	ClientDataLine += User.Password + Separator;
	ClientDataLine += to_string(User.Permission);

	return ClientDataLine;
}

StUserInfo ConvertUserLineToRecord(string Line, string Separator = "#//#")
{
	StUserInfo User;
	vector <string> vString = SplitString(Line, Separator);

	User.Username = vString[0];
	User.Password = vString[1];
	User.Permission = stoi(vString[2]);

	return User;
}

bool ClientExistsByAccountNumber(string AccountNumber, string ClientsClientsFileName)
{
	vector <StClientsInfo> vClients;

	fstream MyFile;
	MyFile.open(ClientsClientsFileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		StClientsInfo Client;

		while (getline(MyFile, Line))
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

bool UserExistsByUserName(string UserName, string UsersFileName)
{
	vector <StUserInfo> vUsers;

	fstream MyFile;
	MyFile.open(UsersFileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		StUserInfo User;

		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);

			if (User.Username == UserName)
			{
				MyFile.close();
				return true;
			}
			vUsers.push_back(User);
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

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exist, Enter another Account Number ? ";
		getline(cin >> ws, Client.AccountNumber);

	}

	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.ClientName);

	cout << "Enter Phone? ";
	getline(cin, Client.PhoneNumber);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

short ReadPermissionsToSet()
{
	short Permissions = 0;
	char Answer = 'n';

	cout << "\nDo you want to give him/her full access (Y / N)? ";
	cin >> Answer;

	if (Answer == 'Y' || Answer == 'y')
	{
		return -1;
	}

	cout << "\nDo you want to give access to : \n ";

	cout << "\nShow clients list (Y / N)? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permissions += enMainMenuePermissions::pListClients;
	}

	cout << "\nAdd new client (Y / N)? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permissions += enMainMenuePermissions::pAddNewClient;
	}

	cout << "\nDelete client (Y / N)? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permissions += enMainMenuePermissions::pDeleteClient;
	}

	cout << "\nUpdate client (Y / N)? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permissions += enMainMenuePermissions::pUpdateClients;
	}

	cout << "\nFind client (Y / N)? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permissions += enMainMenuePermissions::pFindClient;
	}

	cout << "\nTransactions (Y / N)? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permissions += enMainMenuePermissions::pTranactions;
	}

	cout << "\nManage users (Y / N)? ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permissions += enMainMenuePermissions::pManageUsers;
	}

	return Permissions;
}

StUserInfo ReadNewUser()
{
	StUserInfo User;

	cout << "\nEnter User Name? ";
	// Usage of std::ws will extract all the whitespace character
	getline(cin >> ws, User.Username);

	while (UserExistsByUserName(User.Username, UsersFileName))
	{
		cout << "\nUser with username [" << User.Username << "] already exist, Enter another User Name ? ";
		getline(cin >> ws, User.Username);

	}

	cout << "Enter Password? ";
	getline(cin, User.Password);

	User.Permission = ReadPermissionsToSet();

	return User;
}

vector <StUserInfo> LoadUsersDataFromFile(string UsersFileName)
{
	fstream MyFile;
	vector <StUserInfo> vUsersData;
	MyFile.open(UsersFileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		StUserInfo User;

		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			vUsersData.push_back(User);
		}

		MyFile.close();
	}

	return vUsersData;

}

vector <StClientsInfo> LoadClientsDataFromFile(string ClientsClientsFileName)
{
	fstream MyFile;
	vector <StClientsInfo> vClientsData;
	MyFile.open(ClientsClientsFileName, ios::in);

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

void PrintUserRecordLine(StUserInfo User)
{
	cout << "| " << setw(15) << left << User.Username;
	cout << "| " << setw(10) << left << User.Password;
	cout << "| " << setw(40) << left << User.Permission;
}

void PrintClientRecordLine(StClientsInfo Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.ClientName;
	cout << "| " << setw(12) << left << Client.PhoneNumber;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintClientRecordBalanceLine(StClientsInfo Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.ClientName;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAccessDeniedMessage()
{
	cout << "\n-----------------------------------\n";
	cout << "Access Denied,\n";
	cout << "You don't have permission to do this,\n";
	cout << "Please Contact your Admin\n";
	cout << "\n-----------------------------------\n";
}

void ShowAllClientRecordsScreen()
{
	if (!CheckAccessPermissions(enMainMenuePermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}


	vector <StClientsInfo> vClients = LoadClientsDataFromFile(ClientsFileName);

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
		for (StClientsInfo Client : vClients)
		{
			PrintClientRecordLine(Client);
			cout << endl;
		}
	}

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void ShowAllUsersRecordsScreen()
{
	vector <StUserInfo> vUsers = LoadUsersDataFromFile(UsersFileName);

	cout << "\n\t\t\t\t\tClient List (" << vUsers.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(10) << "Password";
	cout << "| " << left << setw(40) << "Permissions";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vUsers.size() == 0)
	{
		cout << "\t\t\tNo Users available in the system!";
	}
	else
	{
		for (StUserInfo User : vUsers)
		{
			PrintUserRecordLine(User);
			cout << endl;
		}
	}

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

void ShowTotalBalancesScreen()
{

	vector <StClientsInfo> vClients = LoadClientsDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	double TotalBalances = 0;

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
		for (StClientsInfo Client : vClients)
		{
			PrintClientRecordBalanceLine(Client);
			TotalBalances += Client.AccountBalance;
			cout << endl;
		}

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;
}


void PrintClientCard(StClientsInfo Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccount Number: " << Client.AccountNumber;
	cout << "\nPin Code : " << Client.PinCode;
	cout << "\nClient Name : " << Client.ClientName;
	cout << "\nPhone : " << Client.PhoneNumber;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";
}

void PrintUserCard(StUserInfo User)
{
	cout << "\nThe following are the user details:\n";
	cout << "-----------------------------------";
	cout << "\nUser Name: " << User.Username;
	cout << "\nPassword : " << User.Password;
	cout << "\nPermissions : " << User.Permission;
	cout << "\n-----------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector <StClientsInfo> vClients, StClientsInfo& Client)
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

bool FindUserByUserNameAndPassword(string UserName, string Password, StUserInfo& User)
{
	vector <StUserInfo> vUsers = LoadUsersDataFromFile(UsersFileName);

	for (StUserInfo UserToFind : vUsers)
	{
		if (UserToFind.Username == UserName && UserToFind.Password == Password)
		{
			User = UserToFind;
			return true;
		}
	}
	return false;
}

bool FindUserByUserName(string UserName, vector <StUserInfo> vUsers, StUserInfo& User)
{
	for (StUserInfo UserToFind : vUsers)
	{
		if (UserToFind.Username == UserName)
		{
			User = UserToFind;
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

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;

}

StUserInfo ChangeUserRecord(string UserName)
{
	StUserInfo User;

	User.Username = UserName;

	cout << "\n\nEnter Password? ";
	getline(cin >> ws, User.Password);

	User.Permission = ReadPermissionsToSet();

	return User;

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <StClientsInfo>& vClients)
{
	for (StClientsInfo& ClientToDelete : vClients)
	{
		if (ClientToDelete.AccountNumber == AccountNumber)
		{
			ClientToDelete.MarkedForDelete = true;
			return true;
		}
	}

	return false;
}

bool MarkUserForDeleteByUserName(string UserName, vector <StUserInfo>& vUsers)
{
	for (StUserInfo& UserToDelete : vUsers)
	{
		if (UserToDelete.Username == UserName)
		{
			UserToDelete.MarkedForDelete = true;
			return true;
		}
	}

	return false;
}

vector <StClientsInfo> SaveClientsDataToFile(string ClientsFileName, vector <StClientsInfo> vClients)
{
	fstream MyFile;
	MyFile.open(ClientsFileName, ios::out);
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

vector <StUserInfo> SaveUsersDataToFile(string UsersFileName, vector <StUserInfo> vUsers)
{
	fstream MyFile;
	MyFile.open(UsersFileName, ios::out);
	string UserDataLine = "";

	if (MyFile.is_open())
	{
		for (StUserInfo User : vUsers)
		{

			if (User.MarkedForDelete == false)
			{
				UserDataLine = ConvertUserRecordToLine(User);
				MyFile << UserDataLine << endl;
			}
		}

		MyFile.close();
	}

	return vUsers;
}

void AddClientsToFile(string ClientsFileName, string ClientDataLine)
{
	fstream MyFile;
	MyFile.open(ClientsFileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << ClientDataLine << endl;
		MyFile.close();
	}
}

void AddUsersToFile(string UsersFileName, string UserDataLine)
{
	fstream MyFile;
	MyFile.open(UsersFileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << UserDataLine << endl;
	}

	MyFile.close();
}

void AddNewClient()
{
	StClientsInfo Client;
	Client = ReadNewClient();
	AddClientsToFile(ClientsFileName, ConvertRecordToLine(Client));
}


void AddNUser()
{
	StUserInfo User;
	User = ReadNewUser();
	AddUsersToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewClients()
{
	char AddMore = 'Y';

	do
	{
		AddNewClient();
		cout << "\nClient added succefully, do you want to add a new client (Y / N)? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

void AddUsers()
{
	char AddMore = 'Y';

	do
	{
		AddNUser();
		cout << "\nUser added succefully, do you want to add a new user (Y / N)? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <StClientsInfo>& vClients)
{
	StClientsInfo Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre you sure you want to delete this client (Y / N)? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{

			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);

			//Refresh clients after delete.
			vClients = LoadClientsDataFromFile(ClientsFileName);

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

bool DeleteUserByUserName(string UserName, vector <StUserInfo>& vUsers)
{
	if (UserName == "Admin")
	{
		cout << "\nYou cannon delete this user." << endl;
		return true;
	}

	StUserInfo User;
	char Answer = 'n';

	if (FindUserByUserName(UserName, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\nAre you sure you want to delete this user (Y / N)? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{

			MarkUserForDeleteByUserName(UserName, vUsers);
			SaveUsersDataToFile(UsersFileName, vUsers);

			//Refresh users after delete.
			vUsers = LoadUsersDataFromFile(UsersFileName);

			cout << "\nUser succesfully deleted." << endl;
			return true;
		}
	}
	else
	{
		cout << "\nUser with User Name (" << UserName << ") not found!" << endl;
		return false;
	}

}


bool UpdatedClientByAccountNumber(string AccountNumber, vector <StClientsInfo>& vClients)
{
	StClientsInfo Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\nAre you sure you want to update this client information (Y / N)? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			for (StClientsInfo& ClientToUpdate : vClients)
			{
				if (ClientToUpdate.AccountNumber == AccountNumber)
				{
					ClientToUpdate = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(ClientsFileName, vClients);
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

bool UpdatedUserByUserName(string UserName, vector <StUserInfo>& vUsers)
{
	StUserInfo User;
	char Answer = 'n';

	if (FindUserByUserName(UserName, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\nAre you sure you want to update this user information (Y / N)? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			for (StUserInfo& UserToUpdate : vUsers)
			{
				if (UserToUpdate.Username == UserName)
				{
					UserToUpdate = ChangeUserRecord(UserName);
					break;
				}
			}
			SaveUsersDataToFile(UsersFileName, vUsers);
			cout << "\nUser succesfully updated." << endl;
			return true;
		}
	}
	else
	{
		cout << "\nClient with account number (" << UserName << ") not found!" << endl;
		return false;
	}

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <StClientsInfo>& vClients)
{
	char Answer = 'n';

	cout << "\nAre you sure you want to perform this transaction (Y / N)? ";
	cin >> Answer;

	if (Answer == 'Y' || Answer == 'y')
	{
		for (StClientsInfo& ClientToDeposit : vClients)
		{
			if (ClientToDeposit.AccountNumber == AccountNumber)
			{
				ClientToDeposit.AccountBalance += Amount;
				cout << "Done succefully, the new balance : {" << ClientToDeposit.AccountBalance << "}.\n";
				SaveClientsDataToFile(ClientsFileName, vClients);
				return true;
			}
		}
	}

	return false;
}

void ShowListUsersScreen()
{
	ShowAllUsersRecordsScreen();
}

void ShowDeleteClientScreen()
{

	if (!CheckAccessPermissions(enMainMenuePermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "=====================================================\n";
	cout << "\t\tDelete Client Screen\n";
	cout << "=====================================================\n";

	vector <StClientsInfo> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadString("Please enter an account number to delete? ");
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
	if (!CheckAccessPermissions(enMainMenuePermissions::pUpdateClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "=====================================================\n";
	cout << "\t\tUpdate Client Screen\n";
	cout << "=====================================================\n";

	vector <StClientsInfo> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadString("Please enter an account number to update? ");
	UpdatedClientByAccountNumber(AccountNumber, vClients);
}

void ShowAddClientsScreen()
{
	if (!CheckAccessPermissions(enMainMenuePermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "=====================================================\n";
	cout << "\t\tAdd New Client Screen\n";
	cout << "=====================================================\n";

	AddNewClients();
}

void ShowFindClientScreen()
{
	if (!CheckAccessPermissions(enMainMenuePermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "=====================================================\n";
	cout << "Find Client Screen \n";
	cout << "=====================================================\n";

	vector <StClientsInfo> vClients = LoadClientsDataFromFile(ClientsFileName);
	StClientsInfo Client;
	string AccountNumber = ReadString("Please enter an account number to search for: ");

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
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
	cout << "\n\nPress any key to go back to the main menu...";
	system("pause>0");
	ShowMainMenuScreen();
}

void GoBackToUsersMenu()
{
	cout << "\n\nPress any key to go back to the users menu...";
	system("pause>0");
	ShowUsersMenuScreen();
}

void GoBackToTransactionsMenu()
{
	cout << "\n\nPress any key to go back to the transactions menu...";
	system("pause>0");
	ShowTransactionsMenuScreen();
}

short ReadMainMenuOptions()
{
	short Option = 0;
	cout << "Choose what do you want to do [1 to 8]: ";
	cin >> Option;
	return Option;
}

short ReadTransactionsMenuOptions()
{
	short Option = 0;
	cout << "Choose what do you want to do [1 to 4]: ";
	cin >> Option;
	return Option;
}

short ReadUsersMenuOptions()
{
	short Option = 0;
	cout << "Choose what do you want to do [1 to 6]: ";
	cin >> Option;
	return Option;
}

void ShowDepositScreen()
{

	cout << "=====================================================\n";
	cout << "\t\tDeposit Screen\n";
	cout << "=====================================================\n";

	StClientsInfo Client;
	vector <StClientsInfo> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadString("\nPlease enter an account number: ");

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with account number {" << AccountNumber << "} not found!\n";
		cin >> AccountNumber;
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter a deposit amount: ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);


}

void ShowWithdrawScreen()
{

	cout << "=====================================================\n";
	cout << "\t\tWithdraw Screen\n";
	cout << "=====================================================\n";

	StClientsInfo Client;
	vector <StClientsInfo> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadString("\nPlease enter an account number: ");

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with account number {" << AccountNumber << "} not found!\n";
		cin >> AccountNumber;
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter a withdraw amount: ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "\nyou exceed the balance, you can withdaw up to {" << Client.AccountBalance << "}." << endl;
		cout << "\nEnter a new withdraw amount: ";
		cin >> Amount;
	}

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}


void PerformMainMenuOptions(enMainMenuOptions MainMenuOptions)
{
	
	switch (MainMenuOptions)
	{
	case enMainMenuOptions::ClientsList:
		system("cls");
		ShowAllClientRecordsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::eAddNewClients:
		system("cls");
		ShowAddClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::DeleteClients:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::UpdateClients:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::FindClients:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::Transactions:
		system("cls");
		ShowTransactionsMenuScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::ManageUsers:
		system("cls");
		ShowUsersMenuScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::Logout:
		system("cls");
		Login();
		break;
	}
}

void PerformTransactionsMenuOptions(enTransactionsMenuChoices TransactionsOptions)
{
	switch (TransactionsOptions)
	{
	case enTransactionsMenuChoices::DepositMenu:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransactionsMenuChoices::Withdraw:
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransactionsMenuChoices::TotalBalances:
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransactionsMenuChoices::MainMenu:
		system("cls");
		ShowMainMenuScreen();
	}
}


void ShowTransactionsMenuScreen()
{
	if (!CheckAccessPermissions(enMainMenuePermissions::pTranactions))
	{
		ShowAccessDeniedMessage();
		return;
	}

	system("cls");
	cout << "=====================================================\n";
	cout << "\t\tTransactions Menu Screen\n";
	cout << "=====================================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balance.\n";
	cout << "\t[4] Main Menu.\n";
	cout << "=====================================================\n";

	PerformTransactionsMenuOptions((enTransactionsMenuChoices)ReadTransactionsMenuOptions());
}

void ShowMainMenuScreen()
{
	StUserInfo User;

	system("cls");
	cout << "=====================================================\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "=====================================================\n";

	cout << "\t[1] Show Clients List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Information.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "===========================================\n";

	PerformMainMenuOptions((enMainMenuOptions)ReadMainMenuOptions());
}

//New extension:

void ShowDeleteUserScreen()
{
	cout << "=====================================================\n";
	cout << "\t\nDelete User Screen\n";
	cout << "=====================================================\n";

	vector <StUserInfo> vUsers = LoadUsersDataFromFile(UsersFileName);
	string UserName = ReadString("Please enter an user name to delete? ");
	DeleteUserByUserName(UserName, vUsers);
}

void ShowUpdateUserScreen()
{
	cout << "=====================================================\n";
	cout << "\t\tUpdate User Screen\n";
	cout << "=====================================================\n";

	vector <StUserInfo> vUsers = LoadUsersDataFromFile(UsersFileName);
	string UserName = ReadString("Please enter an user name to update? ");
	UpdatedUserByUserName(UserName, vUsers);
}

void ShowAddUsersScreen()
{
	cout << "=====================================================\n";
	cout << "\t\tAdd New User Screen\n";
	cout << "=====================================================\n";

	AddUsers();
}

void ShowFindUserScreen()
{

	cout << "=====================================================\n";
	cout << "\t\tFind User Screen \n";
	cout << "=====================================================\n";

	vector <StUserInfo> vUsers = LoadUsersDataFromFile(UsersFileName);
	StUserInfo User;
	string UserName = ReadString("Please enter an user name to search for: ");

	if (FindUserByUserName(UserName, vUsers, User))
	{
		PrintUserCard(User);
	}
	else
	{
		cout << "User with user name (" << UserName << ") not found!" << endl;
	}
}

bool CheckAccessPermissions(enMainMenuePermissions Permission)
{
	if (CurrentUser.Permission == enMainMenuePermissions::eAll)
		return true;

	if ((Permission & CurrentUser.Permission) == Permission)
		return true;
	else
		return false;
}


void PerformUsersMenuOptions(enUserMenuOptions UserMenuOptions)
{

	switch (UserMenuOptions)
	{
	case enUserMenuOptions::ShowUsersList:
		system("cls");
		ShowListUsersScreen();
		GoBackToUsersMenu();
		break;
	case enUserMenuOptions::AddNewUser:
		system("cls");
		AddUsers();
		GoBackToUsersMenu();
		break;
	case enUserMenuOptions::DeleteUser:
		system("cls");
		ShowDeleteUserScreen();
		GoBackToUsersMenu();
		break;
	case enUserMenuOptions::UpdateUser:
		system("cls");
		ShowUpdateUserScreen();
		GoBackToUsersMenu();
		break;
	case enUserMenuOptions::FindUser:
		system("cls");
		ShowFindUserScreen();
		GoBackToUsersMenu();
		break;
	case enUserMenuOptions::MainMenue:
		system("cls");
		ShowMainMenuScreen();
		break;
	}
}

void ShowUsersMenuScreen()
{

	if (!CheckAccessPermissions(enMainMenuePermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		return;
	}
	system("cls");
	cout << "=====================================================\n";
	cout << "\t\tUsers Menu Screen\n";
	cout << "=====================================================\n";
	cout << "\t[1] Users List.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menu.\n";
	cout << "=====================================================\n";

	PerformUsersMenuOptions((enUserMenuOptions)ReadUsersMenuOptions());
}

bool LoadUserInfo(string UserName, string Password)
{
	if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFailed = false;

	string UserName, Password;
	do
	{
		system("cls");
		cout << "\n---------------------------------\n";  
		cout << "\tLogin Screen"; 
		cout << "\n---------------------------------\n";

		if (LoginFailed)
		{
			cout << "Invalid Username/Password!\n";
		}

		cout << "Enter Username? ";
		getline(cin >> ws, UserName);

		cout << "Enter Password? ";
		getline(cin >> ws, Password);

		LoginFailed = !LoadUserInfo(UserName, Password);

	} while (LoginFailed);

	ShowMainMenuScreen();
}

int main()
{

	Login();

	system("pause>0");
	return 0;
}
