
local MD_HDB = 1

function Split(szFullString, szSeparator)
    local nFindStartIndex = 1
    local nSplitIndex = 1
    local nSplitArray = {}
    while true do
        local nFindLastIndex = string.find(szFullString, szSeparator, nFindStartIndex)
        if not nFindLastIndex then
            nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, string.len(szFullString))
            break
        end
        nSplitArray[nSplitIndex] = string.sub(szFullString, nFindStartIndex, nFindLastIndex - 1)
        nFindStartIndex = nFindLastIndex + string.len(szSeparator)
        nSplitIndex = nSplitIndex + 1
    end
    return nSplitArray
end

-- 根据不同后缀名取出id和virusName即可
function analyse_av_sig(line, mode)
    local id
    local virusName
    local arr = Split(line, ":")
    --测试使用，后面可以写脚本把其归一化，id都在第一个，virusName都在第二个
    if MD_HDB == mode then
        id = arr[2]
        virusName = arr[3]
    elseif MD_MDB == mode then
        id = arr[1]
        virusName = arr[2]
    end
    return id, virusName
end

function readdb(fileName)
    local mode
    --file = io.open(fileName, "r")
    --io.input(file)
    --sql.init_sqlite(DB_NAME)

    --根据文件后缀名判断id和文件名位置
    if nil ~= string.find(fileName, "hdb") or nil ~= string.find(fileName, "hsb") then
        mode = MD_HDB
    elseif nil ~= string.find(fileName, "mdb") then
        mode = MD_MDB
    end

    for line in io.lines(fileName) do
        local id,virusName = analyse_av_sig(line, mode)
        --print("id=" .. id .. "\nvirusname=" .. virusName)
        --sql.insert(conn, "id, name", id, name)
    end
    --sql.free_sqlite(DB_NAME)
    --io.close(file)
end

function normalize(fileName, outFileName)
    local mode
    file = io.open(outFileName, "w")
    io.output(file)


    --根据文件后缀名判断id和文件名位置
    if nil ~= string.find(fileName, "hdb") or nil ~= string.find(fileName, "hsb") then
        mode = MD_HDB
    elseif nil ~= string.find(fileName, "mdb") then
        mode = MD_MDB
    end

    for line in io.lines(fileName) do
        local id,virusName = analyse_av_sig(line, mode)
        --print("id=" .. id .. "\nvirusname=" .. virusName)
        io.write(id .. ":" .. virusName .. "\n")
    end
    --print(file:read("*l"))
    io.close(file)
end

--readdb("test.hdb")
normalize("main.hdb", "new.hdb")
