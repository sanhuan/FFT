#include "common.h"

ComplexSeries* dft(ComplexSeries* a)
{
	if(NULL == a)
		return NULL;
	int i, j;
	int length = a->getLength();
	if(length <= 0)
		return NULL;
	ComplexNum w(1.0, 0.0);
	ComplexNum wn(-1*length);
	ComplexNum aa;
	ComplexSeries* y = new ComplexSeries(length);
	if(NULL == y)
	{
		cout<<"dft memory malloc failed"<<endl;
		return NULL;
	}
	ComplexNum yy(0.0, 0.0);

	for(i=0;i<length;i++)
	{
		for(j=0;j<length-1;j++)
		{
			aa = *(a->getValue(length-j-1));
			yy = w*(aa+yy);
		}
		aa = *(a->getValue(0));
		yy = yy+aa;
		y->setValue(i, &yy);
		w = w*wn;
		yy.setValue(0.0, 0.0);
	}
	return y;
}