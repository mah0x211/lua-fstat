# lua-fstat

[![test](https://github.com/mah0x211/lua-fstat/actions/workflows/test.yml/badge.svg)](https://github.com/mah0x211/lua-fstat/actions/workflows/test.yml)
[![codecov](https://codecov.io/gh/mah0x211/lua-fstat/branch/master/graph/badge.svg)](https://codecov.io/gh/mah0x211/lua-fstat)

obtains information about the file pointed to by the argument.


## Installation

```
luarocks install fstat
```

## Usage

```lua
local dump = require('dump')
local fstat = require('fstat')

local stat = assert(fstat('./'))
print(dump(stat))
-- above code will be output as follows;
-- {
--     atime = 1644412057,
--     blksize = 4096,
--     blocks = 0,
--     ctime = 1644412057,
--     dev = 16777220,
--     gid = 20,
--     ino = 12946860530,
--     mode = 16877,
--     mtime = 1644412057,
--     nlink = 14,
--     perm = "0755",
--     rdev = 0,
--     size = 448,
--     type = "directory",
--     uid = 504
-- }
```

## Error Handling

the following functions return the `error` object created by https://github.com/mah0x211/lua-errno module.


## stat, err = fstat( v [, follow_symlink] )

Get file status information.

**Parameters**

- `v:string|file|uint`: `pathname`, `file*` or file descriptor.
- `follow_symlink:boolean`: follow symbolic links. (default: `true`)

**Returns**

- `stat:table`: a table that contains the following fields.
  - `dev:integer`: ID of device containing file.
  - `ino:integer`: inode number.
  - `mode:integer`: Mode of file.
  - `nlink:integer`: number of hard links.
  - `uid:integer`: user ID of owner.
  - `gid:integer`: group ID of owner.
  - `rdev:integer`: device ID (if special file) .
  - `size:integer`: total size, in bytes.
  - `blksize:integer`: blocksize for file system I/O.
  - `blocks:integer`: number of 512B blocks allocated.
  - `atime:integer`: time of last access.
  - `mtime:integer`: time of last modification.
  - `ctime:integer`: time of last status change.
  - `perm:string`: file permission bits (include the sticky bit).
  - `type:string`: `nil` or one of the following; `'file'`, `'directory'`, `'symlink'`, `'character_device'`, `'block_device'`, `'socket'`, `'fifo'`.
- `err:error`: `error` object on failure.



