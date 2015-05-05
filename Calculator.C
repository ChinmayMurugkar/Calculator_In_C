/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                           The  Calculator                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/

//inclusion of required header files
#include < stdio.h > #include < graphics.h > #include < stdlib.h > #include < dos.h >

//change the time for which the buttons glow when clicked (300-450 is ideal)
#define BCD 200
//change the delay exiting program (200-350 is ideal)
#define EXIT 150

union REGS i, o;

char calc(); //function prototypes
void showmouseptr();
void hidemouseptr();
void restrictmouseptr(int x1, int y1, int x2, int y2);
void getmousepos(int * button, int * x, int * y);

void main() {
	int graphdetect = DETECT, graphmode, i;
	char ch = 'y', ans;
	initgraph( & graphdetect, & graphmode, " "); //initializing graphics mode
	load();
	while (ch != 'n') {
		ch = mmenu(); //main menu
		if (ch == 'n') break;
	}
	cleardevice();
	setcolor(7);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 1);
	outtextxy(20, 445, "Bye! See you again ..."); //goodbye message
	gotoxy(19, 12);
	printf("Please wait... ");
	for (i = 1; i <= 25; i++) {
		printf("%c", 219);
		delay(EXIT);
	}
	closegraph();
	restorecrtmode();
	delay(1500);
}

load() {
	int i;
	setcolor(7);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 1);
	outtextxy(20, 445, "Loading ..."); //loading screen
	gotoxy(19, 12);
	printf("Please wait... ");
	for (i = 0; i <= 24; i++) {
		printf("%c", 219);
		delay(EXIT);
	}
	printf("\n\n\t\t");
	outtextxy(60, 200, "Press any key to continue...");
	setfillstyle(0, 0);
	bar(20, 450, 108, 480);
	getch();
	return 0;
}


mmenu() //Displaying the main menu
{
	int i, midx, midy, maxx, maxy;
	char ch;
	midx = getmaxx() / 2;
	cleardevice();

	//main menu
	setlinestyle(0, 1, 1);
	setcolor(7);
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
	setcolor(11);
	outtextxy(midx - 60, 80, "MAIN MENU");
	line(midx - 60, 107, midx + 50, 107);
	settextstyle(SMALL_FONT, HORIZ_DIR, 6);
	outtextxy(midx - 240, 130, "1. The Calculator");
	outtextxy(midx - 240, 150, "2. Mouse interfacing");
	outtextxy(midx - 240, 170, "3. Help");
	outtextxy(midx - 240, 190, "4. About");
	outtextxy(midx - 240, 210, "5. Exit");
	outtextxy(midx - 240, 270, "Enter your choice (1-5)");
	outtextxy(midx - 240, 300, "*** If wrong key is pressed, you will hear");
	outtextxy(midx - 240, 320, "    a beep...");
	setcolor(11);

	ch = getch(); //Accept the user's choice

	switch (ch) {
		case '1':
			calc(); //Displaying the calculator
			break; //Calculations

		case '2':
			mouse(); //Mouse interfacing
			break;

		case '3':
			help(); //Know how to use this calculator
			break;

		case '4':
			about(); //Project made by
			break;

		case '5':
			return ('n'); //Exit from main menu

		default:
			sound(1000); //Beep heard if user strikes a wrong key
			delay(1000);
			nosound();

	}
	return 0;
}

