-- lua 模块与包
module = {}


function module.add(a, b)
    return a + b
end

function minus(a, b)
    return a - b
end

function module.divide(a, b) --module 函数前不能加local 导致error loading错误
    return a / b
end



return module
