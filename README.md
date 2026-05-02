# BudgetArmsEngine (BAV)

BAV is the result of improving the [Minigin Template]("https://github.com/avadae/minigin/"). <br>
Minigin originally used SDL2, so I migrated from SDL2 and other SDL related libraries (Mixer & SDL3_gfx) to
SDL3. <br>

## Features

- Loading images (.png, .jpg)
- Loading audio (.mp3)
- Loading data files (.json)
- Components (restrictive*)
- Scenes
- Graph & Graph Algorithms
- Audio Loading & Playing
- Event Queue & Audio Queue
- Subject & Observer pattern
- Controller & Keyboard Support
- State System (easily expandable)

*restrictive: A GameObject cannot have multiple of the same type of Component. <br>
e.g. A GameObject cannot have two TextureComponents.

## GameObject

The base Object type is an GameObject, other Game Engines might give it different names (Object, Actor, etc.). <br>
You cannot be derived from, since that's (kinda of) against the philosophy of why a component system is used. <br>

## Sound

The SoundSystem uses SDL_Mixer, the [Pimpl pattern]("https://en.cppreference.com/cpp/language/pimpl") **should** make
implementing another sound library a breeze. <br>
It comes with a LoggingSoundSystem to make debugging sound problems easier. <br>

However, due to Mixer types being deeply integrated into other structures AudioTrack/AudioQueue/AudioClip, implementing
another sound library would be a pain. <br>

This will change since the SdlAudioQueue will be moved into the SdlSoundSystem completely, like it should have been from
the beginning. <br>

## Todo's

- Update Sound system (port from SDL2_Mixer to SDL3_Mixer)
- Improve Sound handling (now uses ActiveSoundID, which is an ID for each playing sound).
- Remove ImGui & Steamworks support, since they should be implemented at the Game Level.
- Change the Audioqueue only when the sound system changes
- Create SoundId from SDMB hash
- Remove AudioQueue from ServiceLocator and implement it into SdlSoundSystem
- Change All Singleton Code: eg. Singleton::GetInstance().Function(); to Singleton::Function();
