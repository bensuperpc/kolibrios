#include "kosSyst.h"

#define atexitBufferSize	32

// Autobuild uses FASM method for exe->kos,
// MENUET01 header should be present in EXE.
#ifdef AUTOBUILD
char kosExePath[1024];
char exeStack[16384];
// must be alphabetically first in the image
#pragma data_seg(".1seg")
extern "C" struct
{
	char header[8];
	int headerver;
	void* entry;
	void* i_end;
	void* memsize;
	void* stack;
	void* params;
	void* icon;
} header = {
	{'M', 'E', 'N', 'U', 'E', 'T', '0', '1'},
	1,
	&crtStartUp,
	0,	// filled by doexe2.asm
	0,	// filled by doexe2.asm
	exeStack + sizeof(exeStack),
	NULL,
	kosExePath
};
#pragma data_seg()
#else
char *kosExePath = NULL;
#endif

char pureCallMessage[] = "PURE function call!";

//
void (__cdecl *atExitList[atexitBufferSize])();
int atExitFnNum = 0;
//
int __cdecl atexit( void (__cdecl *func )( void ))
{
	//
	if ( atExitFnNum < atexitBufferSize )
	{
		//
		atExitList[atExitFnNum++] = func;
		return 0;
	}
	else
	{
		return 1;
	}
}


//
Dword RandomSeed = 1;
//
void rtlSrand( Dword seed )
{
	RandomSeed = seed;
}
//
Dword rtlRand( void )
{
  //????? 0x80000776

  Dword dwi, i;

  for ( i = 0; i < 32; i++ )
  {

    dwi = RandomSeed & 0x80000776;
  
      __asm{
            mov   eax, dwi
            mov   edx, eax
            bswap eax
            xor   eax, edx
            xor   al, ah
            setpo al
            movzx eax, al
            mov   dwi, eax
    }

    RandomSeed = ( RandomSeed << 1 ) | ( dwi & 1 );
  }
  
 return RandomSeed;
}

//
void * __cdecl memcpy( void *dst, const void *src, size_t bytesCount )
{
	__asm{
		mov edi, dst
		mov eax, dst
		mov esi, src
		mov ecx, bytesCount
		rep movsb
	}
}

//
void memset( Byte *dst, Byte filler, Dword count )
{
	//
	__asm{
		mov edi, dst
		mov al, filler
		mov ecx, count
		rep stosb
	}
}

//
Dword rtlInterlockedExchange( Dword *target, Dword value )
{
//	Dword result;

	//
	__asm{
		mov eax, value
		mov ebx, target
		xchg eax, [ebx]
//		mov result, eax
	}
	//
//	return result;
}


//////////////////////////////////////////////////////////////////////
//
// ??????????? ??????
//

char * __cdecl strcpy( char *target, const char *source )
{
	char *result = target;

	while( target[0] = source[0] )
	{
		target++;
		source++;
	}

	return result;
}


//////////////////////////////////////////////////////////////////////
//
// ??????????? ????? ???????
//

char * __cdecl strrchr( const char * string, int c )
{
	char *cPtr;

	//
	for ( cPtr = (char *)string + strlen( string ); cPtr >= string; cPtr-- )
	{
		//
		if ( *cPtr == c ) return cPtr;
	}
	//
	return NULL;
}


//////////////////////////////////////////////////////////////////////
//
// ??????????? ????? ??????
//

int __cdecl strlen( const char *line )
{
  int i;

  for( i=0; line[i] != 0; i++ );
  return i;
}



//////////////////////////////////////////////////////////////////////
//
// ??????? ?????????????????? ????? ? ??????
//

unsigned int num2hex( unsigned int num )
{
  if( num < 10 )
    return num + '0';
  return num - 10 + 'A';
}


//////////////////////////////////////////////////////////////////////
//
// ????? ?????? ?? ??????
//

Dword dectab[] = { 1000000000, 100000000, 10000000, 1000000, 100000,
                   10000, 1000, 100, 10, 0 };

