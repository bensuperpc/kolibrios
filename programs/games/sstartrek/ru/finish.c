#include "sst.h"
#include <string.h>
#ifndef KOS32
#include <time.h>
#else
#include <kolibrisys.h>
#endif

void dstrct() {
	/* Finish with a BANG! */
	chew();
	if (damage[DCOMPTR] != 0.0) {
		/* prout("Computer damaged; cannot execute destruct sequence."); */
		prout("ฎฌฏ์๎โฅเ ฏฎขเฅฆคฅญ; ฅขฎงฌฎฆญฎ ง ฏใแโจโ์ แ ฌฎใญจ็โฎฆฅญจฅ.");
		return;
	}
	skip(1);
/* 	prouts("---WORKING---"); skip(1);
	prout("SELF-DESTRUCT-SEQUENCE-ACTIVATED");
 */	prouts("------"); skip(1);
	prout("--");
	prouts("   10"); skip(1);
	prouts("       9"); skip(1);
	prouts("          8"); skip(1);
	prouts("             7"); skip(1);
	prouts("                6"); skip(1);
/* 	prout("ENTER-CORRECT-PASSWORD-TO-CONTINUE-");
	prout("SELF-DESTRUCT-SEQUENCE-OTHERWISE-");
	prout("SELF-DESTRUCT-SEQUENCE-WILL-BE-ABORTED");
 */	prout("-----");
	prout("----");
	prout("---");
	scan();
	chew();
	if (strcmp(passwd, citem) != 0) {
/* 		prouts("PASSWORD-REJECTED;"); skip(1);
		prout("CONTINUITY-EFFECTED");
 */		prouts("-;"); skip(1);
		prout("-");
		skip(1);
		return;
	}
	prouts("-"); skip(1); /*PASSWORD-ACCEPTED*/
	prouts("                   5"); skip(1);
	prouts("                      4"); skip(1);
	prouts("                         3"); skip(1);
	prouts("                            2"); skip(1);
	prouts("                              1"); skip(1);
	if (Rand() < 0.15) {
		prouts("--"); /*GOODBYE-CRUEL-WORLD*/
		skip(1);
	}
	skip(2);
	kaboom();
}

void kaboom(void) {
	stars();
	if (ship==IHE) prouts("***");
	prouts("********* ญโเฎฏจ๏ ");/*Entropy of*/
	crmshp();
	prouts(" ฌ ชแจฌจงจเฎข ญ  *********");/*maximized*/
	skip(1);
	stars();
	skip(1);
	if (nenhere != 0) {
		double whammo = 25.0 * energy;
		int l=1;
		while (l <= nenhere) {
			if (kpower[l]*kdist[l] <= whammo) 
				deadkl(kx[l],ky[l], quad[kx[l]][ky[l]], kx[l], ky[l]);
			l++;
		}
	}
	finish(FDILITHIUM);
}
				

