
#include "Simulator.h"
#include "Plotter.h"

#include "VoltageSource.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Converter.h"

int main()
{
	const double h = 0.7e-6;
	const double tmax = 12e-3;
	const double Va = 20;
	const double R = 1e2;
	const double C = 1e-5;

	Plotter plotter("Project", 1000, 600);
	plotter.SetLabels("vin (V)", "vout (V)");

	Simulator simulator(4, 0);

	VoltageSource V1(1, 0, Va);
	Resistor R1(1, 2, R);
	Capacitor C1(2, 0, C);
	Resistor R2(3, 4, R);
	Capacitor C2(3, 4, C);
	Converter converter(2, 0, 3, 4, 1000, 2e-6, 25.85e-3, 1.5, 1e6, 1e-3, 1);

	simulator.AddDevice(V1);
	simulator.AddDevice(R1);
	simulator.AddDevice(C1);
	simulator.AddDevice(R2);
	simulator.AddDevice(C2);
	simulator.AddDevice(converter);

	simulator.Init(h, tmax, 100, 1e-6, true);

	converter.SetDuty(0.5);
	converter.SetMode(-1);

	while (simulator.IsRunning())
	{
		plotter.AddRow(simulator.GetTime(), converter.GetInputVoltage(), converter.GetOutputVoltage());
		
		if (simulator.GetTime() > tmax / 4 * 3)
		{
			converter.SetMode(0);
		}

		simulator.Step();
	}

	plotter.Plot();

	return 0;
}