//
void sprintk( char *Str, PRINTK *arg )
{
	int i, fmtlinesize, j, ac, k, flag;
	Dword head, tail;
	char c;
	
	//
	ac = 0;

	//
	fmtlinesize = strlen( arg->fmtline );
	//
	if( fmtlinesize == 0 ) return;
  
	//
	for( i = 0, j = 0; i < fmtlinesize; i++ )
	{
		//
		c = arg->fmtline[i];
		//
		if( c != '%' )
		{
			Str[j++] = c;
			continue;
		}
		//
		i++;
		//
		if( i >= fmtlinesize ) break;

		//
		flag = 0;
		//
		c = arg->fmtline[i];
		//
		switch( c )
		{
		//
		case '%':
			Str[j++] = c;
			break;
		// ????? ??????
		case 'S':
			for( k = 0; ( c = ((Byte *)arg->args[ac])[k] ) != 0; k++ )
			{
				Str[j++] = c;
			}
			ac++;
			break;
		// ????? ?????
		case 'B':
			k = (int)(arg->args[ac]) & 0xFF;
			Str[j++] = num2hex( ( k >> 4 ) & 0xF );
			Str[j++] = num2hex( k & 0xF );
			ac++;
			break;
		// ????? ???????
		case 'C':
			Str[j++] = (int)(arg->args[ac]) & 0xFF;
			ac++;
			break;
		// ????? ???????? ????? ? ????????????????? ????
		case 'X':
			for( k = 7; k >= 0; k-- )
			{
				//
				c = num2hex ( ( (Dword)(arg->args[ac]) >> (k * 4) ) & 0xF );
				//
				if( c == '0' )
				{
					if( flag ) Str[j++] = c;
				}
				else
				{
					flag++;
					Str[j++] = c;
				}
			}
			//
			if( flag == 0 ) Str[j++] = '0';
			ac++;
			break;
		// ????? ???????? ????? ? ?????????? ????
		case 'U':
			head = (Dword)(arg->args[ac]);
			tail = 0;
			for( k = 0; dectab[k] != 0; k++ )
			{
				tail = head % dectab[k];
				head /= dectab[k];
				c = head + '0';
				if( c == '0' )
				{
					if( flag ) Str[j++] = c;
				}
				else
				{
					flag++;
					Str[j++] = c;
				}
				//
				head = tail;
			}
			//
			c = head + '0';
			Str[j++] = c;
			ac++;
			break;
		// ????? 64-??????? ????? ? ????????????????? ????
		case 'Q':
			for( k = 7; k >= 0; k-- )
			{
				//
				c = num2hex ( ( *((unsigned int *)(arg->args[ac]) + 1) >> (k * 4) ) & 0xF );
				//
				if( c == '0' )
				{
					if( flag ) Str[j++] = c;
				}
				else
				{
					flag++;
					Str[j++] = c;
				}
			}
			//
			for( k=7; k >= 0; k-- )
			{
				//
				c = num2hex ( ( *((unsigned int *)(arg->args[ac])) >> (k * 4) ) & 0xF );
				//
				if( c == '0' )
				{
					if( flag ) Str[j++] = c;
				}
				else
				{
					flag++;
					Str[j++] = c;
				}
			}
			//
			if( flag == 0 ) Str[j++] = '0';
			//
			ac++;
			break;
		//
		default:
			break;
		}
	}
	//
	Str[j] = 0;
}


// ??????? -1 ?????????? ????????
void kos_ExitApp()
{
	int i;

	//
	for ( i = atExitFnNum - 1; i >= 0; i-- )
	{
		//
		atExitList[i]();
	}
	//
	__asm{
		mov eax, -1
		int 0x40
	}
}


// ??????? 0
void kos_DefineAndDrawWindow(
	Word x, Word y,
	Word sizeX, Word sizeY,
	Byte mainAreaType,
	Dword mainAreaColour,
	Byte headerType,
	Dword headerColour,
	char *title
	)
{
	Dword arg1, arg2, arg3, arg4;

	//
	arg1 = ( x << 16 ) + sizeX;
	arg2 = ( y << 16 ) + sizeY;
	arg3 = ( mainAreaType << 24 ) | mainAreaColour;
	arg4 = ( headerType << 24 ) | headerColour;
	//
	__asm{
		mov eax, 0
		mov ebx, arg1
		mov ecx, arg2
		mov edx, arg3
		mov esi, arg4
		mov edi, title
		int 0x40
	}
}


// ??????? 1 ????????? ?????
void kos_PutPixel( Dword x, Dword y, Dword colour )
{
	//
	__asm{
		mov eax, 1
		mov ebx, x
		mov ecx, y
		mov edx, colour
		int 0x40
	}
}


// ??????? 2 ???????? ??? ??????? ???????
bool kos_GetKey( Byte &keyCode )
{
	Dword result;

	//
	__asm{
		mov eax, 2
		int 0x40
		mov result, eax
	}
	//
	keyCode = result >> 8;
	//
	return ( result & 0xFF ) == 0;
}


