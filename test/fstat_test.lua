local open = io.open
local testcase = require('testcase')
local errno = require('errno')
local fileno = require('io.fileno')
local fstat = require('fstat')

function testcase.fstat_directory_path()
    -- test that get stat by pathname
    local stat, err = assert(fstat('./'))
    assert.is_nil(err)
    for k, isfn in pairs({
        dev = assert.is_int,
        ino = assert.is_int,
        mode = assert.is_int,
        nlink = assert.is_int,
        uid = assert.is_int,
        gid = assert.is_int,
        rdev = assert.is_int,
        size = assert.is_int,
        blksize = assert.is_int,
        blocks = assert.is_int,
        atime = assert.is_int,
        mtime = assert.is_int,
        ctime = assert.is_int,
        perm = assert.is_string,
        type = assert.is_string,
    }) do
        isfn(stat[k])
    end
    assert.equal(stat.type, 'directory')
end

function testcase.fstat_file_path()
    -- test that get stat by pathname
    local stat, err = assert(fstat('./fstat_test.lua'))
    assert.is_nil(err)
    for k, isfn in pairs({
        dev = assert.is_int,
        ino = assert.is_int,
        mode = assert.is_int,
        nlink = assert.is_int,
        uid = assert.is_int,
        gid = assert.is_int,
        rdev = assert.is_int,
        size = assert.is_int,
        blksize = assert.is_int,
        blocks = assert.is_int,
        atime = assert.is_int,
        mtime = assert.is_int,
        ctime = assert.is_int,
        perm = assert.is_string,
        type = assert.is_string,
    }) do
        isfn(stat[k])
    end
    assert.equal(stat.type, 'file')
end

function testcase.fstat_file()
    -- test that get stat by file
    local stat = assert(fstat('./fstat_test.lua'))
    local f = assert(open('./fstat_test.lua'))
    local filestat, err = assert(fstat(f))
    f:close()
    assert.is_nil(err)
    assert.equal(filestat, stat)
end

function testcase.fstat_file_descriptor()
    -- test that get stat by file descriptor
    local stat = assert(fstat('./fstat_test.lua'))
    local f = assert(open('./fstat_test.lua'))
    local fd = assert(fileno(f))
    local fdstat, err = assert(fstat(fd))
    f:close()
    assert.is_nil(err)
    assert.equal(fdstat, stat)
end

function testcase.fstat_error()
    -- test that return error
    local stat, err = fstat('./unknowndir')
    assert.is_nil(stat)
    assert.equal(err.type, errno.ENOENT)

    -- test that throws an error when file descriptor is out of range
    err = assert.throws(fstat, -1)
    assert.match(err, '#1 .+ [(]integer .+ expected, got %-1', false)

    -- test that throws an error when argument type is invalid
    err = assert.throws(fstat, {})
    assert.match(err, '#1 .+ [(]string, file or integer expected, got table',
                 false)
end
