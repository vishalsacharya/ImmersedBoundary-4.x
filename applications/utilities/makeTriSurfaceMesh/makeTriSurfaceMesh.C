/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Application
    makeTriSurfaceMesh

Description
    Generate a triSurface mesh from fvMesh patches

Author
    Zeljko Tukovic

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "triSurface.H"
#include "triSurfaceTools.H"
#include "wallPolyPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    labelHashSet includedPatches;

    forAll (mesh.boundaryMesh(), patchI)
    {
        if(mesh.boundaryMesh()[patchI].type() == wallPolyPatch::typeName)
            includedPatches.insert(patchI);
    }

    triSurface triSurf
    (
        triSurfaceTools::triangulate
        (
            mesh.boundaryMesh(),
            includedPatches
        )
    );


    // Writing triSurface mesh
    Info << "Write triSurface mesh ... ";

    triSurf.write(runTime.caseName() + ".stl");

    Info << "done" << endl;

    return 0;
}


// ************************************************************************* //
