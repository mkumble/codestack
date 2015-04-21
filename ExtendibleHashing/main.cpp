
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
void deleterecord (char *myfile);
void append(char *myfile);
void modify(char *myfile);
void display(char *myfile);
void displayall(char *myfile);






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




void Open()
{     
	
setcolor(30);
settextstyle(1,0,2);
	outtextxy(80,80,"Department of");
outtextxy(100,104,"    Information Science,RVCE");

	setcolor(22);
	settextstyle(1,0,6);

	outtextxy(100,230," Extendible Hashing ");
	setcolor(22);
	settextstyle(2,0,6);
	
	setcolor(30);
	outtextxy(300,400,"Implemented by:");
	settextstyle(2,0,7);
	outtextxy(457,427,"   Mithun Kumble ");
	    
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
     textcolor(30);
     gotoxy(x,y+1);
     textcolor(30);
     cprintf(" --------------- MENU ----------------   ");
     textcolor(8);
     gotoxy(x,y+2);

     textcolor(22);
     gotoxy(x-5,y+3);

     cprintf("1.Insert Record");
     gotoxy(x+10,y+5);
     cprintf("2.Modify Record");
     gotoxy(x-5,y+7);
     cprintf("3.Display a Record");
     gotoxy(x+10,y+9);
     cprintf("4.Display all records");
     gotoxy(x-5,y+11);
     cprintf("5.Delete a record");
     gotoxy(x+10,y+13);
     cprintf("6.Display Directory");
     gotoxy(x-5,y+15);
     cprintf("7.Space Utilization");
     gotoxy(x+10,y+17);
     cprintf("8.Exit");
     gotoxy(x-5,y+19);
     textcolor(30);
           int choice;
     gotoxy(x+7,y+20);
        cprintf(" Enter your Choice :");
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
	   case 7:	 if(flag)
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
		 cout <<"\n\n\n\n\n\n\n\n\n\t\t\t Invalid Option...re-enter";
	     break;

     }
  }
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
     cout <<"            Enter the New Student Details:          ";
     gotoxy(x,y+2);


     gotoxy(x,y+4);
     cout << "   USN         : ";
     gotoxy(x,y+6);
     cout << "   Last Name   : ";
     gotoxy(x,y+8);
     cout << "   First Name  : ";
     gotoxy(x,y+10);
     cout << "   Address     : ";
     gotoxy(x,y+12);
     cout << "   Semester    : ";
     gotoxy(x,y+14);
     cout << "   Branch      : ";
     gotoxy(x,y+16);
     cout << "   College     : ";
     gotoxy(x,y+18);


     gotoxy(4,y-1);
     cout<<"                                                                ";

     gotoxy(x+20,y+4);
     strcpy(s.URN , toupp());

     while(strlen(s.URN) != 10 || s.URN[1]!='R' || s.URN[2]!='V' || s.URN[6]!='S' || ((s.URN[5]!='I') && (s.URN[5]!='C')))
     {
     if(s.URN[1]!='R' || s.URN[2]!='V')
{
gotoxy(x+18,y+20);
	 textcolor(YELLOW+BLINK);
	 cprintf("Only RV IDs allowed");

}

else if(s.URN[6]!='S' || ((s.URN[5]!='I') && (s.URN[5]!='C')))
{
gotoxy(x+18,y+20);
	 textcolor(YELLOW+BLINK);
	 cprintf("Only IS or CS branches are allowed");

}
else
{


	 gotoxy(x+18,y+20);
	 textcolor(YELLOW+BLINK);
	 cprintf("USN must be of 10 characters");
}

	 gotoxy(x+20,y+4);
	 cout <<"                                 ";
	 gotoxy(x+20,y+4);
	 strcpy(s.URN,toupp());
	 gotoxy(x+18,y+20);
	 cout <<"                         ";
	 textcolor(30);

     }

     int res = Dir.Search(s.URN);
     if(res!=-1) {
	 gotoxy(x+18,y+20);
	 textcolor(YELLOW+BLINK);
	 cprintf("USN already in database");
	 textcolor(30);
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
	 textcolor(YELLOW+BLINK);
	 cprintf("Enter Semester between 1 to 8");
	 gotoxy(x+20, y+12);
	 cout<<"                                ";
	 gotoxy(x+20,y+12);
	 gets(s.Semester);
	 s1 = atoi(s.Semester);
	 gotoxy(x+18,y+20);
	 cout <<"                                 ";
	 textcolor(30);
     }


     while(1) {
	 gotoxy(x+20,y+14);
	 textcolor(30);
	 gets(Branch1);
	 strcpy(s.Branch,toupp1(Branch1));

//	 if(!((strcmp(s.Branch,"CSE"))&&(strcmp(s.Branch,"ISE"))&&(strcmp(s.Branch,"ECE"))&&(strcmp(s.Branch,"IT"))&&(strcmp(s.Branch,'\0'))))


if((s.Branch[0]==s.URN[5]) && (s.Branch[1]==s.URN[6]))

	 {
	    gotoxy(x+18,y+20);

	     cout<<"                       ";
	     break;
	 }
	 else
	 {

	     gotoxy(x+18,y+20);
	     textcolor(YELLOW+BLINK);
	   //  cprintf("Not a valid branch..re-enter");
cprintf("The branch you specified doesnot match with your USN...Your branch must be %c%c%c",s.URN[5],s.URN[6],'E');
	 }
	 gotoxy(x+20,y+14);
	 cout<<"                               ";
     }


     gotoxy(x+20,y+16);
     gets(s.College);
