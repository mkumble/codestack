

#include<stdio.h>
#include<conio.h>
#include<process.h>
#include "c:\tc\hash\student.cpp"
#include <iostream.h>
#include<string.h>
#include"c:\tc\hash\draw.cpp"
#include<graphics.h>
#include<ctype.h>
#include<math.h>
#include<stdlib.h>
#include<dos.h>


Directory Dir (4);
int collapsetrue=0,doublesizetrue=0;
void deleterecord (char *myfile,char *m);
void deleterecord1 (char *myfile,char *m);

char Branch[5][4] = {	"ISE",
			"CSE",
			"ECE",
			"IT"  };


char *toupp(void)
{
     char *string = " ";// "this is a string";

     cin >>string;
     for (int i=0; i<strlen(string); i++)
     {
	string[i] = toupper(string[i]);
     }
     return string;
}

char *toupp1(char * string)
{
     for (int i=0; i<strlen(string); i++)
     {
	string[i] = toupper(string[i]);
     }
     return string;
}


void append(char *myfile)
{
     int x=17,y=4;

     Student s;
     char *Branch1,*branch;
     clrscr();

     box(2,1,77,25);
     gotoxy(4,y-1);
     cout<<"                                                                ";
     gotoxy(x,y);


     gotoxy(x,y+1);
     cout <<"            ENTER THE  DATA FOR THE STUDENT :          ";
     gotoxy(x,y+2);


     gotoxy(x,y+4);
     cout << "   REG-NO      : ";
     gotoxy(x,y+6);
     cout << "   LAST NAME   : ";
     gotoxy(x,y+8);
     cout << "   FIRST NAME  : ";
     gotoxy(x,y+10);
     cout << "   ADDRESS     : ";
     gotoxy(x,y+12);
     cout << "   SEMESTER    : ";
     gotoxy(x,y+14);
     cout << "   BRANCH      : ";
     gotoxy(x,y+16);
     cout << "   COLLEGE     : ";
     gotoxy(x,y+18);


     gotoxy(4,y-1);
     cout<<"                                                                ";

     gotoxy(x+20,y+4);
     strcpy(s.URN , toupp());

     while(strlen(s.URN) != 10 )
     {
	 gotoxy(x+18,y+20);
	 textcolor(WHITE+BLINK);
	 cprintf("URN MUST BE 10 CHARACTERS");

	 gotoxy(x+20,y+4);
	 cout <<"                                 ";
	 gotoxy(x+20,y+4);
	 strcpy(s.URN,toupp());
	 gotoxy(x+18,y+20);
	 cout <<"                         ";
	 textcolor(15);

     }
     int res = Dir.Search(s.URN);
     if(res!=-1) {
	 gotoxy(x+18,y+20);
	 textcolor(WHITE+BLINK);
	 cprintf("REG-NO ALREADY EXISTS!");
	 textcolor(15);
	 getch();
	  return;
     }

     fflush(stdin);
     fflush(stdout);
     gotoxy(x+20,y+6);
     gets(s.Lname);

     gotoxy(x+20,y+8);
     gets(s.Fname);

     gotoxy(x+20,y+10);
     gets(s.Address);

     gotoxy(x+20,y+12);
     gets(s.Semester);

     int s1 = atoi(s.Semester);

     while( s1 <= 0 || s1 > 8 || s1=='\0' )
     {
	 gotoxy(x+18,y+20);
	 textcolor(WHITE+BLINK);
	 cprintf("ENTER SEMESTER 1 - 8 !");
	 gotoxy(x+20, y+12);
	 cout<<"                                ";
	 gotoxy(x+20,y+12);
	 gets(s.Semester);
	 s1 = atoi(s.Semester);
	 gotoxy(x+18,y+20);
	 cout <<"                                 ";
	 textcolor(15);
     }


     while(1) {
	 gotoxy(x+20,y+14);
	 textcolor(15);
	 gets(Branch1);
	 strcpy(s.Branch,toupp1(Branch1));

	 if(!((strcmp(s.Branch,"CSE"))&&(strcmp(s.Branch,"ISE"))&&(strcmp(s.Branch,"ECE"))&&(strcmp(s.Branch,"IT"))&&(strcmp(s.Branch,'\0'))))
	 {
	    gotoxy(x+18,y+20);
	 /*if(strcmp(s.Branch,"ISE")==0 || strcmp(s.Branch,"CSE")==0
		 || strcmp(s.Branch,"ELE")==0|| strcmp(s.Branch,"MECH")==0 || strcmp(s.Branch,"TELE")==0  || strcmp(s.Branch,"ARCH")==0 || strcmp(s.Branch,"IP")==0   ||  strcmp(s.Branch,"IT")==0 || strcmp(s.Branch,"ELEC")==0 || strcmp(s.Branch,"CHEM")==0)*/

	     cout<<"                       ";
	     break;
	 }
	 else
	 {

	     gotoxy(x+18,y+20);
	     textcolor(WHITE+BLINK);
	     cprintf("INVALID BRANCH!!");
	 }
	 gotoxy(x+20,y+14);
	 cout<<"                               ";
     }
     gotoxy(x+20,y+16);
     gets(s.College);
     /*while(s.College=="")
     {
	gotoxy(x+18,y+20);
	textcolor(WHITE+BLINK);
	cprintf("INCOMPLETE DATA");
	gotoxy(x+20, y+16);
	cout<<"                                ";
	gotoxy(x+20,y+16);
	gets(s.College);
	gotoxy(x+18,y+20);
	cout <<"                                 ";
	textcolor(15);
     }
       */
     gotoxy(32,22);
     textcolor(WHITE + BLINK);
     cprintf("UPDATED SUCCESSFULLY");
     textcolor(WHITE);
     int recaddr=s.Append(myfile);
     Dir.Insert(s.URN,recaddr);
     getch();
     if(doublesizetrue)
     {
		clrscr();
     cout<<"The Directory Has Doubled\n";
     doublesizetrue=0;
     Dir.Print(cout);
     return;
    }
}



