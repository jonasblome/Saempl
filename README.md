# Sæmpl
A sample manager for automatic sample analysis and clustering, giving you access to properties like key, loudness or tempo of a sample!

Choose a directory on your PC as your sample library (Keybinding: D).
Navigate through your samples in a grid that clusters samples by similarity.
Select single or multiple tiles on the grid and drag and drop them into the "Favourites" section or into your DAW.
Press K to start instant playback of the selected sample and L to stop playback.

![Grid View Zoom 1](https://github.com/jonasblome/Saempl/assets/18214770/229c87a2-2157-4764-9943-0e1005c601bf)


Show all samples in your library in a table, sort the table columns or drag and drop the files into the "Favourites" section or into your DAW.

![Table View](https://github.com/jonasblome/Saempl/assets/18214770/73ff3067-0275-4e64-a332-00b46e9bcd49)


Search through your sample library in folder view. Drag and drop files into the audio player or into your "Favourites".

![Folder View](https://github.com/jonasblome/Saempl/assets/18214770/ac699579-8e45-4207-a24d-7cb35eb63fb2)


Navigate through the samples in the grid, table or folder view with the arrow buttons.
Use keybindings to switch between folder (1), table (2) and grid view (3).
Right click on samples to access a popup menu.

<img width="428" alt="Popup Menu" src="https://github.com/jonasblome/Saempl/assets/18214770/ad6a5805-f86e-4ccb-bf29-ed8d6be3d630">


Filter all samples in your library by activating/deactivating the filter and adding filter rules.
Deactivate or remove filter rules if they don't match anymore what you are looking for.

![Filter Panel](https://github.com/jonasblome/Saempl/assets/18214770/d54e1a87-98a2-4496-a432-24b9bc145ba5)


Press the die icon on top to select a random sample in the table view or grid view (Keybinding: S)
Press refresh to remove externally deleted files and search for new ones in the chosen sample library directory (Keybinding: R).

Integrated audio player for previewing of the audio and its waveform. Zoom in with the slider to have a closer look at the audio file.
Keybindings:
- Press enter to load a file from the navigation panel into the audio player
- Press space to playback the loaded audio file
- Toggle preview panel with T

![Sample Preview](https://github.com/jonasblome/Saempl/assets/18214770/80aba6e7-1a6e-44be-b7cd-1e8cbe32f448)


Hover over buttons or sliders for tooltips:

<img width="900" alt="Tooltip" src="https://github.com/jonasblome/Saempl/assets/18214770/d5ed1677-16ec-482f-a007-14632e31ff43">


Coming very soon:
- Key bindings for instant playback for file tree and table
- Handing down key events to currently showing components
- Improve filtering for key with more suitable string comparison
- Popup menu option to show selected item in favorites or audio preview on grid panel
- “About the makers” panel when clicking on logo with short instructions
- Use interfaces for component interactions instead of direct access
- Toggle option for which features are used during clustering
    - Store feature properties in sample item fields and remove from/copy them to feature vector on toggle
- Adjustable weightings for features to influence clustering
    - Multiply values from stored feature properties in sample items
- Smoothing tempo histogram before picking optimal tempo
- Sample renaming option
- Customizable tag per sample
- Suggestions that are close to favorites average
- Contrasting samples to chosen sample or favorites average
- Replace JUCE arrays with std::vectors in sorting and analyser
- Multithreading for grid sorting
- More features with [spectral analysis](https://docs.twoears.eu/en/latest/afe/available-processors/spectral-features/#tzanetakis2002)
- Fix crash when switching audio output device


Ideas for the Future:
- Sample property editor (keep custom values when analysis version changes)
- Sound category detection and grid tiles colored accordingly
- Slicing of files after silence and renaming them sensibly
- Cutting files up in beat clusters with automatic slice renaming
