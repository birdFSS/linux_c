-- table 类似与c的数组指针

mytable = {}  -- 初始化

print("mytable type = " .. type(mytable))

mytable = nil -- lua资源回收器回收
print("mytable type = " .. type(mytable))

-- mytable[1] = "lua" -- 这时候不能赋值了，相当于free了

mytable = {}

mytable[1] = "apple"
mytable["wow"] = 'banana'
print("mytable[1]=" .. mytable[1])
print("mytable['wow']=", mytable["wow"])

table2 = mytable  -- 指针赋值
table2["wow"] = 1
print([[mytable['wow']=]] .. mytable["wow"])

table2 = nil

print([[mytable['wow']=]] .. mytable["wow"])

table2 = mytable
mytable = nil

print([[table2['wow']=]] .. table2["wow"])
