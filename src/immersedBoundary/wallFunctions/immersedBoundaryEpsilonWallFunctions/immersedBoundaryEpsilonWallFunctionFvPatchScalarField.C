/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "immersedBoundaryEpsilonWallFunctionFvPatchScalarField.H"
#include "RASModel.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace incompressible
{
namespace RASModels
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

immersedBoundaryEpsilonWallFunctionFvPatchScalarField::
immersedBoundaryEpsilonWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    immersedBoundaryFvPatchScalarField(p, iF),
    UName_("U"),
    kName_("k"),
    GName_("RASModel::G"),
    nuName_("nu"),
    nutName_("nut"),
    Cmu_(0.09),
    kappa_(0.41),
    E_(9.8)
{}


immersedBoundaryEpsilonWallFunctionFvPatchScalarField::
immersedBoundaryEpsilonWallFunctionFvPatchScalarField
(
    const immersedBoundaryEpsilonWallFunctionFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    immersedBoundaryFvPatchScalarField(ptf, p, iF, mapper),
    UName_(ptf.UName_),
    kName_(ptf.kName_),
    GName_(ptf.GName_),
    nuName_(ptf.nuName_),
    nutName_(ptf.nutName_),
    Cmu_(ptf.Cmu_),
    kappa_(ptf.kappa_),
    E_(ptf.E_)
{}


immersedBoundaryEpsilonWallFunctionFvPatchScalarField::
immersedBoundaryEpsilonWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    immersedBoundaryFvPatchScalarField(p, iF, dict),
    UName_(dict.lookupOrDefault<word>("U", "U")),
    kName_(dict.lookupOrDefault<word>("k", "k")),
    GName_(dict.lookupOrDefault<word>("G", "RASModel::G")),
    nuName_(dict.lookupOrDefault<word>("nu", "nu")),
    nutName_(dict.lookupOrDefault<word>("nut", "nut")),
    Cmu_(dict.lookupOrDefault<scalar>("Cmu", 0.09)),
    kappa_(dict.lookupOrDefault<scalar>("kappa", 0.41)),
    E_(dict.lookupOrDefault<scalar>("E", 9.8))
{}


immersedBoundaryEpsilonWallFunctionFvPatchScalarField::
immersedBoundaryEpsilonWallFunctionFvPatchScalarField
(
    const immersedBoundaryEpsilonWallFunctionFvPatchScalarField& ewfpsf
)
:
    immersedBoundaryFvPatchScalarField(ewfpsf),
    UName_(ewfpsf.UName_),
    kName_(ewfpsf.kName_),
    GName_(ewfpsf.GName_),
    nuName_(ewfpsf.nuName_),
    nutName_(ewfpsf.nutName_),
    Cmu_(ewfpsf.Cmu_),
    kappa_(ewfpsf.kappa_),
    E_(ewfpsf.E_)
{}


immersedBoundaryEpsilonWallFunctionFvPatchScalarField::
immersedBoundaryEpsilonWallFunctionFvPatchScalarField
(
    const immersedBoundaryEpsilonWallFunctionFvPatchScalarField& ewfpsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    immersedBoundaryFvPatchScalarField(ewfpsf, iF),
    UName_(ewfpsf.UName_),
    kName_(ewfpsf.kName_),
    GName_(ewfpsf.GName_),
    nuName_(ewfpsf.nuName_),
    nutName_(ewfpsf.nutName_),
    Cmu_(ewfpsf.Cmu_),
    kappa_(ewfpsf.kappa_),
    E_(ewfpsf.E_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void immersedBoundaryEpsilonWallFunctionFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    Info<< "Evaluating wall functions" << nl << endl;

    // If G field is not present, execute zero gradient evaluation
    // HJ, 20/Mar/2011
    if (!db().foundObject<volScalarField>(GName_))
    {
        InfoIn(__PRETTY_FUNCTION__)
            << "Cannot access " << GName_ << " field.  for patch "
            << patch().name() << ".  Evaluating as regular immersed boundary"
            << endl;

        immersedBoundaryFvPatchScalarField::evaluate();

        return;
    }

    const RASModel& rasModel = db().lookupObject<RASModel>("RASProperties");
//     const scalar yPlusLam = rasModel.yPlusLam(kappa_, E_);
    const scalarField& y = rasModel.y()[patch().index()];

    const scalar Cmu25 = pow(Cmu_, 0.25);
    const scalar Cmu75 = pow(Cmu_, 0.75);

    volScalarField& G = const_cast<volScalarField&>
        (db().lookupObject<volScalarField>(GName_));

    // Note: epsilon is now a refValue and set in
    // immersedBoundaryFvPatchField
    // HJ, 3/Aug/2011
    scalarField& epsilon = refValue();

    const volScalarField& k = db().lookupObject<volScalarField>(kName_);

    const scalarField& nuw =
        patch().lookupPatchField<volScalarField, scalar>(nuName_);

    const scalarField& nutw =
        patch().lookupPatchField<volScalarField, scalar>(nutName_);

    const fvPatchVectorField& Uw =
        patch().lookupPatchField<volVectorField, vector>(UName_);
    /*
    const scalarField magGradUw = mag(Uw.snGrad());

    // Set epsilon and G
    forAll(nutw, faceI)
    {
        label faceCellI = patch().faceCells()[faceI];

        scalar yPlus = Cmu25*y[faceI]*sqrt(k[faceCellI])/nuw[faceI];

        // Note: epsilon is now a refValue and set in
        // immersedBoundaryFvPatchField
        // HJ, 3/Aug/2011
        epsilon[faceI] = Cmu75*pow(k[faceCellI], 1.5)/(kappa_*y[faceI]);

        if (yPlus > yPlusLam)
        {
            // Original OpenFOAM implementation
//             G[faceCellI] =
//                 (nutw[faceI] + nuw[faceI])*magGradUw[faceI]
//                *Cmu25*sqrt(k[faceCellI])/(kappa_*y[faceI]);

            // Change for consistency with Fluent implementation.
            // Emil Baric, NUMAP-FOAM 2011
            // HJ, 13/Dec/2011
            G[faceCellI] =
                sqr((nutw[faceI] + nuw[faceI])*magGradUw[faceI])/
                (Cmu25*sqrt(k[faceCellI])*kappa_*y[faceI]);
        }
        else
        {
            G[faceCellI] = 0.0;
        }
    }

    // TODO: perform averaging for cells sharing more than one boundary face
*/
    immersedBoundaryFvPatchScalarField::updateCoeffs();
}


void immersedBoundaryEpsilonWallFunctionFvPatchScalarField::evaluate
(
    const Pstream::commsTypes commsType
)
{
    immersedBoundaryFvPatchScalarField::evaluate(commsType);
}


void immersedBoundaryEpsilonWallFunctionFvPatchScalarField::
write(Ostream& os) const
{
    immersedBoundaryFvPatchScalarField::write(os);
    writeEntryIfDifferent<word>(os, "U", "U", UName_);
    writeEntryIfDifferent<word>(os, "k", "k", kName_);
    writeEntryIfDifferent<word>(os, "G", "RASModel::G", GName_);
    writeEntryIfDifferent<word>(os, "nu", "nu", nuName_);
    writeEntryIfDifferent<word>(os, "nut", "nut", nutName_);
    os.writeKeyword("Cmu") << Cmu_ << token::END_STATEMENT << nl;
    os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
    os.writeKeyword("E") << E_ << token::END_STATEMENT << nl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    immersedBoundaryEpsilonWallFunctionFvPatchScalarField
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RASModels
} // End namespace incompressible
} // End namespace Foam

// ************************************************************************* //
