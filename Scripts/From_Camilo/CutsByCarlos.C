#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif

//------------------------------------------------------------------------------
#include <string>
#include <iostream>
#include "TMath.h"


float deltaPhi(float phi1, float phi2) {
  float PHI = phi1-phi2;
  if (PHI >= 3.14159265)
    PHI -= 2*3.14159265;
  else if (PHI < -3.14159265)
    PHI += 2*3.14159265;
 
    return PHI;
}

void CutsByCarlos_New(string argv)
{
  gSystem->Load("libDelphes");
  std::cout<<"Delphes Loaded"<<std::endl;

  // Create chain of root trees
  TChain * chain = new TChain("Delphes","Nombre");
  // chain->Add("/home/rsiii/SSSFDM/VLF/Signal/signal_8078/*.root");
  chain->Add(argv);


  std::cout<<"Chain Created"<<std::endl;

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
  Long64_t numberOfEntries = treeReader->GetEntries();
  // Get pointers to branches used in this analysis 
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
  TClonesArray *branchScalarHT = treeReader->UseBranch("ScalarHT");
  
  std::cout<<"Create object of class ExRootTreeReader"<<std::endl;

  //Creating counters
  Long64_t countTrig=0, count0=0, count1=0, count2=0, count3=0, count4=0, count5=0, countTotal=0;
  
  for ( Int_t entry = 0; entry < numberOfEntries; ++entry ){
    treeReader->ReadEntry(entry);
    // Load selected branches with data from specified event
    
    if (entry%10000==0) std::cout<<"Entry: "<< entry <<std::endl;

    
    int Jet_size = branchJet->GetEntries();  
    int Muon_size = branchMuon->GetEntries();
    
    //Creating pointer for each particle
    Jet* leadingJetptr;
    MissingET* metptr;
    Muon* leadingmuonptr;
    ScalarHT* HTptr;
    Jet* jeti;
    
    

    //Getting the values
    leadingJetptr = (Jet*)branchJet->At(0);
    metptr = (MissingET*)branchMissingET->At(0);
    leadingmuonptr = (Muon*)branchMuon->At(0);
    HTptr = (ScalarHT*)branchScalarHT->At(0);
    
    Double_t mht=0,htx=0,hty=0;

    for(Int_t i = 0; i < Jet_size; i++){
      jeti = (Jet*) branchJet->At(i);
      //      if (jeti->PT<=30.0) continue;
      double phii=jeti->Phi;
      htx+=jeti->PT*TMath::Cos(phii);
      hty+=jeti->PT*TMath::Sin(phii);
    }   
    mht=sqrt(htx*htx+hty*hty);
    
    //Creating Bools
    bool BoolTrig=false, Bool0=false, Bool1=false, Bool2=false, Bool3=false, Bool4=false, Bool5=false;
    
    //monojet trigger: MET>110 GeV and MHT>110 GeV

    if ((metptr->MET>=110)&&(mht>=110)){
      BoolTrig=true;
      countTrig++;
      //std::cout<<entry<<std::endl;
      //std::cout<< <<std::endl;

    }
    
    //Cut0="(Muon_size==1)"
    if (Muon_size==1){ 
      Bool0=true;
      count0++;
    }

    //Cut1="(TMath::Sqrt(2*Muon[0].PT*MissingET.MET*(1-TMath::Cos(deltaPhi(MissingET.Phi,Muon[0].Phi))))<40.)"
    if(Muon_size>0){
      if (TMath::Sqrt(2*leadingmuonptr->PT*metptr->MET*(1-TMath::Cos(deltaPhi(metptr->Phi,leadingmuonptr->Phi))))<40.0){ 
	Bool1=true;
	count1++;
      }
    }

    //Cut2="(0<Jet_size<4.)"
    if ((Jet_size>0)&&(Jet_size<4)){ 
      Bool2=true;
      count2++;
    }


    //Cut3="(Jet[0].PT>20.)"
    if (Jet_size>0){ 
      if (leadingJetptr->PT>20.0){ 
	Bool3=true;
	count3++;
      }
    }
    

    //Cut4="(Muon[0].PT<20.)"

    if(Muon_size>0){
      if (leadingmuonptr->PT<20.0){ 
	Bool4=true;
	count4++;
      }
    }

    if (BoolTrig&&Bool0&&Bool1&&Bool2&&Bool3&&Bool4){
      //std::cout<<"True"<<std::endl;
      countTotal++;
    }


    
  }
  std::cout<<"muestra:    "<<argv<<std::endl;
  //  std::cout<<"NoCuts=     "<<numberOfEntries<<std::endl;
  //  std::cout<<"count0=     "<<count0<<std::endl;
  //  std::cout<<"count1=     "<<count1<<std::endl;
  //  std::cout<<"count2=     "<<count2<<std::endl;
  //  std::cout<<"count3=     "<<count3<<std::endl;
  //  std::cout<<"count4=     "<<count4<<std::endl;
  //  std::cout<<"countTrig = "<<countTrig<<std::endl;
  std::cout<<"countTotal= "<<countTotal<<std::endl;

}

