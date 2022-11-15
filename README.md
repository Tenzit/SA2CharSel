# SA2CharSel
SA2 Character Select mod, extended with nice-to-haves for SA2A speedrunning

## Usage

Download `Character-Select-v2.7z` from [the latest release](https://github.com/Tenzit/SA2CharSel/releases) and extract to your `mods` folder.

To automatically set alt/alt-swap characters, set `Preset` to either `Alt` or `Alt-Swap` in the mod configuration.

Character selection VIA buttons is disabled by default for speedrun purposes. To re-enable it, set `Disable Button Select` to `False` in the mod configuration.

Press the following buttons to override your character while loading a level:
```
Left - Sonic
Right - Shadow
Down - Knuckles
Up - Rouge
R - Mech Tails
L - Mech Eggman
Y - Tails
X - Eggman
B - Alternate Character (Amy, Metal Sonic, etc)
A - Alternate Costume
Hold Start on player 2's controller to force them to spawn in 1P stages.
```

**Note**: Button select does not work for Kart stages, you have to manually set the kart you want in the mod configuration.

## Building from source

Development was done using VS 2017

* Clone the repo using `git clone --recursive`
* Open the solution
* For both `ModLoaderCommon` and `SA2CharSel`, `Right Click -> Properties` and set the platform toolset to whatever you have installed (I used v141)
* `Project -> Retarget Solution` to whatever Windows SDK version you have for both `ModLoaderCommon` and `SA2CharSel`
* You should be able to then build from the build menu
* You'll need to repeat steps 3 and 4 for the `Release` build as well
* Copy the `SA2CharSel.dll` file found in either `%RepoRoot%/Debug` or `%RepoRoot%/Release` to `%RepoRoot%/Character Select v2`
* Copy the entire `%RepoRoot%/Character Select v2` folder to your SA2 `mods` folder