menu() {
	cleardevice();
	//menu
	setcolor(4);
	rectangle(0, 30, getmaxx(), getmaxy()); //bordering
	line(0, 210, 360, 210); //the horizontal line below first and second operand
	line(360, 30, 360, 478); //the vertical line partitioning the calculator
	setcolor(14);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
	outtextxy(185, 0, "* * * The Calculator * * *");

	//calculator's outer lines
	setcolor(9);
	rectangle(400, 140, 600, 440);
	rectangle(415, 155, 585, 185);
	line(400, 200, 600, 200); //horizontal line separating screen from keypad

	//outer rectangles for buttons
	rectangle(412, 212, 588, 242);
	rectangle(412, 254, 588, 284);
	rectangle(412, 296, 588, 326);
	rectangle(412, 338, 588, 368);
	rectangle(412, 380, 498, 428);
	rectangle(512, 380, 588, 428);

	//lines for the buttons
	line(470, 212, 470, 242); //1 and 2
	line(530, 212, 530, 242); //2 and 3
	line(470, 254, 470, 284); //4 and 5
	line(530, 254, 530, 284); //5 and 6
	line(470, 296, 470, 326); //7 and 8
	line(530, 296, 530, 326); //8 and 9
	line(470, 338, 470, 368); //C and 0
	line(530, 338, 530, 368); //0 and .
	line(455, 380, 455, 428); //vertical line for operator box
	line(412, 404, 498, 404); //horizontal line for operator box

	//text
	setcolor(15);
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	outtextxy(530, 400, "O F F");


	//actual buttons' text
	outtextxy(435, 225, "1");
	outtextxy(495, 225, "2");
	outtextxy(555, 225, "3");
	outtextxy(435, 267, "4");
	outtextxy(495, 267, "5");
	outtextxy(555, 267, "6");
	outtextxy(435, 307, "7");
	outtextxy(495, 307, "8");
	outtextxy(555, 307, "9");
	outtextxy(435, 349, "C");
	outtextxy(495, 349, "0");
	outtextxy(555, 349, ".");
	outtextxy(430, 388, "+");
	outtextxy(472, 388, "-");
	outtextxy(430, 412, "*");
	outtextxy(472, 412, "/");
	return 0;
}

