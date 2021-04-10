#include<iostream>
#include<thread>
#include<conio.h>		 //to use _getch
#include<cstdlib>		 //to use rand
#include<ctime>			 //to use time() in srand
#include<unordered_map>  //to make invert and upsidedown function easily   

#define up 72
#define down 80
#define left 75
#define right 77

using namespace std;

short TestMode = 0;		//it will only allow the last shape to print ,to test it---------------------------------------------------------------------------------------------

const short length = 18; //this controls the dimension of game
const short breath = length + (int)(length * 0.2);
static bool gameCONTINUITY = 1;		//this keeps track of state of game ,ended or started and going on
int score = -1;		//as name suggest bro ; )
auto normal_SPEED = 0.4s, SPEED = 0.4s;		//SPEED will change while execution  //lower the speed faster the game  //normal_SPEED will keep travk of default speed value
char a[length][breath];		//main array that containes the main game to display
const short max_shapes_available = 10;		//total shapes possible
short available_shapes = 3;		//this should always be smaller than max_shapes_available for proper functioning ,also initialize new shapes if increasing the counter.
short container[max_shapes_available][4];   //initialize shapes first board position ,rest is taken care off..
char name[17];
char falling_body = '0';
char fallen_body = 'A';		//thi will be updated every time a new piece, in movedown()
short checkHORIZONTALLY = 0;		 //this will invoke while(checkHORIZONTALLY) in run() from any point in game (most casually from movedown() cuz there new fallen pieces are updated)
short PAUSE = 0;	//to add pause and play feature in game
short random;		// to add invert and upside down feature in game
static short now = up;		//this is used in index or menu at start

static short shape_x1 = 1;
static short shape_y1 = breath / 2;
static short shape_x2 = 2;
static short shape_y2 = breath / 2;
static short shape_x3 = 3;
static short shape_y3 = breath / 2;
static short shape_x4 = 1;
static short shape_y4 = breath / 2;

inline int input();
void initialize();
inline void newshape();
void display();		//it does more than just display, cuz of optimization
void clearXYspace();	//this clears the previous shape address in order to isert new one 
int calculateX(int a);
int calculateY(int a);
void setXYshape();		//set shape position according to XY ,next it will use these to print new shape at there new position
void setXYspace(int flag);	//set XY space on a to falling body(0) or fallen body(1) 
void movedown();
void moveleft();
void moveright();
//next one is used to find an element in a hash_map
template <class type1, class type2>
bool find(type1 a, type2 b);
void invert();
void upsidedown();
void checkHorizontally();	//this fun() clears the row and bring all the pieces down if a row is filled horizontally...
void displayAvailableShape(int x);	//this will show available shapes from container inputing shape no. as 'x'
void insertNEWshape();	//this function will input new shapes from user
void run();			//this updates the game as per user input

inline int input()
{
	int ch;
	ch = _getch();
	if (ch == 224)
		return _getch();
	else return ch;
}

void initialize()
{
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < breath - 1; j++)
		{
			if (i == length - 1 || j == 0 || j == breath - 2)
				a[i][j] = '#';
			else
				a[i][j] = ' ';
		}
		a[i][breath - 1] = '\0';
	}
	//
	{
		cout << "\n    -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-    ITS  < -:" << name;
		cout << ":- >  YOU BIT** " << "    -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-\n\n";
		for (int i = 0; i < length; i++)
			printf("					%s\n", &a[i]);
		cout << "\n\n\t\t'u' :- upside down   'i' :- invert   'p' :- pause(any key to continue later...)";
		cout << "\n\n\t\t\t\t\t    SCORE :- " << score;
	}
	//
	//it is important to arrange the index array in increasing order to be able to set x values properly...
	//-0-
	//-0-
	//-0-
	container[0][0] = 2;
	container[0][1] = 2;
	container[0][2] = 5;
	container[0][3] = 8;
	//-0-
	//00-
	//-0-
	container[1][0] = 2;
	container[1][1] = 4;
	container[1][2] = 5;
	container[1][3] = 8;
	//-0-
	//-0-
	//00-
	container[2][0] = 2;
	container[2][1] = 5;
	container[2][2] = 7;
	container[2][3] = 8;
	//we just initialized 3 shapes, thus there are 3 default shapes.
	//available_shapes = 3;
	// this calls the new shape to start a game with new piece.
	newshape();
}

