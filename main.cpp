#include <conio.h>
#include <iostream>
#include <time.h>
#include <windows.h>

using namespace std;
#define MAX_LINHA 11
#define MAX_COLUNA 22

bool movimentoValido(int i, int j, int matriz[MAX_LINHA][MAX_COLUNA], int LINHA, int COLUNA);
bool movimentoValidoCaixa(int i, int j, int matriz[MAX_LINHA][MAX_COLUNA], int LINHA, int COLUNA);
void gotoxy(int XPos, int YPos);
bool ganhou(int matriz[MAX_LINHA][MAX_COLUNA], int objetivos_totais, int LINHA, int COLUNA);
void print_jogo(int matriz[MAX_LINHA][MAX_COLUNA], HANDLE h, int LINHA, int COLUNA);
int contaObjetivos(int matriz[MAX_LINHA][MAX_COLUNA], int LINHA, int COLUNA);
int jogo(int level);
void limpa_tela();

int main()
{
	int level = 0; // nível inicial
	while (true)
	{
		limpa_tela();
		cout << "==== Sokoban ====\n";
		cout << "1) Jogar\n";
		cout << "2) Ajuda\n";
		cout << "3) Sobre\n";
		cout << "4) Sair\n";
		cout << "Selecione uma opcao: ";

		char c = getch();
		if (c == '1')
		{
			level = jogo(level);
			cout << "\nPressione qualquer tecla para retornar ao menu...";
			getch();
		}
		else if (c == '2')
		{
			limpa_tela();
			cout << "Ajuda:\n";
			cout << "WASD para se mover, empurrar caixas (#) para os objetivos "
				"(.)\n";
			cout << "Pressione 'q' para sair do jogo\n";
			cout << "Pressione qualquer tecla para retornar ao menu...";
			getch();
		}
		else if (c == '3')
		{
			system("cls");
			cout << "Equipe de desenvolvimento:\n- Pedro Henrique R. J. "
				"Nicolini\n- "
				"Luiz Eduardo F. N. Goncalves\n";
			cout << "Junho/2026\n";
			cout << "Professor: Thiago Felski\nDisciplina: Algoritmos e "
				"Programacao\n\n";
			cout << "Pressione qualquer tecla para voltar ao menu...";
			getch();
		}
		else if (c == '4' || c == 27)
		{
			limpa_tela();
			break; // sair
		}
	}
	return 0;
}

void limpa_tela()
{
	cout << "\033[H\033[2J"; // limpador de tela universal ANSI -> Stackoverflow
}

