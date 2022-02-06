#include "test/PhysicsSim.h"
#include "ctre/Phoenix.h"

//taken from ctre

PhysicsSim::TalonSRXSimProfile::TalonSRXSimProfile(TalonSRX &talon, double const accelToFullTime, double const fullVel, bool const sensorPhase)
             : SimProfile(), _talon(talon), _accelToFullTime(accelToFullTime), _fullVel(fullVel), _sensorPhase(sensorPhase) {}

void PhysicsSim::TalonSRXSimProfile::Run() {
    double const period = GetPeriod();
    double const accelAmount = _fullVel / _accelToFullTime * period / 1000;

    double outPerc = _talon.GetMotorOutputPercent();
    std::cout << "output: " << outPerc << "\n";
    if (_sensorPhase) {
        outPerc *= -1;
    }

    // Calculate theoretical velocity with some randomness
    double theoreticalVel = outPerc * _fullVel * random(0.95, 1);
    // Simulate motor load
    if (theoreticalVel > _vel + accelAmount) _vel += accelAmount;
    else if (theoreticalVel < _vel - accelAmount)  _vel -= accelAmount;
    else _vel += 0.9 * (theoreticalVel - _vel);

    _pos += _vel * period / 100;

    /// SET SIM PHYSICS INPUTS

    std::cout << "setting physics inputs\n";

    _talon.GetSimCollection().AddQuadraturePosition(_vel * period / 100);
    _talon.GetSimCollection().SetQuadratureVelocity(_vel);

    double supplyCurrent = fabs(outPerc) * 30 * random(0.95, 1.05);
    double statorCurrent = outPerc == 0 ? 0 : supplyCurrent / fabs(outPerc);
    _talon.GetSimCollection().SetSupplyCurrent(supplyCurrent);
    _talon.GetSimCollection().SetStatorCurrent(statorCurrent);

    _talon.GetSimCollection().SetBusVoltage(12 - outPerc * outPerc * 3/4 * random(0.95, 1.05));
}