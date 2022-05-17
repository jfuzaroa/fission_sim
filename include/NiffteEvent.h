#ifndef NIFFTE_NiffteEvent
#define NIFFTE_NiffteEvent

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// NiffteEvent                                                          //
//                                                                      //
// Description of the event and track parameters                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TClonesArray.h"
#include "TRefArray.h"
#include "TVector3.h"
#include "TObject.h"

class NETrack : public TObject {

public:
  Short_t    fTrackNum;    //Index assigned to the track during track-finding
  Double_t   fADC;         //Magnitude of energy associated with track in ADC
  Double_t   fEnergy;      //Energy of track in MeV calculated from ADC
  Short_t    fNDig;        //Number of digits associated with this track
  Short_t    fNSig;        //Number of waveforms associated with this track
  Short_t    fTrkType;     //Assigned by tracker based on how track was found
  Short_t    fVolume;      //Which volume was the track found in (from digits)
  Double_t   fStartTime;   //Earliest time of digits associated with track
  Double_t   fCathodeTime; //Cathode time associated with track
  Double_t   fAnodeTime;   //Anode time associated with track
  Double_t   fnToFAnode;   //nToF from this track's anode timing

  TVector3   fStart;       //Location of start vertex for track
  TVector3   fEnd;         //Location of end point for track
  TVector3   fDirection;   //Direction vector for track
  Double_t   fLength;      //Length of track
  Double_t   fAvgR2;       //Average Radius^2 of charge cloud for track
  Short_t    fFitType;     //Assigned by fitter based on how track was fit
  Short_t    fFitNo;       //Assigned by fitter
  Short_t    fBestFit;     //Flag to indicate whether this was the
			   //best fit - obsolete?
  Float_t    fFitQuality;  //Weight assigned to assess quality of fit
  Bool_t     fReversed;    //Boolean to indicate whether track was reversed
  Bool_t     fHoughTrackInEvent;  //Was Hough Transform finder needed?
  Short_t    fReverseReason;  //Flag for why track was reversed

  Double_t   fBraggPosition;  //Location of Bragg Peak, in relative coords.
  Double_t   fBraggValue;     //Value in ADC of Bragg peak amplitude

  //pid
  Bool_t fFragFlag;        //PID Flag for Fission Fragment
  Bool_t fContProtonFlag;  //PID Flag for Contained Proton
  Bool_t fUncontProtonFlag;  //PID Flag for Uncontained Proton
  Bool_t fAlphaFlag;       //PID Flag for alpha

  TRefArray fDigitList;   //List of digits connected to the track

  std::vector<std::pair<int, double>>  fSimInfo;  //MC Truth info

public:
  NETrack() { }
  virtual ~NETrack();
  Int_t       GetNRefDigits() const { return fDigitList.GetEntries(); }
  TRefArray   &GetDigits() {return fDigitList; }

  void AddSimInfo(int id, double wgt) { fSimInfo.push_back(std::make_pair(id,wgt));}
  void ClearSimInfo() { fSimInfo.clear(); }
  int IsSimulatedtrack() const { return fSimInfo.size(); }
  const std::vector<std::pair<int, double>>& GetSimInfo() const { return fSimInfo; }
  const std::pair<int, double> GetSimInfo(int i) const { return fSimInfo.at(i);}

  ClassDefOverride(NETrack,7)  //A track segment
};


class NEMCTruth : public TObject {

public:
  Int_t      fMCTrackId;  //ID assigned by simulation
  Short_t    fZ;          //Z value of MC particle
  Short_t    fA;          //A value of MC particle

  Double_t   fKineticE;   //KE of MC Particle
  Double_t   fDepositedE; //Deposited energy for MC Particle
  Double_t   fMass;       //Mass of MC Particle

  TVector3   fStart;      //Location of start vertex of MC Particle
  Double_t   fStartTime;  //Start time of MC Particle
  TVector3   fEnd;        //Location of end point of MC Particle track
  Double_t   fLength;     //Length of MC Particle track

  TVector3   fMomentum;   //Momentum at creation point of MC Particle
  TVector3   fDirection;  //Direction vector of MC Particle track

  Double_t   fNeutronE;   //energy of MC neutron initiating fission

public:
  NEMCTruth() { }
  virtual ~NEMCTruth();

  ClassDefOverride(NEMCTruth,2)  //An MC Truth particle
};

class NEDigit : public TObject {

public:
  Int_t     fVolume;     //Volume
  Int_t     fRow;        //Padplane row
  Int_t     fColumn;     //Padplane column
  Int_t     fBucket;     //Time bucket
  TVector3  fLocation;   //3D location of digit in TPC CS
  Double_t  fADC;        //Total ADC of digit
  Short_t   fTrackId;    //track id of track that uses this digit;
                         //-1 means it was unused
  std::vector<std::pair<int, double>>  fSimInfo;  //MC Truth info

public:
  NEDigit() { }
  virtual ~NEDigit();

