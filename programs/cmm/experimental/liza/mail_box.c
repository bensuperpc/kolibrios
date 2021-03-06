//Leency & SoUrcerer, LGPL
//Hidnplayer

#define LIST_INFO_H 59
int status_bar_h = 15;

scroll_bar scroll1 = { 17,200,210, LIST_INFO_H-3,18,0,115,15,0,0xCCCccc,0xD2CED0,0x555555,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1};
scroll_bar scroll_wv;

char *listbuffer;
char *listpointer;

char *mailstart;
char *mailend;
int mailsize;

int cur_charset;

llist list;

enum {
	GET_MAIL = 20,
	SEND_MAIL,
	DELETE_LETTER,
	SAVE_LETTER,
	STOP_LOADING,
	EXIT_MAIL,
	CHANGE_CHARSET,
	CLOSE_CHANGE_CHARSET
};

void MailBoxNetworkProcess() {
	int load_persent;
	if (aim) switch(aim)
	{
		case SEND_NSTAT:
				SetMailBoxStatus(NULL, "Counting mail, awaiting answer...");
				request_len = GetRequest("STAT", NULL);
				Send(socketnum, #request, request_len, 0);
				if (EAX == 0xffffffff) { debugln("Error sending STAT. Retry..."w); break;}
				aim = GET_ANSWER_NSTAT;
				break;

		case GET_ANSWER_NSTAT:
				ticks = Receive(socketnum, #immbuffer, BUFFERSIZE, 0);
				if ((ticks == 0xffffff) || (ticks < 2)) break;

				if (immbuffer[ticks-2]=='\n')
				{
					debugln(#immbuffer);
					if (strstr(#immbuffer,"+OK"))
					{
						strcpyb(#immbuffer, #param, "+OK ", " ");
						mail_list.count = atoi(#param);
						free(listbuffer);
						listbuffer = mem_Alloc(30*mail_list.count); //24* original
						listpointer = listbuffer;
						aim = SEND_NLIST;
						debugln("Receiving mail list...");
					}
					else
					{
						StopConnect("Sorry, can't recieve your mail");
					}
				}
				break;

		case SEND_NLIST:
				WriteText(5, Form.cheight-11, 0x80, sc.work_text, "Send LIST, awaiting answer...");
				request_len = GetRequest("LIST", NULL);
				Send(socketnum, #request, request_len, 0);
				if (EAX == 0xffffffff) {debugln("Error while sending LIST. Retry..."); break;}
				else aim = GET_ANSWER_NLIST;
				break;

		case GET_ANSWER_NLIST:
				ticks = Receive(socketnum, listpointer, listbuffer + 30*mail_list.count - listpointer, MSG_DONTWAIT);
				if (ticks == 0xffffffff) break;
				listpointer = listpointer + ticks;

				if (listpointer - listbuffer < 5) break;
				if (strncmp(listpointer-5,"\n.\n",5)==0)  // note that c-- assembles "\n.\n" to 0x0d, 0x0a, 0x2e, 0x0d, 0x0a
				{
					aim = SEND_RETR;
					debugln("goto SEND_RETR");
					DrawMailBox();

					*listpointer='\0';
					atr.CreateArray();
					atr.SetSizes();
				}
				break;

		case SEND_RETR:
				from = to = date = subj = cur_charset = NULL;
				DrawMailBox();
				request_len = GetRequest("RETR", itoa(mail_list.cur_y+1));
				if (Send(socketnum, #request, request_len, 0) == 0xffffffff)
				{
					StopConnect("Error while trying to get letter from server");
					break;
				}
				mailsize = atr.GetSize(mail_list.cur_y+1) + 1024;
				free(mailstart);
				mailstart = malloc(mailsize);
				mailend = mailstart;
				if (!mailstart)
				{
					debugln("alloc error!");
					aim=NULL;
					break;
				}
				debugval("mailsize", mailsize);
				aim = GET_ANSWER_RETR;
				debugln("goto GET_ANSWER_RETR");
				break;

		case GET_ANSWER_RETR:
				debugval("mailsize", mailsize);
				debugval("mailstart", mailstart);
				debugval("mailend", mailend);
				ticks = Receive(socketnum, mailend, mailsize + mailstart - mailend, MSG_DONTWAIT);
				if (ticks == 0xffffffff) break;
				mailend = mailend + ticks;
				if (mailsize + mailstart - mailend - 2 < 0)
				{
					debugln("Resizing buffer");
					mailsize += 4096;
					mailstart = realloc(mailstart, mailsize);
					if (!mailstart) { StopConnect("Realloc error!"); break;}
				}
				/*
				if (mailsize>9000)
				{
					load_persent = mailend - mailstart * 100 ;
					load_persent /= mailsize - 1024;
					if (load_persent != cur_st_percent) SetMailBoxStatus( load_persent , NULL);
				}
				*/
				//ParseMail();
	}
}


void MailBoxLoop() {
	int id;
	int panels_drag=0, clicked_list=0;
	dword line_col, text_col;

	mail_list.h = Form.cheight/4;
	mail_list.ClearList();
	SetMailBoxStatus( NULL , NULL);
	cur_charset = 0;
	aim = SEND_NSTAT;

	goto _MB_DRAW;

	loop()
	{
		WaitEventTimeout(2);
		switch(EAX & 0xFF)
		{
			case evMouse:
				mouse.get();

				if (!mouse.lkm) panels_drag=0;
				if (mouse.lkm) && (mouse.y>mail_list.y+mail_list.h-1) && (mouse.y<mail_list.y+mail_list.h+6)
				&& (!scroll1.delta2) && (!scroll_wv.delta2) panels_drag = 1;
				if (panels_drag)
				{
					if (mouse.y<mail_list.y+60) || (mouse.y>Form.cheight-60-status_bar_h-LIST_INFO_H) break;
					mail_list.h = mouse.y - mail_list.y-2;
					DrawMailBox();
					break;
				}

				//links.hover(mouse.x, mouse.y);

				if (!mail_list.count) break;
				if (!panels_drag) { scrollbar_v_mouse (#scroll1); scrollbar_v_mouse (#scroll_wv); }

				if (mail_list.first <> scroll1.position)
				{
					mail_list.first = scroll1.position;
					DrawMailList();
					break;
				};
				if (list.first <> scroll_wv.position)
				{
					list.first = scroll_wv.position;
					DrawLetter();
					break;
				};

				if (mail_list.y+mail_list.h + 10 > mouse.y)
				{
					if (mail_list.MouseScroll(mouse.vert)) DrawMailList();
				}
				else
				{
					if (list.MouseScroll(mouse.vert)) DrawLetter();
				}
				if (mouse.lkm) && (mail_list.MouseOver(mouse.x, mouse.y)) && (!clicked_list) clicked_list=1;
				if (!mouse.lkm) && (clicked_list) if (mail_list.ProcessMouse(mouse.x, mouse.y))
				{
					clicked_list = 0;
					if (aim) break;
					DrawMailList();
					aim = SEND_RETR;
				}

				break;
			case evButton:
				id = GetButtonID();
				if (id==1) SaveAndExit();
				if (id==GET_MAIL) aim = SEND_NSTAT;
				if (id==SAVE_LETTER)
				{
					if (!mailstart) break;
					CreateFile(strlen(mailstart), mailstart, "mail.txt");
					pause(10);
					RunProgram("/sys/tinypad", "mail.txt");
				}
				if (id==STOP_LOADING)
				{
					StopLoading();
					DrawStatusBar();
					DrawMailList();
				}
				if (id==EXIT_MAIL)
				{
					StopLoading();
					Close(socketnum);
					LoginBoxLoop();
				}
				if (id==CHANGE_CHARSET)
				{
					DefineButton(0,0,Form.cwidth,Form.cheight, CLOSE_CHANGE_CHARSET+BT_HIDE+BT_NOFRAME);
					DrawRectangle(Form.cwidth-100, Form.cheight-status_bar_h- 70, 70, 82, sc.line);
					DrawRectangle3D(Form.cwidth-99, Form.cheight-status_bar_h- 69, 68, 80, 0xFFFfff, sc.work);
					for (id=0; id<5; id++)
					{
						if (cur_charset==id+1) { line_col=sc.button; text_col=sc.button_text; }
						else { line_col=sc.work; text_col=sc.work_text; }
						DrawBar(Form.cwidth-98, id*16+Form.cheight-status_bar_h- 68, 67, 16, line_col);
						DrawCaptButton(Form.cwidth-100, id*16+Form.cheight-status_bar_h- 68, 70,16, 10+id+BT_HIDE,
						0, text_col, charsets[id+1]);
					}
				}
				if (id==CLOSE_CHANGE_CHARSET) goto _MB_DRAW;

				break;
			case evKey:
				GetKeys();

				if (!aim) && (mail_list.ProcessKey(key_scancode))
				{
					DrawMailList();
					aim = SEND_RETR;
				}

				break;
			case evReDraw: _MB_DRAW:
				if !(DefineWindow(MAILBOX_HEADER)) break;
				scroll1.bckg_col = scroll_wv.bckg_col = 0xBBBbbb;
				scroll1.frnt_col = scroll_wv.frnt_col = sc.work;
				scroll1.line_col = scroll_wv.line_col = sc.line;
				DrawToolbar();
				DrawMailBox();

				break;
			default:
				MailBoxNetworkProcess();
		}
	}
}


void DrawMailBox()
{
	DrawMailList();
	DrawLetterInfo();
	DrawLetter();
	DrawStatusBar();
}


void DrawToolbar() {
	#define BUT_Y 7
	#define BUT_H 22
	#define BUT_W 74
	int toolbar_w = BUT_Y + BUT_H + BUT_Y + 3;
	mail_list.SetSizes(0, toolbar_w, Form.cwidth - scroll1.size_x - 1, mail_list.h, 18);

	DrawBar(0,0, Form.cwidth,toolbar_w-3, sc.work);
	DrawCaptButton(10                    , BUT_Y, BUT_W, BUT_H, GET_MAIL,    sc.button, sc.button_text,"Get mail");
	DrawCaptButton(BUT_W+ 20, BUT_Y, BUT_W+10, BUT_H, SAVE_LETTER, sc.button, sc.button_text,"Save letter");
	DrawCaptButton(Form.cwidth-BUT_W - 10, BUT_Y, BUT_W, BUT_H, EXIT_MAIL,   sc.button, sc.button_text,"< Exit");

	DrawBar(0, mail_list.y-3, mail_list.w,1, sc.line);
	DrawBar(0, mail_list.y-2, mail_list.w,1, 0xdfdfdf);
	DrawBar(0, mail_list.y-1, mail_list.w,1, 0xf0f0f0);
}

void DrawMailList() {
	int i, on_y, on_x, direction;
	dword sel_col;
	mail_list.visible = mail_list.h / mail_list.item_h;

	for (i=30; i<150; i++) DeleteButton(i);
	for (i=0; (i<mail_list.visible) && (i+mail_list.first<mail_list.count); i++)
	{
		on_y = i*mail_list.item_h + mail_list.y;
		if (mail_list.cur_y==mail_list.first+i) sel_col=0xEEEeee; else sel_col=0xFFFfff;
		DrawBar(0, on_y, mail_list.w, mail_list.item_h-1, sel_col);
		direction = atr.GetDirection(i+mail_list.first+1);
		on_x = strlen(itoa(i+mail_list.first+1))*6;
		letter_icons_pal[0]=sel_col;
		PutPaletteImage(sizeof(letter_icons)/3*direction + #letter_icons, 18,12, on_x+18,
			mail_list.item_h-12/2+ on_y, 8, #letter_icons_pal);
		WriteText(on_x + 42, on_y+5, 0x80, 0, atr.GetSubject(i+mail_list.first+1));
		DrawBar(0, on_y + mail_list.item_h-1, mail_list.w, 1, 0xCCCccc);
		WriteText(10, on_y+5, 0x80, 0, itoa(i+mail_list.first+1));
		WriteText(mail_list.w - 40, on_y+5, 0x80, 0, ConvertSize(atr.GetSize(i+mail_list.first+1)));
	}
	DrawBar(0, i*mail_list.item_h + mail_list.y, mail_list.w, -i*mail_list.item_h+mail_list.h, 0xFFFfff);
	DrawScroller1();
}

void DrawLetterInfo() {
	int lt_y = mail_list.y+mail_list.h;
	DrawBar(0, lt_y, mail_list.w, 1, sc.line);
	DrawBar(0, lt_y+1, Form.cwidth, 1, LBUMP);
	DrawBar(0, lt_y+2, Form.cwidth, LIST_INFO_H-4, sc.work);
	WriteText(mail_list.w-30/2, lt_y, 0x80, 0x888888, "= = =");
	WriteText(mail_list.w-30/2, lt_y+1, 0x80, 0xEeeeee, "= = =");
	DrawBar(0, lt_y+LIST_INFO_H-2, Form.cwidth, 1, sc.line); //bottom
	DrawBar(0, lt_y+LIST_INFO_H-1, Form.cwidth, 1, 0xdfdfdf);
	DrawBar(0, lt_y+LIST_INFO_H  , Form.cwidth, 1, 0xf0f0f0);
	WriteTextB(10, lt_y+8 , 0x80, sc.work_text, "From:");
	WriteText (45, lt_y+8 , 0x80, sc.work_text, #from);
	WriteTextB(10, lt_y+20, 0x80, sc.work_text, "To:");
	WriteText (45, lt_y+20, 0x80, sc.work_text, #to);
	WriteTextB(10, lt_y+32, 0x80, sc.work_text, "Date:");
	WriteText (45, lt_y+32, 0x80, sc.work_text, #date);
	WriteTextB(10, lt_y+44, 0x80, sc.work_text, "Subj:");
	WriteText (45, lt_y+44, 0x80, sc.work_text, #subj);
}


void DrawLetter() {
	//DrawTextLines();
	DrawRectangle(scroll_wv.start_x, scroll_wv.start_y, scroll_wv.size_x, scroll_wv.size_y-1, 0xFFFfff);
}

void DrawScroller1() {
	scroll1.max_area = mail_list.count;
	scroll1.cur_area = mail_list.visible;
	scroll1.position = mail_list.first;
	scroll1.all_redraw=1;
	scroll1.start_x = mail_list.x + mail_list.w;
	scroll1.start_y = mail_list.y - 3;
	scroll1.size_y = mail_list.h + 4;
	scrollbar_v_draw(#scroll1);
}



void DrawStatusBar() {
	int st_y = Form.cheight -status_bar_h;
	DrawBar(0, st_y, Form.cwidth, status_bar_h, sc.work);
	if (aim) {
		SetMailBoxStatus(cur_st_percent, cur_st_text);
		DrawCaptButton(240, st_y+1, 36, status_bar_h-3, STOP_LOADING, sc.button, sc.button_text,"Stop");
	}
	DrawCaptButton(Form.cwidth - 100, st_y+1, 70, status_bar_h-2, CHANGE_CHARSET+BT_HIDE,
		sc.work, sc.work_text,cur_charset*10+#charsets);
}



void SetMailBoxStatus(dword percent1, text1) {
	DrawProgressBar(3, Form.cheight -status_bar_h + 1, 220, 12, sc.work, 0xC3C3C3, 0x54B1D6, sc.work_text, percent1);
	WriteText(3, Form.cheight -status_bar_h + 1, 0x80, sc.work_text, text1);
	cur_st_percent = percent1;
	cur_st_text = text1;
}


void StopLoading() {
	aim = NULL;
	mailstart = free(mailstart);
	to = from = date = subj = cur_charset = NULL;
}

int GetLetterSize_(int number) {
   char search_num[24];
   char mailsize1[24];
   strcpy(#search_num, "\x0a");       // 0x0d, 0x0a
   itoa_(#search_num+1, number);
   chrcat(#search_num, ' ');
   strcpyb(listbuffer, #mailsize1, #search_num, "\x0d");
   return atoi(#mailsize1);
}


/*
void EventClickLink() {
	RunProgram("/sys/network/WebView", links.active_url);
}
*/