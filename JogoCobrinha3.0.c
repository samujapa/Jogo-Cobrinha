#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<string.h>

#define ALTURA 50
#define LARGURA 80

typedef struct InfoCobra{
	int Tam,
		Pos[2],
		*RastroX,
		*RastroY;
	char Direcao;
}snake;

typedef struct InfoSave{
	char Nome[20],
		 Dif;
	int Pontuacao;
	
}save;

void menu1P();
void jogo1P();
void jogo2P();
void mgotoxy(int x,int y);
void imprimeMapa();
void movimenta(snake *Cobra);
void rastro(snake *Cobra);
void geraComida(int PComida[]);
int checaComida(snake *Cobra,int PComida[]);
int colisao(int Pos[],snake *Cobra);
void salvaPontuacao(int Pontuacao,int Dif);
void mostraPontuacoes();


int main(){
	int Aux=0;
	
	do{
		do{
			system("cls");
			printf("1:Jogo Solo\n");
			printf("2:Jogar 2 Players\n");
			printf("3:Sair\n");
			scanf("%d",&Aux);
			fflush(stdin);
		}while(Aux!=1 && Aux!=2 && Aux!=3);
		
		
		if(Aux==1){
			menu1P();
		}else if(Aux==2){
			jogo2P();
		}
		
		
	
		
		
	}while(Aux!=3);	
}

void menu1P(){
	int Aux=0;
	do{
		system("cls");
		printf("1:Jogar\n");
		printf("2:Pontuacoes\n");
		printf("3:Menu principal\n");
		printf("4:Sair\n");	
		scanf("%d",&Aux);
		fflush(stdin);
	}while(Aux!=1 && Aux!=2 && Aux!=3 && Aux!=4);
	
	if(Aux==1){
		system("cls");
		printf("Aperte WASD para se mover\n");
		printf("Coma e cresca!!\n");
		printf("Bater em si mesmo ou nas paredes resulta em morte!");
		getch();
		jogo1P();
	}else if(Aux==2){
		mostraPontuacoes();
	}else if(Aux==3) main();
	else exit(1);
	
	
	
	
	
}



void jogo1P(){
	snake *Cobra;
	int Perdeu=0;
	int PComida[2];
	char Tmp='�';
	int Dif;
	
	do{
		system("cls");
		printf("Digite a dificulda com que deseja jogar: F/M/D/I\n");
		scanf("%c",&Tmp);
		fflush(stdin);
		Tmp=toupper(Tmp);
	}while(Tmp!='F' && Tmp!='M' && Tmp!='D'  && Tmp!='I');
	
	if(Tmp=='F') Dif=80;
	else if(Tmp=='M') Dif=50;
	else if(Tmp=='D') Dif=30;
	else Dif=10;
	
	Tmp='�';
	
	
	
	
	Cobra=(snake*)malloc(sizeof(snake));
	Cobra->RastroX=(int*)calloc(4000,sizeof(int));
	Cobra->RastroY=(int*)calloc(4000,sizeof(int));
	Cobra->Tam=1;
	Cobra->Direcao='d';
	Cobra->Pos[0]=1;
	Cobra->Pos[1]=1;
	
	system("cls");
	imprimeMapa();
	geraComida(PComida);
	mgotoxy(1,1);
	
	while(!Perdeu){
		
		while(!kbhit() && !Perdeu){
			rastro(Cobra);
			movimenta(Cobra);
			Perdeu=colisao(Cobra->Pos,Cobra);
			if(Cobra->Pos[0]==0 || Cobra->Pos[0]==LARGURA || Cobra->Pos[1]==0 || Cobra->Pos[1]==ALTURA) Perdeu=1;
			
			if(checaComida(Cobra,PComida)) Cobra->Tam++;//checa se tem comida, se tiver aumenta Tam
			while(colisao(PComida,Cobra)) geraComida(PComida);//enquanto a comida "nascer" numa posi�ao q o corpo ja ocupa, ela vai nascer em outro lugar
			
			Sleep(Dif);
			
			mgotoxy(0,ALTURA+3);
			printf("Tam: %d // %d      ",Cobra->Pos[0],Cobra->Pos[1]);
			
		}
		Tmp=getch();
		if(Tmp=='w' || Tmp=='a' || Tmp=='s' || Tmp=='d'){
			Cobra->Direcao=Tmp;
		}
		
		
	}
	
	

	salvaPontuacao(Cobra->Tam,Dif);

	free(Cobra);
}

