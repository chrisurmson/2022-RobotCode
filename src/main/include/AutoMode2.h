#include "Intake.h"
#include "Shooter.h"
#include "SwerveDrive.h"
#include "Traj2.h"

class AutoMode2 {
   public:
    AutoMode2(SwerveDrive* swerve, Shooter* shooter, Intake* intake) {
        m_swerve = swerve;
        m_shooter = shooter;
        m_intake = intake;
        m_autostate = -1;
    }

    virtual void tick(double time) = 0;
    void start(double time) { m_autostate = 0; };

   protected:
    // utilities for waiting
    virtual void beginWait(double time) { m_wait_start = time; }

    virtual bool hasElapsed(double timeout, double time) {
        return time - m_wait_start > timeout;
    }
    // Non-owning pointers to robot subsystems to run the mode
    SwerveDrive* m_swerve;
    Shooter* m_shooter;
    Intake* m_intake;
    int m_autostate{0};

    double m_wait_start { 0.0 }
}

class TwoBallAuto : public AutoMode2 {
    TwoBallAuto(SwerveDrive* swerve, Shooter* shooter, Intake* intake)
        : AutoMode2(swerve, shooter, intake) {
        // Each axis gets a list of (pos, vel, time) tuples to define its
        // trajectory
        // (dunno if any of these values are right, but hopefully you get the
        // idea)
        auto seg1x = Traj1D({
            std::make_tuple(0.0, 0.0, 0.0),
            std::make_tuple(-0.5, 0.0, 1.5),
            std::make_tuple(-1.0, 0.0, 3.0),
            // hold the end, the traj runs for the longest of the three:
            std::make_tuple(-1.0, 0.0, 3.5),
        });
        auto seg1y = Traj1D({
            std::make_tuple(0.0, 0.0, 0.0),
            std::make_tuple(-0.5, -0.3, 1.5),
            std::make_tuple(-1.0, 0.0, 3.0),
        });
        auto seg1t = Traj1D({
            std::make_tuple(0.0, 0.0, 0.0),
            std::make_tuple(M_PI, 0.0, 1.0),
        });
        segment1 = std::make_unique<RobotTraj>(
            std::move{seg1x}, std::move{seg1y}, std::move{seg1theta});
    }

    virtual void tick(double time) override {
        switch (m_autostate) {
            case 0:
                m_intake.setState(Intake::IDLE);
                m_shooter.setState(Shooter::IDLE);
                m_swerve.Drive(0, 0, 0, 0, true);
                beginWait(time);
                m_autostate++;
                break;
            case 1:
                if (hasElapsed(0.5, time)) m_autostate++;
                break;
            case 2:
                m_shooter->setState(Shooter::SHOOT);
                beginWait(time);
                m_autostate++;
                break;
            case 3:
                if (hasElapsed(0.5, time)) {
                    m_shooter.setState(Shooter::IDLE);
                    m_autostate++
                };
                break;
            case 4:
                // Intake and path follow
                m_intake->setState(Intake::RUN);
                m_shooter->setState(Shooter::LOAD);
                segment1->start(time);
                m_autostate++;
                break;
            case 5:
                if (segment1->is_done(time)) {
                    m_autostate++;
                }
                segment1->drive(time, m_swerve);
                break;
            case 6:
                m_swerve.Drive(0, 0, 0, 0, true);
                m_shooter->setState(Shooter::SHOOT);
                beginWait(time);
                m_autostate++;
                break;
            case 7:
                if (hasElapsed(0.5, time)) m_autostate++;
                break;
            case 8:
                m_shooter->setState(Shooter::IDLE);
                break;
        }
    }

    std::unique_ptr<RobotTraj> segment1;
}