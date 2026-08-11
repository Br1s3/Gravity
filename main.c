#include <raylib.h>
#include <stdio.h>
#include <math.h>

#define DEVER_IMPLEMENTATION
#include "dever.h"

#define FPS 60
#define WIDTH (16*100)
#define HEIGHT (9*100)
#define G (1e-7)
#define UNUSED(x) (void)x

#define carre(x) ((x)*(x))
#define cube(x) ((x)*(x)*(x))

#define NB_planete 3
#define VECTEST

typedef struct
{
    Vec2d p;
    Vec2d v;
    Vec2d a;
    Vec2d coord;
    double m; // The mass
    double t; // The size
    Color c;  // The color
    int id;   // The identification
} Entite;

Vec2d equ_accel(double m[], Vec2d p[], int nb_p, int id)
{
    Vec2d val = {0};
    for (int i = 0; i < nb_p; i++) {
	if (i == id) continue;
	double div = sqrt(carre(p[id].x - p[i].x) + carre(p[id].y - p[i].y));
	val.x += m[i] * (p[id].x - p[i].x)/div;
	val.y += m[i] * (p[id].y - p[i].y)/div;
    }
    val.x *= -G*m[id];
    val.y *= -G*m[id];
    return val;
}


Entite planete[NB_planete] = {{
    {.x = 0.f, .y = 0.f},
    {.x = 0.f, .y = 0.f},
    .m = 100000.f,
    .t = 20.f,
    .c = WHITE,
    .id = 0,
},{
    {.x = -100.f, .y = -100.f},
    {.x = 0.f, .y = 0.f},
    .m = 20000.f,
    .t = 10.f,
    .c = BROWN,
    .id = 1,
},{
    {.x = 100.f, .y = 100.f},
    {.x = 0.f, .y = 0.f},
    .m = 5000.f,
    .t = 5.f,
    .c = GREEN,
    .id = 2,
}};

int main()
{
    InitWindow(WIDTH, HEIGHT, "Simulateur physique");
    SetTargetFPS(FPS);
    char tab[30] = {' '};
    snprintf(tab, 30, "ERROR: Calculation overflow");

    double dt = 0.1;
    double t = 0;
    while (!WindowShouldClose()) {
	BeginDrawing();
	ClearBackground(BLACK);

	double valM[NB_planete] = {0};
	Vec2d val[NB_planete] = {0};
	for (int i = 0; i < NB_planete; i++) {
	    valM[i] = planete[i].m;
	    val[i].x = planete[i].p.x;
	    val[i].y = planete[i].p.y;
	}

	for (int i = 1; i < NB_planete; i++) {
	    
	    Vec2d equ_normalise_accel(double t, Vec2d p, Vec2d v) {
		UNUSED(t); UNUSED(v);
		val[planete[i].id].x = p.x;
		val[planete[i].id].y = p.y;
		return equ_accel(valM, val, NB_planete, planete[i].id);
	    }
	    
	    RK4_2d(dt, t, &planete[i].p, &planete[i].v, equ_normalise_accel);
	    // SymplecticEuler2d(dt, t, &planete[i].p, &planete[i].v, equ_normalise_accel[i]);
	    // ExplicitEuler2d(dt, t, &planete[i].p, &planete[i].v, equ_normalise_accel[i]);
	}
	printf("%5.2lf, %5.2lf\n", planete[1].v.x + WIDTH/2, planete[1].v.y + HEIGHT/2);

	for (int i = 0; i < NB_planete; i++) {
	    DrawCircle(planete[i].p.x+WIDTH/2, -planete[i].p.y+HEIGHT/2, planete[i].t, planete[i].c);
	}
	snprintf(tab, 15, "t = %.2lf", t);
	t += dt;

	DrawText(tab, 20, 40, 20, GREEN);
	DrawFPS(10, 10);
	EndDrawing();
    }
    return 0;
}
