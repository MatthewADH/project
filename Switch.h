#include "Simulator.h"

class Switch : public Device
{
public:

	//Constructor 
	Switch(int nodei, int nodej, double Ron, double Roff, bool pos=false);

	//Device Interface
	void Init();
	void Step(double t, double dt);
	void SetPos(bool pos);
	bool GetPos();

private:

	//member variables 
	int nodei;
	int nodej;
	int nodep;

	double Ron;
	double Roff;
	bool pos;
};

Switch::Switch(int nodei, int nodej, double Ron, double Roff, bool pos)
{
	this->nodei = nodei;
	this->nodej = nodej;
	this->Ron = Ron;
	this->Roff = Roff;
	this->pos= pos;
}

void Switch::Init()
{
	this->nodep = GetNextNode();
}

void Switch::Step(double t, double dt)
{
	AddJacobian(nodei, nodep, 1);
	AddJacobian(nodej, nodep, -1);
	AddJacobian(nodep, nodei, 1);
	AddJacobian(nodep, nodej, -1);

	if (pos) //Switch is on
	{
		AddJacobian(nodep, nodep, -Ron);
		
		double b = -Ron * GetIterationState(nodep);
		AddBEquivalent(nodep, b);
	}
	else //Switch is off
	{
		AddJacobian(nodep, nodep, -Roff);
	
		double b = GetIterationStateDifference(nodei, nodej);
		AddBEquivalent(nodep, b);
	}
}

void Switch::SetPos(bool pos)
{
	this->pos = pos;
}

bool Switch::GetPos()
{
	return pos;
}