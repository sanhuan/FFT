#include "common.h"

ComplexSeries* rfft(ComplexSeries* a)
{
	if(NULL == a)
		return NULL;
	int length = a->getLength();
	if(length <= 0)
		return NULL;
	int t_length = length/2;
	int i=0, j=0;
	ComplexSeries* z_series = new ComplexSeries(length);
	if(NULL==z_series)
	{
		cout<<"rfft memory malloc failed"<<endl;
		return NULL;
	}
	ComplexSeries* z_half_series = new ComplexSeries(t_length);
	if(NULL==z_half_series)
	{
		delete z_series;
		cout<<"rfft memory malloc failed"<<endl;
		return NULL;
	}
	for(i=0;i<t_length;i++)
		z_half_series->setValue(i, a->getValue(2*i)->getRe(), a->getValue(2*i+1)->getRe());

	ComplexSeries* tmp_serise = z_half_series;
	z_half_series = fft_mixbase_4_2(z_half_series);
	delete tmp_serise;
	if(NULL == z_half_series)
		return NULL;

	double f=0.0;
	double re1=0.0, re2=0.0, im1=0.0, im2=0.0;
	ComplexNum result1, result2;
	ComplexNum w;
	for(i=0;i<t_length;i++)
	{
		//共轭复数的傅里叶变换性质
		re1 = z_half_series->getValue(i)->getRe();
		im1 = z_half_series->getValue(i)->getIm();
		re2 = z_half_series->getValue((t_length-i)%t_length)->getRe();
		im2 = -1*z_half_series->getValue((t_length-i)%t_length)->getIm();

		result1.setValue((re1+re2)/2.0, (im1+im2)/2.0);
		result2.setValue((im1-im2)/2.0, (re2-re1)/2.0);

		f = -1*2*PI*(double)i/(double)length;
		w.setValue(f);
		result2 = w*result2;

		z_series->setValue(i, &(result1 + result2));
		z_series->setValue(i+t_length, &(result1 - result2));
	}

	delete z_half_series;
	return z_series;
}

ComplexSeries* FFT(ComplexSeries* a, bool has_im)
{
	if(NULL == a)
		return NULL;
	int length = a->getLength();
	int t_length = 0;
	if(length <= 0)
		return NULL;

	//如果不足2的N次方，则用0补齐
	int model = 0;
	while( (length>>model)>0x0001 )
		model++;
	if(length > (0x0001 << model))
		model++;
	t_length = 0x0001<<model;

	int i;
	ComplexSeries *z_series, *tmp_series;
	if(t_length != length)
	{
		z_series = new ComplexSeries(t_length);
		if(NULL==z_series)
		{
			cout<<"FFT memory malloc failed"<<endl;
			return NULL;
		}
		for(i=0;i<length;i++)
			z_series->setValue(i, a->getValue(i));
		for(;i<t_length;i++)
			z_series->setValue(i, 0.0, 0.0);
	}
	else
		z_series = a;

	//调用适合的fft函数执行快速傅里叶变换
	tmp_series= z_series;
	if(has_im)
		z_series = fft_mixbase_4_2(z_series);
	else
		z_series = rfft(z_series);
	if(t_length != length)
		delete tmp_series;
	return z_series;	
}