
void printString (char*);
void readString(char*);
void readSector(char* , int );
void readFile(char*, char*);
int mod (int, int);
int div (int, int);
void handleInterrupt21(int, int, int, int);
void executeProgram(char*, int);
int getFreeSegment();
void terminate();
void writeSector(char*,int);
void deleteFile(char*);
void writeFile(char*,char*,int);
void handleTimerInterrupt(int, int);

int active[8];
int sp[8];
int currentProcess;
int timeUnits;

int main()
{
	//M3
	//writeSector works
	// char buffer[512];
	// writeSector("hey\0", 30);
	// readSector(buffer, 30);
	// printString(buffer);

	// //deleteFile works
	//  char* buffer[13312];
	//  makeInterrupt21();
	//  interrupt(0x21, 7,"messag\0",0,0);
 	//  interrupt(0x21, 3,"messag\0",buffer,0);
	//  interrupt(0x21, 0,buffer,0,0);

	// int i=0;
	// char buffer1[13312];
	// char buffer2[13312];
	// buffer2[0]='h'; buffer2[1]='e'; buffer2[2]='l'; buffer2[3]='l';
	// buffer2[4]='o';
	// for(i=5; i<13312; i++) buffer2[i]=0x0;
	// makeInterrupt21();
	// interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
	// interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
	// interrupt(0x21,0, buffer1, 0, 0); // print out contents of testW

	//Step 4

	int i;

	currentProcess=0;
	timeUnits=0;

	for(i=0;i<8;i++)
	{
		active[i]=0;
		sp[i]=0xFF00;
	}


	makeTimerInterrupt();
	makeInterrupt21();
	//terminate();

	//our actual main method
	//interrupt(0x21, 4, "hello1\0", 0, 0);
	//interrupt(0x21, 4, "hello2\0", 0, 0);
	interrupt(0x21, 4, "shell\0", 0, 0);
	while(1);
}

void printString (char* chars){

	int i = 0;
	char c = chars[i];
	interrupt(0x10, 0xE*256+0xd,0,0,0);
	while(c!='\0')
	{
    	interrupt(0x10, 0xE*256+c,0,0,0);
		i++;
		c = chars[i];
	}
	interrupt(0x10, 0xE*256+0x9,0,0,0);

}

void readString(char* line){
	char c;
	int i =0;
	while(1)
	{
		//0x0 = null, 0xd = carriage return, 0xa = line feed/n
    		c=interrupt(0x16, 0,0,0,0);
		if(c==0xd){
			line[i]=0x0;
			interrupt(0x10, 0xE*256+0xa,0,0,0);
			return;

		//if it's a backspace (0x8)
		}else if(c==0x8){
				if(i!=0)
				{
				i--;
				interrupt(0x10, 0xE*256+c,0,0,0);
				interrupt(0x10, 0xE*256+' ',0,0,0);
				interrupt(0x10, 0xE*256+c,0,0,0);
				}
			}
		else
		{
				line[i] = c;
				interrupt(0x10, 0xE*256+c,0,0,0);
				i++;
		}
	}
}

void readSector(char* buffer, int sector)
{
	int AH = 2;
	int AL= 1;
	int BX= buffer;
	int CH= div(sector,36);
	int CL= mod(sector,18)+1;
	int DH= mod(div(sector,18),2);
	int DL= 0;

	int AX= AH*256+AL;
	int CX= CH*256+CL;
	int DX= DH*256+DL;

	interrupt(0x13,AX,BX,CX,DX);
}

int mod (int a, int b)
{
	int res=0;

	while(1)
	{
		if(b>a)
		{
			return a;
		}
		else
		{
			if(a==b)
			{
				return 0;
			}
			else
			{
				a=a-b;
			}
		}
	}
	return res;

}

int div (int a, int b)
{
	int res=0;
	while(a>0)
	{
		if(a==b)
		{
			return res+1;
		}
		else
		{
			if(b>a)
			{
				return res;
			}
			else
			{
				a=a-b;
				res=res+1;
			}
		}
	}

	return res;
}

void readFile(char* fileName, char* buffer)
{
	int i,j,m = 0;
	int sector;
	char directory[512];

	readSector(directory, 2);

	for(i=0; i<6; i++){
		if(fileName[i]=='\0'){
			for(j=i; j<6;j++){
				fileName[j]=0x00;
			}
			break;
		}
	}

	for(i = 0; i<16; i++)
	{
		for(j=0; j<6 && fileName[j]!='\0'; j++){
			if(directory[(32*i)+j]!=fileName[j]){
				break;}
		}
		if(fileName[j]=='\0'|| j==6){
			break;
		}
		if(i==15)
		{
			buffer[0]='\0';
			return;
		}
	}

	for(m=(32*i)+6; directory[m]>0; m++){
		sector = directory[m];
		if(sector <= 3) break;
		readSector(buffer, sector);
		buffer += 512;
	}
}

void executeProgram(char* name)
{
	char buffer[13312];
	int i=0;
	int segment;
	setKernelDataSegment();
	segment = getFreeSegment();

	if(segment==0x0000||segment<=0x1000||segment>0xA000)
	{
		return;
	}
	readFile(name, buffer);

	while(i<13312){
		putInMemory(segment, i, buffer[i]);
		i++;
	}
	initializeProgram(segment);

	setKernelDataSegment();
	active[div(segment,0x1000)-2]=1;
	restoreDataSegment();
}


