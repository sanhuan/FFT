#include "common.h"

ComplexSeries* fft_iterative_base2(ComplexSeries* a)
{
	if(NULL == a)
		return NULL;
	int length = a->getLength();
	if(length <= 0)
		return NULL;

	unsigned int model = 0;
	while(1)
	{
		if(!(((unsigned int)length >> model) & 0x0001))
			model++;
		else if( ((unsigned int)length>>model) != 0x0001 )
			return NULL;
		else
			break;
	}

	int i=0, j=0, k=0, len=0, step=0;
	ComplexSeries* z_series = new ComplexSeries(length);
	if(NULL == z_series)
	{
		cout<<"fft_iterative_base2 memory malloc failed"<<endl;
		return NULL;
	}
	ComplexNum* z;
	ComplexNum* z1;
	ComplexNum* z2;
	ComplexNum w(0.0, 0.0);
	ComplexNum ww(0.0, 0.0);
	ComplexNum wn(0.0, 0.0);

	for(i=0;i<length;i++)
	{
		j = reverse((unsigned int)i, model);
		if(-1 == j)
		{
			delete z_series;
			return NULL;
		}
		z = a->getValue(i);
		z_series->setValue(j, z);
	}

	for(i=0;i<model;i++)
	{
		len = (unsigned int)1 << i;
		step = (unsigned int)1 << (i+1);
		wn.setValue(-1*step);
		for(j=0;j<length;j=j+step)
		{
			w.setValue(1.0, 0.0);
			for(k=j;k<j+len;k++)
			{
				z1 = z_series->getValue(k);
				z2 = z_series->getValue(k+len);
				ww = w*(*z2);
				ComplexNum result1 = *z1 + ww;
				ComplexNum result2 = *z1 - ww;
				z_series->setValue(k, &result1);
				z_series->setValue(k+len, &result2);
				w = w*wn;
			}
		}
	}
	return z_series;
}

ComplexSeries* fft_iterative_base4(ComplexSeries* a)
{
	if(NULL == a)
		return NULL;
	int length = a->getLength();
	if(length <=0 )
		return NULL;
	unsigned int model = 0;
	while(1)
	{
		if(!(((unsigned int)length>>model) & 0x0001))
			model++;
		else
		{
			if( ((unsigned int)length>>model) != 0x0001 )
				return NULL;
			else if((model & 0x0001))
				return NULL;
			else
			{
				model = model>>0x0001;
				break;
			}
		}
	}

	double f=0.0;
	int i=0, j=0, k=0, len=0, step=0, n=0, m=0;
	ComplexSeries* z_series = new ComplexSeries(length);
	if(NULL == z_series)
	{
		cout<<"fft_iterative_base4 memory malloc failed"<<endl;
		return NULL;
	}
	ComplexNum *z, *z1, *z2, *z3, *z4;
	ComplexNum w, w1, w2, w3, w4;
	ComplexNum result1, result2, result3, result4;

	for(i=0;i<length;i++)
	{
		j = reverse_base4((unsigned int)i, model);
		if(-1 == j)
		{
			delete z_series;
			return NULL;
		}
		z = a->getValue(i);
		z_series->setValue(j, z);
	}

	for(i=0;i<model;i++)
	{
		len = ((unsigned int)1<<i)<<i;
		step = ((unsigned int)1<<(i+1))<<(i+1);
		n = -1 * step;
		for(j=0;j<length;j+=step)
		{
			for(k=j;k<j+len;k++)
			{
				z1 = z_series->getValue(k);
				z2 = z_series->getValue(k+len);
				z3 = z_series->getValue(k+2*len);
				z4 = z_series->getValue(k+3*len);

				m = k%len;
				f = 2*PI*(double)m/(double)n;
				w2.setValue(f);
				w3.setValue(2.0*f);
				w4.setValue(3.0*f);

				w1 = *z1;
				w2 = (*z2)*w2;
				w3 = (*z3)*w3;
				w4 = (*z4)*w4;

				result1 = w1 + w2 + w3 + w4;
				result3 = w1 - w2 + w3 - w4;
				w2.setPositiveJ(); w4.setPositiveJ();
				result2 = w1 - w2 - w3 + w4;
				result4 = w1 + w2 - w3 - w4;

				z_series->setValue(k, &result1);
				z_series->setValue(k+len, &result2);
				z_series->setValue(k+2*len, &result3);
				z_series->setValue(k+3*len, &result4);
			}
		}
	}
	return z_series;
}

int reverse(unsigned int pos, unsigned int model)
{
	unsigned int t_pos = 0x0000;
	unsigned int temp = 0x0000;
	unsigned int unit = 0x0001;
	unsigned int size = sizeof(int);
	int i;

	if(model>size*8 && pos>(unit<<model)-1)
		return -1;
	for(i=(int)model-1;i>=0;i--)
	{
		temp = (pos>>i)&unit;
		temp = temp<<((int)model-i-1);
		t_pos = t_pos | temp;
	}
	return t_pos;
}

int reverse_base4(unsigned int pos, unsigned int model_4)
{
	unsigned int t_pos = 0x0000;
	unsigned int temp = 0x0000;
	unsigned int unit = 0x0003;
	unsigned int size = sizeof(int);
	unsigned int model = model_4<<1;
	int i;
	if(model>size*8 || pos>(0x0001<<model)-1 || 0x00!=(model&0x0001))
		return -1;

	for(i=model-2;i>=0;i=i-2)
	{
		temp = (pos>>i) & unit;
		temp = temp<<((int)model-i-2);
		t_pos = t_pos | temp;
	}
	return t_pos;
}