void jogo2P(){
	snake *Cobra,*Cobra2;
	int Perdeu=0;
	int PComida[2];
	char Tmp;
	
	//alocando e definindo valores pra cobra
	Cobra=(snake*)malloc(sizeof(snake));
	Cobra->RastroX=(int*)calloc(4000,sizeof(int));
	Cobra->RastroY=(int*)calloc(4000,sizeof(int));
	Cobra->Tam=1;
	Cobra->Direcao='d';
	Cobra->Pos[0]=1;
	Cobra->Pos[1]=1;
	
	//alocando e definindo valores pra cobra2
	Cobra2=(snake*)malloc(sizeof(snake));
	Cobra2->RastroX=(int*)calloc(4000,sizeof(int));
	Cobra2->RastroY=(int*)calloc(4000,sizeof(int));
	Cobra2->Tam=1;
	Cobra2->Direcao='a';
	Cobra2->Pos[0]=LARGURA-1;
	Cobra2->Pos[1]=ALTURA-1;
	
	imprimeMapa();
	geraComida(PComida);
	mgotoxy(1,1);
	
	while(!Perdeu){
		
		while(!kbhit() && !Perdeu){
			if(Cobra->Pos[0]==Cobra2->Pos[0] && Cobra->Pos[1]==Cobra2->Pos[1]){// checa se um nao bateu de cabeca com o outro;
				Perdeu=3;
				break;
			}
			
			rastro(Cobra);
			movimenta(Cobra);
			if(Cobra->Pos[0]==LARGURA) Cobra->Pos[0]=1;
			if(Cobra->Pos[0]==0) Cobra->Pos[0]=LARGURA-1;
			if(Cobra->Pos[1]==ALTURA) Cobra->Pos[1]=1;
			if(Cobra->Pos[1]==0) Cobra->Pos[1]=ALTURA-1;
			
			Perdeu=colisao(Cobra->Pos,Cobra);//checa se a primeira cobra nao bateu em si mesma
			if(Perdeu) break;
			Perdeu=colisao(Cobra->Pos,Cobra2);//checa se a primeira cobra bateu no rabo da segunda
			if(Perdeu) break;
			
			if(checaComida(Cobra,PComida)) Cobra->Tam++;
			while(colisao(PComida,Cobra) || colisao(PComida,Cobra2) ) geraComida(PComida);//enquanto a comida "nascer" numa posi�ao q o corpo de alguma das duas cobras ja ocupa, ela vai nascer em outro lugar
			//-----------------------------------------------------------------//	
			if(Cobra->Pos[0]==Cobra2->Pos[0] && Cobra->Pos[1]==Cobra2->Pos[1]){// checa se um nao bateu de cabeca com o outro;
				Perdeu=3;
				break;
			}
					
			rastro(Cobra2);
			movimenta(Cobra2);
			if(Cobra2->Pos[0]==LARGURA) Cobra2->Pos[0]=1;
			if(Cobra2->Pos[0]==0) Cobra2->Pos[0]=LARGURA-1;
			if(Cobra2->Pos[1]==ALTURA) Cobra2->Pos[1]=1;
			if(Cobra2->Pos[1]==0) Cobra2->Pos[1]=ALTURA-1;
			
			Perdeu=colisao(Cobra2->Pos,Cobra2);// checa se a segunda cobra nao bateu em si mesma
			if(Perdeu){
				Perdeu=2;
				break;
			}
			Perdeu=colisao(Cobra2->Pos,Cobra);//checa se a segunda cobra bateu no rabo da primeira
			if(Perdeu){
				Perdeu=2;
				break;
			}
			
			if(checaComida(Cobra2,PComida)) Cobra2->Tam++;
			while(colisao(PComida,Cobra) || colisao(PComida,Cobra2) ) geraComida(PComida);//enquanto a comida "nascer" numa posi�ao q o corpo de alguma das duas cobras ja ocupa, ela vai nascer em outro lugar
			
			Sleep(100);
			
			mgotoxy(0,ALTURA+3);
			printf("Tam: %d    Tam2: %d      ",Cobra->Tam,Cobra2->Tam);
			
		}		
		Tmp=getch();
		if(Tmp=='w' || Tmp=='a' || Tmp=='s' || Tmp=='d'){
			Cobra->Direcao=Tmp;
		}
		
		if(Tmp=='i') Cobra2->Direcao='w';
		else if(Tmp=='j') Cobra2->Direcao='a';
		else if(Tmp=='k') Cobra2->Direcao='s';
		else if(Tmp=='l') Cobra2->Direcao='d';
		
		
	}
	
	system("cls");
	if(Perdeu==1){
		printf("Jogador 1 perdeu, Parabens jogador 2!!");
	}else if(Perdeu==2){
		printf("Jogador 2 perdeu, Parabens jogador 1!!");
	}else if(Perdeu==3){
		printf("EMPATE, um jogador bateu a cabeca no outro");
	}
	
	free(Cobra);
	
	getch();
}