while((s.College[0]!='R'&& s.College[0]!='r')|| (s.College[1]!='V'&& s.College[1]!='v') || (s.College[2]!='C'&& s.College[2]!='c') || (s.College[3]!='E'&& s.College[3]!='e'))
{gotoxy(32,22);
     textcolor(YELLOW + BLINK);
     cprintf("College must be RVCE..re-enter");
    gotoxy(x+20,y+16);
     gets(s.College);
}
     gotoxy(32,22);
     textcolor(YELLOW + BLINK);
     cprintf("Record has been updated Successfully");
     textcolor(YELLOW);
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
   cout << "\n\t\t Enter the USN of the record to be modified : ";
  strcpy(s.URN,toupp());
  int addr=Dir.Search(s.URN);
  if(addr==-1){
  gotoxy(20,15);
      textcolor(YELLOW + BLINK);
      cprintf("There is no such record");
      textcolor(YELLOW);
   getch();
   return;
  }
  clrscr();
  cout << "\n\t\tThe current contents of the record are : ";
  DelimFieldBuffer :: SetDefaultDelim('|');
  DelimFieldBuffer Buff;
  ifstream file(myfile,ios::in);

  Buff.DRead(file,addr);
  s.Unpack(Buff);
  s.Print(cout);
  getch();
  clrscr();

  cout <<"\n";
gotoxy(180,200);
cout<<"Provide the option which you need to modify"<<endl;
  cout 	<<"    1. First Name"<<endl
	<<"    2. Last Name"<<endl
	<<"    3. Address"<<endl
	<<"    4. Semester"<<endl
	<<"    5. Branch"<<endl
	<<"    6. College"<<endl
	<<"    0. I don't want to modify"<<endl;
  fflush(stdin);
  cout <<endl<< "\n\t\tEnter the field number : ";
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
		 textcolor(YELLOW + BLINK);
		 cprintf("Semester can only be between 1 and 8...Re-enter");
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

		//if(!((strcmp(s.Branch,'\0'))&&(strcmp(s.Branch,"CSE"))&&(strcmp(s.Branch,"ISE"))&&(strcmp(s.Branch,"ECE"))&&(strcmp(s.Branch,"IT"))))  

if((s.Branch[0]==s.URN[5]) && (s.Branch[1]==s.URN[6]))
{
			gotoxy(x+22, y+10);
			cout<<"                         ";
			gotoxy(x+22, y+10);
			break;
		getch();
		}


		 else{
		    gotoxy(x+12,y+20);
		    textcolor(YELLOW+BLINK);
		    cprintf("The branch you specified doesnot match with your USN...Your branch must be %c%c%c",s.URN[5],s.URN[6],'E');
	 
		}
}

