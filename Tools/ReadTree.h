#ifndef ReadTree_h
#define ReadTree_h

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include "TObject.h"
#include "TLeaf.h"
#include "TLeafObject.h"
#include "TObjArray.h"
using namespace std;

//for usual samples, it is recomended to uncommented the lines related to "UInt_t"
//but for samples of SS_ML, please remain the lines related to "UInt_t" commented, otherwise there will be some problems about reading branches due to the problem of the input sample.

class ReadTree
{
		public:
				map<string, int*> Int_Vars;
				map<string, double*> Double_Vars;
				map<string, float*> Float_Vars;
				map<string, bool*> Bool_Vars;
			    map<string, UInt_t*> UInt_Vars;

				map<string, vector<double>*> VectorDouble_Vars;
			    map<string, vector<int>*> VectorInt_Vars;
			    map<string, vector<bool>*> VectorBool_Vars;
                map<string, vector<float>*> VectorFloat_Vars;

				map<string, string> Var_type;
				vector<string> Leafs;
                vector<string> Active_Branches;

				TChain *chain;
				
				string input_file;
				string tree_name;
				string AcBranch;

		public:
				ReadTree(string infile, string intree, string acbranch);
				ReadTree(string infile, string intree);
//				virtual ~ReadTree();
				virtual void Init_all();
				virtual void Init_active();
				virtual void LoadChain();
				virtual void GetBranchNames();
				virtual void GetBranches();
				virtual void PrintProgressBar(int index, int total);
				virtual void Show(Long64_t entry = -1);
				virtual Int_t GetEntry(Long64_t entry);
};

ReadTree::ReadTree(string infile, string intree, string acbranch)
{
		input_file = infile;
		tree_name = intree;
		AcBranch = acbranch;
		Init_active();
}

ReadTree::ReadTree(string infile, string intree)
{
		input_file = infile;
		tree_name = intree;
		Init_all();
}

void ReadTree::GetBranchNames()
{
		ifstream infile (AcBranch.c_str(), ios::in);
		string name;
		while(getline(infile, name))
		{
				Active_Branches.push_back(name);
		}

}

/*
ReadTree::~ReadTree()
{
		delete chain -> GetCurrentFile();
}
*/
void ReadTree::LoadChain()
{
		ifstream input(input_file.c_str(), ios::in);
		string line;

		chain = new TChain(tree_name.c_str());
				while(getline(input, line))
				{
						if (line.find("#") == 0) continue;
						chain -> Add(line.c_str());
				}

		input.close();
}

void ReadTree::GetBranches()
{   
		TObjArray *leaves = chain -> GetListOfLeaves();
		for(int i = 0; i < leaves -> GetEntries(); i++)
		{
				TLeaf *leaf = (TLeaf*) leaves -> At(i);
				string name = (string) leaf -> GetName();
				string type = (string) leaf -> GetTypeName();
	      Leafs.push_back(name);

				int I_type = 0;
        if(type == "Int_t") I_type = 1;
				if(type == "Double_t") I_type = 2;
				if(type == "Bool_t ") I_type = 3;
				if(type == "vector<double>") I_type = 4;
				if(type == "vector<int>") I_type = 5;
				if(type == "vector<bool>") I_type = 6;
				if(type == "Float_t") I_type = 7;
        if(type == "vector<float>") I_type = 8;
        if(type == "UInt_t") I_type = 9;

				switch (I_type)
				{
						case 1:
						{		
								int *Int = new int();
								Int_Vars[name] = Int;
								Var_type[name] = "Int_t";
								break;
						}
						case 2:
						{
								double *Double = new double();
								Double_Vars[name] = Double;
								Var_type[name] = "Double_t";
								break;
						}
						case 3:
						{
								bool *Bool = new bool();
								Bool_Vars[name] = Bool;
								Var_type[name] = "Bool_t";
								break;    
						}
						case 4:
						{
								vector<double> *VectorDouble = new vector<double>();
								VectorDouble_Vars[name] = VectorDouble;
								Var_type[name] = "vector<double>";
								break;
						}
						case 5:
						{
								vector<int> *VectorInt = new vector<int>();
								VectorInt_Vars[name] = VectorInt;
								Var_type[name] = "vector<int>";
								break;
						}
						case 6:
						{
								vector<bool> *VectorBool = new vector<bool>();
								VectorBool_Vars[name] = VectorBool;
								Var_type[name] = "vector<bool>";
								break;
						}
						case 7:
						{
								float *Float = new float();
								Float_Vars[name] = Float;
								Var_type[name] = "Float_t";
								break;
						}
						case 8:
						{
								vector<float> *VectorFloat = new vector<float>();
								VectorFloat_Vars[name] = VectorFloat;
								Var_type[name] = "vector<float>";
								break;
						}
						case 9:
						{
								UInt_t *UInt = new UInt_t();
								UInt_Vars[name] = UInt;
								Var_type[name] = "UInt_t";
								break;
						}
				}
		}
}


