//
//  integrator.cpp
//

#include <iostream>
#include <algorithm>

#include "integrator.h"
#include "ion.h"
#include "ion_trap.h"
#include "ccmd_sim.h"

Integrator::Integrator(Ion_trap& it, Ion_cloud& ic, 
                       const Integration_params& params)
    : trap(&it), ions(&ic), f_coulomb(ic) 
{
    // get Coulomb forces on construction
    f_coulomb.update();
}

Integrator::~Integrator() {}

