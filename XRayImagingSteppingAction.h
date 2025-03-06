#ifndef XRAYIMAGINGSTEPPINGACTION_H
#define XRAYIMAGINGSTEPPINGACTION_H

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class XRayImagingSteppingAction : public G4UserSteppingAction
{
public:
    XRayImagingSteppingAction();
    virtual ~XRayImagingSteppingAction();

    virtual void UserSteppingAction(const G4Step*) override;
};

#endif
