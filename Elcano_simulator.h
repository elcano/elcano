extern const int RxD;
extern const int TxD;
extern const int LED;


#ifdef SIMULATOR
// TODO: Make these consistent with Arduino library
const int INPUT = 0;
const int OUTPUT = 1;
const int LOW = 0;
typedef  unsigned char byte;
void pinMode(int a1, int a2);
void digitalWrite(int a1, int a2);

void C3_Pilot_setup();
void C3_Pilot_loop();
void C4_Planner_setup();
void C4_Planner_loop();
void C6_Navigator_setup();
void C6_Navigator_loop();
void C7_Vision_setup();
void C7_Vision_loop();
#endif // SIMULATOR

namespace C3_Pilot {
    void setup();
    void loop();
}
namespace C4_Planner {
    void setup();
    void loop();
}
namespace C6_Navigator {
    void setup();
    void loop();
}
namespace C7_Vision {
    void setup();
    void loop();
}
