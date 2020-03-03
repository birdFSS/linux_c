tab1 = { int1 = "val1", key1 = "val2", av = "val3" , "val4" }
x = "hello"
for k, v in pairs(tab1) do
    print( k .. "-" .. v )
end

tab1.key1 = nil
for k, v in pairs(tab1) do
    print(k .. " - " .. v)
end