// ??????? 3 ???????? ?????
Dword kos_GetSystemClock()
{
//	Dword result;

	//
	__asm{
		mov eax, 3
		int 0x40
//		mov result, eax
	}
	//
//	return result;
}


// ??????? 4
void kos_WriteTextToWindow(
	Word x,
	Word y,
	Byte fontType,
	Dword textColour,
	char *textPtr,
	Dword textLen
	)
{
	Dword arg1, arg2;

	//
	arg1 = ( x << 16 ) | y;
	arg2 = ( fontType << 24 ) | textColour;
	//
	__asm{
		mov eax, 4
		mov ebx, arg1
		mov ecx, arg2
		mov edx, textPtr
		mov esi, textLen
		int 0x40
	}
}


// ??????? 5 ?????, ? ????? ????? ???????
void kos_Pause( Dword value )
{
	//
	__asm{
		mov eax, 5
		mov ebx, value
		int 0x40
	}
}


// ??????? 7 ?????????? ???????????
void kos_PutImage( RGB * imagePtr, Word sizeX, Word sizeY, Word x, Word y )
{
	Dword arg1, arg2;

	//
	arg1 = ( sizeX << 16 ) | sizeY;
	arg2 = ( x << 16 ) | y;
	//
	__asm{
		mov eax, 7
		mov ebx, imagePtr
		mov ecx, arg1
		mov edx, arg2
		int 0x40
	}
}



// ??????? 8 ?????????? ??????
void kos_DefineButton( Word x, Word y, Word sizeX, Word sizeY, Dword buttonID, Dword colour )
{
	Dword arg1, arg2;

	//
	arg1 = ( x << 16 ) | sizeX;
	arg2 = ( y << 16 ) | sizeY;
	//
	__asm{
		mov eax, 8
		mov ebx, arg1
		mov ecx, arg2
		mov edx, buttonID
		mov esi, colour
		int 0x40
	}
}


// ??????? 9 - ?????????? ? ????????
Dword kos_ProcessInfo( sProcessInfo *targetPtr, Dword processID )
{
//	Dword result;

	//
	__asm{
		mov eax, 9
		mov ebx, targetPtr
		mov ecx, processID
		int 0x40
//		mov result, eax
	}
	//
//	return result;
}


// ??????? 10
Dword kos_WaitForEvent()
{
//	Dword result;

	__asm{
		mov eax, 10
		int 0x40
//		mov result, eax
	}
	
//	return result;
}


// ??????? 11
Dword kos_CheckForEvent()
{
//	Dword result;

	__asm{
		mov eax, 11
		int 0x40
//		mov result, eax
	}
	
//	return result;
}


// ??????? 12
void kos_WindowRedrawStatus( Dword status )
{
	__asm{
		mov eax, 12
		mov ebx, status
		int 0x40
	}
}


// ??????? 13 ?????????? ??????
void kos_DrawBar( Word x, Word y, Word sizeX, Word sizeY, Dword colour )
{
	Dword arg1, arg2;

	//
	arg1 = ( x << 16 ) | sizeX;
	arg2 = ( y << 16 ) | sizeY;
	//
	__asm{
		mov eax, 13
		mov ebx, arg1
		mov ecx, arg2
		mov edx, colour
		int 0x40
	}
}


// ??????? 17
bool kos_GetButtonID( Dword &buttonID )
{
	Dword result;

	//
	__asm{
		mov eax, 17
		int 0x40
		mov result, eax
	}
	//
	buttonID = result >> 8;
	//
	return (result & 0xFF) == 0;
}


// ??????? 23
Dword kos_WaitForEvent( Dword timeOut )
{
//	Dword result;

	__asm{
		mov eax, 23
		mov ebx, timeOut
		int 0x40
//		mov result, eax
	}
	
//	return result;
}


// ????????? ?????????? ? ????????? "????" ??????? 37
void kos_GetMouseState( Dword & buttons, int & cursorX, int & cursorY )
{
	Dword mB;
	Word curX;
	Word curY;
	sProcessInfo sPI;

	//
	__asm{
		mov		eax, 37
		mov		ebx, 0
		int		0x40
		mov		curY, ax
		shr		eax, 16
		mov		curX, ax
		mov		eax, 37
		mov		ebx, 2
		int		0x40
		mov		mB, eax
	}
	//
	kos_ProcessInfo( &sPI );
	//
	buttons = mB;
	cursorX = curX - sPI.processInfo.x_start;
	cursorY = curY - sPI.processInfo.y_start;
}


