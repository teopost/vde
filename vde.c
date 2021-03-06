/*
================================================================================
vde.c (C)Copyright Maggio '95 by TeoSoft Inc. 
--------------------------------------------------------------------------------
Video Extended : Programma di utility nella stesura di script UNIX.
Compilazione   : cc -o vde vde.c -l curses 
================================================================================
*/

#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <term.h>
#include <time.h>
#include <sys/types.h>

#define  TESTATA "\n\nvde, Last mod. 27/11/95 (C)Copyright 1994\n"

char alert_item[20][80];
char login_name[15];

int  item=0;
int  max_len=0;
int  graph=TRUE;
long tp;
FILE *infile;

void gotoxy(int row, int col);
void tbox(int box_t, int box_l, int box_b, int box_r, int shadow, int graphic);
void array_assign(char *alert_list);
void exit_error(int errore);
void Info();
void Instructions();
void getlinea(char *tamp);
char *getenv();

int main(int argc, char **argv)
{
   char titolo[80];
   int ww=0;
   int a=0;
   int c;
   int bell_a;
   int alert_a;
   int t, l, b, r;
   int ombra=TRUE;
   int p=0;
   char iper[256];
   char datario[10];
   char orario[10];
   char infodir[256];
   char stamp[56];
   char riga[80];
   char sistema[12];
   char padre[40];
   char nometemp[64];
   char linea[30];
   long tp;
/* time_t *ts; */
   float x1;
   int n1;


	
   /* CARICAMENTO TABELLA TERMINFO */
   /* ============================ */
   setupterm((char *)0,1,(int *)0);

   system("stty -ofdel tab3");

   /* ABILITAZIONE MODALITA GRAFICA */
   /* ============================= */
   /* putp(ena_acs); */

   if(argc==1)
      exit_error(0);

   if(strcmp(argv[1],"-h")==0)
   {
      Instructions();
      exit(0);
   }

   /* FUNZIONE shroot      */
   /* ==================== */
   if(strcmp(argv[1],"data")==0)
   {
      tp=time(0L);
      switch(localtime(&tp)->tm_wday)
      {
      	case 0: printf("Domenica, ");  break;
      	case 1: printf("Lunedi, ");    break;
      	case 2: printf("Martedi, ");   break;
      	case 3: printf("Mercoledi, "); break;
      	case 4: printf("Giovedi, ");   break;
      	case 5: printf("Venerdi, ");   break;
      	case 6: printf("Sabato, ");    break;
      }
      printf("%.2d ", localtime(&tp)->tm_mday);
      switch(localtime(&tp)->tm_mon)
      {
      	case 0:  printf("Gennaio ");    break;
      	case 1:  printf("Febbraio ");   break;
      	case 2:  printf("Marzo ");      break;
      	case 3:  printf("Aprile ");     break;
      	case 4:  printf("Maggio ");     break;
      	case 5:  printf("Giugno ");     break;
      	case 6:  printf("Luglio ");     break; 
      	case 7:  printf("Agosto ");     break;
      	case 8:  printf("Settembre ");  break;
      	case 9:  printf("Ottobre ");    break;
      	case 10: printf("Novembre ");   break;
      	case 11: printf("Dicembre ");   break;
      }
      printf("'%i", localtime(&tp)->tm_year);
      if((argc==3)&&(strcmp(argv[2], "-o") == 0))
      {
         printf("\n");
	 exit(0);
      }
      else
      { 
         printf(" - Ore ");
         printf("%.2d:%.2d",localtime(&tp)->tm_hour,localtime(&tp)->tm_min);
         printf("\n");
         exit(0);
      }
   }

   /* FUNZIONE shroot      */
   /* ==================== */
   if(strcmp(argv[1],"shroot")==0)
   {
      c=setuid(0);
      if(c != 0)
      {
         fprintf(stderr,"IMPOSSIBILE ESEGUIRE DA ROOT (Verificare permessi)\n");
         exit(1);
      }
      if(argc != 3)
         exit(0);
      exit(system(argv[2]));
   }

   /* FUNZIONE random_name */
   /* ==================== */
   if(strcmp(argv[1],"random_name")==0)
   {
      if(argc==2)
      {
         strcpy(nometemp,"\0");
         strcpy(nometemp, tmpnam(nometemp));
         strcat(nometemp, "\0");
         printf("%s", nometemp);
         exit(0);
      }
      else
         exit_error(1);
   }

   /* FUNZIONE goto x y <testo> */
   /* ========================= */
   if(strcmp(argv[1],"goto")==0)
   {
      if(argc==4)
      {
         gotoxy(atoi(argv[2]),atoi(argv[3]));
         exit(0);
      }
      if(argc==5)
      {
         gotoxy(atoi(argv[2]),atoi(argv[3]));
	 printf("%s",argv[4]);
	 exit(0);
      }
      exit_error(1);
   }

   /* FUNZIONE center riga testo */
   /* ========================== */
   if(strcmp(argv[1],"center")==0)
   {
      if(argc!=4)
         exit_error(1);
      gotoxy(atoi(argv[2]),40-strlen(argv[3])/2);
      printf("%s",argv[3]);
      exit(0);
   }

   /* FUNZIONE box x y z k  per grafica */
   /* ================================= */
   if(strncmp(argv[1],"box",3)==0)
   {
      if(strcmp(argv[1],"box")==0)
         ombra=FALSE;
      if(argc!=6)
         exit_error(1);
      tbox(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),ombra, TRUE);
      exit(0);
   }

   /* FUNZIONE BOX x y z k  per semi-grafica */
   /* ====================================== */
   if(strncmp(argv[1],"BOX",3)==0)
   {
      if(strcmp(argv[1],"BOX")==0)
         ombra=FALSE;
      if(argc!=6)
         exit_error(1);
      tbox(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),ombra, FALSE);
      exit(0);
   }

   /* FUNZIONE alert testo */
   /* ==================== */
   if(strncmp(argv[1],"alert",5)==0)
   {
      if(strcmp(argv[1],"alert")==0)
         ombra=FALSE;
      if(argc==4)
      {
         strcpy(titolo, argv[2]);
	 array_assign(argv[3]);
      }
      if(argc==3)
      {
         strcpy(titolo, "");
	 array_assign(argv[2]);
      }
      if((argc<3)||(argc>4))
         exit_error(2);
      t=11-(item+1+2)/2-1;  /* +1 per l'array item,2 per le cornici */
      l=40-((max_len+2)/2)-1;
      b=t+item+2;
      r=l+max_len+2+1;
      tbox(t, l, b, r, ombra, TRUE);
      if(*titolo)
      {
         putp(enter_reverse_mode);
	 gotoxy(t,40-strlen(titolo)/2);
	 printf("%s",titolo);
	 putp(exit_attribute_mode);
      }
      for(a=0;a<=item;a++) 
      {
	 if(strcmp(alert_item[a],"-")==0)
	 { 
   	    putp(enter_alt_charset_mode);
            gotoxy(t+1+a, l+1);
	    for(ww=l+1;ww<r;ww++)
	    {
		printf("%s", lab_f4);
	    }
   	    putp(exit_alt_charset_mode);
         }
	 else
	 {
            gotoxy(t+1+a, 40-strlen(alert_item[a])/2);
	    printf("%s", alert_item[a]);
	 }
      }
      exit(0);
   }

   /* FUNZIONE ALERT testo */
   /* ==================== */
   if(strncmp(argv[1],"ALERT",5)==0)
   {
      if(strcmp(argv[1],"ALERT")==0)
         ombra=FALSE;
      if(argc==4)
      {
         strcpy(titolo, argv[2]);
	 array_assign(argv[3]);
      }
      if(argc==3)
      {
         strcpy(titolo, "");
	 array_assign(argv[2]);
      }
      if((argc<3)||(argc>4))
         exit_error(2);
      t=11-(item+1+2)/2-1;  /* +1 per l'array item,2 per le cornici */
      l=40-((max_len+2)/2)-1;
      b=t+item+2;
      r=l+max_len+2+1;
      tbox(t, l, b, r, ombra, FALSE);
      for(a=0;a<=item;a++) 
      {
	 if(strcmp(alert_item[a],"-")==0)
	 { 
            gotoxy(t+1+a, l+1);
	    for(ww=l+1;ww<r;ww++)
	       printf("-");
         }
	 else
	 {
            gotoxy(t+1+a, 40-strlen(alert_item[a])/2);
	    printf("%s", alert_item[a]);
	 }
      }
      if(*titolo)
      {
         putp(enter_reverse_mode);
	 gotoxy(t,40-strlen(titolo)/2);
	 printf("%s",titolo);
	 putp(exit_attribute_mode);
      }
      exit(0);
   }
   /* FUNZIONE Inizio Pagina (Home) */
   /* ============================= */
   if(strcmp(argv[1],"home")==0)
   {
      if(argc!=2)
         exit_error(1);
	 putp(cursor_home);
	 exit(0);
   }

   /* FUNZIONE clreol */
   /* =============== */
   if(strcmp(argv[1],"clreol")==0)
   {
      if(argc!=2)
         exit_error(1);
         putp(clr_eol);
	 exit(0);
   }

   /* FUNZIONE clreos */
   /* =============== */
   if(strcmp(argv[1],"clreos")==0)
   {
      if(argc!=2)
         exit_error(1);
      putp(clr_eos);
      exit(0);
   }

   /* FUNZIONE Delete linea */
   /* ===================== */
   if(strcmp(argv[1],"dline")==0)
   {
      if(argc!=2)
         exit_error(1);
       putp(delete_line);
       exit(0);
   }

   /* FUNZIONE right [riga] [colonna] [testo] */
   /* ======================================= */
   if(strcmp(argv[1],"right")==0)
   {
      if(argc!=5)
         exit_error(1);
      gotoxy(atoi(argv[2]), atoi(argv[3])-strlen(argv[4]));	
      printf("%s", argv[4]);
      exit(0);
   }

   /* FUNZIONE Repeat [carattere|line] [quantita] */
   /* =========================================== */
   if(strcmp(argv[1],"repeat")==0)
   {
      if(argc!=4)
         exit_error(1);
      if(strcmp(argv[3],"line")==0)
      {
         putp(enter_alt_charset_mode);
	 for(a=0;a<atoi(argv[2]);a++)
	    printf("%s", lab_f4);
	 putp(exit_alt_charset_mode);
	 exit(0);
      }
      if(strcmp(argv[3],"pad")==0)
      {
         putp(enter_alt_charset_mode);
	 for(a=0;a<atoi(argv[2]);a++)
	    printf("%s", lab_f6);
	 putp(exit_alt_charset_mode);
 	 exit(0);
      }
      for(a=0;a<atoi(argv[2]);a++)
         printf("%s", argv[3]);
      exit(0);
   }

   /* FUNZIONE Flash screen */
   /* ===================== */
   if(strcmp(argv[1],"flash")==0)
   {
      if(argc!=2)
         exit_error(1);
      putp(flash_screen);
      exit(0);
   }

   /* FUNZIONE Abilitazione Reverse */
   /* ============================= */
   if(strcmp(argv[1],"rev")==0)
   {
      if(argc==3)
      {
         putp(enter_reverse_mode);
	 printf("%s",argv[2]);
	 putp(exit_attribute_mode);
	 exit(0);
      }
      if(argc==2)
      {
         putp(enter_reverse_mode);
	 exit(0);
      }
      exit_error(1);
   }

   /* FUNZIONE Abilitazione Blink */
   /* =========================== */
   if(strcmp(argv[1],"blink")==0)
   {
      if(argc==3)
      {
         putp(enter_blink_mode);
	 printf("%s",argv[2]);
	 putp(exit_attribute_mode);
	 exit(0);
      }		
      if(argc==2)
      {
         putp(enter_blink_mode);
	 exit(0);
      }
      exit_error(1);
   }

   /* FUNZIONE Abilitazione Bold */
   /* ========================== */
   if(strcmp(argv[1],"bold")==0)
   {
      if(argc==3)
      {
         putp(enter_bold_mode);
	 printf("%s",argv[2]);
	 putp(exit_attribute_mode);
	 exit(0);
      }
      if(argc==2)
      {
         putp(enter_bold_mode);
	 exit(0);
      }
      exit_error(1);
   }

  /* FUNZIONE Disabilitazione attributi, (Reverse, Blink, Bold) */
  /* ========================================================== */
  if(strcmp(argv[1],"attroff")==0)
  {
     if(argc!=2)
        exit_error(1);
     putp(exit_attribute_mode);
     exit(0);
   }

   /* FUNZIONE Abilitazione e Disabilitazione Grafica */
   /* =============================================== */
   if(strcmp(argv[1],"grafon")==0)
   {
      if(argc!=2)
         exit_error(1);
      putp(enter_alt_charset_mode);
	 exit(0);
   }
   if(strcmp(argv[1],"grafoff")==0)
   {
      if(argc!=2)
         exit_error(1);
      putp(exit_alt_charset_mode);
      exit(0);
   }

   /* FUNZIONE Abilitazione e Disabilitazione Underline */
   /* ================================================= */
   if(strcmp(argv[1],"underlineon")==0)
   {
      if(argc!=2)
         exit_error(1);
      putp(enter_underline_mode);
      exit(0);
   }
   if(strcmp(argv[1],"underlineoff")==0)
   {
      if(argc!=2)
         exit_error(1);
      putp(exit_underline_mode);
      exit(0);
   }

   /* FUNZIONE Logo per schermo standard  */
   /* ==================================  */
   if((strcmp(argv[1],"logo")==0)||(strcmp(argv[1],"LOGO")==0))
   {
      if(argc!=3)
         exit_error(1);
      if(strcmp(argv[1],"logo")==0)
	  graph=TRUE;
      else
	  graph=FALSE;
 
      strcpy(nometemp, "\0");
      strcpy(nometemp, tmpnam(nometemp));
      strcpy(linea, "uuname -l > ");
      strcat(linea, nometemp);
      system(linea);
      infile=fopen(nometemp, "r");
      getlinea(sistema);
      fclose(infile);
      unlink(nometemp);
      strcpy(padre, argv[2]);
      initscr();
      tp=time(0L);
      sprintf(datario, "%.2d/%.2d/%.2d",\
      localtime(&tp)->tm_mday,\
      localtime(&tp)->tm_mon+1,\
      localtime(&tp)->tm_year);
      sprintf(orario, "%.2d:%.2d",\
      localtime(&tp)->tm_hour,\
      localtime(&tp)->tm_min);

      move(0,0);
      standout();
      addstr("                                                                                ");
      move(0,1);
      if(getenv("MAGAZZINO"))
      {
         strcpy(iper, getenv("MAGAZZINO"));
         printw("%s, ", iper);
      }
      printw("%s", datario);
      move(0,79-strlen(orario));
      printw("%s", orario);
      standend();
      move(1,0);
      if(graph)
      {
      	attrset(A_ALTCHARSET);
      	for(p=0;p!=80;p++)
           	addch(*lab_f4);
        attroff(A_ALTCHARSET);
        move(21,0);
        attrset(A_ALTCHARSET);
        for(p=0;p!=80;p++)
         	addch(*lab_f4);
      	attroff(A_ALTCHARSET);
      }
      else
      {
      	for(p=0;p!=80;p++)
           	printw("-");
        move(21,0);
        for(p=0;p!=80;p++)
           	printw("-");
      }

      standout();
      move(22,0);
      addstr("                                                                                ");
      move(22,0);
      if(getenv("REPARTO"))
      {
         strcpy(infodir, getenv("REPARTO"));
         printw(" %s,", infodir);
      }
      if(getenv("LPDEST"))
         strcpy(stamp, getenv("LPDEST"));
      else
         strcpy(stamp, "DEFAULT\0");
	
      if(getenv("LOGNAME"))
         strcpy(login_name, getenv("LOGNAME"));
      else
         strcpy(login_name, "none");

      sprintf(riga, " %s %s:%s,[%s] %s",\
         sistema, login_name, ttyname(fileno(stdin)), termname(), stamp);
      printw("%s", riga);
      move(22,79-strlen(padre));
      printw("%s", padre);
      refresh();
      endwin();
      exit(0);
   }

   /* FUNZIONE Emissione segnale acustico */
   /* =================================== */
   if(strcmp(argv[1],"bell")==0)
   {
      if(argc>3)
         exit_error(1);
      if(argc==2)
         putp(bell);
      else
      {
         for(bell_a=0;bell_a<(atoi(argv[2])*10);bell_a++)
         putp(bell);
      }
      exit(0);
   }

