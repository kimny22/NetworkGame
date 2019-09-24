#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>
#include "ddutil.h"

#include <dsound.h>
#include "dsutil.h"


#define _GetKeyState( vkey ) HIBYTE(GetAsyncKeyState( vkey ))
#define _GetKeyPush( vkey )  LOBYTE(GetAsyncKeyState( vkey ))

HWND MainHwnd;

LPDIRECTDRAW         DirectOBJ;
LPDIRECTDRAWSURFACE  RealScreen;
LPDIRECTDRAWSURFACE  BackScreen;
LPDIRECTDRAWSURFACE  SpriteImage;
LPDIRECTDRAWSURFACE  red;
LPDIRECTDRAWSURFACE  orange;
LPDIRECTDRAWSURFACE  yellow;
LPDIRECTDRAWSURFACE  green;
LPDIRECTDRAWSURFACE  blue;
LPDIRECTDRAWSURFACE  purple;
LPDIRECTDRAWSURFACE  black;
LPDIRECTDRAWSURFACE  title;
LPDIRECTDRAWSURFACE  intro;
LPDIRECTDRAWSURFACE  gameOver;

LPDIRECTDRAWCLIPPER	ClipScreen;

int gFullScreen=0, Click=0;
int gWidth=640, gHeight=480;
int state = 0;
int round = 0;
int* answer;
int index = 0;
int* problem;
int note = 0;
////////////////////

LPDIRECTSOUND       SoundOBJ = NULL;
LPDIRECTSOUNDBUFFER SoundDSB = NULL;
DSBUFFERDESC        DSB_desc;

HSNDOBJ Sound[10];


BOOL _InitDirectSound( void )
{
    if ( DirectSoundCreate(NULL,&SoundOBJ,NULL) == DS_OK )
    {
        if (SoundOBJ->SetCooperativeLevel(MainHwnd,DSSCL_PRIORITY)!=DS_OK) return FALSE;

        memset(&DSB_desc,0,sizeof(DSBUFFERDESC));
        DSB_desc.dwSize = sizeof(DSBUFFERDESC);
        DSB_desc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;

        if (SoundOBJ->CreateSoundBuffer(&DSB_desc,&SoundDSB,NULL)!=DS_OK) return FALSE;
        SoundDSB -> SetVolume(DSBVOLUME_MAX); // DSBVOLUME_MIN
        SoundDSB -> SetPan(DSBPAN_RIGHT);
        return TRUE;
    }
    return FALSE;
}

void _Play( int num )
{
    SndObjPlay( Sound[num], NULL );
}

////////////////////////


BOOL Fail( HWND hwnd )
{
    ShowWindow( hwnd, SW_HIDE );
    MessageBox( hwnd, "DIRECT X 초기화에 실패했습니다.", "게임 디자인", MB_OK );
    DestroyWindow( hwnd );
    return FALSE;
}

void _ReleaseAll( void )
{
    if ( DirectOBJ != NULL )
    {
        if ( RealScreen != NULL )
        {
            RealScreen->Release();
            RealScreen = NULL;
        }
        if ( SpriteImage != NULL )
        {
            SpriteImage->Release();
            SpriteImage = NULL;
        }
        if ( red != NULL )
        {
            red->Release();
            red = NULL;
        }
		if (orange != NULL)
		{
			orange->Release();
			orange = NULL;
		}
		if (yellow != NULL)
		{
			yellow->Release();
			yellow = NULL;
		}
		if (green != NULL)
		{
			green->Release();
			green = NULL;
		}
		if (blue != NULL)
		{
			blue->Release();
			blue = NULL;
		}
		if (purple != NULL)
		{
			purple->Release();
			purple = NULL;
		}
		if (black != NULL)
		{
			black->Release();
			black = NULL;
		}
		if (title != NULL)
		{
			title->Release();
			title = NULL;
		}
		if (intro != NULL)
		{
			intro->Release();
			intro = NULL;
		}
		if (gameOver != NULL)
		{
			gameOver->Release();
			gameOver = NULL;
		}
        DirectOBJ->Release();
        DirectOBJ = NULL;
    }
}

