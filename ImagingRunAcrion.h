#ifndef IMAGINGRUNACTION_HH
#define IMAGINGRUNACTION_HH

#include "G4UserRunAction.hh"

class XRayImagingRunAction : public G4UserRunAction
{
public:
    XRayImagingRunAction();
    virtual ~XRayImagingRunAction();

    virtual G4Run* GenerateRun() override;
    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run*) override;
};

#endif
