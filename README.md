# BudgetArmsEngine (BAE)

BAE is the result of improving the [Minigin Template]("https://github.com/avadae/minigin/"). <br>
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

During the process of merging the SdlSoundSystem and the AudioQueue (previously templated), I noticed that the
AudioQueue should most likely be completely removed. However, due to my extreme stubbornness I doubt that will ever
happen. <br>
My desire to add new features to and improve the sound system is hindering my productivity to finish anything.

The wrapper classes that are Mixer related (Audio & AudioTrack) should be hidden in Pimpl of SdlSoundSystem or removed.
If it's hidden in Pimpl, then it should be renamed to make people know it's Mixer specific, because
Audio/AudioClip/AudioTrack all sound very broad, and neither do they indicate that they are all Mixer specific <br>

## Features to be implemented

Visit the [BudgetArmsEngine Project](https://github.com/users/BudgetArms/projects/3/) page