/* FUNZIONE rnd max_rnd_number */
/* =========================== */
   if(strcmp(argv[1],"rnd")==0)
   {
      if(argc!=3)
         exit_error(1);
      else
      {
	srand(time(0L));
	n1=1+(int)(atoi(argv[2]) * (rand() / 32768.0));
	printf("%d \n", n1);
      }
      exit(0);
   }

exit_error(2);
}
/* POSIZIONAMENTO CURSORE */
/* ====================== */
void gotoxy(int row, int col)
{
/*
   int a;
   putp(cursor_home);
   for(a=0;a<row;a++)
      putp(cursor_down);
   for(a=0;a<col;a++)
      putp(cursor_right);
*/
   printf("%s",tgoto(cursor_address, col, row));
}

void tbox(int box_t, int box_l, int box_b, int box_r, int shadow, int graphic)
{
   int a=0;
   int b=0;
 
   if(graphic)
   	putp(enter_alt_charset_mode);
   gotoxy(box_t, box_l);	

   if(graphic)				/* Primo vertice SX */
   	printf("%s", lab_f0);   
   else
   	printf("+");   

   if(graphic)				/* Primo Bordo alto */
   {
   	for(a=box_l+1;a<box_r;a++)		
      	   printf("%s", lab_f4);
   }
   else
   {
   	for(a=box_l+1;a<box_r;a++)	
      	   printf("-");
   }
   if(graphic)				/* Primo vertice DX */
   	printf("%s", lab_f1);   
   else
   	printf("+");   

   for(a=box_t+1;a<box_b;a++)
   {
      if(shadow==1)
      {
         gotoxy(a, box_l-1);	/* Ombra laterale */
	 if(graphic)
	 	printf("%s", lab_f6);
	 else
	 	printf("#");
      }
      gotoxy(a, box_l);	

      if(graphic)
      	  printf("%s", lab_f5);
      else
      	  printf("|");

      if(graphic)
      	 putp(exit_alt_charset_mode);

      for(b=box_l+1;b<box_r;b++)
	 printf(" ");
      if(graphic)
      {
      	 putp(enter_alt_charset_mode);
         printf("%s", lab_f5);
      }
      else
         printf("|");
   }
   if(shadow==1)
   {
      gotoxy(a, box_l-1);	/* Ombra vertice */	
      if(graphic)
         printf("%s", lab_f6);
      else
         printf("#");
   }
   gotoxy(a, box_l);	
   if(graphic)
   	printf("%s", lab_f3);
   else
   	printf("+");
	

   if(graphic)
   {
   	for(b=box_l+1;b<box_r;b++)
           printf("%s", lab_f4);
   }
   else
   {
   	for(b=box_l+1;b<box_r;b++)
           printf("-");
   }

   if(graphic)
      printf("%s", lab_f2);
   else
      printf("+");

   if(shadow==1)
   {
      gotoxy(a+1, box_l-1);	/* Ombra Inferiore */
      if(graphic)
      {
      	for(a=box_l;a<box_r;a++)
           printf("%s", lab_f6);
      }
      else
      {
      	for(a=box_l;a<box_r;a++)
           printf("#");
      }
   }
   if(graphic)
      putp(exit_alt_charset_mode);
}

