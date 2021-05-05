# Task Warrior extension for evolution

This holds an [evolution](https://wiki.gnome.org/Apps/Evolution) extension that allows adding
E-Mails as tasks for [TaskWarrior](https://taskwarrior.org/). It doesn't do anything fancy (yet) and
it probably is not completely error-save to use in some edge-cases.

I basically created this to serve my personal purpose of adding E-Mails to my task list. If it is
useful to others, I'm happy :-)

This plugin is based on the [Example
Module](https://wiki.gnome.org/Apps/Evolution/Extensions#Example_Module) stripped all unneeded code.
There might be some unneeded code being left around.

## Requirements
 * This plugin requires the `task` executable to be present and executable.
 * Requires evolution >= 3.36.0

**Note**: If evolution is installed on ubuntu using `apt`, you probably need to install the
development header package `evolution-dev`, as well.

## Installation
For evolution >= 3.40.0:
```bash
git clone https://github.com/fmauch/evolution-taskwarrior.git
cd evolution-taskwarrior
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=~/.local/share/evolution/modules \
           -DFORCE_INSTALL_PREFIX=ON ..
make && make install
```

For evolution < 3.40.0:
```bash
git clone https://github.com/fmauch/evolution-taskwarrior.git
cd evolution-taskwarrior
mkdir build && cd build
cmake -G "Unix Makefiles" ..
make && sudo make install
```

After installation, you should have an entry in the context menu when right-clicking on an E-Mail
saying "To Task Warrior" and a toolbar entry saying "Create TaskWarrior task".

## Troubleshooting
This extension doesn't have any meaningful error handling or feedback (yet). If you start evolution
from the shell, you'll see the output similar like

```
Executing: task add +email "This is my mail subject (From: <name@example.org>)"
Created task 87.
```