void ReadTree::Init_all()
{
		LoadChain();
		GetBranches();
		GetBranchNames();

		//Set Branch status and addreses
		chain -> SetBranchStatus("*", 0);
		for(int i = 0; i < Leafs.size(); i++)
		{
				chain -> SetBranchStatus(Leafs[i].c_str(), 1);
				int I_type = 0;
        if(Var_type[Leafs[i]]== "Int_t") I_type = 1;
				if(Var_type[Leafs[i]]== "Double_t") I_type = 2;
				if(Var_type[Leafs[i]]== "Bool_t") I_type = 3;
        if(Var_type[Leafs[i]]== "vector<double>") I_type = 4;
				if(Var_type[Leafs[i]]== "vector<int>") I_type = 5;
				if(Var_type[Leafs[i]]== "vector<bool>") I_type = 6;
				if(Var_type[Leafs[i]]== "Float_t") I_type = 7;
				if(Var_type[Leafs[i]]== "vector<float>") I_type = 8;
        if(Var_type[Leafs[i]]== "UInt_t") I_type = 9;

	  		switch(I_type)
				{
						case 1:
						{
								chain -> SetBranchAddress(Leafs[i].c_str(), Int_Vars[Leafs[i]]);
								break;
						}
						case 2:
						{
								chain -> SetBranchAddress(Leafs[i].c_str(), Double_Vars[Leafs[i]]);
								break;
						}
						case 3:
						{		
								chain -> SetBranchAddress(Leafs[i].c_str(), Bool_Vars[Leafs[i]]);
								break;
						}
						case 4:
						{		
								chain -> SetBranchAddress(Leafs[i].c_str(), &VectorDouble_Vars[Leafs[i]]);
								break;
						}
						case 5:
						{		
								chain -> SetBranchAddress(Leafs[i].c_str(), &VectorInt_Vars[Leafs[i]]);
								break;
						}
						case 6:
						{		
								chain -> SetBranchAddress(Leafs[i].c_str(), &VectorBool_Vars[Leafs[i]]);
								break;
						}
						case 7:
						{
								chain -> SetBranchAddress(Leafs[i].c_str(), Float_Vars[Leafs[i]]);
								break;
						}
						case 8:
						{		
								chain -> SetBranchAddress(Leafs[i].c_str(), &VectorFloat_Vars[Leafs[i]]);
								break;
						}
						case 9:
						{		
								chain -> SetBranchAddress(Leafs[i].c_str(), UInt_Vars[Leafs[i]]);
								break;
						}
				}

		}
}

void ReadTree::Init_active()
{
		LoadChain();
		GetBranches();
		GetBranchNames();

		//Set Branch status and addreses
		chain -> SetBranchStatus("*", 0);
		for(int i = 0; i < Active_Branches.size(); i++)
		{
				chain -> SetBranchStatus(Active_Branches[i].c_str(), 1);
				int I_type = 0;
        if(Var_type[Active_Branches[i]]== "Int_t") I_type = 1;
				if(Var_type[Active_Branches[i]]== "Double_t") I_type = 2;
				if(Var_type[Active_Branches[i]]== "Bool_t") I_type = 3;
        if(Var_type[Active_Branches[i]]== "vector<double>") I_type = 4;
				if(Var_type[Active_Branches[i]]== "vector<int>") I_type = 5;
				if(Var_type[Active_Branches[i]]== "vector<bool>") I_type = 6;
				if(Var_type[Active_Branches[i]]== "Float_t") I_type = 7;
        if(Var_type[Active_Branches[i]]== "vector<float>") I_type = 8;
        if(Var_type[Active_Branches[i]]== "UInt_t") I_type = 9;

	  		switch(I_type)
				{
						case 1:
						{
								chain -> SetBranchAddress(Active_Branches[i].c_str(), Int_Vars[Active_Branches[i]]);
								break;
						}
						case 2:
						{
								chain -> SetBranchAddress(Active_Branches[i].c_str(), Double_Vars[Active_Branches[i]]);
								break;
						}
						case 3:
						{		
								chain -> SetBranchAddress(Active_Branches[i].c_str(), Bool_Vars[Active_Branches[i]]);
								break;
						}
						case 4:
						{		
								chain -> SetBranchAddress(Active_Branches[i].c_str(), &VectorDouble_Vars[Active_Branches[i]]);
								break;
						}
						case 5:
						{		
								chain -> SetBranchAddress(Active_Branches[i].c_str(), &VectorInt_Vars[Active_Branches[i]]);
								break;
						}
						case 6:
						{		
								chain -> SetBranchAddress(Active_Branches[i].c_str(), &VectorBool_Vars[Active_Branches[i]]);
								break;
						}
						case 7:
						{
								chain -> SetBranchAddress(Active_Branches[i].c_str(), Float_Vars[Active_Branches[i]]);
								break;
						}
						case 8:
						{
								chain -> SetBranchAddress(Active_Branches[i].c_str(), &VectorFloat_Vars[Active_Branches[i]]);
								break;
						}
						case 9:
						{		
								chain -> SetBranchAddress(Active_Branches[i].c_str(), UInt_Vars[Active_Branches[i]]);
								break;
						}

				}
		}
}

void ReadTree::PrintProgressBar(int index, int total)
{
		if (index % 100 == 0)
		{
				TString print_bar = " [";
				for (int bar = 0; bar < 20; bar++)
				{
						double current_fraction = double(bar) / 20.0;
						if (double(index) / double(total) > current_fraction)
								print_bar.Append("/");
						else
								print_bar.Append(".");
				}
				print_bar.Append("] ");
				std::cout << print_bar << 100. * (double(index) / double(total)) << "%\r" << std::flush;
		}
}

void ReadTree::Show(Long64_t entry)
{
		if(!chain) return;
		chain -> Show(entry);
}

Int_t ReadTree::GetEntry(Long64_t entry)
{
		if (!chain) return 0;
		return chain -> GetEntry(entry);
}

#endif
