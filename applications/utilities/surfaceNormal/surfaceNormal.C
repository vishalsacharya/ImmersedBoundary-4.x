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

Description
    Write surface and normal vector

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "triSurface.H"
#include "IFstream.H"
#include "OFstream.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{
    argList::noParallel();
    argList::validArgs.clear();

    argList::validArgs.append("surface file");
    argList args(argc, argv);

    fileName surfFileName(args.additionalArgs()[0]);

    Info<< "Reading surf from " << surfFileName << " ..." << endl;

    triSurface ts(surfFileName);

    fileName normalFileName(surfFileName.lessExt() + "_Normals"+".vtk");

    Info<< "Writing normals to file " << normalFileName << endl;
    ts.write(normalFileName);

    Info << "End\n" << endl;

    return 0;
}


// ************************************************************************* //
