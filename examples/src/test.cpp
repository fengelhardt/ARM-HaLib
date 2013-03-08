#include <platform.h>

#include <stdint.h>
#include <arm-halib/core/delay.h>

using arm_halib::delay_ms;

static int h=0, s=1000, v=1000;
static int r=0, g=0, b=0;

void hsv2rgb() {
	int hi = h*6/1000;
	int f = h*6 - hi*1000;
	int p = v*(1000-s)/1000;
	int q = v*(1000-s*f/1000)/1000;
	int t = v*(1000-s*(1000-f)/1000)/1000;
#define MAP(x,y,z) r=(int)(x);g=(int)(y);b=(int)(z);
	switch(hi) {
		case 0: MAP(v,t,p); break;
		case 1: MAP(q,v,p); break;
		case 2: MAP(p,v,t); break;
		case 3: MAP(p,q,v); break;
		case 4: MAP(t,p,v); break;
		case 5: MAP(v,p,q); break;
		case 6: MAP(v,t,p); break;
	}
#undef MAP
}

int main() 
{
    log::emit() << "PWM Test" <<  log::endl;

    platform::RedLed red;
    platform::BlueLed blue;
    platform::GreenLed green;

    platform::SW1 sw1;
    platform::SW2 sw2;

    sw1.pullup(true);
    sw2.pullup(true);

    log::emit() << " SW2:" << (sw2.pullup() ? "true" : "false") << log::endl;

    red.dutyCycle(1100);
    blue.dutyCycle(1100);
    green.dutyCycle(1100);

    red.value(r);
    blue.value(b);
    green.value(g);

    red.enable();
    blue.enable();
    green.enable();

    while(true)
    {   
    	h += 10;
    	if(h>1000) h=0;

    	hsv2rgb();

        red.value(r);
        blue.value(g);
        green.value(b);

        log::emit() << " r" << red.value();
        log::emit() << " g" << green.value();
        log::emit() << " b" << blue.value();
        log::emit() << " SW1:" << (sw1.value() ? "true" : "false");
        log::emit() << " SW2:" << (sw2.value() ? "true" : "false") << "                       \r";

        delay_ms(50);
    }
    return 0;
}

