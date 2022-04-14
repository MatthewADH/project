#include "Simulator.h"
#include <vector>
#include "Diode.h""
#include "Switch.h"

class Converter : public Device
{

public:

	//Constructor
	Converter(int batpos, int batneg, int motpos, int motneg, double fs, double Isat, double Vt, double nfactor, double Ron, double Roff, double mode0, double d=1.0);

	//Device Interface
	void Init();
	void Step(double t, double dt);
	
	//Setters 
	void SetDuty(double );
	void SetMode(double mode);

	//Getters
	double GetInputVoltage();
	double GetOutputVoltage();

private:

	//Member Variables
	int batpos;
	int batneg;
	int motpos;
	int motneg;
	double fs;
	double Ts;
	double mode;
	double d;
	bool on;

	Diode D1;
	Diode D2;
	Diode D3;
	Diode D4;

	Switch S1;
	Switch S2;
	Switch S3;
	Switch S4;

	vector<Device*> devices;

};

Converter::Converter(int batpos, int batneg, int motpos, int motneg, double fs, double Isat, double Vt, double nfactor, double Ron, double Roff, double mode0, double d)  :
	S1(motpos, batpos, Ron, Roff), S2(motneg, batpos, Ron, Roff), S3(batneg, motpos, Ron, Roff), S4(batneg, motneg, Ron, Roff),
	D1(motpos, batpos, Vt, Isat, nfactor), D2(motneg, batpos, Vt, Isat, nfactor), D3(batneg, motpos, Vt, Isat, nfactor), D4(batneg, motneg, Vt, Isat, nfactor)
{
	this-> batpos = batpos;
	this-> batneg = batneg;
	this->motpos = motpos;
	this->motneg = motneg;
	this->fs = fs;
	this->Ts = 1 / fs;
	this->mode = mode0;
	this->d = d;
	
	devices.push_back(&S1);
	devices.push_back(&S2);
	devices.push_back(&S3);
	devices.push_back(&S4);

}

void Converter::Init()
{
	for (int i = 0; i < devices.size(); i++)
	{
		devices[i]->SetSim(*sim);
		devices[i]->Init();
	}
}

void Converter::Step(double t, double dt)
{
	on = fs * fmod(t, Ts) < d;
	
	if (mode == -1) //Reverse 
	{
		S1.SetPos(false);
		S2.SetPos(on);
		S3.SetPos(on);
		S4.SetPos(false);
	}
	else if (mode == 0) //breaking 
	{
		S1.SetPos(on);
		S2.SetPos(on);
		S3.SetPos(false);
		S4.SetPos(false);
	}
	else //forwards 
	{
		S1.SetPos(on);
		S2.SetPos(false);
		S3.SetPos(false);
		S4.SetPos(on);
	}

	for (int i = 0; i < devices.size(); i++)
	{
		devices[i]->Step(t, dt);

	}
}

void Converter::SetDuty(double d)
{
	this->d = clamp(d, 0.0, 1.0);
}

void Converter::SetMode(double mode)
{
	this->mode = mode;
}

double Converter::GetInputVoltage()
{
	return GetStateDifference(batpos, batneg);
}

double Converter::GetOutputVoltage()
{
	return GetStateDifference(motpos, motneg);
}