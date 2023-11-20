# The Telltale Inspector
This is a modding application which aims to allow modders to mod newer games made by the Telltale Tool (Telltale Games).
For developers wishing to know more about internally how it works, please scroll to the bottom.

## What this app can do

### Edit .PROP PropertySet files
PROP files are a set of properties: key and value pairs. Keys are symbols which are just a 64 bit hash. Values are any type the Telltale Tool supports and this tool aims to be able to show as many of those as possible. This tool uses a big database of strings which are possible sources for the key hashes, so if keys in the property set are 'Symbol<XXX>' then no string with the hash was found. I am continually updating the database to remove as many
of those Symbol<XXX> keys as possible.
PROP files are used a lot in the engine and editing them can be a powerful way to change file references and model swap easily.

### Edit & Create .SCENE files
With this tool you can open and edit scene files and edit agent data. You can save back the scene file and then load it in game.

### Edit .D3DMESH files
D3DMesh files contain .PROP files inside them for most materials. This tool lets you modify those prop files and resave the mesh allowing you to
manage the materials and their properties and parameters (if you know what you are doing). 
This tool also allows you to export the mesh as .OBJ, and also do this in bulk. It allows you also to specify how the .MTL file should reference texture files: allowing you to define a prefix/suffix for the texture file name.
This also allows you to bulk convert a folder of meshes to OBJ.

PLEASE NOTE: Converting to OBJ works almost always for newer games, exporting vertices and faces correctly. UVs will not work for large meshes! Character meshes are generally fine, but UVs appear muddled up for large meshes. This is something which I cannot get a solution for at the moment but am working on.

### Convert .D3DTX files
D3DTx files are textures. This tool lets you export the largest mipmap to PNG and do this also in bulk.

### Open any Telltale File
About 99% of telltale files use a packaging format of a Meta Stream. This tool lets you open practically any file in this format and view its data and export it JSON so it is readable. For example if a file type does not have a proper editor yet you can still open it here and view its contents (for larger complex files this may not be possible until a specific editor for that file is made).

### Extract LUA Scripts
This tool lets you bulk convert lua scripts to decrypted and encrypted bytecode as well as compile scripts.

### Create, Extract and Manage .TTARCH2 bundles
This tool allows you to open, add files, remove files, extract all files, extract single files and create archives also allowing you to modify the compression type (oodle/zlib/none) and if it is encrypted.
This means you don't need to use old tools such as ttarchext and telltale explorer to create and view archives.

### Extract .BANK sound banks
This tool allows you to open telltale FMOD soundbanks (.BANK files stored in _ms.ttarch2 archives) and extract OGG audio data in bulk and single files. It allows you to optionally remove all the long FMOD ids from the file names, ie the long {AAAAA-BBBB-CCCC} part.


## For Developers
This tool uses a largely extended version of the Telltale Tool Library (see my other repos). This library is what allows reading of telltale files and re-exporting. This app uses that and provides a good way to view the data from those files.
### Where you fill find code:
<ol>
  <li><strong>Inspector/src/TelltaleInspector.cpp</strong>: This is a very long c++ source which contains pretty much all of the UI, file conversion and the whole app main code flow. It is NOT organised well! Use Ctrl+M+O on visual studio to collapse all so you can scroll through code.</li>
  <li><strong>Inspector/src/ToolLibrary</strong>: This folder contains all of the extended library which reads and writes telltale files.</li>
</ol>

## Supported Platforms
This app is only supported on Windows 10/11 and uses Vulkan for GUI rendering.

## Credit
This tool was made possible by years of reverse engineering by solely me. Over this time, I have had various help from people from the Telltale Modding discord and more. Below are the people who helped me along the way:

#### David M:
David is such a massive help and has guided me through rendering and the telltale tool from the beginning. He provided me with the WDC .PDB which without would have meant none of this is possible. [David's Github](https://github.com/frostbone25)
#### Aluigi Auriemma:
The original creator of ttarchext, would not have initially been able to read archives without help from him before the PDB. [Personal Website](https://aluigi.altervista.org/)
#### RandomTalkingBush:
He spent years on the 3DSMax D3DMesh importer which I still find myself sometimes referencing to help make difference between different D3DMesh file versions as I can only know 100% about the format in the Walking Dead Collection version of the engine. [RTB'S Github](https://github.com/RandomTBush)
#### Simon Pinfold
Very useful python script which provided information on the proprietary fmod sound bank version 5 format. (FSB5). [Simon's Github](https://github.com/HearthSim/python-fsb5)
#### Aizzble ('arizzble')
Continuous support and testing help from aabi, and my partner in building all of [Minecraft Story Mode in Minecraft](https://www.planetminecraft.com/project/mcsm-rebuilt-in-minecraft/) by using my tools and code to convert meshes into Minecraft Java.
#### TheCherno
Known for this game engine youtube channel, he also not only taught me C++ very well but also provided the backend GUI framework library <strong>Walnut</strong> which means in one line of code I have a GUI. The GUI library used is the iconic Dear ImGui. [TheCherno Youtube](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw)
#### Other Credit
Other authors code has been used such as the C++ file dialog helper library NFD and more.