/* ASSEGNA AD UN ARRAY I TESTI PASSATI DA alert[_s] */
/* ================================================ */
void array_assign(char *alert_list)
{
   int scan_pointer=0;
   int a=0;
   int c=0;

   while(scan_pointer<strlen(alert_list))
   {
      if((alert_list[scan_pointer]=='|') || (alert_list[scan_pointer]=='\n'))
      {
         alert_item[item][a]='\0';
	 scan_pointer++;
	 item++;
	 a=0;
      }
      else
      {
         alert_item[item][a]=alert_list[scan_pointer++];
	 a++;
      }
   }
   for(c=0,max_len=0;c<=item;c++)
   {
   if(strlen(alert_item[c])>max_len)
      max_len=strlen(alert_item[c]);
   }
}

/* USCITE CON ERRORI.... */
/* ===================== */
void exit_error(int errore)
{
   Info();
   switch(errore)
   {
      case  1 :
	 printf("PARAMETRI MANCANTI\n\n");
	 break;	
      case  2 :
	 printf("FUNZIONE NON ESISTENTE\n\n");
	 break;	
   }
/*
   putp(reset_1string);
   putp(reset_2string);
   putp(reset_3string);
*/
   exit(errore);
}

/* MEMORIZZA IN UNA VARIABILE IL CONTENUTO DI UNA LINEA DI UN FILE LETTO */
/* ===================================================================== */
void getlinea(char *tamp)      
{
   int ii=0;
   int c=0;
   while((c=getc(infile)) != EOF)
   {
      if (c=='\n')
      {
         tamp[ii]=0;
         return;
      }
      tamp[ii++]=c;
      tamp[ii]=0;
   }
   return;
}

