#include <iostream>
#include <cstdlib>
using namespace std;

enum enOperattionType { Add = 1, Suntract = 2, Multiply = 3, Divide = 4, OPMix=5 };
enum enQuestionLevel { Easy = 1, Medium = 2, Hard = 3, MaxLevel=4 };

struct StQuestion
{
	short Number1 = 0,
		Number2 = 0;
	enOperattionType OperationType;
	enQuestionLevel QuestionLevel;
	short PlayerAnswer = 0,
		CorrectAnswer = 0;
	bool AnswerResult = false;
};

struct StExam
{
	StQuestion QuestionList[100];
	short NumberOfQuestions = 0;
	enOperattionType OpType;
	enQuestionLevel QuestionLevel;
	short NumberOfRightAnswers = 0,
		NumberOFWrongAnswers = 0;
	bool isPass = false;
};

short RandomNumbersGenerator(short From, short To)
{
	return rand() % (To - From + 1) + From;
}

short ReadHowManyQuestions()
{
	short NumberOfQuestions = 0;

	do
	{
		cout << "How many questions do you want to answer ? ";
		cin >> NumberOfQuestions;

	} while (NumberOfQuestions < 1 || NumberOfQuestions > 20);

	return NumberOfQuestions;
}

enQuestionLevel ReadQuestionLevel()
{
	short QuestionLevel = 0;

	do
	{
		cout << "Enter question level [1]:EASY, [2]:MEDIUM, [3]:HARD, [4]:Mix ? ";
		cin >> QuestionLevel;

	} while (QuestionLevel < 1 || QuestionLevel > 4);

	return (enQuestionLevel)QuestionLevel;
}

enOperattionType ReadOperationType()
{
	short OperationType = 0;

	do
	{
		cout << "Enter an operation type [1]:Add, [2]:Subtract, [3]:Multiply, [4]:Divide, [5]:Mix ? ";
		cin >> OperationType;

	} while (OperationType < 1 || OperationType > 5);

	return (enOperattionType)OperationType;
}

short SimpleCalculator(short Number1, short Number2, enOperattionType Operationtype)
{
	switch (Operationtype)
	{
	case enOperattionType::Add:
		return Number1 + Number2;
	case enOperattionType::Suntract:
		return Number1 - Number2;
	case enOperattionType::Multiply:
		return Number1 * Number2;
	case enOperattionType::Divide:
		return Number1 / Number2;
	default:
		return Number1 + Number2;
	}
}

string GetOperationTypeSymbol(enOperattionType OperationType)
{
	switch (OperationType)
	{
	case enOperattionType::Add:
		return "+";
	case enOperattionType::Suntract:
		return "-";
	case enOperattionType::Multiply:
		return "*";
	case enOperattionType::Divide:
		return "/";
	default:
		return "Mix";
	}
}

string GetQuestionLevelText(enQuestionLevel QuestionLevel)
{
	string ArrLevel[4] = { "Easy","Medium","Hard", "Mix"};
	return ArrLevel[QuestionLevel - 1];
}

string GetFinalResultsText(bool isPass)
{
	if (isPass)
	{
		system("color 2f");
		return "PASS :-)";
	}
	else
	{
		system("color 4f");
		cout << "\a";
		return "FAIL :-(";
	}
}

enOperattionType GetRandomOperationType()
{
	short Op = RandomNumbersGenerator(1, 4);
	return (enOperattionType)Op;
}

StQuestion GenerateQuestion(enOperattionType OpType, enQuestionLevel QuestionLevel)
{

	StQuestion Question;

	if (QuestionLevel == enQuestionLevel::MaxLevel)
		QuestionLevel = (enQuestionLevel)RandomNumbersGenerator(1, 3);

	if (OpType == enOperattionType::OPMix)
		OpType = GetRandomOperationType();

	Question.OperationType = OpType;

	switch (QuestionLevel)
	{
	case enQuestionLevel::Easy:

		Question.Number1 = RandomNumbersGenerator(1, 10);
		Question.Number2 = RandomNumbersGenerator(1, 10);

		Question.CorrectAnswer = SimpleCalculator(Question.Number1, Question.Number2, Question.OperationType);
		Question.QuestionLevel = QuestionLevel;
		return Question;

	case enQuestionLevel::Medium:

		Question.Number1 = RandomNumbersGenerator(10, 50);
		Question.Number2 = RandomNumbersGenerator(10, 50);

		Question.CorrectAnswer = SimpleCalculator(Question.Number1, Question.Number2, Question.OperationType);
		Question.QuestionLevel = QuestionLevel;

		return Question;

	case enQuestionLevel::Hard:

		Question.Number1 = RandomNumbersGenerator(50, 100);
		Question.Number2 = RandomNumbersGenerator(50, 100);

		Question.CorrectAnswer = SimpleCalculator(Question.Number1, Question.Number2, Question.OperationType);
		Question.QuestionLevel = QuestionLevel;

		return Question;
	}

	return Question;

}

