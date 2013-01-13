#ifndef COMMOM_H
#define COMMOM_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <time.h>
#define PI 3.14159265358979323846
using namespace std;

class ComplexNum
{
private:
	double re;
	double im;
public:
	ComplexNum() { re=0.0; im=0.0; }
	ComplexNum(double t_re, double t_im) { re=t_re; im=t_im; }
	ComplexNum(double f) { re=cos(f); im=sin(f); }
	ComplexNum(int n) { re=cos(2*PI/(double)n); im=sin(2*PI/(double)n); }	//旋转因子初始化
	ComplexNum(int n, int m) { re=cos(2*PI*(double)m/(double)n); im=sin(2*PI*(double)m/(double)n); }
	void setValue(double t_re, double t_im) { re=t_re;im=t_im; }
	void setValue(double f) { re=cos(f); im=sin(f); }
	void setValue(int n) { re=cos(2*PI/(double)n); im=sin(2*PI/(double)n); }	//旋转因子设置值
	void setValue(int m, int n) { re=cos(2*PI*(double)m/(double)n); im=sin(2*PI*(double)m/(double)n); }
	void operator= (const ComplexNum& z) { re=z.re; im=z.im; }
	ComplexNum operator+ (const ComplexNum& z)
	{
		ComplexNum tmp_z(re+z.re, im+z.im);
		return tmp_z;
	}
	ComplexNum operator- (const ComplexNum& z)
	{
		ComplexNum tmp_z(re-z.re, im-z.im);
		return tmp_z;
	}
	ComplexNum operator* (const ComplexNum& z)
	{
		ComplexNum tmp_z(re*z.re-im*z.im, re*z.im+im*z.re);
		return tmp_z;
	}
	void setNegative() { re=-1*re; im=-1*im; }
	void setPositiveJ() { double tmp=re; re=-1*im; im=tmp; }
	void setNegativeJ() { double tmp=re; re=im; im=-1*tmp; }
	double getRe() { return re; }
	double getIm() { return im; }	
	void print()
	{
		printf("re=%6.8f   im=%6.8f\n", re, im);
	}
};

class ComplexSeries
{
private:
	int length;
	ComplexNum *z;
public:
	ComplexSeries() { length=0; z=NULL; }
	ComplexSeries(int n) { length=n; z=new ComplexNum[n]; }
	~ComplexSeries() { if(NULL!=z) delete [] z; }
	void setValue(int pos, const ComplexNum *t_z)
	{
		if(length>pos && pos>=0)
			*(z+pos) = *t_z;
		else
			return;
	}
	void setValue(int pos, double t_re, double t_im)
	{
		if(length>pos && pos>=0)
			(z+pos)->setValue(t_re, t_im);
		else
			return;
	}
	ComplexNum* getValue(int pos)
	{
		if(length>pos && pos>=0)
			return (z+pos);
		else
			return NULL;
	}
	int getLength() { return length; }
	void print()
	{
		for(int i=0;i<length;i++)
			(z+i)->print();
	}
};

ComplexSeries* dft(ComplexSeries* a);
ComplexSeries* fft_recursive(ComplexSeries* a);
ComplexSeries* fft_iterative_base2(ComplexSeries* a);
ComplexSeries* fft_iterative_base4(ComplexSeries* a);
ComplexSeries* fft_mixbase_2_4(ComplexSeries* a);
ComplexSeries* fft_mixbase_4_2(ComplexSeries* a);
ComplexSeries* rfft(ComplexSeries* a);
ComplexSeries* FFT(ComplexSeries* a, bool has_im);
int reverse(unsigned int pos, unsigned int model);
int reverse_base4(unsigned int pos, unsigned int model_4);

#endif