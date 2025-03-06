#include "EventAction.h"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"

EventAction::EventAction()
    : G4UserEventAction()
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
    // Можно что-то обнулять/готовить в начале события
}

void EventAction::EndOfEventAction(const G4Event*)
{
    // Можно что-то делать в конце события
}
