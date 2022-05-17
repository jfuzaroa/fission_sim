/////////////////////////////////////////////////////////////////
// A NiffteEvent holds the track and digit data for events
// to be used in correlation analyses that are not efficient
// with the flat TTree structure.  NiffteEvents consist of
// a TClonesArray of Digit objects.
// A TClonesArray  of Track objects.
// A TClonesArray of MC Truth objects (if simulation)
// A TClonesArray of Vertex objects (if reconstructed)
//
// Author: J.L. Klay (Cal Poly)
// Date: 15-Sep-2018
//
// Revision Date: 14-Aug-2020 (JKlay)
// Notes: The TClonesArray of digits is now filled for all digits
// assigned to tracks.  The TRefArray of digits associated to a track is now
// populated.  Examples for reading the tree are provided in
// $NIFFTE/sandbox/jklay/example/readNETree.C
// $NIFFTE/sandbox/jklay/example/analysisExample.C
//
// Revision Date: 21-Aug-2020 (JKlay)
// Notes: Added flags to allow user to choose whether to write
// ALL digits, digits associated to tracks, or NO digits.
// If ALL digits, the TRefArray of digits in the tracks will be
// empty and one has to access the digits directly from the tree
//
// This tree is written by the module $NIFFTE/tpc/output/TPCStructuredTree.*
/////////////////////////////////////////////////////////////////

#include "NiffteEvent.h"

ClassImp(NEVertex)
ClassImp(NETrack)
ClassImp(NEMCTruth)
ClassImp(NEDigit)
ClassImp(NiffteEvent)

TClonesArray *NiffteEvent::fgVertices = 0;
TClonesArray *NiffteEvent::fgTracks   = 0;
TClonesArray *NiffteEvent::fgMCTruth   = 0;
TClonesArray *NiffteEvent::fgDigits   = 0;

////////////////////////////////////////////////////////////////////////////////
/// Create a NiffteEvent object.
/// When the constructor is invoked for the first time, the class static
/// variables fgxxx are 0 and the TClonesArray fgxxx are created.

NiffteEvent::NiffteEvent()
{
   if (!fgTracks) fgTracks = new TClonesArray("NETrack", 100);
   if (!fgMCTruth) fgMCTruth = new TClonesArray("NEMCTruth", 100);
   if (!fgDigits) fgDigits = new TClonesArray("NEDigit", 1000);
   if (!fgVertices)   fgVertices   = new TClonesArray("NEVertex", 10);
   fVertices   = fgVertices;
   fTracks = fgTracks;
   fMCTruth = fgMCTruth;
   fDigits = fgDigits;
}

////////////////////////////////////////////////////////////////////////////////

NiffteEvent::~NiffteEvent()
{
   Reset();
}

////////////////////////////////////////////////////////////////////////////////
NEVertex::~NEVertex()
{
}
NETrack::~NETrack()
{
}
NEMCTruth::~NEMCTruth()
{
}
NEDigit::~NEDigit()
{
}


////////////////////////////////////////////////////////////////////////////////
/// Add a new Vertex to the list of vertices for this event.

NEVertex *NiffteEvent::AddVertex()
{
   return new((*fVertices)[fNVertices++]) NEVertex();
}


////////////////////////////////////////////////////////////////////////////////
/// Add a new track to the list of tracks for this event.

NETrack *NiffteEvent::AddTrack()
{
   return new((*fTracks)[fNTracks++]) NETrack();
}

////////////////////////////////////////////////////////////////////////////////
/// Add a new MC truth track to the list of MCTruth for this event.

NEMCTruth *NiffteEvent::AddMCTruth()
{
   return new((*fMCTruth)[fNMCTruth++]) NEMCTruth();
}

////////////////////////////////////////////////////////////////////////////////
/// Add a new Digit to the list of digits for this event.

NEDigit *NiffteEvent::AddDigit()
{
   return new((*fDigits)[fNDigits++]) NEDigit();
}

////////////////////////////////////////////////////////////////////////////////

void NiffteEvent::Clear(Option_t *option)
{
   fVertices->Clear(option);
   fTracks->Clear(option);
   fMCTruth->Clear(option);
   fDigits->Clear(option);
   
}

////////////////////////////////////////////////////////////////////////////////
/// Static function to reset all static objects for this event

void NiffteEvent::Reset(Option_t *)
{
   delete fgVertices; fgVertices = 0;
   delete fgTracks;   fgTracks = 0;
   delete fgMCTruth;   fgMCTruth = 0;
   delete fgDigits;   fgDigits = 0;
   
}
