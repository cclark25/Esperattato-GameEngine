print("Lua called.")
createSquare(10,-10);
createSquare(50.5,-50.5);
createSquare(100.876,-100.145);

TestObject = {}
A = { B = {C =123} }

while true do
print(TestObject.A);
print(A.B.C);
-- print(TestObject.B);
-- print(TestObject.C);
-- print(TestObject.D);
TestObject = createTestObject();
end