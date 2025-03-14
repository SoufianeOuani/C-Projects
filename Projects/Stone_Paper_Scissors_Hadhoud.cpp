#include <iostream>
#include <cstdlib>
using namespace std;


enum enChoices { Stone = 1, Paper = 2, Scissors = 3 };

enum enWinner { Player = 1, Computer = 2, Draw = 3 };

int RandomNumbersGenerator(short From, short To)
{
	return rand() % (To - From + 1) + From;
}

struct StRoundInfo
{
	short RoundNumber = 0;
	enChoices PlayerChoice;
	enChoices ComputerChoice;
	enWinner RoundWinner;
	string RoundWinnerName = "";

};

struct StGameResults
{
	short GameRounds = 0,
		PlayerWinTimes = 0,
		ComputerWinTimes = 0,
		Draws = 0;
	enWinner GameWinner;
	string GameWinnerName = "";
};

enWinner WhoWonTheRound(StRoundInfo RoundInfo)
{

	if (RoundInfo.PlayerChoice == RoundInfo.ComputerChoice)
	{
		return enWinner::Draw;
	}


	switch (RoundInfo.PlayerChoice)
	{
	case enChoices::Stone:
		if (RoundInfo.ComputerChoice == Paper)
		{
			return enWinner::Computer;
		}
		break;
	case enChoices::Paper:
		if (RoundInfo.ComputerChoice == Scissors)
		{
			return enWinner::Computer;
		}
		break;
	case enChoices::Scissors:
		if (RoundInfo.ComputerChoice == Stone)
		{
			return enWinner::Computer;
		}
		break;
	}

	return enWinner::Player;
}

enWinner WhoWonTheGame(short PlayerWinTimes, short ComputerWinTimes)
{
	if (PlayerWinTimes > ComputerWinTimes)
	{
		return enWinner::Player;
	}
	else if (ComputerWinTimes > PlayerWinTimes)
	{
		return enWinner::Computer;
	}
	else
	{
		return enWinner::Draw;
	}

}

void SetWinnerScreenColor(enWinner Winner)
{
	switch (Winner)
	{
	case enWinner::Player:
		system("color 2f");
		break;
	case enWinner::Computer:
		system("color 4f");
		cout << "\a";
		break;
	default:
		system("color 6f");
		break;
	}
}

string ChoiceName(enChoices Choice)
{
	string ArrChoiceName[3] = { "Stone", "Paper", "Scissors" };
	return ArrChoiceName[Choice - 1];
}

string WinnerName(enWinner Winner)
{
	string ArrWinnerName[3] = { "Player", "Computer", "No Winner (Draw)" };
	return ArrWinnerName[Winner - 1];
}

void PrintRoundResults(StRoundInfo RoundInfo)
{
	

	cout << "\n____________ Round [" << RoundInfo.RoundNumber << "] ____________\n\n";
	cout << "Player1 Choice: " << ChoiceName(RoundInfo.PlayerChoice) << endl;
	cout << "Computer Choice: " << ChoiceName(RoundInfo.ComputerChoice) << endl;
	cout << "Round Winner   : [" << RoundInfo.RoundWinnerName << "]\n";
	cout << "___________________________________\n" << endl;

	SetWinnerScreenColor(RoundInfo.RoundWinner);

}

StGameResults FillGameResults(short GameRounds, short PlayerWinTimes, short ComputerWinTimes, short DrawTimes)
{
	StGameResults GameResults;

	GameResults.GameRounds = GameRounds;
	GameResults.PlayerWinTimes = PlayerWinTimes;
	GameResults.ComputerWinTimes = ComputerWinTimes;
	GameResults.Draws = DrawTimes;
	GameResults.GameWinner = WhoWonTheGame(PlayerWinTimes, ComputerWinTimes);
	GameResults.GameWinnerName = WinnerName(GameResults.GameWinner);

	return GameResults;

}

enChoices ReadPlayerChoice()
{
	short Choice = 0;

	do
	{

		cout << "\nYour Choice : [1]:Stone, [2]:Paper, [3]:Scissors ? ";
		cin >> Choice;

	} while (Choice < 1 || Choice > 10);

	return (enChoices)Choice;
}

enChoices GetComputerChoice()
{
	return (enChoices)RandomNumbersGenerator(1, 3);
}

string Tabs(short NumberOfTabs)
{
	string t = "";

	for (short i = 1; i < NumberOfTabs; i++)
	{
		t += "\t";
	}

	return t;
}

void ShowGameOverScreen()
{
	cout << Tabs(5)<< "______________________________________________" << endl;
	cout << Tabs(5) << "\n\t\t\t\t\t\tG a m e O v e r\t\t" << endl;
	cout << Tabs(5) << "______________________________________________" << endl;
}

void ShowFinalGameResults(StGameResults GameResults)
{

	cout << Tabs(5) << "Game Rounds        : " << GameResults.GameRounds << endl;
	cout << Tabs(5) << "PLayer Win Times   : " << GameResults.PlayerWinTimes << endl;
	cout << Tabs(5) << "Computer Win Times : " << GameResults.ComputerWinTimes << endl;
	cout << Tabs(5) << "Draw Times         : " << GameResults.Draws << endl;
	cout << Tabs(5) << "Finl Winner        : " << GameResults.GameWinnerName << endl;
	cout << Tabs(5) << "______________________________________________" << endl;

}

short ReadHowManyRounds()
{
	short GameRounds = 0;

	do
	{
		cout << "How many rounds 1 to 10? ";
		cin >> GameRounds;
		cout << endl;

	} while (GameRounds < 1 || GameRounds > 10);

	return GameRounds;

}

StGameResults PlayGame(short HowManyRounds)
{
	StRoundInfo RoundInfo;
	short PlayerWinTimes = 0, ComputerWinTimes = 0, DrawTimes = 0;


	for (short GameRounds = 1; GameRounds <= HowManyRounds; GameRounds++)
	{
		RoundInfo.RoundNumber = GameRounds;
		cout << "Round " << RoundInfo.RoundNumber << " begins" << endl;
		RoundInfo.PlayerChoice = ReadPlayerChoice();
		RoundInfo.ComputerChoice = GetComputerChoice();
		RoundInfo.RoundWinner = WhoWonTheRound(RoundInfo);
		RoundInfo.RoundWinnerName = WinnerName(RoundInfo.RoundWinner);

		PrintRoundResults(RoundInfo);


		if (RoundInfo.RoundWinner == enWinner::Player)
			PlayerWinTimes++;
		else if (RoundInfo.RoundWinner == enWinner::Computer)
			ComputerWinTimes++;
		else
			DrawTimes++;

	}

	return FillGameResults(HowManyRounds, PlayerWinTimes, ComputerWinTimes, DrawTimes);


}

void ResetScreen()
{
	system("cls");
	system("color 0f");
}

void StartGame()
{
	char PlayAgain = 'Y';

	do
	{

		ResetScreen();
		StGameResults GameResults = PlayGame(ReadHowManyRounds());
		ShowGameOverScreen();
		ShowFinalGameResults(GameResults);

		cout << endl << Tabs(5) << "Do you want to play again (Y/N)? ";
		cin >> PlayAgain;

	} while (PlayAgain == 'Y' || PlayAgain == 'y');
}

int main()
{
	srand((unsigned)time(NULL));

	StartGame();

	return 0;

}