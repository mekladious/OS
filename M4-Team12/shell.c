void getDirectory();
int getSectorsCount(char*);
int div(int, int);
void exit();

int main()
{
	while(1)
	{
		char c;
		char line[200];
		int i=0;
		int j,k;
		char* filename;
		char* filename2;
		char new_line[200];
		char buffer[13312];
		int file_index;
		char file[13312];
		int sectors;
		int killProcessNum;

		//interrupt(0x21, 0, "\n\0", 0, 0);

		interrupt(0x21, 0, "SHELL> \0", 0, 0);

		//taking the input command from the user


		interrupt(0x21, 1, line, 0, 0);
		i=0;

		//now checking the command and responding back
		while(1)
		{
			if(line[0]!='\0')
			{
				if(line[0]=='v' && line[1]=='i' && line[2]=='e' && line[3]=='w' && line[4]==' ')
				{
					for(j=5; line[j]!='\0'; j++)
					{
						filename[i]=line[j];
						i++;
					}
					filename[i]='\0';
					interrupt(0x21, 3, filename, buffer, 0); /*read the file into buffer*/
					if(buffer[0]=='\0')
					{
						interrupt(0x21, 0, "Error, file does not exist!\n\0", 0, 0);
						buffer[0]='\0';
					}
					interrupt(0x21, 3, filename, buffer, 0); /*read the file into buffer*/
					interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
					
					line[0]='\0';
					filename[0]='\0';
					filename2[0]='\0';
					new_line[0]='\0';
					buffer[0]='\0';
					file[0]='\0';
					interrupt(0x21, 5, 0, 0, 0);
					return;

				}
				else if(line[0]=='e' && line[1]=='x' && line[2]=='e' && line[3]=='c' && line[4]=='u' && line[5]=='t' && line[6]=='e' && line[7]==' ')
				{
					for(j=8; line[j]!='\0'; j++)
					{
						filename[i]=line[j];
						i++;
					}
					filename[i]='\0';
					interrupt(0x21, 3, filename, buffer, 0); /*read the file into buffer*/

					if(buffer[0]=='\0')
					{
						interrupt(0x21, 0, "Error, file does not exist!\n\0", 0, 0);
						buffer[0]=' ';
						interrupt(0x21, 5, 0, 0, 0);
					}

					interrupt(0x21, 4, filename, 0x2000, 0);
					return;


				}
				else if(line[0]=='d' && line[1]=='e' && line[2]=='l' && line[3]=='e' && line[4]=='t' && line[5]=='e' && line[6]==' ')
				{

					for(j=7; line[j]!='\0'; j++)
					{
						filename[i]=line[j];
						i++;
					}
					filename[i]='\0';
					interrupt(0x21, 7, filename, 0, 0);

					line[0]='\0';
					filename[0]='\0';
					filename2[0]='\0';
					new_line[0]='\0';
					buffer[0]='\0';
					file[0]='\0';
					interrupt(0x21, 5, 0, 0, 0);
					return;

				}
				else if(line[0]=='c' && line[1]=='o' && line[2]=='p' && line[3]=='y' && line[4]==' ')
				{
					i=0;
					for(j=5; line[j]!='\0'; j++)
					{
						if(line[j]==' ')
							break;
						filename[i]=line[j];
						i++;
					}
					if(line[j]=='\0')
					{
						interrupt(0x21, 0, "Error, destination file missing!\n\0", 0, 0);
						//buffer[0]=' ';

						line[0]='\0';
						filename[0]='\0';
						filename2[0]='\0';
						new_line[0]='\0';
						buffer[0]='\0';
						file[0]='\0';
						interrupt(0x21, 5, 0, 0, 0);
						return;
					}
					filename[i]='\0';
					interrupt(0x21, 3, filename, buffer, 0); /*read the file-name into buffer*/
						
					sectors=getSectorsCount(filename);

					if(buffer[0]=='\0')
					{
						interrupt(0x21, 0, "Error, file does not exist!\n\0", 0, 0);
						//buffer[0]='\0';

						line[0]='\0';
						filename[0]='\0';
						filename2[0]='\0';
						new_line[0]='\0';
						buffer[0]='\0';
						file[0]='\0';
						interrupt(0x21, 5, 0, 0, 0);
						return;
					}
					i=0;
					k=j+1;
					for(j=k; line[j]!='\0'; j++)
					{
						filename2[i]=line[j];
						i++;
					}
					filename2[i]='\0';
					
					interrupt(0x21, 8, filename2, buffer, sectors);

					line[0]='\0';
					filename[0]='\0';
					filename2[0]='\0';
					new_line[0]='\0';
					buffer[0]='\0';
					file[0]='\0';
					interrupt(0x21, 5, 0, 0, 0);
					return;

				}
				else if(line[0]=='d' && line[1]=='i' && line[2]=='r')
				{
					getDirectory();
				//	interrupt(0x21, 0, buffer, 0, 0);

					line[0]='\0';
					filename[0]='\0';
					filename2[0]='\0';
					new_line[0]='\0';
					buffer[0]='\0';
					file[0]='\0';
					interrupt(0x21, 5, 0, 0, 0);
					return;
				}
				 else if(line[0]=='c' && line[1]=='r' && line[2]=='e' && line[3]=='a' && line[4]=='t' && line[5]=='e' && line[6]==' ')
				 {
					i=0;
					for(j=7; line[j]!='\0'; j++)
					{
						filename[i]=line[j];
						i++;
					}
					filename[i]='\0';
					i=0;
					file_index=0;
					interrupt(0x21, 0, "Enter file content: \0", 0, 0);
					while(1)
					{
						interrupt(0x21, 1, new_line, 0, 0); //read new line
						for(i=0; new_line[i]!='\0';i++)
						{
							file[file_index]=new_line[i];
							file_index++;
						}
						file[file_index]=0xa; //'\n'
						file_index++;
						file[file_index]=0xd; //carriage return
						file_index++;

						new_line[0]='\0';
						if(i==0){
							file[file_index]='\0';
							break;
						}
						interrupt(0x21, 0, "\b\r\0", 0, 0);
					}
					interrupt(0x21, 8, filename, file, div(i,512)+1);

					line[0]='\0';
					filename[0]='\0';
					filename2[0]='\0';
					new_line[0]='\0';
					buffer[0]='\0';
					file[0]='\0';
					interrupt(0x21, 5, 0, 0, 0);
					return;

				 }
				else if(line[0]=='k' && line[1]=='i' && line[2]=='l' && line[3]=='l' && line[4]==' ')
				{
					if(line[5]=='1'||line[5]=='2'||line[5]=='3'||line[5]=='4'||line[5]=='5'||line[5]=='6'||line[5]=='7'||line[5]=='8')
					{
						if(line[5]=='1')
						{
							killProcessNum = 0;
						}
						else if(line[5]=='2')
						{
							killProcessNum = 1;
						}
						else if (line[5]=='3')
						{
							killProcessNum = 2;
						}
						else if (line[5]=='4')
						{
							killProcessNum = 3;
						}
						else if (line[5]=='5')
						{
							killProcessNum = 4;
						}
						else if (line[5]=='6')
						{
							killProcessNum = 5;
						}
						else if (line[5]=='7')
						{
							killProcessNum = 6;
						}
						else if (line[5]=='8')
						{
							killProcessNum = 7;
						}
						interrupt(0x21, killProcessNum, 0, 0, 0);
					}
					else 
					{
						interrupt(0x21, 0, "Invalid Command!\0", 0, 0);
						interrupt(0x21, 0, "\n\0", 0, 0);
					}
					line[0]='\0';
					filename[0]='\0';
					filename2[0]='\0';
					new_line[0]='\0';
					buffer[0]='\0';
					file[0]='\0';
					interrupt(0x21, 5, 0, 0, 0);
					return;
				}
				else{

					interrupt(0x21, 0, "Invalid Command!\0", 0, 0);
					interrupt(0x21, 0, "\n\0", 0, 0);

					line[0]='\0';
					filename[0]='\0';
					filename2[0]='\0';
					new_line[0]='\0';
					buffer[0]='\0';
					file[0]='\0';
					interrupt(0x21, 5, 0, 0, 0);
					return;
				}
			}
			else
			{
				return;
			}
		}


	}
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

void getDirectory(){
	int i,j,k,c;
	char directory[512];
	char buffer[512];

	//makeInterrupt21();
	interrupt(0x21,2,directory,2,0);

	for(i=0; i<16; i++){
		k=0;
		if(directory[i*32]!=0x00){
			for(j=0; j<6;j++){
				if(directory[i*32+j]==0x00) break;
				buffer[k]=directory[i*32+j];
				k++;
			}
			buffer[k]='\0';
			interrupt(0x21, 0, buffer, 0,0);
			interrupt(0x10, 0xE*256+0x9,0,0,0);
			interrupt(0x10, 0xE*256+0xa,0,0,0);
			c=0;
			 for(j=6; j<32 && directory[i*32+j]!=0x00; j++){
			 	c++;
			 }
				 switch(c){
					 case 1: buffer[0]='1'; buffer[1]='\0'; break;
					 case 2: buffer[0]='2'; buffer[1]='\0'; break;
					 case 3: buffer[0]='3'; buffer[1]='\0'; break;
					 case 4: buffer[0]='4'; buffer[1]='\0'; break;
					 case 5: buffer[0]='5'; buffer[1]='\0'; break;
					 case 6: buffer[0]='6'; buffer[1]='\0'; break;
					 case 7: buffer[0]='7'; buffer[1]='\0'; break;
					 case 8: buffer[0]='8'; buffer[1]='\0'; break;
					 case 9: buffer[0]='9'; buffer[1]='\0'; break;
					 case 10: buffer[0]='1';buffer[1]='0'; buffer[2]='\0'; break;
					 case 11: buffer[0]='1';buffer[1]='1'; buffer[2]='\0'; break;
					 case 12: buffer[0]='1'; buffer[1]='2'; buffer[2]='\0'; break;
					 case 13: buffer[0]='1'; buffer[1]='3'; buffer[2]='\0'; break;
					 case 14: buffer[0]='1';buffer[1]='4'; buffer[2]='\0';  break;
					 case 15: buffer[0]='1';buffer[1]='5'; buffer[2]='\0';  break;
					 default: buffer[0]=0x00; buffer[1]='\0';
				 }

			interrupt(0x21, 0, buffer, 0,0);
			interrupt(0x10, 0xE*256+0xa,0,0,0);
		}
	}
}

void exit(){
	/*line[0]='\0';
	filename[0]='\0';
	filename2[0]='\0';
	new_line[0]='\0';
	buffer[0]='\0';
	file[0]='\0';
	interrupt(0x21, 5, 0, 0, 0);*/
}
int getSectorsCount(char* fileName){
	int i,j,k;
	int count;
	char directory[512];
	interrupt(0x21,2,directory,2,0);
	for(i=0; i<16; i++){
		j=0;
		if(directory[i*32]!=0x00){
			for(j=0; j<=6;j++){
				if(j==6){
					count=0;
					for(k=6; k<32 && directory[i*32+k]!=0x00; k++){
						count++;
					}
					return count;
				}
				if(directory[i*32+j]!=fileName[j]) {
					break;
				}
			}
		}
	}
	return count;
}
