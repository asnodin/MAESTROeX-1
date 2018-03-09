
#include <Maestro.H>

using namespace amrex;

// compute heating term, rho_Hext
void
Maestro::MakeHeating (Vector<MultiFab>& rho_Hext,
                      const Vector<MultiFab>& scal) 
{
    // timer for profiling
    BL_PROFILE_VAR("Maestro::MakeHeating()",MakeHeating);

    for (int lev=0; lev<=finest_level; ++lev) {

        // get references to the MultiFabs at level lev
              MultiFab& rho_Hext_mf = rho_Hext[lev];
        const MultiFab&     scal_mf =     scal[lev];


        // loop over boxes (make sure mfi takes a cell-centered multifab as an argument)
        for ( MFIter mfi(scal_mf); mfi.isValid(); ++mfi ) {

            // Get the index space of the valid region
            const Box& validBox = mfi.validbox();

            // call fortran subroutine
            // use macros in AMReX_ArrayLim.H to pass in each FAB's data, 
            // lo/hi coordinates (including ghost cells), and/or the # of components
            // We will also pass "validBox", which specifies the "valid" region.
            make_heating(ARLIM_3D(validBox.loVect()), ARLIM_3D(validBox.hiVect()),
                         BL_TO_FORTRAN_3D(rho_Hext_mf[mfi]),
                         BL_TO_FORTRAN_FAB(scal_mf[mfi]), &t_old);
        }
    }
}