char calc() {
	FILE * fp;
	int i, j, k, eight;
	char a[9], b[9], x, * endptr, ch;
	double n1, n2;

	cleardevice();
	menu();
	moveto(425, 167);
	i = 0;
	j = 0;
	fp = fopen("c:\calclog2.txt", "a"); //File to store all the calculations
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
	outtextxy(400, 100, "Enter first number...");
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	eight = 8;
	//in case user adds a dot eight++ will make the no still be of max 8 digits

	a: while (i < eight) {
		if (i == eight) //this breaks the loop if number is of 8 digits
		break;
		x = getch();
		a[i] = x; //retrieving the number entered by the user and storing it in array

		switch (x) {
			case '1':
				outtextxy(getx(), gety(), "1");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(413, 213, 469, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 213, 469, 241);
				outtextxy(435, 225, "1");
				i++;
				goto a;

			case '2':
				outtextxy(getx(), gety(), "2");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 213, 529, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 213, 529, 241);
				outtextxy(495, 225, "2");
				i++;
				goto a;

			case '3':
				outtextxy(getx(), gety(), "3");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(531, 213, 587, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 213, 587, 241);
				outtextxy(555, 225, "3");
				i++;
				goto a;

			case '4':
				outtextxy(getx(), gety(), "4");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(413, 255, 469, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 255, 469, 283);
				outtextxy(435, 267, "4");
				i++;
				goto a;

			case '5':
				outtextxy(getx(), gety(), "5");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 255, 529, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 255, 529, 283);
				outtextxy(495, 267, "5");
				i++;
				goto a;

			case '6':
				outtextxy(getx(), gety(), "6");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(531, 255, 587, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 255, 587, 283);
				outtextxy(555, 267, "6");
				i++;
				goto a;

			case '7':
				outtextxy(getx(), gety(), "7");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(413, 297, 469, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 297, 469, 325);
				outtextxy(435, 307, "7");
				i++;
				goto a;

			case '8':
				outtextxy(getx(), gety(), "8");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 297, 529, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 297, 529, 325);
				outtextxy(495, 307, "8");
				i++;
				goto a;

			case '9':
				outtextxy(getx(), gety(), "9");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(531, 297, 587, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 297, 587, 325);
				outtextxy(555, 307, "9");
				i++;
				goto a;

			case '0':
				outtextxy(getx(), gety(), "0");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 339, 529, 367);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 339, 529, 367);
				outtextxy(495, 349, "0");
				i++;
				goto a;

			case '.':
				outtextxy(getx(), gety(), ".");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(531, 339, 587, 367);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 339, 587, 367);
				outtextxy(555, 349, ".");
				i++;
				eight++;
				goto a;

			case 'n':
				goto c; //for storing the number if it's less than 8 digits

			case 'q':
				ch = 'n'; //to go directly to the end
				goto e;
		}
	}

	c: n1 = strtod(a, & endptr); //strnod converts string to double!
	cleardevice();
	menu();
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
	outtextxy(400, 100, "Enter second number...");
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	moveto(425, 167);
	eight = 8;

	b: while (j < eight) {
		if (j == eight) break;
		x = getch();
		b[j] = x;

		switch (x) {
			case '1':
				outtextxy(getx(), gety(), "1");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(413, 213, 469, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 213, 469, 241);
				outtextxy(435, 225, "1");
				j++;
				goto b;

			case '2':
				outtextxy(getx(), gety(), "2");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 213, 529, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 213, 529, 241);
				outtextxy(495, 225, "2");
				j++;
				goto b;

			case '3':
				outtextxy(getx(), gety(), "3");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(531, 213, 587, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 213, 587, 241);
				outtextxy(555, 225, "3");
				j++;
				goto b;

			case '4':
				outtextxy(getx(), gety(), "4");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(413, 255, 469, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 255, 469, 283);
				outtextxy(435, 267, "4");
				j++;
				goto b;

			case '5':
				outtextxy(getx(), gety(), "5");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 255, 529, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 255, 529, 283);
				outtextxy(495, 267, "5");
				j++;
				goto b;

			case '6':
				outtextxy(getx(), gety(), "6");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(531, 255, 587, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 255, 587, 283);
				outtextxy(555, 267, "6");
				j++;
				goto b;

			case '7':
				outtextxy(getx(), gety(), "7");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(413, 297, 469, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 297, 469, 325);
				outtextxy(435, 307, "7");
				j++;
				goto b;

			case '8':
				outtextxy(getx(), gety(), "8");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 297, 529, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 297, 529, 325);
				outtextxy(495, 307, "8");
				j++;
				goto b;

			case '9':
				outtextxy(getx(), gety(), "9");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(531, 297, 587, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 297, 587, 325);
				outtextxy(555, 307, "9");
				j++;
				goto b;

			case '0':
				outtextxy(getx(), gety(), "0");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 339, 529, 367);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 339, 529, 367);
				outtextxy(495, 349, "0");
				j++;
				goto b;

			case '.':
				outtextxy(getx(), gety(), ".");
				moveto(getx() + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(531, 339, 587, 367);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 339, 587, 367);
				outtextxy(555, 349, ".");
				j++;
				eight++;
				goto b;

			case 'n':
				goto d;

			case 'q':
				ch = 'n';
				goto e;
		}
	}

	d: n2 = strtod(b, & endptr);
	cleardevice();
	menu();
	gotoxy(8, 5);
	printf("First operand=%lf", n1); /*Displaying both the operands*/
	gotoxy(1, 6); /*entered by the user*/
	printf("\n\t\tSecond operand=%lf", n2);
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
	outtextxy(400, 100, "Enter the operator..."); //+, -, * or /
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	g: x = getch();

	switch (x) {
		case 'a':
			setfillstyle(11, 4);
			bar(413, 381, 454, 403);
			delay(BCD);
			setfillstyle(11, 0); //button glow
			bar(413, 381, 454, 403);
			outtextxy(430, 388, "+");
			settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
			outtextxy(10, 270, "The answer is...");
			settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
			gotoxy(2, 20);
			printf("%lf + %lf = %lf", n1, n2, n1 + n2);
			fprintf(fp, "%lf + %lf = %lf\n", n1, n2, n1 + n2);
			break;

		case 's':
			setfillstyle(11, 4);
			bar(456, 381, 497, 403);
			delay(BCD);
			setfillstyle(11, 0); //button glow
			bar(456, 381, 497, 403);
			outtextxy(472, 388, "-");
			settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
			outtextxy(10, 270, "The answer is...");
			settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
			gotoxy(2, 20);
			printf("%lf - %lf = %lf", n1, n2, n1 - n2);
			fprintf(fp, "%lf - %lf = %lf\n", n1, n2, n1 - n2);
			break;

		case 'm':
			setfillstyle(11, 4);
			bar(413, 405, 454, 427);
			delay(BCD);
			setfillstyle(11, 0); //button glow
			bar(413, 405, 454, 427);
			outtextxy(430, 412, "*");
			settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
			outtextxy(10, 270, "The answer is...");
			settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
			gotoxy(2, 20);
			printf("%lf * %lf = %lf", n1, n2, n1 * n2);
			fprintf(fp, "%lf * %lf = %lf\n", n1, n2, n1 * n2);
			break;

		case 'd':
			setfillstyle(11, 4);
			bar(456, 405, 497, 427);
			delay(BCD);
			setfillstyle(11, 0); //button glow
			bar(456, 405, 497, 427);
			outtextxy(472, 412, "/");
			settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
			outtextxy(10, 270, "The answer is...");
			settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
			gotoxy(2, 20);
			printf("%lf / %lf = %lf", n1, n2, n1 / n2);
			fprintf(fp, "%lf / %lf = %lf\n", n1, n2, n1 / n2);
			break;

		case 'q':
			ch = 'n';
			goto e;

		default:
			goto g;
	}

	f: settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
	outtextxy(10, 380, "Another calculation? (y/n)");
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	ch = getch();
	e: if (ch == 'y') {
		setfillstyle(11, 4); //button glow
		bar(413, 339, 469, 367);
		delay(BCD);
		calc();
	} else {
		if (ch == 'n') {
			setfillstyle(11, 4); //button glow
			bar(513, 381, 587, 427);
			delay(BCD);
			setfillstyle(11, 0);
			bar(513, 381, 587, 427);
			outtextxy(530, 400, "O F F");
			delay(BCD);
		} else goto f;
	}
	fclose(fp);
	return ('y');
}




