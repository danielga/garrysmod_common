# Microsoft Visual Studio template projects for Linux debugging

These templates allow debugging Linux Garry's Mod modules from Visual Studio remotely.
There are 2 pairs of files:

- `debugging_linux.vcxproj` and `debugging_linux.vcxproj.user` for the main branch
- `debugging_linux_x86_64.vcxproj` and `debugging_linux_x86_64.vcxproj.user` for the x86-64 branch

One of these pairs (depending on the target branch of your module) should be copied to the directory with the premake5 generated project files for Visual Studio and manually added to your solution.

## Customisation

There are somethings you can (and should) change, in the `.vcxproj` file:

- `<RemoteCompileToolExeSuffix>-9</RemoteCompileToolExeSuffix>`: Represents the version suffix of the `gcc` command. Can be empty to use the system's default `gcc`.
- `<RemoteSRCDSDir>~/garrysmod/server</RemoteSRCDSDir>`: Represents the location of your SRCDS install to use.
  
The following can be changed optionally (depending on how the module is built, for example):

- `<FilesToCopy Include="../../../source/**/*;../../../license.txt;../../../premake5.lua;../../../readme.md" />`: Represents the files to copy from your local system to the remote system. For example, you can an `includes` directory at the root of the module directory by appending `;../../../includes/**/*` to this configuration.