void finish(FINTYPE ifin) {
	int igotit = 0;
	alldone = 1;
	skip(3);
	/* printf("It is stardate %.1f .\n\n", d.date); */
	printf("ขฅงคญฎฅ ขเฅฌ๏ %.1f .\n\n", d.date);
	switch (ifin) {
		case FWON: // Game has been won
			if (d.nromrem != 0)
				/* printf("The remaining %d Romulans surrender to Starfleet Command.\n", */
				printf("ฎแซฅคญจฅ %d เฎฌใซ ญๆฅข แค ซจแ์ ญ  ฌจซฎแโ์ ขฅงคญฎฃฎ ฎฌ ญคฎข ญจ๏.\n",
					   d.nromrem);


/*             prout("You have smashed the Klingon invasion fleet and saved");
            prout("the Federation.");
 */            prout("๋ แซฎฌ ซจ ๅเฅกฅโ ขโฎเฆฅญจ๎ ชซจญฃฎญฎข จ แฏ แซจ ฅคฅเ ๆจ๎.");

#ifdef CAPTURE
            if (alive && brigcapacity-brigfree > 0) { // captured Klingon crew will get transfered to starbase
                kcaptured += brigcapacity-brigfree;
                /* printf("The %d captured Klingons are transferred to Star Fleet Command.\n", */
                printf(" ๅข ็ฅญญ๋ฅ ข ฏซฅญ %d ชซจญฃฎญฎข ฏฅเฅค ญ๋ ขฅงคญฎฌใ ฎฌ ญคฎข ญจ๎.\n",
                       brigcapacity-brigfree);
            }
#endif
			gamewon=1;
			if (alive) {
                double badpt;

				badpt = 5.*d.starkl + casual + 10.*d.nplankl +
						45.*nhelp+100.*d.basekl;
				if (ship == IHF) badpt += 100.0;
				else if (ship == 0) badpt += 200.0;
				if (badpt < 100.0) badpt = 0.0;	// Close enough!
				if (d.date-indate < 5.0 ||
					// killsPerDate >= RateMax
					(d.killk+d.killc+d.nsckill)/(d.date-indate) >=
					0.1*skill*(skill+1.0) + 0.1 + 0.008*badpt) {
					skip(1);
					/* prout("In fact, you have done so well that Starfleet Command"); */
					prout("๋ ญฅฏซฎๅฎ แฏเ ขจซจแ์ แ ง ค ญจฅฌ, จ ขฅงคญฎฅ ฎฌ ญคฎข ญจฅ");
					switch (skill) {
						case SNOVICE:
							/* prout("promotes you one step in rank from \"Novice\" to \"Fair\"."); */
							prout("ฏฎข๋่ ฅโ  แ ข งข ญจจ แ \"ฎขจ็ช \" คฎ \" ซ ฃจ\".");
							break;
						case SFAIR:
							/* prout("promotes you one step in rank from \"Fair\" to \"Good\"."); */
							prout("ฏฎข๋่ ฅโ  แ ข งข ญจจ แ \"กญ คฅฆจข ๎้ฅฃฎ\" คฎ \"โซจ็ญจช \".");
							break;
						case SGOOD:
							/* prout("promotes you one step in rank from \"Good\" to \"Expert\"."); */
							prout("ฏฎข๋่ ฅโ  แ ข งข ญจจ แ \"โซจ็ญจช \" คฎ \"ชแฏฅเโ \".");
							break;
						case SEXPERT:
							/* prout("promotes you to Commodore Emeritus."); */
							prout("ฏฎข๋่ ฅโ  แ ข งข ญจจ คฎ ฎฌฌ ญคฅเ  ฌฅเจโใแ.");
							skip(1);
/* 							prout("Now that you think you're really good, try playing");
							prout("the \"Emeritus\" game. It will splatter your ego.");
 */							prout("ฅฏฅเ์, ฅแซจ ๋ แ็จโ ฅโฅ แฅกฅ เฅ ซ์ญฎ ชเใโ๋ฌ, ฏฎฏเฎกใฉโฅ");
							prout("ใเฎขฅญ์ แซฎฆญฎแโจ \"ฌฅเจโใแ\". โฎ ใโจๅฎฌจเจโ ข ่ฅ ํฃฎ.");
							break;
						case SEMERITUS:
							skip(1);
/* 							prout("Computer-  ERROR-ERROR-ERROR-ERROR");
							skip(1);
							prout("  YOUR-SKILL-HAS-EXCEEDED-THE-CAPACITY-OF-THIS-PROGRAM");
							prout("  THIS-PROGRAM-MUST-SURVIVE");
							prout("  THIS-PROGRAM-MUST-SURVIVE");
							prout("  THIS-PROGRAM-MUST-SURVIVE");
							prout("  THIS-PROGRAM-MUST?- MUST ? - SUR? ? -?  VI");
							skip(1);
							prout("Now you can retire and write your own Star Trek game!");
 */							prout("ฎฌฏ์๎โฅเ-  ---");
							skip(1);
							prout("  -----");
							prout("  --");
							prout("  --");
							prout("  --");
							prout("  --?-  ? - ? ? -?  ");
							skip(1);
							prout("ฅฏฅเ์ ฌฎฆฅโฅ แฎ แฏฎชฎฉญฎฉ คใ่ฎฉ ใฉโจ ข ฎโแ ขชใ จ ญ ฏจแ โ์ แฎกแโขฅญญใ๎ จฃเใ โ เ เฅช!");
							skip(1);
							break;
					}
					if (skill > SGOOD) {
						if (thawed
#ifdef DEBUG
							&& !idebug
#endif
							)
							/* prout("You cannot get a citation, so..."); */
							prout(" แ ญฅ ง ฏจ่ใโ ข  ญญ ซ๋, โ ช ็โฎ...");
						else {
/* 							prout("Do you want your Commodore Emeritus Citation printed?");
							proutn("(You need a 132 column printer.)");
 */							prout("๋ ๅฎโจโฅ ญ ฏฅ็ โ โ์ แฏจแฎช แขฎจๅ ข๋แช ง๋ข ญจฉ, ฎฌ ญคฅเ ฌฅเจโใแ?");
							proutn("( ฌ ฏฎโเฅกใฅโแ๏ 132-แจฌขฎซ์ญ๋ฉ ฏเจญโฅเ.)");
							chew();
							if (ja()) {
								igotit = 1;
							}
						}
					}
				}
				// Only grant long life if alive (original didn't!)
				skip(1);
				/* prout("LIVE LONG AND PROSPER."); */
				prout("    .");
			}
			score(0);
			if (igotit != 0) plaque();
			return;
		case FDEPLETE: // Federation Resources Depleted
/* 			prout("Your time has run out and the Federation has been");
			prout("conquered.  Your starship is now Klingon property,");
			prout("and you are put on trial as a war criminal.  On the");
			proutn("basis of your record, you are ");
 */			prout(" ่ฅ ขเฅฌ๏ ข๋่ซฎ จ ฅคฅเ ๆจ๏ ก๋ซ  ง ๅข ็ฅญ .");
			prout(" ่ ชฎเ กซ์ โฅฏฅเ์ โเฎไฅฉ ชซจญฃฎญฎข,");
			prout("จ  แ กใคใโ แใคจโ์ ช ช ขฎฅญญฎฃฎ ฏเฅแโใฏญจช .");
			proutn(" แแฌฎโเฅข ข ่จ คฅฉแโขจ๏, ๋ ");
			if (d.remkl*3.0 > inkling) {
				prout("ฎฏเ ขค ญ๋.");/*aquitted*/
				skip(1);
				prout("    .");/*LIVE LONG AND PROSPER*/
			}
			else {
/* 				prout("found guilty and");
				prout("sentenced to death by slow torture.");
 */				prout("ฏเจงญ ญ๋ ขจญฎขญ๋ฌ");
				prout("จ ฏเจฃฎขฎเฅญ๋ ช แฌฅเโจ ฏฎค ฏ๋โช ฌจ.");
				alive = 0;
			}
			score(0);
			return;
		case FLIFESUP:
/* 			prout("Your life support reserves have run out, and");
			prout("you die of thirst, starvation, and asphyxiation.");
			prout("Your starship is a derelict in space.");
 */			prout(" ข แ ชฎญ็จซจแ์ ง ฏ แ๋ ขฎงคใๅ  จ ฏจ้จ จ");
			prout("ข๋ แ ชฎฌ ญคฎฉ ใฌฅเซจ ฎโ ฃฎซฎค  จ ฆ ฆค๋, ง ค๋ๅ ๏แ์.");
			prout(" ่ ฌฅเโข๋ฉ ชฎเ กซ์ คเฅฉไใฅโ ข ชฎแฌฎแฅ.");
			break;
		case FNRG:
/* 			prout("Your energy supply is exhausted.");
			skip(1);
			prout("Your starship is a derelict in space.");
 */			prout(" ชฎเ กซ๏ ชฎญ็จซจแ์ ง ฏ แ๋ ํญฅเฃจจ.");
			skip(1);
			prout(" ่ ฌฅเโข๋ฉ ชฎเ กซ์ คเฅฉไใฅโ ข ชฎแฌฎแฅ.");
			break;
		case FBATTLE:
/* 			proutn("The ");
			crmshp();
			prout("has been destroyed in battle.");
			skip(1);
			prout("Dulce et decorum est pro patria mori.");
 */			proutn(" ่ ชฎเ กซ์ ");
			crmshp();
			prout("ก๋ซ ใญจ็โฎฆฅญ ข กฎ๎.");
			skip(1);
			prout("Dulce et decorum est pro patria mori.");
			prout("/ซ คช  จ ฏเฅชเ แญ  ง  เฎคจญใ แฌฅเโ์./");
			break;
		case FNEG3:
/* 			prout("You have made three attempts to cross the negative energy");
			prout("barrier which surrounds the galaxy.");
			skip(1);
			prout("Your navigation is abominable.");
 */			prout("๋ แคฅซ ซจ โเจ ฏฎฏ๋โชจ ฏฅเฅแฅ็์ ก เ์ฅเ ฎโเจๆ โฅซ์ญฎฉ ํญฅเฃจจ");
			prout(" ฎชเใฆ ๎้จฉ ฃ ซ ชโจชใ.");
			skip(1);
			prout("๋ ใฆ แญ๋ฉ ญ ขจฃ โฎเ.");
			score(0);
			return;
		case FNOVA:
/* 			prout("Your starship has been destroyed by a nova.");
			prout("That was a great shot.");
 */			prout(" ่ ชฎเ กซ์ ก๋ซ ใญจ็โฎฆฅญ ญฎขฎฉ งขฅงคฎฉ.");
			prout("ฎเฎ่ ๏ ฏฎฏ๋โช .");
			skip(1);
			break;
		case FSNOVAED:
/* 			proutn("The ");
			crmshp();
			prout(" has been fried by a supernova.");
			prout("...Not even cinders remain...");
 */			proutn(" ่ ชฎเ กซ์ ");
			crmshp();
			prout(" ก๋ซ ใญจ็โฎฆฅญ แขฅเๅญฎขฎฉ.");
			prout("... ญฅ ฎแโ ซฎแ์ ค ฆฅ ฏฅฏซ ...");
			break;
		case FABANDN:
/* 			prout("You have been captured by the Klingons. If you still");
			prout("had a starbase to be returned to, you would have been");
			prout("repatriated and given another chance. Since you have");
			prout("no starbases, you will be mercilessly tortured to death.");
 */			prout("๋ ก๋ซจ ง ๅข ็ฅญ๋ ชซจญฃฎญ ฌจ. แซจ ก๋ ใ ข แ ");
			prout("ฎแโ ซ แ์ ๅฎโ์ ฎคญ  แโ ญๆจ๏ คซ๏ ขฎงขเ ้ฅญจ๏, ข๋ ฌฎฃซจ ก๋");
			prout("ฏฎซใ็จโ์ ขโฎเฎฉ ่ ญแ. ฎ ฏฎแชฎซ์ชใ แโ ญๆจฉ กฎซ์่ฅ ญฅโ,");
			prout("ข แ กฅแฏฎ้ คญฎ ง ฏ๋โ ๎โ คฎ แฌฅเโจ.");
			break;
		case FDILITHIUM:
			/* prout("Your starship is now an expanding cloud of subatomic particles"); */
			prout(" ่ ชฎเ กซ์ ฏเฅขเ โจซแ๏ ข ฎกซ ชฎ แใก โฎฌญ๋ๅ ็ แโจๆ");
			break;
		case FMATERIALIZE:
/* 			prout("Starbase was unable to re-materialize your starship.");
			prout("Sic transit gloria muntdi");
 */			prout("โ ญๆจ๏ ญฅ แฌฎฃซ  ฎกเ โญฎ ฌ โฅเจ ซจงฎข โ์ ข ่ ชฎเ กซ์.");
			prout("Sic transit gloria muntdi");
			prout("/ ช ฏเฎๅฎคจโ ฌจเแช ๏ แซ ข /");
			break;
		case FPHASER:
/* 			proutn("The ");
			crmshp();
			prout(" has been cremated by its own phasers.");
 */			proutn(" ่ ชฎเ กซ์ ");
			crmshp();
			prout(" แ ฌฎใญจ็โฎฆจซแ๏ ฎฃญฅฌ แฎกแโขฅญญ๋ๅ ไ งฅเฎข.");
			break;
		case FLOST:
/* 			prout("You and your landing party have been");
			prout("converted to energy, dissipating through space.");
 */			prout("๋ จ ฃเใฏฏ  ข๋แ คชจ ");
			prout("ฏเฅขเ โจซจแ์ ข ็จแโใ๎ ํญฅเฃจ๎, เ แฏ๋ซฅญญใ๎ ข ชฎแฌฎแฅ.");
			break;
		case FMINING:
/* 			prout("You are left with your landing party on");
			prout("a wild jungle planet inhabited by primitive cannibals.");
			skip(1);
			prout("They are very fond of \"Captain Kirk\" soup.");
			skip(1);
			proutn("Without your leadership, the ");
			crmshp();
			prout(" is destroyed.");
 */			prout("๋ ฎแโ ซจแ์ ขฌฅแโฅ แ ฃเใฏฏฎฉ ข๋แ คชจ");
			prout("ญ  คจชฎฉ ฏซ ญฅโฅ, ญ แฅซฅญญฎฉ คฎจแโฎเจ็ฅแชจฌจ ช ญญจก ซ ฌจ.");
			skip(1);
			prout("ญจ ฎ็ฅญ์ ซ๎ก๏โ แใฏ \" ฏจโ ญ จเช\".");
			skip(1);
			proutn("ฅง  ่ฅฃฎ ชฎฌ ญคฎข ญจ๏, ชฎเ กซ์ ");
			crmshp();
			prout(" ก๋ซ ใญจ็โฎฆฅญ.");
			break;
		case FDPLANET:
/* 			prout("You and your mining party perish.");
			skip(1);
			prout("That was a great shot.");
 */			prout("๋ กฅแแซฅคญฎ ฏเฎฏ ซจ ขฌฅแโฅ แ ่ ๅโฅเ ฌจ.");
			skip(1);
			prout("โฎ ก๋ซ  ๅฎเฎ่ ๏ ฏฎฏ๋โช .");
			skip(1);
			break;
		case FSSC:
			/* prout("The Galileo is instantly annihilated by the supernova."); */
			prout(" ซจซฅฎ ก๋ซ ฌฃญฎขฅญญฎ  ญญจฃจซจเฎข ญ แใฏฅเญฎขฎฉ.");
			// no break;
		case FPNOVA:
/* 			prout("You and your mining party are atomized.");
			skip(1);
			proutn("Mr. Spock takes command of the ");
			crmshp();
			prout(" and");
			prout("joins the Romulans, reigning terror on the Federation.");
 */			prout("๋ แ ฃเใฏฏฎฉ ่ ๅโฅเฎข เ แฏ ซจแ์ ญ   โฎฌ๋.");
			skip(1);
			proutn("จแโฅเ ฏฎช ฏเจญ๏ซ ชฎฌ ญคฎข ญจฅ ");
			crmshp();
			prout(" จ");
			prout("ฏเจแฎฅคจญจซแ๏ ช เฎฌใซ ญๆ ฌ, แฅ๏ เ งเใ่ฅญจฅ จ โฅเเฎเ ฏฎ ขแฅฉ ฅคฅเ ๆจจ.");
			break;
		case FSTRACTOR:
/* 			prout("The shuttle craft Galileo is also caught,");
			prout("and breaks up under the strain.");
			skip(1);
			prout("Your debris is scattered for millions of miles.");
			proutn("Without your leadership, the ");
			crmshp();
			prout(" is destroyed.");
 */			prout(" โโซ  ซจซฅฎ ฏฎฏ ซ ข ฏฎซฅ คฅฉแโขจ๏ แจซฎขฎฃฎ ซใ็ ,");
			prout("จ ก๋ซ เ งฎเข ญ แจซ ฌจ โ๏ฃฎโฅญจ๏.");
			skip(1);
			prout(" ่จ ฎกซฎฌชจ เ แแฅ๏ญ๋ ญ  ฌจซซจฎญ๋ ฌจซ์.");
			proutn("ฅง  ่ฅฃฎ ชฎฌ ญคฎข ญจ๏, ชฎเ กซ์ ");
			crmshp();
			prout(" ก๋ซ ใญจ็โฎฆฅญ.");
			break;
		case FDRAY:
/* 			prout("The mutants attack and kill Spock.");
			prout("Your ship is captured by Klingons, and");
			prout("your crew is put on display in a Klingon zoo.");
 */			prout("ใโ ญโ๋  โ ชฎข ซจ จ ใกจซจ ฏฎช .");
			prout(" ่ ชฎเ กซ์ ก๋ซ ง ๅข ็ฅญ ชซจญฃฎญ ฌจ จ โฅฏฅเ์");
			prout("ข ่  ชฎฌ ญค  ฏเฅคแโ ขซฅญ  ข ชซจญฃฎญแชฎฌ งฎฎฏ เชฅ.");
			break;
		case FTRIBBLE:
/* 			prout("Tribbles consume all remaining water,");
			prout("food, and oxygen on your ship.");
			skip(1);
			prout("You die of thirst, starvation, and asphyxiation.");
			prout("Your starship is a derelict in space.");
 */			prout("เจกกซ๋ แฎฆเ ซจ ขแ๎ ฎแโ ข่ใ๎แ๏ ขฎคใ,");
			prout("ฅคใ จ ชจแซฎเฎค ญ  ข ่ฅฌ ชฎเ กซฅ.");
			skip(1);
			prout("๋ แ ชฎฌ ญคฎฉ ใฌฅเซจ ฎโ ฃฎซฎค  จ ฆ ฆค๋, ง ค๋ๅ ๏แ์.");
			prout(" ่ ฌฅเโข๋ฉ ชฎเ กซ์ คเฅฉไใฅโ ข ชฎแฌฎแฅ.");
			break;
		case FHOLE:
/* 			prout("Your ship is drawn to the center of the black hole.");
			prout("You are crushed into extremely dense matter.");
 */			prout(" แ ง โ๏ญใซฎ ข ๆฅญโเ ็ฅเญฎฉ ค๋เ๋.");
			prout("๋ โฅฏฅเ์ จแชซ๎็จโฅซ์ญฎ ฌ ซฅญ์ช ๏ จ แขฅเๅโขฅเค ๏ ฏ๋ซจญช .");
			break;
#ifdef CLOAKING
		case FCLOAK:
			ncviol++;
/* 			prout("You have violated the Treaty of Algeron.");
			prout("The Romulan Empire can never trust you again.");
 */			prout("๋ ญ เใ่จซจ ซฃฅเฎญแชฎฅ ฎฃซ ่ฅญจฅ.");
			prout("ฎฌใซ ญแช ๏ ฌฏฅเจ๏ ญจชฎฃค  กฎซ์่ฅ ญฅ ฏฎขฅเจโ ข ฌ.");
			break;
#endif
	}
#ifdef CLOAKING
	if (ifin!=FWON && ifin!=FCLOAK && iscloaked!=0) {
/* 		prout("Your ship was cloaked so your subspace radio did not receive anything.");
		prout("You may have missed some warning messages.");
 */		prout(" ่ ชฎเ กซ์ ก๋ซ ญฅขจคจฌ, โ ช ็โฎ ฏฎคฏเฎแโเ ญแขฅญญฎฅ เ คจฎ ญฅ เ กฎโ ซฎ.");
		prout("๋ ฌฎฃซจ ฏเฎฏใแโจโ์ ญฅชฎโฎเ๋ฅ ฏเฅคใฏเฅคจโฅซ์ญ๋ฅ แฎฎก้ฅญจ๏.");
		skip(1);
	}
#endif

	if (ship==IHF) ship= 0;
	else if (ship == IHE) ship = IHF;
	alive = 0;
	if (d.remkl != 0) {
		double goodies = d.remres/inresor;
		double baddies = (d.remkl + 2.0*d.remcom)/(inkling+2.0*incom);
		if (goodies/baddies >= 1.0+0.5*Rand()) {
/* 			prout("As a result of your actions, a treaty with the Klingon");
			prout("Empire has been signed. The terms of the treaty are");
 */			prout(" เฅงใซ์โ โฅ ข ่จๅ คฅแโขจฉ, ก๋ซฎ ฏฎคฏจแ ญฎ ฌจเญฎฅ");
			prout("แฎฃซ ่ฅญจฅ แ ชซจญฃฎญ ฌจ. แซฎขจ๏ แฎฃซ ่ฅญจ๏ ก๋ซจ");
			if (goodies/baddies >= 3.0+Rand()) {
/* 				prout("favorable to the Federation.");
				skip(1);
				prout("Congratulations!");
 */				prout("ข๋ฃฎคญ๋ฌจ คซ๏ ฅคฅเ ๆจจ.");
				skip(1);
				prout("ฎงคเ ขซ๏ฅฌ!");
			}
			else
				/* prout("highly unfavorable to the Federation."); */
				prout("จแชซ๎็จโฅซ์ญฎ ญฅข๋ฃฎคญ๋ฌจ คซ๏ ฅคฅเ ๆจจ.");
		}
		else
			/* prout("The Federation will be destroyed."); */
			prout("ฅคฅเ ๆจ๏ กใคฅโ ใญจ็โฎฆฅญ .");
	}
	else {
/* 		prout("Since you took the last Klingon with you, you are a");
		prout("martyr and a hero. Someday maybe they'll erect a");
		prout("statue in your memory. Rest in peace, and try not");
		prout("to think about pigeons.");
 */		prout("๋ ง กเ ซจ ฏฎแซฅคญฅฃฎ ชซจญฃฎญ  แ แฎกฎฉ ญ  โฎโ แขฅโ.");
		prout(" ่  ฃฅเฎจ็ฅแช ๏ ฆฅเโข  ญฅ กใคฅโ ง ก๋โ  จ, ขฎงฌฎฆญฎ, ฏฎโฎฌชจ ค ฆฅ");
		prout("ขฎงขฅคใโ แโ โ๎ ข  ่ใ ็ฅแโ์. ฎชฎฉโฅแ์ แ ฌจเฎฌ จ แโ เ ฉโฅแ์");
		prout("ญฅ คใฌ โ์ ฎ ฃฎซใก๏ๅ.");
		gamewon = 1;
	}
	score(0);
}