mouse() //Mouse interfacing starts
{
	int i, j, k, eight, midx, midy, maxx, maxy, ch, button, x, y, xcor;
	FILE * fp;
	char a[20], b[20], * endptr;
	double n1, n2;
	cleardevice();
	maxx = getmaxx();
	maxy = getmaxy();
	rectangle(0, 0, maxx, maxy); //outer rectangle
	setviewport(1, 1, maxx - 1, maxy - 1, 1);

	initmouse(); //Initialise the mouse pointer
	bg: menu();
	i = 0;
	j = 0; //Display the calculator
	xcor = 420;
	restrictmouseptr(1, 1, maxx - 1, maxy - 1); //Restrict the movement of the cursor
	showmouseptr();
	gotoxy(1, 2);
	printf("Left Button");
	gotoxy(15, 2);
	printf("Right Button");
	outtextxy(400, 126, "Enter first number...");
	while (!kbhit()) {
		getmousepos( & button, & x, & y); //Reading the first operand

		gotoxy(5, 3);
		e: if ((button & 1) == 1) {
			printf(" PRESSED "); //Show that the button has been pressed

			if (x > 470 && x < 529 && y > 338 && y < 368) {
				x = '0';
				a[i] = x;
				outtextxy(xcor, 166, "0");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(471, 339, 529, 367);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 339, 529, 367);
				outtextxy(495, 349, "0");
				xcor = xcor + 10;
				i++;
			}


			if (x > 412 && x < 470 && y > 212 && y < 242) {
				x = '1';
				a[i] = x;
				outtextxy(xcor, 166, "1");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(413, 213, 469, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 213, 469, 241);
				outtextxy(435, 225, "1");
				xcor = xcor + 10;
				i++;
			}



			if (x > 470 && x < 529 && y > 212 && y < 242) {
				x = '2';
				a[i] = x;
				outtextxy(xcor, 166, "2");
				moveto(xcor + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 213, 529, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 213, 529, 241);
				outtextxy(495, 225, "2");
				xcor = xcor + 10;
				i++;
			}


			if (x > 529 && x < 588 && y > 212 && y < 242) {
				x = '3';
				a[i] = x;
				outtextxy(xcor, 166, "3");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(531, 213, 587, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 213, 587, 241);
				outtextxy(555, 225, "3");
				xcor = xcor + 10;
				i++;
			}




			if (x > 412 && x < 470 && y > 254 && y < 284) {
				x = '4';
				a[i] = x;
				outtextxy(xcor, 166, "4");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(413, 255, 469, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 255, 469, 283);
				outtextxy(435, 267, "4");
				xcor = xcor + 10;
				i++;
			}


			if (x > 470 && x < 529 && y > 254 && y < 284) {
				x = '5';
				a[i] = x;
				outtextxy(xcor, 166, "5");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(471, 255, 529, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 255, 529, 283);
				outtextxy(495, 267, "5");
				xcor = xcor + 10;
				i++;
			}


			if (x > 529 && x < 588 && y > 254 && y < 284) {
				x = '6';
				a[i] = x;
				outtextxy(xcor, 166, "6");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(531, 255, 587, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 255, 587, 283);
				outtextxy(555, 267, "6");
				xcor = xcor + 10;
				i++;
			}



			if (x > 412 && x < 470 && y > 296 && y < 326) {
				x = '7';
				a[i] = x;
				outtextxy(xcor, 166, "7");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(413, 297, 469, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 297, 469, 325);
				outtextxy(435, 307, "7");
				xcor = xcor + 10;
				i++;
			}




			if (x > 470 && x < 529 && y > 296 && y < 326) {
				x = '8';
				a[i] = x;
				outtextxy(xcor, 166, "8");
				moveto(xcor + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 297, 529, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 297, 529, 325);
				outtextxy(495, 307, "8");
				xcor = xcor + 10;
				i++;
			}



			if (x > 529 && x < 588 && y > 296 && y < 326) {
				x = '9';
				a[i] = x;
				outtextxy(xcor, 166, "9");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(531, 297, 587, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 297, 587, 325);
				outtextxy(555, 307, "9");
				xcor = xcor + 10;
				i++;
			}


			if (x > 529 && x < 588 && y > 338 && y < 368) {
				x = '.';
				a[i] = x;
				outtextxy(xcor, 166, ".");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(531, 339, 587, 367);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 339, 587, 367);
				outtextxy(555, 349, ".");
				xcor = xcor + 10;
				i++;
			}



			if (x > 412 && x < 470 && y > 338 && y < 368) {
				setfillstyle(11, 4); //button glow
				bar(413, 339, 469, 367);
				delay(BCD);
				outtextxy(435, 349, "C"); //Accpet the operand
				setfillstyle(11, 0);
				bar(413, 339, 469, 367);
				n1 = strtod(a, & endptr); //strtod converts string to double!
				menu();
				goto d;
			}



			if (x > 512 && x < 588 && y > 380 && y < 428) {
				setfillstyle(11, 4); //button glow
				bar(513, 381, 587, 427);
				delay(BCD);
				setfillstyle(11, 0);
				bar(513, 381, 587, 427);
				outtextxy(530, 400, "O F F"); /*Put off the calculator & return*/
				delay(BCD); /*to the main menu*/
				goto ex;
			}



		} else {
			gotoxy(5, 3);
			printf("RELEASED");
		}

		gotoxy(20, 3);
		(button & 2) == 2 ? printf(" PRESSED ") : printf("RELEASED");

		gotoxy(65, 2); /*Showing the co-ordinates of the*/
		printf("x=%03d y=%03d", x, y); /* current position of the cursor*/
	}
	goto e;


	d: xcor = 420;
	menu();
	gotoxy(1, 2);
	printf("Left Button");
	gotoxy(15, 2);
	printf("Right Button");

	outtextxy(400, 126, "Enter second number...");

	while (!kbhit()) //Reading the second operand
	{
		getmousepos( & button, & x, & y);

		gotoxy(5, 3);
		g: if ((button & 1) == 1) {
			printf(" PRESSED ");


			if (x > 470 && x < 529 && y > 338 && y < 368) {
				x = '0';
				b[j] = x;
				outtextxy(xcor, 166, "0");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(471, 339, 529, 367);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 339, 529, 367);
				outtextxy(495, 349, "0");
				j++;
				xcor = xcor + 10;
			}


			if (x > 412 && x < 470 && y > 212 && y < 242) {
				x = '1';
				b[j] = x;
				outtextxy(xcor, 166, "1");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(413, 213, 469, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 213, 469, 241);
				outtextxy(435, 225, "1");
				xcor = xcor + 10;
				j++;
			}



			if (x > 470 && x < 529 && y > 212 && y < 242) {
				x = '2';
				b[j] = x;
				outtextxy(xcor, 166, "2");
				moveto(xcor + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 213, 529, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 213, 529, 241);
				outtextxy(495, 225, "2");
				xcor = xcor + 10;
				j++;
			}


			if (x > 529 && x < 588 && y > 212 && y < 242) {
				x = '3';
				b[j] = x;
				outtextxy(xcor, 166, "3");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(531, 213, 587, 241);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 213, 587, 241);
				outtextxy(555, 225, "3");
				xcor = xcor + 10;
				j++;
			}




			if (x > 412 && x < 470 && y > 254 && y < 284) {
				x = '4';
				b[j] = x;
				outtextxy(xcor, 166, "4");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(413, 255, 469, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 255, 469, 283);
				outtextxy(435, 267, "4");
				xcor = xcor + 10;
				j++;
			}


			if (x > 470 && x < 529 && y > 254 && y < 284) {
				x = '5';
				b[j] = x;
				outtextxy(xcor, 166, "5");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(471, 255, 529, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 255, 529, 283);
				outtextxy(495, 267, "5");
				xcor = xcor + 10;
				j++;
			}


			if (x > 529 && x < 588 && y > 254 && y < 284) {
				x = '6';
				b[j] = x;
				outtextxy(xcor, 166, "6");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(531, 255, 587, 283);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 255, 587, 283);
				outtextxy(555, 267, "6");
				xcor = xcor + 10;
				j++;
			}



			if (x > 412 && x < 470 && y > 296 && y < 326) {
				x = '7';
				b[j] = x;
				outtextxy(xcor, 166, "7");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(413, 297, 469, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 297, 469, 325);
				outtextxy(435, 307, "7");
				xcor = xcor + 10;
				j++;
			}




			if (x > 470 && x < 529 && y > 296 && y < 326) {
				x = '8';
				b[j] = x;
				outtextxy(xcor, 166, "8");
				moveto(xcor + 10, gety());
				setfillstyle(11, 4); //button glow
				bar(471, 297, 529, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(471, 297, 529, 325);
				outtextxy(495, 307, "8");
				xcor = xcor + 10;
				j++;
			}



			if (x > 529 && x < 588 && y > 296 && y < 326) {
				x = '9';
				b[j] = x;
				outtextxy(xcor, 166, "9");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(531, 297, 587, 325);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 297, 587, 325);
				outtextxy(555, 307, "9");
				xcor = xcor + 10;
				j++;
			}


			if (x > 529 && x < 588 && y > 338 && y < 368) {
				x = '.';
				b[j] = x;
				outtextxy(xcor, 166, ".");
				moveto(xcor + 10, 166);
				setfillstyle(11, 4); //button glow
				bar(531, 339, 587, 367);
				delay(BCD);
				setfillstyle(11, 0);
				bar(531, 339, 587, 367);
				outtextxy(555, 349, ".");
				xcor = xcor + 10;
				j++;
			}


			if (x > 412 && x < 470 && y > 338 && y < 368) {
				setfillstyle(11, 4); //button glow
				bar(413, 339, 469, 367);
				delay(BCD);
				outtextxy(435, 349, "C"); //Accepting the operand
				setfillstyle(11, 0);
				bar(413, 339, 469, 367);
				menu();
				n2 = strtod(b, & endptr); //strnod converts string to double!
				goto f;
			}


			if (x > 512 && x < 588 && y > 380 && y < 428) {
				setfillstyle(11, 4); //button glow
				bar(513, 381, 587, 427);
				delay(BCD);
				setfillstyle(11, 0);
				bar(513, 381, 587, 427);
				outtextxy(530, 400, "O F F"); /*Put off the calculator and */
				delay(BCD); /*return to the main menu*/
				goto ex;
			}



		} else {
			gotoxy(5, 3);
			printf("RELEASED");
		}

		gotoxy(20, 3);
		(button & 2) == 2 ? printf(" PRESSED ") : printf("RELEASED");

		gotoxy(65, 2); /*Showing the co-ordinates of the*/
		printf("x=%03d y=%03d", x, y); /*current position of the cursor*/


	}



	f: menu();
	gotoxy(1, 2);
	printf("Left Button");
	gotoxy(15, 2);
	printf("Right Button");
	gotoxy(2, 9);
	printf("First operand=%lf", n1); /*Displaying the first and the second*/
	gotoxy(1, 10); /*operands entered by the user*/
	printf("\n\t\tSecond operand=%lf", n2);

	outtextxy(400, 126, "Enter the operator...");
	while (!kbhit()) { //Reading the operator
		getmousepos( & button, & x, & y);

		gotoxy(5, 3);
		if ((button & 1) == 1) {
			printf(" PRESSED ");


			if (x > 412 && x < 455 && y > 380 && y < 404) {
				setcolor(9);
				rectangle(173, 375, 221, 393);
				line(197, 375, 197, 393);
				setcolor(WHITE);
				outtextxy(10, 380, "Another Calculation?");
				outtextxy(179, 380, "Y");
				outtextxy(203, 380, "N");
				setfillstyle(11, 4); //button glow
				bar(413, 381, 454, 403);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 381, 454, 403);
				outtextxy(430, 388, "+");
				settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
				outtextxy(10, 270, "The answer is...");
				settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
				gotoxy(2, 20);
				printf("%lf + %lf = %lf", n1, n2, n1 + n2);
				fprintf(fp, "%lf + %lf = %lf\n", n1, n2, n1 + n2);
			}




			if (x > 455 && x < 498 && y > 380 && y < 404) {
				setcolor(9);
				rectangle(173, 375, 221, 393);
				line(197, 375, 197, 393);
				setcolor(WHITE);
				outtextxy(10, 380, "Another Calculation?");
				outtextxy(179, 380, "Y");
				outtextxy(203, 380, "N");
				setfillstyle(11, 4); //button glow
				bar(456, 381, 497, 403);
				delay(BCD);
				setfillstyle(11, 0);
				bar(456, 381, 497, 403);
				outtextxy(472, 388, "-");
				settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
				outtextxy(10, 270, "The answer is...");
				settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
				gotoxy(2, 20);
				printf("%lf - %lf = %lf", n1, n2, n1 - n2);
				fprintf(fp, "%lf - %lf = %lf\n", n1, n2, n1 - n2);
			}




			if (x > 412 && x < 455 && y > 404 && y < 428) {
				setcolor(9);
				rectangle(173, 375, 221, 393);
				line(197, 375, 197, 393);
				setcolor(WHITE);
				outtextxy(10, 380, "Another Calculation?");
				outtextxy(179, 380, "Y");
				outtextxy(203, 380, "N");
				setfillstyle(11, 4); //button glow
				bar(413, 405, 454, 427);
				delay(BCD);
				setfillstyle(11, 0);
				bar(413, 405, 454, 427);
				outtextxy(430, 412, "*");
				settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
				outtextxy(10, 270, "The answer is...");
				settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
				gotoxy(2, 20);
				printf("%lf * %lf = %lf", n1, n2, n1 * n2);
				fprintf(fp, "%lf * %lf = %lf\n", n1, n2, n1 * n2);
			}




			if (x > 455 && x < 498 && y > 404 && y < 428) {
				setcolor(9);
				rectangle(173, 375, 221, 393);
				line(197, 375, 197, 393);
				setcolor(WHITE);
				outtextxy(10, 380, "Another Calculation?");
				outtextxy(179, 380, "Y");
				outtextxy(203, 380, "N");
				setfillstyle(11, 4); //button glow
				bar(456, 405, 497, 427);
				delay(BCD);
				setfillstyle(11, 0);
				bar(456, 405, 497, 427);
				outtextxy(472, 412, "/");
				settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
				outtextxy(10, 270, "The answer is...");
				settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
				gotoxy(2, 20);
				printf("%lf / %lf = %lf", n1, n2, n1 / n2);
				fprintf(fp, "%lf / %lf = %lf\n", n1, n2, n1 / n2);
			}


			if (x > 512 && x < 588 && y > 380 && y < 428) {
				setfillstyle(11, 4);
				bar(513, 381, 587, 427);
				delay(BCD);
				setfillstyle(11, 0); //button glow
				bar(513, 381, 587, 427);
				outtextxy(530, 400, "O F F");
				delay(BCD);
				goto ex;
			}

			if (x > 173 && x < 196 && y > 375 && y < 393) {
				setfillstyle(11, 4);
				bar(173, 375, 196, 393);
				delay(BCD);
				setfillstyle(11, 0); //button glow
				bar(173, 375, 196, 393);
				setcolor(9);
				rectangle(173, 375, 196, 393);
				setcolor(WHITE);
				outtextxy(179, 380, "Y");
				i = 0;
				j = 0;
				goto bg;
			}


			if (x > 198 && x < 220 && y > 376 && y < 393) {
				setfillstyle(11, 4);
				bar(198, 376, 220, 393);
				delay(BCD);
				setfillstyle(11, 0); //button glow
				bar(198, 376, 220, 3936);
				setcolor(9);
				rectangle(198, 376, 220, 393);
				setcolor(WHITE);
				outtextxy(203, 380, "N");
				goto ex;
			}

		} else {
			gotoxy(5, 3);
			printf("RELEASED");
		}

		gotoxy(20, 3);
		(button & 2) == 2 ? printf(" PRESSED ") : printf("RELEASED");

		gotoxy(65, 2);
		printf("x=%03d y=%03d", x, y);


	}
	getch();

	ex: setfillstyle(11, 4); //button glow
	bar(513, 381, 587, 427);
	delay(BCD);
	setfillstyle(11, 0);
	bar(513, 381, 587, 427);
	outtextxy(530, 400, "O F F");
	delay(BCD);
	hidemouseptr();
	return 1;
}



