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
    refineImmersedBoundaryMesh

Description


\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "refineImmersedBoundaryMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#   include "setRootCase.H"

#   include "createTime.H"
#   include "createMesh.H"

    // 1
    {
        labelHashSet refCellSet;

        const vectorField& C = mesh.cellCentres();

        scalar dist = 4;

        forAll(C, cellI)
        {
            if (C[cellI].x() >= -dist)
            {
                if
                (
                    (mag(C[cellI].y()) <= dist)
                 && (mag(C[cellI].z()) <= dist)
                )
                {
                    refCellSet.insert(cellI);
                }
            }
        }

        labelList refCells(refCellSet.toc());

        refineImmersedBoundaryMesh rib(mesh);
        rib.refineMesh(refCells);
    }

    // 2
    {
        labelHashSet refCellSet;

        const vectorField& C = mesh.cellCentres();

        scalar dist = 3;

        forAll(C, cellI)
        {
            if (C[cellI].x() >= -dist)
            {
                if
                (
                    (mag(C[cellI].y()) <= dist)
                 && (mag(C[cellI].z()) <= dist)
                )
                {
                    refCellSet.insert(cellI);
                }
            }
        }

        labelList refCells(refCellSet.toc());

        refineImmersedBoundaryMesh rib(mesh);
        rib.refineMesh(refCells);
    }

    // 3
    {
        labelHashSet refCellSet;

        const vectorField& C = mesh.cellCentres();

        scalar dist = 2.5;

        forAll(C, cellI)
        {
            if (C[cellI].x() >= -dist)
            {
                if
                (
                    (mag(C[cellI].y()) <= dist)
                 && (mag(C[cellI].z()) <= dist)
                )
                {
                    refCellSet.insert(cellI);
                }
            }
        }

        labelList refCells(refCellSet.toc());

        refineImmersedBoundaryMesh rib(mesh);
        rib.refineMesh(refCells);
    }

    // 4
    {
        labelHashSet refCellSet;

        const vectorField& C = mesh.cellCentres();

        scalar dist = 1.5;

        forAll(C, cellI)
        {
            if (C[cellI].x() >= -dist)
            {
                if
                (
                    (mag(C[cellI].y()) <= dist)
                 && (mag(C[cellI].z()) <= dist)
                )
                {
                    refCellSet.insert(cellI);
                }
            }
        }

        labelList refCells(refCellSet.toc());

        refineImmersedBoundaryMesh rib(mesh);
        rib.refineMesh(refCells);
    }

    mesh.write();

    Info<< "End\n" << endl;

    return(0);
}


// ************************************************************************* //
