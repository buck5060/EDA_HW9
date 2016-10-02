#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <cstdlib>
#include <sstream>

using namespace std;

class mos
{
	public:
		mos();
		~mos();
		string width;
		string len;
		string drain;
		string source;
		string base;
		string gate;
		string mosName;
		string mostype;

		string end;
		string nmos;
		string pmos;
};

mos::mos()
{
	end = ".ENDS";
	nmos = "Nch";
	pmos = "Pch";
}

mos::~mos()
{
}

string CreatePmos(string name,double  x, double y, string w, string l)
{
	string cmd;
	stringstream sx;
	stringstream sy;
	sx << x;
	sy << y;

	cmd = "lakerCreateTransistor -point (" + sx.str() + "," + sy.str() + ")) -inst " + name + " -useMaskResolution 0 -device PMOS_1.8V -gates 2  -contType MaxContact -xEnc 0.06 -yEnc 0.06 -xConSpace 0.25 -yConSpace 0.25 -co2Gate 0.16 -contAlign center -xCo2OdEnc 0.1 -yCo2OdEnc 0.1 -param {patternA " + w + " " + l + "  asIs 0.0} -optLayerRepeatPattern { } -traitRepeatPattern { } -layerSetting { PIMP drawing enable retangle }";
	return cmd;
}

string CreateNmos(string name, double x,double y, string w, string l)
{
	string cmd;
	stringstream sx;
        stringstream sy;
        sx << x;
        sy << y;

        cmd = "lakerCreateTransistor -point (" + sx.str() + "," + sy.str() + ")) -inst " + name + " -useMaskResolution 0 -device NMOS_1.8V -gates 2  -contType MaxContact -xEnc 0.06 -yEnc 0.06 -xConSpace 0.25 -yConSpace 0.25 -co2Gate 0.16 -contAlign center -xCo2OdEnc 0.1 -yCo2OdEnc 0.1 -param {patternA  " + w + " " + l + "asIs 0.0} -optLayerRepeatPattern { } -traitRepeatPattern { } -layerSetting { NIMP drawing enable retangle }";
        return cmd;

}

string marklabel(double x, double y, string label)
{
	string cmd;
	stringstream sx;
        stringstream sy;
        sx << x;
        sy << y;

	cmd = "lakerCreateText -layerName METAL1 -purpose pin -point (" + sx.str() + "," + sy.str() + ") -height 1.000000 -text {" + label + "}";

	return cmd;
}

string markcont(double x, double y)
{
	string cmd;
        stringstream sx;
        stringstream sy;
        sx << x;
        sy << y;

        cmd = "lakerCreateContact -point (" + sx.str() + "," + sy.str() + ") -device M1_P1 -routeStatus null -param {M1_P1  1  1  0.22 0.22 0.25 0.25 0.1 0.1 0.005 0.06 0 0.0 0.0 0.0 0.0 0.0 0.0 0 0 } -justify CenterCenter";

	return cmd;
}

string markmetalone(double fx, double fy, double dx, double dy)
{
	string cmd;
        stringstream sx;
        stringstream sy;
        stringstream ex;
        stringstream ey;
        sx << fx;
        sy << fy;
	ex << dx;
	ey << dy;

	cmd = "lakerCreateRect -layerName METAL1 -point (" + sx.str() + "," + sy.str() + ") (" + ex.str() + "," + ey.str() + ")";
	return cmd;
}

string markrouting(string pinName)
{
	string cmd;

	cmd = "lakerLabelRouter -text {" + pinName + "} -connCheck 0 -ruleSet \"default\" -rule \
	{POLY1 {Avail 1} {H 8} {V 4} {Width 0.18} {Spacing 0.25}} \
	{METAL1 {Avail 1} {H 1} {V 8} {Width 0.23} {Spacing 0.23}} \
	{METAL2 {Avail 1} {H 8} {V 1} {Width 0.28} {Spacing 0.28}} \
	{METAL3 {Avail 1} {H 1} {V 8} {Width 0.28} {Spacing 0.28}} \
	{METAL4 {Avail 1} {H 8} {V 1} {Width 0.28} {Spacing 0.28}} \
	{METAL5 {Avail 1} {H 1} {V 8} {Width 0.28} {Spacing 0.28}}\
	{METAL6 {Avail 1} {H 8} {V 1} {Width 1.5} {Spacing 1.5}} \
	{M1_P1 {Avail 1} {H 8} {L1Enc {0.1 0.1}} {L2Enc {0.005 0.06}}} \
	{M1_M2 {Avail 1} {H 5} {L1Enc {0.06 0.06}} {L2Enc {0.06 0.06}}} \
	{M2_M3 {Avail 1} {H 5} {L1Enc {0.06 0.06}} {L2Enc {0.06 0.06}}} \
	{M3_M4 {Avail 1} {H 5} {L1Enc {0.06 0.06}} {L2Enc {0.06 0.06}}} \
	{M4_M5 {Avail 1} {H 5} {L1Enc {0.06 0.06}} {L2Enc {0.06 0.06}}}\
	{M5_M6 {Avail 1} {H 5} {L1Enc {0.06 0.06}} {L2Enc {0.3 0.45}}} -connPathCenter -style Truncate -viaGroup MCell -routeStatus routed -minContacts 2 -pathAlign center -viaPattern Cross" ;

	return cmd;
}