break;

	     case 6 : cout << "\n\t\t    Enter the new College: ";
	  gets(s.College);



while((s.College[0]!='R'&& s.College[0]!='r')|| (s.College[1]!='V'&& s.College[1]!='v') || (s.College[2]!='C'&& s.College[2]!='c') || (s.College[3]!='E'&& s.College[3]!='e'))
{gotoxy(x+12,y+20);
     textcolor(YELLOW + BLINK);
     cprintf("College must be RVCE..re-enter");
    gotoxy(x+24,y+13);
     gets(s.College);
}


	  break;

    default:    gotoxy(x+12,y+20);
		textcolor(YELLOW + BLINK);
		cprintf("You have not specified a valid choice...");
		textcolor(YELLOW);
		return;
  }
   s.Pack(Buff);
   int newaddr=s.Append(myfile);
   fstream ofile(myfile,ios::in|ios::out);
   ofile.seekp(addr,ios::beg);
   Dir.Remove(oldurn);
   ofile.write("*",1);
   gotoxy(28,20);
     textcolor(YELLOW + BLINK);
     cprintf("Update Successful");
     textcolor(YELLOW);

   Dir.Insert(s.URN,newaddr);
   ofile.close();
}




void display(char *myfile)
{
   Student s;
   cout << "\n\t\tEnter the USN to be displayed : ";
   strcpy(s.URN,toupp());
   int addr;
   if((addr = Dir.Search(s.URN))==-1){
      gotoxy(20,15);
      textcolor(YELLOW + BLINK);
      cprintf("No such record exists");
      textcolor(YELLOW);
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
      s.Print(cout);
     file.close();
}





void displayall(char *myfile)
{int count;char quit;
   int x,y=7,z=49,i=1;
   DelimFieldBuffer :: SetDefaultDelim('|');
   DelimFieldBuffer Buff;
   fstream file(myfile,ios::in);
   Student s;
   while(1){

count=1;x=9;
      clrscr();

     
 	 gotoxy(23,1);
	 textcolor(22);
	 cprintf("Details of the Student Database:");
	gotoxy(25,25);
cprintf("Press <q><u><Enter> to quit....");


while(count!=3)
 {


 int res = Buff.Read(file);
      if(res==-1){
	 file.close();
	 return;
      }
      if( Buff.getfirstchar() == '*') continue;
	 s.Unpack(Buff);
 	 gotoxy(23,1);

gotoxy(x+5,4); 
textcolor(22);
cprintf("Record Number:");
	 cprintf(":");cout<<i++;
	 
	 gotoxy(x,y);
	 cprintf("USN          :");

	 gotoxy(x,y+2);
	 cprintf("First Name   :");

	 gotoxy(x,y+4);
	 cprintf("Last Name    :");

	 gotoxy(x,y+6);
	 cprintf("Semester     :");

	 gotoxy(x,y+8);
	 cprintf("Branch       :");

	 gotoxy(x,y+10);
	 cprintf("College      :");

	 gotoxy(x,y+12);
	 cprintf("Address      :");

	 textcolor(YELLOW);
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
	


count++;
x=z;
}

quit=getch();
if(quit=='q')
break;

}
}





void deleterecord (char *myfile)
{
    Student s;
textcolor(22);
    cprintf("\n\tEnter the USN of the record to be deleted : ");
    strcpy(s.URN,toupp());
    int addr=Dir.Search(s.URN);
    if(addr==-1){
      gotoxy(20,15);
      textcolor(YELLOW + BLINK);

      cprintf("Record not found");
      textcolor(YELLOW);
      getch();return;
    }
   fstream ofile(myfile,ios::in|ios::out);
   ofile.seekp(addr,ios::beg);
   ofile.write("*",1);
   ofile.close();
   Dir.Remove(s.URN);
   cout << "\n\n----- Record has been deleted --------- ";
   cout<<" \n\nPress a key to see the directory collapse\n";
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
