inline void newshape()
{
	srand(time(0));
	random = (rand() % available_shapes);
	if (random == available_shapes)
	{
		random = 0;
	}
	setXYshape();
	setXYspace(0);
}

void display()
{
	while (gameCONTINUITY)
	{
		while (gameCONTINUITY && !PAUSE)
		{
			system("CLS");	//clear screen
			movedown();
			//modify the shape before
			cout << "\n    -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-    ITS  < -:" << name;
			cout << ":- >  YOU BIT** " << "    -|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-\n\n";
			for (int i = 0; i < length; i++)
				printf("					%s\n", &a[i]);
			cout << "\n\n\t\t'u' :- upside down   'i' :- invert   'p' :- pause(any key to continue later...)";
			cout << "\n\n\t\t\t\t\t    SCORE :- " << score;
			this_thread::sleep_for(SPEED);
		}
	}
}

void clearXYspace()
{
	a[shape_x1][shape_y1] = ' ';
	a[shape_x2][shape_y2] = ' ';
	a[shape_x3][shape_y3] = ' ';
	a[shape_x4][shape_y4] = ' ';
}

int calculateX(int a)
{
	if (a % 3 == 0)
		return (a / 3);
	else
		return (a / 3) + 1;
}

int calculateY(int a)
{
	if (a % 3 == 0)
		return (breath / 2) + 1;
	else
		return (breath / 2) + (a % 3) - 2;
}

void setXYshape()
{
	if (TestMode)
		random = available_shapes - 1;
	//to calculate x value
	shape_x1 = calculateX(container[random][0]);
	shape_x2 = calculateX(container[random][1]);
	shape_x3 = calculateX(container[random][2]);
	shape_x4 = calculateX(container[random][3]);
	//to calculate y value
	shape_y1 = calculateY(container[random][0]);
	shape_y2 = calculateY(container[random][1]);
	shape_y3 = calculateY(container[random][2]);
	shape_y4 = calculateY(container[random][3]);
}

void setXYspace(int flag) //if flag==0 then places are set for falling body ,if flag==1 then places set for fallen body
{
	if (flag)
	{
		a[shape_x1][shape_y1] = fallen_body;
		a[shape_x2][shape_y2] = fallen_body;
		a[shape_x3][shape_y3] = fallen_body;
		a[shape_x4][shape_y4] = fallen_body;
	}
	else
	{
		a[shape_x1][shape_y1] = falling_body;
		a[shape_x2][shape_y2] = falling_body;
		a[shape_x3][shape_y3] = falling_body;
		a[shape_x4][shape_y4] = falling_body;
	}
}

