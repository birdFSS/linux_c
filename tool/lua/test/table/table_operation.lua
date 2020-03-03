-- table 类似与c的数组指针

mytable = {}  -- 初始化

mytable[1] = "apple"
mytable["wow"] = 'banana'
mytable = { "green", "blue", "yellow", "black" } --直接使用=号，相当与新建一个table
--mytable = { [1] = "green", [2] = "blue", [3] = "yellow", [4] = "black" } -- 这个不是数组，而是关联数组了
print('mytable = { "green", "blue", "yellow", "black"  }')
for a, b in pairs(mytable) do   --pairs是按顺序输出table中的值
    print(a .. "--->" .. b)
end

table.sort(mytable) --升序排序, 这时候是数组
print("table.sort(mytable)")
for a, b in pairs(mytable) do   --pairs是按顺序输出table中的值
    print(a .. "--->" .. b)
end