help() {
	cleardevice();

	//instructions
	setcolor(7);
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
	outtextxy(290, 10, "HELP");
	setcolor(10);
	settextstyle(SMALL_FONT, HORIZ_DIR, 6);
	outtextxy(10, 30, "Here are the controls...");
	outtextxy(10, 49, "Addition      - 'a'");
	outtextxy(10, 68, "Subtraction   - 's'");
	outtextxy(10, 87, "Multiplication- 'm'");
	outtextxy(10, 106, "Division     - 'd'");
	outtextxy(10, 140, "1.Enter the above only when asked, or they will not work.");
	outtextxy(10, 170, "2.For calculation,you have to anyhow enter 8-digit number.");
	outtextxy(10, 185, "  e.g. for '20' enter either '00000020' or '20.000000'");
	outtextxy(10, 215, "3.You need to store the number if it's NOT of 8 digits.");
	outtextxy(10, 230, "  Number can be stored by pressing 'n' as and when required");
	outtextxy(10, 245, "  If we enter a number of 8 digits,it gets stored automatically.");
	outtextxy(10, 275, "4.While working in Menu 2 i.e Mouse interfacing,you have to");
	outtextxy(10, 290, "  click 'c' after entering each operand to accept the same and");
	outtextxy(10, 305, "  then click on the desired operator.");
	outtextxy(10, 335, "5.Please make sure that there is no key stroke while working");
	outtextxy(10, 350, "  with mouse interfacing.If you press any key while working with");
	outtextxy(10, 365, "  mouse interfacing, the mouse cursor stops functioning.");
	outtextxy(10, 420, "*** Press any key to return to main menu...");
	getch();
	return 0;
}


