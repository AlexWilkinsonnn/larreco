// ***************************************************
// trajcluster debug viewer
// 
// Rory Fitzpatrick
// 
// ***************************************************

#ifndef showertree_h
#define showertree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TPolyLine.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

class showertree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t            run;
   Int_t            subrun;
   Int_t            event;

   vector<float>   *BeginWir;
   vector<float>   *BeginTim;
   vector<float>   *BeginAng;
   vector<float>   *BeginChg;
   vector<short>   *BeginVtx;
   vector<float>   *EndWir;
   vector<float>   *EndTim;
   vector<float>   *EndAng;
   vector<float>   *EndChg;
   vector<short>   *EndVtx;
   vector<short>   *MCSMom;
   vector<short>   *PlaneNum;
   vector<int>     *TjID;
   vector<int>     *IsShowerTj;
   vector<int>     *ShowerID;
   vector<int>     *IsShowerParent;
   vector<int>     *StageNum;
   vector<string>  *StageName;

   vector<float>   *Envelope;
   vector<int>     *EnvPlane;
   vector<int>     *EnvStage;
   vector<int>     *EnvShowerID;

   Int_t            nStages;
   Int_t            nPlanes;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_subrun;   //!
   TBranch        *b_event;   //!

   TBranch        *b_BeginWir;   //!
   TBranch        *b_BeginTim;   //!
   TBranch        *b_BeginAng;   //!
   TBranch        *b_BeginChg;   //!
   TBranch        *b_BeginVtx;   //!
   TBranch        *b_EndWir;   //!
   TBranch        *b_EndTim;   //!
   TBranch        *b_EndAng;   //!
   TBranch        *b_EndChg;   //!
   TBranch        *b_EndVtx;   //!
   TBranch        *b_MCSMom;
   TBranch        *b_PlaneNum;   //!
   TBranch        *b_TjID;   //!
   TBranch        *b_IsShowerTj;   //!
   TBranch        *b_ShowerID;   //!
   TBranch        *b_IsShowerParent;   //!
   TBranch        *b_StageNum;   //!
   TBranch        *b_StageName;   //!
   TBranch        *b_Envelope;
   TBranch        *b_EnvPlane;
   TBranch        *b_EnvStage;
   TBranch        *b_EnvShowerID;
   TBranch        *b_nStages;
   TBranch        *b_nPlanes;

   showertree(TTree *tree=0);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
};

#endif

showertree::showertree(TTree *tree) : fChain(0) {
   Init(tree);
}

void showertree::Init(TTree *tree) {
   // Set object pointer
   BeginWir = 0;
   BeginTim = 0;
   BeginAng = 0;
   BeginChg = 0;
   BeginVtx = 0;
   EndWir = 0;
   EndTim = 0;
   EndAng = 0;
   EndChg = 0;
   EndVtx = 0;
   MCSMom = 0;
   PlaneNum = 0;
   TjID = 0;
   IsShowerTj = 0;
   ShowerID = 0;
   IsShowerParent = 0;
   StageNum = 0;
   StageName = 0;
   Envelope = 0;
   EnvPlane = 0;
   EnvStage = 0;
   EnvShowerID = 0;

   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("subrun", &subrun, &b_subrun);
   fChain->SetBranchAddress("event", &event, &b_event);

   fChain->SetBranchAddress("BeginWir", &BeginWir, &b_BeginWir);
   fChain->SetBranchAddress("BeginTim", &BeginTim, &b_BeginTim);
   fChain->SetBranchAddress("BeginAng", &BeginAng, &b_BeginAng);
   fChain->SetBranchAddress("BeginChg", &BeginChg, &b_BeginChg);
   fChain->SetBranchAddress("BeginVtx", &BeginVtx, &b_BeginVtx);
   fChain->SetBranchAddress("EndWir", &EndWir, &b_EndWir);
   fChain->SetBranchAddress("EndTim", &EndTim, &b_EndTim);
   fChain->SetBranchAddress("EndAng", &EndAng, &b_EndAng);
   fChain->SetBranchAddress("EndChg", &EndChg, &b_EndChg);
   fChain->SetBranchAddress("EndVtx", &EndVtx, &b_EndVtx);
   fChain->SetBranchAddress("MCSMom", &MCSMom, &b_MCSMom);
   fChain->SetBranchAddress("PlaneNum", &PlaneNum, &b_PlaneNum);
   fChain->SetBranchAddress("TjID", &TjID, &b_TjID);
   fChain->SetBranchAddress("IsShowerTj", &IsShowerTj, &b_IsShowerTj);
   fChain->SetBranchAddress("ShowerID", &ShowerID, &b_ShowerID);
   fChain->SetBranchAddress("IsShowerParent", &IsShowerParent, &b_IsShowerParent);
   fChain->SetBranchAddress("StageNum", &StageNum, &b_StageNum);
   fChain->SetBranchAddress("StageName", &StageName, &b_StageName);
   fChain->SetBranchAddress("Envelope", &Envelope, &b_Envelope);
   fChain->SetBranchAddress("EnvPlane", &EnvPlane, &b_EnvPlane);
   fChain->SetBranchAddress("EnvStage", &EnvStage, &b_EnvStage);
   fChain->SetBranchAddress("EnvShowerID", &EnvShowerID, &b_EnvShowerID);
   fChain->SetBranchAddress("nStages", &nStages, &b_nStages);
   fChain->SetBranchAddress("nPlanes", &nPlanes, &b_nPlanes);
}