void modify(char * myfile)
{
   Student s;
   cout << "\n\t\t ENTER THE REG-NO OF THE RECORD TO BE MODIFIED : ";
  strcpy(s.URN,toupp());
  int addr=Dir.Search(s.URN);
  if(addr==-1){
  gotoxy(20,15);
      textcolor(WHITE + BLINK);
      cprintf("THE RECORD DOES NOT EXIST");
      textcolor(WHITE);
   getch();
   return;
  }
  clrscr();
  cout << "\n\t\tTHE CURRENT CONTENTS OF THE RECORD IS : ";
  DelimFieldBuffer :: SetDefaultDelim('|');
  DelimFieldBuffer Buff;
  ifstream file(myfile,ios::in);

  Buff.DRead(file,addr);
  s.Unpack(Buff);
  s.Print(cout);
  getch();
  clrscr();

  cout <<"\n";

  cout 	<<"    1. FNAME"<<endl
	<<"    2. LNAME"<<endl
	<<"    3. ADDRESS"<<endl
	<<"    4. SEMESTER"<<endl
	<<"    5. BRANCH"<<endl
	<<"    6. COLLEGE"<<endl
	<<"    0. IGNORE"<<endl;
  fflush(stdin);
  cout <<endl<< "\n\t\tENTER THE FIELD NUMBER TO BE MODIFIED : ";
  int choice;
  cin >> choice;
  char oldurn[11];
  strcpy(oldurn,s.URN);
  cout <<endl;

  switch(choice){

    case 0:return;


    case 1: cout << " \n\t\t   Enter the new First Name: ";
	  gets(s.Fname);
	  break;

    case 2: cout << "\n\t\t    Enter the new Last Name: ";
	  gets(s.Lname);
	  break;

    case 3: cout << "\n\t\t    Enter the new Address: ";
	  gets(s.Address);
	  break;

    case 4: cout << "\n\t\t    Enter the new semester: ";
	  gets(s.Semester);
	   int x,y;
	  int s1 = atoi(s.Semester);

	    while( s1 <= 0 || s1 > 8 || s1=='\0'){
		 gotoxy(x+10,y+18);
		 textcolor(WHITE + BLINK);
		 cprintf("INVALID SEMESTER!");
		 gotoxy(x+22, y+10);
		 cout <<"                                 ";
		 gotoxy(x+22, y+10);
		 gets(s.Semester);
		 s1 = atoi(s.Semester);
		 gotoxy(x+10,y+18);
		 cout <<"                                 ";
		 textcolor(15);
	    }

	  break;

    case 5: cout << "\n\t\t    Enter the new Branch: ";

	  char *Branch1;
	    while(1) {
		gotoxy(x+22,y+10);
		cout<<"                        ";
		textcolor(15);
		gotoxy(x+22,y+10);
		gets(Branch1);
		strcpy(s.Branch,toupp1(Branch1));

		if(!((strcmp(s.Branch,'\0'))&&(strcmp(s.Branch,"CSE"))&&(strcmp(s.Branch,"ISE"))&&(strcmp(s.Branch,"ECE"))&&(strcmp(s.Branch,"IT"))))  {
			gotoxy(x+12,y+20);
			cout<<"                         ";
			gotoxy(x+12,y+20);
			break;
		getch();
		}


		 else{
		    gotoxy(x+12,y+20);
		    textcolor(WHITE+BLINK);
		    cprintf("INVALID BRANCH!!");
		}
	      }
	       break;

	     case 6 : cout << "\n\t\t    Enter the new College: ";
	  gets(s.College);
	  break;

    default:    gotoxy(x+12,y+20);
		textcolor(WHITE + BLINK);
		cprintf("INVALID CHOICE");
		textcolor(WHITE);
		return;
  }
   s.Pack(Buff);
   int newaddr=s.Append(myfile);
   fstream ofile(myfile,ios::in|ios::out);
   ofile.seekp(addr,ios::beg);
   Dir.Remove(oldurn);
   ofile.write("*",1);
   gotoxy(28,20);
     textcolor(WHITE + BLINK);
     cprintf("UPDATED SUCCESSFULLY");
     textcolor(WHITE);

   Dir.Insert(s.URN,newaddr);
   ofile.close();
}