void GenerateQuestionList(StExam& Exam)
{
	for (short QuestionNumber = 0; QuestionNumber < Exam.NumberOfQuestions; QuestionNumber++)
	{
		Exam.QuestionList[QuestionNumber] = GenerateQuestion(Exam.OpType, Exam.QuestionLevel);
	}
}

void PrintTheQuestion(StExam Exam, short QuestionNumber)
{
	cout << "\nQuestion [" << QuestionNumber + 1 << "/" << Exam.NumberOfQuestions << "] \n\n";
	cout << Exam.QuestionList[QuestionNumber].Number1 << " " << GetOperationTypeSymbol(Exam.QuestionList[QuestionNumber].OperationType) << " " << Exam.QuestionList[QuestionNumber].Number2 << endl;
	cout << "____________\n";
	cout << "= \n";
}

short ReadPlayerAnswer()
{
	short PlayerAnswer = 0;
	cin >> PlayerAnswer;
	return PlayerAnswer;
}

void SetScreenColor(bool isRight)
{
	if (isRight)
		system("color 2f");
	else
		system("color 4f");
}

void CorrectTheAnswer(StExam& Exam, short QuestionNumber)
{
	if (Exam.QuestionList[QuestionNumber].PlayerAnswer != Exam.QuestionList[QuestionNumber].CorrectAnswer)
	{
		Exam.NumberOFWrongAnswers++;
		Exam.QuestionList[QuestionNumber].AnswerResult = false;

		cout << "\nWrong Answer :-(\n";
		cout << "The correct answer is : ";
		cout << Exam.QuestionList[QuestionNumber].CorrectAnswer;
	}
	else
	{
		Exam.NumberOfRightAnswers++;
		Exam.QuestionList[QuestionNumber].AnswerResult = true;

		cout << "\nRight Answer :-)\n";
	}
	cout << endl;

	SetScreenColor(Exam.QuestionList[QuestionNumber].AnswerResult);
}

void AskAndCorrectQuestionAnswer(StExam& Exam)
{
	for (short QuestionNumber = 0; QuestionNumber < Exam.NumberOfQuestions; QuestionNumber++)
	{
		PrintTheQuestion(Exam,QuestionNumber);
		Exam.QuestionList[QuestionNumber].PlayerAnswer = ReadPlayerAnswer();
		CorrectTheAnswer(Exam, QuestionNumber);
	}

	Exam.isPass = (Exam.NumberOfRightAnswers >= Exam.NumberOFWrongAnswers);
}

void PrintFinalResults(StExam Exam)
{
	cout << "\n\n____________________________________\n\n";
	cout << "\tResult is : " << GetFinalResultsText(Exam.isPass) << endl;
	cout << "____________________________________\n\n";
	cout << "  Number of Questions     : " << Exam.NumberOfQuestions << endl;
	cout << "  Question Level          : " << GetQuestionLevelText(Exam.QuestionLevel) << endl;
	cout << "  Operation Type          : " << GetOperationTypeSymbol(Exam.OpType) << endl;
	cout << "  Number of right answers : " << Exam.NumberOfRightAnswers << endl;
	cout << "  Number of wrong answers : " << Exam.NumberOFWrongAnswers << endl;
	cout << "____________________________________\n";
}

void PlayMathGame()
{
	StExam Exam;

	Exam.NumberOfQuestions = ReadHowManyQuestions();
	Exam.QuestionLevel = ReadQuestionLevel();
	Exam.OpType = ReadOperationType();

	GenerateQuestionList(Exam);
	AskAndCorrectQuestionAnswer(Exam);
	PrintFinalResults(Exam);
}

void ResetScreen()
{
	system("color 0f");
	system("cls");
}

void StartGame()
{
	char PlayAgain = 'Y';

	do
	{
		ResetScreen();
		PlayMathGame();

		cout << "Do you want to play again (Y/N)? ";
		cin >> PlayAgain;

	} while (PlayAgain == 'Y' || PlayAgain == 'y');
}

int main()
{
	srand((unsigned)time(NULL));

	StartGame();

	return 0;
}