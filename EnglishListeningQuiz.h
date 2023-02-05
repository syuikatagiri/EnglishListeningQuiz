#pragma once

#define MAX_EXTENSION_ 5
#define STRING_MAX_SIZE 300
#define ANSWER_MIN_SIZE	10
#define MAX_SIZE_STRING 300

#define CORON 58
#define CAMMA 76
#define ENTER 10
#define SPACE 32
#define QUEST 95

#define PATH_TEXT	".\\text\\question.txt"
#define PATH_SOUND	".\\sound\\question\\"
#define PATH_SUFFIX		".mp3"
#define QUESTIONS_MAX_NUM 8

#define FIRST_LINE  0
#define SECOND_LINE 15
#define THIRD_LINE  30
#define FOUR_LINE 45
#define FIVE_LINE 60
#define SIX_LINE 75
#define SEVEN_LINE 90

typedef struct {

	int	 currentQuestion[QUESTIONS_MAX_NUM];
	char sentence[STRING_MAX_SIZE][STRING_MAX_SIZE];
	char soundPath[MAX_SIZE_STRING][MAX_SIZE_STRING];
	char answer[MAX_SIZE_STRING];

}quizData;

int questionCount = 0;
int g_drawSentenceFlag = 1;
int g_previouKeyFlag = 0;
int g_missCount = 0;
errno_t g_err;

void InitCurrentQuestion( quizData* quizData );

void InitQuestions( quizData* quizData );

void InitPathSound( quizData* quizData );

void OutputQuestion( quizData* quizData );

void OutputSound( quizData* quizData );

void InputAnswer( quizData* quizData );

void CheckAnswer( quizData* quizData );

void RateAnswer( int g_missCount );

void OutputAnswer( quizData* quizData );