void display(char *myfile)
{
   Student s;
   cout << "\n\t\tENTER THE REG-NO OF THE RECORD TO BE DISPLAYED : ";
   strcpy(s.URN,toupp());
   int addr;
   if((addr = Dir.Search(s.URN))==-1){
      gotoxy(20,15);
      textcolor(WHITE + BLINK);
      cprintf("RECORD NOT FOUND!");
      textcolor(WHITE);
      getch();
      return;
   }
   clrscr();
   DelimFieldBuffer :: SetDefaultDelim('|');
   DelimFieldBuffer Buff;
   fstream file(myfile,ios::in);
   Buff.DRead(file,addr);
   s.Unpack(Buff);
   clrscr();
   //gotoxy(10,3);
   //cout <<"\n\n\t\tTHE CONTENTS OF THE FILE ARE : ";
   s.Print(cout);
   box(2,1,77,25);
   file.close();
}

void deleterecord (char *myfile)
{
    Student s;
    cout << "\n\tENTER THE REG-NO OF THE RECORD TO BE DELETED : ";
    strcpy(s.URN,toupp());
    int addr=Dir.Search(s.URN);
    if(addr==-1){
      gotoxy(20,15);
      textcolor(WHITE + BLINK);

      cprintf("THE RECORD DOES NOT EXIST");
      textcolor(WHITE);
      getch();return;
    }
   fstream ofile(myfile,ios::in|ios::out);
   ofile.seekp(addr,ios::beg);
   ofile.write("*",1);
   ofile.close();
   Dir.Remove(s.URN);
   //Dir.Print(cout);
   cout << "\n\n----- RECORD DELETED SUCCESSFULLY --------- ";

   cout<<" \n\npress any key to see the directory collapse\n";
	 getch();
    if(collapsetrue)
  {         collapsetrue=0;
	cout<<"The Directory has collapsed.";
  }
   else
	cout<<"The Directory has not collapsed.";
   cout<<"\n\n\nThe Directory information...\n";
   Dir.Print(cout);
   getch();


}