void score(int inGame) {
	double timused = d.date - indate;
    int ithperd, iwon, klship;
    int dnromrem = d.nromrem; // Leave global value alone

    if (!inGame) pause(0);

	iskill = skill;
	if ((timused == 0 || d.remkl != 0) && timused < 5.0) timused = 5.0;
	perdate = (d.killc + d.killk + d.nsckill)/timused;
	ithperd = 500*perdate + 0.5;
	iwon = 0;
	if (gamewon) iwon = 100*skill;
	if (ship == IHE) klship = 0;
	else if (ship == IHF) klship = 1;
	else klship = 2;
	if (gamewon == 0 || inGame) dnromrem = 0; // None captured if no win or if still in the game
	iscore = 10*d.killk + 50*d.killc + ithperd + iwon
			 - 100*d.basekl - 100*klship - 45*nhelp -5*d.starkl - casual
		 + 20*d.nromkl + 200*d.nsckill - 10*d.nplankl + dnromrem;
#ifdef CLOAKING
	iscore -= 100*ncviol;
#endif
#ifdef CAPTURE
	iscore += 3*kcaptured;
#endif
	if (alive == 0) iscore -= 200;
	skip(2);
/*     if (inGame) prout("Your score so far --");
    else prout("Your score --");
 */    if (inGame) prout("  ค ญญ๋ฉ ฌฎฌฅญโ ข ่จ ก ซซ๋ --");
    else prout(" ่จ ก ซซ๋ --");
	if (d.nromkl)
	 /* printf(d.nromkl> 1 ? "%6d Romulan ships destroyed            %5d\n" : "%6d Romulan ship destroyed             %5d\n", */
		printf(d.nromkl> 1 ? "%6d เฎฌใซ ญแชจๅ ชฎเ กซฅฉ ใญจ็โฎฆฅญฎ        %5d\n" : "%6d เฎฌใซ ญแชจๅ ชฎเ กซฅฉ ใญจ็โฎฆฅญฎ             %5d\n",
			   d.nromkl, 20*d.nromkl);
	if (dnromrem)
	 /* printf(dnromrem > 1 ? "%6d Romulan ships captured             %5d\n" : "%6d Romulan ship captured              %5d\n", */
		printf(dnromrem > 1 ? "%6d เฎฌใซ ญแชจๅ ชฎเ กซฅฉ ง ๅข ็ฅญฎ         %5d\n" : "%6d เฎฌใซ ญแชจๅ ชฎเ กซฅฉ ง ๅข ็ฅญฎ              %5d\n",
			   dnromrem, dnromrem);
	if (d.killk)
	 /* printf(d.killk > 1 ? "%6d ordinary Klingon ships destroyed   %5d\n" : "%6d ordinary Klingon ship destroyed    %5d\n", */
		printf(d.killk > 1 ? "%6d กฎฅข๋ๅ ชฎเ กซฅฉ ชซจญฃฎญฎข ใญจ็โฎฆฅญฎ    %5d\n" : "%6d กฎฅข๋ๅ ชฎเ กซฅฉ ชซจญฃฎญฎข ใญจ็โฎฆฅญฎ    %5d\n",
			   d.killk,  10*d.killk);
	if (d.killc)
		/* printf(d.killc > 1 ? "%6d Klingon Commander ships destroyed  %5d\n" : "%6d Klingon Commander ship destroyed   %5d\n", */
		printf(d.killc > 1 ? "%6d ชซจญฃฎญแชจๅ ฎฌ ญคฅเฎข ใญจ็โฎฆฅญฎ       %5d\n" : "%6d ชซจญฃฎญแชจๅ ฎฌ ญคฅเฎข ใญจ็โฎฆฅญฎ   %5d\n",
			   d.killc, 50*d.killc);
	if (d.nsckill)
     /* printf("%6d Super-Commander ship destroyed     %5d\n", */
		printf("%6d ใฏฅเฎฌฌ ญคฅเฎข ใญจ็โฎฆฅญฎ        %5d\n",
			   d.nsckill, 200*d.nsckill);
	if (ithperd)
     /* printf("%6.2f Klingons per stardate              %5d\n", */
		printf("%6.2f ชซจญฃฎญฎข ข แเฅคญฅฌ ง  ค โใ       %5d\n",
			   perdate, ithperd);
#ifdef CAPTURE
	if (kcaptured)
		/* printf(kcaptured > 1 ? "%6d Klingons captured                  %5d\n" : "%6d Klingon captured                   %5d\n", */
		printf(kcaptured > 1 ? "%6d ชซจญฃฎญฎข ง ๅข ็ฅญฎ                  %5d\n" : "%6d ชซจญฃฎญฎข ง ๅข ็ฅญฎ                   %5d\n",
		        kcaptured, 3*kcaptured);
#endif
	if (d.starkl)
		/* printf(d.starkl > 1 ? "%6d stars destroyed by your action     %5d\n" : "%6d star destroyed by your action      %5d\n", */
		printf(d.starkl > 1 ? "%6d งขฅงค ใญจ็โฎฆฅญฎ ข ่จฌจ คฅฉแโขจ๏ฌจ     %5d\n" : "%6d งขฅงค ใญจ็โฎฆฅญฎ ข ่จฌจ คฅฉแโขจ๏ฌจ      %5d\n",
			   d.starkl, -5*d.starkl);
	if (d.nplankl)
		/* printf(d.nplankl > 1 ? "%6d planets destroyed by your action   %5d\n" : "%6d planet destroyed by your action    %5d\n", */
		printf(d.nplankl > 1 ? "%6d ฏซ ญฅโ ใญจ็โฎฆฅญฎ ข ่จฌจ คฅฉแโขจ๏ฌจ    %5d\n" : "%6d ฏซ ญฅโ ใญจ็โฎฆฅญฎ ข ่จฌจ คฅฉแโขจ๏ฌจ    %5d\n",
			   d.nplankl, -10*d.nplankl);
	if (d.basekl)
		printf(d.basekl > 1 ? "%6d แโ ญๆจฉ ใญจ็โฎฆฅญฎ ข ่จฌจ คฅฉแโขจ๏ฌจ     %5d\n" : "%6d แโ ญๆจฉ ใญจ็โฎฆฅญฎ ข ่จฌจ คฅฉแโขจ๏ฌจ      %5d\n",
			   d.basekl, -100*d.basekl);
	if (nhelp)
		printf(nhelp > 1 ? "%6d ข๋งฎขฎข ฎ ฏฎฌฎ้จ แฎ แโ ญๆจฉ       %5d\n" : "%6d ข๋งฎขฎข ฎ ฏฎฌฎ้จ แฎ แโ ญๆจฉ        %5d\n",
			   nhelp, -45*nhelp);
	if (casual)
		printf(casual > 1 ? "%6d ฏฎโฅเ์ แเฅคจ ํชจฏ ฆ                 %5d\n" : "%6d ฏฎโฅเ์ แเฅคจ ํชจฏ ฆ                   %5d\n",
			   casual, -casual);
	if (klship)
		printf(klship > 1 ? "%6d ชฎเ กซฅฉ ฏฎโฅเ๏ญฎ จซจ ใญจ็โฎฆฅญฎ            %5d\n" : "%6d ชฎเ กซฅฉ ฏฎโฅเ๏ญฎ จซจ ใญจ็โฎฆฅญฎ             %5d\n",
			   klship, -100*klship);
#ifdef CLOAKING
	if (ncviol>0)
		printf(ncviol > 1 ? "%6d ญ เใ่ฅญจฉ ซฃฅเฎญแชฎฃฎ แฎฃซ ่ฅญจ๏       %5d\n" : "%6d ญ เใ่ฅญจฉ ซฃฅเฎญแชฎฃฎ แฎฃซ ่ฅญจ๏        %5d\n",
		       ncviol, -100*ncviol);
#endif
	if (alive==0)
		/* prout("Penalty for getting yourself killed        -200"); */
		prout("โเ ไ ง  โฎ, ็โฎ ๋ ค ซจ แฅก๏ ใกจโ์        -200");
	if (gamewon) {
		skip(1);
		/* proutn("Bonus for winning "); */
		proutn("ฎญใแ ง  ฏฎกฅคใ ");
		switch (skill) {
/* 			case SNOVICE: proutn("Novice game  "); break;
			case SFAIR: proutn("Fair game    "); break;
			case SGOOD: proutn("Good game    "); break;
			case SEXPERT: proutn("Expert game  "); break;
			case SEMERITUS: proutn("Emeritus game"); break;
 */			case SNOVICE: proutn("ฃเ  ฎขจ็ช   "); break;
			case SFAIR: proutn("ฃเ   ซ ฃจ    "); break;
			case SGOOD: proutn("ฃเ  โซจ็ญจช     "); break;
			case SEXPERT: proutn("ฃเ  ชแฏฅเโ   "); break;
			case SEMERITUS: proutn("ฃเ  ฌฅเจโใแ "); break;
		}
		printf("           %5d\n", iwon);
	}
	skip(2);
    printf("โฎฃฎข๋ฉ ก ซซ                               %5d\n", iscore);/*TOTAL SCORE*/
    /* if (inGame && skill < SGOOD) printf("REMEMBER--The score doesn't really matter until the mission is accomplished!\n"); */
    if (inGame && skill < SGOOD) printf("-- ซซ๋ ญจ็ฅฃฎ ญฅ งญ ็ โ, ฏฎช  ฌจแแจ๏ ญฅ ฎชฎญ็ฅญ !\n");
}