int main ()
{
	//cout << CreatePmos(1,1) << endl; //test
	ifstream netlist("bias.sp");
	ofstream tclcmd("bias.tcl");
	mos* mosList;
	mosList = new mos[15];

	string a,b;
	int temp = 0;

	int nposx = 0;
	int nposy = 6;

	int pposx = 0;
	int pposy = 0;


	netlist.ignore(256,'O');
	netlist.ignore(256,'O');

	netlist >> mosList[0].mosName >> mosList[0].drain >> mosList[0].gate >> mosList[0].source >> mosList[0].base >> mosList[0].mostype >> a >> b;

	mosList[0].width = a[2];
	mosList[0].len = b[2];
	//mosList[0].width = atoi( a[2].c_str() ) ;



	//load as object
	for(int i = 0; strcmp(mosList[i].mosName.c_str(),mosList[0].end.c_str());)
	{
		/* output test
		cout << mosList[i].mosName << ":"<< endl;
		cout << mosList[0].end << ":" << endl;
		cout << mosList[i].drain << endl;
		cout << mosList[i].gate << endl;
		cout << mosList[i].source << endl;
		cout << mosList[i].base << endl;
		cout << mosList[i].mostype << endl;
		cout << mosList[i].width << endl;
		cout << mosList[i].len << endl;
		cout << "" << endl;
		*/
		i++;

		netlist >> mosList[i].mosName >> mosList[i].drain >> mosList[i].gate >> mosList[i].source >> mosList[i].base >> mosList[i].mostype >> a >> b;
		mosList[i].width = a[2];
	        mosList[i].len = b[2];
		//cout << strcmp(mosList[i].mosName.c_str(),mosList[0].end.c_str()) << endl;

	}

	//cout << "creating file" << endl;
	//place and label
	for(int i = 0; i < 14; i++)
	{
		//cout << "classifing" << endl;
		if(strcmp(mosList[i].mostype.c_str(),mosList[i].nmos.c_str()) == 0)
		{
			tclcmd << CreateNmos(mosList[i].mosName, nposx, nposy, mosList[i].width,  mosList[i].len) << endl;
			//cout << "it is nmos" << endl;
			tclcmd << marklabel((nposx + 0.2), (nposy + 1.7), mosList[i].source) << endl;
			tclcmd << marklabel((nposx + 1), (nposy + 2), mosList[i].gate) << endl;
			//gate with contact
			tclcmd << markcont (nposx + 1 - 0.11, nposy + 2 - 0.11) << endl;

			tclcmd << marklabel((nposx + 1.75), (nposy + 1.7), mosList[i].drain) << endl;

			nposx += 5;
		}

		if(strcmp(mosList[i].mostype.c_str(),mosList[i].pmos.c_str()) == 0)
                {
                        tclcmd << CreatePmos(mosList[i].mosName, pposx, pposy, mosList[i].width,  mosList[i].len) << endl;
			tclcmd << marklabel((pposx + 0.21), (pposy + 3.635), mosList[i].source) << endl;
                        tclcmd << marklabel((pposx + 1.4), (pposy + 4), mosList[i].gate) << endl;
			tclcmd << markcont (pposx + 1.4 - 0.11, pposy + 4 - 0.11) << endl;
                        tclcmd << marklabel((pposx + 2.75),( pposy + 3.635), mosList[i].drain) << endl;

			pposx += 5;
                }

	}

	//routing
	string pinList[15];
	int pinSel = 0;
	bool Dexist = 0;
	bool Gexist = 0;
	bool Sexist = 0;

	for (int i = 0; i < 14; i++)
	{
		for(int j = 0; j < 14; j++)
		{
			if(strcmp(mosList[i].drain.c_str(),pinList[j].c_str()) == 0)
				Dexist = 1;
		}

		if(!Dexist)
		{
			cout << "NEW Drain Label~";
			pinList[pinSel++] = mosList[i].drain;
			tclcmd << markrouting(mosList[i].drain) << endl;
		}

		for(int j = 0; j < 14; j++)
		{
			if(strcmp(mosList[i].gate.c_str(),pinList[j].c_str()) == 0)
				Gexist = 1;
		}

		if(!Gexist)
		{
			pinList[pinSel++] = mosList[i].gate;
			tclcmd << markrouting(mosList[i].gate) << endl;
		}

		for(int j = 0; j < 14; j++)
		{
			if(strcmp(mosList[i].source.c_str(),pinList[j].c_str()) == 0)
				Sexist = 1;
		}
		if(!Sexist)
		{
			pinList[pinSel++] = mosList[i].source;
			tclcmd << markrouting(mosList[i].source) << endl;
		}

		Dexist = 0;
	        Gexist = 0;
        	Sexist = 0;

	}


	return 0;
}