void loaddir(char *myfile)
{
  Student s;
  fstream ifile(myfile,ios::in);
  DelimFieldBuffer :: SetDefaultDelim('|');
  DelimFieldBuffer Buff;
  int recaddr;
  while(1){
      if((recaddr=Buff.Read(ifile))==-1)break;
       s.Unpack(Buff);
       Dir.Insert(s.URN,recaddr);
     }
    ifile.close();
}

void updatefile(char *myfile)
{
   fstream ifile(myfile,ios::in);
   fstream ofile("demo4.bak",ios::out);
   char ch;
   while(1){
       ifile >> ch;
       if(ifile.fail()) break;
       ofile << ch;
  }
  ifile.close();
  ofile.close();
  ifile.open("demo4.bak",ios::in);
  ofile.open(myfile,ios::out);
  DelimFieldBuffer :: SetDefaultDelim('|');
  DelimFieldBuffer Buff;
  while(1){
      if(Buff.Read(ifile)==-1)break;
       if(Buff.getfirstchar()=='*')continue;
       else
	Buff.Write(ofile);
     }
    ifile.close();
    ofile.close();
}



void displayall(char *myfile)
{
   int x=22,y=7,i=1;
   DelimFieldBuffer :: SetDefaultDelim('|');
   DelimFieldBuffer Buff;
   fstream file(myfile,ios::in);
   Student s;
   while(1){
      clrscr();

      int res = Buff.Read(file);
      if(res==-1){
	 file.close();
	 return;
      }
      if( Buff.getfirstchar() == '*') continue;
	 s.Unpack(Buff);
	 box(2,1,77,25);
	 gotoxy(x+5,4);
	 textcolor(WHITE);
	 cprintf("STUDENT DATABASE DETAILS");
	 textcolor(15);
	 gotoxy(66,1); cout<<"RECORD : ";
	 textcolor(WHITE);
	 cprintf("#");cout<<i++;
	 textcolor(WHITE);
	 gotoxy(x,y);
	 cprintf("REG-NO       :");

	 gotoxy(x,y+2);
	 cprintf("FIRST NAME   :");

	 gotoxy(x,y+4);
	 cprintf("LAST NAME    :");

	 gotoxy(x,y+6);
	 cprintf("SEMESTER     :");

	 gotoxy(x,y+8);
	 cprintf("BRANCH       :");

	 gotoxy(x,y+10);
	 cprintf("COLLEGE      :");

	 gotoxy(x,y+12);
	 cprintf("ADDRESS    :");

	 textcolor(14);
	 gotoxy(x+20,y);
	 cout << s.URN;

	 gotoxy(x+20,y+2);
	 cout << s.Fname;

	 gotoxy(x+20,y+4);
	 cout << s.Lname;

	 gotoxy(x+20,y+6);
	 cout << s.Semester;

	 gotoxy(x+20,y+8);
	 cout << s.Branch;

	 gotoxy(x+20,y+10);
	 cout << s.College<<endl;

	 gotoxy(x+20,y+12);
	 cout <<s.Address<<endl;
	 getch();
   }
}

