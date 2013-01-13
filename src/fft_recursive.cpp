#include "common.h"

ComplexSeries* fft_recursive(ComplexSeries* a)
{
	if(NULL == a)
		return NULL;
	int length = a->getLength();
	int i;

	if(length <= 0)
		return NULL;
	if(1 == length)
		return a;
	int half_len = ((unsigned int)length) >> 1;
	ComplexSeries* a0 = new ComplexSeries(half_len);
	if(NULL==a0)
	{
		cout<<"fft_recusive memory malloc failed"<<endl;
		return NULL;
	}
	ComplexSeries* a1 = new ComplexSeries(half_len);
	if(NULL==a1)
	{
		delete a0;
		cout<<"fft_recusive memory malloc failed"<<endl;
		return NULL;
	}
	for(i=0;i<half_len;i++)
	{
		a0->setValue(i, a->getValue(i*2));
		a1->setValue(i, a->getValue(i*2+1));
	}
	delete a;

	ComplexSeries* y0 = fft_recursive(a0);
	ComplexSeries* y1 = fft_recursive(a1);
	ComplexSeries* y = new ComplexSeries(length);
	if(NULL == y)
	{
		delete a0;
		delete a1;
		cout<<"fft_recusive memory malloc failed"<<endl;
		return NULL;
	}
	ComplexNum w(1.0, 0.0);
	ComplexNum wn(-1*length);

	for(i=0;i<half_len;i++)
	{
		ComplexNum* yy0 = y0->getValue(i);
		ComplexNum* yy1 = y1->getValue(i);
		ComplexNum ww = w*(*yy1);

		ComplexNum result1 = *yy0 + ww;
		ComplexNum result2 = *yy0 - ww;
		y->setValue(i, &result1);
		y->setValue(i+half_len, &result2);

		w = w*wn;
	}
	delete y0;
	delete y1;

	return y;
}