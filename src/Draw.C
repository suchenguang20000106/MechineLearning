#include "../Tools/ReadTree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include "TH2F.h"
#include "TColor.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFrame.h"

using namespace std;

void DrawOneRoot(string infile, string outpath)
{
    ReadTree *rt = new ReadTree(infile, "Cerenkov_vector");
    int TotalEntries = rt -> chain -> GetEntries();

    TH2F* che = new TH2F("Cherenkov", "", 150, -250, 250, 1500, -400, 1200);
    
    //cout << 0 << endl; 

    for(int i = 0; i < TotalEntries; i++)
    {
        rt -> GetEntry(i);
        vector<double> CheE = *rt -> VectorDouble_Vars["CheE"];
        vector<double> CheY = *rt -> VectorDouble_Vars["CheY"];
        vector<double> CheZ = *rt -> VectorDouble_Vars["CheZ"];

        if(CheE.size() == 0) continue;

        for(int j = 0; j < CheE.size(); j++)
        {
            che -> Fill(CheY[j], CheZ[j],  CheE[j] / 1000.);
        }
        //cout << 1 << endl;
    }

    che -> SetStats(false);

    TCanvas *c = new TCanvas("c","c", 700, 700);
    gStyle->SetPalette(kGreyScale);
    TColor::InvertPalette();

    che -> Draw("COL AH");   
        
    c -> SetFrameLineColor(0);
    c -> SetFrameLineWidth(0);
    c -> SetFrameBorderMode(0);
    c -> SetFrameBorderSize(0);

    gStyle -> SetDrawBorder(0);
    c -> SetBorderMode(0);
    c -> SetBorderSize(0);
    //c -> SetPadBorderMode(0);

    c -> SaveAs((outpath +  infile.substr(infile.rfind("/") + 1, infile.rfind(".") - infile.rfind("/") - 1) + ".png").c_str());
    delete c;
    delete che;
    TColor::InvertPalette();
}

int main(int argc, char** argv)
{
    string filelist = (string)argv[1];
    string outpath = (string)argv[2];

    ifstream *ins = new ifstream(filelist);
    string infile;

    while(getline(*ins, infile))
    {
        DrawOneRoot(infile, outpath);
    }

    return 0;
}