void movedown()//bool speedreduce = 0)
{
	if ((a[shape_x1 + 1][shape_y1] == falling_body || a[shape_x1 + 1][shape_y1] == ' ') && (a[shape_x2 + 1][shape_y2] == falling_body || a[shape_x2 + 1][shape_y2] == ' ') && (a[shape_x3 + 1][shape_y3] == falling_body || a[shape_x3 + 1][shape_y3] == ' ') && (a[shape_x4 + 1][shape_y4] == falling_body || a[shape_x4 + 1][shape_y4] == ' '))
	{
		clearXYspace();
		shape_x1 += 1; shape_x2 += 1; shape_x3 += 1; shape_x4 += 1;
		setXYspace(0);
		return;
	}
	else
	{
		setXYspace(1);
		//increment of fallen_body char
		if (fallen_body < 'Z')
			fallen_body++;
		else
			fallen_body = 'A';
		//this will keep the character representation of fallen body within the range
		score += 10;	//it increases the score just when the body shape becomes permanent..
		//we have to check whether this fallen piece is at top of board, if it is : then its game over..
		if (shape_x1 == 2 || shape_x2 == 2 || shape_x3 == 2 || shape_x4 == 2)
		{
			gameCONTINUITY = 0;
			return;
		}
		//also check for all the permanent shapes whether each vertical line is filled or not, if it is then clear them horizontally..
		checkHORIZONTALLY = (shape_x1 * 10) + (shape_x2 * 10) + (shape_x3 * 10) + (shape_x4 * 10);
		//this thread just envokes if(checkHORIZONTALLY) in run(), cuz this thread will become heavy and it contain display(),thus it would give a laggy experience..
		//next line will call new shape so after making the fallen pieces permanent ,they have to get new shapes with default new position (at the top line of board)
		newshape();
	}
}

void moveleft()
{
	if (a[shape_x1][shape_y1 - 1] != '#' && a[shape_x2][shape_y2 - 1] != '#' && a[shape_x3][shape_y3 - 1] != '#' && a[shape_x4][shape_y4 - 1] != '#' && (a[shape_x1][shape_y1 - 1] == falling_body || a[shape_x1][shape_y1 - 1] == ' ') && (a[shape_x2][shape_y2 - 1] == falling_body || a[shape_x2][shape_y2 - 1] == ' ') && (a[shape_x3][shape_y3 - 1] == falling_body || a[shape_x3][shape_y3 - 1] == ' ') && (a[shape_x4][shape_y4 - 1] == falling_body || a[shape_x4][shape_y4 - 1] == ' '))
	{
		clearXYspace();
		shape_y1 -= 1; shape_y2 -= 1; shape_y3 -= 1; shape_y4 -= 1;
		setXYspace(0);
		return;
	}
}

void moveright()
{
	if (a[shape_x1][shape_y1 + 1] != '#' && a[shape_x2][shape_y2 + 1] != '#' && a[shape_x3][shape_y3 + 1] != '#' && a[shape_x4][shape_y4 + 1] != '#' && (a[shape_x1][shape_y1 + 1] == falling_body || a[shape_x1][shape_y1 + 1] == ' ') && (a[shape_x2][shape_y2 + 1] == falling_body || a[shape_x2][shape_y2 + 1] == ' ') && (a[shape_x3][shape_y3 + 1] == falling_body || a[shape_x3][shape_y3 + 1] == ' ') && (a[shape_x4][shape_y4 + 1] == falling_body || a[shape_x4][shape_y4 + 1] == ' '))
	{
		clearXYspace();
		shape_y1 += 1; shape_y2 += 1; shape_y3 += 1; shape_y4 += 1;
		setXYspace(0);
		return;
	}
}

template <class type1, class type2>
bool find(type1 a, type2 b)
{
	if (a.find(b) == a.end())
		return 0;
	else
	{
		return 1;
	}
}

void invert()
{
	clearXYspace();
	unordered_map<int, int> a;
	for (int i = 0; i < 4; i++)
	{
		a[container[random][i]] = 0;
	}
	for (int i = 0; i < 4; i++)
	{
		if (!find(a, container[random][i] + 2) && (container[random][i] == 1 || container[random][i] == 4 || container[random][i] == 7))
		{
			container[random][i] += 2; continue;
		}
		if (!find(a, container[random][i] - 2) && (container[random][i] == 3 || container[random][i] == 6 || container[random][i] == 9))
			container[random][i] -= 2;
	}
	setXYshape();
	setXYspace(0);
}

