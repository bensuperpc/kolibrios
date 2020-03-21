/*
 * Author Pavel Iakovlev
*/

#define MEMSIZE 4096*10

#include "../lib/io.h"
#include "../lib/obj/console.h"
#include "../lib/array.h"

byte initConsole = 0;
Dictionary functions = {0};
Dictionary variables = {0};

#include "stdcall.h"

#define bufferSize 10000;
#define memoryBrainfuck 30000*4
#define memoryByteBF 1
#define stackBrainFuck 4*1024

#define TStr 1
#define TInt 2
#define TSym 3
#define TBol 4

#define TLen 4*5

dword buffer = 0;
word bufferSymbol = 0;
dword memory = 0;

dword stack = 0;
dword code = 0;
byte tempBuffer[100] = {0};

void consoleInit()
{
	IF(!initConsole) 
	{
		load_dll(libConsole, #con_init, 0);
		con_init stdcall (-1, -1, -1, -1, "Lisp interpreter");
		initConsole = 0xFF;
	}
}

dword evalLisp()
{
	byte s = 0;
	byte args = 0;
	dword pos = 0;
	dword name = 0;
	dword tmp = 0;
	dword tmp2 = 0;
	dword dataArgs = 0;
	dword posArgs = 0;
	dword ret = 0;
	dword p = 0;
	dword i = 0;
	dword ii = 0;
	dataArgs = malloc(16*4);
	posArgs = dataArgs;

	loop()
	{
		s = DSBYTE[code];
		while (s == ' ') || (s == 9) || (s == 10) || (s == 13)
		{
			code++;
			s = DSBYTE[code];
		}
		if (!s) return 0;
		if (s==')') 
		{
			code++;
			args--;
			ret = StdCall(args, name, dataArgs);
			free(name);
			//free(dataArgs);
			return ret;
		}
		if(s == '(') 
		{
			code++;
			DSDWORD[posArgs] = evalLisp();
			args++;
			posArgs += 4;
			continue;
		}
		else if (!args) 
		{
			if (s != ')') // name function
			{
				name = malloc(100);
				pos = name;
				while (s) && (s != ' ') && (s != ')')
				{
					DSBYTE[pos] = s;
					pos++;
					code++;
					s = DSBYTE[code];
				}	
				DSBYTE[pos] = 0;
				args++;
				continue;
			}
		}
		else
		{
			if (s >= '0') && (s <= '9')
			{
				tmp = 0;
				while (s >= '0') && (s <= '9')
				{
					tmp *= 10;
					tmp += s-'0';
					code++;
					s = DSBYTE[code];
				}
				args++;
				EDX = malloc(TLen);
				DSDWORD[EDX] = tmp;
				DSDWORD[EDX+4] = TInt;
				DSDWORD[posArgs] = EDX;
				posArgs += 4;
				continue;
			}
			else if (s == '"')
			{
				i = 1;
				tmp = malloc(1<<i);
				p = tmp;
				code++;
				s = DSBYTE[code];
				ii = 0;
				while (s != '"') && (s)
				{
					ii++;
					if (1<<i < ii)
					{
						i++;
						tmp2 = p-tmp;
						tmp = realloc(tmp, 1<<i);
						p = tmp+tmp2;
					}
					DSBYTE[p] = s;
					p++;
					
					code++;
					s = DSBYTE[code];
				}
				DSBYTE[p] = 0;
				EDX = malloc(TLen);
				DSDWORD[EDX] = tmp;
				DSDWORD[EDX+4] = TStr;
				DSDWORD[EDX+8] = p-tmp;
				DSDWORD[posArgs] = EDX;
				posArgs += 4;
				code++;
				args++;
				continue;
			}
			else
			{
				tmp = malloc(20);
				p = tmp;
				while (s) && (s != ')') && (s != '(') && (s != ' ') && (s != 10) && (s != 13)
				{
					DSBYTE[p] = s;
					p++;
					code++;
					s = DSBYTE[code];
				}
				DSBYTE[p] = 0;
				args++;
				EDX = malloc(TLen);
				DSDWORD[EDX] = tmp;
				DSDWORD[EDX+4] = TSym;
				DSDWORD[posArgs] = EDX;
				posArgs += 4;
				continue;
			}
			
			DSDWORD[posArgs] = tmp;
			posArgs += 4;
		}
		code++;
		args++;
	}
	args--;
	ret = StdCall(args, name, dataArgs);
	free(name);
	//free(dataArgs);
	return ret;
}

void main()
{
	dword brainFuckCode = 0;
	word maxLoop = 1000;
	dword txt = "(set name (input \"Enter you name: \"))(print \"You name \" (get name))";
	
	buffer = malloc(bufferSize);
	memory = malloc(memoryBrainfuck);
	stack = malloc(stackBrainFuck);
	
	Init();
	
	IF(DSBYTE[I_Param])
	{
		IF(io.read(I_Param))
		{
			code = EAX;
			evalLisp();
		}
	}
	else 
	{
		consoleInit();
		con_printf stdcall ("Lisp interpreter v1.5\r\n");
		while(maxLoop)
		{
			con_printf stdcall ("\r\n$ ");
			con_gets stdcall(buffer, bufferSize);
			code = EAX;
			//code = txt;
			con_printf stdcall ("Output: ");
			evalLisp();
			maxLoop--;
		}
	}
	
	IF(initConsole) con_exit stdcall (0);
	ExitProcess();
}

