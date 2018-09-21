#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"
#include "string.h"
typedef int boolean;

boolean true=1;
boolean false=0;

boolean skip00=1;

int w=10,h=10;
int dataOffset=122;
char chars[0xff];
int** bmp;

void R(char* path);
void rR(FILE* fp);
void rH(FILE* fp);
void rC(FILE* fp);
void oC();
void oT(char* path);
void gotoxy(int x,int y);
void gotoxy(int x,int y)
{
	printf("%c[%d;%df",0x1B,y,x);
}
char* num2hexStr(int num);
char* num2hexStr(int num)
{
	// str=(char*)malloc(sizeof(char)*3);
	static char str[2];
	sprintf(str,"%x",num);
	if(strlen(str)==1)
	{
		char ch=str[0];
		sprintf(str,"0%c",ch);
	}
	return str;
}
int r4Bits(FILE *fp,long offset);
int r4Bits(FILE *fp,long offset)
{
	int rtn;
	char str[4];
	char str1[2];
	char str2[2];
	fseek(fp,(long)offset,0);
	int ch2=fgetc(fp);
	int ch1=fgetc(fp);
	strcpy(str1,num2hexStr(ch1));
	strcpy(str2,num2hexStr(ch2));
	//sprintf(str,"%s%s",str1,str2);
	sscanf(str1,"%x",&rtn);
	return rtn;
}

int main(int argc,char *argv[])
{
	for(int x=1;x<argc;x++)
	{
		if(argv[x][0]=='-')
		{
			switch(argv[x][1])
			{
				default:
					printf("Unknow command %s\nStarting Opening file.\n",argv[x]);
					break;
				case 'z':
					skip00=false;
					x++;
					break;
				case 'h':
					printf("bmp <commands> files\n"
						"\t-h:\tShow this help and exit.\n"
						"\t-z:\tDon't Skip 00 00 at every end of pixel line.\n");
					return 0;
			}
		}
		printf("Filename:%s\n",argv[x]);
		R(argv[x]);
	}
	return 0;
}

void R(char* path)
{
	FILE* fp = fopen(path,"r");
	if(fp==NULL)
	{
		puts("File not found or Permission denied.");
		return;
	}
	
	// Read config
	FILE* fpc = fopen("/usr/.configs/bmp.conf","r");
	if(fpc==NULL)
	{
		puts("Config file(/usr/.configs/bmp.conf) not found.");
		return;
	}
	rC(fpc);
	fclose(fpc);

	// Read header
	rH(fp);
	
	// init
	bmp = (int**)malloc(sizeof(int*)*h);
	for(int i=0;i<h;i++)
	{
		bmp[i] = (int*)malloc(sizeof(int)*w);
	}

	rR(fp);
	oT("./output.txt");
	oC();
	fclose(fp);
}

void rH(FILE* fp)
{
	int G=ftell(fp);

	dataOffset=r4Bits(fp,0xaL);
	printf("data offset:%d\n",dataOffset);

	w=r4Bits(fp,0x12L);
	printf("Width:%d\n",w);
	
	h=r4Bits(fp,0x16L);
	printf("Height:%d\n",h);

	fseek(fp,G,0);
}

void rC(FILE* fp)
{
	fseek(fp,0L,0);
	int ch;
	for(int x=0;x<=0xff && (ch=fgetc(fp))!=EOF;x++)
	{
		chars[x]=(char)ch;
	}
}

void rR(FILE* fp)
{
	fseek(fp,(long)dataOffset,0);
	int ch;
	int x=0;
	int y=0;
	int ch2,ch3;
	char opt;
	while((ch=fgetc(fp))!=EOF && y+1<h)
	{
		if(x==w)
		{
			if(skip00) fseek(fp,-1L,1);
			else fseek(fp,1L,1);
			y++;
			if((ch=fgetc(fp))==EOF) break;
			x=0;
		}
		ch2=fgetc(fp);
		ch3=fgetc(fp);
		int n=(ch+ch2+ch3)/3;
		/*switch(n)
		{
			default:
				opt='E';
				break;
			case 0:
				opt='.';
				break;
			case 0xff:
				opt='#';
				break;
		}*/
		opt=chars[n];
		bmp[h-y-1][x]=opt;
		x++;
	}
}

void oC()
{
	gotoxy(0,0);
	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++)
		{
			putchar(bmp[y][x]);
		}
		putchar('\n');
	}
}

void oT(char* path)
{
	FILE* fp=fopen(path,"r+");
	if(fp==NULL)
	{
		printf("CMD error.\n");
		return;
	}
	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++)
		{
			fwrite(&bmp[y][x],1,1,fp);
		}
		char tmp='\n';
		fwrite(&tmp,1,1,fp);
	}
	fclose(fp);
}

