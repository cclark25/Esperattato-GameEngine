function __TS__ObjectKeys(obj)
    local result = {}
    for key in pairs(obj) do
        result[#result + 1] = key
    end
    return result
end



print("Lua called.")
createSquare(10,-10);
createSquare(50.5,-50.5);
createSquare(100.876,-100.145);

A = {  }

while true do
TestObject = createTestObject();
print(TestObject.A);
print(TestObject.B);
print(TestObject.C);
print(TestObject.D);
print((TestObject.SUB.A2));
print(TestObject.SUB.B2);
print(TestObject.FUN())
-- print(type(TestObject))

-- for ____, s in ipairs(
--     __TS__ObjectKeys(TestObject)
-- ) do
--     print( type(TestObject.FUN))
-- end
end