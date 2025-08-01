/**
 *  Copyright (C) 2022 Masatoshi Fukunaga
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
// lua
#include "lua_errno.h"

static int stat2table(lua_State *L, struct stat *buf)
{
    char perm[6] = {0};

    // set fields
    lua_createtable(L, 0, 14);
    // add descriptor
    lauxh_pushint2tbl(L, "dev", buf->st_dev);
    lauxh_pushint2tbl(L, "ino", buf->st_ino);
    lauxh_pushint2tbl(L, "mode", buf->st_mode);
    lauxh_pushint2tbl(L, "nlink", buf->st_nlink);
    lauxh_pushint2tbl(L, "uid", buf->st_uid);
    lauxh_pushint2tbl(L, "gid", buf->st_gid);
    lauxh_pushint2tbl(L, "rdev", buf->st_rdev);
    lauxh_pushint2tbl(L, "size", buf->st_size);
    lauxh_pushint2tbl(L, "blksize", buf->st_blksize);
    lauxh_pushint2tbl(L, "blocks", buf->st_blocks);
    lauxh_pushint2tbl(L, "atime", buf->st_atime);
    lauxh_pushint2tbl(L, "mtime", buf->st_mtime);
    lauxh_pushint2tbl(L, "ctime", buf->st_ctime);
    snprintf(perm, sizeof(perm), "%#o", buf->st_mode & 01777);
    lauxh_pushstr2tbl(L, "perm", perm);
    switch (buf->st_mode & S_IFMT) {
    case S_IFREG:
        lauxh_pushstr2tbl(L, "type", "file");
        break;
    case S_IFDIR:
        lauxh_pushstr2tbl(L, "type", "directory");
        break;
    case S_IFLNK:
        lauxh_pushstr2tbl(L, "type", "symlink");
        break;
    case S_IFCHR:
        lauxh_pushstr2tbl(L, "type", "character_device");
        break;
    case S_IFBLK:
        lauxh_pushstr2tbl(L, "type", "block_device");
        break;
    case S_IFSOCK:
        lauxh_pushstr2tbl(L, "type", "socket");
        break;
    case S_IFIFO:
        lauxh_pushstr2tbl(L, "type", "fifo");
        break;
    }

    return 1;
}

static int stat_lua(lua_State *L, int flgs)
{
    const char *path = lua_tostring(L, 1);
    struct stat buf  = {0};

    if ((flgs & O_NOFOLLOW) ? lstat(path, &buf) : stat(path, &buf)) {
        // got error
        lua_pushnil(L);
        lua_errno_new(L, errno, "fstat");
        return 2;
    }

    return stat2table(L, &buf);
}

static int fstat_lua(lua_State *L)
{
    int fd          = -1;
    int flgs        = O_RDONLY | O_CLOEXEC;
    struct stat buf = {0};

    // followsymlinks option: default true
    if (!lauxh_optboolean(L, 2, 1)) {
        flgs |= O_NOFOLLOW;
    }
    lua_settop(L, 1);

    switch (lua_type(L, 1)) {
    case LUA_TSTRING:
        return stat_lua(L, flgs);

    case LUA_TUSERDATA: {
        FILE *f = lauxh_checkfile(L, 1);
        fd      = fileno(f);
    } break;

    case LUA_TNUMBER: {
        lua_Integer iv = lauxh_checkinteger(L, 1);
        if (iv < 0 || iv > INT_MAX) {
            return lauxh_argerror(L, 1, "integer (0-%d) expected, got %d",
                                  INT_MAX, iv);
        }
        fd = iv;
    } break;

    default:
        return lauxh_argerror(L, 1, "string, file or integer expected, got %s",
                              luaL_typename(L, 1));
    }

    if (fstat(fd, &buf) == -1) {
        // got error
        lua_pushnil(L);
        lua_errno_new(L, errno, "fstat");
        return 2;
    }
    return stat2table(L, &buf);
}

LUALIB_API int luaopen_fstat(lua_State *L)
{
    lua_errno_loadlib(L);
    lua_pushcfunction(L, fstat_lua);
    return 1;
}
