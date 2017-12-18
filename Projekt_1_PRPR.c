#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int funkcia_v()
{
	FILE *fr;
	char c;
	if ((fr = fopen("predaj.txt", "r")) == NULL) // kontrola ci bol textovy subor spravne otvoreny
		{
		printf("Neotvoreny subor");
		return 1;
		}
		
	while(!feof(fr)) // cyklus na vypisanie textoveho suboru 
	{	
		printf("meno a priezvisko: ");
		while((c=getc(fr)) != '\n')
			putchar(c);
		putchar(c);
		
		printf("spz: ");
		while((c=getc(fr)) != '\n')
			putchar(c);
		putchar(c);
		
		printf("typ auta: ");
		while((c=getc(fr)) != '\n')
			putchar(c);
		putchar(c);
		
		printf("cena: ");
		while((c=getc(fr)) != '\n')
			putchar(c);
		putchar(c);
		
		printf("datum: ");
		while((c=getc(fr)) != '\n' && c!=EOF)
		putchar(c);
		putchar(c);
		
		while((c=getc(fr)) != '\n' && c!=EOF)
		putchar(c);
		putchar(c);
	}
	printf("\n");
	fclose(fr);
	return 0;
}
int RozdelDatum(char* datum, int from, int to) // pomocna funkcia pre porovnajData
{
	char *datumA=(char*) malloc(4*sizeof(char));
	int k=0;
	int i;
	for(i=from; i<=to; i++) // cyklus rozdelovania pola datum na jednotlive casti podla potreby
		*(datumA + k++)=datum[i];
	return atoi(datumA);	// funkcia vrati integer
}
float porovnajData(int typauta, char *datum, char *rokAKTUAL,char *cena) 			// pomocna funkcia pre funkcia_o
{
	int rokSubor = RozdelDatum(datum, 0, 3); // volanie pomocnej funkcie RozdelDatum pre rozdelenie pola datum a rokAKTUAL na 3 casti : rok,mesiac,den
	int rokZADAL = RozdelDatum(rokAKTUAL, 0,3); // do lokalnych premennych je ulozeny integer, ktory vrati funkcia RozdelDatum
	int mesiacSubor = RozdelDatum(datum, 4,5);
	int mesiacZADAL = RozdelDatum(rokAKTUAL, 4,5);
	int denSubor = RozdelDatum(datum, 6,7);
	int denZADAL = RozdelDatum(rokAKTUAL, 6,7);
	
	float CenaAuta = atof(cena); // konverzia charu na float pre porovnavanie
	
	int rozdielRokov = rokZADAL-rokSubor; // cyklus podmienok pre priradenie spravnej ceny 
	if(rozdielRokov == 1) 
	{
		if(mesiacSubor<mesiacZADAL)
				return (typauta==1 ? 0.015 : 0.022)*CenaAuta; // funkcia vracia uz finalnu sumu
				
		else if(mesiacSubor=mesiacZADAL && denSubor<=denZADAL)
				return (typauta==1 ? 0.015 : 0.022)*CenaAuta;
	}
	else if(rozdielRokov>1) 
			return (typauta==1 ? 0.015 : 0.022)*CenaAuta;
	else 
		return 0;
	
}
void funkcia_o(int kontrola)
{
	if(kontrola!=0) // kontrola ci uz bola vytvorena funkcia_v
		return;
	char c, rok[8], datum[8], cena[10], typauta[2], *meno, *spz; // deklaracia premennych
	int pocet=1,a;

	scanf("%s",&rok); // nacitanie aktualneho roka
	
	FILE *fr;
	if ((fr = fopen("predaj.txt", "r")) == NULL) // kontrola ci bol textovy subor spravne otvoreny
		return;
	
	while(!feof(fr))
	{
			meno=(char*) calloc(25,sizeof(char));	// cyklus nacitania udajov do poli pre pracu s nimi
			spz=(char*) calloc(8,sizeof(char));
			a=0;
			while((c=getc(fr)) != '\n')
				*(meno + a++)=c;
			a=0;
			while((c=getc(fr)) != '\n')
				*(spz + a++)=c;
			a=0;
			while((c=getc(fr)) != '\n')
				typauta[a++]=c;
			a=0;
			while((c=getc(fr)) != '\n')
				cena[a++]=c;
			a=0;		
			while((c=getc(fr)) != '\n' && c!=EOF)
				datum[a++]=c;
			while((c=getc(fr)) != '\n' && c!=EOF)
			 	putchar(c);
			
			float porovnanie=porovnajData(atoi(typauta), datum, rok,cena);  // volanie funkcie na porovnanie dat z textoveho suboru 
			if (porovnanie==0) 
				continue;

		    printf("%s %s %.2f\n",meno,spz,porovnanie); // vypisanie
		    free(meno); // uvolnenie zvysku pola 
		    free(spz);
	}
	fclose(fr);	
}
char** funkcia_n(int kontrola,int *PocetRiadkov,int*kontrolaPole)
{
	char ignore[1024],**polespz,c; // deklaracia premennych 
	int i,j=0,k;
	
	if(kontrola!=0) // kontrola ci uz bola vykonana funkcia_v
		exit;

	FILE *fr;
	if ((fr = fopen("predaj.txt", "r")) == NULL) // podmienka, ci bol textovy subor spravne otvoreny
		exit;
	
	*PocetRiadkov=0;
	while(!feof(fr))			
	{
		fgets(ignore,sizeof(ignore),fr);		// cyklus pocitania riadkov suboru 
		(*PocetRiadkov)++;
	}
	rewind(fr);
	
	if ( (polespz=((char **) malloc( (*PocetRiadkov/6)*sizeof(char*)))) != NULL) // kontrola ci uz bolo pole pre spz alokovane, ak ano, tak sa vynuluje
	{		
			polespz=((char **) calloc((*PocetRiadkov/6),sizeof(char*)));
			for(i=0;i<(*PocetRiadkov/6);i++)
			{
				polespz[i]=(char *)calloc(7,sizeof(char));
			}
	}
	if ((polespz=((char **) malloc((*PocetRiadkov/6)*sizeof(char*))))==NULL) // alokovanie pola pre spz
		printf("Pole sa nepodarilo alokovat");
	for(i=0;i<(*PocetRiadkov/6);i++)
		polespz[i]=(char *)malloc(7*sizeof(char));
	
	*kontrolaPole=1;
	while(!feof(fr))
	{	
		k=0;
		fgets(ignore,sizeof(ignore),fr);
		while((c=getc(fr)) != '\n') 
		{ 		
			polespz[j][k++]=c;  // cyklus zapisovania spz-tiek do pola
		}
		fgets(ignore,sizeof(ignore),fr);
		fgets(ignore,sizeof(ignore),fr);
		fgets(ignore,sizeof(ignore),fr);
		fgets(ignore,sizeof(ignore),fr);
		j++;
	}
	fclose(fr);
	return polespz;
}
void funkcia_s(int kontrola,char **pole,int *PocetRiadkov,int *kontrolaPole,char **poleN,int*Akontrola)
{
	int i,j,k;		// deklaracia lokalnych premennych funkcie
	if(*kontrolaPole!=1)		// kontrola vytvorenia pola (funkcia_n)
	{
		printf("Pole nie je vytvorene\n");
		return;
	}	
	
	for(i=0;i<(*PocetRiadkov/6);i++) // cyklus vypisovania 
	{
		for(j=0;j<7;j++)
		{
			printf("%c",pole[i][j]);
			if (j==1 || j==4) printf(" ");
		}
	printf("\n");
	if(*Akontrola==0)
		{	
			for(k=0;k<2;k++)
			printf("%c",poleN[i][k]);
			printf("\n");
		}
	}
}
void funkcia_p(char **polespz,int *PocetRiadkov,int*kontrolaPole)
{
	int i,j,k,pocitadlo; 		// deklaracia lokalnych premennych funkcie
	char **polespzVYM;
	
	if(*kontrolaPole!=1) 		// kontrola vytvorenia pola (funkcia_n)
	{
		printf("Pole nie je vytvorene\n");
		return;
	}	
	
	polespzVYM=((char **) malloc((*PocetRiadkov/6)*sizeof(char*))); // alokacia
			for(i=0;i<(*PocetRiadkov/6);i++)
			{														
				polespzVYM[i]=(char *)malloc(7*sizeof(char));
			} 
	
	for(i=0;i<(*PocetRiadkov/6);i++)				// vymena znakov
		for(j=0,k=6;j<7,k>=0;j++,k--)
			polespzVYM[i][j]=polespz[i][k];
	
	for(i=0;i<(*PocetRiadkov/6);i++)
		{	
			pocitadlo=0;
			for(j=0;j<7;j++)
			{
				if (polespzVYM[i][j]==polespz[i][j]) // porovnavanie riadkov s vymenenymi znakmi a povodnych riadkov 
					pocitadlo++;	
			}
			if (pocitadlo==j) 	// podmienka vypisania
			{
				for(k=0;k<2;k++)
				printf("%c",polespzVYM[i][k]); // vypisania
			printf("\n");
			}
		}
}
void funkcia_z(char **pole,int *PocetRiadkov,int*kontrolaPole)
{
	if(*kontrolaPole!=1) // kontrola ci uz je vytvorene pole 
		return;

	char **pole2zn,overenie[1][2]; // deklaracia premennych 
	int i,j,A=0;
	float x;
	
	pole2zn=((char **) malloc(5*sizeof(char*)));
			for(i=0;i<(*PocetRiadkov/6);i++)
			{														// alokacia
				pole2zn[i]=(char *)malloc(2*sizeof(char));
			} 
	
	for(i=0;i<(*PocetRiadkov/6);i++)				
		for(j=0;j<2;j++)
			pole2zn[i][j]=pole[i][j]; // do pola pole2zn sa ukladaju prve 2 znaky spz
	
	while(A<((*PocetRiadkov/6)-2)) // cyklus pre porovnavanie jednotlivych prvych 2 znakov s ostatnymi, vzdy sa posuva o riadok nizsie 
	{ 																	// a porovnava so zvysnymi
		for(j=0;j<2;j++) // zachovanie prvych 2 znakov pre porovnanie s ostatnymi riadkami
			overenie[0][j]=pole2zn[A][j];
		x=0;
		for(i=A;i<(*PocetRiadkov/6);i++)				
			for(j=0;j<2;j++)	
				{
					if(pole2zn[i][j]==overenie[0][0] || pole2zn[i][j]==overenie[0][1])
					x+=0.5;
				}
		if (x>1.5) printf("%c%c %d\n",overenie[0][0],overenie[0][1],(int)x); // konvertovanie z float na Integer 
		A++;
	} 
}
void funkcia_b(char **pole,int*Riadky,int*kontrolaPole)
{
	if(*kontrolaPole!=1)
		{
			printf("Pole nie je vytvorene\n");
			return;
		}
	char hist[10];
	int i,j;
	
	for(i=0;i<10;i++)
	hist[i]=0;
	
	for(i=0;i<(*Riadky/6);i++)
		for(j=2;j<5;j++)
			hist[(pole[i][j])-'0']++;
	
	for(i=0;i<10;i++)
	if(hist[i]!=0)
	printf("%c:%d\n",i+'0',hist[i]);		
}
char** funkcia_a(char **pole,int*Riadky,int*kontrolaPole,int*kontrolaA)
{
	if(*kontrolaPole!=1)
		exit;
	
	*kontrolaA=0;
	char **PoleN;
	int i,j,cifra;
	
	scanf("%d",&cifra);
	
	PoleN=(char **) malloc((*Riadky/6)*sizeof(char*));
	for(i=0;i<(*Riadky/6);i++)
		PoleN[i]=(char *)malloc(7*sizeof(char)); 
	
	for(i=0;i<(*Riadky/6);i++)
		for(j=0;j<8;j++)				// kopirovanie prvkov
			PoleN[i][j]=pole[i][j];

	for(i=0;i<(*Riadky/6);i++)
		for(j=0;j<2;j++) 			 // pouzitie cifry
		{
			if((PoleN[i][j]+cifra)>'Z')
			{
				PoleN[i][j]-=26;
			}
			PoleN[i][j]=PoleN[i][j]+cifra;
		}
	return PoleN;
}
int main()
{
	char vyber,**p,**pN;
	int x,pocet,arraycontrol,Akontrola; // deklaracia premennych 
	scanf("%c",&vyber);
	while(vyber!='k') // cyklus vyberu funkcii 
	{
		switch(vyber)
		{
			case 'a':
				pN=funkcia_a(p,&pocet,&arraycontrol,&Akontrola);
			break;
			case 'v': 
				x=funkcia_v();
			break;
			case 'o':
				funkcia_o(x);
			break;
			case 'n':
				p=funkcia_n(x,&pocet,&arraycontrol);
			break;
			case 's':
				funkcia_s(x,p,&pocet,&arraycontrol,pN,&Akontrola);
			break;
			case 'p':
				funkcia_p(p,&pocet,&arraycontrol);
			break;
			case 'z':
				funkcia_z(p,&pocet,&arraycontrol);
			break;
			case 'b':
				funkcia_b(p,&pocet,&arraycontrol);
			break;
		}
		scanf("%c",&vyber);
	}
	return 1;
}
