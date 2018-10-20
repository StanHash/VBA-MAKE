# "VBA-MAKE"

This is *not* the original Void's Blitzarre Advance (aka CCB_Round1). It can be found [here](https://github.com/FireEmblemUniverse/CCB_Round1).

This is for a tentative port of it's source to EA/make as well as general cleanup.

This is very WIP.

The goals of this project are (in no particlular order):

- To provide an example of a full hack that fully uses the make/EA method instead of the older (and *slower*) `MAKE HACK.cmd` to people.
  - Even outside of the whole make/EA, eventually providing a cleaner and well-documented open source full hack is the goal (this will no doubt have to come with many bugfixes and whatnot).
- For me to test the still experimental "CHAX" make/EA setup on a larger project and improve upon it as I go and do this.
- Maybe if I'm motivated start some kind of "overhaul" of the original VBA? This will come after everything else tho (if ever).
  - *If* I start doing this, maybe bring some more people in so we can have this fun together \:)

Oh and this also has an updated skill system \:)

## Building

The setup required to build this is very much similar to what you need for [FE-CHAX](https://github.com/StanHash/FE-CHAX) (although not all CHAX tools are used yet, but they will most likely be eventually), so tl;dr you need:

- devkitARM and the `DEVKITARM` env var (unused yet)
- some unix/msys
- python 3 in your path
- A non-standard EA as `Tools/EventAssembler` (see CHAX for details and link)
- Png2Dmp, PortraitFormatter and ParseFile in `Tools`
- lyn in `Tools` (unused yet)
- gbagfx in `Tools`

I plan on eventually distributing an all in one "Tools" package to make this a bit easier but right now I'm still messing with everything all the time and updating the packing to go with it would be impractical.

In addition to this, the `Tools/py/NMM2CSV` and `Tools/py/TMX2EA` submodules are broken right now: this is because I'm relying on features that are not available on the master branches of the FEU repositories yet. To fix this, go into those folders and `git checkout c2ea-rewrite-process` (for NMM2CSV) or `git checkout misc_improvements` (for TMX2EA).

# Original VBA Credits

## Chapters

    Prologue    IT BEGINS                 Circles
    Ch1         Secret Chapter            Leonarth
    Ch2         The Bone Hole             Zmr
    Ch3         Compile Proyect           Darr
    Ch4         Monkey Business           Circles
    Ch5         Winter Invasion           Alusq
    Ch6         Fort of Bad               Stan
    Ch7         Port of Bad               Mystic
    Ch8         Yet Another Chapter       Colorz
    Ch9         Tragic Backstory          Klok
    Ch10        here you like Men         Ross
    Ch11        Factory Gold              Wan!
    Ch12        I felt Sadistic           Zmr
    Ch13        Coarse, Rough, Irritating Circles
    Ch14        Attack on Basement        OwainDark/Ardendark/Memesmith
    Ch15        The Shark Straits         Darr
    Ch16        Dead Valley               Venno
    Ch17        "Slowly" Corrupting       Leonarth
    Ch18        Island Clash              Kirb
    Ch19        Lit chapter               Kirb
    Ch20        Great rivalry             Kirb
    Ch21        Doot Doot                 Colorz
    Ch22        N i c e B o a t s         Kirb
    Ch23        Flesh and Bone            Alusq
    Ch24        River Rush                Teraspark
    Ch25        Shrine of Meals           Alusq
    Ch26        Darrman's Lakes           Zmr
    Ch27        Convenient Lake           Kirb
    Ch28        Mess.jpg                  Kirb
    Ch29        Let It Be                 Grey
    Ch30        End of All                Circles

## Mugs

Zmr56 for Bede, Tye, Jake
L95 for Void, Celicia, Mark, Tatara
Coby for Celica
Ross for Samson
Alusq for Tim, Tam, Saara, Banana, Mr Bones, Björn, Roshea
Wan for Arve
Tera for Evil Florina, Doowile
Klok for Edge
Monkeybard for Doot, Maeda, Kaga
Darrman for Zigludo, Tiki
blueeyedman24 for Gwendy
Kirb for Edgeworth, Grit, Marf (edited from Didja Redo's version)
Crazycolorz for Prine
Astelaine for Not Arch

## Maps

    Prologue    IT BEGINS                 Circles
    Ch1         Secret Chapter            Leonarth
    Ch2         The Bone Hole             Zmr
    Ch3         Compile Proyect           Darr
    Ch4         Monkey Business           Aurawolf
    Ch5         Winter Invasion           Alusq
    Ch6         Fort of Bad               Stan
    Ch7         Port of Bad               Mystic
    Ch8         Yet Another Chapter       Colorz
    Ch9         Tragic Backstory          Klok
    Ch10        here you like Men         Klok
    Ch11        Factory Gold              Wan!
    Ch12        I felt Sadistic           Zmr
    Ch13        Coarse, Rough, Irritating Circles
    Ch14        Attack on Basement        OwainDark/Ardendark/Memesmith
    Ch15        The Shark Straits         Bishop Seva
    Ch16        Dead Valley               Venno
    Ch17        "Slowly" Corrupting       Leonarth
    Ch18        Island Clash              Kirb
    Ch19        Lit chapter               Kirb
    Ch20        Great rivalry             Kirb
    Ch21        Doot Doot                 Bishop Seva
    Ch22        N i c e B o a t s         Aurawolf
    Ch23        Flesh and Bone            Alusq
    Ch24        River Rush                Teraspark
    Ch25        Shrine of Meals           Alusq
    Ch26        Darrman's Lakes           Zmr
    Ch27        Convenient Lake           Primefusion
    Ch28        Mess.jpg                  Kirb
    Ch29        Let It Be                 Grey
    Ch30        End of All                Circles

## Writing

MCProductions (text)
Darrman (formatting)

## Music

Alusq
Ash

## General Tools

Nintenlord
Crazycolorz
Circles

## Hax

Leonarth
Stan
Teq
Circles
Colorz
so many

## Misc Graphics

Circles
Seva
Wulfos
Glacoe
Paterick
Teraspark
Monkeybard
Black Mage
Alusq
