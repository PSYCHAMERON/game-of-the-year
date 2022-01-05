#pragma once
// this class is so 'cold' that it freezes things
class delayClass {
    clock_t startTime, curTime;
public:
    delayClass() { startTime = clock(); }
    void resetTimer() {  startTime = clock(); } // function to reset the timer
    bool delay(double delayTime)
    {

        curTime = clock() ;
        if((double)(curTime - startTime)/CLOCKS_PER_SEC>=delayTime)
        {
            return true;
        } // return true if time is up

        return false; // return false if time to spare
    }

    void freeze(double t)
    {
        while(!delay(t)) {

        }
    }
};
/////////////////////////end of delayClass//////////////////////////////////
