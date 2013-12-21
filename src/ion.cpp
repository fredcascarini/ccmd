//
//  ion.cpp
//

#include "ion.h"
#include "ccmd_sim.h"
#include "IonHistogram.h"
#include "Stats.h"

#include <stdexcept>

/**
 *  @class Ion
 *  @brief Base class for all ion types.
 *  The ion base class stores position and velocity, and provides functions to 
 *  update the position due to free-flight, and acceleration due to a force.
 *  This class also stores ion information: mass, charge, name and formula.
 *
 *  The position and velocity vectors are hidden as private variables; they
 *  can be read-out, but only modified correctly by a force or free flight.
 */

Ion::Ion(const Ion_type& type)
: ion_type(&type)
{
    mass = ion_type->mass;
    charge = ion_type->charge;
}

/**
 *  @brief Update position after free flight.
 *  Moves the ion at a constant velocity for a time `dt`.
 *
 *  @param dt Time step.
 */
void Ion::drift(const double dt)
{
    pos += vel*dt;
}

/**
 *  @brief Calculate the acceleration due to the force, and update the velocity.
 *  @param dt   Time step.
 *  @param f    Force vector.
 */
inline void Ion::kick(const double dt, const Vector3D& f)
{
    double time_over_mass = dt/mass;
    vel += f*time_over_mass;
}

/**
 *  @brief Add the kinetic energy of this ion to a histogram.
 *  Calculate the total kinetic energy and kinetic energy directed along each
 *  axis, then give this to an `IonHistogram` object, with the name of this ion
 *  as a label.
 *
 *  @param ionHistogram A reference to the histogram object to update.
 */
void Ion::recordKE(IonHistogram& ionHistogram) const
{
    double energy;
    double mon2 = 0.5 * ion_type->mass;
    //total
    energy = mon2 * vel.norm_sq();
    ionHistogram.addIon(name() + "_total", energy);
    //x - directed
    energy = mon2 * vel[0] * vel[0];
    ionHistogram.addIon(name() + "_x", energy);
    //y - directed
    energy = mon2 * vel[1] * vel[1];
    ionHistogram.addIon(name() + "_y", energy);
    //z - directed
    energy = mon2 * vel[2] * vel[2];
    ionHistogram.addIon(name() + "_z", energy);
}

/** 
 *  @brief Update the statistics stored by this ion.
 *  Appends the current kinetic energy (velocity^2, so this needs to be 
 *  multiplied by mass/2 before output) and position to the `Stats` objects.
 */
void Ion::updateStats()
{
    posStats.append(pos);
    velStats.append(vel*vel);
}

