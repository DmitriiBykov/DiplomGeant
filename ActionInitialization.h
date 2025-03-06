#ifndef ActionInitialization_H
#define ActionInitialization_H

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization();
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const override;
    virtual void Build() const override;
};

#endif
