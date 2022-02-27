#include <iostream>
#include <vector>
#include <frc/smartdashboard/SmartDashboard.h>
// #include "Intake.h"
// #include "Shooter.h"
// #include "SwerveDrive.h"

class AutoMode{
    public:
        AutoMode();

        enum State{
            SHOOT,
            INTAKE,
            DRIVE,
            IDLE,
            DRIVEnINTAKE
        };

        std::vector<State> actions = {DRIVEnINTAKE, IDLE, SHOOT, IDLE, DRIVE, IDLE, DRIVEnINTAKE, IDLE,
            SHOOT, IDLE};
        std::vector<double> times = {2.3, 2.4, 5.4, 5.5, 6.0, 6.1, 7.5, 7.6, 8.9, 9.0};
        std::vector<int> waypointIndex = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3};

        //Two ball Auto
        // std::vector<State> actions = {DRIVEnINTAKE, IDLE, SHOOT, IDLE};
        // std::vector<double> times = {2.3, 2.4, 5.4, 5.5};
        // std::vector<int> waypointIndex = {0, 0, 0, 0};

        void ResetAuto();
        void Periodic(double time);
        State getState();
        int getWaypointIndex();

    private:
        State state;
        size_t index;
};