void mgotoxy(int x,int y){
	COORD p={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),p);
}


void imprimeMapa(){
	system("mode 100,64");
	int i;
	
	//parede da esquerda e parede da direita
	for(i=0;i<=ALTURA;i++){
		mgotoxy(0,i);
		printf("#");
	}
	for(i=0;i<=ALTURA;i++){
		mgotoxy(LARGURA,i);
		printf("#");
	}
	
	//parede de cima e parede de baixo
	for(i=0;i<=LARGURA;i++){
		mgotoxy(i,0);
		printf("#");
	}
	for(i=0;i<=LARGURA;i++){
		mgotoxy(i,ALTURA);
		printf("#");
	}
	
}




void movimenta(snake *Cobra){
	
//	mgotoxy(Pos[0],Pos[1]);
//	printf(" ");
	
		if (Cobra->Direcao=='a') {
			Cobra->Pos[0]=Cobra->Pos[0]-1; //anda pra esquerda
			mgotoxy(Cobra->Pos[0],Cobra->Pos[1]);
			printf("O");
		}
		else if	(Cobra->Direcao=='d'){
			Cobra->Pos[0]=Cobra->Pos[0]+1;//anda pra direita
			mgotoxy(Cobra->Pos[0],Cobra->Pos[1]);
			printf("O");
		}	
		else if (Cobra->Direcao=='w'){
			Cobra->Pos[1]=Cobra->Pos[1]-1;//anda pra cima
			mgotoxy(Cobra->Pos[0],Cobra->Pos[1]);
			printf("O");
		}
		else{
			Cobra->Pos[1]=Cobra->Pos[1]+1;//anda pra baixo
			mgotoxy(Cobra->Pos[0],Cobra->Pos[1]);
			printf("O");
		}
			
}



