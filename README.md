# CoolProjects #

This repository includes a group of projects for me to "reinvent the wheel" just for the fun of it.

This project it is being done in C.

## NiceList ##

This project is a new version of the __ls__ command from __bash__. This version would:

- show hidden items (similar to __ls -a__)

- show the item size, permissions and the item name (similar to __ls -l__)

- show a column name per each of the new info

- __Main Feature__ -> (If possible) show multiple items per line of terminal (similar to __ls__) __BUT__ it would work if you have other informations other than the item name. Example:

```
 Permissions   | Size | Name       | Permissions   | Size | Name       |
-----------------------------------|-----------------------------------|
 d rwx rwx r-- | 10Kb | Directory1 | - rwx rwx r-- | 1Kb  | File3      |
 - rwx r-x r-x |  1Gb | File1      | d rwx r-- r-- | 1Kb  | Directory2 |
 - rwx r-x r-x |  1Mb | File2      | d rwx --x r-- | 1Kb  | Directory3 |
```

Future features:

- allow the user to choose/add new informations per item (group, user, access, modiffication, change dates etc.)

- allow the user to have a config file

- add color support (Wich I have no ideia on how to do XD)