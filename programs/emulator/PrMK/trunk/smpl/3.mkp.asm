;???? ?ணࠬ?? ??? PrMK. ????????஢??? ? ??????? FASM.

;???? "??ᠤ?? ?? ????"
;??९???⠭? ? ????? "5 ????஢ ? ???ப?????????஬" ?.?.???????, ?.?.??????. ??᪢?. "??????? ? ??????⨪?", 1988
;Andriy Slezenko, student@uninet.kiev.ua

;1. ?????? ?ணࠬ??.
;2. ??३?? ? ०?? ????᫥???.
;3. ?/? ?/? "0".
;4. h0 (??砫쭠? ?????? ??ࠡ??) ?? v0 (??砫쭠? ᪮?????) ??.
;   m0 (??室??? ????? ⮯????) ?? g (?᪮७?? ᢮??????? ??????? ??? ???????) ?9.
;5. ?/? "h" XY "v" ??? "m" ??4 "t" (⥪?騥 ???祭??).
;6. S (???室 ⮯???? ? ᫥?????? ᥪ㭤?).
;7. ??३?? ? ?.5 ? ?த?????? ?? ??ᠤ?? ??ࠡ??.

;???? ???? - ??ᠤ??? ??ࠡ?? ?? ????.
;	???????? ? ??? ? ???? ⠪?? ⠪??. ?????? ??室???? ?? ??????ﭨ? N ???஢ ?? ????, ᪮????? ?? V ???஢ ? ᥪ㭤?, ????? ⮯???? ? ????? - S ⮭?. ????稭? ???, ????筮, ???????? ?????????. ????? ࠪ??? ????????? ? ?㭥 ?, ???????? ⮯???? ? ?????⥫?, ?? ??稭??? ??ମ????. ?????? ᥪ㭤? ?? ᢥ⮢?? ⠡?? ? ????????᪮? ?㡪? ??????? ????ଠ??? ? ?????? ??ࠡ?? ??? ?????孮???? ???????, ᪮????? ??? ? ?????? ⮯????. ??? ??? ???????஬ ??ࠡ?? ?㤥?? ??, ?? ??????, ??? ??। ⥬, ??? ??ᬮ????? ?? ⠡??, ?㦭? ?????? ⠪?? ???????: XY ?????뢠?? ᪮?????, ??? ???? ????⮪ ⮯???? ? ????, ??4 - ?६? ? ??砫? ??ମ?????. ??? ??ᠥ??? ?????? ࠪ??? ??? ?????⮩, ?? ??? ?㤥? ?????????? ?? ??襬 ⠡?? ?ࠧ? ??? ??⠭???? ???????????? ??᫥ ?஢?????? ???।???? ???????.
;	?????? ????⭠ - ??ᠤ??? ??ࠡ?? ⠪, ?⮡? ? ?????? ??ᠤ?? ᪮????? ??? ?뫠 ?????? ? ????, ⮯???? ??? ?⮬ ????⥫쭮 ?????室????? ????????, ?? ? ?஢???? ??ᠤ?? ??????॥ - ????祬 ????? ?????? 童??? ???????, ???? ??ॣ?㧪? ??? ??ᠤ?? ?? ?????쪨?. ??, ??? ??ᠥ??? ⮯????, ???????⥫쭮? ?᫮??? - ?᫨ ??? ??⠭???? ?????? 95 ⮭?, ?? ???????? ????? ?? ????? ?⠭?? ???????????.

;????஫???? ?ਬ??:
;????? ?????? ?㤥? 1000 ???஢, ᪮????? - 150 ?/?, ????? ⮯???? - 500 ⮭?. ??, ??? ?㦭? ?????? ? ???⮢?? ????????? ?᪮७?? ᢮??????? ??????? ?? ?㭥 - 1,62 ?/?^2. ?????? ??? ??᫠ ᮮ⢥??⢥??? ? ॣ????? ??, ??, ?? ? ?9.
;???????? ?/? ?/? ? ?? ?????????? ?????? ??砫쭠? ??????. ?????? ? ?????⥫? ?????? ?????? ⮯???? - 20 ?/?. ??१ 10 ᥪ㭤 ?? ?????????? ?????? ??????, ?? ????ன ⥯??? ??室???? ??ࠡ??, 856.4. ??? ?⮫쪮 ?? ⮯???? - 20 ?/? ? ?????? ?⠫? 719.4. ??? ?????? ???????? - 25 ?/?. ?????? 591 ????.
;?஢?ਬ ??⠫???? ??ࠬ????: ᪮????? (??????? XY) - 128,4 ?/?, ????? ⮯???? (???) - 435 ⮭?, ?६? ? ???? (??4) - 3 ?.
;??᫥ ????? ??? ࠧ? ?????? ?? 30 ⮭? ????祣?, ?????? ??ࠡ?? ᭨?????? ?? 366,9 ?.
;??᫥ ???쭥?襣? ????? ??? ࠧ? ?????? ?? 35 ⮭? ????祣?, ⠡?? ???????? ?????? - 194,1 ?, ᪮????? - 76,6, ⮯???? - 305, ?६? - 7.
;????? ????? ???⥪?? ⠪:

