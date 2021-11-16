#include<iostream>
#include<conio.h>
#include"Entornos.h"

using namespace std;
using namespace System;
using namespace Juego;

const int  cargador = 10, ccomandantes = 6, cmariposas = 8, cavispas = 10, cargador_enemigo = 3;

int tiempo = 950, vidas = 3, nivel = 1, puntos = 0;

bool GameOver , YouWin = false;

struct nave {
	int x, y, dx, dy, s;
} co[ccomandantes], a1[cavispas], m1[cmariposas], a2[cavispas], m2[cmariposas];

struct proyectil {
	int x, y, dx, dy; bool a = false;
};

void d_nave(nave& n) {
	Console::ForegroundColor = ConsoleColor::White;
	Console::BackgroundColor = ConsoleColor::Black;
	poscicion(n.x - n.dx, n.y); cout << "  " << char(94) << "  ";
	poscicion(n.x - n.dx, n.y + 1); cout << " " << char(94) << char(219) << char(94) << " ";
	poscicion(n.x - n.dx, n.y + 2); cout << char(201) << char(223) << char(219) << char(223) << char(187);
}

void b_nave(nave& n) {
	Console::ForegroundColor = ConsoleColor::White;
	Console::BackgroundColor = ConsoleColor::Black;
	poscicion(n.x - n.dx, n.y);     cout << "       ";
	poscicion(n.x - n.dx, n.y + 1); cout << "       ";
	poscicion(n.x - n.dx, n.y + 2); cout << "       ";
}

bool m_nave(nave& n) {
	if (kbhit()) {
		char tecla = getch();
		switch (tecla) {
		case 75:if (n.x >= 5)n.x -= n.dx;
			break;
		case 77:if (n.x <= 71)n.x += n.dx;
			break;
		case ' ':return true;
			break;
		default:
			break;
		}
	}
	return false;
}

void disparo(proyectil& b) {
	if (b.a) {
		poscicion(b.x, b.y); cout << " ";
		if (b.x + b.dx <= 0 || b.x + b.dx > 79 || b.y + b.dy <= 4 || b.y + b.dy >= 37) {
			b.a = false; return;
		}
		b.x -= b.dx;
		b.y -= b.dy;
		Console::ForegroundColor = ConsoleColor::DarkRed;
		poscicion(b.x, b.y); cout << char(207);
	}
}

void dis_enemigo(proyectil& b) {
	if (b.a) {
		poscicion(b.x, b.y); cout << " ";
		if (b.x + b.dx <= 0 || b.x + b.dx > 79 || b.y + b.dy <= 4 || b.y + b.dy >= 36) {
			b.a = false; return;
		}
		b.y -= b.dy;
		Console::ForegroundColor = ConsoleColor::DarkYellow;
		poscicion(b.x, b.y); cout << char(207);
	}
}

void d_comandante(nave& n) {
	Console::BackgroundColor = ConsoleColor::Black;
	if (n.s == 1) {
		Console::ForegroundColor = ConsoleColor::DarkMagenta;
	}
	else Console::ForegroundColor = ConsoleColor::Green;
	poscicion(n.x, n.y); cout << " " << char(220) << " ";
	poscicion(n.x, n.y + 1); cout << char(201) << char(223) << char(187);
}

void d_mariposa(nave& n) {
	Console::ForegroundColor = ConsoleColor::DarkRed;
	Console::BackgroundColor = ConsoleColor::Black;
	poscicion(n.x, n.y); cout << char(125) << char(219) << char(123);
	poscicion(n.x, n.y + 1); cout << " " << char(94) << " ";
}

void d_avispa(nave& n) {
	Console::BackgroundColor = ConsoleColor::Black;
	if (n.dy >= 0) {
		poscicion(n.x, n.y); Console::ForegroundColor = ConsoleColor::DarkBlue; cout << ">";
		Console::ForegroundColor = ConsoleColor::DarkYellow; cout << char(219);
		Console::ForegroundColor = ConsoleColor::DarkBlue; cout << "<";
		poscicion(n.x, n.y + 1); Console::ForegroundColor = ConsoleColor::DarkRed; cout << " "; cout << char(94) << " ";
	}
	else
	{
		Console::ForegroundColor = ConsoleColor::DarkRed; poscicion(n.x, n.y); cout << " v ";
		poscicion(n.x, n.y + 1); Console::ForegroundColor = ConsoleColor::DarkBlue; cout << ">";
		Console::ForegroundColor = ConsoleColor::DarkYellow; cout << char(219);
		Console::ForegroundColor = ConsoleColor::DarkBlue; cout << "<";
	}
}