  void AddSimInfo(int id, double wgt) { fSimInfo.push_back(std::make_pair(id,wgt));}
  void ClearSimInfo() { fSimInfo.clear(); }
  const std::vector<std::pair<int, double>>& GetSimInfo() const { return fSimInfo; }
  const std::pair<int, double> GetSimInfo(int i) const { return fSimInfo.at(i);}

  ClassDefOverride(NEDigit,3)  //A TPC voxel or digit
};

class NEVertex : public TObject {

public:
  TVector3  fAverageVertex;        //Location of average vertex
  TVector3  fMinimizedVertex;      //Minimized vertex location
  TVector3  fMinimizedVertexVol0;  //Minimized vertex location in Vol 0
  TVector3  fMinimizedVertexVol1;  //Minimized vertex location in Vol 1

  Double_t  fOffsetVol0;           //Offset from target in volume 0
  Double_t  fOffsetVol1;           //Offset from target in volume 1
  Bool_t    fValidMinimizedVertex; //Flag for assessing minimized vertex

  TRefArray fTrackList;   //List of tracks connected to the vertex

public:
  NEVertex() { }
  virtual ~NEVertex();
  Int_t       GetNRefTracks() const { return fTrackList.GetEntries(); }
  TRefArray   &GetTracks() {return fTrackList; }

  ClassDefOverride(NEVertex,2)  //Vertex class
};

class NiffteEvent : public TObject {

private:
  //Note that the special arrow after the comment slashes below
  //is a preprocessor directive to ROOT that tells the ROOT streamer
  //to store these arrays in the TFile
  TClonesArray  *fVertices;       //->array with all vertices
  TClonesArray  *fTracks;         //->array with all tracks
  TClonesArray  *fMCTruth;        //->array with all mc truth
  TClonesArray  *fDigits;         //->array with all digits

  //These are global static variables to reserve space for the
  //actual streamed arrays.  They are NOT stored in the TFile because
  //they do not have the special magic ROOT pre-processor arrow.
  static TClonesArray *fgVertices; //global static vertex array
  static TClonesArray *fgTracks;   //global static track array
  static TClonesArray *fgMCTruth;  //global static MCtruth array
  static TClonesArray *fgDigits;   //global static digit array

public:
  NiffteEvent();
  virtual ~NiffteEvent();
  void          Clear(Option_t *option ="") override;
  void          Reset(Option_t *option ="");
  Int_t         GetNVertices()   const { return fNVertices; }
  Int_t         GetNTracks() const { return fNTracks; }
  Int_t         GetNMCTruth() const { return fNMCTruth; }
  Int_t         GetNDigits() const { return fNDigits; }
  NEVertex     *AddVertex();
  NETrack      *AddTrack();
  NEMCTruth    *AddMCTruth();
  NEDigit      *AddDigit();
  TClonesArray *GetVertices() const { return fVertices; }
  TClonesArray *GetTracks() const { return fTracks; }
  TClonesArray *GetMCTruth() const { return fMCTruth; }
  TClonesArray *GetDigits() const { return fDigits; }

  Int_t fNVertices;         //Number of vertices in the event
  Int_t fNTracks;           //Number of tracks in the event
  Int_t fNMCTruth;          //Number of mc particles in the event
  Int_t fNDigits;           //Number of digits in the event

  Int_t fEventID;          //Number assigned from run file
  Int_t fRecoDigitCount;   //Total number of digits in the raw event data
  Int_t fRecoTrackCount;   //Total number of tracks found for the
			   //event, will generally be bigger than the
			   //size of the tracks array for the event
  Int_t fNUnusedDigits;    //number of unused digits in this event

  Short_t  fNSignals;      //Event variable - number of waveforms in event
  Double_t fDigitCharge;   //Event variable - total ADC of all digits
  Double_t fVol0DigitCharge;   //Event variable - total ADC of all Volume 0 digits
  Double_t fVol1DigitCharge;   //Event variable - total ADC of all Volume 1 digits
  Double_t fTimeOfFlight;  //Event variable - neutron time of flight from cathode signal
  Double_t fnToFAnode;     //Event variable - neutron time of flight from anode track start
  Double_t fDeltaT;        //Event variable - delta T
  Int_t    fMicropulseNumber; //Event variable - micropulse # from the beam
  Double_t fCathodeEnergy; //Event variable - total energy of cathode signal
  Double_t fADCCutOff;     //Event Variable - ADC cut applied to event
  Double_t fVol0ADCMin;     //Event Variable - ADC cut applied to volume 0
  Double_t fVol1ADCMin;     //Event Variable - ADC cut applied to volume 1

  Int_t fVol0BktOffset;     //Event variables - filled when TPCAlignZ
			    //shifts data to the target plane
  Int_t fVol1BktOffset;     //Event variable - volume 1 bucket offset
  Double_t fVol0ZOffset;  //Event variable - volume 0 z offset
  Double_t fVol1ZOffset;  //Event variable - volume 1 z offset

  ClassDefOverride(NiffteEvent,7)  //Event structure
};

#endif /* end of include guard for NiffteEvent.h */