void upsidedown()
{
	clearXYspace();
	unordered_map<int, int> a;
	for (int i = 0; i < 4; i++)
	{
		a[container[random][i]] = 0;
	}
	for (int i = 0; i < 4; i++)
	{
		if (!find(a, container[random][i] + 6) && (container[random][i] == 1 || container[random][i] == 2 || container[random][i] == 3))
		{
			container[random][i] += 6; continue;
		}
		if (!find(a, container[random][i] - 6) && (container[random][i] == 7 || container[random][i] == 8 || container[random][i] == 9))
			container[random][i] -= 6;
	}
	setXYshape();
	setXYspace(0);
}

void checkHorizontally()
{
	short flag = 0, i, j;
	while (gameCONTINUITY)
	{
		for (i = 0; i < length; i++)
		{
			for (j = 0; j < breath; j++)
			{
				if (a[i][j] != ' ' && a[i][j] != falling_body && a[i][j] != '#')
					flag++;
			}
			if (flag == breath - 2)
			{
				for (j = 1; j < breath - 2; j++)
				{
					a[i][j] = '-';
				}
				this_thread::sleep_for(1s);
				//logic to bring all the pieces down
				for (int x = i; x > 1; x--)
				{
					for (j = 1; j < breath - 2; j++)
					{
						a[x][j] = a[x - 1][j];
					}
				}
			}
			flag = 0;
		}
	}
}

void displayAvailableShape(int random)		//we usually use random to denote which shape in container
{
	cout << "shape " << random + 1 << " :-\n  ";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (((i * 3) + j + 1) == container[random][0] || ((i * 3) + j + 1) == container[random][1] || ((i * 3) + j + 1) == container[random][2] || ((i * 3) + j + 1) == container[random][3])
			{
				cout << fallen_body;
			}
			else
				cout << '-';
		}
		cout << "\n  ";
	}
	cout << "\n";
}

void insertNEWshape()
{
	system("CLS");
	short input, check = 0, val = 1024, blocks = 0;
	if (available_shapes == max_shapes_available)
	{
		cout << " \n\n\t\tNO SHAPES CAN BE ADDED BECAUSE NUMBER OF SHAPES HAS REACHED ITS LIMIT...";
		check = _getch();
		return;
	}
	cout << "\t\t\t\t\t\t  AVAILABLE SHAPES :- \n";
	for (int i = 0; i < available_shapes; i++)
	{
		displayAvailableShape(i);
	}
	cout << "SHAPE MAP :- \n";
	for (int i = 0; i < 9; i++)
	{
		if (i % 3 == 0 && i != 0)
		{
			cout << "\n";
		}
		cout << "  " << i + 1;
	}
	cout << "\n EXAMPLE INPUT (without space only) :- (shape 1)258 , (shape 2)2458 , (shape 3)2578\n\t\t\t\t      --they should be in increasing order...";
	cout << "\n ENTER NEW SHAPE (press 0 to exit):- ";
	cin >> input;
	if (!input)
	{
		cout << "exit";
		return;
	}
	for (int i = 0; input; i++)
	{
		if (val > (input % 10))
			val = input % 10;
		else
		{
			cout << "input was not in assending or increasing order";
			check = _getch();
			break;
		}
		input /= 10;
		blocks++;
		container[available_shapes][i] = (short)val;
	}
	if (check)
	{
		cout << "exit";
		return;
	}
	if (blocks < 2)
	{
		container[available_shapes][1] = (short)container[available_shapes][0]; blocks++;
	}
	if (blocks < 3)
	{
		container[available_shapes][2] = (short)container[available_shapes][1]; blocks++;
	}
	if (blocks < 4)
	{
		container[available_shapes][3] = (short)container[available_shapes][2]; blocks++;
	}
	cout << "\n YOUR NEW SHAPE LOOKS LIKE :- \n ";
	displayAvailableShape(available_shapes);
	cout << "DO YOU WANT TO CONTINUE...('0' for 'no'  or anything else for 'yes')  ";
	available_shapes++;


	cin >> input;
	clearXYspace();
	setXYshape();
	setXYspace(0);
}

