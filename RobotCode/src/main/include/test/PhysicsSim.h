#pragma once

#include <vector>
#include "ctre/Phoenix.h"

class PhysicsSim
{
    class SimProfile;
    class TalonSRXSimProfile;
    
public:
    static PhysicsSim& GetInstance();
    ~PhysicsSim();
    void AddTalonSRX(TalonSRX &talon, double const accelToFullTime, double const fullVel, bool const sensorPhase = false);
    void Run();

private:
    std::vector<SimProfile*> _simProfiles; //list of sim profiles (like simulated talonsrx)

    /* scales a random domain of [0, 2pi] to [min, max] while prioritizing the peaks */
    static double random(double min, double max);
    static double random(double max);

    class SimProfile {
    private:
        std::chrono::steady_clock::time_point _lastTime;
        bool _running = false;

    public:
        virtual void Run() {}
        virtual ~SimProfile() {}

    protected:
        double GetPeriod();
    };


    class TalonSRXSimProfile : public SimProfile {

    private:
        TalonSRX &_talon;
        double const _accelToFullTime = 0;
        double const _fullVel = 0;
        bool const _sensorPhase = 0;

        double _pos = 0;
        double _vel = 0;

     public:

        TalonSRXSimProfile(TalonSRX &talon, double const accelToFullTime, double const fullVel, bool const sensorPhase);


        ~TalonSRXSimProfile() override {}

         void Run() override;
    };
};