// ??????? 40 ?????????? ????? ???????
void kos_SetMaskForEvents( Dword mask )
{
	//
	__asm{
		mov eax, 40
		mov ebx, mask
		int 0x40
	}
}


// ??????? 47 ??????? ? ???? ?????????? ?????
void kos_DisplayNumberToWindow(
   Dword value,
   Dword digitsNum,
   Word x,
   Word y,
   Dword colour,
   eNumberBase nBase,
   bool valueIsPointer
   )
{
	Dword arg1, arg2;

	//
	arg1 = ( valueIsPointer ? 1 : 0 ) |
		( ((Byte)nBase) << 8 ) |
		( ( digitsNum & 0x1F ) << 16 );
	arg2 = ( x << 16 ) | y;
	//
	__asm{
		mov eax, 47
		mov ebx, arg1
		mov ecx, value
		mov edx, arg2
		mov esi, colour
		int 0x40
	}
}


// 48, 4 -- get skin height
Dword kos_GetSkinHeight()
{
	__asm{
		mov eax, 48
		mov ebx, 4
		int 0x40
	}
}


// ??????? 70 ?????? ? ???????? ???????
Dword kos_FileSystemAccess( kosFileInfo *fileInfo )
{
//	Dword result;

	//
	__asm{
		mov eax, 70
		mov ebx, fileInfo
		int 0x40
//		mov result, eax
	}
	//
//	return result;
}


// ??????? 63 ????? ??????? ? ???? ???????
void kos_DebugOutChar( char ccc )
{
	//
	__asm{
		mov eax, 63
		mov ebx, 1
		mov cl, ccc
		int 0x40
	}
}


// ??????? 66 ????? ????????? ?????? ?? ??????????
void kos_SetKeyboardDataMode( Dword mode )
{
	//
	__asm{
		mov eax, 66
		mov ebx, 1
		mov ecx, mode
		int 0x40
	}
}


// ????? ?????? ? ???? ???????
void rtlDebugOutString( char *str )
{
	//
	for ( ; str[0] != 0; str++ )
	{
		kos_DebugOutChar( str[0] );
	}
	//
	kos_DebugOutChar( 13 );
	kos_DebugOutChar( 10 );
}


// ??????? 64 ????????? ?????????? ??????, ?????????? ??? ?????????
bool kos_ApplicationMemoryResize( Dword targetSize )
{
	Dword result;

	//
	__asm{
		mov eax, 64
		mov ebx, 1
		mov ecx, targetSize
		int 0x40
		mov result, eax
	}
	//
	return result == 0;
}


// ??????? 67 ???????? ????????? ????, ???????? == -1 ?? ????????
void kos_ChangeWindow( Dword x, Dword y, Dword sizeX, Dword sizeY )
{
	//
	__asm{
		mov eax, 67
		mov ebx, x
		mov ecx, y
		mov edx, sizeX
		mov esi, sizeY
		int 0x40
	}
}


// 71,1 set window caption
void kos_SetWindowCaption(char *caption)
{
	__asm{
		mov eax, 71
		mov ebx, 1
		mov ecx, caption
		int 0x40
	}
}


// ????? ???????????? ??????
int __cdecl _purecall()
{
	rtlDebugOutString( pureCallMessage );
	kos_ExitApp();
	return 0;
}


// ????? ??????????? ???????????????
// ?????? ????????????? ?????????? ????????? ?????
#pragma section(".CRT$XCA",long,read,write)
#pragma section(".CRT$XCZ",long,read,write)
typedef void (__cdecl *_PVFV)(void);
__declspec(allocate(".CRT$XCA"))  _PVFV __xc_a[1] = { NULL };
__declspec(allocate(".CRT$XCZ"))  _PVFV __xc_z[1] = { NULL };
//
#pragma comment(linker, "/merge:.CRT=.rdata")
//
void crtStartUp()
{
#ifdef AUTOBUILD
// linker will try to remove unused variables; force header to be included
	header.header;
#endif
	// ???????? ?????????????? ?? ??????, NULL'? ??????????
	for ( _PVFV *pbegin = __xc_a; pbegin < __xc_z; pbegin++ )
	{
		//
		if ( *pbegin != NULL )
			(**pbegin)();
	}
	// ?????????????? ????????? ????????? ?????
	rtlSrand( kos_GetSystemClock() );
#ifndef AUTOBUILD
	// ???? ? ????? ????????
	kosExePath = *((char **)0x20);
#endif
	// ????? ??????? ??????? ??????????
	kos_Main();
	// ?????
	kos_ExitApp();
}


