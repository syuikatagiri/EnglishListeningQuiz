#define _CRT_SECURE_NO_WARNINGS

#include "DxLib.h"
#include "EnglishListeningQuiz.h"
#include <time.h>
#include <errno.h>

int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd ) {

	quizData quizData;

	SetGraphMode(640, 480, 16);

	ChangeWindowMode( TRUE );

	if ( DxLib_Init() == -1 ){

		return -1;
	}

	/*デバッグ用コード-----------------------------------*/
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	printf("hellotest");
	/*デバッグ用コード-----------------------------------*/

	InitCurrentQuestion( &quizData );
	InitQuestions( &quizData );
	InitPathSound( &quizData );

	while ( ProcessMessage != 0 ) {

		if ( questionCount == QUESTIONS_MAX_NUM ) {

			break;
		}

		ClearDrawScreen();

		SetDrawScreen( DX_SCREEN_FRONT );

		OutputQuestion( &quizData );

		OutputSound( &quizData );

		InputAnswer( &quizData );

		CheckAnswer( &quizData );

		RateAnswer( g_missCount );

		OutputAnswer( &quizData );

		WaitKey();

		questionCount++;
	}

	/*デバッグ用コード-----------------------------------*/

	printf("%s\n", quizData.sentence[0]);
	printf("%s\n", quizData.sentence[1]);
	printf("test");
	FreeConsole();
	/*デバッグ用コード-----------------------------------*/

	DxLib_End();

	return 0;
}

void InitCurrentQuestion( quizData* quizData ) {

	int temp = 0;
	int i = 0;
	int j = 0;

	srand( ( unsigned int )time( NULL ) );

	for ( i = 0; i < QUESTIONS_MAX_NUM; i++ ) {

		quizData -> currentQuestion[i] = i;
	}

	for ( i = 0; i < QUESTIONS_MAX_NUM; i++ ) {

		j = rand() % QUESTIONS_MAX_NUM;

		temp = quizData->currentQuestion[i];
		quizData->currentQuestion[i] = quizData->currentQuestion[j];
		quizData->currentQuestion[j] = temp;
	}
}

void InitQuestions( quizData* quizData ) {

	int row = 0;
	int col = 0;
	int c;
	FILE* fp = NULL;
	
	memset(quizData->sentence, '\0', QUESTIONS_MAX_NUM);

	if (fopen_s(&fp, PATH_TEXT, "r") == 0 ) {

		for ( ; row < QUESTIONS_MAX_NUM; row++ ) {

			while ( ( c = fgetc(fp) ) != ENTER ) {

				quizData->sentence[row][col++] = c;
			}

			quizData->sentence[row][col] = '\0';

			col = 0;
		}
	}

	else {

		MessageBox( NULL, TEXT("Can't open Text File!!"), TEXT("Error"), MB_ICONSTOP );
		fclose(fp);
		exit(0);
	}

	fclose( fp );
}

void InitPathSound( quizData* quizData ) {

	int row = 0;
	int col = 0;
	int i = 0;
	int j = 0;

	for ( row = 0; row < QUESTIONS_MAX_NUM; row++ ) {

		strcpy_s( &quizData->soundPath[row][col], 100, PATH_SOUND );
	}

	for (row = 0; row < QUESTIONS_MAX_NUM; row++) {

		col = (int)strlen( &quizData->soundPath[row][0] );

		strcpy_s( &quizData->soundPath[row][col], 100, &quizData->sentence[row][0] );
		
	}

	for ( row = 0; row < QUESTIONS_MAX_NUM; row++ ) {

		col = (int)strlen( &quizData->soundPath[row][0] );

		strcpy_s( &quizData->soundPath[row][col], 100, PATH_SUFFIX );
	}
}

void OutputQuestion( quizData* quizData ) {

	int x = 0;
	int row = 0;
	int col = 0;

	row = quizData->currentQuestion[questionCount];

	while ( quizData->sentence[row][col] != '\0' && g_drawSentenceFlag == 1 ) {

		if ( ProcessMessage() == -1 ) { break; }

		DrawFormatString( x, SECOND_LINE, GetColor(255, 255, 255), "%c", quizData->sentence[row][col] );

		x = x + 10;
		col++;
	}

	DrawString( 0, FIRST_LINE, "Listening Test", GetColor(255, 255, 255) );
}