long FAR PASCAL WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{

    switch ( message )
    {
        case	WM_KEYDOWN:            
            switch (wParam)
            {
                case VK_ESCAPE:
                case VK_F12: 
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0;    
				case VK_NUMPAD1:
					if (index < round && note == round) {
						_Play(1);
						state = 1;
						answer[index++] = 1;
					}
					break;
				case VK_NUMPAD2:
					if (index < round && note == round) {
						_Play(2);
						state = 2;
						answer[index++] = 2;
					}
					break;
					
				case VK_NUMPAD3:
					if (index < round && note == round) {
						_Play(3);
						state = 3;
						answer[index++] = 3;
					}
					break;					
				case VK_NUMPAD4:
					if (index < round && note == round) {
						_Play(4);
						state = 4;
						answer[index++] = 4;
					}
					break;
				case VK_NUMPAD5:
					if (index < round && note == round) {
						_Play(5);
						state = 5;
						answer[index++] = 5;
					}
					break;
				case VK_NUMPAD6:
					if (index < round && note == round) {
						_Play(6);
						state = 6;
						answer[index++] = 6;
					}
					break;
				case VK_SPACE:
					if (state == 0) {
						state = 8;
						answer = new int[12];
						problem = new int[12];
						break;
					}
					else if (state == 8) {
						round = 1;
						state = 7;
						break;
					}
					break;
			}
            break;

        case    WM_DESTROY      :  _ReleaseAll();
                                    PostQuitMessage( 0 );
                                    break;
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}

BOOL _GameMode( HINSTANCE hInstance, int nCmdShow, int x, int y, int bpp )
{
    HRESULT result;
    WNDCLASS wc;
    DDSURFACEDESC ddsd;
    DDSCAPS ddscaps;
    LPDIRECTDRAW pdd;

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = GetStockBrush(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "Absolute Pitch Test_20173168김나영";
    RegisterClass( &wc );


	if(gFullScreen){
		if((MainHwnd=CreateWindowEx (0, "Absolute Pitch Test_20173168김나영", NULL, WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), 
				GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL ))==NULL)
			ExitProcess(1);
	}
	else{
		if((MainHwnd=CreateWindow("Absolute Pitch Test_20173168김나영", "Absolute Pitch Test_20173168김나영", WS_OVERLAPPEDWINDOW, 0, 0, x, 
									y, NULL, NULL, hInstance, NULL))==NULL)
			ExitProcess(1);
		SetWindowPos(MainHwnd, NULL, 100, 100, x, y, SWP_NOZORDER);
	}

    SetFocus( MainHwnd );
    ShowWindow( MainHwnd, nCmdShow );
    UpdateWindow( MainHwnd );
    ShowCursor( FALSE );

    result = DirectDrawCreate( NULL, &pdd, NULL );
    if ( result != DD_OK ) return Fail( MainHwnd );

    result = pdd->QueryInterface(IID_IDirectDraw, (LPVOID *) &DirectOBJ);
    if ( result != DD_OK ) return Fail( MainHwnd );

	// 윈도우 핸들의 협력 단계를 설정한다.
	if(gFullScreen){
	    result = DirectOBJ->SetCooperativeLevel( MainHwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
		if ( result != DD_OK ) return Fail( MainHwnd );

		result = DirectOBJ->SetDisplayMode( x, y, bpp);
		if ( result != DD_OK ) return Fail( MainHwnd );

		memset( &ddsd, 0, sizeof(ddsd) );
		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
		ddsd.dwBackBufferCount = 1;

	    result = DirectOBJ -> CreateSurface( &ddsd, &RealScreen, NULL );
	   if ( result != DD_OK ) return Fail( MainHwnd );

		memset( &ddscaps, 0, sizeof(ddscaps) );
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		result = RealScreen -> GetAttachedSurface( &ddscaps, &BackScreen );
		if ( result != DD_OK ) return Fail( MainHwnd );
	}
	else{
	    result = DirectOBJ->SetCooperativeLevel( MainHwnd, DDSCL_NORMAL );
		if ( result != DD_OK ) return Fail( MainHwnd );

		memset( &ddsd, 0, sizeof(ddsd) );
	    ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS;
	    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		ddsd.dwBackBufferCount = 0;

		result = DirectOBJ -> CreateSurface( &ddsd, &RealScreen, NULL );
	    if(result != DD_OK) return Fail(MainHwnd);

		memset( &ddsd, 0, sizeof(ddsd) );
		ddsd.dwSize = sizeof(ddsd);
	    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwWidth = x; 
		ddsd.dwHeight = y;
		result = DirectOBJ->CreateSurface( &ddsd, &BackScreen, NULL );
		if ( result != DD_OK ) return Fail( MainHwnd );

		result = DirectOBJ->CreateClipper( 0, &ClipScreen, NULL);
		if ( result != DD_OK ) return Fail( MainHwnd );

		result = ClipScreen->SetHWnd( 0, MainHwnd );
		if ( result != DD_OK ) return Fail( MainHwnd );

		result = RealScreen->SetClipper( ClipScreen );
		if ( result != DD_OK ) return Fail( MainHwnd );

		SetWindowPos(MainHwnd, NULL, 100, 100, x, y, SWP_NOZORDER | SWP_NOACTIVATE); 
	}


    return TRUE;
}

extern void CommInit(int argc, char **argv);
extern void CommSend(char *sending);
extern void CommRecv(char *recvData);

void CALLBACK _GameProc(HWND hWnd, UINT message, UINT wParam, DWORD lParam)
{
	static int frame1 = 0;
	static int frame2 = 0;
	RECT BackRect = { 0, 0, 640, 480 };
	RECT DispRect = { 0, 0, gWidth, gHeight };
	RECT SpriteRect, WinRect;
	char sendData[200];
	if (state == 0) // 제목
		BackScreen->BltFast(0, 0, title, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
	if(state == 1) //도
		BackScreen->BltFast(0, 0, red, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
	if(state == 2) //레
		BackScreen->BltFast(0, 0, orange, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
	if(state == 3) //미
		BackScreen->BltFast(0, 0, yellow, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
	if(state == 4) //파
		BackScreen->BltFast(0, 0, green, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
	if(state == 5) //솔
		BackScreen->BltFast(0, 0, blue, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
	if(state == 6) //라
		BackScreen->BltFast(0, 0, purple, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
	if(state == 7) { //라운드표시
		BackScreen->BltFast(0, 0, black, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
		SpriteRect.left = 160 * ((round-1) % 4);
		SpriteRect.top = 213 * ((round -1) / 4);
		SpriteRect.right = 160 * ((round - 1) % 4) + 160;
		SpriteRect.bottom = 213 * ((round - 1) / 4) + 213;
		BackScreen->BltFast(gWidth / 2 - 80, gHeight / 2 - 107, SpriteImage, &SpriteRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		int i = rand() % 6 + 1;
		if (++frame2 >= 30) { //문제 출제
			if (note < round) {
				_Play(i);
				problem[note++] = i;
				frame2 = 0;
				sprintf(sendData, "answer%d : %d\n", note, problem[note-1]);
				CommSend(sendData);
			}
			else {
				BackScreen->BltFast(0, 0, black, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);				
				if (frame2 < 31) {
					sprintf(sendData, "What's your answer?\n");
					CommSend(sendData);
				}
			}
		}
	}
	if (state == 8) { //키 설명
		BackScreen->BltFast(0, 0, intro, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
	}
	if (state == 9) { //게임 오버
		BackScreen->BltFast(0, 0, gameOver, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
		state == 10;
	}
	if (index == round && state != 0 && state != 8) { //라운드 수만큼 입력 다 하고
		if (++frame1 >= 30) { //정답이면 다음 라운드
			state = 7;
			index = 0;
			round++;
			note = 0;
			frame1 = 0;
			for (int i = 0; i < round - 1;i++) { // 오답이면 게임오버
				if (problem[i] == answer[i]);
				else {
					state = 9;
					_Play(7);
				}
			}
		}
	}

	if (gFullScreen)
		RealScreen->Flip(NULL, DDFLIP_WAIT);
	else {
		GetWindowRect(MainHwnd, &WinRect);
		RealScreen->Blt(&WinRect, BackScreen, &DispRect, DDBLT_WAIT, NULL);
	}
}


int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    MSG msg;
    if ( !_GameMode(hInstance, nCmdShow, gWidth, gHeight, 32) ) return FALSE;

    SpriteImage = DDLoadBitmap( DirectOBJ, "numbers.BMP", 0, 0 );
    red  = DDLoadBitmap( DirectOBJ, "red.BMP", 0, 0 );
	orange = DDLoadBitmap(DirectOBJ, "orange.BMP", 0, 0);
	yellow = DDLoadBitmap(DirectOBJ, "yellow.BMP", 0, 0);
	green = DDLoadBitmap(DirectOBJ, "green.BMP", 0, 0);
	blue = DDLoadBitmap(DirectOBJ, "blue.BMP", 0, 0);
	purple = DDLoadBitmap(DirectOBJ, "purple.BMP", 0, 0);
	black = DDLoadBitmap(DirectOBJ, "black.BMP", 0, 0);
	title = DDLoadBitmap(DirectOBJ, "title.BMP", 0, 0);
	intro = DDLoadBitmap(DirectOBJ, "intro.BMP", 0, 0);
	gameOver = DDLoadBitmap(DirectOBJ, "gameOver.BMP", 0, 0);
    DDSetColorKey( SpriteImage, RGB(0,0,0) );

	SetTimer(MainHwnd, 1, 30, _GameProc);
	CommInit(NULL, NULL);

    if ( _InitDirectSound() )
    {
        Sound[1] = SndObjCreate(SoundOBJ,"do.WAV",2);
        Sound[2] = SndObjCreate(SoundOBJ,"re.WAV",2);
        Sound[3] = SndObjCreate(SoundOBJ,"mi.WAV",2);
        Sound[4] = SndObjCreate(SoundOBJ,"fa.WAV",2);
        Sound[5] = SndObjCreate(SoundOBJ,"sol.WAV",2);
		Sound[6] = SndObjCreate(SoundOBJ, "la.WAV", 2);
		Sound[7] = SndObjCreate(SoundOBJ, "end.WAV", 2);
    }

    while ( !_GetKeyState(VK_ESCAPE) )
    {

        if ( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
        {
            if ( !GetMessage(&msg, NULL, 0, 0) ) return msg.wParam;

            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }
    DestroyWindow( MainHwnd );

    return TRUE;
}
