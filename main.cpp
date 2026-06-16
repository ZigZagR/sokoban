#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>

using namespace std;

#define LINHA 5
#define COLUNA 5

bool movimentoValido(int x, int y, int matriz[LINHA][COLUNA]);
bool movimentoValidoCaixa(int x, int y, int matriz[LINHA][COLUNA]);
void gotoxy(int XPos, int YPos);
bool ganhou(int matriz[LINHA][COLUNA], int objetivos_totais);
void print_jogo(int matriz[LINHA][COLUNA], HANDLE h);
int contaObjetivos(int matriz[LINHA][COLUNA]);

int main() 
{
	// desativa o cursor no console
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 100;
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // handle da saída do console

	int matriz[LINHA][COLUNA] = 
	{
		{1, 1, 1, 1, 1},
		{1, 0, 0, 0, 1},
		{1, 2, 3, 5, 1},
		{1, 0, 0, 0, 1},
		{1, 1, 1, 1, 1}
	};
	// 0 = vazio, 1 = parede, 2 = player, 3 = caixa 4 = caixa no objetivo, 5 = objetivo

	int player_i, player_j;
	int memo_player = 0; // o que estava abaio do player

	for(int i = 0; i < LINHA; i++)
	{
		for(int j = 0; j < COLUNA; j++)
		{
			if(matriz[i][j] == 2) 
			{
				player_i = i;
				player_j = j;
				break;
			}
		}
	}

	int objetivos_totais = 0;
	for(int i = 0; i < LINHA; i++)
	{
		for(int j = 0; j < COLUNA; j++)
		{
			if(matriz[i][j] == 5) 
			{
				objetivos_totais++;
			}
		}
	}

	char tecla;
	cout << "\033[H\033[2J"; // limpa tela
	

	time_t inicio = time(0);
	const int TEMPO_LIMITE = 300; // em segundos

	while (difftime(time(0), inicio) < TEMPO_LIMITE && !ganhou(matriz, objetivos_totais)) 
	{
		gotoxy(0, 0); // clear
		cout << "Controles: WASD para se mover, Q para sair" << endl; 
		cout << "Empurre as caixas para os objetivos '.'\n";

		// imprime matriz
		print_jogo(matriz, h);
		
		cout << "Tempo restante: " << TEMPO_LIMITE - difftime(time(0), inicio) << "s...\n";
		
		if (kbhit()) 
		{
			tecla = getch(); 

			// sai no 'q' or ESC
			if (tecla == 'q' || tecla == 'Q' || tecla == 27) break;

			int novo_i = player_i, novo_j = player_j;

			switch (tecla) {
				case 'w': novo_i = player_i - 1; break; 
				case 's': novo_i = player_i + 1; break; 
				case 'a': novo_j = player_j - 1; break; 
				case 'd': novo_j = player_j + 1; break; 
				default: break;
			}

			if (!movimentoValido(novo_i, novo_j, matriz))
			{
				continue; // movimento inválido, ignora e passa o loop
			}

			if(matriz[novo_i][novo_j] == 3 || matriz[novo_i][novo_j] == 4)
			{
				int caixa_novo_i = novo_i + (novo_i - player_i); // posição que a caixa vai tentar se mover para 
				int caixa_novo_j = novo_j + (novo_j - player_j);
							
				if (!movimentoValidoCaixa(caixa_novo_i, caixa_novo_j, matriz))
				{
					continue; // ignora e pula pro prox iteração
				}

				// se * vira . denovo
				int memo_caixa; 
				if(matriz[novo_i][novo_j] == 4) 
				{
					memo_caixa = 5; 
				}
				else
				{
					memo_caixa = 0;
				}
				
				// move a caixa
				if (matriz[caixa_novo_i][caixa_novo_j] == 5) 
				{
					matriz[caixa_novo_i][caixa_novo_j] = 4; // no objetivo
				} 
				else 
				{
					matriz[caixa_novo_i][caixa_novo_j] = 3; // normal
				}

				matriz[novo_i][novo_j] = memo_caixa; // atualiza o que tá embaixo da caixa
			}

			matriz[player_i][player_j] = memo_player; // limpa posição ' '
			memo_player = matriz[novo_i][novo_j];

			player_i = novo_i; 
			player_j = novo_j;
			matriz[player_i][player_j] = 2; // reescreve '@' na nova posição
		}
		else 
		{
			// pra não fazer kaboom.
			Sleep(100); 
		}

	}

	time_t timer = difftime(time(0), inicio);
	if(ganhou(matriz, objetivos_totais))
	{
		gotoxy(0, 2); // clear -> resultado após testar 
		print_jogo(matriz, h);
		gotoxy(0, LINHA + 3);
		cout << "Voce ganhou em: " << timer << "s\n";
	}
	else
	{
		gotoxy(0, 2); // clear
		print_jogo(matriz, h);
		gotoxy(0, LINHA + 3);
		cout << "Tempo esgotado! Tente novamente.\n";
	}
	return 0;
}

bool movimentoValido(int x, int y, int matriz[LINHA][COLUNA])
{
	if (x < 0 || x >= LINHA || y < 0 || y >= COLUNA) 
	{
		return false; // fora dos limites
	}
	if (matriz[x][y] == 1)
	{
		return false; // parede
	}  
	return true;
}

bool movimentoValidoCaixa(int x, int y, int matriz[LINHA][COLUNA])
{
	if (x < 0 || x >= LINHA || y < 0 || y >= COLUNA) 
	{
		return false; // fora dos limites
	}
	if (matriz[x][y] == 1 || matriz[x][y] == 3 || matriz[x][y] == 4)
	{
		return false; // parede ou caixa
	}  
	return true;
}

// Código Prof
void gotoxy(int XPos, int YPos) 
{
	COORD coord;
	coord.X = XPos; // Propriedade console
	coord.Y = YPos;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool ganhou(int matriz[LINHA][COLUNA], int objetivos_totais)
{
	if(contaObjetivos(matriz) == objetivos_totais)
	{
		return true;
	}
	return false;
}

void print_jogo(int matriz[LINHA][COLUNA], HANDLE h)
{
	for (int i = 0; i < LINHA; i++) 
	{
		for (int j = 0; j < COLUNA; j++) 
		{
			switch (matriz[i][j]) 
			{
			case 0: // vazio
				cout << " ";
				break;
			case 1: // parede
				SetConsoleTextAttribute(h, BACKGROUND_BLUE); // fundo azul
				cout << " ";
				SetConsoleTextAttribute(h, 7); // volta ao normal (cinza claro sobre preto)
				break;
			case 2: // player
				cout << "@";
				break;
			case 3: // caixa
				cout << "#";
				break;
			case 4: // caixa no objetivo
				cout << "*";
				break;
			case 5: // objetivo
				cout << ".";
				break;
			default:
				cout << " ";
			}
		}
		cout << endl;
	}
}

int contaObjetivos(int matriz[LINHA][COLUNA])
{
	int caixas_no_objetivo = 0;
	for(int i = 0; i < LINHA; i++)
	{
		for(int j = 0; j < COLUNA; j++)
		{
			if(matriz[i][j] == 4)
			{
				caixas_no_objetivo++;
			}
		}
	}
	return caixas_no_objetivo;
}