void OutputSound( quizData* quizData ) {

	int row = 0;

	row = quizData->currentQuestion[questionCount];

	g_err = PlaySoundFile( &quizData->soundPath[row][0], DX_PLAYTYPE_BACK );

	if (g_err != 0) {

		MessageBox( NULL, TEXT( "Can't open Mp3 File!!" ), TEXT( "Error" ), MB_ICONSTOP );
		exit(0);
	}
}

void InputAnswer( quizData* quizData ) {

	int inputHandle = 0;

	inputHandle = MakeKeyInput( MAX_SIZE_STRING, FALSE, TRUE, FALSE );
	SetActiveKeyInput(inputHandle);


	while ( CheckKeyInput( inputHandle ) == 0 ) {

		ClearDrawScreen();

		SetDrawScreen( DX_SCREEN_BACK );

		DrawKeyInputModeString( 640, 480 );

		DrawKeyInputString( 0, THIRD_LINE, inputHandle );

		GetKeyInputString( quizData->answer, inputHandle );

		if ( strlen( quizData->answer ) <= ANSWER_MIN_SIZE ) {

			DrawFormatString( 0, FIVE_LINE, GetColor( 255, 255, 255 )
								,"入力文字数は%d以上入力してください。TABキー:音声を再度、再生" , ANSWER_MIN_SIZE );
		}

		if ( CheckHitKey( KEY_INPUT_LCONTROL ) == 1 && g_previouKeyFlag == 0 ) {

			if ( g_drawSentenceFlag == 0 ) {

				g_drawSentenceFlag = 1;
			}
			else if ( g_drawSentenceFlag == 1 ) {

				g_drawSentenceFlag = 0;
			}
		}

		if ( g_drawSentenceFlag == 1 ) {

			OutputQuestion( quizData );
		}

		g_previouKeyFlag = CheckHitKey( KEY_INPUT_LCONTROL );

		if ( CheckHitKey(KEY_INPUT_TAB) == 1 ) {

			OutputSound( quizData );
		}

		ScreenFlip();
	}
}

void CheckAnswer( quizData* quizData ) {

	int row = 0;
	int col = 0;
	int i = 0;
	int j = 0;

	row = quizData->currentQuestion[questionCount];

	while ( !ProcessMessage() ) {

		if ( quizData->sentence[row][col] == '\0' ) { break; }

		if ( quizData->answer[j] == SPACE || quizData->answer[j] == CAMMA || quizData->answer[j] == QUEST ) {

			j++;
			continue;
		}

		if ( quizData->sentence[row][col] == SPACE || quizData->sentence[row][col] == CAMMA || quizData->sentence[row][col] == QUEST ) {

			col++;
			continue;
		}

		if ( tolower( quizData->answer[j] ) != tolower( quizData->sentence[row][col] ) ) {

			g_missCount++;
		}

		col++;
		j++;
	}
}

void RateAnswer( int g_missCount ) {

	if ( g_missCount == 0 ) {

		DrawString( 0, FOUR_LINE, "Perfect!", GetColor(255, 255, 255) );
	}
	
	else if ( g_missCount <= 3 ) {

		DrawString( 0, FOUR_LINE, "So close!", GetColor(255, 255, 255) );
	}

	else if ( g_missCount <= 6 ) {

		DrawString( 0, FOUR_LINE, "Good", GetColor(255, 255, 255) );
	}

	else {

		DrawString( 0, FOUR_LINE, "You have to study hard!!", GetColor(255, 255, 255) );
	}

	g_missCount = 0;
}

void OutputAnswer( quizData* quizData ) {

	int x = 0;
	int row = 0;
	int col = 0;

	/* 行を指定 */
	row = quizData->currentQuestion[questionCount];

	DrawString( 0, SIX_LINE, "答え", GetColor(255, 255, 255) );

	while ( quizData->sentence[row][col] != '\0' ) {

		if ( ProcessMessage() == -1 ) { break; }

		DrawFormatString( x, SEVEN_LINE, GetColor(205, 92, 92), "%c", quizData->sentence[row][col] );

		x = x + 10;
		col++;
	}

	/* 裏画面を表画面へ反映 */
	ScreenFlip();
}