void Open()
{      // setbkcolor();
	setcolor(WHITE);
       //circle(4,200,400);
	rectangle(6,4,645,485);
	line(4,60,638,60);
	line(4,438,638,438);
	setcolor(77);
		settextjustify(CENTER_TEXT,CENTER_TEXT);
	settextstyle(4,0,5);
	outtextxy(300,27," R.V.C.E ");
	settextstyle(4,0,5);
	outtextxy(250,100, " PROJECT ON ");
	outtextxy(250,150, " FILE STRUCTURES ");
	setcolor(WHITE);
	setbkcolor(5);
	settextstyle(7,0,8);
	outtextxy(350,250," #ING");
	setcolor(15);
	settextstyle(2,0,6);
	outtextxy(350,400, "IMPLEMENTED BY ");
	settextstyle(5,0,5);
	outtextxy(150,457,"  CHITHRA H.M");
	outtextxy(460,457,"   HINA KOUSER");
	getch();
	cleardevice();
}


int main()
{
   clrscr();
   int x=20,y=2;
   int flag=0;
   int result;
   remove("hash.bkt");
   remove("hash.dir");

   result = Dir . Create ("hash");
   char *myfile="DEMO4.txt";
   loaddir(myfile);

     int gdriver =  DETECT, gmode;

	initgraph(&gdriver,&gmode,"c:\\tc\\bgi");

	Open();
	cleardevice();
	//getch();
	closegraph();

   while(1){

     getch();
     clrscr();
     box(1,1,78,24);

     textcolor(12);
     gotoxy(x,y);

     gotoxy(x,y+1);
     textcolor(14);
     cprintf(" ----------------- MAIN MENU --------------------     ");
     textcolor(16);
     gotoxy(x,y+2);

     textcolor(15);
     gotoxy(x+5,y+3);
     cprintf("1.INSERT A RECORD");
     gotoxy(x+5,y+5);
     cprintf("2.MODIFY A RECORD");
     gotoxy(x+5,y+7);
     cprintf("3.DISPLAY A RECORD");
     gotoxy(x+5,y+9);
     cprintf("4.DISPLAY ALL RECORDS");
     gotoxy(x+5,y+11);
     cprintf("5.DELETE A RECORD");
     gotoxy(x+5,y+13);
     cprintf("6.DISPLAY DIRECTORY");
     gotoxy(x+5,y+15);
     cprintf("7.SPACE UTILIZATION");
     gotoxy(x+5,y+17);
     cprintf("8.EXIT");
     gotoxy(x,y+19);
     textcolor(18);
     cprintf("--------------------------------------");
     textcolor(WHITE);
     int choice;
     gotoxy(x+3,y+20);
     cprintf(" ENTER YOUR CHOICE :");
     cin >> choice;
     clrscr();
     switch(choice){
       case 1:
	   append(myfile);
	   break;
       case 2:
	   modify(myfile);flag=1;
	   break;
       case 3:
	   display(myfile);getch();
	   break;
       case 4:
	   displayall(myfile);getch();
	   break;
       case 5:
	   deleterecord(myfile);flag=1;
	   break;
       case 6:
	   Dir.Print(cout);
	   break;
	   case 7:
			//Dir.spaceutil(myfile);
			 if(flag)
		  updatefile(myfile);  //update the file if any delletions or updations are made
		  Dir.spaceutil(myfile);
			getch();
			break;
       case 8:
	   cout << "Updating the file"<<endl;
	   if(flag)
	     updatefile(myfile);
	   exit(0);
       default:
		 cout <<"\n\n\n\n\n\n\n\n\n\t\t\t INVALID CHOICE";
	     break;

     }
  }
}




void deleterecord1 (char *myfile,char *m)
{
	 Student s;
	 strcpy(s.URN,m);
	 int addr=Dir.Search(s.URN);
	 if(addr==-1){
	  // cout << "THE RECORD DOES NOT EXIST"<< endl;
	 //  getch();
	 return;
	 }
	fstream ofile(myfile,ios::in|ios::out);
	ofile.seekp(addr,ios::beg);
	ofile.write("*",1);
	ofile.close();
	Dir.Remove(s.URN);

}