;	???室 ⮯????		??????
;		40		130.5
;		45		85.6
;		40		57.4
;		40		46.2
;		10		38.1
;		10		33.2
;		0		26.6
;		0		18.4
;		5		10.9
;		7		5.1
;		8		1.6
;		10		1.2
;		5		1.5
;			(????? ᪮????? ?????????
;			??????⥫쭮? (-0,3) ? ??ࠡ??
;			??砫 ᭮?? 㤠?????? ?? ?????孮???)
;		0		0.3
;		3		3.1111111
;????㫨?? ?????孮??? ????. ??᫥ ??ᠤ?? ???᭨???? (???), ??? ⮯???? ??⠫??? 82 ⮭??, ? ?६?, ?????祭??? ?? ???? ??????? (??4), 21 ᥪ㭤?.
;? ?⮣? ?? ?????, ??? ᥫ? ?殮??????, ?? ??ࠡ?? 楫, ? ???????? ??? ? ???浪?. ??? ⮫쪮 ⮯???? ???????室?????, ?? ???????? ???? ?? 墠???. ?ਤ???? ?? ?㭥 ????? ??ࠡ??-ᯠ??⥫? ? ?????????, ??? ??? ????⠭ ??ࠢ???? ? ??ᠤ??? ????? ???.
;?? ?? ????砩????. ??? ??ࢮ?? ࠧ? ?????? ?ਫ?筮.

;?ਬ?砭??:
;1. ??᫠ ? ????窠? - ??? ??᫠, ??ᢥ稢????? ?? ??࠭?.
;2. ?믮????? ?.4 ?? ??易⥫쭮. ?? 㬮?砭?? ?।??????????: h=1000 ?, v0=150 ?/?, m0+500 ?, g=1,62 ?/?^2 ??? ????.
;3. ????? ᮮ?饭?? ? ?????⢥ ??ᠤ?? ??????஢?? ????.

;1,1111111 - ?????쭠? ??ᠤ??;
;2,1111111 - ??ଠ?쭠? ??ᠤ??;
;3,1111111 - ?殮??? ??ᠤ??;
;4,1111111 - ??ᠤ?? ? ????? ???࠭??묨 ???०????ﬨ;
;5,1111111 - ???०?????, ????祭??? ??? ??ᠤ??, ?????࠭???. ???????? ????? ? ????? ???? ?? 	    ????????;
;6,1111111 - ?祭? ᨫ???? ???०?????;
;7,1111111 - ࠧ??ଥ⨧???? ?⤥????? ?????? ? १??????? ᨫ쭮?? 㤠??. ????? 童??? 	    ??????? ??? ??஧??;
;8,1111111 - ?????? ???????;
;9,1111111 - ????? ??ࠡ??.


db "<????>"

	 ;????? ᫥???? ???祭?? ॣ???஢ ?⥪? (⮫쪮 qword)

		 ;R.X1
		  dq 0.0
		 ;R.X
		  dq 0.0
		 ;R.Y
		  dq 0.0
		 ;R.Z
		  dq 0.0
		 ;R.T
		  dq 0.0

db "<????>"

		;????????: (? R.0 ?? R.E)

		  times 15 dq 0.0

db "<????>"
		;?ணࠬ?? ? ??????? ??

db 0x0D,0x44,0x0C,0x03,0x4A,0x48,0x02,0x13,0x4C,0x01
db 0x05,0x00,0x4B,0x01,0x0A,0x06,0x02,0x49,0x0D,0x50
db 0x6B,0x6A,0x50,0x40,0xD4,0x14,0x6C,0x14,0x11,0x5C
db  36 ,0x0D,0x40,0x6C,0x51, 37 ,0x4C,0x68,0x13,0x02
db 0x10,0x60,0x14,0x13,0x69,0x14,0x11,0x6B,0x10,0x4B
db 0x6A,0x14,0x11,0x4A,0x5C, 20 ,0x0B,0x6B,0x22,0x21
db 0x10,0x01,0x10,0x4D,0xDD,0x6D,0x09,0x23,0x10,0x50