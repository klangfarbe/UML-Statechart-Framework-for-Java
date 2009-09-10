#include <iostream>

#include <statechart/statechart.h>
#include <statechart/util/smartpointer.h>


using namespace statechart;

class PlayDialToneAction : public Action 
{
  void execute(util::SmartPointer<Metadata>& data, 
	       util::SmartPointer<Parameter>& param) {
    cout << "ACTION: Dial Tone" <<endl;
  }
};

class PlayDialAction : public Action 
{
  void execute(util::SmartPointer<Metadata>& data, 
	       util::SmartPointer<Parameter>& param) {
    cout << "ACTION: BEEP" <<endl;
  }
};

class PlayMessageAction : public Action 
{
  void execute(util::SmartPointer<Metadata>& data, 
	       util::SmartPointer<Parameter>& param) {
    cout << "ACTION: The number you are trying to reach is invalid" <<endl;
  }
};


class PlayBusyToneAction : public Action 
{
  void execute(util::SmartPointer<Metadata>& data, 
	       util::SmartPointer<Parameter>& param) {
    cout << "ACTION: Busy Tone" <<endl;
  }
};

class ConnectAction : public Action 
{
  void execute(util::SmartPointer<Metadata>& data, 
	       util::SmartPointer<Parameter>& param) {
    cout << "ACTION: Connect" <<endl;
  }
};

int main() {
  Statechart* chart = new Statechart();
  util::SmartPointer<Metadata> data(new Metadata);

  State* dial_tone    = new State(chart, new PlayDialToneAction, NULL, NULL, "STATE: dial_tone");
  State* dialing      = new State(chart, NULL, NULL, NULL, "STATE: dialing");
  State* invalid      = new State(chart, new PlayMessageAction, NULL, NULL, "STATE: invalid");
  State* connected    = new State(chart, NULL, NULL, NULL, "STATE: connected");
  State* busy         = new State(chart, new PlayBusyToneAction, NULL, NULL, "STATE: busy");


  State* p1 = new PseudoState(chart, pseudostate_start);
  State* p2 = new FinalState(chart);

  util::SmartPointer<Event> event1(new Event("EVENT: liftHandset"));
  util::SmartPointer<Event> event2(new Event("EVENT: replaceHandset"));
  util::SmartPointer<Event> event3(new Event("EVENT: dial"));
  util::SmartPointer<Event> event4(new Event("EVENT: busy"));
  util::SmartPointer<Event> event5(new Event("EVENT: invalid"));
  util::SmartPointer<Event> event6(new Event("EVENT: connected"));

  new Transition(p1,           dial_tone,    &*event1);
  new Transition(dial_tone,    dialing,      &*event3, new PlayDialAction);
  new Transition(dialing,      dialing,      &*event3, new PlayDialAction);
  new Transition(dialing,      invalid,      &*event5, new PlayMessageAction);
  new Transition(dialing,      connected,    &*event6, new ConnectAction);
  new Transition(dialing,      busy,         &*event4, new PlayBusyToneAction);

  new Transition(dial_tone,    p2,           &*event2);
  new Transition(dialing,      p2,           &*event2);
  new Transition(connected,    p2,           &*event2);
  new Transition(busy,         p2,           &*event2);
  new Transition(invalid,      p2,           &*event2);

  chart->start(data);
  chart->dispatch(data, event1);
  chart->dispatch(data, event3);
  chart->dispatch(data, event5);
  chart->dispatch(data, event2);

  return 1;
}
