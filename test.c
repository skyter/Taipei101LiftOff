#include <stdio.h>
#include <math.h>

/* Manual :
 *
 *                 UP    Y+|             
 *                         |      /|    
 *                         |     / |   
 *                         |    /  |  / X+  RIGHT
 *                         |   /   | /
 *                         |  /    |/
 *                         |  |    /   <== Wall 5M x 5M
 *                         |  |   /	   px py pz ax ay az
 *                         |  |  /         10  0  0  0  0  0
 *                         |  | /
 *                         |  |/
 *                         |  /
 *                         | /
 *  BACK                   |/
 *    Z- ------------------+------------------- Z+  FRONT
 *                        /|
 *                       / |
 *                      /  |
 *                     /   |
 *                    /    |
 *        LEFT    X- /     | Y-  DOWN
 */

typedef struct {
	float px; /* Position X, Y, Z */
	float py;
	float pz;
	float ax; /* Angle X, Y, Z */
	float ay;
	float az;
} tPos;

typedef enum {
	FRONT,
	LEFT,
	BACK,
	RIGHT,
	UP,
	DOWN
} tDir;

typedef enum {
	YAW_R,
	YAW_L,
	ROLL_CW,
	ROLL_CCW,
	PITCH_UP,
	PITCH_DOWN
} tRotate;

tPos *op (tPos *p, tDir dir, float dist)
{
	switch (dir) {
		case FRONT:
			p->pz += dist;
			break;
		case BACK:
			p->pz -= dist;
			break;
		case RIGHT:
			p->px += dist;
			break;
		case LEFT:
			p->px -= dist;
			break;
		case UP:
			p->py += dist;
			break;
		case DOWN:
			p->py -= dist;
			break;
	}
	return p;
}

tPos *rop (tPos *p, tRotate rot, float deg)
{
	switch (rot) {
		case YAW_R:
			p->ay += deg;
			break;
		case YAW_L:
			p->ay -= deg;
			break;
		case ROLL_CW:
			p->az += deg;
			break;
		case ROLL_CCW:
			p->az -= deg;
			break;
		case PITCH_UP:
			p->ax -= deg;
			break;
		case PITCH_DOWN:
			p->ax += deg;
			break;
	}
	return p;
}

void Wall10Mx5M(tPos *p, tDir dir, float dist, int loop) {
	if (loop == 0)
		return;
	printf ("<TrackBlueprint xsi:type=\"TrackBlueprintFlag\">\n"
		"<itemID>DrawingBoardWall5mx5m01</itemID>\n"
		"<instanceID>14</instanceID>\n"
		"<position>\n"
		"<x>%g</x>\n"
		"<y>%g</y>\n"
		"<z>%g</z>\n"
		"</position>\n"
		"<rotation>\n"
		"<x>%g</x>\n"
		"<y>%g</y>\n"
		"<z>%g</z>\n"
		"</rotation>\n"
		"<purpose>Functional</purpose>\n"
		"</TrackBlueprint>\n",
		p->px, p->py, p->pz,
		p->ax, p->ay, p->az);
	loop--;
	Wall10Mx5M (op(p, dir, dist), dir, dist, loop);

}

int Wall101(tPos *p, float side, tDir dir)
{
	tPos q, bak;
	float sa = side, sb;
	int i;
	/* Base */
	for (p->py; p->py < 114.7; p->py += 114.7 / 23) {
		q = *p;
		sb = sa - (10 - sa / 7);
		Wall10Mx5M (&q, dir, sb/7, 7);
		sa -= tan(4 * M_PI / 180) * 114.7 / 23 * 2;
		op (p, dir, tan(4 * M_PI / 180) * 114.7 / 23);
		op (p, (dir + 3) % 4, tan(4 * M_PI / 180) * 114.7 / 23);
	}

	/* 8 node */
	rop (p, PITCH_UP, 4 + 5);
	for (i = 0; i < 8; i++) {
		float baksa = sa, bh = p->py;
		bak = *p;
		for (p->py; p->py < bh + 33.022; p->py += 33.022 / 7) {
			q = *p;
			sb = sa - (10 - sa / 6);
			Wall10Mx5M (&q, dir, sb/6, 6);
			sa += tan(5 * M_PI / 180) * 33.022 / 7 * 2;
			op (p, (dir + 2) % 4, tan(5 * M_PI / 180) * 33.022 / 7);
			op (p, (dir + 1) % 4, tan(5 * M_PI / 180) * 33.022 / 7);
		}
		bak.py += 33.022;
		*p = bak;
		sa = baksa;
	}

	/* crown */
	{
		float bh = p->py;
		op (p, dir, (sa - 27) / 2);
		op (p, (dir + 3) % 4, (sa - 27) / 2);
		sa = 27;
		for (p->py; p->py < bh + 10; p->py += 10 / 2) {
			q = *p;
			sb = sa - (10 - sa / 3);
			Wall10Mx5M (&q, dir, sb/3, 3);
			sa += tan(5 * M_PI / 180) * 10 / 2 * 2;
			op (p, (dir + 2) % 4, tan(5 * M_PI / 180) * 10 / 2);
			op (p, (dir + 1) % 4, tan(5 * M_PI / 180) * 10 / 2);
		}
	}

	/* top crown */
	{
		float bh = p->py;
		op (p, dir, (sa - 21) / 2);
		op (p, (dir + 3) % 4, (sa - 21) / 2);
		sa = 21;
		for (p->py; p->py < bh + 46; p->py += 46 / 10) {
			q = *p;
			sb = sa - (10 - sa / 3);
			Wall10Mx5M (&q, dir, sb/3, 3);
			sa += tan(5 * M_PI / 180) * 46 / 10 * 2;
			op (p, (dir + 2) % 4, tan(5 * M_PI / 180) * 46 / 10);
			op (p, (dir + 1) % 4, tan(5 * M_PI / 180) * 46 / 10);
		}
	}

	/* top */
	{
		float bh = p->py;
		op (p, dir, (sa - 10) / 2);
		op (p, (dir + 3) % 4, (sa - 10) / 2);
		sa = 10;
		for (p->py; p->py < bh + 62; p->py += 62 / 13) {
			q = *p;
			Wall10Mx5M (&q, dir, 10, 1);
		}
	}
}

int main()
{
	tPos p = {0}, q = {0};
	float side = 63.435;
	int dir = LEFT;

	//rop (&p, YAW_R, 90);
	rop (&p, PITCH_DOWN, 4);
	op (&p, FRONT, 30);
	op (&p, LEFT, 5);
	q = p;
	Wall101 (&q, side, dir);

	op (&p, RIGHT, 5);
	dir = (dir + 4 + 1) % 4;
	rop (&p, YAW_L, 90);
	op (&p, FRONT, side-5);
	q = p;
	Wall101 (&q, side, dir);

	op (&p, FRONT, 5);
	dir = (dir + 4 + 1) % 4;
	rop (&p, YAW_L, 90);
	op (&p, LEFT, side-5);
	q = p;
	Wall101 (&q, side, dir);

	op (&p, LEFT, 5);
	dir = (dir + 4 + 1) % 4;
	rop (&p, YAW_L, 90);
	op (&p, BACK, side-5);
	q = p;
	Wall101 (&q, side, dir);
}