/* RIASSUNTO DEI COMANDI */
/* ===================== */
void Info()
{
printf("\
%s\
     ver 1.0, (C)Copyright Settembre 1994,  - Prima versione\n\
     vel 2.0, (C)Copyright Maggio    1995,  - Corretta descrizione del mese\n\
     vel 2.1, (C)Copyright Settembre 1995,  - Sostituito getlogin con getenv\n\
     vel 2.3, (C)Copyright Settembre 1995,  - Aggiunta semi-grafica\n\
     vel 2.4, (C)Copyright Novembre  1995,  - Aggiunta funzione rnd\n\
\n\
Sintassi : vde <funzione> [arg 1] ... [arg n] :\n\
           vde -h : Help esteso.\n\
\n\
Elenco Funzioni\n\
---------------\n\
- goto    row col [text] 			- home\n\
- center  row text  				- dline\n\
- box|BOX     up left down right       		- flash\n\
- box_s|BOX_S   up left down right     		- clreol\n\
- alert|ALERT   titolo \"text_1 |...| text_n\"	- clreos\n\
- alert_s|ALERT_S titolo \"text_1 |...| text_n\"  - attron\n\
- bell 	  [delay]				- attroff\n\
- rev 	  [text]				- grafon\n\
- blink   [text] 				- grafoff\n\
- bold 	  [text]                                - underlineon\n\
- repeat  cicli character|[line]|[pad] 	 	- underlineoff\n\
- right	  row col text	                        - logo|LOGO <titolo>\n\
- random_name                                   - shroot <comando>\n\
- data    [-o]                                  - rnd max_random_number\n\
\n",TESTATA);
}