void plaque(void) {
	FILE *fp=NULL;
#ifndef KOS32	
	time_t t;
#else
	int kos_date, kos_time;
#endif
	char *timestring;
	int nskip;
	char winner[128];
	skip(2);
	
	while (fp == NULL) {
		/* printf("File or device name for your plaque:"); */
		printf("ขฅคจโฅ จฌ๏ ไ ฉซ  คซ๏ ข ่จๅ คฎแโจฆฅญจฉ:");
#ifndef KOS32		
		fgets(winner, 128, stdin);
#else
		gets(winner);
#endif		
		winner[strlen(winner)-1] = '\0';
		fp = fopen(winner, "w");
		if (fp==NULL) {
			printf("ฅขฅเญฎฅ จฌ๏.\n");/*Invalid name*/
		}
	}

	/* printf("Enter name to go on plaque (up to 30 characters):"); */
	printf("ขฅคจโฅ จฌ๏ คซ๏ ฏ ฌ๏โญฎฃฎ งญ ็ช  (ฌ ชแจฌใฌ 30 แจฌขฎซฎข):");
#ifndef KOS32		
		fgets(winner, 128, stdin);
#else
		gets(winner);
#endif		
	winner[strlen(winner)-1] = '\0';
	winner[30] = '\0';
	nskip = 64 - strlen(winner)/2;

	fprintf(fp,"\n\n\n\n");
	/* --------DRAW ENTERPRISE PICTURE. */
	fprintf(fp, "                                                                EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n" );
	fprintf(fp, "                                      EEE                      E  : :                                         :  E\n" );
	fprintf(fp, "                                    EE   EEE                   E  : :                   NCC-1701              :  E\n");
	fprintf(fp, "                    EEEEEEEEEEEEEEEE        EEEEEEEEEEEEEEE    E  : :                                         : E\n");
	fprintf(fp, "                     E                                     E    EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");
	fprintf(fp, "                      EEEEEEEEE               EEEEEEEEEEEEE                 E  E\n");
	fprintf(fp, "                               EEEEEEE   EEEEE    E          E              E  E\n");
	fprintf(fp, "                                      EEE           E          E            E  E\n");
	fprintf(fp, "                                                       E         E          E  E\n");
	fprintf(fp, "                                                         EEEEEEEEEEEEE      E  E\n");
	fprintf(fp, "                                                      EEE :           EEEEEEE  EEEEEEEE\n");
	fprintf(fp, "                                                    :E    :                 EEEE       E\n");
	fprintf(fp, "                                                   .-E   -:-----                       E\n");
	fprintf(fp, "                                                    :E    :                            E\n");
	fprintf(fp, "                                                      EE  :                    EEEEEEEE\n");
	fprintf(fp, "                                                       EEEEEEEEEEEEEEEEEEEEEEE\n");
	fprintf(fp, "\n\n\n");
	fprintf(fp, "                                                       U. S. S. ENTERPRISE\n");
	fprintf(fp, "\n\n\n\n");
	/* fprintf(fp, "                                  For demonstrating outstanding ability as a starship captain\n"); */
	fprintf(fp, "                                    ฏเฎ๏ขซฅญญ๋ฅ ข๋ค ๎้จฅแ๏ คฎแโจฆฅญจ๏ ข คฎซฆญฎแโจ ช ฏจโ ญ  ชฎแฌจ็ฅแชฎฃฎ ชฎเ กซ๏\n");
	fprintf(fp, "\n");
	/* fprintf(fp, "                                                Starfleet Command bestows to you\n"); */
	fprintf(fp, "                                                ขฅงคญฎฅ ฎฌ ญคฎข ญจฅ ฏเฅคแโ ขซ๏ฅโ  แ\n");
	fprintf(fp, "\n");
	fprintf(fp,"%*s%s\n\n", nskip, "", winner);
/* 	fprintf(fp, "                                                           the rank of\n\n");
	fprintf(fp, "                                                       \"Commodore Emeritus\"\n\n");
 */	fprintf(fp, "                                                           ช งข ญจ๎\n\n");
	fprintf(fp, "                                                       \"ฎฌ ญคฎเ ฌฅเจโใแ\"\n\n");
	fprintf(fp, "                                                          ");
	switch (iskill) {
/* 		case SEXPERT: fprintf(fp," Expert level\n\n"); break;
		case SEMERITUS: fprintf(fp,"Emeritus level\n\n"); break;
		default: fprintf(fp," Cheat level\n\n"); break;
 */		case SEXPERT: fprintf(fp," ใเฎขฅญ์ ชแฏฅเโ \n\n"); break;
		case SEMERITUS: fprintf(fp,"ใเฎขฅญ์ ฌฅเจโใแ \n\n"); break;
		default: fprintf(fp," ฎก๋็ญ๋ฉ ็จโฅเแชจฉ ใเฎขฅญ์\n\n"); break;
	}
#ifndef KOS32	
	t = time(NULL);
	timestring = ctime(&t);
	/* fprintf(fp, "                                                 This day of %.6s %.4s, %.8s\n\n", */
	fprintf(fp, "                                                 โฎโ คฅญ์ %.6s %.4s, %.8s\n\n",
			timestring+4, timestring+20, timestring+11);
#else
	kos_date = _ksys_get_date();
	kos_time = _ksys_get_system_clock();
	/* fprintf(fp, "                                                 This day of %02i/%02i/%02i %02i:%02i:%02i\n\n", */
	fprintf(fp, "                                                 โฎโ คฅญ์ %02i/%02i/%02i %02i:%02i:%02i\n\n",
			kos_date >> 16, (kos_date & 0xFF00) >> 8, (kos_date & 0xFF) + 2000,
			kos_time & 0xFF, (kos_time & 0xFF00) >> 8, kos_time >> 16 );
#endif
/* 	fprintf(fp,"                                                        Your score:  %d\n\n", iscore);
	fprintf(fp,"                                                    Klingons per stardate:  %.2f\n", perdate);
 */	fprintf(fp,"                                                         ่ ก ซซ:  %d\n\n", iscore);
	fprintf(fp,"                                                    ซจญฃฎญฎข ญ  ค โใ:  %.2f\n", perdate);
	fclose(fp);
}