int getFreeSegment()
{
	int i=0;
	int segment;
	for(i=0;i<8;i++)
	{
		if(active[i]==0)
		{
			segment = (i+2)*0x1000;
			return segment;
		}
	}
	restoreDataSegment();
	return 0;
}

void terminate()
{
	/*char shell[10];
	shell[0]='s';
	shell[1]='h';
	shell[2]='e';
	shell[3]='l';
	shell[4]='l';
	shell[5]='\0';
	makeInterrupt21();
	interrupt(0x21, 4, shell, 0x2000, 0);*/
	setKernelDataSegment();
	active[currentProcess]=0;
	restoreDataSegment();
	while(1);
}

void writeSector(char* buffer, int sector){

	int AH = 3;
	int AL= 1;
	int BX= buffer;
	int CH= div(sector,36);
	int CL= mod(sector,18)+1;
	int DH= mod(div(sector,18),2);
	int DL= 0;

	int AX= AH*256+AL;
	int CX= CH*256+CL;
	int DX= DH*256+DL;

	interrupt(0x13,AX,BX,CX,DX);
}

void deleteFile(char* name){
	char directory[512];
	char map[512];
	int i,j, m,sector, end;

	makeInterrupt21();
	interrupt(0x21,2,map,1,0);
	interrupt(0x21,2,directory,2,0);

	for(i = 0; i<16; i++) //i->sectors j->name
	{
		for(j=0; j<6; j++){
			if(directory[(32*i)+j]!=name[j])
				break;
		}
		if(j==6){
			directory[i*32]= 0x00;
			for(m=i*32+6;m<(m+1)*32;m++){
				if(directory[m]==0x00)break;
				map[directory[m]]=0x00;
			}
			break;
		}
	}
	if(i==16){
		interrupt(0x21, 0, "File is not found\n\0", 0, 0);
		return;
	}
	interrupt(0x21,6,map,1,0);
	interrupt(0x21,6,directory,2,0);
}


void writeFile(char* name, char* buffer, int secNum){
	char directory[512];
	char map[512];
	int i, j, k, dir;
	int countSec = secNum;

	makeInterrupt21();
	interrupt(0x21,2,map,1,0);
	interrupt(0x21,2,directory,2,0);

	//getting an empty slot in the directory
	for(i=3; i<16; i++){
		if(directory[i*32]==0x00){
			dir=i;
			break;
		}
	}
	if(i==16){
		printString("Sorry, there are no free directories!\0");
		return;
	}
	//filling the first 6 bytes with 0x00
	for(j=0; j<6; j++){
		directory[dir*32+j]=0x00;
	}
	//filling the first 6 bytes with the filename
	for(j=0; j<6; j++){
		if(name[j]!='\0')
			directory[dir*32+j]=name[j];
		else break;
	}
	//finding empty sectors
	dir= dir*32+6;
	for(j=0;j<256 && countSec>0;j++){
		if(map[j]==0x00){
			map[j]=0xFF;
			directory[dir]=j;
			dir++;
			countSec--;
			interrupt(0x21,6,buffer,j,0);
			buffer += 512;
		}
	}
	if(j==256){
		printString("Sorry, there are no free sectors!\0");
		return;
	}
	for(j=6+secNum; j<32;j++){
		directory[j]=0x00;
	}
	interrupt(0x21,6,map,1,0);
	interrupt(0x21,6,directory,2,0);
}

void killProcess(int process)
{
	setKernelDataSegment();
	active[process]=0;
	restoreDataSegment();
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if(ax==0)
	{
		printString(bx);
	}
	else if(ax==1)
	{
		readString(bx);
	}
	else if(ax==2)
	{
		readSector(bx,cx);
	}
	else if(ax==3)
	{
		readFile(bx,cx);
	}
	else if(ax==4)
	{
		executeProgram(bx);
	}
	else if(ax==5)
	{
		terminate();
	}
	else if(ax==6)
	{
		writeSector(bx,cx);
	}
	else if(ax==7)
	{
		deleteFile(bx);
	}
	else if(ax==8)
	{
		writeFile(bx,cx,dx);
	}
	else if(ax==9)
	{
		killProcess(bx);
	}
	else if(ax>9)
	{
		printString("Sorry, an error occured!\0");
	}
}
void handleTimerInterrupt(int segment, int stackPointer)
{
	//printString("Tick\n\0");
	int i,newSegment,newSp;

	setKernelDataSegment();
	currentProcess = div(segment,0x1000)-2;
	timeUnits++;

	if(timeUnits>=100){
		timeUnits=0;
		sp[currentProcess]=stackPointer;
		i=currentProcess+1;
		while(i!=currentProcess)
		{
			if(active[i]==1)
			{
				currentProcess=i;
				break;
			}
			if(i==8)
				i=0;
			else i++;
		}
		newSegment=(currentProcess+2)*0x1000;
		newSp=sp[currentProcess];
		returnFromTimer(newSegment, newSp);
		return;
	}
	restoreDataSegment();
	returnFromTimer(segment, stackPointer);
}
