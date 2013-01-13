#include "common.h"
#include "./BasicExcel/BasicExcelVC6.hpp"
using namespace YExcel;

#define LENGTH 1024*1024
//#define PRINT
#define DFT
#define FFT_RECURSIVE
#define FFT_ITERATIVE_BASE2
#define FFT_ITERATIVE_BASE4
//#define FFT_MIXBASE_2_4
#define FFT_MIXBASE_4_2
#define RFFT
//#define EXCEL

int main()
{
	int i;
	double re, im;

#ifdef EXCEL
	bool has_im = false;
	size_t max_rows=0, max_cols=0;
	BasicExcel excel_in, excel_out;
	BasicExcelWorksheet *in_sheet, *out_sheet;
	BasicExcelCell *in_cell_re, *in_cell_im, *out_cell_re, *out_cell_im;
	ComplexSeries *z_series, *result;

	cout<<"Reading excel file......"<<endl<<endl;
	excel_in.Load("./in.xls");
	in_sheet = excel_in.GetWorksheet("Sheet1");
	if(!in_sheet)
	{
		cout<<"Load excel fail"<<endl;
		system("pause");
		return 0;
	}

	max_rows = in_sheet->GetTotalRows();
	max_cols = in_sheet->GetTotalCols();
	if(1 == max_rows)
	{
		cout<<"You have not input any data!"<<endl;
		system("pause");
		return 0;
	}
	z_series = new ComplexSeries(max_rows-1);
	if(NULL == z_series)
	{
		cout<<"Memory malloc failed"<<endl;
		system("pause");
		return 0;
	}

	for(i=1;i<max_rows;i++)
	{
		in_cell_re = in_sheet->Cell(i, 0);
		in_cell_im = in_sheet->Cell(i, 1);
		if( in_cell_re->Type() != BasicExcelCell::DOUBLE
			||
			in_cell_im->Type() != BasicExcelCell::DOUBLE
		)
		{
			delete z_series;
			cout<<"There is a un-numeric at row of "<<i<<endl;
			system("pause");
			return 0;
		}
		re = in_cell_re->GetDouble();
		im = in_cell_im->GetDouble();
		if(0.0!=im && false==has_im)
			has_im = true;
		z_series->setValue(i-1, re, im);
	}

	cout<<"FFT ING......"<<endl<<endl;
	result = FFT(z_series, has_im);
	if(NULL == result)
	{
		cout<<"FFT failed!"<<endl;
		delete z_series;
		system("pause");
		return 0;
	}

	cout<<"Writing result to excel file......"<<endl<<endl;
	excel_out.New(1);
	out_sheet = excel_out.GetWorksheet("Sheet1");
	if(!out_sheet)
	{
		cout<<"Create excel fail"<<endl;
		system("pause");
		return 0;
	}

	out_cell_re = out_sheet->Cell(0, 0);
	out_cell_im = out_sheet->Cell(0, 1);
	out_cell_re->SetDouble(0.0);
	out_cell_im->SetDouble(0.0);
	max_rows = result->getLength();
	for(i=0;i<max_rows;i++)
	{
		re = result->getValue(i)->getRe();
		im = result->getValue(i)->getIm();
		out_cell_re = out_sheet->Cell(i, 0);
		out_cell_im = out_sheet->Cell(i, 1);
		out_cell_re->SetDouble(re);
		out_cell_im->SetDouble(im);
	}

	excel_out.SaveAs("./out.xls");
	delete result;
	delete z_series;
	system("pause");
	return 0;
#endif
	
#ifdef LENGTH
	SYSTEMTIME time;
	srand((unsigned int)GetCurrentTime());
	ComplexSeries* result;
	ComplexSeries* z=new ComplexSeries(LENGTH);
	for(i=0;i<LENGTH;i++)
	{
		re = rand()%10;
		im = rand()%10;
		#ifdef RFFT
		z->setValue(i, re, 0.0);
		#else
		z->setValue(i, re, im);
		#endif
	}
#endif

#ifdef DFT
	result = z;
	cout<<"DFT START!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	result = dft(result);
	if(NULL == result)
		cout<<"NULL == result!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	cout<<"DFT END!"<<endl;	
	#ifdef PRINT
	if(NULL != result)
		result->print();
	#endif
	cout<<endl;
#endif

#ifdef FFT_ITERATIVE_BASE2
	result = z;
	cout<<"FFT_ITERATIVE_BASE2 START!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	result = fft_iterative_base2(result);
	if(NULL == result)
		cout<<"NULL == result!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	cout<<"FFT_ITERATIVE_BASE2 END!"<<endl;	
	#ifdef PRINT
	if(NULL != result)
		result->print();
	#endif
	cout<<endl;
#endif

#ifdef FFT_ITERATIVE_BASE4
	result = z;
	cout<<"FFT_ITERATIVE_BASE4 START!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	result = fft_iterative_base4(result);
	if(NULL == result)
		cout<<"NULL == result!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	cout<<"FFT_ITERATIVE_BASE4 END!"<<endl;	
	#ifdef PRINT
	if(NULL != result)
		result->print();
	#endif
	cout<<endl;
#endif

#ifdef FFT_MIXBASE_2_4
	result = z;
	cout<<"FFT_MIXBASE_2_4 START!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	result = fft_mixbase_2_4(result);
	if(NULL == result)
		cout<<"NULL == result!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	cout<<"FFT_MIXBASE_2_4 END!"<<endl;	
	#ifdef PRINT
	if(NULL != result)
		result->print();
	#endif
	cout<<endl;
#endif

#ifdef FFT_MIXBASE_4_2
	result = z;
	cout<<"FFT_MIXBASE_4_2 START!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	result = fft_mixbase_4_2(result);
	if(NULL == result)
		cout<<"NULL == result!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	cout<<"FFT_MIXBASE_4_2 END!"<<endl;	
	#ifdef PRINT
	if(NULL != result)
		result->print();
	#endif
	cout<<endl;
#endif

#ifdef RFFT
	result = z;
	cout<<"RFFT START!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	result = rfft(result);
	if(NULL == result)
		cout<<"NULL == result!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	cout<<"RFFT END!"<<endl;	
#ifdef PRINT
	if(NULL != result)
		result->print();
#endif
	cout<<endl;
#endif

#ifdef FFT_RECURSIVE
	result = z;
	cout<<"FFT_RECURSIVE START!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	result = fft_recursive(result);
	if(NULL == result)
		cout<<"NULL == result!"<<endl;
	GetLocalTime(&time);
	cout<<"minute:"<<time.wMinute<<", second:"<<time.wSecond<<", millisecond:"<<time.wMilliseconds<<endl;
	cout<<"FFT_RECURSIVE END!"<<endl;	
	#ifdef PRINT
	if(NULL != result)
		result->print();
	#endif
	cout<<endl;
#endif
	system("pause");
	return 0;
}