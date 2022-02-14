local open = io.open
local testcase = require('testcase')
local errno = require('error').errno
local fileno = require('ioex').fileno
local fstat = require('fstat')

function testcase.fstat()
    -- test that get stat by pathname
    local stat, err, eno = fstat('./')
    assert(stat, err)
    assert.is_nil(eno)
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

    -- test that get stat by file
    local f = assert(open('./fstat_test.lua'))
    stat = assert(fstat(f))
    assert.equal(stat.type, 'file')

    -- test that get stat by file descriptor
    local fd = assert(fileno(f))
    local fdstat = assert(fstat(fd))
    f:close()
    assert.equal(fdstat, stat)

    -- test that return error
    stat, err, eno = fstat('./unknowndir')
    assert.is_nil(stat)
    assert.match(err, 'No .+ directory', false)
    assert.equal(errno[eno], errno.ENOENT)

    -- test that throws an error when file descriptor is out of range
    err = assert.throws(fstat, -1)
    assert.match(err, '#1 .+ [(]integer .+ expected, got %-1', false)

    -- test that throws an error when argument type is invalid
    err = assert.throws(fstat, {})
    assert.match(err, '#1 .+ [(]string, file or integer expected, got table',
                 false)
end