void m_enemigo(nave& n, int o) {
	switch (o) {
	case 1:
		n.x += n.dx;
		if (n.x >= 70) {
			n.dx = 0;
		}
		break;
	case 2:
		if (n.x <= 10 || n.x >= 70) {
			n.dx *= -1;
		}
		n.x += n.dx;
		break;
	case 3:
		if (n.x <= 10 || n.x >= 70) {
			n.dx *= -1;
		}
		n.x -= n.dx;
		break;
	case 4:
		if (n.y >= 34 || n.y <= 15) {
			n.dy *= -1;
		}
		n.y += n.dy;
		if (n.x <= 10 || n.x >= 70) {
			n.dx *= -1;
		}
		n.x -= n.dx;
		break;
	}
}

void b_enemigo(nave& m) {
	Console::BackgroundColor = ConsoleColor::Black;
	poscicion(m.x, m.y);     cout << "    ";
	poscicion(m.x, m.y + 1); cout << "    ";
}

void morir(nave& n) {
	Console::ForegroundColor = ConsoleColor::White;
	poscicion(n.x, n.y);      cout << "* * *";
	poscicion(n.x, n.y + 1);  cout << " * * ";
	poscicion(n.x, n.y + 2);  cout << "* * *";
}

void main() {
	Console::CursorVisible = false;
	Console::SetWindowSize(115, 45);
	int va, var; bool cambio_de_nivel = false;
	Random r;
	nave n = { r.Next(9,70), 35, 1, 0, 3 };
	for (int i = 0; i < ccomandantes; i++) {
		co[i].x = 20 + (7 * i); co[i].y = 4; co[i].dx = 1; co[i].dy = 1; co[i].s = 2;
	}
	for (int i = 0; i < cmariposas; i++) {
		m1[i].x = 31 + (i * 5); m1[i].y = 8; m1[i].dx = 1; m1[i].s = 1; m1[i].dy = 1;
	}
	for (int i = 0; i < cmariposas; i++) {
		m2[i].x = 31 + (i * 5); m2[i].y = 12; m2[i].dx = 1; m2[i].s = 1; m2[i].dy = 1;
	}
	for (int i = 0; i < cavispas; i++) {
		a1[i].x = 26 + (i * 5); a1[i].y = 16; a1[i].dx = 1; a1[i].s = 1; a1[i].dy = 1;
	}
	for (int i = 0; i < cavispas; i++) {
		a2[i].x = 26 + (i * 5); a2[i].y = 23; a2[i].dx = 1; a2[i].s = 1; a2[i].dy = 1;
	}
	proyectil* b = new proyectil[cargador];
	proyectil* ac = new proyectil[cargador_enemigo];
	pantalla_de_inicio();
	while (GameOver == false && YouWin == false) {
		if (tiempo == 0 || vidas <= 0) {
			GameOver = true;
		}
		if (puntos >= 2500) {
			system("cls"); nivel++;
			if (nivel >= 5) {
				YouWin = true;
			}
			puntos = 0; tiempo = 950;
			for (int i = 0; i < cargador; i++) {
				b[i].x = n.x + 1; b[i].y = n.y - 1; b[i].dx = 0; b[i].dy = 1; b[i].a = false;
			}
			cambio_de_nivel = true;
			poscicion(0, 0); you_win(); Console::ForegroundColor = ConsoleColor::DarkYellow;
			poscicion(45, 15); cout << "<Preciona ESPACIO para iniciar el siguiente nivel>";
			while (true) {
				char t = getch();
				if (t == ' ') break;
			}

		}
		if (cambio_de_nivel == true) {
			system("cls"); mapa();
		}
		switch (nivel) {
		case 1:va = 4; var = 2;
			break;
		case 2:va = 3; var = 3;
			break;
		case 3:va = 1; var = 4;
			break;
		case 4:va = 0; var = 5;
			break;
		default:
			break;
		}
		b_nave(n);
		if (m_nave(n)) {
			for (int i = 0; i < cargador; i++) {
				if (b[i].a == false) {
					b[i].x = n.x + 1; b[i].y = n.y; b[i].dx = 0; b[i].dy = 1; b[i].a = true;
					break;
				}
			}
		}
		d_nave(n);
		for (int i = 0; i < cargador; i++) {
			disparo(b[i]);
		}
		for (int i = 0; i < ccomandantes - va; i++) {
			if (cambio_de_nivel == true) {
				co[i].x = 20 + (7 * i) - (2 * var); co[i].y = 4; co[i].dx = 1; co[i].dy = 1; co[i].s = 2;
			}
			for (int j = 0; j < cargador; j++) {
				if ((b[j].x == co[i].x && b[j].y == co[i].y) || (b[j].x == co[i].x && b[j].y == co[i].y + 1) ||
					(b[j].x == co[i].x + 1 && b[j].y == co[i].y) || (b[j].x == co[i].x + 1 && b[j].y == co[i].y + 1) ||
					(b[j].x == co[i].x + 2 && b[j].y == co[i].y) || (b[j].x == co[i].x + 2 && b[j].y == co[i].y + 1)) {
					b[j].a = false; b[j].x = n.x + 1; b[j].y = n.y; co[i].s--;
					if (co[i].s == 0) {
						b[j].a = false;
						co[i].dx = 0;
						puntos += 400; b_enemigo(co[i]); b[j].y = n.y;
						b_enemigo(co[i]);
					}
					b[j].dy = 1;
				}
			}
			if (co[i].dx != 0 && co[i].dy != 0) {
				b_enemigo(co[i]); m_enemigo(co[i], 3); d_comandante(co[i]);
			}
			else {
				b_enemigo(co[i]);
				co[i].x = 5; co[i].y = 3;
			}
			for (int j = 0; j < cargador_enemigo; j++) {
				if (ac[j].a == false) {
					ac[j].x = co[i].x + 1; ac[j].y = co[i].y + 2; ac[j].dx = 0; ac[j].dy = -1; ac[j].a = true; break;
				}
				dis_enemigo(ac[j]);
				if ((ac[j].x == n.x + 2 && ac[j].y == n.y) || (ac[j].x == n.x + 2 && ac[j].y == n.y + 1) ||
					(ac[j].x == n.x + 2 && ac[j].y == n.y + 2) || (ac[j].x == n.x && ac[j].y == n.y + 2) ||
					(ac[j].x == n.x + 1 && ac[j].y == n.y + 1) || (ac[j].x == n.x + 3 && ac[j].y == n.y + 1) ||
					(ac[j].x == n.x + 1 && ac[j].y == n.y + 2) || (ac[j].x == n.x + 3 && ac[j].y == n.y + 2)) {
					ac[j].a = false; n.s -= 1; morir(n);
					if (n.s == 0) {
						vidas -= 1;
						n.s = 3;
					}
				}
			}
		}
		for (int i = 0; i < cmariposas - va; i++) {
			if (cambio_de_nivel == true) {
				m1[i].x = 31 + (i * 5) - (2 * var); m1[i].y = 8; m1[i].dx = 1; m1[i].s = 1; m1[i].dy = 1;
				m2[i].x = 31 + (i * 5) - (2 * var); m2[i].y = 12; m2[i].dx = 1; m2[i].s = 1; m2[i].dy = 1;
			}
			for (int j = 0; j < cargador; j++) {
				if ((b[j].x == m1[i].x + 1 && b[j].y == m1[i].y) || (b[j].x == m1[i].x && b[j].y == m1[i].y + 1) ||
					(b[j].x == m1[i].x && b[j].y == m1[i].y) || (b[j].x == m1[i].x + 1 && b[j].y == m1[i].y + 1) ||
					(b[j].x == m1[i].x + 2 && b[j].y == m1[i].y) || (b[j].x == m1[i].x + 2 && b[j].y == m1[i].y + 1)) {
					b[j].dy = 0; m1[i].dx = 0; m1[i].dy = 0; m1[i].s--;
					if (m1[i].s == 0) {
						puntos += 160; b_enemigo(m1[i]); b[j].x = n.x + 1; b[j].y = n.y; b[j].a = false;
						b_enemigo(m1[i]);
					}
					b[j].dy = 1;
				}
				if ((b[j].x == m2[i].x + 1 && b[j].y == m2[i].y) || (b[j].x == m2[i].x && b[j].y == m2[i].y + 1) ||
					(b[j].x == m2[i].x && b[j].y == m2[i].y) || (b[j].x == m2[i].x + 1 && b[j].y == m2[i].y + 1) ||
					(b[j].x == m2[i].x + 2 && b[j].y == m2[i].y) || (b[j].x == m2[i].x + 2 && b[j].y == m2[i].y + 1)) {
					b[j].dy = 0; m2[i].dx = 0; m2[i].dy = 0; m2[i].s--;
					if (m2[i].s == 0) {
						puntos += 160; b_enemigo(m2[i]); b[j].x = n.x + 1; b[j].y = n.y; b[j].a = false;
						b_enemigo(m2[i]);
					}
					b[j].dy = 1;
				}
			}
			if (m1[i].dx != 0 && m1[i].dy != 0) {
				b_enemigo(m1[i]); m_enemigo(m1[i], 2); d_mariposa(m1[i]);
			}
			else {
				b_enemigo(m1[i]);
				m1[i].x = 5; m1[i].y = 3;
			}
			if (m2[i].dx != 0 && m2[i].dy != 0) {
				b_enemigo(m2[i]); m_enemigo(m2[i], 2); d_mariposa(m2[i]);
			}
			else {
				b_enemigo(m2[i]);
				m2[i].x = 5; m2[i].y = 3;
			}
		}
		for (int i = 0; i < cavispas - va; i++) {
			if (cambio_de_nivel == true) {
				a1[i].x = 26 + (i * 5) - (3 * var); a1[i].y = 16; a1[i].dx = 2; a1[i].s = 1; a1[i].dy = 1;
				a2[i].x = 26 + (i * 5) - (3 * var); a2[i].y = 20; a2[i].dx = 2; a2[i].s = 1; a2[i].dy = 1;
			}
			for (int j = 0; j < cargador; j++) {
				if ((b[j].x == a1[i].x + 1 && b[j].y == a1[i].y) || (b[j].x == a1[i].x && b[j].y == a1[i].y + 1) ||
					(b[j].x == a1[i].x && b[j].y == a1[i].y) || (b[j].x == a1[i].x + 1 && b[j].y == a1[i].y + 1) ||
					(b[j].x == a1[i].x + 2 && b[j].y == a1[i].y) || (b[j].x == a1[i].x + 2 && b[j].y == a1[i].y + 1)) {
					b[j].dy = 0; a1[i].dx = 0; a1[i].dy = 0; a1[i].s--;
					if (a1[i].s == 0) {
						puntos += 100; b_enemigo(a1[i]); b[j].x = n.x + 1; b[j].y = n.y; b[j].a = false; b_enemigo(a1[i]);
					}
					b[j].dy = 1;
				}
				if ((b[j].x == a2[i].x + 1 && b[j].y == a2[i].y) || (b[j].x == a2[i].x && b[j].y == a2[i].y + 1) ||
					(b[j].x == a2[i].x && b[j].y == a2[i].y) || (b[j].x == a2[i].x + 1 && b[j].y == a2[i].y + 1) ||
					(b[j].x == a2[i].x + 2 && b[j].y == a2[i].y) || (b[j].x == a2[i].x + 2 && b[j].y == a2[i].y + 1)) {
					b[j].dy = 0; a2[i].dx = 0; a2[i].dy = 0;
					a2[i].s--;
					if (a2[i].s == 0) {
						puntos += 100; b_enemigo(a2[i]); b[j].x = n.x + 1; b[j].y = n.y; b[j].a = false; b_enemigo(a2[i]);
					}
					b[j].dy = 1;
				}
			}
			if ((a1[i].dx != 0 && a1[i].dy != 0)) {
				b_enemigo(a1[i]); m_enemigo(a1[i], 4); d_avispa(a1[i]);
			}
			else {
				b_enemigo(a1[i]);
				a1[i].x = 5; a1[i].y = 3;
			}
			if ((a2[i].dx != 0 && a2[i].dy != 0)) {
				b_enemigo(a2[i]); m_enemigo(a2[i], 4); d_avispa(a2[i]);
			}
			else {
				b_enemigo(a2[i]);
				a2[i].x = 5; a2[i].y = 3;
			}
		}
		Console::ForegroundColor = ConsoleColor::DarkMagenta;
		poscicion(90, 16); cout << "------GALAGA------";
		Console::ForegroundColor = ConsoleColor::White;
		poscicion(95, 18); cout << "Nivel: " << nivel;
		poscicion(95, 20); cout << "puntos: " << puntos;
		poscicion(95, 22); cout << "Tiempo: " << tiempo / 10.0;
		poscicion(95, 24); cout << "vidas: " << vidas;
		poscicion(95, 26); cout << "salud: " << n.s << " " << char(3);
		poscicion(100, 30); cout << "By: "; poscicion(102, 32); cout << "John Leon";
		if (cambio_de_nivel == true) {
			cambio_de_nivel = false;
		}
		_sleep(50);
		tiempo--;
	}
	if (GameOver == true) {
		system("cls"); game_over();
	}
	if (YouWin == true) {
		system("cls"); you_win();
	}
	system("pause>NULL");
}