#include "jimlib.h"
#include "serialLog.h"
#ifndef CSIM
#include "Adafruit_HX711.h"
#endif
Adafruit_HX711 hx(23,22);


JStuff j;
CLI_VARIABLE_FLOAT(x, 1);

void setup() {
    hx.begin();
    j.begin();
    j.cli.on("RESET", [](){ ESP.restart(); });
}

void loop() {
    j.run();
    pinMode(19, OUTPUT);
    digitalWrite(19, 1);
    delay(1);
    hx.powerDown(true);
    delay(1);
    hx.powerDown(false); 
    delay(100); 
    float sum = 0.0;
    int reads = 20;
    for (int i = 0; i < reads; i++) 
         sum += hx.readChannelRaw();
    sum /= reads;
    digitalWrite(19, 0);
    OUT("%.2f", sum);
    delay(1);
}

#ifdef CSIM
class SketchCsim : public Csim_Module {
    public:
    void setup() { HTTPClient::csim_onPOST("http://.*/log", 
        [](const char *url, const char *hdr, const char *data, string &result) {
 	return 200; }); }
    string dummy;
    void parseArg(char **&a, char **la) override { if (strcmp(*a, "--dummy") == 0) dummy = *(++a); }
    void loop() override {}
} sketchCsim;
#endif
 