void run()
{
	static short i, check, counter;
	char ch;
	counter = 0;
	while (gameCONTINUITY)
	{
		int INPUT = input();
		switch (INPUT)
		{
		case 75:	moveleft();	 PAUSE = 0;								break;
		case 77:	moveright(); PAUSE = 0;								break;
		case 'p':	PAUSE == 0 ? PAUSE = 1 : PAUSE = 0;					break;
		case 'i':	invert();	 PAUSE = 0;								break;
		case 'u':	upsidedown(); PAUSE = 0;								break;
		case 80:	SPEED == 0.06s ? SPEED = normal_SPEED : SPEED = 0.06s;	PAUSE = 0;			break;
		default:	PAUSE = 0;
		}
	}
}
void constantINPUT()
{
	while (gameCONTINUITY)
		now = input();
}

int main()
{
	initialize();	//it should be initialize before so, that new shape can be added with all the default shapes updated
	short i = 1, previousSELECTION = 0;		//it will keep track of what option was chosen	   ;if 0 then NEW GAME	  ;if 1 then insertNEWshape();
	thread constantINPUT_THREAD(constantINPUT);
GATA:
	now = up;
	while (gameCONTINUITY)
	{
		system("CLS");
		if (now == up)
		{
			if (i == 1)
			{
				cout << "\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t-----------------------  NEW GAME  -----------------------";
				cout << "\n\t                    \t\t\t       ADD NEW SHAPE";
				cout << "\n\n\n\n\n\n\n\n\npress up and down arrow keys to navigate and any other key to select that option...";
				i = 2;
			}
			else
			{
				cout << "\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t#######################  NEW GAME  #######################";
				cout << "\n\t                    \t\t\t       ADD NEW SHAPE";
				cout << "\n\n\n\n\n\n\n\n\npress up and down arrow keys to navigate and any other key to select that option...";
				i = 1;
			}
			previousSELECTION = 0;
		}
		else if (now == down)
		{
			if (i == 1)
			{
				cout << "\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t                         NEW GAME";
				cout << "\n\t\t\t\t---------------------  ADD NEW SHAPE  --------------------";
				cout << "\n\n\n\n\n\n\n\n\npress up and down arrow keys to navigate and any other key to select that option...";
				i = 2;
			}
			else
			{
				cout << "\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t                         NEW GAME";
				cout << "\n\t\t\t\t#####################  ADD NEW SHAPE  ####################";
				cout << "\n\n\n\n\n\n\n\n\npress up and down arrow keys to navigate and any other key to select that option...";
				i = 1;
			}
			previousSELECTION = 1;
		}
		else
		{
			if (previousSELECTION)
			{
				insertNEWshape();
				goto GATA;
			}
			else
			{
				gameCONTINUITY = 0;
				constantINPUT_THREAD.join();
				break;
			}
		}
	}
	system("CLS");
	gameCONTINUITY = 1;
	while (gameCONTINUITY)
	{
		thread displayTHREAD(display); //display part and continuos moving down of parts
		thread runTHREAD(run); //this continuously takes input and update direction once runs the game
		thread checkHorizontallyTHREAD(checkHorizontally);
		checkHorizontallyTHREAD.join();
		runTHREAD.join();
		displayTHREAD.join();
	}
	char caps = 'A';
	char low = 'a';
	for (int i = 0; i < 25; i++)
	{
		system("CLS");
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t" << caps << (char)(1 + caps) << (char)(2 + caps) << (char)(3 + caps) << " " << (char)(2 + caps) << (char)(1 + caps) << " :- ";
		cout << (char)(caps + 1) << low << (char)(low + 2) << (char)(low + 3) << low << (char)(low + 1) << (char)(low + 2) << " " << caps << (char)(low + 2) << low << (char)(low + 3) << (char)(low + 1);
		if (caps == 'w')
			caps = 'A';
		else
			caps++;
		if (low == 'w')
			low = 'a';
		else
			low++;
	}
	system("CLS");
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\tMADE BY :- Bharat Patel\n\n\n\n\n\n\n\n\n\n";
}
