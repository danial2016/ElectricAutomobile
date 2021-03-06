/*
* tdoa.c
* Calculates an (x,y,z)-coordinate in a room using the algorithm from [1].
*
* Created: 2018-04-13
* Author: Danial Mahmoud and Jonas Eiselt
*
* [1] A Synthesizable Low Power VHDL Model of the Exact Solution of
* Three Dimensional Hyperbolic Positioning System
*/

#include <asf.h>
#include <inttypes.h>
#include <stdio.h>
#include <complex.h>
#include <math.h>
#include "tdoa.h"
#include "adc_with_pdc.h"
#include "dec_string.h"


struct transmitter_coord {
	double xi, xj, xk, xl;
	double yi, yj, yk, yl;
	double zi, zj, zk, zl;
} coord_t;

struct tdoa {
	double tij, tik, tkj, tkl;
} tdoa_t;

static double v = 343.29;

static double Rik;
static double Rij;
static double Rkl;
static double Rkj;

static double xji;
static double xki;
static double xjk;
static double xlk;

static double yji;
static double yki;
static double yjk;
static double ylk;

static double zji;
static double zki;
static double zjk;
static double zlk;

static double powXi;
static double powXj;
static double powXk;
static double powXl;

static double powYi;
static double powYj;
static double powYk;
static double powYl;

static double powZi;
static double powZj;
static double powZk;
static double powZl;

void tdoa_init()
{
	coord_t.xi = 0;
	coord_t.xj = 1.82;
	coord_t.xk = 0;
	coord_t.xl = 1.82;

	coord_t.yi = 0;
	coord_t.yj = 0;
	coord_t.yk = 1.9;
	coord_t.yl = 1.9;

	coord_t.zi = 1.98;
	coord_t.zj = 1.96;
	coord_t.zk = 1.96;
	coord_t.zl = 2;

	xji = coord_t.xj - coord_t.xi;
	xki = coord_t.xk - coord_t.xi;
	xjk = coord_t.xj - coord_t.xk;
	xlk = coord_t.xl - coord_t.xk;

	yji = coord_t.yj - coord_t.yi;
	yki = coord_t.yk - coord_t.yi;
	yjk = coord_t.yj - coord_t.yk;
	ylk = coord_t.yl - coord_t.yk;

	zji = coord_t.zj - coord_t.zi;
	zki = coord_t.zk - coord_t.zi;
	zjk = coord_t.zj - coord_t.zk;
	zlk = coord_t.zl - coord_t.zk;

	powXi = pow2(coord_t.xi);
	powXj = pow2(coord_t.xj);
	powXk = pow2(coord_t.xk);
	powXl = pow2(coord_t.xl);

	powYi = pow2(coord_t.yi);
	powYj = pow2(coord_t.yj);
	powYk = pow2(coord_t.yk);
	powYl = pow2(coord_t.yl);

	powZi = pow2(coord_t.zi);
	powZj = pow2(coord_t.zj);
	powZk = pow2(coord_t.zk);
	powZl = pow2(coord_t.zl);
}


void set_sdoa(int16_t sample_i, int16_t sample_j, int16_t sample_k, int16_t sample_l)
{
	// tj = 1, ti = 1.00085, tk = 0.9956, tl = 0.99397
	tdoa_t.tij = (sample_i-sample_j)/sample_frequency; //0.00085;
	tdoa_t.tik = (sample_i-sample_k)/sample_frequency; //0.00525;
	tdoa_t.tkj = (sample_k-sample_j)/sample_frequency; //-0.0044;
	tdoa_t.tkl = (sample_k-sample_l)/sample_frequency; //0.00163;
	Rij = v * tdoa_t.tij;
	Rik = v * tdoa_t.tik;
	Rkj = v * tdoa_t.tkj;
	Rkl = v * tdoa_t.tkl;
}

void set_tdoa(double ti, double tj, double tk, double tl)
{
	// tj = 1, ti = 1.00085, tk = 0.9956, tl = 0.99397
	tdoa_t.tij = ti-tj; //0.00085;
	tdoa_t.tik = ti-tk; //0.00525;
	tdoa_t.tkj = tk-tj; //-0.0044;
	tdoa_t.tkl = tk-tl; //0.00163;
	Rij = v * tdoa_t.tij;
	Rik = v * tdoa_t.tik;
	Rkj = v * tdoa_t.tkj;
	Rkl = v * tdoa_t.tkl;
}

void calculate_position(double *x, double *y, double *z)
{
	double Yi = (Rij*yki)-(Rik*yji);
	double Yk = (Rkj*ylk)-(Rkl*yjk);

	double powRij = pow2(Rij);
	double powRik = pow2(Rik);
	double powRkj = pow2(Rkj);
	double powRkl = pow2(Rkl);

	double A = (Rik*xji-Rij*xki)/Yi;
	double B = (Rik*zji-Rij*zki)/Yi;
	double C = (Rik*(powRij+powXi-powXj+powYi-powYj+powZi-powZj) - (Rij*(powRik+powXi-powXk+powYi-powYk+powZi-powZk)))/(2*Yi);
	
	double D = (Rkl*xjk-Rkj*xlk)/Yk;
	double E = (Rkl*zjk-Rkj*zlk)/Yk;
	double F = (Rkl*(powRkj+powXk-powXj+powYk-powYj+powZk-powZj) - (Rkj*(powRkl+powXk-powXl+powYk-powYl+powZk-powZl)))/(2*Yk);
	
	double G = (E-B)/(A-D);
	double H = (F-C)/(A-D);
	double W = A*G + B;
	double J = A*H + C;
	
	double K = powRik + powXi - powXk + powYi - powYk + powZi - powZk + 2 * xki * H + 2 * yki * J;
	double L = 2*(xki*G+yki*W+2*zki);
	double M = 4*powRik*(pow2(G)+pow2(W)+1)-pow2(L);
	double N = 8*powRik*(G*(coord_t.xi-H) + W*(coord_t.yi-J) + coord_t.zi) + (2*L*K);
	double O = 4*powRik*(pow2(coord_t.xi-H) + pow2(coord_t.yi-J) + pow2(coord_t.zi)) - pow2(K);

	double n2m = N/(2*M), om = O/M;
	//double Imz = n2m + sqrt(pow2(n2m) - om);
	/*double in = pow2(n2m) - om;
	if (in <= 0)
	{
		in = 1;
	}
	*z = n2m - sqrt(in);
	*x = G * (*z) + H;
	*y = W * (*z) + J;
	*/
	
	double complex Imz = n2m - csqrt(pow2(n2m) - om);
	double complex Imx = G * Imz + H;
	double complex Imy = W * Imz + J;
	
	double temp = 0;
	
	if (cimagf(Imz)!= 0)
	{
		temp = sqrt(pow2(cimagf(Imz)) + pow2(crealf(Imz)));
		temp = temp - cimagf(Imz);
		
		Imz = temp-1;
	}
	
	*z = Imz;
	*x = crealf(Imx);
	*y = crealf(Imy);
}

/* Math function for raising x to the power of 2 */
double pow2(double x)
{
	return x*x;
}