/* ISTRUZIONI PIU' DETTAGLIATE DELLE FUNZIONI */
/* ========================================== */
void Instructions()
{
int junk;
/* PAGINA 1 */
/* ======== */
printf("\
%s\n\
Elenco funzioni :\n\
\n\
goto [riga] [colonna] <testo>\n\
\t Posizionamento del cursore nelle coordinate specificate.\n\
\n\
center [riga] [testo]\n\
\t Centratura della stringa [testo] nella riga [riga].\n\
\n\
box   [alto] [sinistra] [basso] [destra]\n\
\t Disegna una finestra nella coordinate desiderate.\n\
\n\
box_s [alto] [sinistra] [basso] [destra]\n\
\t Disegna una finestra con ombreggiatura nella coordinate desiderate.\n\
\n\
alert [titolo] [testo 1|testo 2|....|testo n]\n\
\t Stampa n righe di testo in una finestra giustificata a centro\n\
\t video. Il separatore dei testi e' il simbolo | (pipe).\n\
\t Se nella successione del testo si inserisce un segno meno, nella\n\
\t finestra sara' inoltre disegnata una linea di separazione :\n\
\t Es : vde alert \" ATTENZIONE \" \"LINEA |-| DI TESTO \"\n\
\n\
", TESTATA);
printf("Premi <Invio> per continuare");
junk=getchar();

/* PAGINA 2 */
/* ======== */
printf("\
%s\n\
Elenco funzioni :\n\
\n\
alert_s [titolo] [testo 1|testo 2|....|testo n]\n\
\t Stampa n righe di testo in una finestra disegnata a centro\n\
\t video con ombreggiatura. Il separatore testi e' il simbolo | (pipe)\n\
\t Se nella successione del testo si inserisce un segno meno, nella\n\
\t finestra sara' inoltre disegnata una linea di separazione :\n\
\t Es : vde alert \" ATTENZIONE \" \"LINEA |-| DI TESTO \"\n\
\n\
home\n\
\t Posiziona il cursore nelle coordinate 0, 0\n\
\n\
dline\n\
\t Elimina la riga sottostante il cursore\n\
\n\
flash\n\
\t Esegue un lampeggio del video come avvertimento visivo\n\
\t Se il terminale non lo prevede emette un suono.\n\
\n\
\n\
\n\
", TESTATA);
printf("Premi <Invio> per continuare");
junk=getchar();

/* PAGINA 3 */
/* ======== */
printf("\
%s\n\
Elenco funzioni :\n\
\n\
rev <testo>\n\
\t Abilita il reverse sul output video. Se seguito dal testo\n\
\t <testo> stampa a video e disabilita il reverse\n\
\n\
clreol\n\
\t Cancella dalla posizione del cursore alla fine della riga\n\
\n\
clreos\n\
\t Cancella dalla posizione del cursore alla fine dello schermo\n\
\n\
blink <testo>\n\
\t Abilita il lampeggio dell' output.Se seguito dal testo\n\
\t <testo> stampa a video e disabilita il blink.\n\
\n\
bold <testo> \n\
\t Abilita l'alta luminosita' dell' output. Se seguito dal testo\n\
\t <testo> stampa a video e disabilita il bold.\n\
\n\
\n\
",TESTATA);
printf("Premi <Invio> per continuare");
junk=getchar();

/* PAGINA 4 */
/* ======== */
printf("\
%s\n\
Elenco funzioni :\n\
\n\
attroff\n\
\t Disabilita gli attributi rev, blink, bold.\n\
\n\
grafon\n\
\t Attiva la modalita' grafica.\n\
\n\
grafoff\n\
\t Disattiva la modalita' grafica.\n\
\n\
underlineon\n\
\t Attiva la modalita' underline (sottolineata) sull' output.\n\
\n\
underlineoff\n\
\t Disabilita la modalita' underline (sottolineata).\n\
\n\
bell <ritardo>\n\
\t Emette un suono come avvertimento per il numero di cicli\n\
\t specificati in <ritardo>. Il ritardo e' facoltativo\n\
\n\
", TESTATA);
printf("Premi <Invio> per continuare");
junk=getchar();

/* PAGINA 5 */
/* ======== */
printf("\
%s\n\
Elenco funzioni :\n\
\n\
repeat [n_volte] [stringa]\n\
\t Stampa [n_volte] la [stringa] passata come parametro. Se stringa \n\
\t assume il valore \"line\" la stampa a video crea una riga con i \n\
\t caratteri grafici, il valore \"pad\" stampa un carattere ombreggiato.\n\
\n\
right [riga] [colonna] [testo]\n\
\t Stampa nella [riga] [colonna] indicata il [testo] giust. a destra.\n\
\n\
logo [titolo]\n\
\t Crea una maschera inserendo in alto a destra il contenuto della\n\
\t variabile d' ambiente MAGAZZINO e in basso a destra il contenuto\n\
\t dell variabile d' ambiente REPARTO.\n\
\n\
random_name\n\
\t Crea un nome temporaneo\n\
\n\
data [-o]\n\
\t Ritorna una stringa con data e ora in lingua italiana. Lo switch -o\n\
\t esclude l' orario dalla stringa.\n\
\n\
rnd max_number_ran\n\
\t Ritorna un numero casuale compreso fra 0 e max_number_ran.\n\
\n\
",TESTATA);
}
