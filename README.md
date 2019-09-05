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

## Engine hacks changes

I'm going to take a lot of liberties as to what I add and change, as well as in how I do so.

Trying to keep using old stuff for the sake of doing so is getting tiring and I'd rather do what I feel would be better. The skill system in this project will probably end up looking very different from the main Skill System branch; and will probably be much incompatible.

This will involve rethinking hooks, modularise everything and stuff; And porting half (and only half! see below) of the hacks to C \:)

### ASM vs. C

As you *may* know, I have been primarily hacking in C for the past almost year now (I started doing that around december 2017). So it would be logical (albeit a bit overkill) for me to port everything to C and stuff.

I'm not going to do that.

Don't get me wrong, the plan *still is* to review everything; but for this project in particular, since the goal is provide a complete example, I want to keep a healthy mix of both ASM and C. So even some of the new stuff I'm adding will be 100% ASM.

However, everything written in ASM *will* be ported to use lyn and the [`FE-CLib`](https://github.com/StanHash/FE-CLib) reference, will hopefully no exception.

## Building

![cool gif](https://cdn.discordapp.com/attachments/358238759227228165/504352658824626198/TimeMake.gif)

The setup required to build this is very much similar to what you need for [FE-CHAX](https://github.com/StanHash/FE-CHAX) (although not all CHAX tools are used yet, but they will most likely be eventually), so tl;dr you need:

- devkitARM and the `DEVKITARM` env var
- some unix/msys
- python 3 in your path
- A non-standard EA as `Tools/EventAssembler` (see CHAX for details and link), used for generation make dependencies
  - Right now this also requires a custom build of [ColorzCore](https://github.com/FireEmblemUniverse/ColorzCore) (specifically from the [`core-relative-include` branch in my fork of it](https://github.com/StanHash/ColorzCore/tree/core-relative-include)), as ColorzCore is now used for the "final" pass.
    - This also requires the raws and stdlib from [my unofficial "experimental" stdlib fork](https://github.com/StanHash/EAStandardLibrary/tree/experimental)!
      - So much stuff oof
- Png2Dmp, PortraitFormatter and ParseFile in `Tools`
- lyn in `Tools`
- gbagfx in `Tools`

I plan on eventually distributing an all in one "Tools" package to make this a bit easier but right now I'm still messing with everything all the time and updating the packing to go with it would be impractical.

## File extensions conventions

Because of make rules and also consistency, I am defining rules for what extension corresponds to what file type:

| File type         | Extension | Can generate | Can be generated from           | Notes |
| ----------------- | --------- | ------------ | ------------------------------- | ----- |
| Event Source      | `.event`  | -            | `.csv`+`.nmm`; `.tmx`; `.o`... | events generated from `.o` have full extension `.lyn.event`.
| Compressed Binary | `.lz`     | -            | any                             | generated `.lz` don't lose original extension (`.4bpp` => `4bpp.lz`).
| Arbitrary Binary  | `.bin`    | -            | -                               | -
| Generated Binary  | `.dmp`    | -            | `.o`; `.tmx`; `.png` (portrait), `.fetxt` | -
| GBA Tile Graphics | `.4bpp`   | -            | `.png`                          | -
| GBA Palette       | `.gbapal` | -            | `.png`                          | -
| FE Formatted Text | `.fetxt`  | `.dmp`       | `.txt` (textprocess)            | generated dmps have full extension `.fetxt.dmp`.
| Generic Image     | `.png`    | `.4bpp`; `.gbapal`; `.dmp` (portrait) | -      | portrait `.dmp`s have `_mug`, `_minimug`, `_frames` and `_palette` postfixes.
| ASM Source        | `.s`      | `.o`         | -                               | -
| Generated ASM     | `.asm`    | -            | `.c`                            | for reference and debugging C only
| C Source          | `.c`      | `.asm`, `.o` | -                               | -
| Relocatable Object | `.o`     | `.lyn.event`; `.dmp` | `.s`                    | ELF
| Tiled Map         | `.tmx`    | `.dmp`+`.event` | -                            | `.dmp`s have `_data` postfix
| Table CSV Sheet   | `.csv`    | `.event`     | -                               | `.event` generation requires corresponding `.nmm`
| Table NMM         | `.nmm`    | `.event`     | -                               | `.event` generation requires corresponding `.csv`

Note: all files don't follow those conventions yet, but this plan is that they eventually will.

# Original VBA Credits

Chapters
----

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

Mugs
----

- Zmr56 for Bede, Tye, Jake
- L95 for Void, Celicia, Mark, Tatara
- Coby for Celica
- Ross for Samson
- Alusq for Tim, Tam, Saara, Banana, Mr Bones, Björn, Roshea
- Wan for Arve
- Tera for Evil Florina, Doowile
- Klok for Edge
- Monkeybard for Doot, Maeda, Kaga
- Darrman for Zigludo, Tiki
- blueeyedman24 for Gwendy
- Kirb for Edgeworth, Grit, Marf (edited from Didja Redo's version)
- Crazycolorz for Prine
- Astelaine for Not Arch

Maps
----

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

Writing
----

- MCProductions (text)
- Darrman (formatting)

Music
----

- Alusq
- Ash

General Tools
----

- Nintenlord
- Crazycolorz
- Circles

Hax
----

- Leonarth
- Stan
- Teq
- Circles
- Colorz
- so many

Misc Graphics
----

- Circles
- Seva
- Wulfos
- Glacoe
- Paterick
- Teraspark
- Monkeybard
- Black Mage
- Alusq