void rastro(snake *Cobra){
	int i;
	
	//apaga ultimo rastro
	if(Cobra->RastroX[Cobra->Tam-1]!=0 && Cobra->RastroY[Cobra->Tam-1]!=0){// quando come a ultima posi��o � q 0,0 isso impede q apague um peda�o do mapa
		mgotoxy(Cobra->RastroX[Cobra->Tam-1],Cobra->RastroY[Cobra->Tam-1]);
		printf(" ");
	}
	
	
	//move as posi��es dos rastros
	for(i=Cobra->Tam-1;i>0;i--){
		Cobra->RastroX[i]=Cobra->RastroX[i-1];
		Cobra->RastroY[i]=Cobra->RastroY[i-1];
	}
	
	//Rastro recebe a posi��o atual
	Cobra->RastroX[0]=Cobra->Pos[0];
	Cobra->RastroY[0]=Cobra->Pos[1];
	
	//Coloca um X na posi��o atual,  Logo em seguida no "jogo" ele vai pra proxima posi��o (na mesma itera��o do while
	mgotoxy(Cobra->RastroX[0],Cobra->RastroY[0]);
	printf("X");

	

	
}



void geraComida(int PComida[]){
	srand(time(NULL));
	
	
	PComida[0]=rand()%LARGURA-1;
	PComida[1]=rand()%ALTURA-1;
	
	if(PComida[0]<=0) PComida[0]=1;
	if(PComida[1]<=0) PComida[1]=1;
	
	mgotoxy(PComida[0],PComida[1]);
	printf("@");	
}

int checaComida(snake *Cobra,int PComida[]){
	if(Cobra->Pos[0]==PComida[0] && Cobra->Pos[1]==PComida[1]){
		geraComida(PComida);
		return 1;
	}else
		return 0;
	
}



int colisao(int Pos[2],snake *Cobra){
	int i;
	
	for(i=0;i<Cobra->Tam;i++){
		if(Pos[0]==Cobra->RastroX[i] && Pos[1]==Cobra->RastroY[i]){
			return 1;
		}
	}
	return 0;
}


void salvaPontuacao(int Pontuacao,int Dif){
	char Nome[20],Resp;
	save Info;
	FILE *Arq;
	Arq=fopen("pontuacao","r+");
	if(Arq==NULL){
		Arq=fopen("pontuacao","w");
	}
	system("cls");
	printf("Deseja salvar a pontuacao atual(%d) ?(S/N) ",Pontuacao);
	do{
		Resp=getch();	
		Resp=toupper(Resp);
	}while(Resp!='S' && Resp!='N');
	if(Resp=='N'){
		return;
	}
	
	//reaproveitando Resp
	if(Dif==10) Resp='I';
	else if(Dif==30) Resp='D';
	else if(Dif==50) Resp='M';
	else  Resp='F';
	
	printf("\nDigite o nome com qual deseja salvar(Max 20 char)");
	fgets(Nome,20,stdin);
	
	strcpy(Info.Nome,Nome);
	//Info.Nome=Nome;
	Info.Pontuacao=Pontuacao;
	Info.Dif=Resp;
	
	fseek(Arq,0,2);
	fwrite(&Info,sizeof(save),1,Arq);
	fclose(Arq);
	printf("\nInfo.Dif: %c \nInfo.Nome: %sInfo.Pontuacao: %d",Info.Dif,Info.Nome,Info.Pontuacao);
	printf("\nDificuldade: %c \nNome: %s\nPontuacao: %d",Dif,Nome,Pontuacao);
	getch();
}

void mostraPontuacoes(){
	system("cls");
	
	save Info;
	FILE *Arq;
	Arq=fopen("pontuacao","r");//abre arquivo pra leitura
	if(Arq==NULL){
		printf("N�o hq nenhuma pontuacao salva");//se nao tiver arquivo � pq ainda nao tem nenhum save
		getch();
	}else{
		printf("Pontuacoes");
		printf("\n");
		
		fread(&Info,sizeof(save),1,Arq);
		while(!feof(Arq)){//enquanto nao for o fim do arquivo, ele le as novas informa��es e printa elas, entao l� novamente
			printf("\nDificuldade: %c \nNome: %sPontuacao: %d",Info.Dif,Info.Nome,Info.Pontuacao);
			printf("\n");
			fread(&Info,sizeof(save),1,Arq);
		}
		getch();		
	}
	
	fclose(Arq);
	
}