int jogo(int level)
{
	// desativa o cursor no console
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 100;
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // handle da saída do console

	int LINHA, COLUNA;
	if (level == 0)
	{
		LINHA = 5;
		COLUNA = 5;
	}
	else if (level == 1)
	{
		LINHA = 7;
		COLUNA = 8;
	}
	else if (level == 2)
	{
		LINHA = 11;
		COLUNA = 22;
	}
	else
	{
		cout << "Nivel invalido!";
		return -1;
	}

	int matriz_nivel0[MAX_LINHA][MAX_COLUNA] = 
	{
	    {1, 1, 1, 1, 1},
	    {1, 0, 0, 0, 1},
	    {1, 2, 3, 5, 1},
	    {1, 0, 0, 0, 1},
	    {1, 1, 1, 1, 1}
	};

	int matriz_nivel1[MAX_LINHA][MAX_COLUNA] = 
	{
	    {1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 0, 0, 0, 5, 5, 1, 1},
	    {1, 0, 1, 3, 0, 0, 1, 1},
	    {1, 2, 0, 0, 3, 0, 0, 1},
	    {1, 0, 1, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1}
	};

	int matriz_nivel2[MAX_LINHA][MAX_COLUNA] =
	    {
		// não deu de gerar com ia isso aqui infelizmente. Juro que tentei me poupar o trabalho, mas estou eu aqui, nove horas da noite de uma segunda feira, digitando 0 e 1.
		{0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 1, 1, 1, 0, 0, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 1, 0, 0, 3, 0, 0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 5, 5, 1},
		{1, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 1},
		{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 0, 0, 5, 5, 1},
		{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
	// 0 = vazio, 1 = parede, 2 = player, 3 = caixa 4 = caixa no objetivo, 5 = objetivo
	int matriz[MAX_LINHA][MAX_COLUNA];

	if (level == 0)
	{
		for (int i = 0; i < LINHA; i++)
		{
			for (int j = 0; j < COLUNA; j++)
			{
				matriz[i][j] = matriz_nivel0[i][j];
			}
		}
	}
	else if (level == 1)
	{
		for (int i = 0; i < LINHA; i++)
		{
			for (int j = 0; j < COLUNA; j++)
			{
				matriz[i][j] = matriz_nivel1[i][j];
			}
		}
	}
	else if (level == 2)
	{
		for (int i = 0; i < LINHA; i++)
		{
			for (int j = 0; j < COLUNA; j++)
			{
				matriz[i][j] = matriz_nivel2[i][j];
			}
		}
	}
	else
	{
		cout << "Nivel invalido!";
		return -1;
	}

	int player_i, player_j;
	int memo_player = 0; // o que estava abaio do player

	for (int i = 0; i < LINHA; i++)
	{
		for (int j = 0; j < COLUNA; j++)
		{
			if (matriz[i][j] == 2)
			{
				player_i = i;
				player_j = j;
				break;
			}
		}
	}

	int objetivos_totais = 0;
	for (int i = 0; i < LINHA; i++)
	{
		for (int j = 0; j < COLUNA; j++)
		{
			if (matriz[i][j] == 5)
			{
				objetivos_totais++;
			}
		}
	}

	char tecla;
	limpa_tela();

	time_t inicio = time(0);
	const int TEMPO_LIMITE = 300; // em segundos

	while (difftime(time(0), inicio) < TEMPO_LIMITE && !ganhou(matriz, objetivos_totais, LINHA, COLUNA))
	{
		gotoxy(0, 0); // clear
		cout << "Controles: WASD para se mover, Q para sair" << endl;
		cout << "Empurre as caixas para os objetivos '.'\n";

		// imprime matriz
		print_jogo(matriz, h, LINHA, COLUNA);

		cout << "Tempo restante: " << TEMPO_LIMITE - difftime(time(0), inicio)
		     << "s...\n";

		if (kbhit())
		{
			tecla = getch();

			// sai no q ou ESC '27'
			if (tecla == 'q' || tecla == 'Q' || tecla == 27)
			{
				break;
			}

			int novo_i = player_i, novo_j = player_j;

			switch (tecla)
			{
			case 'w':
				novo_i = player_i - 1;
				break;
			case 's':
				novo_i = player_i + 1;
				break;
			case 'a':
				novo_j = player_j - 1;
				break;
			case 'd':
				novo_j = player_j + 1;
				break;
			default:
				break; // verificação
			}

			if (!movimentoValido(novo_i, novo_j, matriz, LINHA, COLUNA))
			{
				continue; // movimento inválido, ignora e passa o loop SKIP
			}

			if (matriz[novo_i][novo_j] == 3 || matriz[novo_i][novo_j] == 4) // CAIXAS
			{
				int caixa_novo_i = novo_i + (novo_i - player_i); // posição que a caixa vai tentar se mover para
				int caixa_novo_j = novo_j + (novo_j - player_j);

				if (!movimentoValidoCaixa(caixa_novo_i, caixa_novo_j, matriz, LINHA, COLUNA))
				{
					continue; // ignora e pula pro prox iteração
				}

				// se * vira . denovo
				int memo_caixa;
				if (matriz[novo_i][novo_j] == 4)
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

				matriz[novo_i][novo_j] =
				    memo_caixa; // atualiza o que tá embaixo da caixa
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

	if (ganhou(matriz, objetivos_totais, LINHA, COLUNA))
	{
		gotoxy(0, 2); // clear -> resultado após testar
		print_jogo(matriz, h, LINHA, COLUNA);
		gotoxy(0, LINHA + 3);
		cout << "Voce venceu o nivel " << level + 1 << " em: " << timer << "s\n";

		if (level == 2)
		{
			cout << "Parabens! Voce zerou o jogo.\n";
			return 0;
		}
		else
		{
			return level + 1;
		}
	}
	else
	{
		gotoxy(0, 2); // clear
		print_jogo(matriz, h, LINHA, COLUNA);
		gotoxy(0, LINHA + 3);
		cout << "Tempo esgotado! Tente novamente.\n";
		return level;
	}
}

bool movimentoValido(int i, int j, int matriz[MAX_LINHA][MAX_COLUNA], int LINHA, int COLUNA)
{
	if (i < 0 || i >= LINHA || j < 0 || j >= COLUNA)
	{
		return false; // fora dos limites
	}
	if (matriz[i][j] == 1)
	{
		return false; // parede
	}
	return true;
}

bool movimentoValidoCaixa(int i, int j, int matriz[MAX_LINHA][MAX_COLUNA], int LINHA, int COLUNA)
{
	if (i < 0 || i >= LINHA || j < 0 || j >= COLUNA)
	{
		return false; // fora dos limites
	}
	if (matriz[i][j] == 1 || matriz[i][j] == 3 || matriz[i][j] == 4)
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

bool ganhou(int matriz[MAX_LINHA][MAX_COLUNA], int objetivos_totais, int LINHA, int COLUNA)
{
	if (contaObjetivos(matriz, LINHA, COLUNA) == objetivos_totais)
	{
		return true;
	}
	return false;
}

void print_jogo(int matriz[MAX_LINHA][MAX_COLUNA], HANDLE h, int LINHA, int COLUNA)
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
			case 1:						     // parede
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

int contaObjetivos(int matriz[MAX_LINHA][MAX_COLUNA], int LINHA, int COLUNA)
{
	int caixas_no_objetivo = 0;
	for (int i = 0; i < LINHA; i++)
	{
		for (int j = 0; j < COLUNA; j++)
		{
			if (matriz[i][j] == 4)
			{
				caixas_no_objetivo++;
			}
		}
	}
	return caixas_no_objetivo;
}