# SimplePluginHost

Welcome ! Have you ever wondered how to implement an audio plugin host ? Are you tired of system dependent library hierarchy bullshit ? Do you still not understand how something as widespread as pthread is so hard to make portable I still haven't figured it out ? Wonder no more !
Through the power of black magic (children to sacrifice not included) you can now use JUCE in a way noone sane would, enabling it to be exported as a static library and used in other programs without even limiting yourself to using one framework ! You can have multiple frameworks in your program and get DOUBLE the amount of unnecessary bloating, barely understandable object oriented chaos, and even spooky errors ! We even have compatibility with stupid gui libraries that randomly crash !! And we don't offer any guarantees !!!

(Whatever you do however DO NOT look too much the source code you might want to end me...)
Also I have no idea how I'm going to port this to windows, life's unkowns are what makes it exciting right ??

## How it works
This is a projucer project, to edit the juce modules imported just open it using the editor. Instead of reading JUCE's documentation (which I knew would explain to me how to properly implement all my code in order to have JUCE control everything in funny framework fashion...), I decided to have JUCE initialisation functions where they shouldn't be and headers containing implementation independent code (complete with void* pointers!!!!). The headers get copied to _export/include_ and you can either add `-I (path)` to your compilation arguments or you can copy them again somewhere else Idc Xdd
As for the cpp code itself I tried to make simple and straightforward shit in order for people smarter than me to be able to make something clean and usable out of this.

## Dependencies
I don't remember what I did lol but here's what you need to compile it for VST plugins :
- JUCE (of course)
- Steinberg SDK
- pthread (hee hee hee ha)

## How to use it
Very simple, just use the makefile. If you're not on linux (cringe ?!) then just find another way to export it using projucer I haven't tried to do that yet.
Commands :
```console
make
```
This will compile all and put everything you need in the export folder.

```console
make clean
```
This will remove everything in the export folder.

```console
make clean-all
```
Will `make clean` and destroy any binaries created in the juce folders (useful if you changed anything in projucer).

## Random shit included with this package (make sure everything is in there cause we ain't refunding you)
Amörbius sussy battle royale (ekesbokessirisekes edition) ft. Walter White from fnf with No Brim 3 & knuckles + gru fards DLC (Dariacore music remix)
Release date: 1984 with love from Brazil
Play as the morbius main character gigachad to defeat the poopshitters in this epic battle for the future of LEGO city.