about() {
	int midx;
	midx = getmaxx() / 2;
	cleardevice();
	setlinestyle(0, 1, 1);
	setcolor(7);
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
	outtextxy(midx - 30, 90, "ABOUT");
	setcolor(10);
	settextstyle(SMALL_FONT, HORIZ_DIR, 6);
	outtextxy(40, 130, "Project name:-'THE CALCULATOR' ");
	outtextxy(140, 200, "Project fabricators:");
	outtextxy(40, 230, "Ankur Dukare");
	outtextxy(280, 230, "Chinmay Murugkar");
	outtextxy(40, 380, "*** Press any key to return to main menu...");
	getch();
	return 1;
}


//Function code for initialising the mouse
initmouse() {
	i.x.ax = 0; //ax=0 resets mouse and gets status
	int86(0x33, & i, & o);
	return (o.x.ax);
}

void showmouseptr() {
	i.x.ax = 1; //ax=1 shows the mouse pointer
	int86(0x33, & i, & o);
}


void restrictmouseptr(int x1, int y1, int x2, int y2) {
	i.x.ax = 7; //ax=7 sets horizontal limits for mouse pointer
	i.x.cx = x1; //cx=minimum x co-ordinate
	i.x.dx = x2; //dx=maximum x co-ordinate
	int86(0x33, & i, & o);
	i.x.ax = 8; //ax=8 sets vertical limits for the mouse pointer
	i.x.cx = y1; //cx=minimum y co-ordinate
	i.x.dx = y2; //dx=maximum y co-ordinate
	int86(0x33, & i, & o);
}


void getmousepos(int * button, int * x, int * y) {
	i.x.ax = 3; //ax=3 gets the mouse position and button status
	int86(0x33, & i, & o); * button = o.x.bx; //bx=mouse button status
	*
	x = o.x.cx; //cx=x co-ordinate
	*
	y = o.x.dx; //dx=y co-ordunate
}


void hidemouseptr() {
	i.x.ax = 2; //ax=2 hides the mouse pointer
	int86(0x33, & i, & o);
}
