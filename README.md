# Very early development.

### It's like Runelite for RS3, but not as verbose. Our client needs some love and Jamflex isn't doing it :(

Discord here -> https://discord.gg/C2TUSwXhqQ

# Building

* Clone the project
* Preferrably use vcpkg to install dependencies.
* Build the project.

```
vcpkg install glew:x64-windows
vcpkg install imgui[core,opengl3-binding,win32-binding]:x64-windows
```

# More up to date screenshot

![Current State](https://media.discordapp.net/attachments/484796759437213716/1089661439821750366/image.png)

![Old State](https://i.imgur.com/ufqBn4X.jpg)

# Notes about the client.

This game is extremely different from any game I've looked at before. **A lot** of functionality stems from a VM that interprets their own inhouse language/languages(plural?).

These being cs2/Runescript.

Engine hooks are exposed to the VM and then called by cs2/Runescript.

# Functions of interest.

### Note, these functions are operating under the format of module base address + function location. Rebase your disassembler to 0x0.

Investigating what happens when you click on the game's canvas, you'll find yourself at the a function that determines what type of action you're performing and what object you're performing it on, as well as a pointer to a Vec2d struct containing the x/y coordinates of your mouse at the time of click.

That function is located here `0xB6560`.

Checking what calls this function, we're lead to some interesting results.

![Handle Click xrefs](https://i.imgur.com/RgF8rQ8.png)

I'm mainly interested in "MiniMenuMainLogic". Which is internally structured as `jag::MiniMenu::MainLogic`.

This is located at `0xB51D0`

Outside of runtime functions (remember the VM I brought up earlier?), there's only one call.

![MiniMenuMainLogic xrefs](https://i.imgur.com/JYngoXJ.png)

Which brings us to the client's main logic loop.

`jag::Client::MainLogic` -> `0xED7F0`


Network wise, it's fairly basic.

Packet opcodes are shifted by an isaac cipher.

Any opcodes to do with cs2 are shuffled independently of the isaac cipher, meaning reversing just the isaac cipher isn't enough.

`jag::Isaac::TakeNext` (`0x84F40`) takes in the original opcode, say `0x7B`, and shifts it based on the current index of the Isaac class. Each shift, the index is decremented by 1. Once the index is 0, it resets to 255 and generates a byte array with a length of 255 to then be used for the next 255 packets.

The function responsible for generating the new Isaac table (255 length byte array) is `jag::Isaac::Generate`, located at `0x640730`.

`jag::ServerConnection::MakeClientMessage`(`0x822E0`) takes an out parameter and writes a `ClientMessage` skeleton to it.

`jag::ServerConnection::SendClientMessage` (`0x29F700`) sends that message over the wire.



