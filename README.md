# __NiceList__ #

This project is a new version of the __ls__ command from __bash__ and it's being developed in __C__. 

## Main Goals ##

- Show hidden items (similar to __ls -a__)

- Show the item size, permissions and the item name (similar to __ls -l__)

- Show a column name per each of the new info

- __Main Feature__ -> (If possible) show multiple items per line of terminal (similar to __ls__) __BUT__ it would work if you have other informations other than the item name. Example:

```
 Permissions   | Size | Name       | Permissions   | Size | Name       |
-----------------------------------|-----------------------------------|
 d rwx rwx r-- | 10Kb | Directory1 | - rwx rwx r-- | 1Kb  | File3      |
 - rwx r-x r-x |  1Gb | File1      | d rwx r-- r-- | 1Kb  | Directory2 |
 - rwx r-x r-x |  1Mb | File2      | d rwx --x r-- | 1Kb  | Directory3 |
```

 ## Future Features ##

- Fix a problem with relative paths.

- Allow the use of flags (Currently you can only change bool variables in the top of main file)

- Allow the user to have a config file

- Allow the user to choose/add new informations per item (group, user, access, modiffication, change dates etc.)

- Make algorithm to find the best number of columns. (Slightly Advanced)

## Instalation and Run Support ##

__Note :__ It's expected to run this in a Linux based Operative Systems.

__Step 1__ - Clone the contents of the repository.

__Step 2__ - Run __cd NiceList/__ and __make__.

__Step 3__ - Use __./nls__ to run the executable. (You must have run permissions. Check __ls -l__ if you don't)

__Step 4 (Optional)__ - Add __nls__ to your PATH.
