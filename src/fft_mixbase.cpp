#include "common.h"

ComplexSeries* fft_mixbase_2_4(ComplexSeries* a)
{
	if(NULL == a)
		return NULL;
	int length = a->getLength();
	if(length <= 0)
		return NULL;
	unsigned int model_2 = 0;
	unsigned int model_4 = 0;
	while (1)
	{
		if(!(((unsigned int)length>>model_2) & 0x0001))
			model_2++;
		else if( ((unsigned int)length>>model_2)!=0x0001 )
			return NULL;
		else
		{
			model_4 = model_2>>0x0001;
			break;
		}
	}

	double f=0.0;
	int i=0, j=0, k=0, len=0, step=0, n=0, m=0;
	ComplexSeries* z_series = new ComplexSeries(length);
	if(NULL == z_series)
	{
		cout<<"fft_mixbase_2_4 memory malloc failed"<<endl;
		return NULL;
	}
	ComplexNum *z, *z1, *z2, *z3, *z4;
	ComplexNum w, w1, w2, w3, w4;
	ComplexNum result1, result2, result3, result4;

	for(i=0;i<length;i++)
	{
		j = reverse((unsigned int)i, model_2);
		if(-1 == j)
		{
			delete z_series;
			return NULL;
		}
		z = a->getValue(i);
		z_series->setValue(j, z);
	}

	if(model_2 & 0x0001)
	{
		len = 1;
		step = 2;
		for(i=0;i<length;i+=step)
		{
			z1 = z_series->getValue(i);
			z2 = z_series->getValue(i+len);
			result1 = *z1 + *z2;
			result2 = *z1 - *z2;
			z_series->setValue(i, &result1);
			z_series->setValue(i+len, &result2);
		}
		len = 2;
		step = 8;
	}
	else
	{
		len = 1;
		step = 4;
	}

	for(i=0;i<model_4;i++)
	{
		for(j=0;j<length;j+=step)
		{
			for(k=j;k<j+len;k++)
			{
				f = -1*2*PI*(double)(k%len)/(double)step;
				w2.setValue(2.0*f);
				w3.setValue(f);
				w4.setValue(3.0*f);

				z1 = z_series->getValue(k);
				z2 = z_series->getValue(k+len);
				z3 = z_series->getValue(k+2*len);
				z4 = z_series->getValue(k+3*len);

				w1 = *z1;
				w2 = (*z2)*w2;
				w3 = (*z3)*w3;
				w4 = (*z4)*w4;

				result1 =w1 + w2 + w3 + w4;
				result3 = w1 + w2 - w3 - w4;
				w3.setNegativeJ();
				w4.setNegativeJ();
				result2 = w1 - w2 + w3 - w4;
				result4 = w1 - w2 - w3 + w4;

				z_series->setValue(k, &result1);
				z_series->setValue(k+len, &result2);
				z_series->setValue(k+2*len, &result3);
				z_series->setValue(k+3*len, &result4);
			}
		}
		len = len<<2;
		step = step<<2;
	}
	return z_series;
}

ComplexSeries* fft_mixbase_4_2(ComplexSeries* a)
{
	if(NULL == a)
		return NULL;
	int length = a->getLength();
	if(length <= 0)
		return NULL;
	unsigned int model_2 = 0;
	unsigned int model_4 = 0;
	while (1)
	{
		if(!(((unsigned int)length>>model_2) & 0x0001))
			model_2++;
		else if( ((unsigned int)length>>model_2)!=0x0001 )
			return NULL;
		else
		{
			model_4 = model_2>>0x0001;
			break;
		}
	}

	double f=0.0;
	int i=0, j=0, k=0, len=0, step=0, n=0, m=0;
	ComplexSeries* z_series = new ComplexSeries(length);
	if(NULL == z_series)
	{
		cout<<"fft_mixbase_4_2 memory malloc failed"<<endl;
		return NULL;
	}
	ComplexNum *z, *z1, *z2, *z3, *z4;
	ComplexNum w, w1, w2, w3, w4;
	ComplexNum result1, result2, result3, result4;
	double re1, re2, re3, re4, im1, im2, im3, im4;

	for(i=0;i<length;i++)
	{
		j = reverse((unsigned int)i, model_2);
		if(-1 == j)
		{
			delete z_series;
			return NULL;
		}
		z = a->getValue(i);
		z_series->setValue(j, z);
	}

	//最低级的基4特殊处理，以加快速度
	if(model_4 > 0)
	{
		len = 1;
		step = 4;
		for(i=0;i<length;i+=step)
		{
			re1 = z_series->getValue(i)->getRe();
			re2 = z_series->getValue(i+1)->getRe();
			re3 = z_series->getValue(i+2)->getRe();
			re4 = z_series->getValue(i+3)->getRe();
			im1 = z_series->getValue(i)->getIm();
			im2 = z_series->getValue(i+1)->getIm();
			im3 = z_series->getValue(i+2)->getIm();
			im4 = z_series->getValue(i+3)->getIm();

			result1.setValue(re1+re2+re3+re4, im1+im2+im3+im4);
			result2.setValue(re1-re2+im3-im4, im1-im2-re3+re4);
			result3.setValue(re1+re2-re3-re4, im1+im2-im3-im4);
			result4.setValue(re1-re2-im3+im4, im1-im2+re3-re4);

			z_series->setValue(i, &result1);
			z_series->setValue(i+1, &result2);
			z_series->setValue(i+2, &result3);
			z_series->setValue(i+3, &result4);
		}
		len = len<<2;
		step = step<<2;
	}
	else
	{
		len = 1;
		step = 2;
	}

	//剩下的基4处理
	for(i=1;i<model_4;i++)
	{
		for(j=0;j<length;j+=step)
		{
			for(k=j;k<j+len;k++)
			{
				f = -1*2*PI*(double)(k%len)/(double)step;
				w2.setValue(2.0*f);
				w3.setValue(f);
				w4.setValue(3.0*f);

				z1 = z_series->getValue(k);
				z2 = z_series->getValue(k+len);
				z3 = z_series->getValue(k+2*len);
				z4 = z_series->getValue(k+3*len);

				w1 = *z1;
				w2 = (*z2)*w2;
				w3 = (*z3)*w3;
				w4 = (*z4)*w4;

				result1 = w1 + w2 + w3 + w4;
				result3 = w1 + w2 - w3 - w4;
				w3.setNegativeJ();
				w4.setNegativeJ();
				result2 = w1 - w2 + w3 - w4;
				result4 = w1 - w2 - w3 + w4;

				z_series->setValue(k, &result1);
				z_series->setValue(k+len, &result2);
				z_series->setValue(k+2*len, &result3);
				z_series->setValue(k+3*len, &result4);
			}
		}
		len = len<<2;
		step = step<<2;
	}

	//最高级的基2处理
	if((model_2 & 0x0001))
	{
		len = length/2;
		for(i=0;i<len;i++)
		{
			f = -1*2*PI*(double)i/(double)length;
			w.setValue(f);
			z1 = z_series->getValue(i);
			z2 = z_series->getValue(i+len);
			w = (*z2)*w;

			result1 = *z1 + w;
			result2 = *z1 - w;

			z_series->setValue(i, &result1);
			z_series->setValue(i+len, &result2);
		}
	}
	return z_series;
}